#include "viewWidget.h"
#include <qevent>
#include <algorithm>
#include <iostream>

using namespace std;

viewWidget::viewWidget(CentralDataStruct& centralData, QWidget* parent) : QWidget(parent), centralData(centralData)
{
	installEventFilter(this);
	resize(parent->size());
}

viewWidget::~viewWidget()
{}

bool viewWidget::eventFilter(QObject* obj, QEvent* evt)
{
	if (!centralData.vectorSql.empty() && obj == this)
	{
		switch (evt->type())
		{
			case QEvent::Paint: { eventPaint(); return true; }
			default: { return false; }
		}
	}
	else { return false; }
}

void viewWidget::eventPaint()
{
	prepareVectorView();
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(ColorOrange), 2.0, Qt::SolidLine, Qt::RoundCap));
	painter.drawRect(rect());

	QPointF firstPointPoly, lastPointPoly, firstPointLine, lastPointLine;

	switch (viewType)
	{
		case ViewType::Horizontal:
		case ViewType::Column:
		{
			firstPointPoly = { border, height() / 2.0 };
			lastPointPoly = { width() - border, height() / 2.0 };
			firstPointLine = firstPointPoly - QPointF{ border / 2.0, 0.0 };
			lastPointLine = lastPointPoly + QPointF{ border / 2.0, 0.0 };
			break;
		}
		case ViewType::Vertical:
		{
			firstPointPoly = { width() / 2.0, border };
			lastPointPoly = { width() / 2.0, height() - border };
			firstPointLine = firstPointPoly - QPointF{ 0.0, border / 2.0 };
			lastPointLine = lastPointPoly + QPointF{ 0.0, border / 2.0 };
			break;
		}
		default: { break; }
	}

	QPolygonF poly;
	poly << firstPointPoly;

	for (size_t i = 0; i < vectorView.size(); ++i)
	{
		QPointF point;

		switch (viewType)
		{
			case ViewType::Horizontal:
			case ViewType::Column:
			{
				point = { ((double)i / vectorView.size()) * (width() - border * 2.0) + border,
					((-vectorView[i] + rangeValue / 2.0) / rangeValue) * (height() - border * 2.0) + border };
				break;
			}
			case ViewType::Vertical:
			{
				point = { ((vectorView[i] + rangeValue / 2.0) / rangeValue) * (width() - border * 2.0) + border,
					((double)i / vectorView.size()) * (height() - border * 2.0) + border };

				break;
			}

			default: { break; }
		}

		if (vectorView[i] == maxValue) { maxPoint = point; };
		if (vectorView[i] == maxValue) { minPoint = point; };
		poly << point;
	}

	poly << lastPointPoly;

	if (viewType == ViewType::Horizontal || viewType == ViewType::Vertical)
	{
		painter.setPen(QPen(QColor(ColorBlue), 1.0, Qt::SolidLine, Qt::RoundCap));
		painter.setBrush(QBrush(QColor(ColorBlueTransp), Qt::SolidPattern));
		painter.drawPolygon(poly);
	}
	else
	{
		std::vector<QPointF> columnPoints;
		poly.remove(vectorView.size() - 1);
		poly.remove(0);

		int displaySize = (width() - border * 2.0);
		auto koeff = static_cast<double>(displaySize) / static_cast<double>(poly.size());

		for (int i = 0; i < displaySize; i += smoothLevel + 2)
		{
			int it1 = static_cast<int>(i / koeff);
			int it2 = static_cast<int>((i + 1) / koeff) + 1;

			columnPoints.push_back({ i + border, std::accumulate(poly.begin() + it1, poly.begin() + it2, QPointF{ 0.0, 0.0 },
																 [&](QPointF acc, QPointF val)-> QPointF { return{ val.x(), (acc.y() + val.y()) }; }).y() / (it2 - it1) });
		}

		painter.setPen(Qt::NoPen);

		for (int i = 0; i < columnPoints.size(); ++i)
		{
			if (columnPoints[i].y() <= firstPointPoly.y())
			{
				painter.setBrush(QBrush(QColor(ColorGreenTransp), Qt::SolidPattern));
				painter.drawRect(QRectF{ columnPoints[i].x(), columnPoints[i].y(), static_cast<double>(smoothLevel) + 1, firstPointPoly.y() - columnPoints[i].y() });
			}
			else
			{
				painter.setBrush(QBrush(QColor(ColorRedTransp), Qt::SolidPattern));
				painter.drawRect(QRectF{ columnPoints[i].x(), firstPointPoly.y(), static_cast<double>(smoothLevel) + 1, columnPoints[i].y() - firstPointPoly.y() });
			}
		}
	}

	painter.setPen(QPen(QColor(ColorOrange), 1.0, Qt::SolidLine, Qt::RoundCap));
	painter.setBrush(QBrush(Qt::NoBrush));
	painter.drawLine(firstPointLine, lastPointLine);
}

void viewWidget::prepareVectorView()
{
	VectorDouble lod(filterSize);
	auto filerSizeHalf = (filterSize - 1) / 2;
	vectorView.clear();

	for (int i = 0; i < lod.size(); ++i)
	{
		auto r2 = (static_cast<double>(i - filerSizeHalf) * (i - filerSizeHalf));
		lod[i] = exp(-(r2 / (2.0 * sigma * sigma)));
	}

	VectorDouble mirror(centralData.vectorSql.size() + filerSizeHalf * 2);

	for (int i = 0; i < mirror.size(); ++i)
	{
		auto ptr = i - filerSizeHalf;
		if (ptr < 0) { ptr = abs(ptr) - 1; }
		if (ptr > centralData.vectorSql.size() - 1) { ptr = centralData.vectorSql.size() - (ptr - centralData.vectorSql.size()) - 1; }
		mirror[i] = centralData.vectorSql[ptr].value;
	}

	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		vectorView.push_back(std::inner_product(mirror.begin() + i, mirror.begin() + i + filterSize, lod.begin(), 0.0));
	}

	auto pair = std::minmax_element(vectorView.begin(), vectorView.end(), [](auto prev, auto next) { return prev < next; });
	minValue = (*pair.first);
	maxValue = (*pair.second);
	rangeValue = std::fabs(minValue) < std::fabs(maxValue) ? std::fabs(maxValue) * 2.0 : std::fabs(minValue) * 2.0;
}

void viewWidget::changeViewTypeSlot(ViewType type)
{
	viewType = type;
	update();
}

void viewWidget::smoothLevelSlot(int num)
{
	if (num == 0) { filterSize = 1; sigma = 1.0; }
	else { filterSize = num * 2 + 1; sigma = static_cast<double>(num) / 3.0 + 1.0; }

	smoothLevel = num + 1;
	update();
}
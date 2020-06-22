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
	auto pair = std::minmax_element(centralData.vectorSql.begin(), centralData.vectorSql.end(), [](auto prev, auto next) { return prev.value < next.value; });
	minValue = (*pair.first).value;
	maxValue = (*pair.second).value;
	rangeValue = std::fabs(minValue) < std::fabs(maxValue) ? std::fabs(maxValue) * 2.0 : std::fabs(minValue) * 2.0;

	QPainter painter(this);

	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(ColorOrange), 2.0, Qt::SolidLine, Qt::RoundCap));
	painter.drawRect(rect());

	QPointF firstPointPoly, lastPointPoly, firstPointLine, lastPointLine;

	switch (viewType)
	{
		case ViewType::Horizontal:
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
		case ViewType::Column: { break; }
		default: { break; }
	}

	QPolygonF poly;
	poly << firstPointPoly;

	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		switch (viewType)
		{
			case ViewType::Horizontal:
			{
				poly << QPointF{ ((double)i / centralData.vectorSql.size()) * (width() - border * 2.0) + border,
					((-centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue) * (height() - border * 2.0) + border };
				break;
			}
			case ViewType::Vertical:
			{
				poly << QPointF{ ((centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue) * (width() - border * 2.0) + border,
					((double)i / centralData.vectorSql.size()) * (height() - border * 2.0) + border };
				break;
			}
			case ViewType::Column: { break; }
			default: { break; }
		}
	}

	poly << lastPointPoly;

	painter.setPen(QPen(QColor(ColorBlue), 1.5, Qt::SolidLine, Qt::RoundCap));
	painter.setBrush(QBrush(QColor(ColorBlueTransp), Qt::SolidPattern));
	painter.drawPolygon(poly);

	painter.setPen(QPen(QColor(ColorOrange), 1.0, Qt::SolidLine, Qt::RoundCap));
	painter.setBrush(QBrush(Qt::NoBrush));
	painter.drawLine(firstPointLine, lastPointLine);
}

void viewWidget::changeViewTypeSlot(ViewType type)
{
	viewType = type;
	update();
}
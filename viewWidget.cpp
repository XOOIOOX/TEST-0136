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
			case QEvent::Paint:
			{
				eventPaint();
				return true;
			}
			default:
			{ return false; }
		}
	}
	else
	{
		return true;
	}
}

void viewWidget::eventPaint()
{
	auto pair = std::minmax_element(centralData.vectorSql.begin(), centralData.vectorSql.end(), [](auto prev, auto next) { return prev.value < next.value; });
	minValue = (*pair.first).value;
	maxValue = (*pair.second).value;

	rangeValue = std::fabs(minValue) < std::fabs(maxValue) ? std::fabs(maxValue) * 2.0 : std::fabs(minValue) * 2.0;
	cout << "min: " << minValue << endl;
	cout << "max: " << maxValue << endl;
	cout << "input range: " << rangeValue << endl;

	QPolygonF line;
	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		switch (viewType)
		{
			case ViewType::Horizontal:
			{
				line << QPointF{ (double)i / centralData.vectorSql.size() * (width() - border * 2.0) + border,
					(-centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue * (height() - border * 2.0) };
				break;
			}
			case ViewType::Vertical:
			{
				line << QPointF{ (centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue * (width() - border * 2.0),
					(double)i / centralData.vectorSql.size() * (height() - border * 2.0) + border };
				break;
			}
			case ViewType::Column: { break; }
			default: { break; }
		}
	}

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(ColorOrange), 1, Qt::SolidLine, Qt::RoundCap));

	switch (viewType)
	{
		case ViewType::Horizontal:
		{
			painter.drawLine(QPointF{ border, height() / 2.0 }, QPointF{ width() - border, height() / 2.0 });
			break;
		}
		case ViewType::Vertical:
		{
			painter.drawLine(QPointF{ width() / 2.0, border }, QPointF{ width() / 2.0, height() - border });
			break;
		}
		case ViewType::Column:
		{ break; }
		default:
		{ break; }
	}

	painter.setPen(QPen(QColor(ColorBlue), 1, Qt::SolidLine, Qt::RoundCap));
	painter.setBrush(QBrush(QColor(ColorBlueTransp), Qt::SolidPattern));
	painter.drawPolyline(line);
}

void viewWidget::changeViewTypeSlot(ViewType type)
{
	viewType = type;
	update();
}
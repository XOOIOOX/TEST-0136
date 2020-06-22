#include "viewWidget.h"
#include <qevent>
#include <algorithm>
#include <iostream>

using namespace std;

viewWidget::viewWidget(CentralDataStruct& centralData, QWidget* parent) : QWidget(parent), centralData(centralData)
{
	installEventFilter(this);
	resize(parent->size());
	//painter = new QPainter(this);
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
	//rangeValue = maxValue - minValue;

	switch (viewType)
	{
		case ViewType::Horizontal: { horizontalGraph(); break; }
		case ViewType::Vertical: { vetricalGraph(); break; }
		case ViewType::Column: { columnlGraph(); break; }
		default:
		{ break; }
	}
}

void viewWidget::horizontalGraph()
{
	QPolygonF line;
	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		line << QPointF{ (double)i / centralData.vectorSql.size() * size().width(), (-centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue * size().height() };
	}

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(43, 181, 255), 2, Qt::SolidLine, Qt::SquareCap));
	painter.setBrush(QBrush(QColor(43, 181, 255), Qt::SolidPattern));
	painter.drawPolyline(line);
}

void viewWidget::vetricalGraph()
{
	QPolygonF line;
	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		line << QPointF{  (centralData.vectorSql[i].value + rangeValue / 2.0) / rangeValue * size().width() , (double)i / centralData.vectorSql.size() * size().height() };
	}

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(43, 181, 255), 2, Qt::SolidLine, Qt::SquareCap));
	painter.setBrush(QBrush(QColor(43, 181, 255), Qt::SolidPattern));
	painter.drawPolyline(line);
}

void viewWidget::columnlGraph()
{}

void viewWidget::changeViewTypeSlot(ViewType type)
{
	viewType = type;
	update();
}
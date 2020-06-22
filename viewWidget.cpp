#include "viewWidget.h"
#include <qevent>
#include <QPainter>

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
	QPainter painter(this);
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


}

void viewWidget::vetricalGraph()
{}

void viewWidget::columnlGraph()
{}

void viewWidget::changeViewTypeSlot(ViewType type)
{}
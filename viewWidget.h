#pragma once
#include "Defs.h"
#include <QWidget>
#include <QPainter>

class viewWidget : public QWidget
{
	Q_OBJECT

public:
	viewWidget(CentralDataStruct& centralData, QWidget* parent = nullptr);
	~viewWidget();

	bool eventFilter(QObject* obj, QEvent* evt);
private:
	CentralDataStruct& centralData;
	ViewType viewType{ ViewType::Vertical };
	double minValue{ 0.0 };
	double maxValue{ 0.0 };
	double rangeValue{ 0.0 };
	double rangeWidget{ 0.0 };

	void eventPaint();
	void horizontalGraph();
	void vetricalGraph();
	void columnlGraph();

public slots:
	void changeViewTypeSlot(ViewType type);
};

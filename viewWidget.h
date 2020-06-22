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
	ViewType viewType{ ViewType::Horizontal };
	double minValue{ 0.0 };
	double maxValue{ 0.0 };
	double rangeValue{ 0.0 };
	double rangeWidget{ 0.0 };
	double border{ 10.0 };

	void eventPaint();

public slots:
	void changeViewTypeSlot(ViewType type);
};

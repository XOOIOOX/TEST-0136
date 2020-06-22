#pragma once
#include "Defs.h"
#include <QWidget>

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

	void eventPaint();
	void horizontalGraph();
	void vetricalGraph();
	void columnlGraph();

public slots:
	void changeViewTypeSlot(ViewType type);
};

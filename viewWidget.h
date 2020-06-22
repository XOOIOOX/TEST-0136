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
	CentralDataStruct& centralData;						// ссылка на общие данные
	ViewType viewType{ ViewType::Horizontal };			// тип отображения
	double minValue{ 0.0 };								// минимальное значение в выборке
	double maxValue{ 0.0 };								// максимальное значение в выборке
	double rangeValue{ 0.0 };							// диапазон значений выборки
	double border{ ViewBorder };						// бордюр

	void eventPaint();									// метод отрисовки

public slots:
	void changeViewTypeSlot(ViewType type);				// слот смены типа отображения
};

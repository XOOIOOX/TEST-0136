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
	QPointF maxPoint{ 0.0, 0.0 };						// максимальная точка на графике
	QPointF minPoint{ 0.0, 0.0 };						// минимальная точка на графике
	VectorDouble vectorView;							// вектор значений
	int filterSize{ 41 };								// размер фильтра (по умолчанию без сглаживания)
	double sigma{ 1.0 };								// сигма (ширина) фильтрации
	int smoothLevel{ 1 };								// уровень фильтрации

	void eventPaint();									// метод отрисовки
	void prepareVectorView();							// подготовка верктора значений

public slots:
	void changeViewTypeSlot(ViewType type);				// слот смены типа отображения
	void smoothLevelSlot(int num);						// слот установки размера фильтра
};

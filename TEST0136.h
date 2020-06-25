#pragma once

#include <QtWidgets/QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QTableView>
#include "ui_TEST0136.h"
#include "Defs.h"
#include "TableModel.h"
#include "viewWidget.h"

class TEST0136 : public QMainWindow
{
	Q_OBJECT

public:
	TEST0136(QWidget* parent = Q_NULLPTR);

private:
	Ui::TEST0136Class ui;
	QTableView* tableView;									// вьюшка таблицы
	CentralDataStruct centralData;							// общие данные
	TableModel* tableModel;									// модель таблицы
	QItemSelectionModel selectionModel;						// модель выбора
	QSqlDatabase dbase;										// текущая БД
	QStringList tablesList;									// список таблиц
	int selectedTable{ 0 };									// номер выбранной таблицы
	int selectedGroup{ BadIndex };							// номер выбранной группы
	ListInt groupsList;										// список всех групп
	viewWidget* view;										// вьюшка

	void selectedTableLoad();								// загрузка таблицы по номеру выбранной
	void readGroups();										// чтение групп
	void selectedGroupLoad();								// чтение выбранной группы
	void groupSelectSpinSetup();							// сетап спинбокса для выбора группы
	void tableSelectSpinSetup();							// сетап спинбокса для выбора таблицы
	//void smoothValues();									// сглаживание данных

public slots:
	void selectedTableChangeSlot(int num);					// слот смены выбранной таблицы
	void selectedGroupChangeSlot(int num);					// слот смены выбранной группы
	void setViewHorizontalSlot();							// слот для горизонтального отображения
	void setViewVerticalSlot();								// слот для вертикального отображения
	void setViewColumnlSlot();								// слот для отображения столбцами
	void currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous);
};

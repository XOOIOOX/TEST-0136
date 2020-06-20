#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TEST0136.h"
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>
#include "Defs.h"
#include "TableModel.h"
#include "QSqlQuery"

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
	QItemSelectionModel selectionModel;
	QSqlDatabase dbase;										// текущая БД
	QStringList tablesList;									// список таблиц
	int selectedTable{ 0 };									// номер выбранной таблицы
	int selectedGroup{ BadIndex };							// номер выбранной группы
	ListInt groupsList;

	void selectedTableLoad();								// загрузка таблицы по номеру выбранной
	void readGroups();
	void selectedGroupLoad();
	void groupSelectSpinSetup();
	void tableSelectSpinSetup();

public slots:
	void selectedTableChangeSlot(int num);					// слот смены выбранной таблицы
	void selectedGroupChangeSlot(int num);					// слот смены выбранной группы
	void currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous);
};

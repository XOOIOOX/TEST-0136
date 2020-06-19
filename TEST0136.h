#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TEST0136.h"
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>
#include "DataItemDelegate.h"

class TEST0136 : public QMainWindow
{
	Q_OBJECT

public:
	TEST0136(QWidget* parent = Q_NULLPTR);

private:
	Ui::TEST0136Class ui;
	QTableView* tableView;								// вьюшка таблицы
	QSqlTableModel* tableModel;							// sql-ная табличная модель
	QSqlDatabase dbase;									// текущая БД
	QStringList tablesNames;							// список таблиц
	int selectedTable{ 0 };								// номер выбранной таблицы
	DataItemDelegate* itemDelegate;

	void tableLoad();									// загрузка таблицы по номеру выбранной

public slots:
	void selectedChangeSlot(int num);					// слот смены выбранной таблицы
};

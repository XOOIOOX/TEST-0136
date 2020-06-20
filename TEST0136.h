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

	void groupSelectSpinSetup();

private:
	Ui::TEST0136Class ui;
	QTableView* tableView;								// вьюшка таблицы
	CentralDataStruct centralData;						// общие данные
	TableModel* currentTableModel;						// модель таблицы
	QItemSelectionModel selectionSql;
	QSqlDatabase dbase;									// текущая БД
	QStringList tablesNames;							// список таблиц
	int selectedTable{ 0 };								// номер выбранной таблицы
	int selectedGroup{ BadIndex };						// номер выбранной группы
	ListInt groupsList;
	QSqlQuery dbQuery{ dbase };

	void tableLoad();									// загрузка таблицы по номеру выбранной
	void readGroups();
	void readSelectedGroup();

public slots:
	void selectedTableChangeSlot(int num);					// слот смены выбранной таблицы
	void selectedGroupChangeSlot(int num);					// слот смены выбранной группы
	void currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous);
};

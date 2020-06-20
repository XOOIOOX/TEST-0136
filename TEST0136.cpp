#include "TEST0136.h"

#include "QSqlTableModel"
#include "QSqlQueryModel"
#include "QStandardItemModel"
#include <algorithm>

TEST0136::TEST0136(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	dbase = QSqlDatabase::addDatabase("QSQLITE");
	dbase.setDatabaseName("sample.db");
	if (!dbase.open()) { qDebug() << "Cannot read db!"; }
	if (QSqlDatabase::drivers().isEmpty()) { qDebug() << "no drivers"; }

	tablesNames = dbase.tables();
	ui.tableSelectSpin->setRange(0, tablesNames.size() - 1);

	connect(ui.tableSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedTableChangeSlot, Qt::DirectConnection);
	connect(&selectionSql, &QItemSelectionModel::currentRowChanged, this, &TEST0136::currentIndexChangedSlot);

	tableView = ui.tableView;
	currentTableModel = new TableModel{ nullptr, centralData };
	selectionSql.setModel(currentTableModel);
	tableView->setModel(currentTableModel);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

	readGroups();

	//tableLoad();
	tableView->show();
}

void TEST0136::tableLoad()
{
	//tableModel->setTable(tablesNames[selectedTable]);
	//tableModel->select();
	//tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
	//tableModel->setHeaderData(0, Qt::Horizontal, tr("Дата"));
	//tableModel->setHeaderData(1, Qt::Horizontal, tr("Значение"));
	//tableModel->setHeaderData(2, Qt::Horizontal, tr("Группа"));
}

void TEST0136::readGroups()
{
	auto number = QString("Number");
	QSqlQuery query("SELECT " + number + " " + "FROM " + tablesNames[selectedTable]);
	while (query.next()) { groupsList.push_back(query.value(query.record().indexOf(number)).toInt()); }
	groupsList.sort();
	groupsList.unique();
}

void TEST0136::selectedGroupChangeSlot(int num)
{
	//selectedGroup
}

void TEST0136::readSelectedGroup()
{
	if (selectedGroup != BadIndex)
	{
		centralData.vectorSql.clear();
		QSqlQuery query("SELECT  * FROM " + tablesNames[selectedTable] + "WHERE Number=" + QString(selectedGroup));

		while (query.next())
		{
			centralData.vectorSql.push_back(
				{
					query.value(query.record().indexOf(0)).toDateTime(),
					query.value(query.record().indexOf(0)).toDouble(),
					query.value(query.record().indexOf(0)).toInt()
				});
		}
	}
}

void TEST0136::selectedTableChangeSlot(int num)
{
	if (num < tablesNames.size() && num >= 0 && num != selectedTable)
	{
		selectedTable = num;
		tableLoad();
	}
}

void TEST0136::currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous)
{
	if (centralData.currentIndexSql != current.row())
	{
		centralData.currentIndexSql = current.row();
	}
}
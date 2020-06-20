#include "TEST0136.h"

#include "QSqlTableModel"
#include "QSqlQueryModel"
#include "QStandardItemModel"
#include <algorithm>
#include <xutility>

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

	tableLoad();
	tableView->show();
}

void TEST0136::tableLoad()
{
	readGroups();
	groupSelectSpinSetup();
	readSelectedGroup();
}

void TEST0136::groupSelectSpinSetup()
{
	if (!groupsList.empty())
	{
		ui.groupSelectSpin->setRange(0, groupsList.size() - 1);
		ui.groupSelectSpin->setDisabled(false);
		connect(ui.groupSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedGroupChangeSlot, Qt::DirectConnection);
	}
	else
	{
		ui.groupSelectSpin->setRange(0, 0);
		ui.groupSelectSpin->setDisabled(true);
		disconnect(ui.groupSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedGroupChangeSlot);
	}
}

void TEST0136::readGroups()
{
	auto number = QString("Number");
	QSqlQuery query("SELECT " + number + " " + "FROM " + tablesNames[selectedTable]);
	groupsList.clear();
	while (query.next()) { groupsList.push_back(query.value(query.record().indexOf(number)).toInt()); }
	groupsList.sort();
	groupsList.unique();
	if (!groupsList.empty()) { selectedGroup = groupsList.front(); }
	else { selectedGroup = BadIndex; }
}

void TEST0136::readSelectedGroup()
{
	if (selectedGroup != BadIndex)
	{
		centralData.vectorSql.clear();
		QSqlQuery query("SELECT * FROM " + tablesNames[selectedTable] + " WHERE Number=" + QString::number(*std::next(groupsList.begin(), selectedGroup)));

		while (query.next())
		{
			centralData.vectorSql.push_back(
				{
					QDateTime::fromSecsSinceEpoch(query.value(query.record().indexOf("Time")).toULongLong()),
					query.value(query.record().indexOf("Value")).toDouble(),
					query.value(query.record().indexOf("Number")).toInt()
				});
		}
	}
}

void TEST0136::selectedTableChangeSlot(int num)
{
	if (num != selectedTable)
	{
		selectedTable = num;
		tableLoad();
		tableView->update();
	}
}

void TEST0136::selectedGroupChangeSlot(int num)
{
	if (num != selectedGroup)
	{
		selectedGroup = num;
		readSelectedGroup();
		tableView->update();
	}
}

void TEST0136::currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous)
{
	if (centralData.currentIndexSql != current.row())
	{
		centralData.currentIndexSql = current.row();
	}
}
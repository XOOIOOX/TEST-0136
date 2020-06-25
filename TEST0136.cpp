#include "TEST0136.h"
#include <numeric>

TEST0136::TEST0136(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	dbase = QSqlDatabase::addDatabase("QSQLITE");
	dbase.setDatabaseName("./sample.db");
	if (!dbase.open()) { qDebug() << "Cannot read db!"; }
	if (QSqlDatabase::drivers().isEmpty()) { qDebug() << "No drivers !"; }

	view = new viewWidget(centralData, ui.view);
	tablesList = dbase.tables();
	tableSelectSpinSetup();
	connect(&selectionModel, &QItemSelectionModel::currentRowChanged, this, &TEST0136::currentIndexChangedSlot);
	tableView = ui.tableView;
	tableModel = new TableModel{ nullptr, centralData };
	selectionModel.setModel(tableModel);
	tableView->setModel(tableModel);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	tableView->verticalHeader()->hide();
	selectedTableLoad();
	tableView->show();
	connect(ui.horizontalButton, &QPushButton::clicked, this, &TEST0136::setViewHorizontalSlot);
	connect(ui.verticalButton, &QPushButton::clicked, this, &TEST0136::setViewVerticalSlot);
	connect(ui.columnButton, &QPushButton::clicked, this, &TEST0136::setViewColumnlSlot);
}

void TEST0136::selectedTableLoad()
{
	readGroups();
	groupSelectSpinSetup();
	selectedGroupLoad();
}

void TEST0136::groupSelectSpinSetup()
{
	if (!groupsList.empty())
	{
		disconnect(ui.groupSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedGroupChangeSlot);
		ui.groupSelectSpin->setRange(0, groupsList.size() - 1);
		ui.groupSelectSpin->setDisabled(false);
		ui.groupSelectSpin->setValue(selectedGroup);
		connect(ui.groupSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedGroupChangeSlot, Qt::DirectConnection);
	}
	else
	{
		disconnect(ui.groupSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedGroupChangeSlot);
		ui.groupSelectSpin->setRange(0, 0);
		ui.groupSelectSpin->setDisabled(true);
	}
}

void TEST0136::tableSelectSpinSetup()
{
	ui.tableSelectSpin->setRange(0, tablesList.size() - 1);
	connect(ui.tableSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedTableChangeSlot, Qt::DirectConnection);
}

void TEST0136::smoothValues()
{
	VectorDouble lod(filterSize);																			// НЧ-фильтр
	double sigma = 2.0;
	auto filerSizeHalf = (filterSize - 1) / 2;

	for (int i = 0; i < lod.size(); ++i)
	{
		auto r2 = (static_cast<double>(i - filerSizeHalf) * (i - filerSizeHalf));
		lod[i] = exp(-(r2 / (2.0 * sigma * sigma)));
	}

	VectorDouble mirror(centralData.vectorSql.size() + filerSizeHalf * 2);

	for (int i = 0; i < mirror.size(); ++i)
	{
		auto ptr = i - filerSizeHalf;
		if (ptr < 0) { ptr = abs(ptr) - 1; }
		if (ptr > centralData.vectorSql.size() - 1) { ptr = centralData.vectorSql.size() - (ptr - centralData.vectorSql.size()) - 1; }
		mirror[i] = centralData.vectorSql[ptr].value;
	}

	centralData.vectorView.clear();
	for (size_t i = 0; i < centralData.vectorSql.size(); ++i)
	{
		centralData.vectorView.push_back(std::inner_product(mirror.begin() + i, mirror.begin() + i + filterSize, lod.begin(), 0.0));
	}
}

void TEST0136::readGroups()
{
	auto number = QString("Number");
	QSqlQuery query("SELECT " + number + " FROM " + tablesList[selectedTable]);
	groupsList.clear();
	while (query.next()) { groupsList.push_back(query.value(query.record().indexOf(number)).toInt()); }
	groupsList.sort();
	groupsList.unique();
	if (!groupsList.empty()) { selectedGroup = groupsList.front(); }
	else { selectedGroup = BadIndex; }
}

void TEST0136::selectedGroupLoad()
{
	if (selectedGroup != BadIndex)
	{
		centralData.vectorSql.clear();
		QSqlQuery query("SELECT * FROM " + tablesList[selectedTable] + " WHERE Number=" + QString::number(*std::next(groupsList.begin(), selectedGroup)));
		while (query.next())
		{
			centralData.vectorSql.push_back(
				{
					QDateTime::fromSecsSinceEpoch(query.value(query.record().indexOf("Time")).toULongLong()),
					query.value(query.record().indexOf("Value")).toDouble(),
					query.value(query.record().indexOf("Number")).toInt()
				});
		}

		tableModel->sortByDate();
		tableView->scrollToTop();
		smoothValues();
		view->update();
	}
}

void TEST0136::selectedTableChangeSlot(int num)
{
	if (num != selectedTable)
	{
		selectedTable = num;
		selectedTableLoad();
	}
}

void TEST0136::selectedGroupChangeSlot(int num)
{
	if (num != selectedGroup)
	{
		selectedGroup = num;
		selectedGroupLoad();
	}
}

void TEST0136::currentIndexChangedSlot(const QModelIndex& current, const QModelIndex& previous)
{
	if (centralData.currentIndex != current.row()) { centralData.currentIndex = current.row(); }
}

void TEST0136::setViewHorizontalSlot() { view->changeViewTypeSlot(ViewType::Horizontal); }
void TEST0136::setViewVerticalSlot() { view->changeViewTypeSlot(ViewType::Vertical); }
void TEST0136::setViewColumnlSlot() { view->changeViewTypeSlot(ViewType::Column); }
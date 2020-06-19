#include "TEST0136.h"
#include "QSqlTableModel"
//#include "QSqlQueryModel"

TEST0136::TEST0136(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	dbase = QSqlDatabase::addDatabase("QSQLITE");
	dbase.setDatabaseName("sample.db");
	if (!dbase.open()) { qDebug() << "Cannot read db!"; }

	QSqlQuery query;

	if (QSqlDatabase::drivers().isEmpty())
	{
		qDebug() << "no drivers";
	}

	tablesNames = dbase.tables();
	ui.tableSelectSpin->setRange(0, tablesNames.size() - 1);
	connect(ui.tableSelectSpin, qOverload<int>(&QSpinBox::valueChanged), this, &TEST0136::selectedChangeSlot, Qt::DirectConnection);

	tableView = ui.tableView;
	tableModel = new QSqlTableModel(tableView);
	tableLoad();

	tableView->setModel(tableModel);
	tableView->show();
}

void TEST0136::tableLoad()
{
	tableModel->setTable(tablesNames[selectedTable]);
	tableModel->select();
	tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
	tableModel->setHeaderData(0, Qt::Horizontal, tr("Дата"));
	tableModel->setHeaderData(1, Qt::Horizontal, tr("Значение"));
	tableModel->setHeaderData(2, Qt::Horizontal, tr("Группа"));
}

void TEST0136::selectedChangeSlot(int num)
{
	if (num < tablesNames.size() && num >= 0 && num != selectedTable)
	{
		selectedTable = num;
		tableLoad();
	}
}
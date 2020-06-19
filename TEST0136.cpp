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

	if (!query.exec("SELECT * FROM 'data12'"))
	{
		qDebug() << "Cannot select";
	}

	table = ui.tableView;
	model = new QSqlTableModel(table);
	model->setTable("data12");
	model->select();
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setHeaderData(0, Qt::Horizontal, tr("Name"));
	model->setHeaderData(1, Qt::Horizontal, tr("Salary"));

	table->setModel(model);
	table->show();
}
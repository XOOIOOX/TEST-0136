#include "TableModel.h"
#include <iostream>

TableModel::TableModel(QObject* parent, CentralDataStruct& input) : QAbstractTableModel(parent), centralData(input)
{}

TableModel::~TableModel()
{}

int TableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.isValid()) { return 0; }
	return centralData.vectorSql.size();
}

int TableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.isValid()) { return 0; }
	return 3;
}

QVariant TableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid()) { return QVariant(); }
	if (index.row() < 0 || index.row() >= centralData.vectorSql.size()) { return QVariant(); }
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (index.column())
		{
			case 0:
			{
				return centralData.vectorSql[index.row()].date.toString("dd.MM.yyyy hh:mm:ss");
			}

			case 1:
			{
				return centralData.vectorSql[index.row()].value;
			}
			case 2:
			{
				return centralData.vectorSql[index.row()].group;
			}

			default:
			{ return QVariant(); }
		}
	}
	else { return QVariant(); }
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		switch (index.column())
		{
			case 0:
			{
				centralData.vectorSql[index.row()].date = value.value<QDateTime>();
				break;
			}

			case 1:
			{
				centralData.vectorSql[index.row()].value = value.value<double>();
				break;
			}
			case 2:
			{
				centralData.vectorSql[index.row()].group = value.value<int>();;
				break;
			}

			default:
			{ return false; }
		}

		emit dataChanged(index, index);
		return true;
	}

	return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole) { return QVariant(); }
	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
			case 0:
			{
				return QString("Дата");
			}

			case 1:
			{
				return QString("Значение");
			}
			case 2:
			{
				return QString("Группа");
			}

			default:
			{ return QVariant(); }
		}
	}
	else
	{
		return QString::number(section);
	}
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
	auto flags = QAbstractTableModel::flags(index);
	return index.isValid() ? flags | Qt::ItemIsEditable : flags;
}

bool TableModel::insertRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
	Q_UNUSED(row)
		Q_UNUSED(count)
		Q_UNUSED(parent)
		return false;
}

bool TableModel::removeRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
	Q_UNUSED(row)
		Q_UNUSED(count)
		Q_UNUSED(parent)
		return false;
}

void TableModel::update()
{
	beginResetModel();
	endResetModel();
}
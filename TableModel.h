#pragma once

#include "Defs.h"
#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	TableModel(QObject* parent, CentralDataStruct& input);
	~TableModel();

	CentralDataStruct& centralData;

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
	void update();
};

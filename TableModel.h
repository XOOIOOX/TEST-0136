#pragma once

#include "Defs.h"
#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	TableModel(QObject* parent, CentralDataStruct& input);
	~TableModel();

	CentralDataStruct& centralData;																			// ссылка на центральные данные

	int rowCount(const QModelIndex& parent = QModelIndex()) const;											// количество строк в модели
	int columnCount(const QModelIndex& parent = QModelIndex()) const;										// количество колонок в модели	
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;								// данные модели
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);					// запись данных в модель (в данные момент ничего не редактируем, не используется)
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;		// данные зазаголовка
	Qt::ItemFlags flags(const QModelIndex& index) const;													// флаги модели
	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());							// вставка строк (не реализовано)
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());							// удаление строк (не реализовано)
	void update();																							// апдейт модели для случая изменений извне
	void sortByDate();																						// сортировка по дате (первый столбец)
};

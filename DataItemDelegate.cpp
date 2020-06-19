#include "DataItemDelegate.h"
#include "QDateTime"
#include "QPainter"
#include "QString"

void DataItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	QStyledItemDelegate::paint(painter, option, QModelIndex());
	index.data().toInt();
	painter->drawText(QPoint{ 0, 0 }, QString{ "Тест" });
}

QSize DataItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QWidget* DataItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	return parent;
}

void DataItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index)
{}

void DataItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index)
{}

void DataItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
}
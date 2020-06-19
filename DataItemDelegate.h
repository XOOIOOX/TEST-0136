#pragma once

#include <QStyledItemDelegate>
#include "QStyleOption"

class DataItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:

	using QStyledItemDelegate::QStyledItemDelegate;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index);
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index);
	void setEditorData(QWidget* editor, const QModelIndex& index);
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index);
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

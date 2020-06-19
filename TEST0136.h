#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TEST0136.h"
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>

class TEST0136 : public QMainWindow
{
    Q_OBJECT

public:
    TEST0136(QWidget *parent = Q_NULLPTR);

private:
    Ui::TEST0136Class ui;
    QTableView* table;
    QSqlTableModel* model;
    QSqlDatabase dbase;
};

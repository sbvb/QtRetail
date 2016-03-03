#ifndef STOCK_H
#define STOCK_H
#include <QtSql>
#include <QDebug>
#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Stock;
}

class Stock : public QDialog
{
    Q_OBJECT

public:
    MainWindow conn;
    explicit Stock(QWidget *parent = 0);
    ~Stock();

private slots:
    void on_addstockButton_clicked();

    void on_editstockButton_clicked();

    void on_removeButton_2_clicked();

    void on_stockButton_clicked();

private:
    Ui::Stock *ui;
};

#endif // STOCK_H

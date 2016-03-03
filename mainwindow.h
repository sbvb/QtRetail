#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql>
#include <QDebug>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase db;
    void connClose()
    {
        db.close();
        db.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/Rodrigo/Desktop/QTRetail-master/database/QtRetail.db");

        if (!db.open()){
            qDebug () << "failed";
            return false;
        }

        else {
            qDebug () <<"connected";
            return true;
        }
    }

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SearchButton_clicked();

    void on_stockButton_clicked();

    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_cancelButton_clicked();

    void on_finshButton_clicked();

    void on_valueView_activated(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    void getProduct();
};

#endif // MAINWINDOW_H

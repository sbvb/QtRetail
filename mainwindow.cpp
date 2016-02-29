#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stock.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SearchButton_clicked()
{
    QString product;
        product = ui->searchWidget->text();

        if (!connOpen())
        {
            qDebug () << "Failed to open Database.";
        }

        bool connOpen();

        QSqlQuery qry;

        qry.prepare("select * from Stock where name = '"+product+"'");

        if (qry.exec())
        {
                int count = 0;
                while (qry.next())
                    {
                        count++;
                    }
                if (count == 1)
                {
                    qDebug () << " Product found.";

                    QSqlQuery qry2;
                    qry2.prepare("insert into Search select id,name,price,quantity from Stock where name = '"+product+"'");
                    qry2.exec();

                    QSqlQueryModel * modal = new QSqlQueryModel ();
                    QSqlQuery * qry3 = new QSqlQuery(db);
                    qry3->prepare("select * from Search");

                    qry3->exec();
                    modal->setQuery(*qry3);
                    ui->productTable->setModel(modal);



                }

                else
                    QMessageBox::critical(this,tr ("Error!"),tr ("Product not found."));

         }
            connClose();

}
void MainWindow::getProduct(){

}


void MainWindow::on_stockButton_clicked()
{
    Stock stock;
    stock.setModal(true);
    stock.exec();
}

void MainWindow::on_addButton_clicked()
{
    connOpen();

        QString product;
        product = ui->searchWidget->text();

        QSqlQuery qry4;
        qry4.prepare("select * from Stock where name = '"+product+"'");

        if(qry4.exec())
        {
            int count = 0;
            while (qry4.next())
                {
                    count++;
                }
            if (count == 1)
            {

                QSqlQuery qry5;
                qry5.prepare("insert into Cart (id,name,price) select id,name,price from Stock where name = '"+product+"'");
                qry5.exec();


                QSqlQuery qry6;
                qry6.prepare("update Cart set quantity = quantity +1 where name = '"+product+"'");
                qry6.exec();


                QSqlQueryModel * modal = new QSqlQueryModel ();
                QSqlQuery * qry7 = new QSqlQuery(db);
                qry7->prepare("select * from Cart");

                qry7->exec();
                modal->setQuery(*qry7);
                ui->orderTable->setModel(modal);

                QSqlQuery qry8;
                qry8.prepare("Delete from Search");
                qry8.exec();

            }

        }

        connClose();
}

void MainWindow::on_removeButton_clicked()
{
    connOpen();

        QString product;
        product = ui->searchWidget->text();

        QSqlQuery qry9;
        qry9.prepare("select * from Stock where name = '"+product+"'");

        if(qry9.exec())
        {

            int count = 0;
            while (qry9.next())
                {
                    count++;
                }
            if (count == 1)
            {
                QSqlQuery qry10;
                qry10.prepare("update Cart set quantity = quantity -1 where name = '"+product+"'");
                qry10.exec();


                QSqlQuery qry102;
                qry102.prepare("delete from Cart where quantity == 0");
                qry102.exec();



                QSqlQueryModel * modal = new QSqlQueryModel ();
                QSqlQuery * qry11 = new QSqlQuery(db);
                qry11->prepare("select * from Cart");

                qry11->exec();
                modal->setQuery(*qry11);
                ui->orderTable->setModel(modal);

                QSqlQuery qry12;
                qry12.prepare("Delete from Search");
                qry12.exec();

            }

        }

        connClose();
}

void MainWindow::on_cancelButton_clicked()
{
    connOpen();

    QSqlQuery qry13;
    qry13.prepare("Delete from Cart");
    qry13.exec();

    QSqlQueryModel * modal = new QSqlQueryModel ();
    QSqlQuery * qry14 = new QSqlQuery(db);
    qry14->prepare("select * from Cart");

    qry14->exec();
    modal->setQuery(*qry14);
    ui->orderTable->setModel(modal);

    connClose();
}

void MainWindow::on_finshButton_clicked()
{
    connOpen();

    QSqlQuery qry15;
    qry15.prepare("insert into Sold (id, name, price) select id, name, price from Cart");
    if(qry15.exec())
    { qDebug () << "done1";}


   QSqlQuery qry16;
    qry16.prepare("update Sold set quantity = (select quantity from Cart where id = (select id from Sold)) + (select quantity from Sold where id = (select id from Cart)) where id = (select id from Cart)");
    if(qry16.exec())
    { qDebug () << "done2";}

    /* QSqlQuery qry17;
    qry17.prepare("update Stock set quantity = (select quantity from Stock where id = (select id from Sold)) - (select quantity from Sold where id = (select id from Stock)) where id = (select id from Cart)");
    if(qry17.exec())
    { qDebug () << "done3";}*/

    QSqlQuery qry18;
    qry18.prepare("Delete from Cart");
    qry18.exec();

    QSqlQueryModel * modal = new QSqlQueryModel ();
    QSqlQuery * qry19 = new QSqlQuery(db);
    qry19->prepare("select * from Cart");

    qry19->exec();
    modal->setQuery(*qry19);
    ui->orderTable->setModel(modal);

    QSqlQuery qry20;
    qry20.prepare("delete from Stock where quantity == 0");
    qry20.exec();

    connClose();
}

#include "stock.h"
#include "ui_stock.h"
#include <QMessageBox>

Stock::Stock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stock)
{
    ui->setupUi(this);
}

Stock::~Stock()
{
    delete ui;
}

void Stock::on_addstockButton_clicked()
{
    MainWindow conn;
        QString name, id, price, quantity;
        name = ui->stockName->text();
        id = ui->stockId->text();
        price = ui->stockPrice->text();
        quantity = ui->stockQuantity->text();


        if (!conn.connOpen())
        {
            qDebug () << "Failed to open Database.";
            return;
         }

        bool connOpen();

        QSqlQuery qry;

        qry.prepare("insert into Stock (id,name,price,quantity) values ('"+id+"','"+name+"','"+price+"','"+quantity+"')");

        if(qry.exec())
        {
            QMessageBox::critical(this,tr ("Done!"),tr ("Product added to database."));
            conn.connClose();
        }
        else
            QMessageBox::critical(this,tr("error::"), qry.lastError().text());

}



void Stock::on_editstockButton_clicked()
{
    MainWindow conn;
           QString name, id, price, quantity;
           name = ui->stockName->text();
           id = ui->stockId->text();
           price = ui->stockPrice->text();
           quantity = ui->stockQuantity->text();

           if (!conn.connOpen())
           {
               qDebug () << "Failed to open Database.";
               return;
            }

           bool connOpen();

           QSqlQuery qry;

           qry.prepare("update Stock set id = '"+id+"', name = '"+name+"', price = '"+price+"', quantity = '"+quantity+"' where id = '"+id+"'");

           if(qry.exec())
           {
               QMessageBox::critical(this,tr ("Done!"),tr ("Product updated to database."));
               conn.connClose();
           }
           else
               QMessageBox::critical(this,tr("error::"), qry.lastError().text());

}



void Stock::on_removeButton_2_clicked()
{
    MainWindow conn;
           QString id;
           id = ui->idstock->text();

           if (!conn.connOpen())
           {
               qDebug () << "Failed to open Database.";
               return;
            }

           bool connOpen();

           QSqlQuery qry;

           qry.prepare("Delete from Stock where id = '"+id+"'");

           if(qry.exec())
           {
               QMessageBox::critical(this,tr ("Done!"),tr ("Product deleted from database."));
               conn.connClose();
           }
           else
               QMessageBox::critical(this,tr("error::"), qry.lastError().text());

}


void Stock::on_stockButton_clicked()
{
    MainWindow conn;
        QSqlQueryModel * modal = new QSqlQueryModel ();

        conn.connOpen();
        QSqlQuery * qry = new QSqlQuery(conn.db);
        qry->prepare("select * from Stock");

        qry->exec();
        modal->setQuery(*qry);
        ui->tableView->setModel(modal);
        conn.connClose();
}

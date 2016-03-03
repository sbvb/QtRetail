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
    //it adds a new  product from tb_product
    MainWindow conn;
        QString name, id, price, quantity;
        name = ui->stockName->text();
        id = ui->stockId->text();
        price = ui->stockPrice->text();


        if (!conn.connOpen())
        {
            qDebug () << "Failed to open Database.";
            return;
         }

        bool connOpen();//open the connection with database

        QSqlQuery qry;

        qry.prepare("insert into tb_product (id,name,price) values ('"+id+"','"+name+"','"+price+"')");

        if(qry.exec())
        {
            QMessageBox::critical(this,tr ("Done!"),tr ("Product added to database."));
            conn.connClose();//close the connection with database

        }
        else
            QMessageBox::critical(this,tr("error::"), qry.lastError().text());

}



void Stock::on_editstockButton_clicked()
{
    //it updates the select product from tb_product
    MainWindow conn;
           QString name, id, price, quantity;
           name = ui->stockName->text();
           id = ui->stockId->text();
           price = ui->stockPrice->text();

           if (!conn.connOpen())
           {
               qDebug () << "Failed to open Database.";
               return;
            }

           bool connOpen();//open the connection with database

           QSqlQuery qry;

           qry.prepare("update tb_product set id = '"+id+"', name = '"+name+"', price = '"+price+"' where id = '"+id+"'");

           if(qry.exec())
           {
               QMessageBox::critical(this,tr ("Done!"),tr ("Product updated to database."));
               conn.connClose();//close the connection with database

           }
           else
               QMessageBox::critical(this,tr("error::"), qry.lastError().text());


}



void Stock::on_removeButton_2_clicked()
{
    //it removes the select product from tb_product
    MainWindow conn;
           QString id;
           id = ui->idstock->text();

           if (!conn.connOpen())
           {
               qDebug () << "Failed to open Database.";
               return;
            }

           bool connOpen();//open the connection with database

           QSqlQuery qry;

           qry.prepare("Delete from tb_product where id = '"+id+"'");

           if(qry.exec())
           {
               QMessageBox::critical(this,tr ("Done!"),tr ("Product deleted from database."));
               conn.connClose();//close the connection with database

           }
           else
               QMessageBox::critical(this,tr("error::"), qry.lastError().text());


}


void Stock::on_stockButton_clicked()

//it loads all the products from tb_product

{
    MainWindow conn;
        QSqlQueryModel * modal = new QSqlQueryModel ();

        conn.connOpen(); //open the connection with database

        QSqlQuery * qry = new QSqlQuery(conn.db);
        qry->prepare("select * from tb_product");

        qry->exec();
        modal->setQuery(*qry);
        ui->tableView->setModel(modal);

        conn.connClose();//close the connection with database

}

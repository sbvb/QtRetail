#include "sqlite3.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stock.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>



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

void MainWindow::on_create_db_button_clicked()
{
   qDebug() << "on_create_db_button_clicked";

   sqlite3 *db;
   char *zErrMsg = 0;
   const char *dbName = DATABASE;

   int rc = sqlite3_open(dbName, &db);

   if (rc) {
       qDebug() << "Can't open database: " << sqlite3_errmsg(db) << endl;
       sqlite3_close(db);
       return;
   }

   const char *commands[] = {
       "create table tb_product(id integer primary key autoincrement, name text, price real)"
       , "create table tb_cart(id integer primary key autoincrement, creation datetime)"
       , "create table tb_product_cart(id integer primary key autoincrement, product_fk integer, cart_fk integer, quant integer)"
   };
   int size = sizeof (commands) / sizeof (const char *);

   for (int i = 0; i < size; i++) {
//        rc = sqlite3_exec(db, commands[i], myCallback, 0, &zErrMsg);
       rc = sqlite3_exec(db, commands[i], 0, 0, &zErrMsg);
       if (rc != SQLITE_OK) {
           qDebug() << "SQL error: " << zErrMsg << endl;
           sqlite3_free(zErrMsg);
       }
   }

   sqlite3_close(db);

}

// static
void processDB(sqlite3 *db, const char **commands, int size) {
    qDebug() << "== processDB()" << endl;
    char *zErrMsg = 0;
        for (int i = 0; i < size; i++) {
        int rc = sqlite3_exec(db, commands[i], 0, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            qDebug() << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
        }
    }
    sqlite3_close(db);
}

// static
int myCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i] ? argv[i] : "NULL";
        qDebug() << azColName[i] << " = " << arg << " ; ";
    }
    qDebug() << endl;
    return 0;
}


void MainWindow::on_insert_product_clicked()
{
    qDebug() << "on_insert_product_clicked";
    sqlite3 *db;
    const char *dbName = DATABASE;
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        qDebug() << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    const char *commands[] = {
        "insert into tb_product (name, price) values (\"product name\",1.25)"
    };
    int size = sizeof (commands) / sizeof (const char *);
    processDB(db, commands, size);

}

void MainWindow::on_create_cart_clicked()
{
    qDebug () << "on_create_cart_clicked";
    qDebug() << "on_insert_product_clicked";
    sqlite3 *db;
    const char *dbName = DATABASE;
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        qDebug() << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    const char *commands[] = {
        "insert into tb_cart (creation) values (datetime('now','localtime'))"
    };
    int size = sizeof (commands) / sizeof (const char *);
    processDB(db, commands, size);

}

void MainWindow::on_product_to_cart_clicked()
{
    qDebug () << "on_product_to_cart_clicked";
    sqlite3 *db;
    const char *dbName = DATABASE;
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        qDebug() << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    const char *commands[] = {
        "insert into tb_product_cart (product_fk, cart_fk, quant) values (1,5,3)"
    };
    int size = sizeof (commands) / sizeof (const char *);
    processDB(db, commands, size);

}

void MainWindow::on_list_all_clicked()
{
    qDebug () << "on_list_all_clicked";

    sqlite3 *db;
    const char *dbName = DATABASE;
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        qDebug () << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    const char *commands[] = {
        "select * from tb_product"
        , "select * from tb_cart"
        , "select * from tb_product_cart"
    };
    int size = sizeof (commands) / sizeof (const char *);

    char *zErrMsg = 0;
    for (int i = 0; i < size; i++) {
        int rc = sqlite3_exec(db, commands[i], myCallback, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            qDebug () << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
        }
    }

    sqlite3_close(db);
}



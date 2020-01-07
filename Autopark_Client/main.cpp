#include "loginwindow.h"
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

//  QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
//  db.setDatabaseName("DRIVER={Oracle in OraClient12Home1};DSN=autopark_datasource;"
//                     "DBQ=autopark;UID=c##nikolastojkoski;PWD=nikolastojkoski");
//  if(!db.open())
//  {
//    qDebug() << db.lastError().text();
//    return 0;
//  }
//  QSqlQuery query(db);
//  query.exec("select name from routes");
//  while(query.next())
//  {
//    qDebug() << query.value(0).toString();
//  }


  //MainWindow w;
  //w.show();

  LoginWindow w;
  w.show();

  return a.exec();
  //return 0;
}

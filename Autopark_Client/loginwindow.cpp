#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::LoginWindow)
{
  ui->setupUi(this);
  db = QSqlDatabase::addDatabase("QODBC");
}

LoginWindow::~LoginWindow()
{
  delete ui;
}

void LoginWindow::on_loginButton_clicked()
{

  db.setDatabaseName("DRIVER={Oracle in OraClient12Home1};DSN=autopark_datasource;"
                     "DBQ=autopark;UID=c##nikolastojkoski;PWD=nikolastojkoski");
  if(!db.open())
  {
    qDebug() << db.lastError().text();
    return;
  }

  QString username = findChild<QLineEdit*>("usernameEdit")->text();
  QString password = findChild<QLineEdit*>("passwordEdit")->text();

  QSqlQuery query(db);
  query.exec(queries::selectHash.arg(password));
  query.next();
  QString hashedPassword = query.value(0).toString();

  query.exec(queries::selectUser.arg(username).arg(hashedPassword));

  if(!query.first())
  {
    QMessageBox messageBox;
    messageBox.setWindowTitle("Error");
    messageBox.setText("<p align='center'>Invalid username or password!</p>");
    messageBox.setStyleSheet("QLabel{min-width: 200px;}");
    messageBox.exec();
    return;
  }

  clientWindow = new ClientWindow(db);
  clientWindow->show();

  hide();

}

void LoginWindow::execQuery(const QString &query)
{
  qDebug() << query;
  QSqlQuery query_(db);
  if(!query_.exec(query))
  {
    qDebug() << query_.lastError().text();
    QMessageBox::critical(this, tr("Error!"), query_.lastError().text().split("\n")[0]);
  }
}






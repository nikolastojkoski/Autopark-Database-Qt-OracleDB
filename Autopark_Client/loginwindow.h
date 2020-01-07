#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtSql/QSqlDatabase>
#include "queries.h"
#include "clientwindow.h"

namespace Ui {
  class LoginWindow;
}

class LoginWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow();

private slots:
  void on_loginButton_clicked();

private:
  Ui::LoginWindow *ui;

  QSqlDatabase db;

  ClientWindow* clientWindow = nullptr;

  void execQuery(const QString &query);


};

#endif // LOGINWINDOW_H

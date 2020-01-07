#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QTableView>
#include <QComboBox>

namespace Ui {
  class ClientWindow;
}

class ClientWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit ClientWindow(QSqlDatabase &database, QWidget *parent = nullptr);
  ~ClientWindow();

private slots:
  void on_tabWidget_currentChanged(int index);
  void on_sortJournalComboBox_currentIndexChanged(int index);
  void on_insertJournalButton_clicked();
  void on_deleteJournalButton_clicked();
  void on_editJournalButton_clicked();
  void on_returnedJournalButton_clicked();
  void on_deletePersonnelButton_clicked();
  void on_editPersonnelButton_clicked();
  void on_insertPersonnelButton_clicked();
  void on_sortPersonnelComboBox_currentIndexChanged(int index);
  void on_deleteAutoButton_clicked();
  void on_editAutoButton_clicked();
  void on_insertAutoButton_clicked();
  void on_sortAutoComboBox_currentIndexChanged(int index);

  void on_insertRouteButton_clicked();

  void on_deleteRouteButton_clicked();

  void on_editRouteButton_clicked();

  void on_sortRoutesComboBox_currentIndexChanged(int index);

  void on_additionalComboBox_currentIndexChanged(int index);

  void on_additionalRouteComboBox_currentIndexChanged(int index);

private:
  Ui::ClientWindow *ui;
  QSqlDatabase db;

  std::map<QString, QSqlQueryModel*> models;
  std::map<QString, QTableView*> tableViews;
  std::map<QString, QComboBox*> sortComboBoxes;

  void setupTable(const QString &tableViewName, const QString &query,
                  const QString &accessibleName, int columnToHide = -1);
  void setupSortComboBox(const QString &comboBoxName, const QString &tableAccessibleName, int numTableColumns);
  void setupAdditionalComboBox();
  void setTableSortMode(const QString &tableAccessibleName, int column);

  void updateTable(const QString &tableAccessibleName, int columnToHide = -1);
  void updateModel(QSqlQueryModel *model);
  void updateTableView(QTableView* tableView, int columnToHide = -1);

  void execQuery(const QString &query);
  bool isAnyRowSelected(const QString &tableAccesibleName);
  int getSelectedRow(const QString &tableAccessibleName);
  QString getCellValue(const QString &tableAccessibleName, int row, int col);
  QString getHeaderValue(const QString &tableAccessibleName, int column);

  QString getAutoID(const QString &autoNum);
  QString getRouteID(const QString &routeName);
  QString getPersonnelID(const QString &lastName, const QString &name, const QString &fatherName);
  QString getCurrentDate();
  QString getTimeOut(const QString &id);
  QString getTimeIn(const QString &id);
  QStringList* getAutoNumList();
  QStringList* getRouteNameList();
  QStringList* getPersonnelList();

};

#endif // CLIENTWINDOW_H

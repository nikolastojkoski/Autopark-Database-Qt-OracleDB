#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include "queries.h"
#include "editdialog.h"

ClientWindow::ClientWindow(QSqlDatabase &database, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientWindow)
{
  ui->setupUi(this);
  db = database;

  QTabWidget* tabWidget = findChild<QTabWidget*>("tabWidget");
  tabWidget->setTabText(0, "Журнал");
  tabWidget->setTabText(1, "Персонал");
  tabWidget->setTabText(2, "Автомобили");
  tabWidget->setTabText(3, "Маршруты");
  tabWidget->setTabText(4, "Дополнительно");
  tabWidget->setCurrentIndex(0);

  setupTable("journalTableView", queries::selectJournal, "journal", 0);
  setupTable("personnelTableView", queries::selectPersonnel, "personnel", 0);
  setupTable("autoTableView", queries::selectAuto, "auto", 0);
  setupTable("routesTableView", queries::selectRoutes, "routes", 0);
  setupSortComboBox("sortJournalComboBox", "journal", 5);
  setupSortComboBox("sortPersonnelComboBox", "personnel", 4);
  setupSortComboBox("sortAutoComboBox", "auto", 5);
  setupSortComboBox("sortRoutesComboBox", "routes", 2);
  setupAdditionalComboBox();

}

ClientWindow::~ClientWindow()
{
  delete ui;
}

void ClientWindow::setupTable(const QString &tableViewName, const QString &query,
                              const QString &accessibleName, int columnToHide)
{
  QSqlQueryModel *model = new QSqlQueryModel;
  model->setQuery(query, db);
  models[accessibleName] = model;

  QTableView* tableView = findChild<QTableView*>(tableViewName);
  tableView->setModel(model);
  tableView->update();
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  if(columnToHide != -1)
    tableView->setColumnHidden(columnToHide, true);

  tableView->resizeColumnsToContents();
  tableViews[accessibleName] = tableView;
}
void ClientWindow::setupSortComboBox(const QString &comboBoxName, const QString &tableAccessibleName, int numTableColumns)
{
  QComboBox* sortComboBox = findChild<QComboBox*>(comboBoxName);
  sortComboBoxes[tableAccessibleName] = sortComboBox;
  sortComboBox->addItem(" - ");
  for(int i = 0; i < numTableColumns; i++)
    sortComboBox->addItem(getHeaderValue(tableAccessibleName, i));
}
void ClientWindow::setupAdditionalComboBox()
{
  QComboBox* routeComboBox = findChild<QComboBox*>("additionalRouteComboBox");
  routeComboBox->addItems(*getRouteNameList());
  routeComboBox->setCurrentIndex(0);

  QComboBox* queryComboBox = findChild<QComboBox*>("additionalComboBox");
  queryComboBox->addItem("Количество автомобилей в рейсе");
  queryComboBox->addItem("Самое короткое время проезда");

}

void ClientWindow::setTableSortMode(const QString &tableAccessibleName, int column)
{
  if(column == 0)
    return;

  QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
  sortModel->setDynamicSortFilter(true);
  sortModel->setSourceModel(models[tableAccessibleName]);
  tableViews[tableAccessibleName]->setModel(sortModel);
  tableViews[tableAccessibleName]->sortByColumn(column-1, Qt::AscendingOrder);
}

void ClientWindow::on_insertJournalButton_clicked()
{
  EditDialog *w = new EditDialog(this);
  w->addLineEditItem("Время отбытия:", getCurrentDate());
  w->addComboBoxItem("Автомобиль:", getAutoNumList());
  w->addComboBoxItem("Маршрут:", getRouteNameList());
  w->exec();
  if(w->accepted())
  {
    QString time_out = w->getValue(0);
    QString auto_id = getAutoID(w->getValue(1));
    QString route_id = getRouteID(w->getValue(2));
    execQuery(queries::insertJournal.arg(time_out).arg(auto_id).arg(route_id));
    updateTable("journal", 0);
  }
}

void ClientWindow::on_deleteJournalButton_clicked()
{
  if(!isAnyRowSelected("journal"))
    return;

  QString id = getCellValue("journal", getSelectedRow("journal"), 0);
  execQuery(queries::deleteJournal.arg(id));
  updateTable("journal", 0);
}

void ClientWindow::on_editJournalButton_clicked()
{
  if(!isAnyRowSelected("journal"))
    return;

  EditDialog *w = new EditDialog(this);
  w->addLineEditItem("Время отбытия:");
  w->addLineEditItem("Время прибытия:");
  w->addComboBoxItem("Автомобиль:", getAutoNumList());
  w->addComboBoxItem("Маршрут:", getRouteNameList());

  QString id = getCellValue("journal", getSelectedRow("journal"), 0);
  for(int i = 0; i < 4; i++)
    w->setValue(i, getCellValue("journal", getSelectedRow("journal"), i + 1));

  w->exec();
  if(w->accepted())
  {
    QString time_out = w->getValue(0);
    QString time_in = w->getValue(1);
    QString auto_id = getAutoID(w->getValue(2));
    QString route_id = getRouteID(w->getValue(3));
    execQuery(queries::updateJournal.arg(time_out).arg(time_in).arg(auto_id).arg(route_id).arg(id));
    updateTable("journal", 0);
  }
  delete w;
}

void ClientWindow::on_returnedJournalButton_clicked()
{
  if(!isAnyRowSelected("journal"))
    return;

  QString time_in = getCurrentDate();
  QString id = getCellValue("journal", getSelectedRow("journal"), 0);
  execQuery(queries::updateTimeInJournal.arg(time_in).arg(id));
  updateTable("journal", 0);
}

void ClientWindow::on_deletePersonnelButton_clicked()
{
  if(!isAnyRowSelected("personnel"))
    return;
  QString id = getCellValue("personnel", getSelectedRow("personnel"), 0);
  execQuery(queries::deletePersonnel.arg(id));
  updateTable("personnel", 0);
}

void ClientWindow::on_editPersonnelButton_clicked()
{
  if(!isAnyRowSelected("personnel"))
    return;

  EditDialog *w = new EditDialog(this);

  for(int i = 0; i < 3; i++)
    w->addLineEditItem(getHeaderValue("personnel", i+1),
                       getCellValue("personnel", getSelectedRow("personnel"), i+1));
  w->exec();
  if(w->accepted())
  {
    QString id = getCellValue("personnel", getSelectedRow("personnel"), 0);
    QString last_name = w->getValue(0);
    QString first_name = w->getValue(1);
    QString father_name = w->getValue(2);
    execQuery(queries::updatePersonnel.arg(last_name).arg(first_name).arg(father_name).arg(id));
    updateTable("personnel", 0);
  }
  delete w;
}

void ClientWindow::on_insertPersonnelButton_clicked()
{
  EditDialog *w = new EditDialog(this);
  for(int i = 0; i < 3; i++)
    w->addLineEditItem(getHeaderValue("personnel", i+1));
  w->exec();
  if(w->accepted())
  {
    QString last_name = w->getValue(0);
    QString first_name = w->getValue(1);
    QString father_name = w->getValue(2);
    execQuery(queries::insertPersonnel.arg(last_name).arg(first_name).arg(father_name));
    updateTable("personnel", 0);
  }
  delete w;
}
void ClientWindow::on_deleteAutoButton_clicked()
{
  if(!isAnyRowSelected("auto"))
    return;

  QString id = getCellValue("auto", getSelectedRow("auto"), 0);
  execQuery(queries::deleteAuto.arg(id));
  updateTable("auto", 0);
}

void ClientWindow::on_editAutoButton_clicked()
{
  if(!isAnyRowSelected("auto"))
    return;
  EditDialog *w = new EditDialog(this);
  for(int i = 0; i < 3; i++)
    w->addLineEditItem(getHeaderValue("auto", i+1),
                       getCellValue("auto", getSelectedRow("auto"), i+1));
  w->addComboBoxItem(getHeaderValue("auto", 4), getPersonnelList());
  w->setValue(3, getCellValue("auto", getSelectedRow("auto"), 4));
  w->exec();
  if(w->accepted())
  {
    QString num = w->getValue(0);
    QString color = w->getValue(1);
    QString mark = w->getValue(2);
    QStringList FIO = w->getValue(3).split(" ");
    QString personnel_id = getPersonnelID(FIO.value(0), FIO.value(1), FIO.value(2));
    QString id = getCellValue("auto", getSelectedRow("auto"), 0);
    execQuery(queries::updateAuto.arg(num).arg(color).arg(mark).arg(personnel_id).arg(id));
    updateTable("auto", 0);
  }
  delete w;
}

void ClientWindow::on_insertAutoButton_clicked()
{
  EditDialog *w = new EditDialog(this);
  for(int i = 0; i < 3; i++)
    w->addLineEditItem(getHeaderValue("auto", i+1));
  w->addComboBoxItem(getHeaderValue("auto", 4), getPersonnelList());
  w->exec();
  if(w->accepted())
  {
    QString num = w->getValue(0);
    QString color = w->getValue(1);
    QString mark = w->getValue(2);
    QStringList FIO = w->getValue(3).split(" ");
    QString personnel_id = getPersonnelID(FIO.value(0), FIO.value(1), FIO.value(2));
    execQuery(queries::insertAuto.arg(num).arg(color).arg(mark).arg(personnel_id));
    updateTable("auto", 0);
  }
  delete w;
}

void ClientWindow::on_insertRouteButton_clicked()
{
  EditDialog *w = new EditDialog(this);
  w->addLineEditItem(getHeaderValue("routes", 1));
  w->exec();
  if(w->accepted())
  {
    QString routeName = w->getValue(0);
    execQuery(queries::insertRoute.arg(routeName));
    updateTable("routes", 0);
  }
  delete w;
}

void ClientWindow::on_deleteRouteButton_clicked()
{
  if(!isAnyRowSelected("routes"))
    return;
  QString id = getCellValue("routes", getSelectedRow("routes"), 0);
  execQuery(queries::deleteRoute.arg(id));
  updateTable("routes", 0);
}

void ClientWindow::on_editRouteButton_clicked()
{
  if(!isAnyRowSelected("routes"))
    return;
  EditDialog *w = new EditDialog(this);
  w->addLineEditItem(getHeaderValue("routes", 1),
                     getCellValue("routes", getSelectedRow("routes"), 1));
  w->exec();
  if(w->accepted())
  {
    QString routeName = w->getValue(0);
    QString id = getCellValue("routes", getSelectedRow("routes"), 0);
    execQuery(queries::updateRoute.arg(routeName).arg(id));
    updateTable("routes", 0);
  }
  delete w;
}

void ClientWindow::updateModel(QSqlQueryModel *model)
{
  QString queryStr = model->query().executedQuery();
  model->clear();
  model->query().clear();
  model->setQuery(queryStr, db);
}

void ClientWindow::updateTableView(QTableView* tableView, int columnToHide)
{
  tableView->update();
  tableView->resizeColumnsToContents();
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  if(columnToHide != -1)
    tableView->setColumnHidden(columnToHide, true);
}

void ClientWindow::execQuery(const QString &query)
{
  qDebug() << query;
  QSqlQuery query_(db);
  if(!query_.exec(query))
  {
    qDebug() << query_.lastError().text();
    QMessageBox::critical(this, tr("Error!"), query_.lastError().text().split("\n")[0]);
  }
}
void ClientWindow::updateTable(const QString &tableAccessibleName, int columnToHide)
{
  updateModel(models[tableAccessibleName]);
  updateTableView(tableViews[tableAccessibleName], columnToHide);
}
bool ClientWindow::isAnyRowSelected(const QString &tableAccesibleName)
{
  QModelIndexList selectedIndex = tableViews[tableAccesibleName]->selectionModel()->selectedRows();
  if(selectedIndex.empty())
    return false;
  return true;
}
int ClientWindow::getSelectedRow(const QString &tableAccessibleName)
{
  return tableViews[tableAccessibleName]->selectionModel()->selectedRows().first().row();
}
QString ClientWindow::getCellValue(const QString &tableAccessibleName, int row, int col)
{
  QModelIndex index = tableViews[tableAccessibleName]->model()->index(row, col, QModelIndex());
  return tableViews[tableAccessibleName]->model()->data(index).toString();
}
QString ClientWindow::getHeaderValue(const QString &tableAccessibleName, int column)
{
  return tableViews[tableAccessibleName]->model()->headerData(column, Qt::Horizontal).toString();
}

QString ClientWindow::getAutoID(const QString &autoNum)
{
  QSqlQuery query(db);
  query.exec(queries::selectAutoId.arg(autoNum));
  query.next();
  return query.value(0).toString();
}

QString ClientWindow::getRouteID(const QString &routeName)
{
  QSqlQuery query(db);
  query.exec(queries::selectRouteId.arg(routeName));
  query.next();
  return query.value(0).toString();
}

QString ClientWindow::getPersonnelID(const QString &lastName, const QString &name, const QString &fatherName)
{
  QSqlQuery query(db);
  query.exec(queries::selectPersonnelId.arg(lastName).arg(name).arg(fatherName));
  query.next();
  return query.value(0).toString();
}

QString ClientWindow::getCurrentDate()
{
  QSqlQuery query(db);
  query.exec(queries::selectCurrentDate);
  query.next();
  return query.value(0).toString();
}

QString ClientWindow::getTimeOut(const QString &id)
{
  QSqlQuery query(db);
  query.exec(queries::selectTimeOut.arg(id));
  query.next();
  return query.value(0).toString();
}

QString ClientWindow::getTimeIn(const QString &id)
{
  QSqlQuery query(db);
  query.exec(queries::selectTimeIn.arg(id));
  query.next();
  return query.value(0).toString();
}

QStringList *ClientWindow::getAutoNumList()
{
  QSqlQuery query(db);
  query.exec(queries::selectAutoNums);

  QStringList* list = new QStringList;
  while(query.next())
  {
    list->append(query.value(0).toString());
  }
  return list;
}

QStringList *ClientWindow::getRouteNameList()
{
  QSqlQuery query(db);
  query.exec(queries::selectRouteNames);

  QStringList* list = new QStringList;
  while(query.next())
  {
    list->append(query.value(0).toString());
  }
  return list;
}

QStringList *ClientWindow::getPersonnelList()
{
  QSqlQuery query(db);
  query.exec(queries::selectPersonnelFIOs);

  QStringList* list = new QStringList;
  while(query.next())
  {
    list->append(query.value(0).toString());
  }
  return list;
}

void ClientWindow::on_tabWidget_currentChanged(int index)
{
  static bool firstTime = true;
  if(firstTime)
  {
    firstTime = false;
    return;
  }
  switch(index)
  {
    case 0:
      updateTable("journal", 0);
      break;
    case 1:
      updateTable("personnel", 0);
      break;
    case 2:
      updateTable("auto", 0);
      break;
    case 3:
      updateTable("routes", 0);
      break;
    case 4:
      updateTable("additional");
      break;
  }
}

void ClientWindow::on_sortJournalComboBox_currentIndexChanged(int index)
{
  setTableSortMode("journal", index);
}
void ClientWindow::on_sortPersonnelComboBox_currentIndexChanged(int index)
{
  setTableSortMode("personnel", index);
}
void ClientWindow::on_sortAutoComboBox_currentIndexChanged(int index)
{
  setTableSortMode("auto", index);
}
void ClientWindow::on_sortRoutesComboBox_currentIndexChanged(int index)
{
  setTableSortMode("routes", index);
}

void ClientWindow::on_additionalComboBox_currentIndexChanged(int index)
{
  QComboBox* routeComboBox = findChild<QComboBox*>("additionalRouteComboBox");
  QString routeId = getRouteID(routeComboBox->currentText());
  switch(index)
  {
    case 0:
      setupTable("additionalTableView", queries::selectNumActiveAuto.arg(routeId), "additional");
      break;
    case 1:
     setupTable("additionalTableView", queries::selectRouteRecord.arg(routeId), "additional");
      break;
  }

}

void ClientWindow::on_additionalRouteComboBox_currentIndexChanged(int index)
{
  on_additionalComboBox_currentIndexChanged(findChild<QComboBox*>("additionalComboBox")->currentIndex());
}

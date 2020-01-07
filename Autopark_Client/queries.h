#ifndef QUERIES_H
#define QUERIES_H

#include <QString>

namespace queries
{

  static QString selectUser = "SELECT ID, USERNAME from users1 where username='%1' and password='%2'";

  static QString selectJournal = "select journal.ID, TO_CHAR(TIME_OUT, 'DD-MON-YYYY HH:MI:SS') as \"Время отбытия\", "
                                 "TO_CHAR(TIME_IN, 'DD-MON-YYYY HH:MI:SS') as \"Время прибытия\", "
                                 "auto.NUM as \"Номер автомобиля\", routes.name as \"Маршрут\" from journal, routes, auto "
                                 "where routes.id = journal.ROUTE_ID and auto.id = journal.AUTO_ID";
  static QString selectPersonnel = "select ID, LAST_NAME as \"Фамилия\", FIRST_NAME as \"Имя\", FATHER_NAME as \"Отчество\" "
                                   "from auto_personnel";
  static QString selectAuto = "select auto.ID, NUM as \"Номер\", COLOR as \"Цвет\", MARK as \"Марка\", "
                              "auto_personnel.LAST_NAME || ' ' || auto_personnel.FIRST_NAME || ' ' || "
                              "auto_personnel.FATHER_NAME as \"ФИО\" from auto, auto_personnel "
                              "where auto_personnel.ID = auto.PERSONNEL_ID";
  static QString selectRoutes = "select ID, NAME as \"Маршрут\" from routes";

  static QString selectHash = "select ora_hash('%1') from dual;";

  static QString insertJournal = "insert into journal(TIME_OUT, TIME_IN, AUTO_ID, ROUTE_ID) values ('%1', NULL, '%2', '%3');";
  static QString deleteJournal = "delete from journal where id = %1";
  static QString updateJournal = "update journal set TIME_OUT='%1', TIME_IN='%2', AUTO_ID=%3, ROUTE_ID=%4 where ID=%5";
  static QString updateTimeInJournal = "update journal set TIME_IN='%1' where ID=%2";
  static QString selectAutoId = "select id from auto where num = '%1'";
  static QString selectRouteId = "select id from routes where name = '%1'";
  static QString selectPersonnelId = "select id from auto_personnel where LAST_NAME='%1' and FIRST_NAME='%2' and FATHER_NAME='%3'";
  static QString selectCurrentDate = "select TO_CHAR(CURRENT_DATE, 'DD-MON-YYYY HH:MI:SS') FROM dual";
  static QString selectAutoNums = "select num from auto";
  static QString selectRouteNames = "select name from routes";
  static QString selectPersonnelFIOs = "select LAST_NAME || ' ' || FIRST_NAME || ' ' || FATHER_NAME as FIO from auto_personnel";
  static QString selectTimeOut = "select TO_CHAR(TIME_OUT, 'DD-MON-YYYY HH:MI:SS') from journal where id=%1";
  static QString selectTimeIn = "select TO_CHAR(TIME_IN, 'DD-MON-YYYY HH:MI:SS') from journal where id=%2";
  static QString deletePersonnel = "delete from auto_personnel where id = %1";
  static QString updatePersonnel = "update auto_personnel set LAST_NAME='%1', FIRST_NAME='%2', FATHER_NAME='%3' where id=%4";
  static QString insertPersonnel = "insert into auto_personnel(LAST_NAME, FIRST_NAME, FATHER_NAME) values ('%1','%2','%3')";
  static QString deleteAuto = "delete from auto where id = %1";
  static QString updateAuto = "update auto set NUM='%1', COLOR='%2', MARK='%3', PERSONNEL_ID=%4 where ID=%5";
  static QString insertAuto = "insert into auto(NUM, COLOR, MARK, PERSONNEL_ID) values ('%1','%2','%3','%4')";
  static QString insertRoute = "insert into routes(NAME) values ('%1')";
  static QString updateRoute = "update routes set NAME = '%1' where id = %2";
  static QString deleteRoute = "delete from routes where id = %1";

  //static QString selectRouteRecord = "select AUTO_ID, ROUTE_TIME from (select ID, AUTO_ID, TO_CHAR(TIME_IN - TIME_OUT) as ROUTE_TIME from journal where ROUTE_ID = %1 and TIME_IN is not NULL order by ROUTE_TIME asc) where rownum=1";
  static QString selectRouteRecord = "select AUTO.NUM as \"Номер автомобиля\", ROUTE_TIME as \"Время проезда\" from (select ID, AUTO_ID, TO_CHAR(TIME_IN - TIME_OUT, 'HH:MI:SS') as ROUTE_TIME from journal where ROUTE_ID = %1 and TIME_IN is not NULL order by ROUTE_TIME asc), auto where rownum=1 and auto.ID = AUTO_ID";
  static QString selectNumActiveAuto = "select count(*) as \"Количество автомобилей в рейсе\" from journal where TIME_IN is null and ROUTE_ID = %1";

}

#endif // QUERIES_H

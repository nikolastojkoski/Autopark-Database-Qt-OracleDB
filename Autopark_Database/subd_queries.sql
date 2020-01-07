select * from auto;
select * from auto_personnel;
select * from journal;
select * from routes;

---????????????? ???????
select * from auto_personnel order by last_name;
select * from auto_personnel order by first_name desc;
select count(id) from auto where substr(auto.num, 1, 1) = 'p'

---??????? ? ?????????????
select auto_personnel.last_name, auto.num from auto_personnel, auto 
  where auto.personnel_id = auto_personnel.id and auto_personnel.last_name = 'Stojkoski';
  
select distinct route_id as active_route from journal
  where journal.time_out < CURRENT_DATE and CURRENT_DATE < journal.time_in; 

---??????????? ??????(join)
select journal.time_out, routes.name from journal full join routes on routes.id = journal.route_id

select time_in, time_out, routes.name, auto.num from journal 
  full join routes on routes.id = journal.route_id 
  full join auto on auto.id = journal.auto_id

--??? ?????????? ???????
select a.name, auto.num, a.route_time from
 (select routes.name, min(time_in - time_out) as route_time from journal, routes 
  where journal.route_id = routes.id group by routes.name) a, auto, journal, routes
where routes.name = a.name and routes.id = journal.route_id 
and (journal.time_in - journal.time_out) = a.route_time and auto.id = journal.auto_id
 
select count(journal.id) as num_active from journal, routes
where (CURRENT_DATE > time_out and CURRENT_DATE < time_in) and routes.id = journal.route_id and routes.name = 'route2';

--??????? ??????
insert into auto (num, color, mark, personnel_id) values (
insert into auto_personnel (first_name, last_name, father_name) values ('','','')



create temp table th (pzn integer, grp char (5));
create unique index i_th on th (pzn);

load from 'c:\temp\BtM-Gruppen-neu.csv' delimiter ';'
insert into th;

update th set grp = lpad (trim(grp), 5, '0')

select distinct grp from th
into temp th2

insert into th2 values ('01190');
insert into th2 values ('01191');
insert into th2 values ('01192');
insert into th2 values ('01189');
insert into th2 values ('01193');
insert into th2 values ('01220');
insert into th2 values ('01221');

unload to 'C:\temp\zdkbtm.txt'
select grp, btm_bez
from th2, zdkbtm where grp = btm_gruppe

-------------------------------

create temp table th (pzn integer, grp char (5));
create unique index i_th on th (pzn);

load from 'c:\temp\BtM-Gruppen-neu.csv' delimiter ';'
insert into th;

update th set grp = lpad (trim(grp), 5, '0')
update th set pzn = (select articleno from articlecodes where code_type = 16 and article_code = th.pzn)

-- select * from narctransactioncatalog where transactiondate >= 20140101 and articleno in (select pzn from th)

update narctransactioncatalog set narcgroup=(select grp from th where pzn = narctransactioncatalog.articleno) where transactiondate >= 20140101 and articleno in (select pzn from th)


create temp table th2 (grp char (5), name char(30));
create unique index i_th2 on th2 (grp);

load from  'C:\temp\zdkbtm.txt'
insert into th2;

-- select count(*) from narctransactioncatalog where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 0

update narctransactioncatalog set narcname=(select name from th2 where grp = narctransactioncatalog.narcgroup) where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 0;
update narctransactioncatalog set narcname=(select name from th2 where grp = narctransactioncatalog.narcgroup) where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 1;
update narctransactioncatalog set narcname=(select name from th2 where grp = narctransactioncatalog.narcgroup) where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 2;
update narctransactioncatalog set narcname=(select name from th2 where grp = narctransactioncatalog.narcgroup) where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 3;
update narctransactioncatalog set narcname=(select name from th2 where grp = narctransactioncatalog.narcgroup) where transactiondate >= 20140101 and narcgroup in (select grp from th2) and mod(catalogid, 5) = 4;

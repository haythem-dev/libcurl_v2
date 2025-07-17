ACHTUNG: 
==============
Soll der login::Manager:: für Testzwecke (Testdatenbank) verwendet werden, 
dann muss in das Verzeichnis, in dem das Binary gestartet wird folgende Datei leer angelegt werden:

login4test.cfg

angelegt werden


---------------------------------------------------
Identifizierte Tesfälle:
---------------------------------------------------
-> Rollback
-> lock
-> 
-> Falscher Typ: 
	a) Bsp.: k; 
	b) Einträge b mit gleicher PurschaseOrderNo; 
	c) Einträge u und s auf a testen
-> Merge
-> Unlock Fehler provozieren
---------------------------------------------------

select * from kunde where vertriebszentrumnr = 5 and idfnr = 3287350;
select * from bliefer where besla_nr = 3287350;
select bga_nr, * from bliefer where  bga_nr != 0;

---------------------------------------------------
select * from customernomapping;
select * from narctransactiontransfer;
select * from narctransactionoriginal;
select * from narctransactioncatalog;
select * from narclock;
---------------------------------------------------

delete from narctransactionoriginal where 1=1;
delete from narctransactioncatalog  where 1=1;
delete from narclock  where 1=1;
delete from narctransactiontransfer  where 1=1;
delete from customernomapping  where 1=1;
load from 'd:\marathon\narcotics\unix\sql\narctransfer-testdata.txt' insert into narctransactiontransfer;
insert into narclock values( 5, 2744756, 'test', 20080328, 100000);
insert into customernomapping values(5,345822,9000001);

insert into narctransactioncatalog 
values( 2461945, 5, 5138602,	20080328,	125831,	0,	'b',	2226, 5, 5, 0, 0, 'Supplier Number <0> was transferred!', ' ', 0, ' ', ' ',  0.0000, ' ',  0.000 )
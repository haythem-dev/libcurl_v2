-------------------------------------------------------------------------------------------------------------------------------
Spaltennamen
-------------------------------------------------------------------------------------------------------------------------------
select  colname
from systables t , syscolumns c
where t.tabid = c.tabid 
and t.tabname = 'artikellokal';

select  colname
from systables t , syscolumns c
where t.tabid = c.tabid 
and t.tabname = 'kunde';

-------------------------------------------------------------------------------------------------------------------------------
narctransactiontransfer
-------------------------------------------------------------------------------------------------------------------------------
update narctransactiontransfer set completedflag = '1';
update narctransactiontransfer set branchno =  '5';

select * from narctransactiontransfer;
select count(*) from narctransactiontransfer;

unload to "c:\ntt" select * from narctransactiontransfer;
delete from narctransactiontransfer;


load from "c:\ntt2" insert into narctransactiontransfer;

-------------------------------------------------------------------------------------------------------------------------------
narctransactioncatalog
-------------------------------------------------------------------------------------------------------------------------------
select * from bliefer; 

where bga_nr != ' ';
update beskopf set besla_nr = 3321733 where bestell_nr = 30718 and filialnr = 5;
select * from beskopf;
select * from beskopf where besla_nr = 3321733 and filialnr = 5;
select * from narctransactioncatalog;

select * from narctransactioncatalog where branchno = 5;
select * from narctransactioncatalog where customersuppliername != '';
select * from narctransactioncatalog where articleno in (
4187521,
293054,
8911876
);


--------------------------------------------------------------------------------------------------------
Bestands-Änderung
--------------------------------------------------------------------------------------------------------
                                               SOLL-Bestand                     aktueller_Bestand 
                                                (Angaben von Köln)         (steht in unserer Tabelle)
-----------------------------------------------------------------------------------------------------------------------------------------
PZN  2744710   Anfangsb.                 3                                          2
PZN  6892270   Anfangsb.               14                                         13
PZN  6332312   Anfangsb.               29                                         28
PZN  0583094   Anfangsb.              34                                          33
PZN  0165563   Anfangsb.               4                                            3

--------------------------------------------------------------------------------------------------------
Anfangs-Artikel
--------------------------------------------------------------------------------------------------------
select * 
from narctransactioncatalog 
where branchno = 46 
and transactiontype = 'u' 
and articleno in (
2744710   ,
6892270   ,
6332312   ,
0583094   ,
0165563   
);

--------------------------------------------------------------------------------------------------------
alle Artikel
--------------------------------------------------------------------------------------------------------
select * 
from narctransactioncatalog 
where branchno = 46 
and articleno in (
2744710   ,
6892270   ,
6332312   ,
0583094   ,
0165563   
);

--------------------------------------------------------------------------------------------------------
Artikel 0165563 
--------------------------------------------------------------------------------------------------------
select * 
from narctransactioncatalog 
where branchno = 46 
and articleno in (
0165563   
);

u pdate narctransactioncatalog 
set qtyinstock = qtyinstock + 1
where branchno = 46 
and articleno in (
0165563 );

--------------------------------------------------------------------------------------------------------
Artikel 2744710 --> 8
--------------------------------------------------------------------------------------------------------
u pdate narctransactioncatalog 
set qtyinstock = qtyinstock + 1
where branchno = 46 
and articleno in (
2744710 );
--------------------------------------------------------------------------------------------------------
Artikel 6892270 --> 12
--------------------------------------------------------------------------------------------------------
u pdate narctransactioncatalog 
set qtyinstock = qtyinstock + 1
where branchno = 46 
and articleno in (
6892270 );

--------------------------------------------------------------------------------------------------------
Artikel 6332312 --> 41
--------------------------------------------------------------------------------------------------------
u pdate narctransactioncatalog 
set qtyinstock = qtyinstock + 1
where branchno = 46 
and articleno in (
6332312   );

--------------------------------------------------------------------------------------------------------
Artikel 0583094 --> 31
--------------------------------------------------------------------------------------------------------
u pdate narctransactioncatalog 
set qtyinstock = qtyinstock + 1
where branchno = 46 
and articleno in (
0583094   );

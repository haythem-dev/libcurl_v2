



s e l e c t  colname
from systables t , syscolumns c
where t.tabid = c.tabid 
and t.tabname = 'artikelkonto';




select * from artikelkonto;

select k.*
from artikelkonto zartikel z
where z.btm = '1'
and z.artikel_nr = k.artikel_nr
and k.filialnr = 52
and zeitunix between 1151746200
and 1151791140


select * from narctransactioncatalog where branchno = 82
and qtyinstock < 0;


select * from narctransactioncatalog 

select count(*) from narctransactioncatalog 
where transactiontype = 'u' 
and qtyinstock < 0
and branchno = 82;


select count(*) from narctransactioncatalog 
where transactiondate = 20060701
and transactiontype != 'u';

unload to "c:\btm_ungleich_u_td_ist_20060701"
select * from narctransactioncatalog 
where transactiontype != 'u'
and transactiondate = 20060701;


###
d e l e t e from narctransactioncatalog 
where transactiontype != 'u'
and transactiondate = 20060701;
and articleno in (
1208777
,1098461
,1299018
,1348188
,1348188
,2842045
,2842051
,4432624
,6332329
,8617740
,8859756
,8859756
,8859905
,550522,
,574994,
,575108,
,575114,
,255763,
,583065,
,583094,
,2842068
,2387807
,2388155
,2388155
,322695,
,4235065
,692966,
,8859874
,8859733
);


i n s e r t into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,1208777,20060701,95752,3381652,'a',1271,0,1,23,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,1098461,20060701,100142,4459037,'a',1535,0,1,43,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,1299018,20060701,100351,4425877,'a',1641,0,1,178,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,1348188,20060701,95707,3381652,'a',1271,0,1,119,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,1348188,20060701,110034,4218428,'a',4125,0,2,117,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2842045,20060701,93735,4246318,'a',828,0,1,167,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2842051,20060701,100158,4459037,'a',1535,0,1,240,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,4432624,20060701,104408,4269070,'a',3567,0,1,13,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,6332329,20060701,110729,4215878,'a',4301,0,1,22,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8617740,20060701,91921,4246726,'a',555,0,1,45,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8859756,20060701,101351,4517450,'a',2007,0,1,213,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8859756,20060701,112947,4216122,'a',4942,0,1,212,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8859905,20060701,91924,4246726,'a',555,0,1,30,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,550522,20060701,100145,4459037,'a',1535,0,1,157,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,574994,20060701,100133,4459037,'a',1535,0,1,102,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,575108,20060701,94722,4419747,'a',991,0,1,198,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,575114,20060701,92941,4246525,'a',681,0,1,108,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,255763,20060701,95832,4216197,'a',1359,0,2,4,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,583065,20060701,93726,4246318,'a',828,0,3,16,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,583094,20060701,93728,4246318,'a',828,0,10,38,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2842068,20060701,95704,3381652,'a',1271,0,1,88,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2387807,20060701,93730,4246318,'a',828,0,1,50,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2388155,20060701,95745,3381652,'a',1271,0,1,215,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,2388155,20060701,100012,4520593,'a',1426,0,1,214,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,322695,20060701,110734,4215878,'a',4301,0,1,9,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,4235065,20060701,91903,4246726,'a',555,0,1,39,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,692966,20060701,112311,4248240,'a',4658,0,1,59,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8859874,20060701,100233,4476544,'a',1515,0,1,51,'1',0);
insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, orderno, qtytransaction, qtyinstock,completedflag, posno) values(82,8859733,20060701,091924,4246726,'a',555,0,1,108,'1',0);

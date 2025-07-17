unload to 'C:\temp\narc_gi_05.txt'
select articleno, purchaseorderno, labelqty from gilabel 
where branchno = 5 and 
articleno in (select artikel_nr from zartikel where btm = '1') and 
(causecodeno in (select causecodeno from gicausecode where stockrelflag = 1) or causecodeno = 0) and
labellastchangedate <  datetime (2015-10-02) YEAR TO DAY  and labellastchangedate > datetime (2015-10-01 12:30) YEAR TO MINUTE



create temp table th (pzn integer, bnr integer, qty integer);
load from 'C:\temp\narc_gi_05.txt'
insert into th

insert into narctransactiontransfer(branchno, articleno, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, qtytransaction, completedflag)
select '5', pzn, '20151001', '200000', '0' , 'b', bnr, qty, '1' from th



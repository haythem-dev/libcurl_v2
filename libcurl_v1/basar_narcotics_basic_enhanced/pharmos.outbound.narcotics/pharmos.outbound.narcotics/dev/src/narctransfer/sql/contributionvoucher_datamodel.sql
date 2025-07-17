CREATE TABLE informix.narccontributionvoucher
(
   documentno integer DEFAULT 0,
   transactiondate date DEFAULT 12319999,
   documenttype smallint,
   acknowledgementlink text
)
;

CREATE UNIQUE INDEX xpknarccontributionvoucher ON informix.narccontributionvoucher
(
  acknowledgementlink,
  documentno,
  transactiondate,
  documenttype
)
;

CREATE TABLE informix.narctransactioncatalog
(
   catalogid integer,
   branchno smallint DEFAULT 0,
   articleno integer DEFAULT 0,
   transactiondate integer DEFAULT 0,
   transactiontime integer DEFAULT 0,
   customersupplierno integer DEFAULT 0,
   transactiontype char(2),
   purchaseorderno integer DEFAULT 0,
   qtytransaction integer DEFAULT 0,
   qtyinstock integer DEFAULT 0,
   deletedflag char(1) DEFAULT 0,
   processedflag char(1) DEFAULT 0,
   customersuppliername char(40),
   customersuppliercity char(30),
   invoiceno integer DEFAULT 0,
   narcgroup char(3),
   narcname char(30),
   narcpotency decimal(9,4) DEFAULT 0.0000,
   narcunit char(9),
   narcfactor decimal(8,3) DEFAULT 0.000,
   contributionvoucherno integer DEFAULT 0,
   acknowledgementstatus smallint DEFAULT 0
)
;

CREATE UNIQUE INDEX xpknarctransactioncatalog ON informix.narctransactioncatalog(catalogid)
;

CREATE INDEX xie1narctransactioncatalog ON informix.narctransactioncatalog
(
  branchno,
  articleno,
  transactiondate,
  transactiontime
)
;

CREATE INDEX xie2narctransactioncatalog ON informix.narctransactioncatalog
(
  branchno,
  customersupplierno,
  transactiondate,
  transactiontime
)
;

CREATE INDEX xie3narctransactioncatalog ON informix.narctransactioncatalog
(
  branchno,
  purchaseorderno
)
;

CREATE INDEX xie4narctransactioncatalog ON informix.narctransactioncatalog
(
  branchno,
  deletedflag
)
;

CREATE INDEX xie5narctransactioncatalog ON informix.narctransactioncatalog
(
  branchno,
  transactiontype,
  processedflag,
  deletedflag
)
;

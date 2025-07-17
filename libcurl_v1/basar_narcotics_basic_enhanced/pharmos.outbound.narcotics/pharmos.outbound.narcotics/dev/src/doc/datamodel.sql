
CREATE TABLE customernomapping
(
	branchno              smallint   DEFAULT 0 NOT NULL,
	customernoextern      integer   DEFAULT 0 NOT NULL,
	customernointern      integer   DEFAULT 0 NOT NULL,
	switchaddressflag     smallint   DEFAULT 0 NOT NULL
)
EXTENT SIZE 20
NEXT SIZE 10
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKcustomernomapping ON customernomapping
(
	branchno              ASC,
	customernoextern      ASC,
	customernointern      ASC
)
;



CREATE TABLE NarcDeliverynoteCopy
(
	catalogid             integer   DEFAULT 0 NOT NULL,
	rectificationcause    smallint   DEFAULT 0 NOT NULL,
	rectificationcomment  varchar(256)   DEFAULT '' NOT NULL,
	originalqty           integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 50
NEXT SIZE 20
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKnarcdeliverynotecopy ON NarcDeliverynoteCopy
(
	catalogid             ASC
)
;



CREATE TABLE NarcDiffQtyInStock
(
	DiffID                serial  NOT NULL,
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	CreationDate          integer   DEFAULT 0 NOT NULL,
	DiffQtyInStock        integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 100
NEXT SIZE 10
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKNarcDiffQtyInStock ON NarcDiffQtyInStock
(
	DiffID                ASC
)
;



CREATE INDEX XIE1NarcDiffQtyInStock ON NarcDiffQtyInStock
(
	BranchNo              ASC,
	ArticleNo             ASC
)
;



CREATE TABLE narcinvalidarticle
(
	articleno             integer   DEFAULT 0 NOT NULL,
	invalidarticledate    integer   DEFAULT 0 NOT NULL,
	narcgroup             char(5)   DEFAULT '' NOT NULL,
	narcname              char(30)   DEFAULT '' NOT NULL,
	narcpotency           decimal(9,4)   DEFAULT 0 NOT NULL,
	narcunit              char(9)   DEFAULT '' NOT NULL
)
EXTENT SIZE 3
NEXT SIZE 1
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKnarcinvalidarticle ON narcinvalidarticle
(
	articleno             ASC
)
;



CREATE TABLE NarcLock
(
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	UserName              char(30)   DEFAULT '' NOT NULL,
	LockDate              integer   DEFAULT 0 NOT NULL,
	LockTime              integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 2
NEXT SIZE 1
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKnarclock ON NarcLock
(
	BranchNo              ASC,
	ArticleNo             ASC
)
;



CREATE TABLE NarcNumber
(
	filialnr              smallint   DEFAULT 0 NOT NULL,
	nr_art                char(6)   DEFAULT '' NOT NULL,
	nr                    integer   DEFAULT 0 NOT NULL,
	nr_maximum            integer   DEFAULT 0 NOT NULL,
	nr_start              integer   DEFAULT 0 NOT NULL,
	ueberlauf_aktion      smallint   DEFAULT 0 NOT NULL
)
EXTENT SIZE 100
NEXT SIZE 10
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKNarcNumber ON NarcNumber
(
	filialnr              ASC,
	nr_art                ASC
)
;



CREATE TABLE NarcOpenOrders
(
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	OrderDate             integer   DEFAULT 0 NOT NULL,
	PurchaseOrderNo       integer   DEFAULT 0 NOT NULL,
	SupplierNo            integer   DEFAULT 0 NOT NULL,
	QtyOrder              integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 130
NEXT SIZE 40
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKnarcopenorders ON NarcOpenOrders
(
	BranchNo              ASC,
	ArticleNo             ASC,
	OrderDate             ASC,
	PurchaseOrderNo       ASC
)
;



CREATE TABLE NarcProtocol
(
	ProtocolID            serial  NOT NULL,
	CatalogID             integer   DEFAULT 0 NOT NULL,
	UserName              char(30)   DEFAULT '' NOT NULL,
	ProtocolDate          integer   DEFAULT 0 NOT NULL,
	ProtocolTime          integer   DEFAULT 0 NOT NULL,
	ActivityType          char(1)   DEFAULT '' NOT NULL,
	ChangedAttribute      char(30)   DEFAULT '' NOT NULL,
	ValueOld              char(20)   DEFAULT '' NOT NULL,
	ValueNew              char(20)   DEFAULT '' NOT NULL
)
EXTENT SIZE 12500
NEXT SIZE 4000
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKnarcprotocol ON NarcProtocol
(
	ProtocolID            ASC
)
;



CREATE INDEX XIE1narcprotocol ON NarcProtocol
(
	CatalogID             ASC,
	ProtocolDate          ASC,
	ProtocolTime          ASC
)
;



CREATE TABLE NarcTransactionCatalog
(
	CatalogID             serial  NOT NULL,
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	TransactionDate       integer   DEFAULT 0 NOT NULL,
	TransactionTime       integer   DEFAULT 0 NOT NULL,
	CustomerSupplierNo    integer   DEFAULT 0 NOT NULL,
	TransactionType       char(2)   DEFAULT '' NOT NULL,
	PurchaseOrderNo       integer   DEFAULT 0 NOT NULL,
	QtyTransaction        integer   DEFAULT 0 NOT NULL,
	QtyInStock            integer   DEFAULT 0 NOT NULL,
	DeletedFlag           char(1)   DEFAULT '0' NOT NULL,
	ProcessedFlag         char(1)   DEFAULT '0' NOT NULL,
	CustomerSupplierName  char(40)   DEFAULT '' NOT NULL,
	CustomerSupplierStreet  char(30)   DEFAULT '' NOT NULL,
	CustomerSupplierCity  char(30)   DEFAULT '' NOT NULL,
	InvoiceNo             integer   DEFAULT 0 NOT NULL,
	NarcGroup             char(5)   DEFAULT '' NOT NULL,
	NarcName              char(30)   DEFAULT '' NOT NULL,
	NarcPotency           decimal(9,4)   DEFAULT 0 NOT NULL,
	NarcUnit              char(9)   DEFAULT '' NOT NULL,
	NarcFactor            decimal(8,3)   DEFAULT 0 ,
	ContributionVoucherNo  integer   DEFAULT 0 NOT NULL,
	Username              char(20)   DEFAULT '' NOT NULL,
	PrintedDate           integer   DEFAULT 0 NOT NULL,
	AcknowledgementStatus  smallint   DEFAULT 0 NOT NULL,
	TourId                char(6)   DEFAULT '' NOT NULL,
	PrintedTime           integer   DEFAULT 0 NOT NULL,
	contributionvouchertransferstatus  smallint   DEFAULT 0 NOT NULL,
	contributionvouchertransferdate  datetime year to second   DEFAULT datetime (9999-12-31 23:59:59) YEAR TO SECOND NOT NULL,
	deliverynotecopytransferstatus  smallint   DEFAULT 0 NOT NULL,
	deliverynotecopytransferdate  datetime year to second   DEFAULT datetime (9999-12-31 23:59:59) YEAR TO SECOND NOT NULL,
	surname               char(20)   DEFAULT '' NOT NULL,
	forename              char(20)   DEFAULT '' NOT NULL
)
EXTENT SIZE 95000
NEXT SIZE 30000
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKNarcTransactionCatalog ON NarcTransactionCatalog
(
	CatalogID             ASC
)
;



CREATE INDEX XIE1NarcTransactionCatalog ON NarcTransactionCatalog
(
	BranchNo              ASC,
	ArticleNo             ASC,
	TransactionDate       ASC,
	TransactionTime       ASC
)
;



CREATE INDEX XIE2NarcTransactionCatalog ON NarcTransactionCatalog
(
	BranchNo              ASC,
	CustomerSupplierNo    ASC,
	TransactionDate       ASC,
	TransactionTime       ASC
)
;



CREATE INDEX XIE3NarcTransactionCatalog ON NarcTransactionCatalog
(
	BranchNo              ASC,
	PurchaseOrderNo       ASC
)
;



CREATE INDEX XIE4NarcTransactionCatalog ON NarcTransactionCatalog
(
	BranchNo              ASC,
	DeletedFlag           ASC,
	AcknowledgementStatus  ASC
)
;



CREATE INDEX XIE5NarcTransactionCatalog ON NarcTransactionCatalog
(
	BranchNo              ASC,
	TransactionType       ASC,
	ProcessedFlag         ASC
)
;



CREATE TABLE NarcTransactionOriginal
(
	OriginalID            serial  NOT NULL,
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	TransactionDate       integer   DEFAULT 0 NOT NULL,
	TransactionTime       integer   DEFAULT 0 NOT NULL,
	CustomerSupplierNo    integer   DEFAULT 0 NOT NULL,
	TransactionType       char(2)   DEFAULT '' NOT NULL,
	PurchaseOrderNo       integer   DEFAULT 0 NOT NULL,
	QtyTransaction        integer   DEFAULT 0 NOT NULL,
	QtyInStock            integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 90000
NEXT SIZE 30000
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKNarcTransactionOriginal ON NarcTransactionOriginal
(
	OriginalID            ASC
)
;



CREATE INDEX XIE1NarcTransactionOriginal ON NarcTransactionOriginal
(
	BranchNo              ASC,
	ArticleNo             ASC,
	TransactionDate       ASC,
	TransactionTime       ASC
)
;



CREATE TABLE NarcTransactionTransfer
(
	TransferID            serial  NOT NULL,
	BranchNo              smallint   DEFAULT 0 NOT NULL,
	ArticleNo             integer   DEFAULT 0 NOT NULL,
	TransactionDate       integer   DEFAULT 0 NOT NULL,
	TransactionTime       integer   DEFAULT 0 NOT NULL,
	CustomerSupplierNo    integer   DEFAULT 0 NOT NULL,
	TransactionType       char(2)   DEFAULT '' NOT NULL,
	PurchaseOrderNo       integer   DEFAULT 0 NOT NULL,
	OrderNo               integer   DEFAULT 0 NOT NULL,
	QtyTransaction        integer   DEFAULT 0 NOT NULL,
	QtyInStock            integer   DEFAULT 0 NOT NULL,
	CompletedFlag         char(1)   DEFAULT '0' NOT NULL,
	PosNo                 integer   DEFAULT 0 NOT NULL
)
EXTENT SIZE 22
NEXT SIZE 10
LOCK MODE ROW
;



CREATE UNIQUE INDEX XPKNarcTransactionTransfer ON NarcTransactionTransfer
(
	TransferID            ASC
)
;



CREATE INDEX XIE1NarcTransactionTransfer ON NarcTransactionTransfer
(
	BranchNo              ASC,
	ArticleNo             ASC,
	OrderNo               ASC
)
;



CREATE INDEX XIE2NarcTransactionTransfer ON NarcTransactionTransfer
(
	BranchNo              ASC,
	OrderNo               ASC
)
;





create procedure erwin_raise_except(err int,msg varchar(255))
raise exception err,0,msg;
end procedure;

CREATE TRIGGER tI_NarcProtocol INSERT ON NarcProtocol
  REFERENCING NEW AS INSERTED
  FOR EACH ROW
-- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
-- INSERT trigger on NarcProtocol
    -- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
    -- NarcTransactionCatalog R/3 NarcProtocol on child insert restrict 
    -- ERWIN_RELATION:CHECKSUM="00010374", PARENT_OWNER="", PARENT_TABLE="NarcTransactionCatalog"
    -- CHILD_OWNER="", CHILD_TABLE="NarcProtocol"
    -- P2C_VERB_PHRASE="R/3", C2P_VERB_PHRASE="", 
    -- FK_CONSTRAINT="", FK_COLUMNS="CatalogID"
    WHEN (
      -- %NotnullFK(inserted," IS NOT NULL AND") 
      
      NOT EXISTS (
        SELECT * FROM NarcTransactionCatalog
        WHERE
          -- %JoinFKPK(inserted,NarcTransactionCatalog," = "," AND") 
          inserted.CatalogID = NarcTransactionCatalog.CatalogID
      )
    )
      (EXECUTE PROCEDURE erwin_raise_except(
        -746,
        'Cannot insert NarcProtocol because NarcTransactionCatalog does not exist.'
      )) 


;

CREATE TRIGGER tU_NarcProtocol UPDATE ON NarcProtocol
  REFERENCING OLD AS DELETED NEW AS INSERTED
  FOR EACH ROW
-- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
-- UPDATE trigger on NarcProtocol
    -- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
    -- NarcTransactionCatalog R/3 NarcProtocol on child update restrict 
    -- ERWIN_RELATION:CHECKSUM="000101dc", PARENT_OWNER="", PARENT_TABLE="NarcTransactionCatalog"
    -- CHILD_OWNER="", CHILD_TABLE="NarcProtocol"
    -- P2C_VERB_PHRASE="R/3", C2P_VERB_PHRASE="", 
    -- FK_CONSTRAINT="", FK_COLUMNS="CatalogID"
    WHEN (
      -- %NotnullFK(inserted," IS NOT NULL AND") 
      
      NOT EXISTS (
        SELECT * FROM NarcTransactionCatalog
        WHERE
          -- %JoinFKPK(inserted,NarcTransactionCatalog," = "," AND") 
          inserted.CatalogID = NarcTransactionCatalog.CatalogID
      )
    )
      (EXECUTE PROCEDURE erwin_raise_except(
        -746,
        'Cannot update NarcProtocol because NarcTransactionCatalog does not exist.'
      )) 


;




create procedure erwin_raise_except(err int,msg varchar(255))
raise exception err,0,msg;
end procedure;

CREATE TRIGGER tD_NarcTransaction DELETE ON NarcTransactionCatalog
  REFERENCING OLD AS DELETED
  FOR EACH ROW
-- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
-- DELETE trigger on NarcTransactionCatalog
    -- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
    -- NarcTransactionCatalog R/3 NarcProtocol on parent delete restrict 
  -- ERWIN_RELATION:CHECKSUM="0000e561", PARENT_OWNER="", PARENT_TABLE="NarcTransactionCatalog"
    -- CHILD_OWNER="", CHILD_TABLE="NarcProtocol"
    -- P2C_VERB_PHRASE="R/3", C2P_VERB_PHRASE="", 
    -- FK_CONSTRAINT="", FK_COLUMNS="CatalogID"
    WHEN (EXISTS (
      SELECT * FROM NarcProtocol
      WHERE
        --  %JoinFKPK(NarcProtocol,deleted," = "," AND") 
        NarcProtocol.CatalogID = deleted.CatalogID
    ))
       (EXECUTE PROCEDURE erwin_raise_except(
         -746,
         'Cannot delete NarcTransactionCatalog because NarcProtocol exists.'
       )) 


;

CREATE TRIGGER tU_NarcTransaction UPDATE ON NarcTransactionCatalog
  REFERENCING OLD AS DELETED NEW AS INSERTED
  FOR EACH ROW
-- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
-- UPDATE trigger on NarcTransactionCatalog
  -- ERwin Builtin Mittwoch, 27. April 2016 11:59:16
  -- NarcTransactionCatalog R/3 NarcProtocol on parent update restrict 
  -- ERWIN_RELATION:CHECKSUM="00010619", PARENT_OWNER="", PARENT_TABLE="NarcTransactionCatalog"
    -- CHILD_OWNER="", CHILD_TABLE="NarcProtocol"
    -- P2C_VERB_PHRASE="R/3", C2P_VERB_PHRASE="", 
    -- FK_CONSTRAINT="", FK_COLUMNS="CatalogID"
  WHEN(
    NOT (
        -- %JoinPKPK(inserted,deleted," = "," AND") 
        inserted.CatalogID = deleted.CatalogID
    ) AND
    EXISTS (
      SELECT * FROM NarcProtocol
      WHERE
        --  %JoinFKPK(NarcProtocol,deleted," = "," AND") 
        NarcProtocol.CatalogID = deleted.CatalogID
    ))
       (EXECUTE PROCEDURE erwin_raise_except(
         -746,
         'Cannot update NarcTransactionCatalog because NarcProtocol exists.'
       )) 


;


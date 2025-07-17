BEGIN WORK;

-- Altering column definitions
ALTER TABLE "informix".narctransactioncatalog
	ADD
	(
	customersupplierstreet character(30) default '' NOT NULL BEFORE customersuppliercity,
	username character(20) default '' NOT NULL BEFORE acknowledgementstatus,
	printeddate integer default 0 NOT NULL BEFORE acknowledgementstatus
	);

COMMIT WORK;


update narcnumber set nr_art = 'DNCFNO', nr_maximum = 999 where nr_art = 'SERVCH';
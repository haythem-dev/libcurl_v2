SYNONYME
select * from zdkbtm;
select * from zartwg;


select * from liefer;
select * from fliefer;
select * from bliefer;

select * from zartikel where artikel_nr = 15183;

select * from artikelzentral;
select * from artikelf;



SELECT b.btm_gruppe               AS group, 
               b.btm_bez                    AS name, 
               b.btm_gehalt                AS concentration, 
               b.btm_einheit                AS packageunit, 
               w.artikel_nr                  AS articleno, 
               z.artikel_name   AS articlename, 
               z.darreichform   AS articlepharmaform, 
               z.einheit            AS articlepackegeunit, 
               f.lagerfachnr      AS articlestocklocation 
FROM     zdkbtm b, zartwg w, zartikel z, artikelf f
WHERE z.artikel_nr = w.artikel_nr 
AND       z.artikel_nr = f.artikel_nr 
AND       w.warengruppe = b.btm_gruppe
AND       z.artikel_nr = '1983074'
AND       f.filialnr = '6'
AND       w.art = '6' ;



SELECT	b.btm_gruppe	AS group, 		b.btm_bez       AS name, 		b.btm_gehalt    AS concentration, 		b.btm_einheit   AS packageunit, 		w.artikel_nr    AS articleno, 		z.artikel_name  AS articlename, 		z.darreichform  AS articlepharmaform, 		z.einheit       AS articlepackegeunit, 		f.lagerfachnr   AS articlestocklocation FROM   zdkbtm b, zartwg w, zartikel z, artikelf f WHERE	z.artikel_nr	= w.artikel_nr AND    z.artikel_nr	= f.artikel_nr AND    w.warengruppe	= b.btm_gruppe AND    z.artikel_nr	= '1983074' AND    f.filialnr		= '6' AND    w.art			= '6';

SELECT catalogid, branchno, articleno, artikel_name AS articlename, darreichform AS pharmaform, einheit AS packegeunit, transactiondate, transactiontime, customersupplierno, transactiontype, purchaseorderno, qtytransaction, qtyinstock, DECODE(transactiontype, 
	'c',  (SELECT nameapo from kunde WHERE kunde.filialnr=branchno AND kunde.idfnr = customersupplierno ), 
	'b',  (SELECT lant_name from bliefer, liefer WHERE liefer.lant_nr=bliefer.lant_nr AND bliefer.besla_nr = customersupplierno ), ' ' ) AS customersuppliername 
FROM narctransactioncatalog, zartikel 
WHERE branchno='6' 
AND articleno=zartikel.artikel_nr 
AND articleno = '1983074' 
AND transactiondate BETWEEN '20060308' AND '20060310' 
AND transactiontype= 'c' 
AND deletedflag= '0' 
AND processedflag= '0' 
ORDER BY transactiondate desc, transactiontime desc;

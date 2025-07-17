select date(), * from narctransactioncatalog where articleno = 2156350  and branchno = 6;

select * from artikelkonto;
select dbinfo( "UTC_TO_DATETIME", artikelkonto.zeitunix ) from artikelkonto;



Bei den Bestandsdifferenzen wird mit die PZN 2156350 mit Menge 2 angezeigt. Hier hat meine Kollegin heute morgen schon den WE von gestern eingegeben. 
Als ich die Schaltfläche "Aktualisierung Bestandsdifferenzen" anklickte, verschwand diese Buchung. 
Danach wurde mir eine neue Differenz mit Menge -1 angezeigt: PZN 4603965. 
Beim Aufruf der Vergleichsbuchungen wurde mir auf BTM-Buch-Seite und Pharmos-Seite nur ein Wareneingang von 2 angezeigt. Abbuchungen fehlten. Im Nachhinein hat sich herausgestellt, dass dort Daten angezeigt wurden, die gar nicht zu dieser PZN gehörten (Überprüfung über BTM-Buch). Beim erneuten Aufruf der Bestandsdifferenzen war die schon korrigierte erste Bestandsdifferenz wieder zu sehen. Nach nochmaligem Klicken auf die Schaltfläche "Aktualisierung Bestandsdifferenzen" erscheint nun folgende Fehlermeldung:

select * from narcdiffqtyinstock where articleno = 2156350  and branchno = 6;
select * from narctransactioncatalog where articleno = 2156350  and branchno = 6;
select * from narctransactionoriginal where articleno = 2156350  and branchno = 6;


select bestand, * from artikellokal where artikel_nr = 2156350  and filialnr  = 6;


select * from narcdiffqtyinstock where articleno = 4603965 and branchno = 6;
select * from narcdiffqtyinstock where articleno = 4603965 and branchno = 6;


select bestand, * from artikellokal where artikel_nr = 1039470 and filialnr  = 6;
select * from narctransactioncatalog where articleno = 1039470 and branchno = 6;


select bestand, bestandreserviert,* from artikellokal where artikel_nr = 2195663 and filialnr  = 6;

select * from narctransactioncatalog where articleno = 2195663 and branchno = 6;
select * from narctransactionoriginal where articleno = 2195663 and branchno = 6;


select bestand, * from artikellokal where artikel_nr = 1039470 and filialnr  = 6;
select * from narctransactioncatalog where articleno = 1039470 and branchno = 6;

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
select * from narctransactioncatalog 
select * from narctransactioncatalog where branchno = 6 and articleno = 693262;

u p d a t e narctransactioncatalog set qtyinstock = qtyinstock+1 where branchno = 6 and articleno = 693262;

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Am 01.07.06 gibt es für die Stadpark Apotheke (BGA-Nr. 4276584) im BTM-Buch einen Abverkauf. 
Im Feld Bestell-/Auftragsnr. wird die Auftragsnummer 1285 angezeigt. 
Auf dem Originalbeleg, sowie in der AE und im PHARMOS ist diesem Auftrag jedoch die Auftragsnummer 1286 zugewiesen.

select * from narctransactioncatalog where customersupplierno = 4276584;

= Auftragsnummer 1285
select * from narctransactionoriginal where branchno = 6 and transactiondate = 20060701 and customersupplierno = 4276584;
u p d a t e narctransactionoriginal set purchaseorderno = 1286 where branchno = 6 and transactiondate = 20060701 and customersupplierno = 4276584 and purchaseorderno = 1285;


artno = 693262
select * from artikelkonto where filialnr = 6 and artikel_nr = 693262 and zeitunix between 1151708461 and 1151791199;
and belegnr = 1286;
and idfnr = 4276584;


select * from akdauftrag where filialnr = 6 and idfnr = 4276584 and kdauftragnrfiliale = 1285;
select * from akdauftrag where datum = 20060701 and kdauftragnr = 13640;
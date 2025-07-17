select count(*)  from zartikel;

select count(*)  from zartikel where btm = '1';

select count(*)  from beskopf;
select count(*)  from beskopf where status_akt = 'A';
select count(*)  from besposi;

select count(*) from beskopf k, besposi p, zartikel z where k.bestell_nr = p.bestell_nr and k.filialnr = p.filialnr and p.artikel_nr = z.artikel_nr and z.btm = 1 and k.status_akt = 'A';


select * from narcopenorders;
select count(*) from narcopenorders;
select * from narcopenorders where branchno in (6,8,9,31,33,40,46,56,59,70,78,81,82);
select count(*) from narcopenorders where branchno in (6,8,9,31,33,40,46,56,59,70,78,81,82);

select * from narcopenorders order by orderdate desc;
select * from narcopenorders order by orderdate desc;

delete from narcopenorders;
	
delete from narcopenorders where orderdate < 20040406;

select * from narcopenorders where branchno = 6;
select count(*) from narcopenorders where branchno = 6;

insert into narcopenorders
select k.filialnr, p.artikel_nr,  k.datum_bestell, k.bestell_nr, k.besla_nr, p. menge_bestell from beskopf k, besposi p, zartikel z where k.bestell_nr = p.bestell_nr and k.filialnr = p.filialnr and p.artikel_nr = z.artikel_nr and z.btm = 1 and k.status_akt = 'A';



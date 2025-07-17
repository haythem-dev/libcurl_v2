SELECT n1.branchno, n1.articleno, n1.creationdate, n1.diffqtyinstock
FROM   narcdiffqtyinstock n1 
WHERE  n1.branchno=6
AND    n1.articleno=1983074
AND    creationdate = (	select	max(creationdate) 
			from	narcdiffqtyinstock 
			WHERE	articleno = n1.articleno 
			and	branchno = n1.branchno);

select count(*) from narcdiffqtyinstock;

BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

SELECT (o1.qtyinstock-c1.qtyinstock) AS diff, o1.qtyinstock, c1.qtyinstock, *
FROM narctransactionoriginal AS o1, narctransactioncatalog AS c1
WHERE o1.articleno=c1.articleno
and o1.qtyinstock <> c1.qtyinstock
and o1.articleno=15237
and o1.transactiontime = ( select max(o2.transactiontime) from narctransactionoriginal o2 where o2.transactiondate = (select max(o3.transactiondate) from narctransactionoriginal o3 where o1.articleno = o3.articleno))
and c1.transactiontime = ( select max(c2.transactiontime) from narctransactioncatalog c2 where c2.transactiondate = (select max(c3.transactiondate) from narctransactioncatalog c3 where c1.articleno = c3.articleno));



BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

select * from narctransactionoriginal order by articleno;
select * from narctransactioncatalog order by articleno;

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
select (o1.qtyinstock - c1.qtyinstock) as diff , o1.qtyinstock,  c1.qtyinstock, o1.articleno, *
from narctransactionoriginal  o1, narctransactioncatalog  c1 
where o1.articleno = c1.articleno
and o1.qtyinstock != c1.qtyinstock
and o1.transactiontime = (	select max( o2.transactiontime) 
			from narctransactionoriginal o2
			where  o1.articleno = o2.articleno and o2.transactiondate = (	select max( o3.transactiondate ) 
									from narctransactionoriginal  o3 
									where o3.articleno = o1.articleno) ) 
and c1.transactiontime = (	select max( c2.transactiontime) 
			from narctransactioncatalog c2
			where  c1.articleno = c2.articleno and c2.transactiondate = (	select max( c3.transactiondate ) 
									from narctransactioncatalog c3  ) ) ;



select *
from narctransactionoriginal  o1 
where o1.transactiontime = (	select max(transactiontime) 
			from narctransactionoriginal o2
			where  o2.transactiondate = (	select max(transactiondate) 
									from narctransactionoriginal  ) ) ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Treffer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
select * from narcdiffqtyinstock;
delete from narcdiffqtyinstock;

insert into narcdiffqtyinstock ( branchno, articleno, creationdate )
select o.branchno, o.articleno, o.articleno 
from narctransactionoriginal o , narctransactioncatalog c 
where (	select oo.qtyinstock 
	from narctransactionoriginal oo 
	where transactiontime = (select max(transactiontime) from narctransactionoriginal where transactiondate = (select max(transactiondate) from narctransactionoriginal where articleno = o.articleno ) ) ) == o.qtyinstock and 
	(select cc.qtyinstock 
	from narctransactioncatalog cc 
	where transactiontime = (select max(transactiontime) from narctransactioncatalog where transactiondate = (select max(transactiondate) from narctransactioncatalog where articleno = c.articleno ) )) == c.qtyinstock 
	and o.qtyinstock != c.qtyinstock;


insert into narcdiffqtyinstock ( branchno, articleno, creationdate ) select o.branchno, o.articleno, o.articleno from narctransactionoriginal o , narctransactioncatalog c where (	select oo.qtyinstock 	from narctransactionoriginal oo 	where transactiontime = (select max(transactiontime) from narctransactionoriginal where transactiondate = (select max(transactiondate) from narctransactionoriginal where articleno = o.articleno ) ) ) == o.qtyinstock and 	(select cc.qtyinstock 	from narctransactioncatalog cc 	where transactiontime = (select max(transactiontime) from narctransactioncatalog where transactiondate = (select max(transactiondate) from narctransactioncatalog where articleno = c.articleno ) )) == c.qtyinstock 	and o.qtyinstock != c.qtyinstock;

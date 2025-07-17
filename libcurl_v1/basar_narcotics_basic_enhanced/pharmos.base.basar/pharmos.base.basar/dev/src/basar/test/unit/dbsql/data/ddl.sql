DROP TABLE     IF EXISTS but_master;
DROP TABLE     IF EXISTS but_detail;
DROP TABLE     IF EXISTS but_var;

DROP PROCEDURE IF EXISTS but_proc0;
DROP PROCEDURE IF EXISTS but_proc1;
DROP FUNCTION  IF EXISTS but_func0;
DROP FUNCTION  IF EXISTS but_func1;
DROP FUNCTION  IF EXISTS but_func2;
DROP FUNCTION  IF EXISTS but_func3;


CREATE TABLE but_master 
(
	colser		serial																							NOT NULL,
	colint		integer							DEFAULT 0 														NOT NULL,
	colsmint	smallint						DEFAULT 0 														NOT NULL PRIMARY KEY,
	colfl		float							DEFAULT 0.0000000000000000 										NOT NULL,
	colsmfl		smallfloat						DEFAULT 0.00000000 												NOT NULL,
	coldec		decimal(9,2)					DEFAULT 0.00 													NOT NULL,
	colmon		money(9,2)						DEFAULT 0.00 													NOT NULL,
	colch1		character(1)					DEFAULT '' 														NOT NULL,
	colch2		character(10)					DEFAULT '' 														NOT NULL,
	colvch1		varchar(1,0)					DEFAULT '' 														NOT NULL,
	colvch2		varchar(10,0)					DEFAULT '' 														NOT NULL,
	colvch3		varchar(254,0)					DEFAULT ''														NOT NULL,
	colvch4		varchar(255,0)					DEFAULT ''														NOT NULL,
	collvch		lvarchar						DEFAULT ''														NOT NULL,
	coldate		date							DEFAULT '12/31/9999'											NOT NULL,
	coldt1		datetime YEAR TO SECOND			DEFAULT datetime (9999-12-31 23:59:59    ) YEAR TO SECOND		NOT NULL,
	coldt2		datetime YEAR TO FRACTION(3)	DEFAULT datetime (9999-12-31 23:59:59.999) YEAR TO FRACTION(3)	NOT NULL,
	coltime1	datetime HOUR TO SECOND			DEFAULT datetime (           23:59:59    ) HOUR TO SECOND		NOT NULL,
	coltime2	datetime HOUR TO FRACTION(3)	DEFAULT datetime (           23:59:59.999) HOUR TO FRACTION(3)	NOT NULL,
	colspan1    interval DAY(9)  TO FRACTION(3) DEFAULT interval (999999999  23:59:59.999) DAY(9) TO FRACTION(3) NOT NULL
)
IN dbsdata
EXTENT SIZE 1000 NEXT SIZE 330
LOCK MODE ROW;

CREATE UNIQUE INDEX xpkbut_master ON but_master 
(
	colser 
) 
in dbsindex;
CREATE INDEX xie1but_master ON but_master
(
	colint , 
	colsmint
)
in dbsindex;
CREATE INDEX xie2but_master ON but_master
(
	colvch3
)
in dbsindex;

CREATE TABLE but_detail 
(
	colser		serial																							NOT NULL,
	colint		integer							DEFAULT 0 														NOT NULL,
	colsmint	smallint						DEFAULT 0 														NOT NULL,
	colfl		float							DEFAULT 0.0000000000000000 										NOT NULL,
	colsmfl		smallfloat						DEFAULT 0.00000000 												NOT NULL,
	coldec		decimal(9,2)					DEFAULT 0.00 													NOT NULL,
	colmon		money(9,2)						DEFAULT 0.00 													NOT NULL,
	colch1		character(1)					DEFAULT '' 														NOT NULL,
	colch2		character(10)					DEFAULT '' 														NOT NULL,
	colvch1		varchar(1,0)					DEFAULT '' 														NOT NULL,
	colvch2		varchar(10,0)					DEFAULT '' 														NOT NULL,
	colvch3		varchar(254,0)					DEFAULT ''														NOT NULL,
	colvch4		varchar(255,0)					DEFAULT ''														NOT NULL,
	collvch		lvarchar						DEFAULT ''														NOT NULL,
	coldate		date							DEFAULT '12/31/9999'											NOT NULL,
	coldt1		datetime YEAR TO SECOND			DEFAULT datetime (9999-12-31 23:59:59    ) YEAR TO SECOND		NOT NULL,
	coldt2		datetime YEAR TO FRACTION(3)	DEFAULT datetime (9999-12-31 23:59:59.999) YEAR TO FRACTION(3)	NOT NULL,
	coltime1	datetime HOUR TO SECOND			DEFAULT datetime (           23:59:59    ) HOUR TO SECOND		NOT NULL,
	coltime2	datetime HOUR TO FRACTION(3)	DEFAULT datetime (           23:59:59.999) HOUR TO FRACTION(3)	NOT NULL,
	colspan1    interval DAY(9)  TO FRACTION(3) DEFAULT interval (999999999  23:59:59.999) DAY(9) TO FRACTION(3) NOT NULL
)
IN dbsdata
EXTENT SIZE 1000 NEXT SIZE 330
LOCK MODE ROW;

CREATE UNIQUE INDEX xpkbut_detail ON but_detail 
(
	colser 
) 
in dbsindex;
CREATE INDEX xie1but_detail ON but_detail
(
	colint , 
	colsmint
)
in dbsindex;
CREATE INDEX xie2but_detail ON but_detail
(
	colvch3
)
in dbsindex;

CREATE TABLE but_var 
(
	colser		serial																							 NOT NULL,
	colint		integer							DEFAULT 0 														 NOT NULL,
	colsmint	smallint						DEFAULT 0 														 NOT NULL,
	colfl		float							DEFAULT 0.0000000000000000 										 NOT NULL,
	colsmfl		smallfloat						DEFAULT 0.00000000 												 NOT NULL,
	coldec		decimal(9,2)					DEFAULT 0.00 													 NOT NULL,
	colmon		money(9,2)						DEFAULT 0.00 													 NOT NULL,
	colch1		character(1)					DEFAULT '' 														 NOT NULL,
	colch2		character(10)					DEFAULT '' 														 NOT NULL,
	colvch1		varchar(1,0)					DEFAULT '' 														 NOT NULL,
	colvch2		varchar(10,0)					DEFAULT '' 														 NOT NULL,
	colvch3		varchar(254,0)					DEFAULT ''														 NOT NULL,
	colvch4		varchar(255,0)					DEFAULT ''														 NOT NULL,
	collvch		lvarchar						DEFAULT ''														 NOT NULL,
	coldate		date							DEFAULT '12/31/9999'											 NOT NULL,
	coldt1		datetime YEAR TO SECOND			DEFAULT datetime (9999-12-31 23:59:59    ) YEAR TO SECOND		 NOT NULL,
	coldt2		datetime YEAR TO FRACTION(3)	DEFAULT datetime (9999-12-31 23:59:59.999) YEAR TO FRACTION(3)	 NOT NULL,
	coltime1	datetime HOUR TO SECOND			DEFAULT datetime (           23:59:59    ) HOUR TO SECOND		 NOT NULL,
	coltime2	datetime HOUR TO FRACTION(3)	DEFAULT datetime (           23:59:59.999) HOUR TO FRACTION(3)	 NOT NULL,
	colspan1    interval DAY(9)  TO FRACTION(3) DEFAULT interval (999999999  23:59:59.999) DAY(9) TO FRACTION(3) NOT NULL
)
IN dbsdata
EXTENT SIZE 1000 NEXT SIZE 330
LOCK MODE ROW;

CREATE UNIQUE INDEX xpkbut_var ON but_var 
(
	colser 
) 
in dbsindex;
CREATE INDEX xie1but_var ON but_var
(
	colint , 
	colsmint
)
in dbsindex;
CREATE INDEX xie2but_var ON but_var
(
	colvch3
)
in dbsindex;

create procedure but_proc0()
	define a integer;
	let a = 10;
end procedure
document "used by basar unit test; created by m.eichenlaub 2008-05-21";

-----------------------------------------------------------------------
create procedure but_proc1(id int)
	define a integer;
	let a = 10 * id;
end procedure
document "used by basar unit test; created by m.eichenlaub 2008-05-21";

-----------------------------------------------------------------------
create function but_func0() 
	returning integer;
	return 10;
end function
document "used by basar unit test; created by m.eichenlaub 2008-05-21";

-----------------------------------------------------------------------
create function but_func1() 
	returning integer as ret0;
	return 20;
end function
document "used by basar unit test; created by m.eichenlaub 2008-05-21";

-----------------------------------------------------------------------
create function but_func2(id int) 
	returning integer as ret0,
				integer as ret1;
	return id, 4* id;
end function
document "used by basar unit test; created by m.eichenlaub 2008-05-21";

-----------------------------------------------------------------------
create function but_func3(id int) 
	returning integer  as ret0,
				integer  as ret1,
				smallint as ret2;
	define col0 like but_master.colser;
	define col1 like but_master.colint;
	define col2 like but_master.colsmint;
	foreach
		select colser, colint + id, colsmint - id
		into col0, col1, col2 
		from but_master
		order by 1
		return col0, col1, col2 with resume;
end foreach;
end function
document "used by basar unit test; created by m.eichenlaub 2008-05-21";


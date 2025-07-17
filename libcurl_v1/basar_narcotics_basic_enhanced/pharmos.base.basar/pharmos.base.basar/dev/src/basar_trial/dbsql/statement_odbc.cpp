
#include <libbasardbsql.h>
#include "testdbsql.h"

#include <libbasarcmnutil_decimal.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_datetime.h>

#include <stdio.h>

using basar::Int16;
using basar::Int32;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::ConstString;
using basar::UInt32;
using basar::VarString;

//------------------------------------------------------------------------------
//	ODBC
//------------------------------------------------------------------------------

namespace __statement_odbc__
{

//------------------------------------------------------------------------------
void test_statement_execimmodbc(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execimmodbc: --------------------\n");
	printf("\n");

	StatementRef stmtRef = connRef.createStatement();

	Int32 iRows;

	//iRows = stmtRef.execute("begin work;");
	//printf("begin work       : rows %d\n", iRows);
	connRef.begin();
	iRows = stmtRef.execute("update ppuser set activ = 1 "
				   			"where userid > 250").m_affectedRows;
	printf("update ppuser(1): rows %d\n", iRows);
	connRef.commit();
	//iRows = stmtRef.execute("commit;"); //no combination exeimm <-> qt possible!
	

	connRef.begin();
	iRows = stmtRef.execute("update ppuser set activ = 0 "
				   	       "where userid = 260").m_affectedRows;
	printf("update ppuser(2): rows %d\n", iRows);

	connRef.rollback();
}

//------------------------------------------------------------------------------
void test_statement_execsingleodbc(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execsingleodbc: ------------\n");
	printf("\n");

	ConstString stmt = "select username, activ from ppuser where userid = 261";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value activ : %d\n", resRef.getInt32("activ"));
		printf("    value username : %s\n", resRef.getString("username"));
	}

	resRef .close();
	stmtRef.close();
	//connRef.close();

	resRef = stmtRef.executeSingleQuery(stmt);

	cnt = 0;

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());
		
		printf("    value activ    : %d\n", resRef.getInt16(1));
		printf("    value username : %s\n" , resRef.getString(0));
	}
}


//------------------------------------------------------------------------------
void test_statement_execsingleodbc2		(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execsingleodbc2: ------------\n");
	printf("\n");

	ConstString stmt = "select artikel_nr, festbetrag1, rabattfaehig, privatsortiment, artikel_name  from zartikel where artikel_nr = 398008";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value artikel_nr : %d\n", resRef.getInt32("artikel_nr"));
		printf("    value festbetrag1 : %s\n", resRef.getDecimal("festbetrag1").toString().c_str());
		printf("    value rabattfaehig : %s\n", resRef.getString(2));
		printf("    value privatsortiment : %s\n", resRef.getString("privatsortiment"));
		printf("    value artikel_name : %s\n", resRef.getString(4));
	}

	resRef .close();
	stmtRef.close();


	ConstString stmt2 = "select klassen_nr, reihenfolge, param_aktiv, faktor, vwmax, filialnr from klasse where klassen_nr = 5016 and filialnr = 51";

	StatementRef stmtRef2 = connRef.createStatement();
	ResultsetRef resRef2  = stmtRef2.executeSingleQuery(stmt2);

	cnt = 0;
	
	printf("\nstatement       : <%s>\n\n", stmt2);
	printf("selected columns: %d\n", resRef2.getSelectedCols());

	while (true == resRef2.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef2.getFetchedRows());

		printf("    value klassen_nr : %d\n", resRef2.getInt32(0));
		printf("    value reihenfolge : %d\n", resRef2.getInt16("reihenfolge"));
		printf("    value param_aktiv : %s\n", resRef2.getString(2));
		printf("    value faktor : %s\n", resRef2.getDecimal(3).toString().c_str());
		printf("    value vwmax : %f\n", resRef2.getFloat32(4));
		printf("    value filialnr : %d\n", resRef2.getInt16(5));
	}

	resRef2 .close();
	stmtRef2.close();
}

//------------------------------------------------------------------------------
void test_statement_execmultiodbc		(ConnectionRef connRef2)
{
	printf("\n\n");
	printf("--- test_statement_execmultiodbc: ------------\n");
	printf("\n");

	ConstString stmt = "select first 50 u.username as name, u.activ, g.groupid from ppuser u, ppcorappusergroup g "
						"where u.userid = g.userid order by 2";

	StatementRef stmtRef = connRef2.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value groupid : %d\n", resRef.getInt32(2));
		printf("    value activ : %d\n", resRef.getInt32("activ"));
		printf("    value username : %s\n\n", resRef.getString("name"));
	}

	try
	{
		resRef.reopen();
	}
	catch ( const basar::Exception & except )
	{
		printf("\n%s\n", except.what().c_str());
		printf("\nODBC reopen not implemented --> correctly thrown exception\n");
	}

	cnt = 0;

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());
		
		printf("    value groupid : %d\n", resRef.getInt32("groupid"));
		printf("    value activ : %d\n", resRef.getInt16(1));
		printf("    value username : %s\n\n", resRef.getString(0));
	}

	resRef .close();
	stmtRef.close();
}

//------------------------------------------------------------------------------
void test_statement_execsingleodbc3		(ConnectionRef connRef2)
{
	printf("\n\n");
	printf("--- test_statement_execsingleodbc3 -----------\n");
	printf("\n");

	ConstString stmt =	"select count(*) as count ,min(userid) as min, sum(groupid) as sum from ppcorappusergroup "
						"where groupid = 15"; //!< doesn't work without alias !!!

	StatementRef stmtRef = connRef2.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);

	Int32  cnt = 0;
	
	printf("\nstatement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value count(*)          : %f\n"  , resRef.getDecimal("count").toFloat64());
		printf("    value count(*)          : <%s>\n", resRef.getDecimal("count").toString (2).c_str());
		printf("    value 1 min             : %d\n"  , resRef.getInt32  ("min"));
		printf("    value 2 sum             : %f\n"  , resRef.getDecimal(2).toFloat64());
	}

//---
	ConstString stmt2 =	"select max(userid) "//count(*) "
						//"as count "
						"from ppcorappusergroup "
						"where groupid = 15";

	basar::Decimal d = stmtRef.executeAggregate(stmt2);

	printf("\n\nstatement       : <%s>\n\n", stmt2);
	printf("    value count(*)  : %f\n", d.toFloat64());
//---
}

//------------------------------------------------------------------------------
void test_statement_execnestedodbc		(ConnectionRef connRef1, ConnectionRef connRef2)
{
	printf("\n");
	printf("--- test_statement_execnestedodbc ---------------\n");
	printf("\n");

	ConstString stmt1 = "select first 10 artikel_mc from zartikel ";
	ConstString stmt2 = "select first 10 username, surname, activ from ppuser ";

	StatementRef stmtRef1 = connRef1.createStatement();
	StatementRef stmtRef2 = connRef2.createStatement();
	ResultsetRef resRef1  = stmtRef1.executeQuery(stmt1);
	ResultsetRef resRef2  = stmtRef2.executeQuery(stmt2);

	UInt32 cnt = 0;

	printf("statement1       : <%s>\n\n", stmt1);
	printf("selected columns : %d\n", resRef1.getSelectedCols());
	printf("statement2       : <%s>\n\n", stmt2);
	printf("selected columns : %d\n", resRef2.getSelectedCols());

	bool more = true;

	while (true == more)
	{
		more = resRef1.next();
		more = more && resRef2.next();

		if (true == more)
		{
			++cnt;
			printf("fetched rows    : %d\n", cnt);

			printf("    value artikel_mc   : %s\n"    , resRef1.getString   ("artikel_mc"  ));
			printf("    value username	   : %s\n"    , resRef2.getString	("username" ));
			printf("    value activ		   : <%d>\n"  , resRef2.getInt16    ("activ"));
		}
	}

	printf("\n\n");

	resRef1 .close();
	stmtRef1.close();
	resRef2 .close();
	stmtRef2.close();

	printf("summarized fetch total time in DB library %d ms\n", basar::db::sql::getDbLibFetchTime());
	printf("fetched: %d rows\n", cnt);

	printf("\n\n");
}

//------------------------------------------------------------------------------
void test_statement_execimmparamodbc		(ConnectionRef connRef1, ConnectionRef connRef2)
{
	printf("\n");
	printf("--- test_statement_execimmparamodbc: --------------------\n");
	printf("\n");

	Int32 iRows;
	ConstString stmt =	"update ppuser set changepw = 1 where userid > ? "; //10000
	
	PreparedStatementRef stmtRef = connRef1.createPreparedStatement(stmt);


	stmtRef.setInt32(0,10000);
	iRows  = stmtRef.execute().m_affectedRows;

	printf("update ppuser(1): rows %d\n", iRows);


	ConstString stmt2 =	"update dttest set date = ?, dtim = ?, dtim3 = ? where nr = 2 ";
	stmtRef = connRef2.createPreparedStatement(stmt2);
	basar::Date dt;
	basar::DateTime d1(2005, 11, 13);
	Int32 d = 20050201,
		  t = 121314567;
	basar::DateTime d2(d, t);

	stmtRef.setDate(0, dt);
	stmtRef.setDateTime(1, d1);
	stmtRef.setDateTime(2, d2);
	iRows  = stmtRef.execute().m_affectedRows;

	printf("update dttest(1): rows %d\n", iRows);
}

//------------------------------------------------------------------------------
void test_statement_execsingleparamodbc		(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execsingleparamodbc: ------------\n");
	printf("\n");

	ConstString stmt =	"select username from ppuser "
						"where userid = ? "; //261
	
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	stmtRef.setInt32(0,261);
	ResultsetRef resRef  = stmtRef.executeSingleQuery();


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value username : %s\n", resRef.getString("username"));
	}
}

//------------------------------------------------------------------------------
void test_statement_execmultiparamodbc		(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execmultiparamodbc: ------------\n");
	printf("\n");

	ConstString stmt =	"select first 50 userid, username, activ from ppuser "
						"where activ = ? and userid > ? "; //1, 250
	
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	stmtRef.setInt32(1,250);
	stmtRef.setInt16(0,1);

	ResultsetRef resRef  = stmtRef.executeQuery();


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value username : %s\n", resRef.getString("username"));
		printf("    value userid : %d\n", resRef.getInt32(0));
		printf("    value activ : %d\n", resRef.getInt16("activ"));
	}
}

//------------------------------------------------------------------------------
void test_statement_lvarcharodbc(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_lvarcharodbc: --------------------\n");
	printf("\n");

	ConstString stmt = "select branchno, information from branchinfo where branchno = 51"; //lvarchar
	//ConstString stmt = "select bemerkungen, kdauftragnr from akdauftrag where kdauftragnr = 53553"; //varchar

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    information : %s\n", resRef.getString("information"));
		printf("    branchno    : %d\n", resRef.getInt16("branchno"));
	}
}

//------------------------------------------------------------------------------
void test_statement_dateodbc(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_dateodbc: --------------------\n");
	printf("\n");

	ConstString stmt = "select dtim2, nr, date from dttest where nr = 12"; 
	
	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    date       : %ld\n", resRef.getDate("date").getDate());
		printf("    dtim-date  : %ld\n", resRef.getDateTime("dtim2").getDate());
		printf("    dtim-time  : %ld\n", resRef.getDateTime("dtim2").getTime());
		printf("    nr         : %d\n", resRef.getInt16("nr"));
	}
}

//------------------------------------------------------------------------------
void test_statement_dateodbc_ext(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_dateodbc: --------------------\n");
	printf("\n");

	ConstString stmt = "select l.dtim2 , l.dtim3, l.dtim4, l.nr, l.date, l.name, "
		               "r.dtim2 as r1, r.dtim3 as r3, r.dtim4 as r4, r.nr as r5, r.date as r6, r.name as r7 "
		               "from dttest l, outer dttest r "
					   "where l.nr not in (12, 5) and r.nr = 5 and l.nr=r.nr"; 
	
	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    date       : %ld\n", resRef.getDate("date").getDate());
		printf("    dtim-date  : %ld\n", resRef.getDateTime("dtim2").getDate());
		printf("    dtim-time  : %ld\n", resRef.getDateTime("dtim2").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDate    ("dtim3").getDate());
		//printf("    dtim-time  : %ld\n", resRef.getDate    ("dtim3").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDateTime("dtim4").getDate());
		printf("    dtim-time  : %ld\n", resRef.getDateTime("dtim4").getTime());
		printf("    nr         : %d\n", resRef.getInt16("nr"));
		printf("    name       : %s\n", resRef.getString("name"));

		printf("    date       : %ld\n", resRef.getDate("r6").getDate());
		printf("    dtim-date  : %ld\n", resRef.getDateTime("r1").getDate());
		printf("    dtim-time  : %ld\n", resRef.getDateTime("r1").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDate    ("r3").getDate());
		//printf("    dtim-time  : %ld\n", resRef.getDate    ("dtim3").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDateTime("r4").getDate());
		printf("    dtim-time  : %ld\n", resRef.getDateTime("r4").getTime());
		printf("    nr         : %d\n", resRef.getInt16("r5"));
		printf("    name       : %s\n", resRef.getString("r7"));

	}
}

//------------------------------------------------------------------------------
void prepinsert_odbc(ConnectionRef connRef)
{
	printf("\n");
	printf("--- prepinsert_odbc --------------------\n");
	printf("\n");

	Int32 iRows;
	ConstString stmt =	"insert into dttest (nr, date, dtim2, dtim3, dtim4, name) values (?, ?, ?, ?, ?, ?)";
	
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);

	using namespace basar;

	stmtRef.setInt16   (0, 2003);
	stmtRef.setDate    (1, Date(1964, 1, 28));
	stmtRef.setDateTime(2, DateTime(19680708, 103456789));
	stmtRef.setDateTime(3, DateTime(19690809, 114426123));
	stmtRef.setDateTime(4, DateTime());//20690809, 174506765));
	stmtRef.setString  (5, "xyzxxx");

	iRows  = stmtRef.execute().m_affectedRows;

	stmtRef.setInt16   (0, 2004);
	stmtRef.setDate    (1, Date());
	stmtRef.setDateTime(2, DateTime());

	iRows  += stmtRef.execute().m_affectedRows;

	printf("insert into dttest: rows %d\n", iRows);
}

//------------------------------------------------------------------------------
void test_statement_date_ext(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_date: --------------------\n");
	printf("\n");

	ConstString stmt = "select l.dtim3 , l.dtim, l.dtim5, l.nr, l.date, l.name, l.varname, l.lvarname, l.price, "
		               "r.dtim3 as r1, r.dtim as r3, r.dtim5 as r4, r.nr as r5, r.date as r6, r.name as r7, r.varname as r9,r.lvarname as r10, r.price as r8 "
		               "from dttest l, outer dttest r "
					   "where l.nr not in (12220) and r.nr = 5 and l.nr=r.nr"; 
	
	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    nr         : %ld\n", resRef.getInt16   ("nr"   ));
		printf("    name       : <%s>\n" , resRef.getString("name"));
		printf("    varname    : <%s>\n" , resRef.getString("varname"));
		printf("    lvarname   : <%s>\n" , resRef.getString("lvarname"));
		printf("    date       : %ld\n", resRef.getDate    ("date" ).getDate());
		printf("    price      : %f\n" , resRef.getDecimal ("price" ).toFloat64());

		printf("    dtim-date 3: %ld\n", resRef.getDateTime("dtim3").getDate());
		printf("    dtim-time 3: %ld\n", resRef.getDateTime("dtim3").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDate    ("dtim").getDate());
		//printf("    dtim-time  : %ld\n", resRef.getDate    ("dtim").getTime());
		printf("    dtim-date 5: %ld\n", resRef.getDateTime("dtim5").getDate());
		printf("    dtim-time 5: %ld\n", resRef.getDateTime("dtim5").getTime());


		printf("    nr         : %d\n",  resRef.getInt16   ("r5"));
		printf("    name       : <%s>\n", resRef.getString  ("r7"));
		printf("    varname    : <%s>\n", resRef.getString  ("r9"));
		printf("    lvarname   : <%s>\n", resRef.getString("r10"));
		printf("    date       : %ld\n", resRef.getDate    ("r6").getDate());
		printf("    price      : %f\n",  resRef.getDecimal ("r8" ).toFloat64());

		printf("    dtim-date 3: %ld\n", resRef.getDateTime("r1").getDate());
		printf("    dtim-time 3: %ld\n", resRef.getDateTime("r1").getTime());
		printf("    dtim-date  : %ld\n", resRef.getDate    ("r3").getDate());
		//printf("    dtim-time  : %ld\n", resRef.getDate    ("r3").getTime());
		printf("    dtim-date 5: %ld\n", resRef.getDateTime("r4").getDate());
		printf("    dtim-time 5: %ld\n", resRef.getDateTime("r4").getTime());
	}
}


//------------------------------------------------------------------------------
void test_statement_date_ext2(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_date: --------------------\n");
	printf("\n");

	{
		ConstString stmt = "select first 2 l.nr, l.name from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement: <%s>\n\n", stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}

	{
		ConstString stmt = "select first 2 l.nr, l.varname from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement: <%s>\n\n", stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}
	
	{
		ConstString stmt = "select first 2 l.nr, l.lvarname from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement: <%s>\n\n", stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}
}


//------------------------------------------------------------------------------
void statementodbc(ConnectionRef connRef1, 
			       ConnectionRef connRef2)
{
#ifndef X1234
	test_statement_execimmodbc			(connRef2);
	test_statement_execsingleodbc		(connRef2);
	test_statement_execsingleodbc2		(connRef1);
	test_statement_execmultiodbc		(connRef2);
	test_statement_execsingleodbc3		(connRef2);
	
	test_statement_execnestedodbc		(connRef1, connRef2);

	test_statement_execimmparamodbc		(connRef2, connRef1);
	test_statement_execsingleparamodbc	(connRef2);
	test_statement_execmultiparamodbc	(connRef2);

	{
		basar::db::sql::DatabaseInfo  dbInfo;

		dbInfo.dbServer     = "sofumi1";
		dbInfo.database     = "sofumi1_pos1g";		
		dbInfo.eConnectType = basar::ODBC;

		ConnectionRef connRefPos1g = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

		test_statement_lvarcharodbc (connRefPos1g); //pos1g

		connRefPos1g.close();
	}

	test_statement_dateodbc (connRef1);

	prepinsert_odbc(connRef1);
	test_statement_date_ext(connRef1);
	test_statement_date_ext2(connRef1);
#else
#endif
}

}	// namespace

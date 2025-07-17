
#include <libbasardbsql.h>
#include "testdbsql.h"

#include <libbasarcmnutil_decimal.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_datetime.h>

#include <stdio.h>

using basar::Int8;
using basar::Int16;
using basar::Int32;
using basar::Decimal;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::ConstString;
using basar::UInt32;
using basar::VarString;

#include <esqlc.h>

namespace __statement_infx__
{

//------------------------------------------------------------------------------
//	INFX
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void test_statement_execimminfx(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execimminfx: --------------------\n");
	printf("\n");

	StatementRef stmtRef = connRef.createStatement();

	basar::BULong iRows;

	iRows = stmtRef.execute("begin work;").m_affectedRows;
	printf("begin work       : rows %d\n", iRows);

	iRows = stmtRef.execute("update kddispo set version_row = version_row + 1 "
				   			"where vertriebszentrumnr = 9").m_affectedRows;
							//and kddisponr = 294968");

	printf("update kddispo(1): rows %d\n", iRows);
	//stmtRef.execute("rollback work");
	connRef.commit();

	connRef.begin();
	iRows = stmtRef.execute("update kddispo set version_row = version_row + 1022 "
				   	       "where vertriebszentrumnr = 9").m_affectedRows;
	printf("update kddispo(2): rows %d\n", iRows);
	connRef.rollback();
}


//------------------------------------------------------------------------------
void test_statement_execquery(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execquery ------------------\n");
	printf("\n");

	ConstString stmt =  // before and after "select" are TABs!
					   "	select	k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez, "
		               "t.kddisponr as kdtext_kddisponr, t.vertriebszentrumnr, t.text_kz, t.text "
		               "from kddispo k left outer join kdtext t "
		               "on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		               "where k.vertriebszentrumnr = 9 "
					   "order by k.kddisponr desc";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	/*while*/ if (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value kddispo_kddisponr : %d\n"   , resRef.getInt32   ("kddispo_kddisponr" ));
		printf("    value version_row       : %d\n"   , resRef.getInt32   ("version_row"       ));
		printf("    value preisekapo        : %f\n"    , resRef.getDecimal ("preisekapo"        ).toFloat64());
		printf("    value preisekapo        : <%s>\n"  , resRef.getDecimal ("preisekapo"        ).toString ().c_str());
		printf("    value artikel_bez       : <%s>\n"  , resRef.getString  ("artikel_bez"       ));
		printf("    value kdtext_kddisponr  : %d\n"   , resRef.getInt32   ("kdtext_kddisponr"  ));
		printf("    value vertriebszentrumnr: %d\n"   , resRef.getInt16   ("vertriebszentrumnr"));
		printf("    value text_kz           : <%s>\n"  , resRef.getString  ("text_kz"           ));
		printf("    value text              : <%s>\n"  , resRef.getString  ("text"              ));
	}

	printf("\n\n");

	resRef.reopen();

	cnt = 0;

	while (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value 0 : %d\n"  , resRef.getInt32   (0));
		printf("    value 1 : %d\n"  , resRef.getInt32   (1));
		printf("    value 2 : %f\n"   , resRef.getDecimal (2).toFloat64());
		printf("    value 2 : <%s>\n" , resRef.getDecimal (2).toString ().c_str());
		printf("    value 3 : <%s>\n" , resRef.getString  (3));
		printf("    value 4 : %d\n"  , resRef.getInt32   (4));
		printf("    value 5 : %d\n"  , resRef.getInt16   (5));
		printf("    value 6 : <%s>\n" , resRef.getString  (6));
		printf("    value 7 : <%s>\n" , resRef.getString  (7));

	}

	printf("summarized fetch total time in DB library %d ms\n", basar::db::sql::getDbLibFetchTime());
	printf("fetched: %d rows\n", resRef.getFetchedRows());

	printf("\n\n");

}

//------------------------------------------------------------------------------
void test_statement_execquery2(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execquery2 infx: -----------------\n");
	printf("\n");

	ConstString stmt = 
					   "select k.* "
		               "from kddispo k left outer join kdtext t "
		               "on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		               "where k.vertriebszentrumnr = 9 "
					   "order by k.kddisponr desc";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	/*while*/ if (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value kddispo_kddisponr : %d\n"   , resRef.getInt32   ("kddisponr"  ));
		printf("    value version_row       : %d\n"   , resRef.getInt32   ("version_row"));
		printf("    value preisekapo        : %f\n"    , resRef.getDecimal ("preisekapo"        ).toFloat64());
		printf("    value preisekapo        : <%s>\n"  , resRef.getDecimal ("preisekapo"        ).toString ().c_str());
		printf("    value artikel_bez       : %s\n"    , resRef.getString  ("artikel_bez"));
	}

	printf("\n\n");


	resRef.reopen();

	cnt = 0;

	while (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value 0 : %d\n"  , resRef.getInt32   (0));
		printf("    value 3 : %d\n"  , resRef.getInt32   (2));
		printf("    value 8 : %f\n"   , resRef.getDecimal (8).toFloat64());
		printf("    value 8 : <%s>\n" , resRef.getDecimal (8).toString ().c_str());
		printf("    value 5 : %s\n"   , resRef.getString  (5));
	}

	printf("\n\n");

	resRef .close();
	stmtRef.close();
}

//------------------------------------------------------------------------------
void test_statement_execquery3(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execquery3 infx: ------------------\n");
	printf("\n");

	ConstString stmt = 
					   "select k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez, "
		               "t.kddisponr as kdtext_kddisponr, t.vertriebszentrumnr, t.text_kz, t.text "
		               "from kdtext t left outer join kddispo k "
		               "on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		               "where t.vertriebszentrumnr = 9 "
					   "order by t.kddisponr desc";
	
	connRef.setRawFetching(true);

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value kddispo_kddisponr : %d\n"   , resRef.getInt32   ("kddispo_kddisponr" ));
		printf("    value version_row       : %d\n"   , resRef.getInt32   ("version_row"       ));
		printf("    value preisekapo        : %f\n"    , resRef.getDecimal ("preisekapo"        ).toFloat64());
		printf("    value preisekapo        : <%s>\n"  , resRef.getDecimal ("preisekapo"        ).toString ().c_str());
		printf("    value artikel_bez       : <%s>\n"  , resRef.getString  ("artikel_bez"       ));
		printf("    value kdtext_kddisponr  : %d\n"   , resRef.getInt32   ("kdtext_kddisponr"  ));
		printf("    value vertriebszentrumnr: %d\n"   , resRef.getInt16   ("vertriebszentrumnr"));
		printf("    value text_kz           : <%s>\n"  , resRef.getString  ("text_kz"           ));
		printf("    value text              : <%s>\n"  , resRef.getString  ("text"              ));
	}

	resRef .close();
	stmtRef.close();
	//connRef.close();

	connRef.setRawFetching(false);

	printf("summarized fetch total time in DB library %d ms\n", basar::db::sql::getDbLibFetchTime());
	printf("fetched: %d rows\n", resRef.getFetchedRows());

	printf("\n\n");
}

//------------------------------------------------------------------------------
void test_statement_execsinglequery(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execsinglequery infx: ------------\n");
	printf("\n");

	ConstString stmt =	"select version_row from kddispo "
						"where vertriebszentrumnr = 9 "
						"and kddisponr = 294968";
	
	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);


	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value version_row : %d\n", resRef.getInt32("version_row"));
	}
/*
	resRef .close();
	stmtRef.close();
	//connRef.close();
*/	
	resRef = stmtRef.executeSingleQuery(stmt);

	cnt = 0;

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());
		
		printf("    value 0           : %d\n", resRef.getInt32(0));
	}
}

//------------------------------------------------------------------------------
void test_statement_execsinglequery2(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execsinglequery2 infx: -----------\n");
	printf("\n");

	ConstString stmt =	"select count(*),min(kddisponr), sum(version_row) from kddispo "
						"where vertriebszentrumnr = 9";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);

	Int32  cnt = 0;
	
	printf("\nstatement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value count(*)          : %f\n"  , resRef.getDecimal("(count(*))").toFloat64());
		printf("    value count(*)          : <%s>\n", resRef.getDecimal("(count(*))").toString (2).c_str());
		printf("    value 1 min             : %d\n"  , resRef.getInt32  ("(min)"));
		printf("    value 2 sum             : %f\n"  , resRef.getDecimal("(sum)").toFloat64());
		printf("    value 2 sum             : <%s>\n", resRef.getDecimal("(sum)").toString ().c_str());
	}
/*
	resRef .close();
	stmtRef.close();
	//connRef.close();
*/	

//---
	ConstString stmt2 =	"select max(version_row) " //count(*) "
						//"as num "
		                "from kddispo "
						"where vertriebszentrumnr = 9";

	basar::Decimal d = stmtRef.executeAggregate(stmt2);

	printf("\n\nstatement       : <%s>\n\n", stmt2);
	printf("    value count(*)  : %f\n", d.toFloat64());
//---
		
	
	ConstString stmt3 =	"select count(*) as num, min(kddisponr), sum(version_row) "
		                "from kddispo "
						"where vertriebszentrumnr = 9";

	resRef  = stmtRef.executeSingleQuery(stmt3);

	cnt = 0;
	
	printf("\n\nstatement       : <%s>\n\n", stmt3);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value count(*) as num   : %f\n", resRef.getDecimal("num").toFloat64());
		printf("    value 0 count(*)        : %f\n", resRef.getDecimal(0)    .toFloat64());
		printf("    value 1 min             : %d\n", resRef.getInt32  (1)    );
		printf("    value 2 sum             : %f\n", resRef.getDecimal(2)    .toFloat64());
	}

	printf("\n");
}

//------------------------------------------------------------------------------
void test_statement_execnestedquery(ConnectionRef connRef1,
										   ConnectionRef connRef2)
{
	printf("\n");
	printf("--- test_statement_execnestedquery infx: ---------------\n");
	printf("\n");

	ConstString stmt1 = "select first 10 kddisponr, preisekapo, artikel_bez from kddispo ";
	ConstString stmt2 = "select first 10 matchcode, idfnr from kunde ";

	connRef1.setRawFetching(true);
	connRef2.setRawFetching(false);

	StatementRef stmtRef1 = connRef1.createStatement();
	ResultsetRef resRef1  = stmtRef1.executeQuery(stmt1);

    StatementRef stmtRef2 = connRef2.createStatement();
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

			printf("    value kddisponr    : %d\n"    , resRef1.getInt32   ("kddisponr"  ));
			printf("    value preisekapo   : %f\n"    , resRef1.getDecimal ("preisekapo" ).toFloat64());
			printf("    value artikel_bez  : <%s>\n"  , resRef1.getString  ("artikel_bez"));

			printf("    value matchcode    : <%s>\n"  , resRef2.getString  ("matchcode"));
			printf("    value idfnr        : %d\n"   , resRef2.getInt32   ("idfnr"  ));
		}
	}

	printf("\n\n");

	resRef1 .close();
	stmtRef1.close();
	resRef2 .close();
	stmtRef2.close();

	connRef1.setRawFetching(false);
	connRef2.setRawFetching(false);

	printf("summarized fetch total time in DB library %d ms\n", basar::db::sql::getDbLibFetchTime());
	printf("fetched: %d rows\n", cnt);

	printf("\n\n");
}

//------------------------------------------------------------------------------
void test_statement_execsinglequeryparam(ConnectionRef connRef)
{
	BLOG_TRACE_METHOD(LoggerPool::s_LoggerStmt, "test_statement_execsinglequeryparam");

	printf("\n\n");
	printf("--- test_statement_execsinglequeryparam infx: ------------\n");
	printf("\n");


	ConstString stmt =	"select version_row from kddispo "
						"where vertriebszentrumnr = ? "
						"and kddisponr = ?"; // 294968
	
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "created prepared statement");

	stmtRef.setInt16(0, 9);
	stmtRef.setInt32(1, 294968);

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "set Ints");

	ResultsetRef resRef  = stmtRef.executeSingleQuery();

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "exec'ed single query");

	Int32  cnt = 0;
	
	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "resRef.getSelectedCols()");

	while (true == resRef.next())
	{
		BLOG_TRACE(LoggerPool::s_LoggerStmt, "in loop after resultset next");

		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value version_row : %d\n", resRef.getInt32("version_row"));
	}


	BLOG_TRACE(LoggerPool::s_LoggerStmt, "after loop with resultset next");
	
/*
	resRef .close();
	stmtRef.close();
	//connRef.close();
*/	
	resRef = stmtRef.executeSingleQuery();

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "exec'ed single query again");

	cnt = 0;

	while (true == resRef.next())
	{
		BLOG_TRACE(LoggerPool::s_LoggerStmt, "in loop after 2nd resultset next");

		++cnt;
		printf("fetched rows    : %d\n", resRef.getFetchedRows());
		
		printf("    value 0           : %d\n", resRef.getInt32(0));
	}

	BLOG_TRACE(LoggerPool::s_LoggerStmt, "after loop with 2nd resultset next");
}

//------------------------------------------------------------------------------
void test_statement_execqueryparam(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_execqueryparam infx: ------------------\n");
	printf("\n");

	ConstString stmt = 
					   "select k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez, "
		               "t.kddisponr as kdtext_kddisponr, t.vertriebszentrumnr, t.text_kz, t.text "
		               "from kddispo k left outer join kdtext t "
		               "on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		               "where k.vertriebszentrumnr = ? "
					   "order by k.kddisponr desc";

	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	stmtRef.setInt16(0,9);
	ResultsetRef resRef  = stmtRef.executeQuery();

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	/*while*/ if (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value kddispo_kddisponr : %d\n"   , resRef.getInt32   ("kddispo_kddisponr" ));
		printf("    value version_row       : %d\n"   , resRef.getInt32   ("version_row"       ));
		printf("    value preisekapo        : %f\n"    , resRef.getDecimal ("preisekapo"        ).toFloat64());
		printf("    value preisekapo        : <%s>\n"  , resRef.getDecimal ("preisekapo"        ).toString ().c_str());
		printf("    value artikel_bez       : <%s>\n"  , resRef.getString  ("artikel_bez"       ));
		printf("    value kdtext_kddisponr  : %d\n"   , resRef.getInt32   ("kdtext_kddisponr"  ));
		printf("    value vertriebszentrumnr: %d\n"   , resRef.getInt16   ("vertriebszentrumnr"));
		printf("    value text_kz           : <%s>\n"  , resRef.getString  ("text_kz"           ));
		printf("    value text              : <%s>\n"  , resRef.getString  ("text"              ));
	}

	printf("\n\n");


	resRef .close();
	stmtRef.close();

	//resRef.reopen();
}

//------------------------------------------------------------------------------
void test_statement_execimmparam(ConnectionRef connRef)
{
	BLOG_TRACE_METHOD(LoggerPool::s_LoggerStmt, "test_statement_execimmparam");

	printf("\n");
	printf("--- test_statement_execimmparam: --------------------\n");
	printf("\n");

	ConstString stmt = "update kddispo set version_row = version_row + 1 "
				   		"where vertriebszentrumnr = ? and lower(dispo_status) = ?";

	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	

	stmtRef.setInt16 (0, 9  );
	stmtRef.setString(1, "r");

	basar::BULong iRows  = stmtRef.execute().m_affectedRows;

	printf("update kddispo(1): rows %d\n", iRows);

	ConstString stmt2 =	"update dttest set date = ?, dtim = ?, dtim3 = ? where nr = 103 ";
	
	stmtRef = connRef.createPreparedStatement(stmt2);
	
	basar::Date dt;
	dt.setDate(20050606);
	basar::DateTime d1(2005, 8, 9);
	Int32 d = 20050201,  t = 121314567;
	basar::DateTime d2(d, t);

	stmtRef.setDate(0, dt); //overwriting previous set value is possible !
	stmtRef.setDateTime(1, d1);
	stmtRef.setDateTime(2, d2);

	iRows  = stmtRef.execute().m_affectedRows;
	printf("update dttest(1): rows %d\n", iRows);

	basar::Date dt2;
	stmtRef.setDate(0, dt2);

	iRows  = stmtRef.execute().m_affectedRows;
	printf("update dttest(2): rows %d\n", iRows);
}

//------------------------------------------------------------------------------
void test_statement_lvarchar(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_lvarchar: --------------------\n");
	printf("\n");

	ConstString stmt = "select branchno, information from branchinfo where branchno = 51"; //lvarchar
	//ConstString stmt = "select bemerkungen, kdauftragnr from akdauftrag where kdauftragnr = 53553"; //varchar

	connRef.setRawFetching(true);

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

	connRef.setRawFetching(false);
}

//------------------------------------------------------------------------------
void test_statement_date(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_date: --------------------\n");
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

		printf("    date       : %d\n", resRef.getDate("date").getDate());
		printf("    dtim-date  : %d\n", resRef.getDateTime("dtim2").getDate());
		printf("    dtim-time  : %d\n", resRef.getDateTime("dtim2").getTime());
		printf("    nr         : %d\n" , resRef.getInt16("nr"));
	}
}

//------------------------------------------------------------------------------
void test_statement_execcursorupdate(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execcursorupdate infx: ------------\n");
	printf("\n");

	ConstString stmtSelect = "select artikel_nr, klassen_nr, filialnr from artklasse for update";
	ConstString stmtUpdate = "update artklasse set filialnr = 40";

	{	// big transaction 
		connRef.begin();
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef = stmtRef.executeQuery(stmtSelect);
		while (true == resRef.next())
		{
			if (resRef.getInt16("filialnr") == 44)
			{
				resRef.execCurrentRow(stmtUpdate);
			}
		}

		resRef .close();
		stmtRef.close();

		connRef.rollback();
	}
	
	{	// small transaction 
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef = stmtRef.executeQuery(stmtSelect, basar::db::sql::CURS_HOLD_OVER_COMMIT);

		for (;;)
		{
			connRef.begin();

			if (false == resRef.next())
			{
				connRef.rollback();
				break;
			}

			if (resRef.getInt16("filialnr") == 44)
			{
				resRef.execCurrentRow(stmtUpdate);
			}

			connRef.rollback();
		}

		resRef .close();
		stmtRef.close();
	}
}

//------------------------------------------------------------------------------
void test_statement_execcursorinsert(ConnectionRef connRef)
{
	BLOG_TRACE_METHOD(LoggerPool::s_LoggerStmt, "test_statement_execcursorinsert");

	printf("\n\n");
	printf("--- test_statement_execcursorinsert infx: ------------\n");
	printf("\n");

	ConstString stmt =	"insert into parameterkd values (?, ?, ?, ?, ?) ";

	connRef.begin();
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	InsertsetRef insRef  = stmtRef.executeInsert();

	VarString help;
	help = "test";
	help = "2004-11-11 14:12:34.444";

	stmtRef.setInt16 (0, 9);
	stmtRef.setString(1, help.c_str());
	stmtRef.setString(2, help.c_str());
	stmtRef.setString(3, help.c_str());
	stmtRef.setString(4, help.c_str());


	insRef.insertRow();

	help = "abcdefee";

	stmtRef.setInt16 (0, 10);
	stmtRef.setString(1, help.c_str());
	stmtRef.setString(2, help.c_str());
	stmtRef.setString(3, help.c_str());
	stmtRef.setString(4, help.c_str());

	insRef.insertRow();


	insRef.flushRows();

	//connRef.commit();
	connRef.rollback();

}

//------------------------------------------------------------------------------
void test_statement_execcursorinsert2(ConnectionRef connRef)
{
	printf("\n\n");
	printf("--- test_statement_execcursorinsert2 infx: ------------\n");
	printf("\n");

	ConstString stmt =	"insert into dttest (nr,date,dtim3) values (5, ?, ?) ";
//	ConstString stmt =	"insert into dttest (nr,date) values (5, ?) ";
		//set date = ?, dtim = ?, dtim3 = ? where nr = 3"

	printf("begin() ...\n");
	printf("isInTransaction = %d\n", connRef.isInTransaction());
	connRef.begin();

	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);
	InsertsetRef         insRef  = stmtRef.executeInsert();

	basar::Date dt;
	Int32 d = 20050201,
		  t = 121314567;
	stmtRef.setDate    (0, dt);
	stmtRef.setDateTime(1, basar::DateTime(d, t));

	printf("execute (<%s>) ...\n", stmt);

	insRef.insertRow();

	insRef.flushRows();

	printf("rollback() ...\n");

	//connRef.commit();
	connRef.rollback();

	printf("isInTransaction = %d\n\n", connRef.isInTransaction());
}

//------------------------------------------------------------------------------
void testinfx_holdcursor(ConnectionRef connRef)
{
	printf("\n");
	printf("--- testinfx_holdcursor: ------------------\n");
	printf("\n");

	ConstString stmt = 
					   "select k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez "
		               "from kddispo k "
		               "where k.vertriebszentrumnr = 9 "
					   "order by k.kddisponr desc";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt, basar::db::sql::CURS_DEFAULT);

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	// first

	connRef.begin();
	connRef.commit();	// closes cursor

	try
	{
		while(true == resRef.next())
		{
		}

		// should never be reached
		assert(0);
	}
	catch ( const basar::db::sql::DbException &  except )
	{
		printf("\n%s\n", except.what().c_str());
		printf("\ncursor without hold was automatically closed by commit() --> correctly thrown exception\n");
	}

	printf("\nfetched: %d rows\n", resRef.getFetchedRows());
	resRef .close();
	stmtRef.close();


	printf("\n\n");

	// second: now with hold
	ConstString stmt2 = "select k.kddisponr as kddispo_kddisponr "
						"from kddispo k "
						"where k.vertriebszentrumnr = 9 "
						"order by k.kddisponr desc";

	stmtRef = connRef.createStatement();
	resRef  = stmtRef.executeQuery(stmt2, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	printf("statement       : <%s>\n\n", stmt2);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	connRef.begin();
	connRef.commit();	// closes cursor

	try
	{
		while (resRef.next())
		{
		}
	}
	catch ( const basar::Exception &  except )		// should never be reached
	{
		printf("\n\n%s\n", except.what().c_str());
		assert(0);
	}

	printf("\ncursor with hold is still open after commit() --> correct behaviour\n");
	printf("\nfetched: %d rows\n", resRef.getFetchedRows());
	resRef .close();
	stmtRef.close();


	printf("\n");
}

//------------------------------------------------------------------------------
void testinfx_fetchbuffersize(ConnectionRef connRef)
{
	printf("\n");
	printf("--- testinfx_fetchbuffersize: ------------------\n");
	printf("\n");

	ConstString stmt = "select first 1000 * from zartikel";
	int size;
	clock_t t1, t2;

#ifndef XXX1

	connRef.setFetBufSize(0x7fff);
	size = connRef.getFetBufSize();

	printf("\nfetch buffer size %d\n", size);

	StatementRef stmtRef1 = connRef.createStatement();
	ResultsetRef resRef1  = stmtRef1.executeQuery(stmt, basar::db::sql::CURS_DEFAULT);

	t1 = clock();
	while(true == resRef1.next())
	{
	}
	t2 = clock();

	printf("clocks: %ld\n", t2 - t1);
	printf("fetched: %d rows\n\n", resRef1.getFetchedRows());

	resRef1 .close();
	stmtRef1.close();

#endif

#ifndef XXX2

	connRef.setFetBufSize(0x1000);
	size = connRef.getFetBufSize();

	printf("\nfetch buffer size %d\n", size);

	StatementRef stmtRef2 = connRef.createStatement();
	ResultsetRef resRef2  = stmtRef2.executeQuery(stmt, basar::db::sql::CURS_DEFAULT);

	t1 = clock();
	while(true == resRef2.next())
	{
	}
	t2 = clock();

	printf("clocks: %ld\n", t2 - t1);
	printf("fetched: %d rows\n\n", resRef2.getFetchedRows());

	resRef2 .close();
	stmtRef2.close();

#endif
}

//------------------------------------------------------------------------------
void testinfx_update_cursorbehaviour(ConnectionRef connRef)
{
	printf("\n");
	printf("--- testinfx_update_cursorbehaviour: ------------------\n");
	printf("\n");

	ConstString stmt =  
					   "select k.kddisponr, k.version_row, k.preisekapo, k.artikel_bez "
		               "from kddispo k "
		               "where k.vertriebszentrumnr = 9 "
					   "order by k.kddisponr desc";

	StatementRef selStmtRef = connRef   .createStatement();
	ResultsetRef resRef     = selStmtRef.executeQuery (	stmt
														, basar::db::sql::CURS_HOLD_OVER_COMMIT
														);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n\n", resRef.getSelectedCols());

	while ((3 > cnt) && resRef.next() )
	{
		++cnt;

		printf("[%3d]: ", resRef.getFetchedRows());

		printf("kddisponr: %7d "    , resRef.getInt32   (0));
		printf("version_row: %4d "  , resRef.getInt32   (1));
		printf("preisekapo: %9.2f " , resRef.getDecimal (2).toFloat64());
		printf("artikel_bez: <%s>\n", resRef.getString  (3));
	}

	printf("break\n");


	{
		VarString    updStmt;
		updStmt.format( "update kddispo "
			            "set version_row = version_row + 1 "
				   		"where vertriebszentrumnr = 9 "
						"and kddisponr = %d",
						resRef.getInt32(0));	// current kddisponr

		connRef.begin();

		StatementRef updStmtRef = connRef   .createStatement();
		basar::BULong iRows      = updStmtRef.execute(updStmt).m_affectedRows;
		printf("update kddispo: SQL statement\n"
			   "<%s>\n"
			   "updated rows %d\n", 
			   updStmt.c_str(),
			   iRows);


		updStmt.format( "update kddispo "
			            "set version_row = version_row + 1 "
				   		"where vertriebszentrumnr = 9 "
						"and kddisponr = %d",
						294968);

		iRows = updStmtRef.execute(updStmt).m_affectedRows;

		printf("update kddispo: SQL statement\n"
			   "<%s>\n"
			   "updated rows %d\n", 
			   updStmt.c_str(),
			   iRows);

		connRef.commit();
	}

	while (true == resRef.next())
	{
		++cnt;

		printf("[%3d]: ", resRef.getFetchedRows());

		printf("kddisponr: %7d "    , resRef.getInt32   (0));
		printf("version_row: %4d "  , resRef.getInt32   (1));
		printf("preisekapo: %9.2f " , resRef.getDecimal (2).toFloat64());
		printf("artikel_bez: <%s>\n", resRef.getString  (3));
	}

	printf("\nfetched: %d rows\n", resRef.getFetchedRows());

	printf("\n\n");
}

//------------------------------------------------------------------------------
void testinfx_validresultset(ConnectionRef connRef)
{
	printf("\n");
	printf("--- testinfx_validresultset: ------------------\n");
	printf("\n");

	ConstString stmt =  
					   "select k.kddisponr, k.version_row, k.preisekapo, k.artikel_bez "
		               "from kddispo k "
		               "where k.vertriebszentrumnr = 9 "
					   "order by k.kddisponr desc";

	ResultsetRef resRef;
	UInt32 cnt = 0;

	{
		StatementRef selStmtRef = connRef   .createStatement();
		
		resRef     = selStmtRef.executeQuery (	stmt );


		printf("statement       : <%s>\n\n", stmt);
		printf("selected columns: %d\n\n", resRef.getSelectedCols());

		while ((3 > cnt) && resRef.next() )
		{
			++cnt;

			printf("[%3d]: ", resRef.getFetchedRows());

			printf("kddisponr: %7d "    , resRef.getInt32   (0));
			printf("version_row: %4d "  , resRef.getInt32   (1));
			printf("preisekapo: %9.2f " , resRef.getDecimal (2).toFloat64());
			printf("artikel_bez: <%s>\n", resRef.getString  (3));
		}
	}

	printf("break\n");

	try
	{
		while (true == resRef.next())
		{
			++cnt;

			printf("[%3d]: ", resRef.getFetchedRows());

			printf("kddisponr: %7d "    , resRef.getInt32   (0));
			printf("version_row: %4d "  , resRef.getInt32   (1));
			printf("preisekapo: %9.2f " , resRef.getDecimal (2).toFloat64());
			printf("artikel_bez: <%s>\n", resRef.getString  (3));
		}

		printf("\nfetched: %d rows\n", resRef.getFetchedRows());
	}
	catch ( const basar::db::sql::DbException &  except )
	{
		printf("\n%s\n", except.what().c_str());
		printf("\nstatement already closed => correctly thrown exception\n");
	}


	printf("\n\n");
}

//------------------------------------------------------------------------------
void prepinsert_infx(ConnectionRef connRef)
{
	printf("\n");
	printf("--- prepinsert_infx --------------------\n");
	printf("\n");

	basar::BULong iRows;
	ConstString stmt =	"insert into dttest (nr, date, dtim2, dtim3, dtim4, name, varname, lvarname, varmax) "
		                "values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
	
	PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);

	using namespace basar;

	Int16    col0 = 4000;					// nr
	Date     col1(1964, 1, 28);				// date
	DateTime col2(19680708, 103456789);		// dtim2
	DateTime col3(19690809, 114426123);		// dtim3
	DateTime col4;							// dtim4
	Int8     col5[8   + 1] = "nm123456";	// name
	Int8     col6[8   + 1] = "vn123456";	// varname
	Int8     col7[8   + 1] = "lv123456";	// lvarname
	Int8     col8[255 + 1] = "vmcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij12345";

	stmtRef.setInt16   (0, col0);
	stmtRef.setDate    (1, col1);
	stmtRef.setDateTime(2, col2);
	stmtRef.setDateTime(3, col3);
	stmtRef.setDateTime(4, col4);
	stmtRef.setString  (5, col5);
	stmtRef.setString  (6, col6);
	stmtRef.setString  (7, col7);
	stmtRef.setString  (8, col8);

	iRows  = stmtRef.execute().m_affectedRows;

	col0 +=1;
	col1.addDate(1);
	col2.addTime(10000000);
	col3.addTime(20000000);
	col4.addTime(10000000);

	printf("col2 = (%10d, %10d)\n", col2.getDate(), col2.getTime());
	printf("col3 = (%10d, %10d)\n", col3.getDate(), col3.getTime());
	printf("col4 = (%10d, %10d)\n", col4.getDate(), col4.getTime());

	::sprintf(col5, "nm223456");
	::sprintf(col6, "vn223456");
	::sprintf(col7, "lv223456");
	::sprintf(col8, "vm223456");


	iRows  += stmtRef.execute().m_affectedRows;

	col0 += 1;

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

		printf("    nr         : %d\n", resRef.getInt16   ("nr"   ));
		printf("    name       : <%s>\n" , resRef.getString("name"));
		printf("    varname    : <%s>\n" , resRef.getString("varname"));
		printf("    lvarname   : <%s>\n" , resRef.getString("lvarname"));
		printf("    date       : %d\n", resRef.getDate    ("date" ).getDate());
		printf("    price      : %f\n" , resRef.getDecimal ("price" ).toFloat64());

		printf("    dtim-date 3: %d\n", resRef.getDateTime("dtim3").getDate());
		printf("    dtim-time 3: %d\n", resRef.getDateTime("dtim3").getTime());
		printf("    dtim-date  : %d\n", resRef.getDateTime("dtim").getDate());
		printf("    dtim-time  : %d\n", resRef.getDateTime("dtim").getTime());
		printf("    dtim-date 5: %d\n", resRef.getDateTime("dtim5").getDate());
		printf("    dtim-time 5: %d\n", resRef.getDateTime("dtim5").getTime());


		printf("    nr         : %d\n",  resRef.getInt16   ("r5"));
		printf("    name       : <%s>\n", resRef.getString  ("r7"));
		printf("    varname    : <%s>\n", resRef.getString  ("r9"));
		printf("    lvarname   : <%s>\n", resRef.getString("r10"));
		printf("    date       : %d\n", resRef.getDate    ("r6").getDate());
		printf("    price      : %f\n",  resRef.getDecimal ("r8" ).toFloat64());

		printf("    dtim-date 3: %d\n", resRef.getDateTime("r1").getDate());
		printf("    dtim-time 3: %d\n", resRef.getDateTime("r1").getTime());
		printf("    dtim-date  : %d\n", resRef.getDateTime("r3").getDate());
		printf("    dtim-time  : %d\n", resRef.getDateTime("r3").getTime());
		printf("    dtim-date 5: %d\n", resRef.getDateTime("r4").getDate());
		printf("    dtim-time 5: %d\n", resRef.getDateTime("r4").getTime());
	}
}


//------------------------------------------------------------------------------
void test_statement_date_ext2(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_statement_date: --------------------\n");
	printf("\n");

	bool flRaw;

	{
		flRaw = false;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.name from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}

	{
		flRaw = true;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.name from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}

	{
		flRaw = false;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.varname, l.varmax from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
			printf("    #2         : <%s>\n" , resRef.getString  (2));
		}
	}


	{
		flRaw = true;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.varname, l.varmax from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
			printf("    #2         : <%s>\n" , resRef.getString  (2));
		}
	}
	
	{
		flRaw = false;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.lvarname from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}


	{
		flRaw = true;

		connRef.setRawFetching(flRaw);

		ConstString stmt = "select first 2 l.nr, l.lvarname from dttest l ";
	
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement (raw=%d) : <%s>\n\n", flRaw, stmt);

		while (true == resRef.next())
		{
			printf("    nr         : %d\n"   , resRef.getInt16   ("nr"   ));
			printf("    #1         : <%s>\n" , resRef.getString  (1));
		}
	}

	connRef.setRawFetching(false);
}

//------------------------------------------------------------------------------
void test_custom(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_custom infx: -----------------\n");
	printf("\n");

	ConstString stmt = "SELECT nr, varmax FROM dttest";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (resRef.next())
	{
		++cnt;

		printf("fetched rows      : %d\n", resRef.getFetchedRows());

		printf("    nr    : %d\n"   , resRef.getInt16   ("nr"    ));
		printf("    varmax: %s\n"   , resRef.getString  ("varmax"));
	}
}

//------------------------------------------------------------------------------
void test_inv(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_inv infx: -----------------\n");
	printf("\n");

	ConstString stmt = "select first 2 "
		               "kddisponr, "
					   "dispo_status, "
					   "current as cr, "
					   "datetime(12:34) hour to minute as dt, "
					   //"interval(43:21) hour to minute as iv, " // not possible in basar 16.11.07
					   "cast(interval(43:21) hour to minute as char(20)) as civ, "
					   "1 "
					   "from kddispo;";

	StatementRef stmtRef = connRef.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	basar::DateTime cr, dt;
	basar::DateTime iv;

	while (resRef.next())
	{
		++cnt;

		cr = resRef.getDateTime("cr");
		dt = resRef.getDateTime("dt");
		//iv = resRef.getDateTime("iv");


		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    kddisponr   : %d\n"   , resRef.getInt32   ("kddisponr"   ));
		printf("    dispo_status: %s\n"   , resRef.getString  ("dispo_status"));
		printf("    current     : %d %d\n", cr.getDate(), cr.getTime());
		printf("    dt          : %d %d\n", dt.getDate(), dt.getTime());
		//printf("    iv          : %d %d\n", iv.getDate(), iv.getTime());
		printf("    civ         : <%s>\n"   , resRef.getString  ("civ"));
	}
}

//------------------------------------------------------------------------------
void test_type_interval(ConnectionRef connRef)
{
	/* DDL dttest:

		CREATE TABLE pps1g@onl_sofumi1:dttest
		(
			nr			smallint						DEFAULT 0 NOT NULL,
			dtim		datetime YEAR TO DAY			DEFAULT datetime (9999-12-31) YEAR TO DAY						NOT NULL,
			intrv1		interval DAY(2) TO SECOND		DEFAULT interval (  0 00:00:00) DAY(2) TO SECOND				NOT NULL,
			intrv2		interval YEAR(4) TO MONTH		DEFAULT interval (    1-00) YEAR(4) TO MONTH					NOT NULL,
			date		date							DEFAULT '12/31/9999'											NOT NULL,
			dtim2		datetime YEAR TO SECOND			DEFAULT datetime (9999-12-31 23:59:59) YEAR TO SECOND			NOT NULL,
			dtim3		datetime YEAR TO FRACTION(3)	DEFAULT datetime (9999-12-31 23:59:59.999) YEAR TO FRACTION(3)	NOT NULL,
			dtim4		datetime HOUR TO SECOND			DEFAULT datetime (23:59:59) HOUR TO SECOND						NOT NULL,
			dtim5		datetime HOUR TO FRACTION(3)	DEFAULT datetime (23:59:59.999) HOUR TO FRACTION(3)				NOT NULL,
			price		decimal(9,2)					DEFAULT 0														NOT NULL,
			name		character(8)					DEFAULT ''														NOT NULL,
			varname		varchar(8,0)					DEFAULT ''														NOT NULL,
			lvarname	lvarchar						DEFAULT 'NULL'													NOT NULL,
			varmax		varchar(255,0)					DEFAULT ''														NOT NULL
		)
	*/

	printf("\n");
	printf("--- test_type_interval infx: -----------------\n");
	printf("\n");

	{	// --- select
		ConstString stmt = 
			            "select "
						"nr, name, "
						"current as cr, "
						"datetime(12:34) hour to minute as dt, "
						//"interval(43:21) hour to minute as iv, " // 16.11.07: not possible for basar 
						"trim(leading from cast(interval(43:21) hour to minute as char(20))) as civ, "
						"trim(leading from cast(intrv1 as char(15))) as iv1, "
						"trim(leading from cast(intrv2 as char(15))) as iv2 "
						"from dttest "
						"where nr in (1, 2);";

		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		printf("statement       : <%s>\n\n", stmt);

		basar::DateTime cr, dt;
		basar::DateTime iv;

		while (resRef.next())
		{
			cr = resRef.getDateTime("cr");
			dt = resRef.getDateTime("dt");
			//iv = resRef.getDateTime("iv");


			printf("fetched rows    : %d\n", resRef.getFetchedRows());

			printf("    nr          : %d\n"   , resRef.getInt16   ("nr"  ));
			printf("    name        : %s\n"   , resRef.getString  ("name"));
			printf("    current     : %d %d\n", cr.getDate(), cr.getTime());
			printf("    dt          : %d %d\n", dt.getDate(), dt.getTime());
			//printf("    iv          : %d %d\n", iv.getDate(), iv.getTime());
			printf("    civ         : <%s>\n"   , resRef.getString  ("civ"));
			printf("    iv1         : <%s>\n"   , resRef.getString  ("iv1"));
			printf("    iv2         : <%s>\n"   , resRef.getString  ("iv2"));

			printf("\n");
		}
	}

	// #####################################################

	{ // --- update a: interval values
		ConstString  stmt    = "update dttest "
			                   "set date = today, dtim3 = current, "
							   "    intrv1 = interval(36 12:54:23) day to second, "
                               "    intrv2 = interval(45-11) year to month "
							   "where nr in (1, 2)";
		printf("\nstatement: <%s>\n\n", stmt);
		StatementRef stmtRef = connRef.createStatement();


		basar::BULong rows = stmtRef.execute(stmt).m_affectedRows;

		printf("updated rows %d\n", rows);

	}

	{ // --- update b: string values
		ConstString  stmt    =	"update dttest "
								"set date = today, dtim3 = current, "
							    "    intrv1 = cast('45 07:31:01' as char(15) ), "
								"    intrv2 = cast('23-07'       as char(15) ) "
								"where nr in (1, 2)";
		printf("\nstatement: <%s>\n\n", stmt);
		StatementRef stmtRef = connRef.createStatement();

		basar::BULong rows = stmtRef.execute(stmt).m_affectedRows;

		printf("updated rows %d\n", rows);

	}

	{ // --- update c: prepared statement
		ConstString  stmt    =	"update dttest "
								"set date = today, dtim3 = current, "
							    "    intrv1 = cast (? as char(15) ), "
								"    intrv2 = cast (? as char(15) )  "
								"where nr in (1, 2)";
		printf("\nstatement: <%s>\n\n", stmt);

		PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);

		stmtRef.setString(0, "98 23:12:56");
		stmtRef.setString(1, "9786-11"    );

		basar::BULong rows = stmtRef.execute().m_affectedRows;

		printf("updated rows %d\n", rows);
	}


	// ####################################################

	{ // --- insert a: interval values
		ConstString  stmt    = "insert into dttest (nr, name, intrv1, intrv2, date, dtim3) values "
			                   "((select max(rowid) from dttest), "
							   " 'n'||(select max(rowid) from dttest), "
							   " interval(00 12:54:23) day to second, "
							   " interval(345-07) year to month, "
							   "today, current) ";
		printf("\nstatement: <%s>\n\n", stmt);
		StatementRef stmtRef = connRef.createStatement();

		basar::BULong rows = stmtRef.execute(stmt).m_affectedRows;
		printf("inserted rows %d\n", rows);
	}

	{ // --- insert b: string values
		ConstString  stmt    = "insert into dttest (nr, name, intrv1, intrv2, date, dtim3) values "
			                   "((select max(rowid) from dttest), "
							   " 'n'||(select max(rowid) from dttest), "
							   " cast('78 08:01:59' as char(15)), "
							   " cast('5234-02'      as char(15)), "
							   "today, current) ";
		printf("\nstatement: <%s>\n\n", stmt);
		StatementRef stmtRef = connRef.createStatement();

		basar::BULong rows = stmtRef.execute(stmt).m_affectedRows;
		printf("inserted rows %d\n", rows);
	}

	{ // --- insert c: prepared statement
		ConstString  stmt    = "insert into dttest (nr, name, intrv1, intrv2, date, dtim3) values "
			                   "((select max(rowid) from dttest), "
							   " 'n'||(select max(rowid) from dttest), "
							   " cast(? as char(15)), "
							   " cast(? as char(15)), "
							   "today, current) ";
		printf("\nstatement: <%s>\n\n", stmt);
		PreparedStatementRef stmtRef = connRef.createPreparedStatement(stmt);

		stmtRef.setString(0, "04 00:00:00");
		stmtRef.setString(1, "6321-04"    );

		basar::BULong rows = stmtRef.execute().m_affectedRows;
		printf("inserted rows %d\n", rows);
	}
}

//------------------------------------------------------------------------------
void test_apr_select(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_apr_select infx: -----------------\n");
	printf("\n");

	ConstString stmt1 = "SELECT MAX(sortid + 1) as max FROM aprpricereductionitem WHERE processid =   79";
	ConstString stmt2 = "SELECT MAX(sortid + 1) as max FROM aprpricereductionitem WHERE processid = 1179";

	{
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt1);

		printf("statement       : <%s>\n\n", stmt1);

		while (resRef.next())
		{
			printf("    max %g\n", resRef.getDecimal("max").toFloat64());
		}
	}

	{
		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt2);

		printf("statement       : <%s>\n\n", stmt2);

		while (resRef.next())
		{
			printf("    max %g\n", resRef.getDecimal("max").toFloat64());
		}
	}

}

//------------------------------------------------------------------------------
void test_apr_insert(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_apr_insert infx: -----------------\n");
	printf("\n");

	ConstString stmt1 = "INSERT INTO aprpricereductionitem "
						"(processid, articleno, tradepricediff, sortid) "
						" VALUES "
						"(79, 1234,  5.67, "
						"NVL("
						"    (SELECT MAX(sortid + 1) FROM aprpricereductionitem WHERE processid = 1179)"
						", 1) "
						")";
	ConstString stmt2 = "INSERT INTO aprpricereductionitem "
						"(processid, articleno, tradepricediff, sortid) "
						" VALUES "
						"(79, 5678,  9.87, "
						"(SELECT MAX(sortid + 1) FROM aprpricereductionitem WHERE processid = 1179)"
						")";

	basar::BULong		 rows;
	StatementRef stmtRef = connRef.createStatement();

	connRef.begin();

	rows = stmtRef.execute(stmt1).m_affectedRows;
	printf("inserted rows %d\n", rows);

	rows = 0;

	try
	{
		rows = stmtRef.execute(stmt2).m_affectedRows;
	}
	catch (const basar::Exception & except)
	{
		printf("%s\n", except.what().c_str());
	}

	printf("inserted rows %d\n", rows);

	connRef.rollback();
}

//------------------------------------------------------------------------------
void test_serialid(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_serialid infx: -----------------\n");
	printf("\n");

	ConstString stmt[] = 
	{ 
		"update dttest set varname = 'xyz' where nr = 1",			// hit
		"update dttest set varname = 'xyz' where nr = -9999999",	// no hit
		"update xxxxxx set xxxxxxx = 'xyz' where nr = -9999999",
		"insert into dttest (nr) values (9999)",											// w/o serial column
		"insert into aprpricereduction (processid, manufacturerno) values (9999,8888)",		// dedicated serial value
		"insert into aprpricereduction (processid, manufacturerno) values (0   ,7777)",		// let the DB server calculate
/*
		"update dttest set varname = 'xyz' where nr = -9999999	
		"insert into aprpricereduction (processid, manufacturerno) values (0   ,7777);",	// let the DB server calculate

		"insert into aprpricereduction (processid, manufacturerno) values (0   ,7777);"		// let the DB server calculate
		"update dttest set varname = 'xyz' where nr = -9999999;"
*/
	};

	Int32          serial;
	basar::BULong  rows;
	InfxErrorCodes rc;
	StatementRef   stmtRef = connRef.createStatement();

	serial = infxGetSerialValue();
	printf("serial value    :      %8d\n", serial);

	ConstString * pStmt;
	Int32         i;

	connRef.begin();

	for (pStmt = stmt, i = 0; i < sizeof(stmt)/sizeof(*stmt); ++pStmt, ++i)
	{
		rows   = stmtRef.execute(*pStmt, false).m_affectedRows;
		rc     = infxGetErrorCodes();
		serial = infxGetSerialValue();

		printf("statement       : <%s>\n", *pStmt);
		printf("rows            :      %8d\n"  , rows);
		printf("serial value    :      %8d\n", serial);
		printf("errorcodes      : main %8d, isam %8d\n\n", rc.main, rc.isam);
	}

	connRef.rollback();

	serial = infxGetSerialValue();
	printf("serial value    :      %8d\n", serial);
}

//------------------------------------------------------------------------------
void test_errorcodes(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_errorcodes infx: -----------------\n");
	printf("\n");

	ConstString stmt1 = "update dttest set varname = 'xyz' where nr = -9999999";
	ConstString stmt2 = "update xxxxxx set xxxxxxx = 'xyz' where nr = -9999999";

	{
		InfxErrorCodes rc = infxGetErrorCodes();
		printf("errorcodes      : main %8d, isam %8d\n\n", rc.main, rc.isam);
	}

	{
		StatementRef stmtRef = connRef.createStatement();
		basar::BULong rows    = stmtRef.execute(stmt1, false).m_affectedRows;

		InfxErrorCodes rc = infxGetErrorCodes();

		printf("statement       : <%s>\n", stmt1);
		printf("rows            : %d\n"  , rows);
		printf("errorcodes      : main %8d, isam %8d\n\n", rc.main, rc.isam);
	}

	{
		StatementRef stmtRef = connRef.createStatement();
		basar::BULong rows    = stmtRef.execute(stmt2, false).m_affectedRows;

		InfxErrorCodes rc = infxGetErrorCodes();

		printf("statement       : <%s>\n", stmt2);
		printf("rows            : %d\n"  , rows);
		printf("errorcodes      : main %8d, isam %8d\n\n", rc.main, rc.isam);
	}
}

//------------------------------------------------------------------------------
void test_prepared(ConnectionRef connRef)
{
	ConstString stmt1[] =
	{
		"select version_row from kddispo where vertriebszentrumnr = ? and kddisponr = ?",
		"select version_row from kddispo where vertriebszentrumnr = 3 and kddisponr = 1",

		"select k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez, "
		"t.kddisponr as kdtext_kddisponr, t.vertriebszentrumnr, t.text_kz, t.text "
		"from kddispo k left outer join kdtext t "
		"on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		"where k.vertriebszentrumnr = ? "
		"order by k.kddisponr desc",
		"select k.kddisponr as kddispo_kddisponr, k.version_row, k.preisekapo, k.artikel_bez, "
		"t.kddisponr as kdtext_kddisponr, t.vertriebszentrumnr, t.text_kz, t.text "
		"from kddispo k left outer join kdtext t "
		"on k.kddisponr = t.kddisponr and k.vertriebszentrumnr = t.vertriebszentrumnr "
		"where k.vertriebszentrumnr = 1 "
		"order by k.kddisponr desc",

		"update kddispo set version_row = version_row + 1 "
		"where vertriebszentrumnr = ? and lower(dispo_status) = ?",

		"insert into parameterkd values (?, ?, ?, ?, ?) ",
		"insert into dttest (nr,date,dtim3) values (-5555, ?, ?) ",
		"update dttest "
		"set date = today, dtim3 = current, "
		"    intrv1 = cast (? as char(15) ), "
		"    intrv2 = cast (? as char(15) )  "
		"where nr in (1, 2)",
		"insert into dttest (nr, name, intrv1, intrv2, date, dtim3) values "
		"((select max(rowid) from dttest), "
		" 'n'||(select max(rowid) from dttest), "
		" cast(? as char(15)), "
		" cast(? as char(15)), "
		"today, current) "
	};

	ConstString stmt2[] =
	{
		"update kddispo set version_row = version_row + 1 "
		"where vertriebszentrumnr = 3 and lower(dispo_status) = 'e'",

		"insert into parameterkd values (3, 'a', 'b', 'c', 'd') ",

		"insert into dttest (nr,date,dtim3) values (-5555, '11/30/2000', datetime (9999-12-31 23:59:59.999) year to fraction(3)) ",

		"update dttest "
		"set date = today, dtim3 = current, "
		"    intrv1 = cast (interval (0 00:00:00) DAY(2) TO SECOND as char(15) ), "
		"    intrv2 = cast (interval (1-00) YEAR(4) TO MONTH as char(15) )  "
		"where nr in (1, 2)",

		"insert into dttest (nr, name, intrv1, intrv2, date, dtim3) values "
		"((select max(rowid) from dttest), "
		" 'n'||(select max(rowid) from dttest), "
		" cast(interval (0 00:00:00) DAY(2) TO SECOND as char(15)), "
		" cast(interval (1-00) YEAR(4) TO MONTH as char(15)), "
		"today, current) "
	};

	PreparedStatementRef stmtRef;
	basar::BULong        iRows;

	for (int i = 0; i < sizeof(stmt1)/sizeof(*stmt1); ++i)
	{
		stmtRef = connRef.createPreparedStatement(stmt1[i]);
	}

	connRef.begin();

	for (int i = 0; i < sizeof(stmt2)/sizeof(*stmt2); ++i)
	{
		stmtRef = connRef.createPreparedStatement(stmt2[i]);
		iRows   = stmtRef.execute().m_affectedRows;

	}

	connRef.rollback();
}

//------------------------------------------------------------------------------

void test_cmp_selectedDateTime(ConnectionRef connRef)
{
	printf("\n");
	printf("--- test_cmp_selectedDateTime infx: -----------------\n");
	printf("\n");

	{
		ConstString stmt = "SELECT branchno, "
						"Max(headlastchangedate )::datetime year to fraction(3) as headlastchangedate "
						"FROM pps1g:giheadrep "
						"GROUP BY branchno ";

		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		UInt32 cnt = 0;

		printf("statement       : <%s>\n\n", stmt);

		basar::DateTime col_1[1000];
		basar::DateTime cmp(20080206, 170843000);

		while (resRef.next())
		{

			col_1[cnt] = resRef.getDateTime("headlastchangedate");

			printf("    col_1       : %d %d\n", col_1[cnt].getDate(), col_1[cnt].getTime());
			printf("    branch      : %d\n", resRef.getInt16("branchno"));

			++cnt;
		}

		bool b = (col_1[0] == cmp);

		printf("%s: (%08d, %09d) == (%08d, %09d)\n", 
			   b ? "true" : "false",
			   col_1[0].getDate(), col_1[0].getTime(), 
			   cmp.getDate(), cmp.getTime());
	}

	//-------------------------

	{
		ConstString stmt = "SELECT branchno, "
						"Max(headlastchangedate ) as headlastchangedate "
						"FROM pps1g:giheadrep "
						"GROUP BY branchno ";

		StatementRef stmtRef = connRef.createStatement();
		ResultsetRef resRef  = stmtRef.executeQuery(stmt);

		UInt32 cnt = 0;

		printf("statement       : <%s>\n\n", stmt);

		basar::DateTime col_1[1000];
		basar::DateTime cmp(20080206, 170843000);

		while (resRef.next())
		{

			col_1[cnt] = resRef.getDateTime("headlastchangedate");

			printf("    col_1       : %d %d\n", col_1[cnt].getDate(), col_1[cnt].getTime());
			printf("    branch      : %d\n", resRef.getInt16("branchno"));

			++cnt;
		}

		bool b = (col_1[0] == cmp);

		printf("%s: (%08d, %09d) == (%08d, %09d)\n", 
			   b ? "true" : "false",
			   col_1[0].getDate(), col_1[0].getTime(), 
			   cmp.getDate(), cmp.getTime());
	}
}


//------------------------------------------------------------------------------
void test_RawNvlStringInfx(ConnectionRef connect)
{
	printf("\n");
	printf("--- test_cmp_selectedDateTime infx: -----------------\n");
	printf("\n");

	static ConstString sql = 

		"SELECT FIRST 1 "
			//"NVL(m.colint, '  ')               as col1, "
			//"SUBSTR(NVL(m.colint, '  '), 1, 2) as col2, "
			//"NVL(m.colvch2, '  ')              as col3 "
			//"RPAD(NVL(m.colvch2::char(2), '  ')::char(2), 2)     as col3 "

/*			" '  '                      as col1, "
			" cast ('  ' as char(2))    as col2, "
			" cast ('  ' as varchar(2)) as col3, " */

/*			" nvl(m.colch2, '  ')                       as col1, "
			" nvl(m.colch2, cast ('  ' as char(2))    ) as col2, "
			" nvl(m.colch2, cast ('  ' as varchar(2)) ) as col3, " */
/*
			" nvl(m.colvch2, '  ')                       as col1, "
			" nvl(m.colvch2::char(2)   , '  '    ) as col2, "
			" nvl(m.colvch2::varchar(2), cast ('  ' as varchar(2)) ) as col3, " */

			" nvl(m.colvch2, '  ')                       as col1, "
			" nvl(m.colvch2::char(10)   , substr('  ', 1, 2)::char(2)    ) as col2, "
			" nvl(m.colvch2::varchar(10), cast ('  ' as varchar(2)) ) as col3, " 

			" 1 "
		"FROM but_detail d, OUTER but_master m "
		"WHERE d.colser = m.colser "
		  "AND d.colser > 10;";

	try
	{
		connect.setRawFetching(true);

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		Int32     cnt = 0;
		VarString col1,
			      col2,
				  col3;

		while (result.next())
		{
			col1  = result.getString("col1" );
			col2  = result.getString("col2" );
			col3  = result.getString("col3" );

			printf("    col1: <%s>\n", col1.c_str());
			printf("    col2: <%s>\n", col2.c_str());
			printf("    col3: <%s>\n", col3.c_str());

			++cnt;
		}

		assert(1 == cnt);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
    }
}

//------------------------------------------------------------------------------
void test_checkstmt(ConnectionRef connect)
{
	printf("\n");
	printf("--- test_checkstmt ------------------\n");
	printf("\n");

	ConstString stmt =  // before and after "select" are TABs!
					   "SELECT oh.branchno as branchno, oh.pharmacyno as pharmacyno "
					   ", count(op.posno) as anzpos "
					   "FROM pos1g@onl_sofumi1_tcp:orderhead oh LEFT JOIN pos1g@onl_sofumi1_tcp:orderpos op "
					   "on op.branchNo=oh.branchNo and op.orderNo=oh.OrderNo "
					   "WHERE oh.branchno=5 "
					   "and op.posno is not null "
					   "GROUP BY oh.branchno, oh.pharmacyno;";

	StatementRef stmtRef = connect.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	while (true == resRef.next())
	{
		++cnt;

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value branchno  : %d\n"   , resRef.getInt16   ("branchno"  ));
		printf("    value pharmacyno: %d\n"   , resRef.getInt32   ("pharmacyno"));
		//printf("    value anzpos    : %f\n"   , resRef.getDecimal ("anzpos"    ).toFloat64());
	}

	printf("\n\n");
}

//------------------------------------------------------------------------------
void test_checkstmt2(ConnectionRef connect)
{
	printf("\n");
	printf("--- test_checkstmt2 ------------------\n");
	printf("\n");

	ConstString stmt = //"SELECT cast(NVL(MIN(stockbookingprice), 0.0) as decimal) "
					   "SELECT stockbookingprice "
		               "FROM gilabel "
					   "WHERE branchno=-90 AND purchaseorderno=4 AND articleno=2766";

	StatementRef stmtRef = connect.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	Decimal d1;

	while (true == resRef.next())
	{
		++cnt;

		d1 = resRef.getDecimal(0);

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value stockbookingprice  : %f\n"   , d1.toFloat64());
		printf("    value stockbookingprice  : %s\n"   , d1.toString().c_str());
	}

	printf("\n\n");
}

//------------------------------------------------------------------------------
void test_checkstmt3(ConnectionRef connect)
{
	printf("\n");
	printf("--- test_checkstmt2 ------------------\n");
	printf("\n");

	ConstString stmt = "SELECT t1.poslastchangedate, t1.purchaseorderqty, t1.deliverynoteqty, t1.purchasingprice, "
		               "t1.giposstatus, t1.batchflag, t1.articleno, t1.enteredpurchasingprice, t1.purchaseorderno, "
					   "t1.branchno, t1.virtualtoteno, NVL(t2.cacointernaldesc, '**') AS cacointernaldesc, "
					   "t3.artikel_nr_herst AS articlenomanu, t3.artikel_name AS articlename, t3.einheit AS unity, "
					   "t3.darreichform AS form, t3.hersteller_nr AS manufacturyno, t3.artikel_mc AS articlematchcode, "
					   "t4.lagerfachnr AS storagelocation, t5.giuserlist, t5.deliverynotecode, t5.purchaseordersupplierno, "
					   "t5.headlastchangedate, t5.numofpurchaseorderpos, t5.headnotices, t5.giheadstatus, t5.manualfinishtime, "
					   "t5.purchaseordertype, t7.lant_nr as supplierno, t6.datum_bestell as purchaseorderdate, t6.datum_we as gidate, "
					   "nvl(t8.dauerae, 0) as permanentdiscount, nvl(t8.aktionsae, 0) as promotiondiscount, "
					   "nvl(t8.skonto, 0) as cashdiscountpct, nvl(t8.delkredereprozent, 0) as delcrederepct, "
					   "nvl(t8.bonusfaktor, 0) as bonusfactor, nvl (t8.gep, 0) as ppepurchasingprice, "
					   "nvl(t9.bestellart_pharmos, '99') as ordertypepharmos, t10.preis_bestell as realorderprice, "
					   "t10.preis_aktuell as orderpurchasingprice, t11.sap_lantnr as suppliersapno, "
					   "(SELECT CAST(COUNT(*) AS INTEGER) FROM gilabel "
					   "WHERE branchno=-90 AND purchaseorderno=4 AND articleno=t1.articleno) AS posnumoflabel, "
					   "(SELECT NVL(MIN(labelno), 0) FROM gilabel WHERE branchno=-90 AND purchaseorderno=4 AND "
					   "articleno=t1.articleno) AS firstlabelno, (SELECT CAST(nvl(min(stockbookingprice), 0.0) AS DECIMAL) "
					   "FROM gilabel WHERE branchno=-90 AND purchaseorderno=4 AND articleno=t1.articleno) "
					   "AS bookingpurchasingprice, "
					   "(SELECT CAST(NVL(MIN(s.stationno), 0) AS SMALLINT) FROM gilabel l, storagelocationarea sl, "
					   "corstorlocstation css, station s WHERE l.branchno=-90 AND l.purchaseorderno=4 AND "
					   "l.articleno=t1.articleno AND l.branchno=sl.branchno AND l.storagelocation>=sl.storlocfrom "
					   "AND l.storagelocation<=sl.storlocto AND sl.storlocareaid=css.storlocareaid AND css.moduleid=1 "
					   "AND css.stationid=s.stationid) AS destinationno, (SELECT CAST(SUM(l.labelqty) AS INTEGER) FROM gilabel l "
					   "LEFT OUTER JOIN gicausecode c ON l.causecodeno=c.causecodeno WHERE l.branchno=-90 AND l.purchaseorderno=4 "
					   "AND l.articleno=t1.articleno AND (c.stockrelflag=1 OR l.causecodeno=0)) AS receivedqty FROM giposition t1, "
					   "OUTER gicausecode t2, zartikel t3, artikelf t4, gihead t5, beskopf t6, bliefer t7, OUTER articlecondppe t8, "
					   "OUTER ekbestellarten t9, besposi t10, liefer t11 WHERE t1.branchno=-90 AND t1.purchaseorderno=4 "
					   "AND t1.causecodeno=t2.causecodeno AND t1.articleno=t3.artikel_nr AND t1.articleno=t4.artikel_nr "
					   "AND t4.filialnr=t1.branchno AND t1.branchno=t5.branchno AND t1.purchaseorderno=t5.purchaseorderno "
					   "AND t1.branchno=t6.filialnr AND t1.purchaseorderno=t6.bestell_nr AND t7.besla_nr = t5.purchaseordersupplierno "
					   "AND t8.artikel_nr = t1.articleno AND t8.lant_nr = t7.lant_nr AND t8.jahr = cast(to_char(current, '%Y') as integer) "
					   "AND t8.monat = cast(to_char(current, '%m') as integer) AND t9.bestellart_alt = t6.bestell_art "
					   "AND t10.bestell_nr = t6.bestell_nr AND t10.filialnr = t6.filialnr AND t10.artikel_nr = t1.articleno "
					   "AND t11.lant_nr = t7.lant_nr ORDER BY articlename, unity, firstlabelno";

	StatementRef stmtRef = connect.createStatement();
	ResultsetRef resRef  = stmtRef.executeQuery(stmt);

	UInt32 cnt = 0;

	printf("statement       : <%s>\n\n", stmt);
	printf("selected columns: %d\n", resRef.getSelectedCols());

	Decimal d1;

	while (true == resRef.next())
	{
		++cnt;

		d1 = resRef.getDecimal("bookingpurchasingprice");

		printf("fetched rows    : %d\n", resRef.getFetchedRows());

		printf("    value stockbookingprice  : %f\n"   , d1.toFloat64());
		printf("    value stockbookingprice  : %s\n"   , d1.toString().c_str());
	}

	printf("\n\n");
}

//------------------------------------------------------------------------------
void statementinfx(ConnectionRef connRef1, 
			       ConnectionRef connRef2)
{
#ifdef _ZZZZZZZZ_

	test_statement_execimminfx		(connRef2);
	test_statement_execquery		(connRef2);
	test_statement_execquery2		(connRef2);
	test_statement_execquery3		(connRef2);
	test_statement_execsinglequery	(connRef2);
	test_statement_execsinglequery2	(connRef2);

	test_statement_execnestedquery  (connRef1, connRef2);


	test_statement_execqueryparam		(connRef2);
	test_statement_execimmparam			(connRef2);
	test_statement_execsinglequeryparam	(connRef2);

	test_statement_execcursorinsert		(connRef2);
	test_statement_execcursorinsert2	(connRef2);
	test_statement_execcursorupdate		(connRef2);

	{
		basar::db::sql::DatabaseInfo  dbInfo;

		dbInfo.dbServer     = "onl_sofumi1_tcp";
		dbInfo.database     = "pos1g";		
		dbInfo.eConnectType = basar::INFORMIX;

		ConnectionRef connRefPos1g = basar::db::sql::Manager::getInstance().createConnect(dbInfo);


		test_statement_lvarchar (connRefPos1g); //pos1g

		connRefPos1g.close();
	}

	testinfx_holdcursor	(connRef2);
	testinfx_fetchbuffersize(connRef2);
	testinfx_update_cursorbehaviour(connRef2);
	testinfx_validresultset(connRef2);
	test_statement_date (connRef2);
	test_statement_execquery		(connRef2);

	prepinsert_infx(connRef2);
	test_statement_date_ext(connRef2);
	test_statement_date_ext2(connRef2);
	test_custom(connRef2);

	test_type_interval(connRef2);

	test_apr_insert(connRef2);
	test_apr_select(connRef2);


	test_serialid(connRef2);
	test_errorcodes(connRef2);
	test_prepared(connRef2);

	test_cmp_selectedDateTime(connRef2);
	test_RawNvlStringInfx(connRef2);

	test_checkstmt(connRef2);

#else

	test_checkstmt3(connRef2);

#endif
}

}	// namespace

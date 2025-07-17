//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   19.02.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include "odbcfix.h"

#include <string.h>
#include <iostream>

// Qt 4.4.3 ODBC Bug with prepared statements for Informix DB
//#define SKIP_ODBC_PREPSTMTS_QT_4_4_3_BUG

//------------------------------------------------------------------------------
SUITE(FixedStrings)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;
using basar::Int32;
using basar::UInt32;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
TEST(DefaultStringsInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		// connection default: raw fetching switched off

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(RawStringsInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.setRawFetching(true);	// string without right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21   ", ch2 .c_str());	// length 10
		CHECK_EQUAL("COLCH21"   , vch2.c_str());	// automatically righttrimmed by DB server
		CHECK      ( 0 != ::strcmp("COLCH21"   , ch2 .c_str()));
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(TrimStringsInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.setRawFetching(false);	// string with right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(Switch_Raw_Trim_Raw_Trim_StringsInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		for (Int32 i = 0; i < 2; ++i)
		{
			{
				connect.setRawFetching(true);	// string without right trim

				StatementRef  stmt    = connect.createStatement();
				ResultsetRef  result  = stmt.executeQuery(sql);

				CHECK(stmt.isOpen  ());	// stmt must be executed first!
				CHECK(result.isOpen());

				Int32     cnt = 0;
				Int16     smint;
				VarString ch2;
				VarString vch2;

				while (result.next())
				{
					smint = result.getInt16 ("colsmint");
					ch2   = result.getString("colch2"  );
					vch2  = result.getString("colvch2" );

					++cnt;
				}

				CHECK_EQUAL(   1     , cnt);
				CHECK_EQUAL(2001     , smint);
				CHECK_EQUAL("COLCH21   ", ch2 .c_str());	// length 10
				CHECK_EQUAL("COLCH21"   , vch2.c_str());	// automatically righttrimmed by DB server
				CHECK      ( 0 != ::strcmp("COLCH21"   , ch2 .c_str()));
				CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));
			}

			{
				connect.setRawFetching(false);	// string with right trim

				StatementRef  stmt    = connect.createStatement();
				ResultsetRef  result  = stmt.executeQuery(sql);

				CHECK(stmt.isOpen  ());	// stmt must be executed first!
				CHECK(result.isOpen());

				Int32     cnt = 0;
				Int16     smint;
				VarString ch2;
				VarString vch2;

				while (result.next())
				{
					smint = result.getInt16 ("colsmint");
					ch2   = result.getString("colch2"  );
					vch2  = result.getString("colvch2" );

					++cnt;
				}

				CHECK_EQUAL(   1     , cnt);
				CHECK_EQUAL(2001     , smint);
				CHECK_EQUAL("COLCH21", ch2 .c_str());
				CHECK_EQUAL("COLCH21", vch2.c_str());
				CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
				CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
			}
		}
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(PreparedStatementDefaultInfx)
{
	static ConstString sql = 
		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(PreparedStatementRawInfx)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());

	connect.setRawFetching(true);	// string without right trim

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(PreparedStatementTrimmedInfx)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());

	connect.setRawFetching(false);	// trimmed strings

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(RawNvlStringInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "

			"NVL(m.colint, '  ')               as col1, "
			"SUBSTR(NVL(m.colint, '  '), 1, 2) as col2, "

			"NVL(m.colvch2         , '  ')     as col3, "
			"NVL(m.colvch2::char(2), '  ')     as col4 "

		"FROM but_detail d, OUTER but_master m "
		"WHERE d.colser = m.colser "
		  "AND d.colser > 10;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.setRawFetching(true);

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString col1,
			      col2,
				  col3, 
				  col4;

		while (result.next())
		{
			col1  = result.getString("col1" );
			col2  = result.getString("col2" );
			col3  = result.getString("col3" );
			col4  = result.getString("col4" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		VarString::value_type blanks[41] = { 0 };
		memset(blanks, ' ', sizeof(blanks) - 1);


		CHECK_EQUAL(blanks,  col1.c_str());
		CHECK_EQUAL(sizeof(blanks) - 1, static_cast<UInt32>(strlen(col1.c_str())) );

		CHECK_EQUAL("  ",  col2.c_str());
		CHECK_EQUAL(2, static_cast<Int32>(strlen(col2.c_str())) );

		CHECK_EQUAL("",  col3.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col3.c_str())) );

		CHECK_EQUAL("  ",  col4.c_str());
		CHECK_EQUAL(2, static_cast<Int32>(strlen(col4.c_str())) );
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(TrimNvlStringInfx)
{
	static ConstString sql = 

		"SELECT FIRST 1 "

			"NVL(m.colint, '  ')               as col1, "
			"SUBSTR(NVL(m.colint, '  '), 1, 2) as col2, "

			"NVL(m.colvch2         , '  ')     as col3, "
			"NVL(m.colvch2::char(2), '  ')     as col4 "

		"FROM but_detail d, OUTER but_master m "
		"WHERE d.colser = m.colser "
		  "AND d.colser > 10;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.setRawFetching(false);

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString col1,
			      col2,
				  col3, 
				  col4;

		while (result.next())
		{
			col1  = result.getString("col1" );
			col2  = result.getString("col2" );
			col3  = result.getString("col3" );
			col4  = result.getString("col4" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		CHECK_EQUAL("", col1.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col1.c_str())) );

		CHECK_EQUAL("",  col2.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col2.c_str())) );

		CHECK_EQUAL("",  col3.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col3.c_str())) );

		CHECK_EQUAL("",  col4.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col4.c_str())) );
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#ifdef HAS_ODBC

TEST_FIXTURE(OdbcFix, DefaultStringsODBC)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		// connection default: raw fetching switched off

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, RawStringsODBC)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		connect.setRawFetching(true);	// string without right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21   ", ch2 .c_str());	// length 10
		CHECK_EQUAL("COLCH21"   , vch2.c_str());	// automatically righttrimmed by DB server
		CHECK      ( 0 != ::strcmp("COLCH21"   , ch2 .c_str()));
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, TrimStringsODBC)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		connect.setRawFetching(false);	// string with right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, PreparedStatementDefaultODBC_good)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colint = ? "
		"WHERE  colint = ? and colch1 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	connect.begin();

	try
	{
		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);

		Int32 i = 0;
		stmt.setInt32 (i++, 2);
		stmt.setInt32 (i++, 3);
		stmt.setString(i++, "a");

		// fixed in Qt 4.4.3; SQL error for Qt 4.3.4 (no issue with Qt 4.1.4): 
		// see $QTDIR/src/sql/drivers/odbc/qsql_odbc.cpp (4.3.4): 
		// QODBCResult::exec():
		// {
		//    ...
		//    switch (val.type())
		//       ...
		//		 case default: 
		//          ...
		//          SQLBindParameter(...,
        //                           SQL_C_CHAR,
		//							 SQL_LONGVARCHAR,	// better: ba.length() > 254 ? SQL_LONGVARCHAR : SQL_VARCHAR,
		//                           ...
		//CHECK_THROW(ret = stmt.execute(), DbException);

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(0, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, PreparedStatementDefaultODBC)
{
#ifdef SKIP_ODBC_PREPSTMTS_QT_4_4_3_BUG
	CHECK(true);
	std::cout << "TEST(PreparedStatementDefaultODBC) skipped "
		         "because of Qt 4.4.3 ODBC bug with prepared statements for Informix DB" 
		      << std::endl;
#else
	static ConstString sql = 
		
		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
#endif
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, PreparedStatementRawODBC)
{
#ifdef SKIP_ODBC_PREPSTMTS_QT_4_4_3_BUG
	CHECK(true);
	std::cout << "TEST(PreparedStatementRawODBC)     skipped "
		         "because of Qt 4.4.3 ODBC bug with prepared statements for Informix DB" 
		      << std::endl;
#else
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());

	connect.setRawFetching(true);	// string without right trim

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
#endif
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, PreparedStatementTrimmedODBC)
{
#ifdef SKIP_ODBC_PREPSTMTS_QT_4_4_3_BUG
	CHECK(true);
	std::cout << "TEST(PreparedStatementTrimmedODBC) skipped "
		         "because of Qt 4.4.3 ODBC bug with prepared statements for Informix DB" 
		      << std::endl;
#else
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());

	connect.setRawFetching(false);	// trimmed strings

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
#endif
}

//------------------------------------------------------------------------------

// MS SQL
TEST(DefaultStringsMSSQL_ODBC)
{
	static ConstString sql = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		CHECK(connect.isOpen());

		// connection default: raw fetching switched off

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(RawStringsMSSQL_ODBC)
{
	static ConstString sql = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		CHECK(connect.isOpen());

		connect.setRawFetching(true);	// string without right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21   ", ch2 .c_str());	// length 10
		CHECK_EQUAL("COLCH21"   , vch2.c_str());	// automatically righttrimmed by DB server
		CHECK      ( 0 != ::strcmp("COLCH21"   , ch2 .c_str()));
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(TrimStringsMSSQL_ODBC)
{
	static ConstString sql = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		CHECK(connect.isOpen());

		connect.setRawFetching(false);	// string with right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());
		CHECK      ( 0 != ::strcmp("COLCH21   ", ch2 .c_str()));	// length 10
		CHECK      ( 0 != ::strcmp("COLCH21   ", vch2.c_str()));	// length 10
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(PreparedStatementDefaultMSSQL_ODBC_good)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colint = ? "
		"WHERE  colint = ? and colch1 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	connect.begin();

	try
	{
		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);

		Int32 i = 0;
		stmt.setInt32 (i++, 2);
		stmt.setInt32 (i++, 3);
		stmt.setString(i++, "a");

		// fixed in Qt 4.4.3; SQL error for Qt 4.3.4 (no issue with Qt 4.1.4): 
		// see $QTDIR/src/sql/drivers/odbc/qsql_odbc.cpp (4.3.4): 
		// QODBCResult::exec():
		// {
		//    ...
		//    switch (val.type())
		//       ...
		//		 case default: 
		//          ...
		//          SQLBindParameter(...,
        //                           SQL_C_CHAR,
		//							 SQL_LONGVARCHAR,	// better: ba.length() > 254 ? SQL_LONGVARCHAR : SQL_VARCHAR,
		//                           ...
		//CHECK_THROW(ret = stmt.execute(), DbException);

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(0, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(PreparedStatementDefaultMSSQL_ODBC)
{
	static ConstString sql = 
		
		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(PreparedStatementRawMSSQL_ODBC)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());

	connect.setRawFetching(true);	// string without right trim

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(PreparedStatementTrimmedMSSQL_ODBC)
{
	static ConstString sql = 

		"UPDATE but_var "
		"SET    colch2 = ?, colvch2 = ? "
		"WHERE  colch2 = ? AND colvch2 = ?;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());

	connect.setRawFetching(false);	// trimmed strings

	connect.begin();

	try
	{
		{
			static ConstString prep = "INSERT INTO but_var (colch2, colvch2) values ('ZYXWVUT', 'TUVWXYZ');";

			StatementRef      stmt = connect.createStatement();
			ExecuteReturnInfo ret  = stmt.execute(prep);

			CHECK      (!ret.hasError());
			CHECK_EQUAL(1, (int)ret.m_affectedRows);
		}

		PreparedStatementRef  stmt;
		ExecuteReturnInfo     ret;

		// --- trimmed strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "a2345");
		stmt.setString(1, "b2345");
		stmt.setString(2, "ZYXWVUT");
		stmt.setString(3, "TUVWXYZ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createPreparedStatement(sql);
		stmt.setString(0, "c2345     ");
		stmt.setString(1, "d2345     ");
		stmt.setString(2, "a2345     ");
		stmt.setString(3, "b2345     ");

		ret = stmt.execute();
		CHECK      (!ret.hasError());
		CHECK_EQUAL(1, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
#endif	// HAS_ODBC

//------------------------------------------------------------------------------
#ifdef HAS_CICS

//------------------------------------------------------------------------------
TEST(DefaultStringsCICS)
{
	static ConstString sql = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK( ! connect.isOpen() );
		connect.open();
		CHECK(connect.isOpen());

		// connection default: raw fetching switched off

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString fil;
		VarString name;
		VarString nr;

		while (result.next())
		{
			fil  = result.getString("par_k_fil");
			name = result.getString("par_k_name"  );
			nr   = result.getString("par_k_nr" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		CHECK_EQUAL("01", fil .c_str());
		CHECK_EQUAL("01", name.c_str());
		CHECK_EQUAL("03", nr  .c_str());

		CHECK( 0 != ::strcmp("01 " , fil .c_str()));
		CHECK( 0 != ::strcmp("01  ", name.c_str()));
		CHECK( 0 != ::strcmp("03 " , nr  .c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(RawStringsCICS)
{
	static ConstString sql = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK( ! connect.isOpen() );
		connect.open();
		CHECK(connect.isOpen());

		connect.setRawFetching(true);	// string without right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString fil;
		VarString name;
		VarString nr;

		while (result.next())
		{
			fil  = result.getString("par_k_fil");
			name = result.getString("par_k_name"  );
			nr   = result.getString("par_k_nr" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		CHECK_EQUAL("01"  , fil .c_str());
		CHECK_EQUAL("01  ", name.c_str());
		CHECK_EQUAL("03"  , nr  .c_str());

		CHECK( 0 != ::strcmp("01 ", fil .c_str()));
		CHECK( 0 != ::strcmp("01" , name.c_str()));
		CHECK( 0 != ::strcmp("03 ", nr  .c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(TrimStringsCICS)
{
	static ConstString sql = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK( ! connect.isOpen() );
		connect.open();
		CHECK(connect.isOpen());

		connect.setRawFetching(false);	// string with right trim

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString fil;
		VarString name;
		VarString nr;

		while (result.next())
		{
			fil  = result.getString("par_k_fil");
			name = result.getString("par_k_name"  );
			nr   = result.getString("par_k_nr" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		CHECK_EQUAL("01", fil .c_str());
		CHECK_EQUAL("01", name.c_str());
		CHECK_EQUAL("03", nr  .c_str());

		CHECK( 0 != ::strcmp("01 " , fil .c_str()));
		CHECK( 0 != ::strcmp("01  ", name.c_str()));
		CHECK( 0 != ::strcmp("03 " , nr  .c_str()));
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(PreparedStatementDefaultCICS)
{
	static ConstString sqlstmt = 

		"UPDATE vsam.dkpar_ut "
		"SET    dummy = '%s' "
		"WHERE  par_k_name = '%s';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	CHECK( ! connect.isOpen() );
	connect.open();
	CHECK(connect.isOpen());

	// connection default: raw fetching switched off

	try
	{
		StatementRef      stmt;
		ExecuteReturnInfo ret;
		VarString         sql;

		// --- trimmed strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: a2345)", "01");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: b2345)   ", "01  ");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(PreparedStatementRawCICS)
{
	static ConstString sqlstmt = 

		"UPDATE vsam.dkpar_ut "
		"SET    dummy = '%s' "
		"WHERE  par_k_name = '%s';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	CHECK( ! connect.isOpen() );
	connect.open();
	CHECK(connect.isOpen());

	connect.setRawFetching(true);	// string without right trim

	try
	{
		StatementRef          stmt;
		ExecuteReturnInfo     ret;
		VarString             sql;

		// --- trimmed strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: c2345)", "01");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: d2345)   ", "01  ");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(PreparedStatementTrimmedCICS)
{
	static ConstString sqlstmt = 

		"UPDATE vsam.dkpar_ut "
		"SET    dummy = '%s' "
		"WHERE  par_k_name = '%s';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	CHECK( ! connect.isOpen() );
	connect.open();
	CHECK(connect.isOpen());

	connect.setRawFetching(false);	// trimmed strings

	try
	{
		StatementRef      stmt;
		ExecuteReturnInfo ret;
		VarString         sql;

		// --- trimmed strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: e2345)", "01");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);

		// --- raw strings

		stmt = connect.createStatement();
		sql.format(sqlstmt, "BasarUnitTest (prepared statement test: f2345)   ", "01  ");

		ret = stmt.execute(sql);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#endif	// HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

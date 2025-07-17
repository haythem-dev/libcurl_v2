#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include "odbcfix.h"

#include <iostream>

//------------------------------------------------------------------------------
SUITE(StoredProc)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int32;

using basar::db::sql::DbException;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
TEST(InfxExecProc0)
{
	static ConstString sqlStmt = "EXECUTE PROCEDURE but_proc0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret     = stmt.execute(sqlStmt);

		CHECK(!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecProc1)
{
	static ConstString sqlStmt = "EXECUTE PROCEDURE but_proc1(4);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret     = stmt.execute(sqlStmt);

		CHECK(!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc0_a)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt  = 0;
		Int32 col0 = 0;

		while (result.next())
		{
			++cnt;

			col0 = result.getInt32(0);

			CHECK_EQUAL(10, col0);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc0_b)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt  = 0;
		Int32 col0 = 0;

		while (result.next())
		{
			++cnt;

			col0 = result.getInt32(0);

			CHECK_EQUAL(10, col0);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc1)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func1();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt   = 0;
		Int32 col0a = 0;
		Int32 col0b = 0;

		while (result.next())
		{
			++cnt;

			col0a = result.getInt32(0);
			col0b = result.getInt32("ret0");

			CHECK_EQUAL(20, col0a);
			CHECK_EQUAL(20, col0b);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc2)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func2(6);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt   = 0;

		while (result.next())
		{
			++cnt;

			CHECK_EQUAL( 6, result.getInt32("ret0"));
			CHECK_EQUAL(24, result.getInt32("ret1"));
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc3a)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func3(13);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		CHECK_THROW(result.next(), DbException);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(InfxExecFunc3b)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func3(13);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
			CHECK(0 != result.getInt32("ret0"));

			CHECK_EQUAL(100014 + cnt, result.getInt32("ret1"));
			CHECK_EQUAL(  1988 + cnt, result.getInt16("ret2"));

			++cnt;
		}

		CHECK_EQUAL(10, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#ifdef HAS_ODBC

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecProc0)
{
	static ConstString sqlStmt = "EXECUTE PROCEDURE but_proc0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret     = stmt.execute(sqlStmt);

		CHECK(!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecProc1)
{
	static ConstString sqlStmt = "EXECUTE PROCEDURE but_proc1(4);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret     = stmt.execute(sqlStmt);

		CHECK(!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecFunc0_a)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt  = 0;
		Int32 col0 = 0;

		while (result.next())
		{
			++cnt;

			col0 = result.getInt32(0);

			CHECK_EQUAL(10, col0);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecFunc0_b)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func0();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt  = 0;
		Int32 col0 = 0;

		while (result.next())
		{
			++cnt;

			col0 = result.getInt32(0);

			CHECK_EQUAL(10, col0);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecFunc1)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func1();";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt   = 0;
		Int32 col0a = 0;
		Int32 col0b = 0;

		while (result.next())
		{
			++cnt;

			col0a = result.getInt32(0);
			col0b = result.getInt32("ret0");

			CHECK_EQUAL(20, col0a);
			CHECK_EQUAL(20, col0b);
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecFunc2)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func2(6);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeSingleQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt   = 0;

		while (result.next())
		{
			++cnt;

			CHECK_EQUAL(6 , result.getInt32("ret0"));
			CHECK_EQUAL(24, result.getInt32("ret1"));
		}

		CHECK_EQUAL(1, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcExecFunc3)
{
	static ConstString sqlStmt = "EXECUTE FUNCTION but_func3(14);";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ResultsetRef      result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
			++cnt;

			CHECK(0 != result.getInt32("ret0"));

			while (result.next())
			{
				CHECK(0 != result.getInt32("ret0"));

				CHECK_EQUAL(100015 + cnt, result.getInt32("ret1"));
				CHECK_EQUAL(  1987 + cnt, result.getInt16("ret2"));

				++cnt;
			}
		}

		CHECK_EQUAL(10, cnt);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#endif // HAS_ODBC

//------------------------------------------------------------------------------
#ifdef HAS_CICS

// -------------------------------------------------------------------------------------
TEST(testdbsqlcics_storedproc)
{
    try
    {
        ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
        CHECK(!connRef.isOpen());
        connRef.open();

		ConstString sql = "CALL STP.TCASTP ('PSH501','19270000000SH5011');";

        StatementRef statement  = connRef.createStatement();
		ResultsetRef result     = statement.executeSingleQuery(sql);

		Int32 cnt = 0;

		VarString cola;
		VarString colb;
		VarString hlp1,
			      hlp2;

		const VarString::value_type CMP[] = "19270000000SH50";	// + "93KEINE DATEN"
		const VarString::size_type  LEN   = sizeof(CMP) - 1;	// minus '\0'

		while (result.next())
		{
			cola = result.getString(0);
			colb = result.getString("param");
			hlp1 = cola.substr( 0, 4);
			hlp2 = colb.substr(11, 4);

			CHECK_EQUAL(cola.c_str(), colb.c_str());
			CHECK(LEN < cola.size());

			CHECK_EQUAL(VarString(CMP).substr( 0, 4).c_str(), hlp1.c_str());	// "1927"
			CHECK_EQUAL(VarString(CMP).substr(11, 4).c_str(), hlp2.c_str());	// "SH50"

			++cnt;
		}

		CHECK_EQUAL(1, cnt);
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured2 = false;
        CHECK(ExceptionOccured2);
    }
	catch ( ... )
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

//------------------------------------------------------------------------------
#endif // HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

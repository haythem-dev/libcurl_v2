#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasarcmnutil.h"

#include "testconnection.h"
#include "odbcfix.h"

#include <iostream>

#ifdef _WIN32
// Informix CSDK: evaluate define to detect CSDK version
#include <gls.h>
#endif

//------------------------------------------------------------------------------
SUITE(StmtTypes)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::Int32;
using basar::Float64;
using basar::Decimal;
using basar::InvalidStateException;
using basar::InvalidParameterException;

using basar::db::sql::Manager;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ConnectionRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
TEST(InfxStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST(InfxStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef  stmt    = connect.createStatement();
		
		CHECK_THROW(stmt.executeQuery(sqlStmt), DbException);
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
TEST(InfxStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

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
TEST(InfxStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();
		
		CHECK_THROW(stmt.execute(sqlStmt), DbException);
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
TEST(InfxStmtExecAggregate_Good)
{
	static ConstString sqlStmt = 

		"SELECT COUNT(*) "
		"FROM   but_master;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef  stmt    = connect.createStatement();
		Decimal       cnt     = stmt.executeAggregate(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK_CLOSE(10.0, cnt.toFloat64(), 0.0);
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
TEST(InfxStmtExecAggregate_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef  stmt    = connect.createStatement();

		CHECK_THROW(stmt.executeAggregate(sqlStmt), InvalidStateException);
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
TEST(InfxPrepStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::INFX);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ResultsetRef         result  = stmt.executeQuery();

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST(InfxPrepStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::INFX);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		
		CHECK_THROW(stmt.executeQuery(), DbException);
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
TEST(InfxPrepStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::INFX);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ExecuteReturnInfo    ret     = stmt.execute();

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
TEST(InfxPrepStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::INFX);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		
		CHECK_THROW(stmt.execute(), DbException);
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
TEST(InfxUnload_Bad)
{
	static ConstString sqlStmt = 

		"UNLOAD TO but_master.data "
		"SELECT * "
		"FROM   but_master;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef      stmt    = connect.createStatement();

		CHECK_THROW ( stmt.execute(sqlStmt), DbException);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
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

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, OdbcStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST_FIXTURE(OdbcFix, OdbcStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result;
		
		result = stmt.executeQuery(sqlStmt);
		CHECK_EQUAL(false, result.next());
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
TEST_FIXTURE(OdbcFix, OdbcStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

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
TEST_FIXTURE(OdbcFix, OdbcStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret;
		
		ret = stmt.execute(sqlStmt);
		CHECK_EQUAL(false, ret.hasError());
		CHECK_EQUAL(-1   , (int)ret.m_affectedRows);	// Qt ODBC: couldn't be determined
		CHECK_EQUAL(static_cast<unsigned int>(0), static_cast<unsigned int>(ret.m_lastSerial.size()));	
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
TEST_FIXTURE(OdbcFix, OdbcStmtExecAggregate_Good)
{
	static ConstString sqlStmt = 

		"SELECT COUNT(*) "
		"FROM   but_master;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef  stmt    = connect.createStatement();
		Decimal       cnt     = stmt.executeAggregate(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK_CLOSE(10.0, cnt.toFloat64(), 0.0);
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
TEST_FIXTURE(OdbcFix, OdbcStmtExecAggregate_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef  stmt    = connect.createStatement();

		CHECK_THROW(stmt.executeAggregate(sqlStmt), InvalidStateException);
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
TEST_FIXTURE(OdbcFix, OdbcPrepStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ResultsetRef         result  = stmt.executeQuery();

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST_FIXTURE(OdbcFix, OdbcPrepStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ResultsetRef         result;
		
		result = stmt.executeQuery();
		CHECK_EQUAL(false, result.next());
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
TEST_FIXTURE(OdbcFix, OdbcPrepStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SET ISOLATION TO DIRTY READ;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ExecuteReturnInfo    ret     = stmt.execute();

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
TEST_FIXTURE(OdbcFix, OdbcPrepStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ExecuteReturnInfo    ret;
		
		ret = stmt.execute();
		CHECK_EQUAL(false, ret.hasError());
		CHECK_EQUAL(-1   , (int)ret.m_affectedRows);	// Qt ODBC: couldn't be determined
		CHECK_EQUAL(static_cast<unsigned int>(0), static_cast<unsigned int>(ret.m_lastSerial.size()));	
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

#ifdef _WIN32
// checking %INFORMIXDIR%/incl/esql/gls.h
#if GL_NUMVER < 600004 
//------------------------------------------------------------------------------
// needed by CSDK < 4.10 before switching from Informix ODBC to MS SQL Server ODBC
TEST(Dummy_for_Cleanup_Informix)
{
	DatabaseInfo dbESQLC;
	dbESQLC.setInfx(ESQLC_DBSERVER, ESQLC_DB_WEST);

	ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);
}
#endif
#endif

//------------------------------------------------------------------------------
// MS SQL
//------------------------------------------------------------------------------

TEST(MSSQLOdbcStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST(MSSQLOdbcStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result;
		
		result = stmt.executeQuery(sqlStmt);
		CHECK_EQUAL(false, result.next());
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
TEST(MSSQLOdbcStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
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
TEST(MSSQLOdbcStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret;
		
		ret = stmt.execute(sqlStmt);
		CHECK_EQUAL(false, ret.hasError());
		CHECK_EQUAL(-1   , (int)ret.m_affectedRows);	// Qt ODBC: couldn't be determined
		CHECK_EQUAL(static_cast<unsigned int>(0), static_cast<unsigned int>(ret.m_lastSerial.size()));	
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
TEST(MSSQLOdbcStmtExecAggregate_Good)
{
	static ConstString sqlStmt = 

		"SELECT COUNT(*) "
		"FROM   but_master;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		StatementRef  stmt    = connect.createStatement();
		Decimal       cnt     = stmt.executeAggregate(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK_CLOSE(10.0, cnt.toFloat64(), 0.0);
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
TEST(MSSQLOdbcStmtExecAggregate_Bad)
{
	static ConstString sqlStmt = 

		"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		StatementRef  stmt    = connect.createStatement();

		CHECK_THROW(stmt.executeAggregate(sqlStmt), InvalidStateException);
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
TEST(MSSQLOdbcPrepStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ResultsetRef         result  = stmt.executeQuery();

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
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
TEST(MSSQLOdbcPrepStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ResultsetRef         result;
		
		result = stmt.executeQuery();
		CHECK_EQUAL(false, result.next());
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
TEST(MSSQLOdbcPrepStmtExecute_Good)
{
	static ConstString sqlStmt = 

        "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ExecuteReturnInfo    ret     = stmt.execute();

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
TEST(MSSQLOdbcPrepStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   but_master "
		"ORDER BY colint;";

	try
	{
		ConnectionRef        connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		PreparedStatementRef stmt    = connect.createPreparedStatement(sqlStmt);
		ExecuteReturnInfo    ret;
		
		ret = stmt.execute();
		CHECK_EQUAL(false, ret.hasError());
		CHECK_EQUAL(-1   , (int)ret.m_affectedRows);	// Qt ODBC: couldn't be determined
		CHECK_EQUAL(static_cast<unsigned int>(0), static_cast<unsigned int>(ret.m_lastSerial.size()));	
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

TEST(CicsStmtExecQuery_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   vsam.dkpar_ut "
		"WHERE  par_k_fil = '01' "
		"ORDER BY par_k_nr;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
			++cnt;
		}

		CHECK_EQUAL(3, cnt);
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
TEST(CicsStmtExecQuery_Bad)
{
	static ConstString sqlStmt = 

		"SELECT;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef  stmt    = connect.createStatement();
		
        CHECK_THROW(stmt.executeQuery(sqlStmt), DbException);
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
TEST(CicsStmtExecute_Bad)
{
	static ConstString sqlStmt = 

		"SELECT;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef      stmt    = connect.createStatement();

		CHECK_THROW(stmt.execute(sqlStmt), basar::db::sql::DbException);
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
TEST(CicsStmtExecute_Good)
{
	static ConstString sqlStmt = 

		"SELECT * "
		"FROM   vsam.dkpar_ut "
		"WHERE  par_k_fil = '01' "
		"ORDER BY par_k_nr;";

	try
	{
		ConnectionRef     connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef      stmt    = connect.createStatement();
		ExecuteReturnInfo ret;
		
		ret = stmt.execute(sqlStmt);
		CHECK_EQUAL(false, ret.hasError());
		CHECK_EQUAL(3    , (int)ret.m_affectedRows);	
		CHECK_EQUAL(static_cast<unsigned int>(0), static_cast<unsigned int>(ret.m_lastSerial.size()));	
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
TEST(CicsStmtExecAggregate_Good)
{
	static ConstString sqlStmt = 

		"SELECT COUNT(*) "
		"FROM   vsam.dkpar_ut "
		"WHERE  par_k_fil = '01';";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef  stmt    = connect.createStatement();
		Decimal       cnt     = stmt.executeAggregate(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK_CLOSE(3.0, cnt.toLong32(), 0.0);
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
TEST(CicsStmtExecAggregate_Bad)
{
	static ConstString sqlStmt = 

		"SELECT;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		connect.open();
		StatementRef  stmt    = connect.createStatement();

		CHECK_THROW(stmt.executeAggregate(sqlStmt), DbException);
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
#endif // HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

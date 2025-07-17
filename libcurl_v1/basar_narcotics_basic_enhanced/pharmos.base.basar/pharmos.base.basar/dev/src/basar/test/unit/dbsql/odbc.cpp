//---------------------------------------------------------------
#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include "odbcfix.h"

//---------------------------------------------------------------
using basar::db::sql::ConnectionRef;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::Manager;

using basar::test::unit::dbsql::TestConnection;

//---------------------------------------------------------------
SUITE(Odbc)
{

//---------------------------------------------------------------
#ifdef HAS_ODBC

//---------------------------------------------------------------
class Fixture
{
public:
	Fixture()
	{
		dbODBC .setODBC(ODBC_DSN_WEST);
		dbESQLC.setInfx(ESQLC_DBSERVER, ESQLC_DB_WEST);
	}

	~Fixture()
	{
	}

	basar::db::sql::DatabaseInfo dbODBC;
	basar::db::sql::DatabaseInfo dbESQLC;
	// issue with Informix CSDK 3.70 Windows 32 bit
	OdbcFix                      odbcfix;
};

//---------------------------------------------------------------
TEST_FIXTURE(Fixture, Infx_after_ODBC_failure_after_dtor)
{
	try
	{
		{
			ConnectionRef connODBC = Manager::getInstance().createConnect(dbODBC);
		}

		ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
// issue with Informix CSDK 3.70 Windows 32 bit (v2.90 is OK)
TEST_FIXTURE(Fixture, Infx_after_ODBC_failure_after_close)
{
	try
	{
		ConnectionRef connODBC = Manager::getInstance().createConnect(dbODBC);
		connODBC.close();

		ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
// issue with Informix CSDK 3.70 Windows 32 bit (v2.90 is OK)
TEST_FIXTURE(Fixture, Infx_after_ODBC_failure_after_close_same_ConnectionRef)
{
	try
	{
		ConnectionRef conn;

		conn = Manager::getInstance().createConnect(dbODBC);
		conn.close();

		conn = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
TEST_FIXTURE(Fixture, Infx_after_ODBC_success_same_ConnectionRef)
{
	try
	{
		ConnectionRef conn;

		// d'tor close() ?
		conn = Manager::getInstance().createConnect(dbODBC);
		conn = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
TEST_FIXTURE(Fixture, Infx_after_ODBC_success)
{
	try
	{
		ConnectionRef connODBC  = Manager::getInstance().createConnect(dbODBC);
		ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
#ifdef HAS_CICS

TEST_FIXTURE(Fixture, CICS_after_ODBC)
{
	try
	{
		{
			ConnectionRef connODBC = Manager::getInstance().createConnect(dbODBC);
		}

		ConnectionRef connCICS = TestConnection::createTestConnection(TestConnection::CICS);
	}
	catch (...)
	{
		CHECK(false);
	}
}

#endif

//---------------------------------------------------------------
TEST_FIXTURE(Fixture, ODBC_after_ODBC)
{
	try
	{
		{
			ConnectionRef connODBC = Manager::getInstance().createConnect(dbODBC);
		}

		ConnectionRef connESQLC = Manager::getInstance().createConnect(dbODBC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
TEST_FIXTURE(Fixture, Infx_after_Infx)
{
	try
	{
		{
			ConnectionRef conn1 = Manager::getInstance().createConnect(dbESQLC);
		}

		ConnectionRef conn2 = Manager::getInstance().createConnect(dbESQLC);
	}
	catch (...)
	{
		CHECK(false);
	}
}

//---------------------------------------------------------------
#endif	// HAS_ODBC

//---------------------------------------------------------------
}	// SUITE

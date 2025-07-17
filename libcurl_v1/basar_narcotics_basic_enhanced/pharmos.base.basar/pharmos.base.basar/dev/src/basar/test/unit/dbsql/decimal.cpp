//----------------------------------------------------------------------------
/** $Id: decimal.cpp$
 *  \author Michael Eichenlaub
 *  \date   18.05.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasardbsql_manager.h"
#include "libbasarcmnutil.h"

#include "testconnection.h"
#include <iostream>

//------------------------------------------------------------------------------
SUITE(Decimal)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;
using basar::Int32;
using basar::UInt32;
using basar::ULong32;
using basar::Decimal;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;
using basar::db::sql::Manager;

using basar::test::unit::dbsql::TestConnection;


//------------------------------------------------------------------------------
TEST(calc)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"coldec, "
			"2803.34::decimal(12,3) as val0, "
			"  99.99::decimal( 6,3) as val1, "
			"2803.34::decimal(12,3) * (99.99::decimal(6,3) / 100.0) as val2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString ms_sql = 

		"SELECT TOP 1 "
			"coldec, "
			"cast (2803.34 as decimal(12,3)) as val0, "
			"cast (99.99 as decimal( 6,3)) as val1, "
			"cast (2803.34 as decimal(12,3)) * (cast (99.99 as decimal(6,3)) / 100.0) as val2 "
		"FROM   but_master "
		"ORDER BY colint;";

	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
		TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();

		CHECK_EQUAL(static_cast<ULong32> (conns.size()), static_cast<ULong32> (Manager::getInstance().size    ()));
		CHECK_EQUAL(static_cast<ULong32> (conns.size()), static_cast<ULong32> (Manager::getInstance().sizeOpen()));

		for (TestConnection::TestConnectionMap::iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			eConnType = itConn->first;
	
			StatementRef  stmt    = itConn->second.createStatement();
			ResultsetRef  result;
			
			if (TestConnection::MSSQL_ODBC == eConnType)
			{
			    result = stmt.executeQuery(ms_sql);
			}
			else
			{
			    result = stmt.executeQuery(sql);
			}

			Int32   cnt = 0;

			Decimal coldec;
			Decimal col0;
			Decimal col1;
			Decimal col2;

			Decimal calc;

			while (result.next())
			{
				coldec = result.getDecimal("coldec");

				col0   = result.getDecimal("val0");
				col1   = result.getDecimal("val1");
				col2   = result.getDecimal("val2");

				++cnt;
			}

			CHECK_EQUAL(   1  , cnt   );

			// check selected columns
			CHECK_EQUAL("500001.61"    , coldec.toNonLocString().c_str());

			CHECK_EQUAL(  "2803.34"    , col0  .toNonLocString().c_str());
			CHECK_EQUAL(    "99.99"    , col1.  toNonLocString().c_str());
			CHECK_EQUAL(  "2803.059666", col2.  toNonLocString().c_str());

			calc = col0 * (col1 / Decimal(100.0));
			CHECK_EQUAL("2803.059666", calc.toNonLocString().c_str());

		}
	}
    catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
}	// SUITE


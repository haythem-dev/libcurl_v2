//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   19.02.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include <iostream>

#define __SWITCH_COLUMNTESTS__
#ifdef  __SWITCH_COLUMNTESTS__

//------------------------------------------------------------------------------
SUITE(ColNames)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::Int32;
using basar::UInt32;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
TEST(ColumnNamesInformix)
{
	const TestConnection::TestConnectionType CONN[] = 
	{
#ifdef HAS_ODBC
		TestConnection::ODBC,
		TestConnection::MSSQL_ODBC,
#endif
		TestConnection::INFX
	};

	static ConstString sql = 

		"SELECT FIRST 1 "
			"colser , colser  AS COL0_UPPER, colser  AS col0_lower, "
			"colch2 , colch2  AS COL1_UPPER, colch2  AS col1_lower, "
			"colvch2, colvch2 AS COL2_UPPER, colvch2 AS col2_lower, "
			"coldate, coldate AS COL3_UPPER, coldate AS col3_lower, "
			"coldt1 , coldt1  AS COL4_UPPER, coldt1  AS col4_lower  "
		"FROM   but_master";

    //different syntax in T-SQL
    static ConstString ms_sql =
    	
    	"SELECT TOP 1 "
			"colser , colser  AS COL0_UPPER, colser  AS col0_lower, "
			"colch2 , colch2  AS COL1_UPPER, colch2  AS col1_lower, "
			"colvch2, colvch2 AS COL2_UPPER, colvch2 AS col2_lower, "
			"coldate, coldate AS COL3_UPPER, coldate AS col3_lower, "
			"coldt1 , coldt1  AS COL4_UPPER, coldt1  AS col4_lower  "
		"FROM   but_master";

	try
	{
		ConnectionRef connect;

		for (size_t i = 0; i < sizeof(CONN)/sizeof(CONN[0]); ++i)
		{
			connect = TestConnection::createTestConnection(CONN[i]);
			CHECK(connect.isOpen());

			StatementRef  stmt    = connect.createStatement();
			
			ResultsetRef result;
			
			if (TestConnection::MSSQL_ODBC == CONN[i])
			{
			    result  = stmt.executeQuery(ms_sql);
			}
			else
			{
			    result  = stmt.executeQuery(sql);
			}

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			CHECK_EQUAL((UInt32)15, result.getSelectedCols());

			CHECK( result.isExist("colser"    ));
			CHECK(!result.isExist("COLSER"    ));
			CHECK( result.isExist("col0_upper"));
			CHECK(!result.isExist("COL0_UPPER"));
			CHECK( result.isExist("col0_lower"));
			CHECK(!result.isExist("COL0_LOWER"));

			CHECK( result.isExist("colch2"    ));
			CHECK(!result.isExist("COLCH2"    ));
			CHECK( result.isExist("col1_upper"));
			CHECK(!result.isExist("COL1_UPPER"));
			CHECK( result.isExist("col1_lower"));
			CHECK(!result.isExist("COL1_LOWER"));

			CHECK( result.isExist("colvch2"   ));
			CHECK(!result.isExist("COLvCH2"   ));
			CHECK( result.isExist("col2_upper"));
			CHECK(!result.isExist("COL2_UPPER"));
			CHECK( result.isExist("col2_lower"));
			CHECK(!result.isExist("COL2_LOWER"));

			CHECK( result.isExist("coldate"   ));
			CHECK(!result.isExist("COLDATE"   ));
			CHECK( result.isExist("col3_upper"));
			CHECK(!result.isExist("COL3_UPPER"));
			CHECK( result.isExist("col3_lower"));
			CHECK(!result.isExist("COL3_LOWER"));

			CHECK( result.isExist("coldt1"    ));
			CHECK(!result.isExist("COLDT1"    ));
			CHECK( result.isExist("col4_upper"));
			CHECK(!result.isExist("COL4_UPPER"));
			CHECK( result.isExist("col4_lower"));
			CHECK(!result.isExist("COL4_LOWER"));

			Int32 cnt = 0;
			while (result.next())
				++cnt;

			CHECK_EQUAL(1, cnt);
		}
	}
    catch (...)
    {
        CHECK(false);
    }
}

//------------------------------------------------------------------------------

#ifdef HAS_CICS

TEST(ColumnNamesCICS)
{
	static ConstString sql = 

		"SELECT "
			"PAR_K_FIL    , PAR_K_FIL     AS COL0_UPPER, PAR_K_FIL     AS col0_lower, "
			"DKPAR_EUIHW  , DKPAR_EUIHW   AS COL1_UPPER, DKPAR_EUIHW   AS col1_lower, "
			"TEST_DATE    , TEST_DATE     AS COL2_UPPER, TEST_DATE     AS col2_lower, "
			"TEST_TIME    , TEST_TIME     AS COL3_UPPER, TEST_TIME     AS col3_lower, "
			"TEST_FLOAT2  , TEST_FLOAT2   AS COL4_UPPER, TEST_FLOAT2   AS col4_lower  "
		"FROM VSAM.DKPAR_UT";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK( ! connect.isOpen() );

		connect.open();
		CHECK( connect.isOpen() );

		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sql);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		CHECK_EQUAL((UInt32)15, result.getSelectedCols());

		CHECK( result.isExist("par_k_fil" ));
		CHECK(!result.isExist("PAR_K_FIL" ));
		CHECK( result.isExist("col0_upper"));
		CHECK(!result.isExist("COL0_UPPER"));
		CHECK( result.isExist("col0_lower"));
		CHECK(!result.isExist("COL0_LOWER"));

		CHECK( result.isExist("dkpar_euihw"));
		CHECK(!result.isExist("DKPAR_EUIHW"));
		CHECK( result.isExist("col1_upper" ));
		CHECK(!result.isExist("COL1_UPPER" ));
		CHECK( result.isExist("col1_lower" ));
		CHECK(!result.isExist("COL1_LOWER" ));

		CHECK( result.isExist("test_date" ));
		CHECK(!result.isExist("TEST_DATE" ));
		CHECK( result.isExist("col2_upper"));
		CHECK(!result.isExist("COL2_UPPER"));
		CHECK( result.isExist("col2_lower"));
		CHECK(!result.isExist("COL2_LOWER"));

		CHECK( result.isExist("test_time"    ));
		CHECK(!result.isExist("TEST_TIME"    ));
		CHECK( result.isExist("col3_upper"   ));
		CHECK(!result.isExist("COL3_UPPER"   ));
		CHECK( result.isExist("col3_lower"   ));
		CHECK(!result.isExist("COL3_LOWER"   ));

		CHECK( result.isExist("test_float2"));
		CHECK(!result.isExist("TEST_FLOAT2"));
		CHECK( result.isExist("col4_upper"));
		CHECK(!result.isExist("COL4_UPPER"));
		CHECK( result.isExist("col4_lower"));
		CHECK(!result.isExist("COL4_LOWER"));

		CHECK(result.next());
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

#endif	// CICS

//------------------------------------------------------------------------------
}	// SUITE

#endif	// switch

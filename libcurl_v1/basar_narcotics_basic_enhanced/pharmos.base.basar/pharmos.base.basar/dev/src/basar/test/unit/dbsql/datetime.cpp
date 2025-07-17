//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   14.02.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasarcmnutil.h"

#include "testconnection.h"
#include <iostream>

//------------------------------------------------------------------------------
SUITE(DateTime)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::I18nString;
using basar::Int16;
using basar::Int32;
using basar::UInt32;
using basar::ULong32;
using basar::DateTime;
using basar::Time;
using basar::Date;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;
using basar::db::sql::Manager;

using basar::test::unit::dbsql::TestConnection;


//------------------------------------------------------------------------------
TEST(AddTime)
{
	static ConstString sql = 

		"SELECT FIRST 1 "
			"colint, "
			"coldate, coldt1, coldt2, coltime1, coltime2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString ms_sql = 

		"SELECT TOP 1 "
			"colint, "
			"coldate, coldt1, coldt2, coltime1, coltime2 "
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
			

			Int32     cnt = 0;
			Int32     colint;
			DateTime  colYs;
			DateTime  colYt;
			Time      colhs;	// because of ODBC
			DateTime  colht;

            Date      colDate;

			while (result.next())
			{
				colint = result.getInt32   ("colint");

				colYs  = result.getDateTime("coldt1");
				colYt  = result.getDateTime("coldt2");

                if (TestConnection::MSSQL_ODBC == eConnType)
                {
                    //MSSQL 2005 does not support datatype DATE
                    DateTime dt = result.getDateTime("coldate");
                    colDate = dt.getDate();
                    
                    //MSSQL 2005 does not support datatype TIME
                    colhs = result.getDateTime("coltime1");
                }
                else
                {
                    colDate = result.getDate("coldate");
                    colhs  = result.getTime ("coltime1");	// because of ODBC
                }

				
				colht  = result.getDateTime("coltime2");	// because of ODBC

				++cnt;
			}

			CHECK_EQUAL(   1  , cnt   );

			// check selected columns

			CHECK_EQUAL(100001, colint);

			CHECK_EQUAL( 87640530, colYs.getDate());
			CHECK_EQUAL(205956000, colYs.getTime());

			CHECK_EQUAL( 87640530, colYt.getDate());
			CHECK_EQUAL( 52758990, colYt.getTime());

			CHECK_EQUAL(DateTime::NULL_DATE, colhs.getDate());
			CHECK_EQUAL(235300000          , colhs.getTime());

			CHECK_EQUAL(DateTime::NULL_DATE, colht.getDate());
		    CHECK_EQUAL(235905030          , colht.getTime());


			// check add methods
			colYs.addDays(-9);
			CHECK_EQUAL( 87640521, colYs.getDate());
			CHECK_EQUAL(205956000, colYs.getTime());

			colYs.addTime(-500000);
			CHECK_EQUAL( 87640521, colYs.getDate());
			CHECK_EQUAL(205456000, colYs.getTime());

			colYs.addDays(-19);
			CHECK_EQUAL( 87640502, colYs.getDate());
			CHECK_EQUAL(205456000, colYs.getTime());

			colYs.addTime(-459999);
			CHECK_EQUAL( 87640502, colYs.getDate());
			if ( (eConnType == TestConnection::ODBC) || (eConnType == TestConnection::ODBC_BG)  ||
			        (TestConnection::MSSQL_ODBC == eConnType)     )
				CHECK_EQUAL(204956001, colYs.getTime());
			else
				CHECK_EQUAL(204956000, colYs.getTime());	// regard: year to second => no millisec


			colYt.addTime(-500000);
			CHECK_EQUAL( 87640530, colYt.getDate());
			CHECK_EQUAL( 52258990, colYt.getTime());

			colYt.addDays(-19);
			CHECK_EQUAL( 87640511, colYt.getDate());
			CHECK_EQUAL( 52258990, colYt.getTime());

			colYt.addTime(-459999);
			CHECK_EQUAL( 87640511, colYt.getDate());
			CHECK_EQUAL( 51758991, colYt.getTime());


			colhs.addTime(-500000);
			CHECK_EQUAL(DateTime::NULL_DATE, colhs.getDate());
			CHECK_EQUAL(234800000          , colhs.getTime());

			colhs.addTime(-459999);
			CHECK_EQUAL(DateTime::NULL_DATE, colhs.getDate());
			if ( 
			        (eConnType == TestConnection::ODBC) || (eConnType == TestConnection::ODBC_BG) ||
			        (TestConnection::MSSQL_ODBC == eConnType)     
			   )
				CHECK_EQUAL(234300001, colhs.getTime());
			else
				CHECK_EQUAL(234300000, colhs.getTime());	// regard: hour to second => no millisec

			colht.addTime(-500000);
			CHECK_EQUAL(DateTime::NULL_DATE, colht.getDate());
		    CHECK_EQUAL(235405030          , colht.getTime());

			colht.addTime(-459999);
			CHECK_EQUAL(DateTime::NULL_DATE, colht.getDate());
		    CHECK_EQUAL(234905031          , colht.getTime());
		
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

TEST(DatetimeInStatementReturningNullValues)
{
	static ConstString sql = 
		"select bd.coldt1, bd.coltime1 from but_master bm left join but_detail bd on bm.colint = bd.colint";

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
			ResultsetRef  result  = stmt.executeQuery(sql);

			Int32     cnt = 0;
			DateTime  colYs;
			Time	  colhs;
            

			I18nString dt;
			I18nString ts;

			while (result.next())
			{				
				colYs  = result.getDateTime("coldt1");
				if (TestConnection::MSSQL_ODBC == eConnType)
				{
				    colhs  = result.getDateTime    ("coltime1");
				}
				else
				{
				    colhs  = result.getTime    ("coltime1");
				}
				
				
				
				dt = colYs.toStrTime();
				ts = colhs.toStrTime();
				++cnt;
			}

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


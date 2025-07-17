#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include <iostream>

#include "libbasarcmnutil.h"

// db::sql::
using basar::db::sql::ConnectionRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ExecuteReturnInfo;

//------------------------------------------------------------------------------
SUITE(TimeSpan)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;
using basar::Int32;
using basar::UInt32;
using basar::ULong32;
using basar::DateTime;
using basar::Time;
using basar::Date;
using basar::TimeSpan;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;
using basar::db::sql::Manager;

using basar::test::unit::dbsql::TestConnection;

    TEST(SelectTimeSpan)
    {
        static ConstString sql = 

		"SELECT FIRST 1 "
        "(coldt1 - coldt2) as myInterval "
		"FROM   but_master "
		"ORDER BY colint;";

        static ConstString ms_sql = 

		"SELECT TOP 1 "
        "(coldt1 - coldt2) as myInterval "
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
			        result  = stmt.executeQuery(ms_sql);
			    }
			    else
			    {
			        result  = stmt.executeQuery(sql);
			    }
			    

                Int32     cnt = 0;
                TimeSpan colTs;

			    while (result.next())
			    {
                    if (TestConnection::INFX == itConn->first || TestConnection::INFX_BG == itConn->first)
                    {
                        colTs = result.getTimeSpan("myInterval");
                        CHECK_EQUAL("00 15:31:58.000", colTs.toNormalizedString());
                    }
                    else
                    {
                        CHECK_THROW(result.getTimeSpan("myInterval"), basar::NotImplementedException);
                    }

				    ++cnt;
			    }

                CHECK_EQUAL(   1  , cnt   );

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

	TEST(InsertTimeSpanPrepared)
	{
		static ConstString sql = "insert into but_var (colspan1) values (?)";

		static ConstString stmt = "select colspan1 from but_var where colspan1 = ?";

		TestConnection::TestConnectionType eConnType    = TestConnection::INFX;

		TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
		TestConnection::TestConnectionMap ::const_iterator itConn;

		TimeSpan ts;

		ts.fromNormalizedString("25 12:00:00.000");

		//const basar::BULong expectedRows = 1;

		try
		{
			for (itConn = connRefs.begin(); itConn != connRefs.end(); ++itConn)
			{	
				eConnType = itConn->first;
				ConnectionRef connRef   = itConn->second;

				//testing informix connections only
				if (TestConnection::INFX == itConn->first || TestConnection::INFX_BG == itConn->first)
                {
					PreparedStatementRef statement;
					CHECK(statement.isNull());

					connRef.begin();

					statement = connRef.createPreparedStatement(sql);

					CHECK(!statement.isNull());
					CHECK(statement.isOpen());

					statement.setTimeSpan(0,ts);

					ExecuteReturnInfo	execResult = statement.execute();

					//CHECK_EQUAL(expectedRows, execResult.m_affectedRows);
					
					CHECK_EQUAL(basar::db::sql::DBErrorCode::SUCCESS, execResult.m_error);

					PreparedStatementRef statement2;

					statement2 = connRef.createPreparedStatement(stmt);

					statement2.setTimeSpan(0, ts);


					ResultsetRef resultSet2  = statement2.executeQuery();

					bool hasNext = resultSet2.next();

					CHECK(hasNext);

					if (hasNext)
					{
						TimeSpan res = resultSet2.getTimeSpan(0);
						CHECK_EQUAL("25 12:00:00.000", res.toNormalizedString());
					}

					

					connRef.rollback();
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

	TEST(InsertRangedTimeSpanPrepared)
	{
		static ConstString sql = "insert into but_var (colspan1) values (?)";

		static ConstString stmt = "select colspan1 from but_var where colspan1 = ?";

		TestConnection::TestConnectionType eConnType    = TestConnection::INFX;

		TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
		TestConnection::TestConnectionMap ::const_iterator itConn;

		TimeSpan ts;

		ts.fromString("%H:%M:%S%F3", "12:00:00.000");

		ts.setRange(TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC);

		//const basar::BULong expectedRows = 1;

		try
		{
			for (itConn = connRefs.begin(); itConn != connRefs.end(); ++itConn)
			{	
				eConnType = itConn->first;
				ConnectionRef connRef   = itConn->second;

				//testing informix connections only
				if (TestConnection::INFX == itConn->first || TestConnection::INFX_BG == itConn->first)
                {
					PreparedStatementRef statement;
					CHECK(statement.isNull());

					connRef.begin();

					statement = connRef.createPreparedStatement(sql);

					CHECK(!statement.isNull());
					CHECK(statement.isOpen());

					statement.setTimeSpan(0,ts);

					ExecuteReturnInfo	execResult = statement.execute();

					//CHECK_EQUAL(expectedRows, execResult.m_affectedRows);
					
					CHECK_EQUAL(basar::db::sql::DBErrorCode::SUCCESS, execResult.m_error);

					PreparedStatementRef statement2;

					statement2 = connRef.createPreparedStatement(stmt);

					statement2.setTimeSpan(0, ts);


					ResultsetRef resultSet2  = statement2.executeQuery();

					bool hasNext = resultSet2.next();

					CHECK(hasNext);

					if (hasNext)
					{
						TimeSpan res = resultSet2.getTimeSpan(0);
						CHECK_EQUAL("00 12:00:00.000", res.toNormalizedString());
					}

					

					connRef.rollback();
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

    TEST(TimeSpanInStatementReturningNullValues)
    {
	    static ConstString sql = 
		    "select bv.colspan1 from but_master bm left join but_var bv on bv.colser = bm.colint";

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
                if (TestConnection::INFX == itConn->first || TestConnection::INFX_BG == itConn->first)
                {
			        eConnType = itConn->first;
        	
			        StatementRef  stmt    = itConn->second.createStatement();
			        ResultsetRef  result  = stmt.executeQuery(sql);

			        Int32     cnt = 0;
			        TimeSpan  colTs;
        			
			        basar::I18nString ts;

			        while (result.next())
			        {				
				        colTs  = result.getTimeSpan("colspan1");
                        
                        CHECK(colTs.isNull());
                        
                        CHECK_EQUAL("999999999 23:59:59.999", colTs.toNormalizedString());
                        
				        ++cnt;
			        }
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
}

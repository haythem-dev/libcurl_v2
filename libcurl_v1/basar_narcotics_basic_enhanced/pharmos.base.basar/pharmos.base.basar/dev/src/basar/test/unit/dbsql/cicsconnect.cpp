
#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasarcmnutil_decimal.h"
#include "testconnection.h"
#include "testcicsconnection.h"

#include <iostream>

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Decimal;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::Manager;
using basar::db::sql::DbException;

using basar::test::unit::dbsql::TestCICSConnection;

#ifdef HAS_CICS

//------------------------------------------------------------------------------
SUITE(CicsConnect)
{

//------------------------------------------------------------------------------
class FixCics
{
public:
    ConnectionRef fxConn;

    FixCics()
    {
        openSession();
    }

    ~FixCics()
    {
    }

    void openSession()
    {
        try
        {   
            fxConn = TestCICSConnection::createConnectionRef(
                CICS_CONFIG_TIMEOUT);
            REQUIRE CHECK(fxConn.isValid());
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

    // query causes caught timeout exception, so tca session is aborted
    void abortSession()
    {
        bool exceptThrown = false;

        try
        {   
            // tca session is aborted
            execTimedoutQuery();
        }
        catch(const basar::db::sql::DbException &)
        {
            exceptThrown = true;
        }

        CHECK(exceptThrown);
    }

    void execInTimeQuery()
    {
        try
        {   
            ConstString  sql  = "SELECT MIN(par_k_fil) FROM vsam.dkpar_ut";
            StatementRef stmt = fxConn.createStatement();
            ResultsetRef res  = stmt.executeSingleQuery(sql);
            res.next();

            VarString    val  = res.getString(0);
            CHECK_EQUAL("01", val.c_str());
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

    void execTimedoutQuery()
    {
        ConstString sql = "SELECT MIN(art_kubz) FROM vsam.dkart";
        StatementRef stmt = fxConn.createStatement();

        Decimal val = stmt.executeAggregate(sql);
    }
};

//------------------------------------------------------------------------------
TEST_FIXTURE(FixCics, test_repeated_close)
{
    try
    {
        fxConn.close();
        fxConn.close();
    }
    catch(...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
// close tca connection should handle aborted session without exception
TEST_FIXTURE(FixCics, test_close_corrupt_connection)
{
    abortSession();

    bool exceptThrown = false;

    try
    {
        fxConn.close();
    }
    catch (...)
    {
        exceptThrown = true;
    }

    CHECK(!exceptThrown);
}

//------------------------------------------------------------------------------
// connection d'tor should handle aborted session without exception
TEST_FIXTURE(FixCics, test_explicit_destruct_corrupt_connection)
{
    abortSession();

    // explicit fxConn d'tor -> no exception
    fxConn.reset();
}

//------------------------------------------------------------------------------
// implicit connection d'tor should handle aborted session without exception
TEST_FIXTURE(FixCics, test_implicit_destruct_corrupt_connection)
{
    abortSession();

    // implicit fxConn d'tor -> no exception
}

//------------------------------------------------------------------------------
// aborted connection should allow reconnect
TEST_FIXTURE(FixCics, test_reconnect)
{
    abortSession();
    fxConn.reset();

    openSession();
}

//------------------------------------------------------------------------------
// aborted connection should allow reconnect and querying
TEST_FIXTURE(FixCics, test_reconnect_query)
{
    abortSession();
    fxConn.reset();
    openSession();

    execInTimeQuery();
}

//------------------------------------------------------------------------------
// aborted connection should allow repeated reconnects and querying
TEST_FIXTURE(FixCics, test_repeated_aborts)
{
    for (int i = 0; i < 3; ++i)
    {
        abortSession();
        fxConn.reset();
        openSession();
    }

    for (int i = 0; i < 3; ++i)
    {
        execInTimeQuery();
    }
}

//------------------------------------------------------------------------------
// aborted connection should allow repeated reconnects and querying
TEST_FIXTURE(FixCics, test_repeated_aborts_execs)
{
    for (int i = 0; i < 3; ++i)
    {
        abortSession();
        fxConn.reset();
        openSession();
        execInTimeQuery();
    }
}

//------------------------------------------------------------------------------
}   // SUITE

#endif  // HAS_CICS


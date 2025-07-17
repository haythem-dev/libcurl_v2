#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasarcmnutil.h"

#include "testconnection.h"

using basar::ConstString;
using basar::Int32;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
SUITE(InfxSqlFunc)
{
    TEST(to_date)
    {
        static ConstString sqlStmt =

            "SELECT "
                "DBINFO('version', 'full') AS dbinfo, "
                "TO_DATE('2018-04-26 08:48:34.789', "
                        "'%04Y-%02m-%02d %02H:%02M:%02S.%03F3') AS infxdate "
            "FROM sysmaster:sysdual;";

        try
        {
            ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
            StatementRef  stmt    = connect.createStatement();
            ResultsetRef  result  = stmt.executeQuery(sqlStmt);

            CHECK(stmt.isOpen());
            CHECK(result.isOpen());

            Int32 cnt = 0;

            while (result.next())
            {
                ++cnt;
            }

            CHECK_EQUAL(1, cnt);

            CHECK_EQUAL(
                "2018-04-26 08:48:34.789",
                result.getDateTime(1).toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX));
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

    TEST(to_char)
    {
        static ConstString sqlStmt =

            "SELECT "
                "DBINFO('version', 'full') AS dbinfo, "
                "TO_CHAR(DATETIME(2018-04-26 08:48:34.789) YEAR TO FRACTION(3), "
                        "'%04Y-%02m-%02d %02H:%02M:%02S.%03F3')::CHAR(30) "
            "FROM sysmaster:sysdual;";

        try
        {
            ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
            StatementRef  stmt    = connect.createStatement();
            ResultsetRef  result  = stmt.executeQuery(sqlStmt);

            CHECK(stmt.isOpen());
            CHECK(result.isOpen());

            Int32 cnt = 0;

            while (result.next())
            {
                ++cnt;
            }

            CHECK_EQUAL(1, cnt);

            CHECK_EQUAL("2018-04-26 08:48:34.789", result.getString(1));
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
} // SUITE
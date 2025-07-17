#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasarcmnutil.h"

#include "utdbsqlhelper.h"
#include "testconnection.h"
#include "odbcfix.h"
#include "defaultmasterbuttable.h"

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;
using basar::db::sql::PreparedStatementRef;

// general:
using basar::ConstString;
using basar::UInt32;
using basar::Int32;
using basar::VarString;
using basar::Decimal;
using basar::BULong;

// test:
using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::DefaultMasterBUTTable;
using basar::test::unit::dbsql::BUTTable;
using basar::test::unit::dbsql::Defines;

//------------------------------------------------------------------------------
SUITE(InfxBigSerialSerial8)
{
    TEST(bigserial_insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        const basar::Int64 expectedSerial = 42LL;
        const basar::Int32 dummy = 0x0815; //aributary data to insert in order to generate a serial

        try
        {
            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            std::ostringstream os;

            os << "create temp table mytest (id bigserial(" << expectedSerial << "), data integer default 0 not null)";

            stmt.execute(os.str()); //don't care for return here, throws on error anyway

            os.str("");

            os << "insert into mytest (data) values (" << dummy << ")";

            ExecuteReturnInfo res = stmt.execute(os.str());

            CHECK(false == res.m_lastSerial.empty());

            CHECK_EQUAL(expectedSerial, res.m_lastSerial[0]);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //------------------------------------------------------------------------------

    TEST(serial8_insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        const basar::Int64 expectedSerial = 42LL;
        const basar::Int32 dummy = 0x0815; //aributary data to insert in order to generate a serial

        try
        {
            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            std::ostringstream os;

            os << "create temp table mytest (id serial8(" << expectedSerial << "), data integer default 0 not null)";

            stmt.execute(os.str()); //don't care for return here, throws on error anyway

            os.str("");

            os << "insert into mytest (data) values (" << dummy << ")";

            ExecuteReturnInfo res = stmt.execute(os.str());

            CHECK(false == res.m_lastSerial.empty());

            CHECK_EQUAL(expectedSerial, res.m_lastSerial[0]);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //------------------------------------------------------------------------------

    TEST(serial_serial8_insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        const basar::Int64 expectedSerial8 = 42LL;
        const basar::Int32 expectedSerial = 21;
        const basar::Int32 dummy = 0x0815; //aributary data to insert in order to generate a serial

        try
        {
            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            std::ostringstream os;

            os << "create temp table mytest (id serial8(" << expectedSerial8 << "), smallid serial(" << expectedSerial << "), data integer default 0 not null)";

            stmt.execute(os.str()); //don't care for return here, throws on error anyway

            os.str("");

            os << "insert into mytest (data) values (" << dummy << ")";

            ExecuteReturnInfo res = stmt.execute(os.str());

            CHECK(false == res.m_lastSerial.empty());

            //serial takes lead
            CHECK_EQUAL(expectedSerial, res.m_lastSerial[0]);

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            CHECK_EQUAL(expectedSerial8, rs.getInt64("id"));
            CHECK_EQUAL(expectedSerial, rs.getInt32("smallid"));

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //------------------------------------------------------------------------------

    TEST(serial_bigserial_insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        const basar::Int64 expectedBigSerial = 42LL;
        const basar::Int32 expectedSerial = 21;
        const basar::Int32 dummy = 0x0815; //aributary data to insert in order to generate a serial

        try
        {
            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            std::ostringstream os;

            os << "create temp table mytest (id bigserial(" << expectedBigSerial << "), smallid serial(" << expectedSerial << "), data integer default 0 not null)";

            stmt.execute(os.str()); //don't care for return here, throws on error anyway

            os.str("");

            os << "insert into mytest (data) values (" << dummy << ")";

            ExecuteReturnInfo res = stmt.execute(os.str());

            CHECK(false == res.m_lastSerial.empty());

            //serial takes lead
            CHECK_EQUAL(expectedSerial, res.m_lastSerial[0]);

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            CHECK_EQUAL(expectedBigSerial, rs.getInt64("id"));
            CHECK_EQUAL(expectedSerial, rs.getInt32("smallid"));

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }
}

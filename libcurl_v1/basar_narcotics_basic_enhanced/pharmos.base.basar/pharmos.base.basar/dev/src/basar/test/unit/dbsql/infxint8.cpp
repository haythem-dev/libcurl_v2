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
SUITE(Infxint8)
{
    TEST(int8_insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Int64 expectedInt64 = -12147483647LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            std::ostringstream os;

            os << "insert into mytest ( myint8 ) values (" << expectedInt64 << ")";

            ExecuteReturnInfo ret = stmt.execute(os.str());

            CHECK(false == ret.hasError());

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_insert_read)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Int64 expectedInt64 = -12147483647LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            std::ostringstream os;

            os << "insert into mytest ( myint8 ) values (" << expectedInt64 << ")";

            ExecuteReturnInfo ret = stmt.execute(os.str());

            CHECK(false == ret.hasError());

            ResultsetRef res = stmt.executeQuery("select * from mytest");

            bool hasNext = res.next();

            CHECK(true == hasNext);

            basar::Int64 myInt64 = res.getInt64("myint8");

            CHECK_EQUAL(expectedInt64, myInt64);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_read_wrong_access_function)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Int64 expectedInt64 = -12147483647LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            std::ostringstream os;

            os << "insert into mytest ( myint8 ) values (" << expectedInt64 << ")";

            ExecuteReturnInfo ret = stmt.execute(os.str());

            CHECK(false == ret.hasError());

            ResultsetRef res = stmt.executeQuery("select * from mytest");

            bool hasNext = res.next();

            CHECK(true == hasNext);

            CHECK_THROW(res.getInt32("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getInt16("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getDecimal("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getString("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getDate("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getTime("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getDateTime("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getTimeSpan("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getFloat32("myint8"), basar::InvalidParameterException);
            CHECK_THROW(res.getFloat64("myint8"), basar::InvalidParameterException);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_prepstmt_insert_read)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Int64 expectedInt64 = -12147483647LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest(myint8) values(?)");

            prepStmt.setInt64(0, expectedInt64);

            ExecuteReturnInfo ret = prepStmt.execute();

            CHECK(false == ret.hasError());

            ResultsetRef res = stmt.executeQuery("select * from mytest");

            bool hasNext = res.next();

            CHECK(true == hasNext);

            basar::Int64 myInt64 = res.getInt64("myint8");

            CHECK_EQUAL(expectedInt64, myInt64);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_prepstmt_insert_prepstmt_read)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Int64 expectedInt64 = -12147483647LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest(myint8) values(?)");

            prepStmt.setInt64(0, expectedInt64);

            ExecuteReturnInfo ret = prepStmt.execute();

            CHECK(false == ret.hasError());

            PreparedStatementRef prepStmtRead = conn.createPreparedStatement("select * from mytest");

            ResultsetRef res = prepStmtRead.executeQuery();

            bool hasNext = res.next();

            CHECK(true == hasNext);

            basar::Int64 myInt64 = res.getInt64("myint8");

            CHECK_EQUAL(expectedInt64, myInt64);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_prepstmt_insert_wrong_set_function)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {

            const basar::Date dummyDate;
            const basar::Time dummyTime;
            const basar::DateTime dummyDateTime;
            const basar::TimeSpan dummyTimeSpan;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest(myint8) values(?)");

            CHECK_THROW(prepStmt.setInt16(0, 0), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setInt32(0, 0), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setString(0, ""), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setDecimal(0, static_cast<basar::Decimal>(0)), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setDate(0, dummyDate), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setTime(0, dummyTime), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setDateTime(0, dummyDateTime), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setTimeSpan(0, dummyTimeSpan), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setFloat32(0, 0.0f), basar::InvalidParameterException);
            CHECK_THROW(prepStmt.setFloat64(0, 0.0), basar::InvalidParameterException);
        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }
    //---------------------------------------------------------------------------------------------------------

    TEST(int8_min_val)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Long64 minInt64 = -9223372036854775806LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest(myint8) values(?)");

            prepStmt.setInt64(0, minInt64);

            ExecuteReturnInfo ret = prepStmt.execute();

            CHECK(false == ret.hasError());

            ResultsetRef res = stmt.executeQuery("select * from mytest");

            bool hasNext = res.next();

            CHECK(true == hasNext);

            basar::Int64 myInt64 = res.getInt64("myint8");

            CHECK_EQUAL(minInt64, myInt64);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }

    }

    //---------------------------------------------------------------------------------------------------------

    TEST(int8_max_val)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            const basar::Long64 maxInt64 = 9223372036854775807LL;

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            stmt.execute("create temp table mytest (myint8 INT8 default 0 not null)");

            PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest(myint8) values(?)");

            prepStmt.setInt64(0, maxInt64);

            ExecuteReturnInfo ret = prepStmt.execute();

            CHECK(false == ret.hasError());

            ResultsetRef res = stmt.executeQuery("select * from mytest");

            bool hasNext = res.next();

            CHECK(true == hasNext);

            basar::Int64 myInt64 = res.getInt64("myint8");

            CHECK_EQUAL(maxInt64, myInt64);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }

    }

    //---------------------------------------------------------------------------------------------------------

}
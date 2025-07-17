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
SUITE(InfxSimpleBlob)
{
    TEST(infx_text_Insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            std::ostringstream os;

            os << "create temp table mytest (mytext TEXT)";

            StatementRef stmt = conn.createStatement();

            stmt.execute(os.str());

            os.str("");

            //insert only works via prepStmt

            os << "insert into mytest(mytext) values (?)";

            PreparedStatementRef prepStmt = conn.createPreparedStatement(os.str());

            prepStmt.setString(0, expectedText.c_str());

            prepStmt.execute();

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            basar::VarString dbString = rs.getString("mytext");

            CHECK_EQUAL(expectedText, dbString);

            rs.reset();

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    TEST(infx_text_Insert_read_multiple)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            std::ostringstream os;

            os << "create temp table mytest (myid SERIAL, mytext TEXT)";

            StatementRef stmt = conn.createStatement();

            stmt.execute(os.str());

            os.str("");

            //insert only works via prepStmt

            os << "insert into mytest(mytext) values (?)";

            PreparedStatementRef prepStmt = conn.createPreparedStatement(os.str());

            for (int i = 1; i <= 10; ++i)
            {
                prepStmt.setString(0, expectedText.c_str());

                prepStmt.execute();
            }

            os.str("");

            os << "select * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            for (int i = 1; i <= 10; ++i)
            {
                CHECK(rs.next());

                basar::VarString dbString = rs.getString("mytext");

                CHECK_EQUAL(expectedText, dbString);

                CHECK_EQUAL(i, rs.getInt32("myid"));
            }

            rs.reset();

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    TEST(infx_text_Insert_empty_string)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            static const basar::VarString expectedText = "";

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            std::ostringstream os;

            os << "create temp table mytest (mytext TEXT)";

            StatementRef stmt = conn.createStatement();

            stmt.execute(os.str());

            os.str("");

            //insert only works via prepStmt

            os << "insert into mytest(mytext) values (?)";

            PreparedStatementRef prepStmt = conn.createPreparedStatement(os.str());

            prepStmt.setString(0, expectedText.c_str());

            prepStmt.execute();

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            basar::VarString dbString = rs.getString("mytext");

            CHECK_EQUAL(expectedText, dbString);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    TEST(infx_byte_Insert)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            std::ostringstream os;

            os << "create temp table mytest (mybyte BYTE)";

            StatementRef stmt = conn.createStatement();

            stmt.execute(os.str());

            os.str("");

            //insert only works via prepStmt

            os << "insert into mytest(mybyte) values (?)";

            //note: binary is a typedefed std::vector<char>
            basar::Binary bin(expectedText.begin(), expectedText.end());

            PreparedStatementRef prepStmt = conn.createPreparedStatement(os.str());

            prepStmt.setBinary(0, bin);

            prepStmt.execute();

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            basar::Binary binFromDb = rs.getBinary("mybyte");

            basar::VarString dbString;

            dbString.assign(binFromDb.begin(), binFromDb.end());

            CHECK_EQUAL(expectedText, dbString);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

    TEST(infx_byte_Insert_empty_vector)
    {
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            static const basar::VarString expectedText = "";

            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            std::ostringstream os;

            os << "create temp table mytest (mybyte BYTE not null)";

            StatementRef stmt = conn.createStatement();

            stmt.execute(os.str());

            os.str("");

            //insert only works via prepStmt

            os << "insert into mytest(mybyte) values (?)";

            //note: binary is a typedefed std::vector<char>
            basar::Binary bin(expectedText.begin(), expectedText.end());

            PreparedStatementRef prepStmt = conn.createPreparedStatement(os.str());

            prepStmt.setBinary(0, bin);

            prepStmt.execute();

            os.str("");

            os << "select first 1 * from mytest";

            ResultsetRef rs = stmt.executeQuery(os.str());

            CHECK(rs.next());

            basar::Binary binFromDb = rs.getBinary("mybyte");

            CHECK(binFromDb.empty());

            basar::VarString dbString;

            dbString.assign(binFromDb.begin(), binFromDb.end());

            CHECK_EQUAL(expectedText, dbString);

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }

}

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "testconnection.h"
#include "libbasardbsql.h" //for easier setup
#include "libbasarproperty_propertydescription.h"

#include "infxint8_builder.h"

using namespace basar::test::unit::dbaspect;

basar::ConstString myAccDef = "accDef";
basar::ConstString myAccInst = "accInt";
basar::ConstString myAccMeth = "accMeth";

SUITE(DBAspect_Int64)
{
    TEST(int8_insert)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

/*		const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> MyBuilder::create() const
		{
			return boost::shared_ptr<basar::db::aspect::SQLStringBuilder>(new MyBuilder(*this));
		}*/

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 INT8 default 0 not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyInt8InsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setInt64(PROPDEF_MYINT8.getName(), expectedInt64);

        //test

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(int8_select)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 INT8 default 0 not null)");

            std::ostringstream os;
            os << "insert into mytest(myint8) values (" << expectedInt64 << ")";
            stmt.execute(os.str());
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyInt8SelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        CHECK_EQUAL(expectedInt64, accInst.getPropertyTable().begin().getInt64(PROPDEF_MYINT8.getName()));
    }

    //--------------------------------------------------------------------------------------------------

    TEST(int8_insert_prepared)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 INT8 default 0 not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyInt8PreparedInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setInt64(PROPDEF_MYINT8.getName(), expectedInt64);

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(int8_select_prepared)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 INT8 default 0 not null)");

            std::ostringstream os;
            os << "insert into mytest(myint8) values (" << expectedInt64 << ")";
            stmt.execute(os.str());
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyInt8PreparedSelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        CHECK_EQUAL(expectedInt64, accInst.getPropertyTable().begin().getInt64(PROPDEF_MYINT8.getName()));
    }

    //--------------------------------------------------------------------------------------------------

    TEST(bigint_insert)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 BIGINT default 0 not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBigIntInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setInt64(PROPDEF_MYINT8.getName(), expectedInt64);

        //test

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(bigint_select)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 BIGINT default 0 not null)");

            std::ostringstream os;
            os << "insert into mytest(myint8) values (" << expectedInt64 << ")";
            stmt.execute(os.str());
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBigIntSelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        CHECK_EQUAL(expectedInt64, accInst.getPropertyTable().begin().getInt64(PROPDEF_MYINT8.getName()));
    }

    //--------------------------------------------------------------------------------------------------

    TEST(bigint_insert_prepared)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 BIGINT default 0 not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBigIntPreparedInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setInt64(PROPDEF_MYINT8.getName(), expectedInt64);

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(bigint_select_prepared)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        const basar::Int64 expectedInt64 = -12147483647;

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(myint8 BIGINT default 0 not null)");

            std::ostringstream os;
            os << "insert into mytest(myint8) values (" << expectedInt64 << ")";
            stmt.execute(os.str());
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYINT8);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBigIntPreparedSelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        CHECK_EQUAL(expectedInt64, accInst.getPropertyTable().begin().getInt64(PROPDEF_MYINT8.getName()));
    }

}
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "testconnection.h"
#include "libbasardbsql.h" //for easier setup
#include "libbasarproperty_propertydescription.h"

#include "infxsimpleblob_builder.h"

using namespace basar::test::unit::dbaspect;

basar::ConstString myAccDef = "accDef";
basar::ConstString myAccInst = "accInt";
basar::ConstString myAccMeth = "accMeth";

SUITE(DBAspect_SimpleBlob)
{
    TEST(text_insert)
    {
        static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(mytext TEXT not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYTEXT);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyTextInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setString(PROPDEF_MYTEXT.getName(), expectedText);

        //test

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(text_insert_non_prepared)
    {
        static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(mytext TEXT not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYTEXT);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyNonPreparedTextInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setString(PROPDEF_MYTEXT.getName(), expectedText);

        //test

        CHECK_THROW(accInst.execute(myAccMeth, yit), basar::Exception);
    }

    //--------------------------------------------------------------------------------------------------

    TEST(text_select)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(mytext TEXT not null)");

            std::ostringstream os;
            os << "insert into mytest(mytext) values (?)";

            basar::db::sql::PreparedStatementRef prepStmt = sqlConn.createPreparedStatement(os.str());

            prepStmt.setString(0, expectedText.c_str());

            prepStmt.execute();

        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYTEXT);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyTextSelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        CHECK_EQUAL(expectedText, accInst.getPropertyTable().begin().getString(PROPDEF_MYTEXT.getName()));
    }

    //--------------------------------------------------------------------------------------------------

    TEST(binary_insert)
    {
        static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

        //note: binary is a typedefed std::vector<char>
        basar::Binary bin(expectedText.begin(), expectedText.end());

        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(mybinary BYTE not null)");
        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYBINARY);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBinaryInsertBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        basar::db::aspect::AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.insert(basar::FOR_INSERT);

        yit.setBinary(PROPDEF_MYBINARY.getName(), bin);

        //test

        accInst.execute(myAccMeth, yit);
    }

    //--------------------------------------------------------------------------------------------------
    TEST(binary_select)
    {
        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        static const basar::VarString expectedText = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.";

        //note: binary is a typedefed std::vector<char>
        basar::Binary bin(expectedText.begin(), expectedText.end());

        //setup
        {
            basar::db::sql::ConnectionRef sqlConn = conn.getSqlConnection();

            basar::db::sql::StatementRef stmt = sqlConn.createStatement();

            stmt.execute("create temp table mytest(mybinary BYTE not null)");

            std::ostringstream os;
            os << "insert into mytest(mybinary) values (?)";

            basar::db::sql::PreparedStatementRef prepStmt = sqlConn.createPreparedStatement(os.str());

            prepStmt.setBinary(0, bin);

            prepStmt.execute();

        }

        if ( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(myAccDef) )
        {
            basar::db::aspect::Manager::getInstance().releaseAccessorDefinition(myAccDef);
        }

        basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition(myAccDef);

        accDef.getPropertyDescriptionList().push_back(PROPDEF_MYBINARY);

        {
            basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod(myAccMeth);
            MyBinarySelectBuilder b;
            am.push_back(b);
        }

        basar::db::aspect::AccessorInstanceRef accInst = accDef.createInstance(conn);

        //test

        basar::db::aspect::ExecuteResultInfo retInfo = accInst.execute(myAccMeth);

        CHECK(!retInfo.hasError());

        basar::Binary dbBin = accInst.getPropertyTable().begin().getBinary(PROPDEF_MYBINARY.getName());

        basar::VarString dbString;

        dbString.assign(dbBin.begin(), dbBin.end());

        CHECK_EQUAL(expectedText, dbString);
    }

}
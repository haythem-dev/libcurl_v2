#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "accessornames.h"
#include "accessmethodnames.h"
#include "buttablepropertydescriptions.h"


using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::Manager;
using basar::db::aspect::ExecuteResultInfo;

using basar::DateTime;
using basar::Time;
using basar::VarString;

using basar::test::unit::dbaspect::TestConnection;


BEGIN_QUERY_BUILDER_DECLARATION(SelectDateTime)
END_BUILDER_DECLARATION

DONT_CHECK_PROPERTIES(SelectDateTime)
BUILDER_DEFINITION(SelectDateTime)
SQL_PATTERN(SelectDateTime, "select coldt1, coltime1 from but_master")

const basar::property::PropertyDescriptionArray pdarray =
{
    PROPDEF_COLDT1,
    PROPDEF_COLTIME1
};

const basar::property::PropertyDescriptionVector pdcols(2, pdarray);

namespace accessor_selectdatetime
{
    BEGIN_ACCESSOR_DEFINITION("name_selectdatetime")
        PROPERTY_DESCRIPTION_LIST(pdcols)
        ACCESS_METHOD("method_selectdatetime")
        SQL_BUILDER_CREATE(SelectDateTime)
        SQL_BUILDER_PUSH_BACK(SelectDateTime)
    END_ACCESSOR_DEFINITION
}

SUITE(CachingPolicy)
{
    TEST(NoCaching_DateTime)
    {
        ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        try
        {
            AccessorInstanceRef inst = Manager::getInstance().createAccessorInstance(
                "inst_selectdatetime",
                "name_selectdatetime",
                conn,
                basar::db::aspect::NO_CACHING);

            {
                ExecuteResultInfo res = inst.execute("method_selectdatetime");

                CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
                CHECK_EQUAL(0, (int)res.getAffectedRows());

                AccessorPropertyTableRef        proptbl = inst.getPropertyTable();
                AccessorPropertyTable_YIterator yit = proptbl.begin();

                VarString dt = yit.getSQLString("coldt1");
                VarString tm = yit.getSQLString("coltime1");

                proptbl.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            conn.close();
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        conn.close();
    }
}   // SUITE

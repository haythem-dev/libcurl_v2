
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "libbasarproperty.h"
#include "libbasardbsql.h"

#include "testconnection.h"
#include "testnames.h"
#include "libbasardbsql_exceptions.h"

//------------------------------------------------------------------------------
using basar::FOR_CLEAN;
using basar::VarString;
using basar::I18nString;
using basar::TimeSpan;
using basar::ConstString;
using basar::SupportedStateEnum;
using basar::InvalidIteratorException;

using namespace basar::db;

using sql::DbException;
using sql::StatementRef;
using sql::ExecuteReturnInfo;

using aspect::ConnectionRef;
using aspect::AccessorInstanceRef;
using aspect::Manager;
using aspect::ON_DEMAND_CACHING;
using aspect::FULL_CACHING;
using aspect::ExecuteResultInfo;
using aspect::AccessorPropertyTableRef;
using aspect::AccessorPropertyTable_YIterator;
using aspect::SQLStringBuilder;

using basar::test::unit::dbaspect::TestConnection;

SUITE(TestTimeSpan)
{

	TEST(SelectTimeSpan)
	{
		try
		{
			ConnectionRef connect1 = TestConnection::createTestConnection(TestConnection::INFX);
			CHECK(connect1.isOpen());

			AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_SelectTimeSpan",
											 ACCESSORNAME_TestTimespan,
											 connect1,
											 ON_DEMAND_CACHING);

			ExecuteResultInfo               ret;
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;

			ret = accInst1.execute(ACCESSMETHOD_ButTables_TimeSpan_Select);

			CHECK( ! ret.hasError() );

			propTbl = accInst1.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			CHECK_EQUAL("23 12:32:16.123", yit.getTimeSpan ("colspan1").toNormalizedString()        );

			++yit;
			CHECK(yit == propTbl.end());
		}
		catch (basar::Exception & ex)
		{
			std::cout << "basar exception: " << ex.what().c_str() << std::endl;
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

	TEST(UpdateTimeSpan)
	{
		try
		{
			ConnectionRef connect1 = TestConnection::createTestConnection(TestConnection::INFX);
			CHECK(connect1.isOpen());

			AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_SelectTimeSpan",
											 ACCESSORNAME_TestTimespan,
											 connect1,
											 ON_DEMAND_CACHING);

			ExecuteResultInfo               ret;
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;
	

			//select timespan field
			ret = accInst1.execute(ACCESSMETHOD_ButTables_TimeSpan_Select);

			CHECK( ! ret.hasError() );

			propTbl = accInst1.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			CHECK_EQUAL("23 12:32:16.123", yit.getTimeSpan ("colspan1").toNormalizedString()        );

			++yit;
			CHECK(yit == propTbl.end());

			//update selected timespan field
			yit = propTbl.begin();

			TimeSpan ts;

			ts.fromNormalizedString("28 06:43:16.456");

			yit.setTimeSpan("colspan1", ts);

			CHECK_EQUAL("28 06:43:16.456", yit.getTimeSpan("colspan1").toNormalizedString());

			accInst1.beginTransaction();

			ret = accInst1.execute(ACCESSMETHOD_ButTables_TimeSpan_Update, yit);

			CHECK( ! ret.hasError() );

			//read and check updated value
			ret = accInst1.execute(ACCESSMETHOD_ButTables_TimeSpan_Select);

			CHECK( ! ret.hasError() );

			propTbl = accInst1.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();

			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			CHECK_EQUAL("28 06:43:16.456", yit.getTimeSpan("colspan1").toNormalizedString());

			accInst1.rollbackTransaction();

		}
		catch (basar::Exception & ex)
		{
			std::cout << "basar exception: " << ex.what().c_str() << std::endl;
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


    TEST(MatchTimeSpan)
    {
        ConnectionRef connect1 = TestConnection::createTestConnection(TestConnection::INFX);
        CHECK(connect1.isOpen());

        AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance
        ("Inst_ButTable_SelectTimeSpan",
            ACCESSORNAME_TestTimespan,
            connect1,
            FULL_CACHING); //using full caching to allow insert of row (as result has to be read)

        ExecuteResultInfo               ret;
        AccessorPropertyTableRef        propTbl;
        AccessorPropertyTable_YIterator yit;
        AccessorPropertyTable_YIterator yitToMatchTo;

        //select timespan field
        ret = accInst1.execute(ACCESSMETHOD_ButTables_TimeSpan_Select);

        CHECK(!ret.hasError());

        propTbl = accInst1.getPropertyTable();
        CHECK(!propTbl.isNull());

        yit = propTbl.begin();
        CHECK(!yit.isNull());
        CHECK(yit != propTbl.end());

        yitToMatchTo = propTbl.insert(basar::FOR_INSERT);

        yitToMatchTo.match(yit);

        CHECK_EQUAL(yit.getTimeSpan("colspan1").toNormalizedString(), yitToMatchTo.getTimeSpan("colspan1").toNormalizedString());

        AccessorPropertyTable_YIterator yitClone = yit.cloneToUnboundedPropertyRow();

        CHECK_EQUAL(yit.getTimeSpan("colspan1").toNormalizedString(), yitClone.getTimeSpan("colspan1").toNormalizedString());

    }

}
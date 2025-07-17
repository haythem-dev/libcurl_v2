//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   21.05.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql.h"

#include "testconnection.h"
#include "testnames.h"

#include "buttablepropertydescriptions.h"

//------------------------------------------------------------------------------
SUITE(TestStoredProcedures)
{

//------------------------------------------------------------------------------

using basar::Int32;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
TEST(InfxExecProc1)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("Inst_ButProcs_ExecProc1",
											 ACCESSORNAME_ButProcs_Stored_Procs,
											 connect,
											 ON_DEMAND_CACHING);

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButProcs_ExecProc1);
		CHECK( ! ret.hasError() );
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

//------------------------------------------------------------------------------
TEST(InfxExecProc2_withVariable)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("Inst_ButProcs_ExecProc2",
											 ACCESSORNAME_ButProcs_Stored_Procs,
											 connect,
											 ON_DEMAND_CACHING);
        AccessorPropertyTable_YIterator yit = accInst.getPropertyTable().insert( basar::FOR_CLEAN );

        yit.setInt32( PROPDEF_COLRET0.getName(), 42 );

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButProcs_ExecFunc2_WithVariable, yit);
		CHECK( ! ret.hasError() );

        AccessorPropertyTableRef propTab = accInst.getPropertyTable();

        CHECK(! propTab.isNull() );

		yit = propTab.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTab.end());

		while (yit != propTab.end())
		{
			CHECK_EQUAL(42, yit.getInt32("ret0"));
			CHECK_EQUAL(168, yit.getInt32("ret1"));

			++yit;
		}

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

//------------------------------------------------------------------------------
TEST(InfxExecFunc2)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef SqlConnect = connect.getSqlConnection();

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButProcs_ExecFunc2",
													 ACCESSORNAME_ButProcs_Stored_Procs,
													 connect,
													 ON_DEMAND_CACHING);

		ret = accInst.execute(ACCESSMETHOD_ButProcs_ExecFunc2);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		while (yit != propTbl.end())
		{
			CHECK_EQUAL( 6, yit.getInt32("ret0"));
			CHECK_EQUAL(24, yit.getInt32("ret1"));

			++yit;
		}
	
		CHECK(yit == propTbl.end());
		CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));
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

//------------------------------------------------------------------------------
TEST(InfxExecFunc3)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef SqlConnect = connect.getSqlConnection();

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButProcs_ExecFunc3",
													 ACCESSORNAME_ButProcs_Stored_Procs,
													 connect,
													 ON_DEMAND_CACHING);

		ret = accInst.execute(ACCESSMETHOD_ButProcs_ExecFunc3);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		while (yit != propTbl.end())
		{
			CHECK(0 != yit.getInt32("ret0"));

			CHECK_EQUAL(100014 + static_cast<Int32>(propTbl.getNumberOfFetchedRows()) - 1, yit.getInt32("ret1"));
			CHECK_EQUAL(  1988 + static_cast<Int32>(propTbl.getNumberOfFetchedRows()) - 1, yit.getInt16("ret2"));

			++yit;
		}
	
		CHECK(yit == propTbl.end());
		CHECK_EQUAL(10, static_cast<Int32>(propTbl.size()));
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

//------------------------------------------------------------------------------
}	// SUITE

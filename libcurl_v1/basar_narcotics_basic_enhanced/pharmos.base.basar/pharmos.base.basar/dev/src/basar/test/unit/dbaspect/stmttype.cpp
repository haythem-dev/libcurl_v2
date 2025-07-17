//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   21.05.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql.h"

#include "libbasardbsql_exceptions.h"

#include "testconnection.h"
#include "testnames.h"

//------------------------------------------------------------------------------
SUITE(TestStatementTypes)
{

//------------------------------------------------------------------------------
using basar::Int32;
using basar::InvalidStateException;

using basar::db::sql::DbException;

using basar::db::aspect::ExecuteErrorEnum;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
TEST(InfxQueryGood)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Statement_Types_Query",
													 ACCESSORNAME_ButTable_Statement_Types,
													 connect,
													 ON_DEMAND_CACHING);

		ret = accInst.execute(ACCESSMETHOD_ButMaster_StmtTypes_Select);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		while (yit != propTbl.end())
		{
			++yit;
		}
	
		CHECK(yit == propTbl.end());
		CHECK_EQUAL(2, static_cast<Int32>(propTbl.size()));
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
TEST(InfxQueryBad)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Statement_Types_QueryWrong",
													 ACCESSORNAME_ButTable_Statement_Types,
													 connect,
													 ON_DEMAND_CACHING);

		CHECK_THROW(ret = accInst.execute(ACCESSMETHOD_ButMaster_StmtTypes_SelectWrong), DbException);
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
TEST(InfxExecuteGood)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Statement_Types_Execute",
											 ACCESSORNAME_ButTable_Statement_Types,
											 connect,
											 ON_DEMAND_CACHING);

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_StmtTypes_Execute);
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
TEST(InfxExecuteBad)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Statement_Types_ExecuteWrong",
											 ACCESSORNAME_ButTable_Statement_Types,
											 connect,
											 ON_DEMAND_CACHING);

		ExecuteResultInfo ret;
		
		CHECK_THROW(ret = accInst.execute(ACCESSMETHOD_ButVar_StmtTypes_ExecuteWrong), DbException);
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
TEST(InfxAggregQueryGood)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		std::pair<bool, basar::Decimal> count;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Statement_Types_AggregQueryWrong",
													 ACCESSORNAME_ButTable_Statement_Types,
													 connect,
													 ON_DEMAND_CACHING);

		count = accInst.executeAggregate(ACCESSMETHOD_ButMaster_StmtTypes_Aggreg);
		CHECK_EQUAL(true, count.first);
		CHECK(10 == count.second.toInt32());
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
TEST(InfxAggregQueryBad)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		std::pair<bool, basar::Decimal> count;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Statement_Types_AggregQuery",
													 ACCESSORNAME_ButTable_Statement_Types,
													 connect,
													 ON_DEMAND_CACHING);

		CHECK_THROW(count = accInst.executeAggregate(ACCESSMETHOD_ButMaster_StmtTypes_AggregWrong), 
			        InvalidStateException);
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

TEST(InfxSingleQueryGood)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Statement_Types_SingleQuery",
													 ACCESSORNAME_ButTable_Statement_Types,
													 connect,
													 ON_DEMAND_CACHING);

		ret = accInst.execute(ACCESSMETHOD_ButMaster_StmtTypes_SingleQuery);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		while (yit != propTbl.end())
		{
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
}	// SUITE

//----------------------------------------------------------------------------
/** $Id: transaction.cpp 3914 2008-04-10 08:43:52Z michael.eichenlaub $
 *  \author Michael Eichenlaub
 *  \date   27.10.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql_managerimpl.h"
#include "libbasardbsql_manager.h"

#include "testconnection.h"
#include "testnames.h"

//------------------------------------------------------------------------------
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorInstanceList_Iterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
SUITE(SuiteManager)
{

//------------------------------------------------------------------------------

TEST(hasAccessorDefinition)
{
	try
	{
        CHECK( false == basar::db::aspect::Manager::getInstance().hasAccessorDefinition("bogus accessor definition name"));
        
        //accessors register during static init
        CHECK(basar::db::aspect::Manager::getInstance().hasAccessorDefinition(ACCESSORNAME_ButTable_Successive_Statements));
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

TEST(hasAccessorInstance)
{
    basar::db::sql::Manager::clear();
    
    try
    {
        CHECK( false == basar::db::aspect::Manager::getInstance().hasAccessorInstance("bogus accessor instance name"));

		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance	("Inst_ButTable_Successive_Statements_Stored",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING,
																					 true);
        
        CHECK(Manager::getInstance().hasAccessorInstance("Inst_ButTable_Successive_Statements_Stored"));
        
        Manager::getInstance().releaseAccessorInstance("Inst_ButTable_Successive_Statements_Stored");
        
        CHECK(false == Manager::getInstance().hasAccessorInstance("Inst_ButTable_Successive_Statements_Stored"));
        
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

TEST(NonExecutedAccessorDeregisteringDuringTransaction)
{
   	basar::db::sql::Manager::clear();
   	
   	try
   	{
   		
   		//open transaction, create AccessorInstance, Instance is to be destroyed during transaction
   		
   		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

        conn.beginTransaction();

        {

            AccessorInstanceRef accInst2 = Manager::getInstance().createAccessorInstance	("Inst_ButTable_Successive_Statements_2",
																				    ACCESSORNAME_ButTable_Successive_Statements,
																				    conn, ON_DEMAND_CACHING); 
		}
		
		conn.rollbackTransaction();
	}
	catch(...)
	{}																				 
}

//------------------------------------------------------------------------------

TEST(Shutdown)
{
	basar::db::sql::Manager::clear();
	
	try
	{
		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance	("Inst_ButTable_Successive_Statements",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// -------------------------------------------

		CHECK(conn.isOpen());

		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeConn());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeOpenConn());

		accInst.reset();
		basar::db::sql::Manager::clear();	// clear sql connection
	}	// no assertion may occur here: ~AccessorPropertyTableRef -> ~CacheController -> 
	    //                              ~StatementRef             -> ~StatementInfx (sql connection has gone)
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
TEST(SafeAccessToRegisteredAccessors_2AccInst)
{
	try
	{
		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance("Inst_ButTable_Successive_Statements_a",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);

		AccessorInstanceRef accInst2 = Manager::getInstance().createAccessorInstance("Inst_ButTable_Successive_Statements_b",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);


		accInst1.beginTransaction();

		// -------------------------------------

		ExecuteResultInfo ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl1 = accInst1.getPropertyTable();
		CHECK( !propTbl1.isNull() );

		AccessorPropertyTable_YIterator yit1 = propTbl1.begin();
		CHECK( ! yit1.isNull() );
		CHECK( yit1 != propTbl1.end());

		CHECK_EQUAL(2001     , yit1.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit1.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit1.getString("colvch2" ).c_str());

		++yit1;
		CHECK(yit1 == propTbl1.end());

		// -------------------------------------------

		ret = accInst2.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl2 = accInst2.getPropertyTable();
		CHECK( !propTbl2.isNull() );

		AccessorPropertyTable_YIterator yit2 = propTbl2.begin();
		CHECK( ! yit2.isNull() );
		CHECK( yit2 != propTbl2.end());

		CHECK_EQUAL(2010      , yit2.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit2.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit2.getString("colvch2" ).c_str());

		++yit2;
		CHECK(yit2 == propTbl2.end());

		accInst2.reset();

		// --------------------------------------------

		accInst1.rollbackTransaction();		// must not execute accInst2.resetInternalSnapshot() !!!

		accInst1.reset();

		CHECK(conn.isOpen());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeConn());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeOpenConn());

		basar::db::sql::Manager::clear();	// clear sql connection
	}	// no assertion may occur here: ~AccessorPropertyTableRef -> ~CacheController -> 
	    //                              ~StatementRef             -> ~StatementInfx (sql connection has gone)
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
TEST(SafeAccessToRegisteredAccessors_1AccInst_2created)
{
	try
	{
		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance("Inst_ButTable_Successive_Statements_a",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);

		accInst1.beginTransaction();

		// -------------------------------------

		ExecuteResultInfo ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl1 = accInst1.getPropertyTable();
		CHECK( !propTbl1.isNull() );

		AccessorPropertyTable_YIterator yit1 = propTbl1.begin();
		CHECK( ! yit1.isNull() );
		CHECK( yit1 != propTbl1.end());

		CHECK_EQUAL(2001     , yit1.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit1.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit1.getString("colvch2" ).c_str());

		++yit1;
		CHECK(yit1 == propTbl1.end());

		// -------------------------------------------

		accInst1 = Manager::getInstance().createAccessorInstance(   "Inst_ButTable_Successive_Statements_b",
																	ACCESSORNAME_ButTable_Successive_Statements,
																	conn,
																	ON_DEMAND_CACHING);

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl2 = accInst1.getPropertyTable();
		CHECK( !propTbl2.isNull() );

		AccessorPropertyTable_YIterator yit2 = propTbl2.begin();
		CHECK( ! yit2.isNull() );
		CHECK( yit2 != propTbl2.end());

		CHECK_EQUAL(2010      , yit2.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit2.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit2.getString("colvch2" ).c_str());

		++yit2;
		CHECK(yit2 == propTbl2.end());

		// --------------------------------------------

		accInst1.rollbackTransaction();		

		accInst1.reset();

		CHECK(conn.isOpen());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeConn());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeOpenConn());

		basar::db::sql::Manager::clear();	// clear sql connection
	}	// no assertion may occur here: ~AccessorPropertyTableRef -> ~CacheController -> 
	    //                              ~StatementRef             -> ~StatementInfx (sql connection has gone)
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
TEST(SafeAccessToRegisteredAccessors_1AccInst_1created)
{
	try
	{
		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance("Inst_ButTable_Successive_Statements_a",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);

		accInst1.beginTransaction();

		// -------------------------------------

		ExecuteResultInfo ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl1 = accInst1.getPropertyTable();
		CHECK( !propTbl1.isNull() );

		AccessorPropertyTable_YIterator yit1 = propTbl1.begin();
		CHECK( ! yit1.isNull() );
		CHECK( yit1 != propTbl1.end());

		CHECK_EQUAL(2001     , yit1.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit1.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit1.getString("colvch2" ).c_str());

		++yit1;
		CHECK(yit1 == propTbl1.end());

		// -------------------------------------------

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl2 = accInst1.getPropertyTable();
		CHECK( !propTbl2.isNull() );

		AccessorPropertyTable_YIterator yit2 = propTbl2.begin();
		CHECK( ! yit2.isNull() );
		CHECK( yit2 != propTbl2.end());

		CHECK_EQUAL(2010      , yit2.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit2.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit2.getString("colvch2" ).c_str());

		++yit2;
		CHECK(yit2 == propTbl2.end());

		// --------------------------------------------

		accInst1.rollbackTransaction();	

		accInst1.reset();

		CHECK(conn.isOpen());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeConn());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeOpenConn());

		basar::db::sql::Manager::clear();	// clear sql connection
	}	// no assertion may occur here: ~AccessorPropertyTableRef -> ~CacheController -> 
	    //                              ~StatementRef             -> ~StatementInfx (sql connection has gone)
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
TEST(SafeAccessToRegisteredAccessors_1AccInst_1created_2transactions)
{
	try
	{
		ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(conn.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance("Inst_ButTable_Successive_Statements_a",
																				     ACCESSORNAME_ButTable_Successive_Statements,
																					 conn,
																					 ON_DEMAND_CACHING);

		accInst1.beginTransaction();

		// -------------------------------------

		ExecuteResultInfo ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl1 = accInst1.getPropertyTable();
		CHECK( !propTbl1.isNull() );

		AccessorPropertyTable_YIterator yit1 = propTbl1.begin();
		CHECK( ! yit1.isNull() );
		CHECK( yit1 != propTbl1.end());

		CHECK_EQUAL(2001     , yit1.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit1.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit1.getString("colvch2" ).c_str());

		++yit1;
		CHECK(yit1 == propTbl1.end());

		// -------------------------------------------

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl2 = accInst1.getPropertyTable();
		CHECK( !propTbl2.isNull() );

		AccessorPropertyTable_YIterator yit2 = propTbl2.begin();
		CHECK( ! yit2.isNull() );
		CHECK( yit2 != propTbl2.end());

		CHECK_EQUAL(2010      , yit2.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit2.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit2.getString("colvch2" ).c_str());

		++yit2;
		CHECK(yit2 == propTbl2.end());

		// --------------------------------------------

		accInst1.rollbackTransaction();	

		accInst1.beginTransaction();

		// -------------------------------------

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl1 = accInst1.getPropertyTable();
		CHECK( !propTbl1.isNull() );

		yit1 = propTbl1.begin();
		CHECK( ! yit1.isNull() );
		CHECK( yit1 != propTbl1.end());

		CHECK_EQUAL(2001     , yit1.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit1.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit1.getString("colvch2" ).c_str());

		++yit1;
		CHECK(yit1 == propTbl1.end());

		// -------------------------------------------

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		propTbl2 = accInst1.getPropertyTable();
		CHECK( !propTbl2.isNull() );

		yit2 = propTbl2.begin();
		CHECK( ! yit2.isNull() );
		CHECK( yit2 != propTbl2.end());

		CHECK_EQUAL(2010      , yit2.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit2.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit2.getString("colvch2" ).c_str());

		++yit2;
		CHECK(yit2 == propTbl2.end());

		// --------------------------------------------

		accInst1.rollbackTransaction();		

		accInst1.reset();

		CHECK(conn.isOpen());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeConn());
		CHECK_EQUAL(1U, (unsigned int)Manager::getInstance().sizeOpenConn());

		basar::db::sql::Manager::clear();	// clear sql connection
	}	// no assertion may occur here: ~AccessorPropertyTableRef -> ~CacheController -> 
	    //                              ~StatementRef             -> ~StatementInfx (sql connection has gone)
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

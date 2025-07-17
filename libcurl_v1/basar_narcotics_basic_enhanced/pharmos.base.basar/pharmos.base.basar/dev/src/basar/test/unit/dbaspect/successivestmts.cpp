//----------------------------------------------------------------------------
/** $Id$
 *  \author Michael Eichenlaub
 *  \date   19.02.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "libbasardbsql.h"

#include "testconnection.h"
#include "testnames.h"
#include "odbcfix.h"

#include <string.h>

//------------------------------------------------------------------------------
using basar::db::aspect::ExecuteErrorEnum;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::SINGLE_ROW_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

#define TESTDBACPECT_TESTSUCCESSIVESTATEMENTS
#ifdef TESTDBACPECT_TESTSUCCESSIVESTATEMENTS

//------------------------------------------------------------------------------
SUITE(TestSuccessiveStatements)
{

//------------------------------------------------------------------------------
TEST(Infx_Select_Select_PropTabClear_SingleRowCaching)
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
													("Inst_ButTable_Successive_Statements",
													 ACCESSORNAME_ButTable_Successive_Statements,
													 connect,
													 SINGLE_ROW_CACHING);


		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

        //propTbl.clear();

		// --- step 2 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_c);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2002      , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH22", yit.getString("colch2"  ).c_str());
        CHECK_EQUAL("\\#", yit.getString("colvch2" ).c_str());

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


//------------------------------------------------------------------------------
TEST(Infx_Select_Select_SqlTrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef SqlConnect = connect.getSqlConnection();
		
		SqlConnect.begin();

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Successive_Statements",
													 ACCESSORNAME_ButTable_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);


		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// --- step 2 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b, false);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		SqlConnect.rollback();
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
TEST(Infx_Select_Select_AccTrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());


		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Successive_Statements",
													 ACCESSORNAME_ButTable_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		accInst.beginTransaction();

		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// --- step 2 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b, false);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		accInst.rollbackTransaction();
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
TEST(Infx_Select_Select_DbAspectConnTrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			ExecuteResultInfo               ret;
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;
			AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
														("Inst_ButTable_Successive_Statements",
														ACCESSORNAME_ButTable_Successive_Statements,
														connect,
														ON_DEMAND_CACHING);

			// --- step 1 --------------------------------

			ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
			CHECK( ! ret.hasError() );

			propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());

			// --- step 2 --------------------------------

			ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b, false);
			CHECK( ! ret.hasError() );

			propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK( yit != propTbl.end());

			CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		connect.rollbackTransaction();
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
TEST(Infx_Select_Update_sqltrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef sqlConn = connect.getSqlConnection();

		sqlConn.begin();

		AccessorInstanceRef accInstSel = Manager::getInstance().createAccessorInstance
											("InstSel_ButMaster_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);
		AccessorInstanceRef accInstUpd = Manager::getInstance().createAccessorInstance
											("InstUpd_ButMaster_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);

		{	// --- step 1 --------------------------------

			ExecuteResultInfo ret = accInstSel.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
			CHECK( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInstSel.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		{	// --- step 2 --------------------------------

			ExecuteResultInfo ret = accInstUpd.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
			CHECK( ! ret.hasError() );
		}

		sqlConn.rollback();
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
TEST(Infx_Select_Update_aspconntrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorInstanceRef accInstSel = Manager::getInstance().createAccessorInstance
												("InstSel_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);
			AccessorInstanceRef accInstUpd = Manager::getInstance().createAccessorInstance
												("InstUpd_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);

			{	// --- step 1 --------------------------------

				ExecuteResultInfo ret = accInstSel.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
				CHECK( ! ret.hasError() );

				AccessorPropertyTableRef propTbl = accInstSel.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				// check YIterator
				CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

				++yit;
				CHECK(yit == propTbl.end());
			}

			{	// --- step 2 --------------------------------

				ExecuteResultInfo ret = accInstUpd.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
				CHECK( ! ret.hasError() );
			}
		}

		connect.rollbackTransaction();
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
TEST(Infx_Select_Update_Select_acctrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("InstSel_ButMaster_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);
		accInst.beginTransaction();

		{	// --- step 1: fetch first -------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_c);
			CHECK( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());
		}

		{	// --- step 2: update ------------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
			CHECK( ! ret.hasError() );
		}

		{	// --- step 3: fetch next -------------------

			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			++yit;
			CHECK(yit != propTbl.end());

			CHECK_EQUAL(2002     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH22", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("\\#"    , yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		accInst.rollbackTransaction();
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
TEST(Infx_Select_Update_Select_aspconntrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
												("InstSel_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);

			{	// --- step 1: fetch first -------------------

				ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_c);
				CHECK( ! ret.hasError() );

				AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				// check YIterator
				CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());
			}

			{	// --- step 2: update ------------------------

				ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
				CHECK( ! ret.hasError() );
			}

			{	// --- step 3: fetch next -------------------

				AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				++yit;
				CHECK(yit != propTbl.end());

				CHECK_EQUAL(2002     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH22", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("\\#"    , yit.getString("colvch2" ).c_str());

				++yit;
				CHECK(yit == propTbl.end());
			}
		}

		connect.rollbackTransaction();
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
TEST(Infx_Update_Update_sqltrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef sqlConn = connect.getSqlConnection();
		sqlConn.begin();

		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_ButTable_Successive_Statements",
													 ACCESSORNAME_ButTable_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------
        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a);
		    CHECK( ! ret.hasError() );
        }
		// --- step 2 --------------------------------

        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_b, false);
		    CHECK( ! ret.hasError() );
        }

		sqlConn.rollback();
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
TEST(Infx_Update_Update_dbaspectconntrans)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;
			AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
														("Inst_ButTable_Successive_Statements",
														ACCESSORNAME_ButTable_Successive_Statements,
														connect,
														ON_DEMAND_CACHING);

			// --- step 1 --------------------------------
            {
			    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a);
			    CHECK( ! ret.hasError() );
            }
			// --- step 2 --------------------------------
            {
			    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_b, false);
			    CHECK( ! ret.hasError() );
            }
    }

		connect.rollbackTransaction();
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
#ifdef HAS_ODBC

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, ODBC_Select_Select)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			ExecuteResultInfo               ret;
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;
			AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
														("Inst_ButTable_Successive_Statements",
														ACCESSORNAME_ButTable_Successive_Statements,
														connect,
														ON_DEMAND_CACHING);

			// --- step 1 --------------------------------

			ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
			CHECK( ! ret.hasError() );

			propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());

			// --- step 2 --------------------------------

			ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b, false);
			CHECK( ! ret.hasError() );

			propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit != propTbl.end());

			CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
			CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
			CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		connect.rollbackTransaction();
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
TEST_FIXTURE(OdbcFix, ODBC_Select_Update)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorInstanceRef accInstSel = Manager::getInstance().createAccessorInstance
												("InstSel_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);
			AccessorInstanceRef accInstUpd = Manager::getInstance().createAccessorInstance
												("InstUpd_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);

			{	// --- step 1 --------------------------------

				ExecuteResultInfo ret = accInstSel.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
				CHECK( ! ret.hasError() );

				AccessorPropertyTableRef propTbl = accInstSel.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				// check YIterator
				CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());

				++yit;
				CHECK(yit == propTbl.end());
			}

			{	// --- step 2 --------------------------------

				ExecuteResultInfo ret = accInstUpd.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
				CHECK( ! ret.hasError() );
			}
		}

		connect.rollbackTransaction();
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
TEST_FIXTURE(OdbcFix, ODBC_Select_Update_Select)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
												("InstSel_ButMaster_Successive_Statements",
												ACCESSORNAME_ButTable_Successive_Statements,
												connect,
												ON_DEMAND_CACHING);

			{	// --- step 1: fetch first -------------------

				ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_c);
				CHECK( ! ret.hasError() );

				AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				// check YIterator
				CHECK_EQUAL(2001     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH21", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("COLCH21", yit.getString("colvch2" ).c_str());
			}

			{	// --- step 2: update ------------------------

				ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a, false);
				CHECK( ! ret.hasError() );
			}

			{	// --- step 3: fetch next -------------------

				AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
				CHECK( !propTbl.isNull() );

				AccessorPropertyTable_YIterator yit = propTbl.begin();
				CHECK( ! yit.isNull() );
				CHECK( yit != propTbl.end());

				++yit;
				CHECK(yit != propTbl.end());

				CHECK_EQUAL(2002     , yit.getInt16 ("colsmint")        );
				CHECK_EQUAL("COLCH22", yit.getString("colch2"  ).c_str());
				CHECK_EQUAL("\\#"    , yit.getString("colvch2" ).c_str());

				++yit;
				CHECK(yit == propTbl.end());
			}
		}

		connect.rollbackTransaction();
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
TEST_FIXTURE(OdbcFix, ODBC_Update_Update)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		connect.beginTransaction();

		{
			AccessorPropertyTableRef        propTbl;
			AccessorPropertyTable_YIterator yit;
			AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
														("Inst_ButTable_Successive_Statements",
														ACCESSORNAME_ButTable_Successive_Statements,
														connect,
														ON_DEMAND_CACHING);

			// --- step 1 --------------------------------
            {
			    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_a);
			    CHECK( ! ret.hasError() );
            }
			// --- step 2 --------------------------------
            {
			    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButVar_Successive_Statements_Update_b, false);
			    CHECK( ! ret.hasError() );
            }
		}
	
		connect.rollbackTransaction();
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
#endif	// HAS_ODBC


//------------------------------------------------------------------------------
#ifdef HAS_CICS

//------------------------------------------------------------------------------
TEST(CICS_Select_Select_Good)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
		CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// --- step 2 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b, true);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL("01", yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("01", yit.getString("par_k_name").c_str());
		CHECK_EQUAL("03", yit.getString("par_k_nr"  ).c_str());

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

//------------------------------------------------------------------------------
TEST(CICS_Select_Select_Critical)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
		CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// --- step 2 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b, false);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
		CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

		++yit;
		CHECK(yit != propTbl.end());

		CHECK_EQUAL("01", yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("01", yit.getString("par_k_name").c_str());
		CHECK_EQUAL("03", yit.getString("par_k_nr"  ).c_str());

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

//------------------------------------------------------------------------------
TEST(CICS_Select_SelectCount_Critical)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		std::pair<bool, basar::Decimal> count;
		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check YIterator
		CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
		CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
		CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// --- step 2 --------------------------------

		count = accInst.executeAggregate(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_SelectCount);
		CHECK_EQUAL(true, count.first);
		CHECK(1 == count.second.toInt32());

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

TEST(CICS_Select_Update_Critical_OneAccessorInstance)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst    = Manager::getInstance().createAccessorInstance
											("InstSel_CICS_VSAM_DKPAR_UT_Successive_Statements",
											 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);

		{	// --- step 1 --------------------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a);
			CHECK( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
			CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
			CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		{	// --- step 2 --------------------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a, false);
			CHECK( ! ret.hasError() );
			CHECK_EQUAL(1, (int)ret.getAffectedRows());

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
TEST(CICS_Select_Update_Select_Critical)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance
											("InstSel_CICS_VSAM_DKPAR_UT_Successive_Statements",
											 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);

		{	// --- step 1: fetch first -------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_c);
			CHECK( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
			CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
			CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());
		}

		{	// --- step 2: update ------------------------

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a, false);
			CHECK( ! ret.hasError() );
			CHECK_EQUAL(1, (int)ret.getAffectedRows());

		}

		{	// --- step 3: fetch next -------------------

			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			++yit;
			CHECK(yit == propTbl.end());

			// further yit.get...() calls don't succeed 
			// because of CICS case, select statement is closed by execute(update) 

			// check YIterator
			// CHECK_EQUAL("01" , yit.getString("par_k_fil" ).c_str());
			// CHECK_EQUAL("01" , yit.getString("par_k_name").c_str());
			// CHECK_EQUAL("01" , yit.getString("par_k_nr"  ).c_str());
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

TEST(CICS_Select_Update_Critical_TwoAccessorInstances)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		AccessorInstanceRef accInstSel = Manager::getInstance().createAccessorInstance
											("InstSel_CICS_VSAM_DKPAR_UT_Successive_Statements",
											 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);

		AccessorInstanceRef accInstUpd = Manager::getInstance().createAccessorInstance
											("InstUpd_CICS_VSAM_DKPAR_UT_Successive_Statements",
											 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
											 connect,
											 ON_DEMAND_CACHING);

		{	// --- step 1 --------------------------------

			ExecuteResultInfo ret = accInstSel.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a);
			CHECK( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInstSel.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			// check YIterator
			CHECK_EQUAL("01"      , yit.getString("par_k_fil" ).c_str());
			CHECK_EQUAL("PA29"    , yit.getString("par_k_name").c_str());
			CHECK_EQUAL("01"      , yit.getString("par_k_nr"  ).c_str());

			++yit;
			CHECK(yit == propTbl.end());
		}

		accInstSel.reset();

		{	// --- step 2 --------------------------------

			ExecuteResultInfo ret = accInstUpd.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a, false);
			CHECK( ! ret.hasError() );
			CHECK_EQUAL(1, (int)ret.getAffectedRows());

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
TEST(CICS_Update_Update_GoodA)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------
        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a);
		    CHECK( ! ret.hasError() );
		    CHECK_EQUAL(1, (int)ret.getAffectedRows());
        }

		// --- step 2 --------------------------------
        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b, true);
		    CHECK( ! ret.hasError() );
		    CHECK_EQUAL(1, (int)ret.getAffectedRows());
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
TEST(CICS_Update_Update_GoodB)
{
	try
	{
		ConnectionRef connect;
		{
			UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
			connect = TestConnection::createTestConnection(TestConnection::CICS);
		}
		CHECK(connect.isOpen());

		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------
        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a);
		    CHECK( ! ret.hasError() );
		    CHECK_EQUAL(1, (int)ret.getAffectedRows());
        }
		// --- step 2 --------------------------------
        {
		    ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b, false);
		    CHECK( ! ret.hasError() );
		    CHECK_EQUAL(1, (int)ret.getAffectedRows());
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
TEST(CICS_Insert_DuplicateError)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK(connect.isOpen());

		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;
		AccessorInstanceRef	            accInst = Manager::getInstance().createAccessorInstance
													("Inst_CICS_VSAM_DKPAR_UT_Successive_Statements",
													 ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements,
													 connect,
													 ON_DEMAND_CACHING);

		// --- step 1 --------------------------------

		ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Insert,
			                  true,		// clearBeforeSelect
							  false);	// flagExcept
		CHECK_EQUAL( basar::db::aspect::SQL_ERROR_DUPL_INSERT, ret.getError());
		CHECK_EQUAL(0, (int)ret.getAffectedRows());
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
#endif	// HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

#endif

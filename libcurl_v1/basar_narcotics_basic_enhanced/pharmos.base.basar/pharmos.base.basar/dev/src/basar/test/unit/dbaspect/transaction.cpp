//----------------------------------------------------------------------------
/** $Id: transaction.cpp 3914 2008-04-10 08:43:52Z michael.eichenlaub $
 *  \author Michael Eichenlaub
 *  \date   11.04.2008
*/
//----------------------------------------------------------------------------

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

#define TESTDBASPECT_TESTTRANSACTION
#ifdef TESTDBASPECT_TESTTRANSACTION

//------------------------------------------------------------------------------
SUITE(TestTransaction)
{
//------------------------------------------------------------------------------
TEST(Infx_2infxconnects_and_1transaction_bad)
{
	try
	{
		ConnectionRef connect1 = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect1.isOpen());

		ConnectionRef connect2 = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect2.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect1,
											 ON_DEMAND_CACHING);
		AccessorInstanceRef accInst2 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect2,
											 ON_DEMAND_CACHING);


		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;

		// -------------------------------------------

		accInst1.beginTransaction();

		// --- step 1 --------------------------------

		CHECK_THROW(accInst2.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a), DbException);

		// --- step 2 --------------------------------

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		propTbl = accInst1.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

		// -------------------------------------------

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

//------------------------------------------------------------------------------
TEST(Infx_2infxconnects_and_1transaction_concurrent_transaction)
{
	try
	{
		ConnectionRef connect1 = TestConnection::createTestConnection(TestConnection::INFX_CONCURR_TRANSACT);
		CHECK(connect1.isOpen());

		ConnectionRef connect2 = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect2.isOpen());

		AccessorInstanceRef accInst1 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect1,
											 ON_DEMAND_CACHING);
		AccessorInstanceRef accInst2 = Manager::getInstance().createAccessorInstance
											("Inst_ButTable_Successive_Statements",
											 ACCESSORNAME_ButTable_Successive_Statements,
											 connect2,
											 ON_DEMAND_CACHING);


		ExecuteResultInfo               ret;
		AccessorPropertyTableRef        propTbl;
		AccessorPropertyTable_YIterator yit;

		accInst1.beginTransaction();

		// --- step 1 --------------------------------

		accInst2.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a);
		CHECK( ! ret.hasError() );

		propTbl = accInst2.getPropertyTable();
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

		ret = accInst1.execute(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b);
		CHECK( ! ret.hasError() );

		propTbl = accInst1.getPropertyTable();
		CHECK( !propTbl.isNull() );

		yit = propTbl.begin();
		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK_EQUAL(2010      , yit.getInt16 ("colsmint")        );
		CHECK_EQUAL("COLCH210", yit.getString("colch2"  ).c_str());
		CHECK_EQUAL("COLCH210", yit.getString("colvch2" ).c_str());

		++yit;
		CHECK(yit == propTbl.end());

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

//------------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	#define DEF_COLKEY		"colint"
	#define DEF_COLTEXT		"colvch3"

	#define DEF_COLKEY_VAL	-256
	#define DEF_COLTEXT_VAL	"abcdefGHIJK"

	//--------------------------------------------------------------------------
	static ConstString ACCDEF         = "YIterTransAccDefName" ;
	static ConstString ACCINST        = "YIterTransAccInstName";
	static ConstString ACCMETH_QUERY  = "YIterTransAccMethNameQuery";
	static ConstString ACCMETH_WRITE  = "YIterTransAccMethNameWrite";

	//--------------------------------------------------------------------------
	BEGIN_QUERY_BUILDER_DECLARATION(QueryBuilderYIterTrans)
	END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION(WriteBuilderYIterTrans)
	END_BUILDER_DECLARATION

	//--------------------------------------------------------------------------
    BUILDER_DEFINITION       (QueryBuilderYIterTrans)
    DONT_CHECK_PROPERTIES	 (QueryBuilderYIterTrans)
    SQL_PATTERN              (QueryBuilderYIterTrans,
							 "SELECT FIRST 1 "
							  DEF_COLKEY  ", " 
						      DEF_COLTEXT " "
							 "FROM but_master;")

    BUILDER_DEFINITION       (WriteBuilderYIterTrans)
    ENSURE_PROPERTIES_ARE_SET(WriteBuilderYIterTrans, DEF_COLKEY  "; "
													  DEF_COLTEXT ";")
    SQL_PATTERN              (WriteBuilderYIterTrans,
							 "INSERT INTO but_var "
							 "(" DEF_COLKEY  ", " 
						    	 DEF_COLTEXT ") "
							 "VALUES "
							 "(#" DEF_COLKEY  "#, " 
							 "'#" DEF_COLTEXT "#');")

	//--------------------------------------------------------------------------
	namespace ns_YIterTrans
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF)
			PROPERTY_DESCRIPTION_LIST("int32  " DEF_COLKEY  ";"
									  "string " DEF_COLTEXT ";")
			SQL_BUILDER_CREATE       (QueryBuilderYIterTrans)
			SQL_BUILDER_CREATE       (WriteBuilderYIterTrans)

			ACCESS_METHOD(ACCMETH_QUERY)
			SQL_BUILDER_PUSH_BACK(QueryBuilderYIterTrans)

			ACCESS_METHOD(ACCMETH_WRITE)
			SQL_BUILDER_PUSH_BACK(WriteBuilderYIterTrans)
		END_ACCESSOR_DEFINITION
	}

	//--------------------------------------------------------------------------
	class FxYIterBase
	{
	public:
		FxYIterBase() : conn      (TestConnection::createDefaultConnectionRef()),
			            accInstSql(Manager::getInstance().createAccessorInstance(ACCINST,
					 												             ACCDEF,
																			     conn,
																				 ON_DEMAND_CACHING))
		{
		}

		~FxYIterBase()
		{
			VarString sqlDel;
			sqlDel.format("DELETE FROM but_var WHERE %s = %d AND %s = '%s';",
						  DEF_COLKEY , DEF_COLKEY_VAL , 
						  DEF_COLTEXT, DEF_COLTEXT_VAL);

			sql::ConnectionRef  sqlConn = conn   .getSqlConnection();
			StatementRef        stmt    = sqlConn.createStatement ();
			ExecuteReturnInfo   ret     = stmt   .execute         (sqlDel);

			// CHECK ( !ret.hasError());                  C4297 function assumed not to throw an exception but does
			// CHECK ( (0 == (int)ret.m_affectedRows) ||  C4297 function assumed not to throw an exception but does
			// 	    (1 == (int)ret.m_affectedRows)   );
		}

		static bool isYItUnChanged(const AccessorPropertyTable_YIterator it,
		                       	   SupportedStateEnum                    state)
		{
			if (DEF_COLKEY_VAL != it.getInt32(DEF_COLKEY))
				return false;

			if (state != it.getState(DEF_COLKEY).getState())
				return false;

			if (I18nString(DEF_COLTEXT_VAL) != it.getString(DEF_COLTEXT) )
				return false;

			if (state != it.getState(DEF_COLTEXT).getState())
				return false;

			return true;
		}

	protected:
		ConnectionRef                   conn;
		AccessorInstanceRef             accInstSql;
	};

	//--------------------------------------------------------------------------
	class FxYIterUnattached : public FxYIterBase
	{
	public:
		FxYIterUnattached()
		{
			accInstVal   = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
					 												             ACCDEF,
																			     ConnectionRef());
			accPropTblVal = accInstVal.getPropertyTable();
			accPropTblVal.insert(FOR_CLEAN);

			accPropTblYItVal = accPropTblVal.begin();

			accPropTblYItVal.setInt32 (DEF_COLKEY , DEF_COLKEY_VAL );
			accPropTblYItVal.setString(DEF_COLTEXT, DEF_COLTEXT_VAL);
		}

	protected:
		AccessorInstanceRef             accInstVal;
		AccessorPropertyTableRef	    accPropTblVal;
		AccessorPropertyTable_YIterator accPropTblYItVal;
	};

	//--------------------------------------------------------------------------
	class FxYIterAttached : public FxYIterBase
	{
	public:
		FxYIterAttached()
		{
			ExecuteResultInfo res = accInstSql.execute(ACCMETH_QUERY);
			CHECK (! res.hasError());

			accPropTblVal = accInstSql.getPropertyTable();
			CHECK_EQUAL(1, (int)accPropTblVal.size());

			accPropTblYItVal = accPropTblVal.begin();
			CHECK(accPropTblVal.end() != accPropTblYItVal);

			CHECK_EQUAL(100001    , accPropTblYItVal.getInt32 (DEF_COLKEY ));
			CHECK_EQUAL("0COLCH21", accPropTblYItVal.getString(DEF_COLTEXT));

			accPropTblYItVal.setInt32 (DEF_COLKEY , DEF_COLKEY_VAL );
			accPropTblYItVal.setString(DEF_COLTEXT, DEF_COLTEXT_VAL);
		}

	protected:
		AccessorPropertyTableRef	    accPropTblVal;
		AccessorPropertyTable_YIterator accPropTblYItVal;
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterUnattached, Unattached_YIterator_without_commit_unchanged)
{
	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterUnattached, Unattached_YIterator_after_commit_unchanged)
{
	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.beginTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.commitTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterUnattached, Unattached_YIterator_after_rollback_unchanged)
{
	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.beginTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.rollbackTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterAttached, Attached_YIterator_without_commit_changed)
{
	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_UPDATE));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		{
			AccessorPropertyTableRef tbl = accInstSql.getPropertyTable();
			CHECK_EQUAL(1, (int)tbl.size());

			AccessorPropertyTable_YIterator yit = tbl.begin();
			CHECK(tbl.end() != yit);
			CHECK( isYItUnChanged(yit, basar::SS_CLEAN));
		}

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterAttached, Attached_YIterator_after_commit_changed)
{
	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_UPDATE));

		accInstSql.beginTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_UPDATE));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.commitTransaction();

		{
			AccessorPropertyTableRef tbl = accInstSql.getPropertyTable();
			CHECK_EQUAL(1, (int)tbl.size());

			AccessorPropertyTable_YIterator yit = tbl.begin();
			CHECK(tbl.end() != yit);
			CHECK( isYItUnChanged(yit, basar::SS_CLEAN));
		}

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxYIterAttached, Attached_YIterator_after_rollback_unchanged)
{
	using basar::property::PropertyTable_YIterator;

	try
	{
		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_UPDATE));

		accInstSql.beginTransaction();

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_UPDATE));

		ExecuteResultInfo res = accInstSql.execute(ACCMETH_WRITE, accPropTblYItVal);
		CHECK      (!res.hasError());
		CHECK_EQUAL(1, (int)res.getAffectedRows());

		CHECK( isYItUnChanged(accPropTblYItVal, basar::SS_CLEAN));

		accInstSql.rollbackTransaction();

		{
			AccessorPropertyTableRef tbl = accInstSql.getPropertyTable();
			CHECK_EQUAL(1, (int)tbl.size());

			AccessorPropertyTable_YIterator yit = tbl.begin();
			CHECK(tbl.end() != yit);
			CHECK( isYItUnChanged(yit, basar::SS_UPDATE));
		}

	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
    catch(...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool occurred = false;
        CHECK(occurred);
    }
}

//------------------------------------------------------------------------------
}	// SUITE

#endif	// TESTDBACPECT_TESTTRANSACTION

//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   23.04.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql.h"

#include "testconnection.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"

#include <string.h>

#define TESTDBASPECT_TESTFIXEDSTRINGS
#ifdef TESTDBASPECT_TESTFIXEDSTRINGS

//------------------------------------------------------------------------------
SUITE(TestFixedStrings)
{

using basar::VarString;
using basar::Int32;
using basar::UInt32;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
TEST(RawNvlStringInfx)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef sqlConnect = connect.getSqlConnection();
		sqlConnect.setRawFetching(true);

		AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButTable_FixedStringsRaw",
																					ACCESSORNAME_ButTable_FixedStrings,
																					connect,
																					ON_DEMAND_CACHING);

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButTables_FixedStrings_Select_a);

		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();

		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		VarString col1;
		VarString col2;

		while (yit != propTbl.end())
		{
			col1 = yit.getString(PROPDEF_COLNVL1.getName());
			col2 = yit.getString(PROPDEF_COLNVL2.getName());
	
			++yit;
		}

		CHECK      ( yit == propTbl.end());
		CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));

		VarString::value_type blanks[41] = { 0 };
		memset(blanks, ' ', sizeof(blanks) - 1);

		CHECK_EQUAL(blanks,  col1.c_str());
		CHECK_EQUAL(sizeof(blanks) - 1, static_cast<UInt32>(strlen(col1.c_str())) );

		CHECK_EQUAL("  ",  col2.c_str());
		CHECK_EQUAL(2, static_cast<Int32>(strlen(col2.c_str())) );

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
TEST(TrimNvlStringInfx)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		basar::db::sql::ConnectionRef sqlConnect = connect.getSqlConnection();
		sqlConnect.setRawFetching(false);

		AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButTable_FixedStringsTrim",
																					ACCESSORNAME_ButTable_FixedStrings,
																					connect,
																					ON_DEMAND_CACHING);

		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButTables_FixedStrings_Select_a);

		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();

		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		VarString col1;
		VarString col2;

		while (yit != propTbl.end())
		{
			col1 = yit.getString(PROPDEF_COLNVL1.getName());
			col2 = yit.getString(PROPDEF_COLNVL2.getName());
	
			++yit;
		}

		CHECK      ( yit == propTbl.end());
		CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));

		CHECK_EQUAL("", col1.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col1.c_str())) );

		CHECK_EQUAL("",  col2.c_str());
		CHECK_EQUAL(0, static_cast<Int32>(strlen(col2.c_str())) );
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

#endif	// TESTDBASPECT_TESTFIXEDSTRINGS

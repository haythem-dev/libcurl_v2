#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"

#include "testconnection.h"
#include "testnames.h"
#include "odbcfix.h"

#define TESTDBACPECT_TESTLOWERCASECOLUMNS
#ifdef TESTDBACPECT_TESTLOWERCASECOLUMNS

//------------------------------------------------------------------------------
SUITE(TestLowercaseColumns)
{

//------------------------------------------------------------------------------
using basar::Int32;
using basar::Int16;
using basar::ConstString;
using basar::I18nString;
using basar::UnknownPropertyNameException;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
TEST(ColumnNamesINFX)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(connect.isOpen());

		AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButMasterTableReader_SomeValues",
																					ACCESSORNAME_ButMasterTableReader_SomeValues,
																					connect,
																					ON_DEMAND_CACHING);
		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMasterTableReader_SomeValues);

		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();

		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK(      0 < yit.getInt32 ("colser"  ));
		CHECK( 100000 < yit.getInt32 ("colint"  ));
		CHECK(   2000 < yit.getInt16 ("colsmint"));

        CHECK( ""    != yit.getString("colch1"  ));

		CHECK_THROW(yit.getInt32 ("COLSER"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getInt32 ("COLINT"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getInt16 ("COLSMINT"), UnknownPropertyNameException);
		CHECK_THROW(yit.getString("COLCH1"  ), UnknownPropertyNameException);
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

TEST_FIXTURE(OdbcFix, ColumnNamesODBC)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(connect.isOpen());

		AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButMasterTableReader_SomeValues",
																					ACCESSORNAME_ButMasterTableReader_SomeValues,
																					connect,
																					ON_DEMAND_CACHING);
		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButMasterTableReader_SomeValues);

		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();

		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		CHECK(      0 < yit.getInt32 ("colser"  ));
		CHECK( 100000 < yit.getInt32 ("colint"  ));
		CHECK(   2000 < yit.getInt16 ("colsmint"));
		CHECK( ""    != yit.getString("colch1"  ));

		CHECK_THROW(yit.getInt32 ("COLSER"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getInt32 ("COLINT"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getInt16 ("COLSMINT"), UnknownPropertyNameException);
		CHECK_THROW(yit.getString("COLCH1"  ), UnknownPropertyNameException);
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

#endif 

//------------------------------------------------------------------------------
#ifdef HAS_CICS

TEST(ColumnNamesCICS)
{
	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK(connect.isOpen());

		AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_CICS_VSAM_DKPAR_UT_Reader_Lean",
																					ACCESSORNAME_CICS_VSAM_DKPAR_UT_Reader_Lean,
																					connect,
																					ON_DEMAND_CACHING);
		ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Reader_Lean);

		CHECK( ! ret.hasError() );

		AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
		CHECK( !propTbl.isNull() );

		AccessorPropertyTable_YIterator yit = propTbl.begin();

		CHECK( ! yit.isNull() );
		CHECK( yit != propTbl.end());

		// check property decsription list
		CHECK( yit.contains("par_k_fil" ));
		CHECK(!yit.contains("PAR_K_FIL" ));
		CHECK( yit.contains("col0_upper"));
		CHECK(!yit.contains("COL0_UPPER"));
		CHECK( yit.contains("col0_lower"));
		CHECK(!yit.contains("COL0_LOWER"));

		CHECK( yit.contains("dkpar_euihw"));
		CHECK(!yit.contains("DKPAR_EUIHW"));
		CHECK( yit.contains("col1_upper" ));
		CHECK(!yit.contains("COL1_UPPER" ));
		CHECK( yit.contains("col1_lower" ));
		CHECK(!yit.contains("COL1_LOWER" ));

		CHECK( yit.contains("test_date" ));
		CHECK(!yit.contains("TEST_DATE" ));
		CHECK( yit.contains("col2_upper"));
		CHECK(!yit.contains("COL2_UPPER"));
		CHECK( yit.contains("col2_lower"));
		CHECK(!yit.contains("COL2_LOWER"));

		CHECK( yit.contains("test_time"    ));
		CHECK(!yit.contains("TEST_TIME"    ));
		CHECK( yit.contains("col3_upper"   ));
		CHECK(!yit.contains("COL3_UPPER"   ));
		CHECK( yit.contains("col3_lower"   ));
		CHECK(!yit.contains("COL3_LOWER"   ));

		CHECK( yit.contains("test_float2"));
		CHECK(!yit.contains("TEST_FLOAT2"));
		CHECK( yit.contains("col4_upper" ));
		CHECK(!yit.contains("COL4_UPPER" ));
		CHECK( yit.contains("col4_lower" ));
		CHECK(!yit.contains("COL4_LOWER" ));

		// check YIterator
		yit.getString   ("par_k_fil"    );
		yit.getString   ("col0_upper"   );
		yit.getString   ("col0_lower"   );
		yit.getDecimal  ("dkpar_euihw"  );
		yit.getDecimal  ("col1_upper"   );
		yit.getDecimal  ("col1_lower"   );
		yit.getDate     ("test_date"    );
		yit.getDate     ("col2_upper"   );
		yit.getDate     ("col2_lower"   );
		yit.getTime     ("test_time"    );
		yit.getTime     ("col3_upper"   );
		yit.getTime     ("col3_lower"   );
		yit.getFloat64  ("test_float2"  );
		yit.getFloat64  ("col4_upper"   );
		yit.getFloat64  ("col4_lower"   );

		CHECK_THROW(yit.getString   ("PAR_K_FIL"    ), UnknownPropertyNameException);
		CHECK_THROW(yit.getString   ("COL0_UPPER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getString   ("COL0_LOWER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDecimal  ("DKPAR_EUIHW"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDecimal  ("COL1_UPPER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDecimal  ("COL1_LOWER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDate     ("TEST_DATE"    ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDate     ("COL2_UPPER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getDate     ("COL2_LOWER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getTime     ("TEST_TIME"    ), UnknownPropertyNameException);
		CHECK_THROW(yit.getTime     ("COL3_UPPER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getTime     ("COL3_LOWER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getFloat64  ("TEST_FLOAT2"  ), UnknownPropertyNameException);
		CHECK_THROW(yit.getFloat64  ("COL4_UPPER"   ), UnknownPropertyNameException);
		CHECK_THROW(yit.getFloat64  ("COL4_LOWER"   ), UnknownPropertyNameException);

		++yit;
		CHECK(yit != propTbl.end());
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

#endif 

//------------------------------------------------------------------------------
}	// SUITE
#endif

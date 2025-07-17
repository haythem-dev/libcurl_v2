//----------------------------------------------------------------------------
/** $Id$
 *  \author Michael Eichenlaub
 *  \date   18.02.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "cicsvsamdkpar_ut.h"
#include "testnames.h"
#include "libbasarproperty.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_Reader_Lean)
    BUILDER_DEFINITION(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_Reader_Lean)
    SQL_PATTERN(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_Reader_Lean,
				"SELECT "
					"PAR_K_FIL    , PAR_K_FIL     AS COL0_UPPER, PAR_K_FIL     AS col0_lower, "
					"DKPAR_EUIHW  , DKPAR_EUIHW   AS COL1_UPPER, DKPAR_EUIHW   AS col1_lower, "
					"TEST_DATE    , TEST_DATE     AS COL2_UPPER, TEST_DATE     AS col2_lower, "
					"TEST_TIME    , TEST_TIME     AS COL3_UPPER, TEST_TIME     AS col3_lower, "
					"TEST_FLOAT2  , TEST_FLOAT2   AS COL4_UPPER, TEST_FLOAT2   AS col4_lower  "
				"FROM VSAM.DKPAR_UT"
			   )
 
namespace accessor_cics_vsam_dkpar_ut_reader_lean
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_CICS_VSAM_DKPAR_UT_Reader_Lean)
		PROPERTY_DESCRIPTION_LIST("string   par_k_fil; "
		                          "string   col0_upper; "
		                          "string   col0_lower; "
		                          "decimal  dkpar_euihw; "
								  "decimal  col1_upper; "
								  "decimal  col1_lower; "
								  "date     test_date; "
								  "date     col2_upper; "
								  "date     col2_lower; "
								  "time     test_time; "
								  "time     col3_upper; "
								  "time     col3_lower; "
								  "float64  test_float2; "
								  "float64  col4_upper; "
								  "float64  col4_lower; ")
		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Reader_Lean)
		SQL_BUILDER_CREATE(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_Reader_Lean)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_Reader_Lean)
	END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------

DONT_CHECK_PROPERTIES(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_a)
    BUILDER_DEFINITION(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_a)
    SQL_PATTERN(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_a,
				"SELECT "
					"par_k_fil, "
					"par_k_name, "
					"par_k_nr, "
					"test_lint "
				"FROM VSAM.DKPAR_UT "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = 'PA29' "
				"AND   par_k_nr   = '01';"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_b)
    BUILDER_DEFINITION(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_b)
    SQL_PATTERN(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_b,
				"SELECT "
					"par_k_fil, "
					"par_k_name, "
					"par_k_nr, "
					"test_lint "
				"FROM VSAM.DKPAR_UT "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = '01' "
				"AND   par_k_nr   = '03';"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_c)
    BUILDER_DEFINITION(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_c)
    SQL_PATTERN(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_c,
				"SELECT "
					"par_k_fil, "
					"par_k_name, "
					"par_k_nr, "
					"test_lint "
				"FROM     VSAM.DKPAR_UT "
				"WHERE    par_k_fil  = '01' "
				"AND      par_k_nr   = '01' "
				"ORDER BY par_k_name;"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_count)
    BUILDER_DEFINITION(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_count)
    SQL_PATTERN(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_count,
				"SELECT "
				    "count(*) "
				"FROM VSAM.DKPAR_UT "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = '01' "
				"AND   par_k_nr   = '03';"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_a)
    BUILDER_DEFINITION(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_a)
    SQL_PATTERN(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_a,
				"UPDATE VSAM.DKPAR_UT "
				"SET dummy = 'BasarUnitTest (successive test a)' "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = 'PA29' "
				"AND   par_k_nr   = '01';"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_b)
    BUILDER_DEFINITION(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_b)
    SQL_PATTERN(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_b,
				"UPDATE VSAM.DKPAR_UT "
				"SET dummy = 'BasarUnitTest (successive test b)' "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = '01' "
				"AND   par_k_nr   = '03';"
			   )
 
DONT_CHECK_PROPERTIES(SQLBuilderInsert_CICS_VSAM_DKPAR_UT_1_row)
    BUILDER_DEFINITION(SQLBuilderInsert_CICS_VSAM_DKPAR_UT_1_row)
    SQL_PATTERN(SQLBuilderInsert_CICS_VSAM_DKPAR_UT_1_row,
				"INSERT INTO VSAM.DKPAR_UT "
				"(PAR_K_FIL, PAR_K_NAME, par_k_nr, DUMMY) "
				"VALUES "
				"('01', '01', '03', 'BasarUnitTest (insert-update-test)');"
				)

namespace accessor_cics_vsam_dkpar_ut_successive_statements
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements)

		PROPERTY_DESCRIPTION_LIST("string par_k_fil; "
		                          "string par_k_name; "
		                          "string par_k_nr; "
								  "int32  test_lint; ")

		SQL_BUILDER_CREATE(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_a)
		SQL_BUILDER_CREATE(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_b)
		SQL_BUILDER_CREATE(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_c)
		SQL_BUILDER_CREATE(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_count)
		SQL_BUILDER_CREATE(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_a)
		SQL_BUILDER_CREATE(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_b)
		SQL_BUILDER_CREATE(SQLBuilderInsert_CICS_VSAM_DKPAR_UT_1_row)

		// ----------------------------------------------------------

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_a)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_b)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_c)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_1_row_c)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_SelectCount)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_CICS_VSAM_DKPAR_UT_count)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a)
		SQL_BUILDER_PUSH_BACK(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_a)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b)
		SQL_BUILDER_PUSH_BACK(SQLBuilderUpdate_CICS_VSAM_DKPAR_UT_1_row_b)

		ACCESS_METHOD(ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Insert)
		SQL_BUILDER_PUSH_BACK(SQLBuilderInsert_CICS_VSAM_DKPAR_UT_1_row)

	END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------

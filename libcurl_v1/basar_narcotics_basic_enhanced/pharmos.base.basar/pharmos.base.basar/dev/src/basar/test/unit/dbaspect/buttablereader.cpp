/*
 * @file buttablereader.cpp
 * @author Anke Klink
 * @date 2007
 */
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "buttablereader.h"
#include "libbasarproperty.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;
using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------
// Variable List of Values
// -----------------------------------------------------------------------------------------------------------------
VarString selectString_SQLBuilderQuery_ButMasterTableReader_SomeValues(
    "select " + propertyDescriptionVectorButTableDescriptionSomeValues.toCommaSeparatedNames() +  " from but_master");

DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_SomeValues)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_SomeValues)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_SomeValues,
    selectString_SQLBuilderQuery_ButMasterTableReader_SomeValues.c_str())

 
namespace accessor_read_butmastertable_SomeValues
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_SomeValues)
		PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionSomeValues)
		ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_SomeValues)
		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_SomeValues)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_SomeValues)
	END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// Integer only:
// -----------------------------------------------------------------------------------------------------------------

VarString selectString_SQLBuilderQuery_ButMasterTableReader_Int("select " + 
																propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() +  
																" from but_master");
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_Int)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_Int)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_Int,
    selectString_SQLBuilderQuery_ButMasterTableReader_Int.c_str())

 
namespace accessor_read_butmastertable_int
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_Int)
		PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionIntValues)
		ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_Int)
		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_Int)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_Int)
	END_ACCESSOR_DEFINITION
}
// -----------------------------------------------------------------------------------------------------------------
// All Values:
// -----------------------------------------------------------------------------------------------------------------

VarString selectString_SQLBuilderQuery_ButMasterTableReader_All("select " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master");

 DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All,
    selectString_SQLBuilderQuery_ButMasterTableReader_All.c_str())

// test with properties as string
 namespace accessor_read_butmastertable_all
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All)
      PROPERTY_DESCRIPTION_LIST(
      "int32 colser; int32 colint; int16 colsmint; "
      "float64 colfl ; float32 colsmfl ; "
      "decimal coldec ; decimal colmon ; "
      "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
      "string colvch3 ; string colvch4 ; string collvch ;"
      "date coldate ;"
      "datetime coldt1 ; datetime coldt2 ; time coltime1 ; ")

      ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_All)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All)
      END_ACCESSOR_DEFINITION
 }

 // test with properties added
  namespace accessor_read_butmastertable_all_2 
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All_2)
      BEGIN_PROPERTY_DESCRIPTION_LIST 
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSER);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDEC);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLMON);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH3);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH4);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDATE);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLTIME1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLLVCH);
      END_PROPERTY_DESCRIPTION_LIST

      ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_All)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All)
      END_ACCESSOR_DEFINITION
 }

// test with properties as vector:
 namespace accessor_read_butmastertable_all_3 
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All_3)
    PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAll)
      ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_All)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All)
      END_ACCESSOR_DEFINITION
 }

 //no results:
 VarString selectString_SQLBuilderQuery_ButMasterTableReader_All_no_results("select " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master where 1 = 0");
  DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All_no_results)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All_no_results)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All_no_results,
    selectString_SQLBuilderQuery_ButMasterTableReader_All_no_results.c_str())

 namespace accessor_read_butmastertable_all_no_results
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All_no_results)
      BEGIN_PROPERTY_DESCRIPTION_LIST 
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSER);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDEC);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLMON);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH3);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH4);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDATE);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLTIME1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLLVCH);
      END_PROPERTY_DESCRIPTION_LIST

      ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_All_no_results)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All_no_results)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All_no_results)
      END_ACCESSOR_DEFINITION
 }

 //one result:
 VarString selectString_SQLBuilderQuery_ButMasterTableReader_All_one_result("select first 1 " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master");
  DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All_one_result)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All_one_result)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All_one_result,
    selectString_SQLBuilderQuery_ButMasterTableReader_All_one_result.c_str())

 namespace accessor_read_butmastertable_all_one_result
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All_one_result)
      BEGIN_PROPERTY_DESCRIPTION_LIST 
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSER);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMINT);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSMFL);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDEC);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLMON);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH3);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLVCH4);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDATE);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLDT2);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLTIME1);
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLLVCH);
      END_PROPERTY_DESCRIPTION_LIST

      ACCESS_METHOD(ACCESSMETHOD_ButMasterTableReader_All_one_result)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All_one_result)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All_one_result)
      END_ACCESSOR_DEFINITION
 }
// -----------------------------------------------------------------------------------------------------------------

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_ButTable_Join_byColser_Int,"selectedid;")
    BUILDER_DEFINITION(SQLBuilderQuery_ButTable_Join_byColser_Int)
    SQL_PATTERN(SQLBuilderQuery_ButTable_Join_byColser_Int,
    "select k.colser as master_colser , k.colint as master_colint , k.colsmint as master_colsmint ,  t.colser as detail_colser, t.colint as detail_colint, t.colsmint as detail_colsmint from but_master k left outer join but_detail t  on k.colser = t.colint where k.colser = #selectedid# order by t.colsmint ")


namespace accessor_buttable_join_bycolser_int
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButTable_Join_byColser_Int)
      PROPERTY_DESCRIPTION_LIST(
      "int32 master_colser; int32 master_colint; int16 master_colsmint; "
      "int32 detail_colser; int32 detail_colint; int16 detail_colsmint; ")

      ACCESS_METHOD(ACCESSMETHOD_ButTable_Join_byColser_Int)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButTable_Join_byColser_Int)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButTable_Join_byColser_Int)
      END_ACCESSOR_DEFINITION
 }

// -----------------------------------------------------------------------------------------------------------------
// misc tests:
// -----------------------------------------------------------------------------------------------------------------
	VarString Sql_SQLBuilderQuery_ButMaster_a(	"SELECT FIRST 1 " + 
												propertyDescriptionVectorButTableSuccessiveStatements.toCommaSeparatedNames() +  
												" FROM BUT_MASTER "
												"ORDER BY colint;");
	DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_a)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_a)
    SQL_PATTERN(SQLBuilderQuery_ButMaster_a, Sql_SQLBuilderQuery_ButMaster_a.c_str())


	VarString Sql_SQLBuilderQuery_ButMaster_b(	"SELECT FIRST 1 " + 
												propertyDescriptionVectorButTableSuccessiveStatements.toCommaSeparatedNames() +  
												" FROM BUT_MASTER "
												"ORDER BY colint DESC;");
	DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_b)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_b)
    SQL_PATTERN(SQLBuilderQuery_ButMaster_b, Sql_SQLBuilderQuery_ButMaster_b.c_str())

	VarString Sql_SQLBuilderQuery_ButMaster_c(	"SELECT FIRST 2 " + 
												propertyDescriptionVectorButTableSuccessiveStatements.toCommaSeparatedNames() +  
												" FROM BUT_MASTER "
												"ORDER BY colint;");
	DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_c)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_c)
    SQL_PATTERN(SQLBuilderQuery_ButMaster_c, Sql_SQLBuilderQuery_ButMaster_c.c_str())

    
    //SQLBuilderQuery_ButMaster_Single
	VarString Sql_SQLBuilderQuery_ButMaster_Single(	"SELECT FIRST 1 " + 
												propertyDescriptionVectorButTableSuccessiveStatements.toCommaSeparatedNames() +  
												" FROM BUT_MASTER;");
	DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_Single)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_Single)
    SQL_PATTERN(SQLBuilderQuery_ButMaster_Single, Sql_SQLBuilderQuery_ButMaster_Single.c_str())

	DONT_CHECK_PROPERTIES(SQLBuilderUpdate_ButVar_a)
    BUILDER_DEFINITION(SQLBuilderUpdate_ButVar_a)
    SQL_PATTERN(SQLBuilderUpdate_ButVar_a,
				"UPDATE but_var "
				"SET    colch2 = 'i2345', colvch2 = 'j2345' "
				"WHERE  colch2 = 'COLCH21';"
			   )
 
	DONT_CHECK_PROPERTIES(SQLBuilderUpdate_ButVar_b)
    BUILDER_DEFINITION(SQLBuilderUpdate_ButVar_b)
    SQL_PATTERN(SQLBuilderUpdate_ButVar_b,
				"UPDATE but_var "
				"SET    colch2 = 'k2345', colvch2 = 'l2345' "
				"WHERE  colch2 = 'COLCH210';"
			   )

	DONT_CHECK_PROPERTIES(SQLBuilderAggregQuery_ButMaster_a)
    BUILDER_DEFINITION(SQLBuilderAggregQuery_ButMaster_a)
    SQL_PATTERN(SQLBuilderAggregQuery_ButMaster_a, "SELECT COUNT(*) FROM BUT_MASTER;")

	DONT_CHECK_PROPERTIES(SQLBuilderSetIsolation)
    BUILDER_DEFINITION(SQLBuilderSetIsolation)
    SQL_PATTERN(SQLBuilderSetIsolation, "SET ISOLATION TO DIRTY READ;")


	DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_cWrong)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_cWrong)
    SQL_PATTERN(SQLBuilderQuery_ButMaster_cWrong, Sql_SQLBuilderQuery_ButMaster_c.c_str())

 
	DONT_CHECK_PROPERTIES(SQLBuilderAggregQuery_ButMaster_aWrong)
    BUILDER_DEFINITION(SQLBuilderAggregQuery_ButMaster_aWrong)
    SQL_PATTERN(SQLBuilderAggregQuery_ButMaster_aWrong, "SELECT COUNT(*) FROM BUT_MASTER;")

	DONT_CHECK_PROPERTIES(SQLBuilderSetIsolationWrong)
    BUILDER_DEFINITION(SQLBuilderSetIsolationWrong)
    SQL_PATTERN(SQLBuilderSetIsolationWrong, "SET ISOLATION TO DIRTY READ;")

// -----------------------------------------------------------------------------------------------------------------
// SuccessiveStatements test:
// -----------------------------------------------------------------------------------------------------------------

namespace accessor_buttable_successive_statements
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButTable_Successive_Statements)

		//PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableSuccessiveStatements)
		// contains tabs and spaces
		PROPERTY_DESCRIPTION_LIST("int16   	colsmint; "		
								  "string	colch2; "
								  "string	colvch2; "
								 );



		// ----------------------------------------------------------

		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_a )
		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_b )
		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_c )
		SQL_BUILDER_CREATE(SQLBuilderUpdate_ButVar_a)
		SQL_BUILDER_CREATE(SQLBuilderUpdate_ButVar_b)

		// ----------------------------------------------------------

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_Successive_Statements_Select_a)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_a)

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_Successive_Statements_Select_b)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_b)

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_Successive_Statements_Select_c)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_c)

		ACCESS_METHOD(ACCESSMETHOD_ButVar_Successive_Statements_Update_a)
		SQL_BUILDER_PUSH_BACK(SQLBuilderUpdate_ButVar_a)

		ACCESS_METHOD(ACCESSMETHOD_ButVar_Successive_Statements_Update_b)
		SQL_BUILDER_PUSH_BACK(SQLBuilderUpdate_ButVar_b)

	END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// statement type tests:
// -----------------------------------------------------------------------------------------------------------------

namespace accessor_buttable_statementtypes
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButTable_Statement_Types)

		PROPERTY_DESCRIPTION_LIST(propDescVecButTableStmtTypes)

		// ----------------------------------------------------------

		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_c      )
		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_Single )
		SQL_BUILDER_CREATE(SQLBuilderSetIsolation           )
		SQL_BUILDER_CREATE(SQLBuilderAggregQuery_ButMaster_a)

		SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_cWrong      )
		SQL_BUILDER_CREATE(SQLBuilderSetIsolationWrong           )
		SQL_BUILDER_CREATE(SQLBuilderAggregQuery_ButMaster_aWrong)

		// ----------------------------------------------------------

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_StmtTypes_Select)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_c)

        ACCESS_METHOD(ACCESSMETHOD_ButMaster_StmtTypes_SingleQuery)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_Single)

		ACCESS_METHOD(ACCESSMETHOD_ButVar_StmtTypes_Execute)
		SQL_BUILDER_PUSH_BACK(SQLBuilderSetIsolation)

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_StmtTypes_Aggreg)
		SQL_BUILDER_PUSH_BACK(SQLBuilderAggregQuery_ButMaster_a)

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_StmtTypes_SelectWrong)
		SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_cWrong)

		ACCESS_METHOD(ACCESSMETHOD_ButVar_StmtTypes_ExecuteWrong)
		SQL_BUILDER_PUSH_BACK(SQLBuilderSetIsolationWrong)

		ACCESS_METHOD(ACCESSMETHOD_ButMaster_StmtTypes_AggregWrong)
		SQL_BUILDER_PUSH_BACK(SQLBuilderAggregQuery_ButMaster_aWrong)

	END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// TimeSpan tests:
// -----------------------------------------------------------------------------------------------------------------

VarString Sql_SQLBuilderQuery_ButMaster_SelectTimeSpan(	"SELECT first 1 colspan1, colvch4 "
											" FROM BUT_MASTER "
											"ORDER BY colint;");
DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_SelectTimeSpan)
BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_SelectTimeSpan)
SQL_PATTERN(SQLBuilderQuery_ButMaster_SelectTimeSpan, Sql_SQLBuilderQuery_ButMaster_SelectTimeSpan.c_str())


VarString Sql_SQLBuilderQuery_ButMaster_UpdateTimeSpan(	"UPDATE BUT_MASTER SET "
													   " colspan1='#colspan1#' "
											"WHERE colser = 1");
DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMaster_UpdateTimeSpan)
BUILDER_DEFINITION(SQLBuilderQuery_ButMaster_UpdateTimeSpan)
SQL_PATTERN(SQLBuilderQuery_ButMaster_UpdateTimeSpan, Sql_SQLBuilderQuery_ButMaster_UpdateTimeSpan.c_str())

namespace accessor_buttable_timespan
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestTimespan)
		
		BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLSPAN1);
		END_PROPERTY_DESCRIPTION_LIST

	// ----------------------------------------------------------

	SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_SelectTimeSpan)
	SQL_BUILDER_CREATE(SQLBuilderQuery_ButMaster_UpdateTimeSpan)
	// ----------------------------------------------------------

	ACCESS_METHOD(ACCESSMETHOD_ButTables_TimeSpan_Select)
	SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_SelectTimeSpan)

	ACCESS_METHOD(ACCESSMETHOD_ButTables_TimeSpan_Update)
	SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMaster_UpdateTimeSpan)

	END_ACCESSOR_DEFINITION
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

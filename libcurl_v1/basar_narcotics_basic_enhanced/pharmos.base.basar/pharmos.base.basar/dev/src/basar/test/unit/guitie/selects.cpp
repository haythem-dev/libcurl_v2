 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "selects.h"
#include "accessornames.h"
#include "buttablepropertydescriptionvectors.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;
using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------
// string tests:
VarString getNumberOfSelectMoreLines()
{
    VarString retval;
    retval.format("%d",NUMBER_OF_SELECT_MORE_LINES);
    return retval;
}

VarString selectStringSelectTwoStringsOneLine = " SELECT  first 1 " 
        + propDescVecButTableTwoStrings.toCommaSeparatedNames() 
        + " from but_master ";

BUILDER_DEFINITION(TwoStringSelectOneLine)
    DONT_CHECK_PROPERTIES(TwoStringSelectOneLine)
    SQL_PATTERN(TwoStringSelectOneLine,selectStringSelectTwoStringsOneLine.c_str())


VarString selectStringSelectAllStringsOneLine = " SELECT  first 1 "  
        + propDescVecButTableDescAllStrings.toCommaSeparatedNames() 
        + " from but_master ";

BUILDER_DEFINITION(StringSelectOneLine)
    DONT_CHECK_PROPERTIES(StringSelectOneLine)
    SQL_PATTERN(StringSelectOneLine,selectStringSelectAllStringsOneLine.c_str())

VarString selectStringSelectTwoStringsMoreLines = " SELECT first " 
       + getNumberOfSelectMoreLines() 
       + " "
       + propDescVecButTableTwoStrings.toCommaSeparatedNames() 
       + " from but_master ";

BUILDER_DEFINITION(TwoStringSelectMoreLines)
    DONT_CHECK_PROPERTIES(TwoStringSelectMoreLines)
    SQL_PATTERN(TwoStringSelectMoreLines,selectStringSelectTwoStringsMoreLines.c_str())


VarString selectStringSelectAllStringsMoreLines = " SELECT first " 
       + getNumberOfSelectMoreLines() 
       + " "
       + propDescVecButTableDescAllStrings.toCommaSeparatedNames() 
       + " from but_master ";

BUILDER_DEFINITION(StringSelectMoreLines)
    DONT_CHECK_PROPERTIES(StringSelectMoreLines)
    SQL_PATTERN(StringSelectMoreLines,selectStringSelectAllStringsMoreLines.c_str())


namespace accessor_selectTwoString
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_SELECT_TWOSTRINGS)
        PROPERTY_DESCRIPTION_LIST(propDescVecButTableTwoStrings)
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
            SQL_BUILDER_CREATE(TwoStringSelectOneLine)
            SQL_BUILDER_PUSH_BACK(TwoStringSelectOneLine)  
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
            SQL_BUILDER_CREATE(TwoStringSelectMoreLines)
            SQL_BUILDER_PUSH_BACK(TwoStringSelectMoreLines)  
    END_ACCESSOR_DEFINITION
}


namespace accessor_selectTwoString_reverse
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE)
        PROPERTY_DESCRIPTION_LIST(propDescVecButTableTwoStringsReverse)
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
            SQL_BUILDER_CREATE(TwoStringSelectOneLine)
            SQL_BUILDER_PUSH_BACK(TwoStringSelectOneLine)  
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
            SQL_BUILDER_CREATE(TwoStringSelectMoreLines)
            SQL_BUILDER_PUSH_BACK(TwoStringSelectMoreLines)  
    END_ACCESSOR_DEFINITION
}

namespace accessor_selectStrings
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_SELECT_STRINGS)
        PROPERTY_DESCRIPTION_LIST(propDescVecButTableDescAllStrings)
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
            SQL_BUILDER_CREATE(StringSelectOneLine)
            SQL_BUILDER_PUSH_BACK(StringSelectOneLine)  
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
            SQL_BUILDER_CREATE(StringSelectMoreLines)
            SQL_BUILDER_PUSH_BACK(StringSelectMoreLines)  
    END_ACCESSOR_DEFINITION
}

namespace accessor_selectStrings_reverse 
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_SELECT_STRINGS_REVERSE)
        PROPERTY_DESCRIPTION_LIST(propDescVecButTableDescAllStringsReverse)
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
            SQL_BUILDER_CREATE(StringSelectOneLine)
            SQL_BUILDER_PUSH_BACK(StringSelectOneLine)  
        ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
            SQL_BUILDER_CREATE(StringSelectMoreLines)
            SQL_BUILDER_PUSH_BACK(StringSelectMoreLines)  
    END_ACCESSOR_DEFINITION
}

// all tests:

VarString selectString_SQLBuilderQuery_ButMasterTableReader_All(
    " SELECT first " 
       + getNumberOfSelectMoreLines() 
    + propDescVecButTableDescAll.toCommaSeparatedNames() 
    +  " from but_master");


 DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All,
    selectString_SQLBuilderQuery_ButMasterTableReader_All.c_str())

VarString selectString_SQLBuilderQuery_ButMasterTableReader_All_OneLine(
    "select first 1 " 
    + propDescVecButTableDescAll.toCommaSeparatedNames() 
    +  " from but_master");

DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All_OneLine)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All_OneLine)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All_OneLine,
    selectString_SQLBuilderQuery_ButMasterTableReader_All_OneLine.c_str())



 namespace accessor_read_butmastertable_all
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All)
    PROPERTY_DESCRIPTION_LIST(propDescVecButTableDescAll)
      ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All)
     ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All_OneLine)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All_OneLine)
      END_ACCESSOR_DEFINITION
 }

 // ------------------------------------------------------------------------------------------

 VarString selectStringSelectAllDatesOneLine = 
		" SELECT  first 1 "
		"coldate, coldt1, coldt2, coltime1, "
		"DATETIME(2019-08-31) YEAR TO DAY AS coldt_date, "
		"DATETIME(23:46:57.123) HOUR TO FRACTION(3) AS coldt_time "
		"FROM but_detail WHERE colser = 14";


BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_AllDates_OneLine)
    DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_AllDates_OneLine)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_AllDates_OneLine,selectStringSelectAllDatesOneLine.c_str())


 namespace accessor_read_butmastertable_all_dates
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_AllDates)
    PROPERTY_DESCRIPTION_LIST(propDescVecButTableDescAllDates)
      //ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
      //SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_AllDates)
      //SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_AllDates)
     ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
      SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_AllDates_OneLine)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_AllDates_OneLine)
      END_ACCESSOR_DEFINITION
 }

 // ----------------------------------------------------------------------------------------------

 VarString selectString_SQLBuilderQuery_ButMasterTable_TimeSpan("Select first 1 " + propDescVecButTableDescTimeSpan.toCommaSeparatedNames() + " from but_master");
 
DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_TimeSpan)
	BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_TimeSpan)
	SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_TimeSpan,
	selectString_SQLBuilderQuery_ButMasterTable_TimeSpan.c_str())

namespace accessor_read_butmastertable_timespan
{
	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_TimeSpan)
		PROPERTY_DESCRIPTION_LIST(propDescVecButTableDescTimeSpan)
		ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_ONELINE)
			SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_TimeSpan)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_TimeSpan)
		END_ACCESSOR_DEFINITION
}




VarString selectString_SQLBuilderQuery_ButMasterTableReader_All_Plus(
    " SELECT first " 
       + getNumberOfSelectMoreLines() 
    + propDescVecButTableDescAll.toCommaSeparatedNames() 
	+ ", " + PROPDEF_COLCH1.getName() + " as unknownaccstring "
    +  " from but_master");

DONT_CHECK_PROPERTIES(SQLBuilderQuery_ButMasterTableReader_All_Plus)
    BUILDER_DEFINITION(SQLBuilderQuery_ButMasterTableReader_All_Plus)
    SQL_PATTERN(SQLBuilderQuery_ButMasterTableReader_All_Plus,
    selectString_SQLBuilderQuery_ButMasterTableReader_All_Plus.c_str())

namespace accessor_read_butmastertable_all_plus
 {
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_ButMasterTableReader_All_Plus)
       PROPERTY_DESCRIPTION_LIST(
	       propDescVecButTableDescAll.toDescriptionString() + "string unknownaccstring;")
      ACCESS_METHOD(ACCESSORMETHODNAME_SELECT_MORELINES)
         SQL_BUILDER_CREATE(SQLBuilderQuery_ButMasterTableReader_All_Plus)
          SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_ButMasterTableReader_All_Plus)
      END_ACCESSOR_DEFINITION
 }

 // ----------------------------------------------------------------------------------------------

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

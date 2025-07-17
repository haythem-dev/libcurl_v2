 /** $Id$                                          
  *                                                  
  * @file testsqlresolvequeries.cpp                                  
  * @brief ...                                       
  * @author Anke Klink                      
  * @date 2007                            
  */    
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testsqlresolvequeries.h"
#include "libbasarproperty.h"
#include "testnames.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;

using basar::property::PropertyDescriptionArray;
using basar::property::PropertyDescriptionVector;

using basar::db::aspect::SQLStringBuilder;
// -----------------------------------------------------------------------------------------------------------------


// select Statement
VarString selectStringBase("select " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master where ");

// various where statements:
VarString whereEntryStringCOLSER = 
          PROPDEF_COLSER.getName() + " = " +  PROPDEF_COLSER.toSQLReplacementString();

VarString whereEntryStringCOLCH1 = 
          PROPDEF_COLCH1.getName()   + " = '" +  PROPDEF_COLCH1.toSQLReplacementString() + "' ";

VarString whereEntryStringCOLDT1 = 
          PROPDEF_COLDT1.getName()   +  " =  datetime(" +  PROPDEF_COLDT1.toSQLReplacementString()     + ") year to fraction ";

VarString whereEntryStringNoFloatsNoDates =             
         PROPDEF_COLSMINT.getName() + " = "  +  PROPDEF_COLSMINT.toSQLReplacementString() + " and "
       + PROPDEF_COLDEC.getName()   + " = "  +  PROPDEF_COLDEC.toSQLReplacementString()   + " and "
       + PROPDEF_COLMON.getName()   + " = "  +  PROPDEF_COLMON.toSQLReplacementString()   + " and "
       + PROPDEF_COLCH1.getName()   + " = '" +  PROPDEF_COLCH1.toSQLReplacementString()   + "' and "
       + PROPDEF_COLCH2.getName()   + " = '" +  PROPDEF_COLCH2.toSQLReplacementString()   + "' and "
       + PROPDEF_COLVCH1.getName()  + " = '" +  PROPDEF_COLVCH1.toSQLReplacementString()  + "' and "
       + PROPDEF_COLVCH2.getName()  + " = '" +  PROPDEF_COLVCH2.toSQLReplacementString()  + "' and "
       + PROPDEF_COLVCH3.getName()  + " = '" +  PROPDEF_COLVCH3.toSQLReplacementString()  + "' and "
       + PROPDEF_COLVCH4.getName()  + " = '" +  PROPDEF_COLVCH4.toSQLReplacementString()  + "' and "
       + PROPDEF_COLLVCH.getName()  + " = '" +  PROPDEF_COLLVCH.toSQLReplacementString()  + "' ";


VarString whereEntryStringFloats  = 
         PROPDEF_COLFL.getName()    + " = " +  PROPDEF_COLFL.toSQLReplacementString()    + " and "
       + PROPDEF_COLSMFL.getName()  + " = " +  PROPDEF_COLSMFL.toSQLReplacementString();
       
VarString whereEntryStringDates =
         PROPDEF_COLDATE.getName()  +  " =  date('" +  PROPDEF_COLDATE.toSQLReplacementString()   + "') and "
       + PROPDEF_COLDT1.getName()   +  " =  datetime(" +  PROPDEF_COLDT1.toSQLReplacementString()     + ") year to fraction and "
       + PROPDEF_COLDT2.getName()   +  " =  datetime(" +  PROPDEF_COLDT2.toSQLReplacementString()     + ") year to fraction and "
       + PROPDEF_COLTIME1.getName() +  " = " +  PROPDEF_COLTIME1.toSQLReplacementString();

VarString whereEntryStringAll             = whereEntryStringNoFloatsNoDates + " and " + whereEntryStringDates + " and " + whereEntryStringFloats;
VarString whereEntryStringNoFloats        = whereEntryStringNoFloatsNoDates + " and " + whereEntryStringDates;
VarString whereEntryStringNoDates         = whereEntryStringNoFloatsNoDates + " and " + whereEntryStringFloats;

VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere(selectStringBase + whereEntryStringAll);

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere, propertyDescriptionVectorButTableDescriptionAllStartSMINT)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere,selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere.c_str())
 
namespace accessor_sqlbuilder_selectwhere
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAll)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere)
      SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere)
      END_ACCESSOR_DEFINITION
 }

// -----------------------------------------------------------------------------------------------------------------

VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate(selectStringBase + whereEntryStringNoDates);

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate,propertyDescriptionVectorButTableDescriptionAllStartSMINT)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate, selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate.c_str())

namespace accessor_sqlbuilder_selectwhere_nodate
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_nodate)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAll )
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nodate)
      SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nodate)
      END_ACCESSOR_DEFINITION
 }
// -----------------------------------------------------------------------------------------------------------------
VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat(selectStringBase + whereEntryStringNoFloatsNoDates);

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat,propertyDescriptionVectorButTableDescriptionAllStartSMINT)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat,
    selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat.c_str())

namespace accessor_sqlbuilder_selectwhere_nofloat
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_nofloat)
     PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAll )
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat)
      SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat)
      SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_nofloat)
      END_ACCESSOR_DEFINITION
 }
// -----------------------------------------------------------------------------------------------------------------

VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString(
    selectStringBase + whereEntryStringCOLSER + " and " + whereEntryStringCOLCH1 );
const PropertyDescriptionArray propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString =  
    {
		PROPDEF_COLSER, 
        PROPDEF_COLCH1 
    };
const PropertyDescriptionVector 
        propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString(2, propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString);


 ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString, propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString,selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString.c_str())

namespace accessor_testsqlbuilder_selectwhere_testendswithmaskedstring
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString)
      END_ACCESSOR_DEFINITION
 }

// -----------------------------------------------------------------------------------------------------------------

VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1(
    selectStringBase + whereEntryStringCOLSER + " and " + PROPDEF_COLVCH1.getName() + "='\\#'");

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1,
						  propertyDescriptionVectorButTableDescriptionColserOnly) 
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1, selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1.c_str())

namespace accessor_testsqlbuilder_selectwhere_markedstringselecttest1 
 { 
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest1) 
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionColserOnly) 
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest1) 
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1) 
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest1) 
      END_ACCESSOR_DEFINITION 
 } 
 
// -------------------------------------------------------------------------------------------------------------

 VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2(
    selectStringBase + whereEntryStringCOLSER + " and " + PROPDEF_COLVCH2.getName() + "='\\\\#' ");

 ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2, 
						  propertyDescriptionVectorButTableDescriptionColserOnly) 
 BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2)
 SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2, selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2.c_str())

 namespace accessor_testsqlbuilder_selectwhere_markedstringselecttest2 
 { 
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest2) 
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionColserOnly) 
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest2) 
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2) 
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest2) 
      END_ACCESSOR_DEFINITION 
 } 
 
// -------------------------------------------------------------------------------------------------------------

 VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3(
    selectStringBase + whereEntryStringCOLSER + " and " + PROPDEF_COLVCH3.getName() + "='\\\\ \\\\ \\\\ \\ \\#\\#\\#\\#\\#' ");

 ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3, propertyDescriptionVectorButTableDescriptionColserOnly) 
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3, selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3.c_str())

namespace accessor_testsqlbuilder_selectwhere_markedstringselecttest3 
 { 
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest3) 
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionColserOnly) 
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest3) 
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3) 
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest3) 
      END_ACCESSOR_DEFINITION 
 } 
 
// -------------------------------------------------------------------------------------------------------------

 VarString selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4(
    selectStringBase 
    + PROPDEF_COLVCH3.getName() + "='\\\\ \\\\ \\\\ \\ \\#\\#\\#\\#\\#' and "
    + whereEntryStringCOLSER + " and " 
    + PROPDEF_COLVCH2.getName() + "='\\\\#' and "
    + PROPDEF_COLVCH1.getName() + "='\\#' ");
 
 ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4, propertyDescriptionVectorButTableDescriptionColserOnly)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4,selectString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4.c_str())

namespace accessor_testsqlbuilder_selectwhere_markedstringselecttest4
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest4)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionColserOnly)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest4)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_markedStringSelectTest4)
      END_ACCESSOR_DEFINITION
}
 
 // -------------------------------------------------------------------------------------------------------------
VarString sqlString_TestSqlBuilder_SelectWhere_likeTest1 = 
       selectStringBase + PROPDEF_COLCH2.getName() + " like '%#" + PROPDEF_COLCH1.getName() +"#' ";

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_likeTest1, propertyDescriptionVectorButTableDescriptionColserOnly)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_likeTest1)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_likeTest1, sqlString_TestSqlBuilder_SelectWhere_likeTest1.c_str())

namespace accessor_testsqlbuilder_selectwhere_liketest1
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_likeTest1)
      PROPERTY_DESCRIPTION_LIST(
             "int32 colser; string colch1;"
        )
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_likeTest1)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_likeTest1)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_likeTest1)
      END_ACCESSOR_DEFINITION
 }
// -------------------------------------------------------------------------------------------------------------

 // select ... from but_master where colser=#colser# and date='#date#')
 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date = 
     "   select "   + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() + " from but_master "   
     +  "where  "   + PROPDEF_COLSER.getName() +  "="        + PROPDEF_COLSER.toSQLReplacementString() 
     +  "and    "   + PROPDEF_COLDATE.getName()+  "= date('" + PROPDEF_COLDATE.toSQLReplacementString() + "')";
    
 basar::property::PropertyDescriptionArray propertyArr_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date = {PROPDEF_COLSER, PROPDEF_COLDATE };
 basar::property::PropertyDescriptionVector propertyVec_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date(2,propertyArr_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date);


ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date, propertyVec_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date )
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date, sqlString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date.c_str())

namespace accessor_testsqlbuilder_selectwhere_date
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_Date)
      PROPERTY_DESCRIPTION_LIST(propertyVec_SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_Date)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_Date)
      END_ACCESSOR_DEFINITION
 }
// -------------------------------------------------------------------------------------------------------------

 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime = 
    selectStringBase + whereEntryStringCOLDT1 + " and " +  whereEntryStringCOLSER;

 const PropertyDescriptionArray propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime =  
    {
		PROPDEF_COLSER, 
        PROPDEF_COLDT1
    };
const PropertyDescriptionVector 
        propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime(2, propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_SelectWhere_testEndsWithMaskedString);


 ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime, propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime)
    BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime)
    SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime,
	sqlString_SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime.c_str())

namespace accessor_testsqlbuilder_selectwhere_datetime
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_SelectWhere_DateTime)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_SelectWhere_DateTime)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_SelectWhere_DateTime)
      END_ACCESSOR_DEFINITION
 }

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

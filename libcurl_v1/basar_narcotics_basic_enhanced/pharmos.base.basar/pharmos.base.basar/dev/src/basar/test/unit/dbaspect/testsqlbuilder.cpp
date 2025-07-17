 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"

#include "testsqlbuilder.h"
#include "testnames.h"

#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;

using basar::property::PropertyDescriptionArray;
using basar::property::PropertyDescriptionVector;

using basar::db::aspect::SQLStringBuilder;

// -------------------------------------------------------------------------------------------------------------

 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere = 
    "select " + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() +  " from but_master where "
    + PROPDEF_COLINT.getName() + " = "  +  PROPDEF_COLINT.toSQLReplacementString() + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLSER.getName() + " = " +  PROPDEF_COLSER.toSQLReplacementString();

 const PropertyDescriptionArray propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere =  
    {
        PROPDEF_COLINT,
		PROPDEF_COLSER,
        PROPDEF_COLCH2
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere(3, propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere);

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere, propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere,
	sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere.c_str())

ENSURE_PROPERTIES_ARE_SET(
                          SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2, 
                          " colint;  colser;  colch2;")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2,
	sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere.c_str())

ENSURE_PROPERTIES_ARE_SET(
                          SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3, 
                          " colch2;  colint;  colser;")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3,
	sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere.c_str())


namespace accessor_testsqlbuilder_IntAndOneString
 {
  BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_IntAndOneString_SelectWhere)
      PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
      ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2)
    ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3)
        SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3)
        SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3)
      END_ACCESSOR_DEFINITION
 }

 
 // -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------


 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere = 
    "select " + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() +  " from but_master where "
    + PROPDEF_COLSER.getName() + " = " +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLINT.getName() + " = "  +  PROPDEF_COLINT.toSQLReplacementString() + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 
 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert = 
    "insert into but_var (" 
    + PROPDEF_COLSMINT.getName() +","
    + PROPDEF_COLCH2.getName() + ","
    + PROPDEF_COLINT.getName() + ","
    + PROPDEF_COLCH1.getName() + ") values ("
    + PROPDEF_COLSER.toSQLReplacementString() +",'"
    + PROPDEF_COLCH2.toSQLReplacementString() + "',"
    + PROPDEF_COLINT.toSQLReplacementString() + ",'"
    + PROPDEF_COLCH1.toSQLReplacementString() + "') "; 

 VarString sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntOnly = 
    "select " + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() +  " from but_master ";

 const PropertyDescriptionArray propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere =  
    {
        PROPDEF_COLCH1,
        PROPDEF_COLINT,
		PROPDEF_COLSER,
        PROPDEF_COLCH2
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere(4, propertyDescriptionArraySQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere);

ENSURE_PROPERTIES_ARE_SET(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere, propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere,
            sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere.c_str())

ENSURE_PROPERTIES_ARE_SET(
    SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2, 
    "colch1; colint;  colser;  colch2 ;")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2,
    sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere.c_str())

ENSURE_PROPERTIES_ARE_SET(
    SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3, 
    " colch2; colch1; colint;  colser ;")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3,
    sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere.c_str())

ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
           SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert, 
           "colch1; colint;  colser;  colch2 ;", "INSERT")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert,
            sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert.c_str())

ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
           SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert_2, 
           propertyDescriptionVectorSQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere, "INSERT")
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert_2)
SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert_2,
            sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert.c_str())

DONT_CHECK_PROPERTIES(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly)
BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly)
                      SQL_PATTERN(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly,
            sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntOnly.c_str())

namespace accessor_testsqlbuilder_IntAndTwoString
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_EnsureTest)
        PROPERTY_DESCRIPTION_LIST("string colch1;int32 colint; int32 colser;string  colch2;")
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert)
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert_2)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert_2)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert_2)
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly)

    END_ACCESSOR_DEFINITION
}



BUILDER_DEFINITION(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_OneInsert)
ENSURE_PROPERTIES_ARE_SET(
           SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_OneInsert, 
           "colch1; colint;  colser;  colch2 ;")

	void SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_OneInsert::buildSQLString()
	{
    resolve(
            sqlString_SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_Insert.c_str());
    }

namespace accessor_testsqlbuilder_intandtwostring_oneinsert
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_IntAndTwoStrings_OneInsert)
        PROPERTY_DESCRIPTION_LIST("string colch1;int32 colint; int32 colser;string  colch2;")
        ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_OneInsert)
           SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_OneInsert)
           SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_OneInsert)
    END_ACCESSOR_DEFINITION
}

namespace accessor_testsqlbuilder_checkDefaultParameterList
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestDefaultParameter_SetAndGet)
         PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAll)
		 ACCESS_METHOD(ACCESSMETHOD_OneSqlbuilder_1)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere)
		 ACCESS_METHOD(ACCESSMETHOD_OneSqlbuilder_2)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_2)
			 ACCESS_METHOD(ACCESSMETHOD_OneSqlbuilder_3)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndOneString_SelectWhere_3)
		 ACCESS_METHOD(ACCESSMETHOD_MoreSqlbuilder_1)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2)
			SQL_BUILDER_CREATE(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
			SQL_BUILDER_PUSH_BACK(SQLBuilderQuery_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3)
      END_ACCESSOR_DEFINITION
}

 // -------------------------------------------------------------------------------------------------------------

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

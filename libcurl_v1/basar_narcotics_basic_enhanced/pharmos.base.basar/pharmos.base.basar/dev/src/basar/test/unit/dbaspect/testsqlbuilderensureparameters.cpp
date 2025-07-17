 /*
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "testsqlbuilderensureparameters.h"
#include "libbasarproperty.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"


// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;

using basar::property::PropertyDescriptionArray;
using basar::property::PropertyDescriptionVector;

using basar::db::aspect::SQLStringBuilder;

// -------------------------------------------------------------------------------------------------------------

 VarString sqlString_TestSqlBuilder_EnsureParameters_SelectInt = 
    "select first 1 " 
    + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames()
    + " from but_master";

VarString sqlString_TestSqlBuilder_EnsureParameters_SelectAllEnsureParameter = 
    "select first 1 " 
    + PROPDEF_COLCH2.getName()   + ", "  
    + PROPDEF_COLCH1.getName()   + ", "  
    + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() 
    + " from but_master";
  
VarString sqlString_TestSqlBuilder_EnsureParameters_SelectIntOnly = 
    "select first 1 " 
    + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() 
    +  " from but_master ";

 VarString sqlString_TestSqlBuilder_EnsureParameters_SelectWhere = 
    "select " 
    + propertyDescriptionVectorButTableDescriptionIntValues.toCommaSeparatedNames() 
    + " from but_master where "
    + PROPDEF_COLSER.getName()   + " = "   +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLINT.getName()   + " = "   +  PROPDEF_COLINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLSMINT.getName()   + " = "   +  PROPDEF_COLSMINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 VarString sqlString_TestSqlBuilder_EnsureParameters_Insert = 
    "insert into but_var (" 
    + PROPDEF_COLLVCH.getName() + ","
    + PROPDEF_COLFL.getName() + ","
    + PROPDEF_COLCH2.getName()   + ","
    + PROPDEF_COLINT.getName()   + ","
    + PROPDEF_COLCH1.getName()   + ","
    + PROPDEF_COLSMINT.getName() + ") values ("
    + " 'DBASPECT_ENSURETEST_INSERT' ,"
    + PROPDEF_COLSER.toSQLReplacementString() + ",'"
    + PROPDEF_COLCH2.toSQLReplacementString() + "',"
    + PROPDEF_COLINT.toSQLReplacementString() + ",'"
    + PROPDEF_COLCH1.toSQLReplacementString() + "',"
    + PROPDEF_COLSMINT.toSQLReplacementString() + " ) "; 

 VarString sqlString_TestSqlBuilder_EnsureParameters_Update = 
    "update but_var set " +  PROPDEF_COLLVCH.getName() + " ='DBASPECT_ENSURETEST_UPDATE'  where "
    + PROPDEF_COLSER.getName()   + " = "   +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLSMINT.getName() + " = "   +  PROPDEF_COLSMINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLINT.getName()   + " = "   +  PROPDEF_COLINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 VarString sqlString_TestSqlBuilder_EnsureParameters_Delete = 
    "delete from but_var where "
    + PROPDEF_COLSER.getName()   + " = "   +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLSMINT.getName() + " = "   +  PROPDEF_COLSMINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLINT.getName()   + " = "   +  PROPDEF_COLINT.toSQLReplacementString()  + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 // statements UnsetAndInStateTest:

  VarString sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert = 
    "insert into but_var (" 
    + PROPDEF_COLLVCH.getName() + ","
    + PROPDEF_COLFL.getName() + ","
    + PROPDEF_COLCH2.getName()   + ","
    + PROPDEF_COLCH1.getName()   + ","
    + PROPDEF_COLSMINT.getName() + ") values ("
    + " 'DBASPECT_ENSURETEST_INSERT2', "
    + PROPDEF_COLSER.toSQLReplacementString() + ",'"
    + PROPDEF_COLCH2.toSQLReplacementString() + "','"
    + PROPDEF_COLCH1.toSQLReplacementString() + "',"
    + " 41) "; 

 VarString sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update = 
    "update but_var set " +  PROPDEF_COLLVCH.getName() + " = 'DBASPECT_ENSURETEST_UPDATE2'  where "
    + PROPDEF_COLSER.getName()   + " = "   +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 VarString sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete = 
    "delete from but_var where "
    + PROPDEF_COLSER.getName()   + " = "   +  PROPDEF_COLSER.toSQLReplacementString()  + " and "
    + PROPDEF_COLCH2.getName()   + " = '"  +  PROPDEF_COLCH2.toSQLReplacementString()  + "' and "
    + PROPDEF_COLCH1.getName()   + " = '"  +  PROPDEF_COLCH1.toSQLReplacementString()  + "'";

 // Parameterlist: EnsureParameters

 const PropertyDescriptionArray propertyDescriptionArrayTestSqlBuilder_EnsureParameters =  
    {
        PROPDEF_COLCH1,
        PROPDEF_COLINT,
        PROPDEF_COLSMINT,
		PROPDEF_COLSER,
        PROPDEF_COLCH2
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorTestSqlBuilder_EnsureParameters(
        5, 
        propertyDescriptionArrayTestSqlBuilder_EnsureParameters);

VarString parameterString_TestSqlBuilder_EnsureParameters_WithBlanks = " colch1 ; colint ;  colser ;  colch2 ; colsmint; ";
VarString parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks = "colch1;colch2;colint;colser;colsmint;";

// Parameterlist: EnsureYParameters

 const PropertyDescriptionArray propertyDescriptionArrayTestSqlBuilder_EnsureYParameters =  
    {
        PROPDEF_COLINT,
        PROPDEF_COLSMINT,
		PROPDEF_COLSER
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorTestSqlBuilder_EnsureYParameters(
        3, 
        propertyDescriptionArrayTestSqlBuilder_EnsureYParameters);

VarString parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks = "colint;colser;colsmint;";
VarString parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks = " colint ; colsmint ; colser ; ";

 // Parameterlist: EnsureParameters_UnsetAndInStateTest
 const PropertyDescriptionArray propertyDescriptionArrayTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam =  
    {
        PROPDEF_COLCH1,
		PROPDEF_COLSER,
        PROPDEF_COLCH2
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam(3, propertyDescriptionArrayTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam);

VarString parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks = "colch1;colser;colch2;";
VarString parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks = " colch1 ;   colser ;  colch2 ; ";

const PropertyDescriptionArray propertyDescriptionArrayTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam =  
    {
        PROPDEF_COLINT,
		PROPDEF_COLSMINT,
		PROPDEF_COLFL
    };

const PropertyDescriptionVector 
        propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam(
           3, 
           propertyDescriptionArrayTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam);

VarString parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks = "colint;colsmint;";
VarString parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks = " colint ; colsmint ; ";

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Select Y-Iterator:
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly , 
             sqlString_TestSqlBuilder_EnsureParameters_SelectInt.c_str() )
DONT_CHECK_PROPERTIES(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly)

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters , 
             sqlString_TestSqlBuilder_EnsureParameters_SelectAllEnsureParameter.c_str() )
DONT_CHECK_PROPERTIES(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters)

namespace acessornamespace_TestSqlBuilder_EnsureParameters_SelectYIterator
{ 
   BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator) 
       PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorTestSqlBuilder_EnsureParameters ); 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters)
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters ) 
    END_ACCESSOR_DEFINITION 
}

namespace acessornamespace_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly
{ 
   BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly) 
       PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorTestSqlBuilder_EnsureYParameters ); 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly ) 
  END_ACCESSOR_DEFINITION 
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : Select
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere , 
             sqlString_TestSqlBuilder_EnsureParameters_SelectWhere.c_str() )
ENSURE_PROPERTIES_ARE_SET(
             TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters )

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere , 
             sqlString_TestSqlBuilder_EnsureParameters_SelectWhere.c_str() )
ENSURE_PROPERTIES_ARE_SET(
             TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks )

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere , 
             sqlString_TestSqlBuilder_EnsureParameters_SelectWhere.c_str() )
ENSURE_PROPERTIES_ARE_SET(
             TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks )

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : Insert
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             propertyDescriptionVectorTestSqlBuilder_EnsureYParameters , 
             basar::db::aspect::PropertyStateSet(basar::SS_INSERT))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "INSERT")

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : Update
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             propertyDescriptionVectorTestSqlBuilder_EnsureYParameters , 
             basar::db::aspect::PropertyStateSet(basar::SS_UPDATE))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "UPDATE")

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : Delete
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(
             TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters , 
             propertyDescriptionVectorTestSqlBuilder_EnsureYParameters , 
             basar::db::aspect::PropertyStateSet(basar::SS_DELETE))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureYParameters_WithoutBlanks , 
             "DELETE")

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : UnsetAndInStateTest Insert
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam , 
             basar::db::aspect::PropertyStateSet(basar::SS_INSERT))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "INSERT")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Insert.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "INSERT")
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : UnsetAndInStateTest Update
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam , 
             basar::db::aspect::PropertyStateSet(basar::SS_UPDATE))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "UPDATE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Update.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "UPDATE")
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : UnsetAndInStateTest Delete
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam , 
             propertyDescriptionVectorTestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam , 
             basar::db::aspect::PropertyStateSet(basar::SS_DELETE))

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "DELETE")

BUILDER_DEFINITION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete)
SQL_PATTERN( TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete , 
             sqlString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_Delete.c_str() )
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(
             TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_SetParam_WithoutBlanks , 
             parameterString_TestSqlBuilder_EnsureParameters_UnsetAndInStateTest_StateParam_WithoutBlanks , 
             "DELETE")

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Ensure Tests : Accessor
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

namespace acessornamespace_TestSqlBuilder_EnsureParameters 
{ 
   BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestSqlBuilder_EnsureParameters) 
       PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorTestSqlBuilder_EnsureParameters );
       
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly )
         SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly)
         SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly)
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters)
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters ) 
  
   // Select where (Parameters Are Set Test):
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere ) 
       // Insert
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert ) 
       // Update     
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update ) 
       // Delete:
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete ) 
   
       // Unset Test Insert:
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert ) 
      
 
       // Unset Test  Update:
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update ) 
      
       // Unset Test Delete:
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete ) 
       ACCESS_METHOD(ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_CREATE(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete )
          SQL_BUILDER_PUSH_BACK(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete ) 
      

       END_ACCESSOR_DEFINITION 
}




// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

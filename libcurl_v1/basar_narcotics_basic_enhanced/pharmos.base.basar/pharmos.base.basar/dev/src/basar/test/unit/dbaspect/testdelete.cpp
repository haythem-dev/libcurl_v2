 /** $Id$                                          
  *                                                  
  * @file testdelete.cpp                                  
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testdelete.h"
#include "libbasarproperty.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;

using basar::property::PropertyDescriptionVector;

using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------
// TestDelete_DeleteRow:
// -----------------------------------------------------------------------------------------------------------------

VarString testDeleteDeleteRow_deleteRow = "delete from but_var where " + PROPDEF_COLSER.getName() + " = " +  PROPDEF_COLSER.toSQLReplacementString() ;
VarString testDeleteDeleteRow_insertRow = "insert into but_var ( " + PROPDEF_COLINT.getName()  + "," + PROPDEF_COLSMINT.getName() + ") values (" + PROPDEF_COLINT.toSQLReplacementString() + "," + PROPDEF_COLSMINT.toSQLReplacementString() + ")" ;
VarString testDeleteDeleteRow_selectRow = "select " + PROPDEF_COLSER.getName() + ", " + PROPDEF_COLINT.getName() + ", " + PROPDEF_COLSMINT.getName()
                                          + " from but_var where " + PROPDEF_COLSER.getName() + " = " +  PROPDEF_COLSER.toSQLReplacementString();

const PropertyDescriptionVector  testDeleteDeleteRow_colserVec =  propertyDescriptionVectorButTableDescriptionColserOnly;
const PropertyDescriptionVector  testDeleteDeleteRow_colintVec =  propertyDescriptionVectorButTableDescriptionAllSettableInts;
const PropertyDescriptionVector  testDeleteDeleteRow_propVec   =  propertyDescriptionVectorButTableDescriptionAllInts;

// TestDelete_DeleteRow 
BUILDER_DEFINITION(TestDelete_DeleteRow)
    ENSURE_PROPERTIES_ARE_SET(TestDelete_DeleteRow, testDeleteDeleteRow_colserVec)
    SQL_PATTERN(TestDelete_DeleteRow, testDeleteDeleteRow_deleteRow.c_str())

// TestDelete_InsertRow 
BUILDER_DEFINITION(TestDelete_InsertRow)
    ENSURE_PROPERTIES_ARE_SET(TestDelete_InsertRow, testDeleteDeleteRow_colintVec)
    SQL_PATTERN(TestDelete_InsertRow, testDeleteDeleteRow_insertRow.c_str())

//TestDelete_SelectRow 
BUILDER_DEFINITION(TestDelete_SelectRow)
    ENSURE_PROPERTIES_ARE_SET(TestDelete_SelectRow, testDeleteDeleteRow_colserVec)
    SQL_PATTERN(TestDelete_SelectRow,testDeleteDeleteRow_selectRow.c_str())

// ACCESSMETHOD_TestDelete_DeleteRow
namespace accessor_TestDelete_DeleteRow
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestDelete_DeleteRow)
       
        PROPERTY_DESCRIPTION_LIST(testDeleteDeleteRow_propVec)
        
        ACCESS_METHOD(ACCESSMETHOD_TestDelete_DeleteRow)
        SQL_BUILDER_CREATE(TestDelete_DeleteRow)
        SQL_BUILDER_PUSH_BACK(TestDelete_DeleteRow)

        ACCESS_METHOD(ACCESSMETHOD_TestDelete_InsertRow)
        SQL_BUILDER_CREATE(TestDelete_InsertRow)
        SQL_BUILDER_PUSH_BACK(TestDelete_InsertRow)

        ACCESS_METHOD(ACCESSMETHOD_TestDelete_SelectRow)
        SQL_BUILDER_CREATE(TestDelete_SelectRow)
        SQL_BUILDER_PUSH_BACK(TestDelete_SelectRow)

    END_ACCESSOR_DEFINITION
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

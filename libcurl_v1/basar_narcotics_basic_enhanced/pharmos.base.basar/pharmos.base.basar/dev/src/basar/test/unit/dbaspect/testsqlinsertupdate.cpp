 /** $Id$                                          
  *                                                  
  * @file testsqlresolvequeries.cpp                                  
  * @brief ...                                       
  * @author Anke Klink                      
  * @date 2007                            
  */    
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testsqlinsertupdate.h"

#include "libbasarproperty.h"

#include "testnames.h"
#include "buttablepropertydescriptionvectors.h"

// -----------------------------------------------------------------------------------------------------------------
using basar::VarString;
using basar::db::aspect::SQLStringBuilder;

// -----------------------------------------------------------------------------------------------------------------
//DateTimeUpdateDate:
// -----------------------------------------------------------------------------------------------------------------

VarString dateTimeUpdateDate_updateString       = "UPDATE but_detail SET coldate=date('#coldate#') ,coldt1=datetime(#coldt1#) year to fraction , coltime1=datetime(#coltime1#) hour to fraction WHERE colser = 9";
VarString dateTimeUpdateDate_selectStringOneRow = "SELECT "+ propertyDescriptionVectorButTableFirstDate.toCommaSeparatedNames() +" FROM but_detail WHERE colser = 9";
VarString dateTimeUpdateDate_selectString2Rows  = "SELECT "+ propertyDescriptionVectorButTableFirstDate.toCommaSeparatedNames() +" FROM but_detail WHERE colser = 9 or colser = 10";

BUILDER_DEFINITION(DateTimeUpdateDate)
    ENSURE_PROPERTIES_ARE_SET(DateTimeUpdateDate,propertyDescriptionVectorButTableFirstDate)
    SQL_PATTERN(DateTimeUpdateDate, 
    dateTimeUpdateDate_updateString.c_str())

    BUILDER_DEFINITION(DateTimeSelect)
    DONT_CHECK_PROPERTIES(DateTimeSelect)
    SQL_PATTERN(DateTimeSelect,dateTimeUpdateDate_selectStringOneRow.c_str())

    BUILDER_DEFINITION(DateTimeSelect2Rows)
    DONT_CHECK_PROPERTIES(DateTimeSelect2Rows)
    SQL_PATTERN(DateTimeSelect2Rows, dateTimeUpdateDate_selectString2Rows.c_str())

namespace accessor_DateTimeSelect 
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_DateTimeSelect)
       
        PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableFirstDate)
        
        ACCESS_METHOD(ACCESSMETHOD_DateTimeUpdateDate)
        SQL_BUILDER_CREATE(DateTimeUpdateDate)
        SQL_BUILDER_PUSH_BACK(DateTimeUpdateDate)

        ACCESS_METHOD(ACCESSMETHOD_DateTimeSelect)
        SQL_BUILDER_CREATE(DateTimeSelect)
        SQL_BUILDER_PUSH_BACK(DateTimeSelect)

        ACCESS_METHOD(ACCESSMETHOD_DateTimeSelect2Rows)
        SQL_BUILDER_CREATE(DateTimeSelect2Rows)
        SQL_BUILDER_PUSH_BACK(DateTimeSelect2Rows)
    END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// TestInsertUpdate_InsertOne:
// -----------------------------------------------------------------------------------------------------------------

BUILDER_DEFINITION(TestInsertUpdateInsert1)
        DONT_CHECK_PROPERTIES(TestInsertUpdateInsert1)
            SQL_PATTERN(TestInsertUpdateInsert1, 
            "insert into but_var (colint, colsmint, colvch1) values (9999, 99, 'A')")

          
namespace accessor_TestInsertUpdate_InsertOne
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestInsertUpdate_InsertOne)
        PROPERTY_DESCRIPTION_LIST(PROPDEF_COLINT.toDescriptionString() )
        
        ACCESS_METHOD(ACCESSMETHOD_TestInsertUpdate_InsertOne)
        SQL_BUILDER_CREATE(TestInsertUpdateInsert1)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdateInsert1)
          END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// TestInsertUpdate_InsertList:
// -----------------------------------------------------------------------------------------------------------------


BUILDER_DEFINITION(TestInsertUpdateInsert2)
        DONT_CHECK_PROPERTIES(TestInsertUpdateInsert2)
            SQL_PATTERN(TestInsertUpdateInsert2, 
            "insert into but_var (colint, colsmint, colvch1) values (9998, 98, 'A')")

          
namespace accessor_TestInsertUpdate_InsertList
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestInsertUpdate_InsertList)
        BEGIN_PROPERTY_DESCRIPTION_LIST
        PROPERTY_DESCRIPTION_LIST_ADD(PROPDEF_COLINT)

        ACCESS_METHOD(ACCESSMETHOD_TestInsertUpdate_InsertList)
        SQL_BUILDER_CREATE(TestInsertUpdateInsert1)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdateInsert1)
        SQL_BUILDER_CREATE(TestInsertUpdateInsert2)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdateInsert2)
          END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------
// TestInsertUpdate_InsertDuplicateOne
// -----------------------------------------------------------------------------------------------------------------
BUILDER_DEFINITION(TestInsertUpdate_InsertDuplicate)
         ENSURE_PROPERTIES_ARE_SET(TestInsertUpdate_InsertDuplicate, "colser;")
            SQL_PATTERN(TestInsertUpdate_InsertDuplicate, 
            "insert into but_var (colser, colint, colsmint, colvch1) values (#colser#,8888, 88, 'A')")

          
namespace accessor_TestInsertUpdate_InsertDuplicate
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestInsertUpdate_InsertDuplicate)
        PROPERTY_DESCRIPTION_LIST("int32 colser;")
        
        ACCESS_METHOD(ACCESSMETHOD_TestInsertUpdate_InsertDuplicate)
        SQL_BUILDER_CREATE(TestInsertUpdate_InsertDuplicate)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdate_InsertDuplicate)
    END_ACCESSOR_DEFINITION
}

namespace accessor_TestInsertUpdate_InsertListWithDuplicate
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_TestInsertUpdate_InsertListWithDuplicate)
        PROPERTY_DESCRIPTION_LIST("int32 colser;")
        
        ACCESS_METHOD(ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate)
 
        SQL_BUILDER_CREATE(TestInsertUpdateInsert1)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdateInsert1) 
        SQL_BUILDER_CREATE(TestInsertUpdate_InsertDuplicate)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdate_InsertDuplicate)
        SQL_BUILDER_CREATE(TestInsertUpdateInsert2)
        SQL_BUILDER_PUSH_BACK(TestInsertUpdateInsert2) 
    END_ACCESSOR_DEFINITION
}

// -----------------------------------------------------------------------------------------------------------------

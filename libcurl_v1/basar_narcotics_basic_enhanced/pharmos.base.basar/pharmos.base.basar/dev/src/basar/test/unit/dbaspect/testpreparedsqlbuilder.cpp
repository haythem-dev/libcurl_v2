
#include "libbasardbaspect.h"
#include "libbasarproperty.h"

#include "testpreparedsqlbuilder.h"

//build definitions
//SQLPreparedQuery_BasicTest
BUILDER_DEFINITION(SQLPreparedQuery_BasicTest)
bool SQLPreparedQuery_BasicTest::isExecutable() const
{
    return true;
}

void SQLPreparedQuery_BasicTest::buildSQLString()
{
    resolve("select * from bogustable where indexnumber = #colser#");
}

//SQLPreparedQuery_Wrong
BUILDER_DEFINITION(SQLPreparedQuery_Wrong)
bool SQLPreparedQuery_Wrong::isExecutable() const
{
    return true;
}

void SQLPreparedQuery_Wrong::buildSQLString()
{
    resolve("first select into where index=#colser# form but_master");
}

//SQLPreparedQuery_NoVariable
BUILDER_DEFINITION(SQLPreparedQuery_NoVariable)
bool SQLPreparedQuery_NoVariable::isExecutable() const
{
    return true;
}

void SQLPreparedQuery_NoVariable::buildSQLString()
{
    resolve("select frist 1 * from but_master");
}

//SQLPreparedSingleQuery_WithVariable
BUILDER_DEFINITION(SQLPreparedSingleQuery_WithVariable)
bool SQLPreparedSingleQuery_WithVariable::isExecutable() const
{
    return true;
}

void SQLPreparedSingleQuery_WithVariable::buildSQLString()
{
    resolve("select first 1 * from but_master where colser > #colser# ;");
}

//SQLPreparedAggregateQuery_WithVariable
BUILDER_DEFINITION(SQLPreparedAggregateQuery_WithVariable)
bool SQLPreparedAggregateQuery_WithVariable::isExecutable() const
{
    return true;
}

void SQLPreparedAggregateQuery_WithVariable::buildSQLString()
{
    resolve("select count(*) as coldec from but_master where colser > #colser#;");
}

//SQLPreparedWrite_BasicTest
BUILDER_DEFINITION(SQLPreparedWrite_BasicTest)
bool SQLPreparedWrite_BasicTest::isExecutable() const
{
    return true;
}

void SQLPreparedWrite_BasicTest::buildSQLString()
{
    resolve("update bogustable set somevalue = #colint# where indexnumber = #colser#");
}

//SQLPreparedQuery_ButMasterTableReader_All
const basar::VarString selectString_SQLPreparedQuery_ButMasterTableReader_All("select " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master");

 DONT_CHECK_PROPERTIES(SQLPreparedQuery_ButMasterTableReader_All)
    BUILDER_DEFINITION(SQLPreparedQuery_ButMasterTableReader_All)
    SQL_PATTERN(SQLPreparedQuery_ButMasterTableReader_All,
    selectString_SQLPreparedQuery_ButMasterTableReader_All.c_str())

//SQLPreparedQuery_ButMasterTableReader_All_WithVariable
const basar::VarString selectString_SQLPreparedQuery_ButMasterTableReader_All_WithVariable("select " + propertyDescriptionVectorButTableDescriptionAll.toCommaSeparatedNames() +  " from but_master where colser=#colser#");

 DONT_CHECK_PROPERTIES(SQLPreparedQuery_ButMasterTableReader_All_WithVariable)
    BUILDER_DEFINITION(SQLPreparedQuery_ButMasterTableReader_All_WithVariable)
    SQL_PATTERN(SQLPreparedQuery_ButMasterTableReader_All_WithVariable,
    selectString_SQLPreparedQuery_ButMasterTableReader_All_WithVariable.c_str())


//SQLPreparedInsert_ButVarTableWriter_All_WithVariables
BUILDER_DEFINITION(SQLPreparedInsert_ButVarTableWriter_All_WithVariables)
bool SQLPreparedInsert_ButVarTableWriter_All_WithVariables::isExecutable() const
{
    return true;
}

void SQLPreparedInsert_ButVarTableWriter_All_WithVariables::buildSQLString()
{
    resolve("insert into but_var(colint, colsmint, colfl, colsmfl, coldec, colmon, colch1, colch2, colvch1, colvch2, colvch3, colvch4, collvch, coldate, coldt1, coldt2, coltime1) "
            "values (#colint#, #colsmint#, #colfl#, #colsmfl#, #coldec#, #colmon#, #colch1#, #colch2#, #colvch1#, #colvch2#, #colvch3#, #colvch4#, #collvch#, #coldate#, #coldt1#, #coldt2#, #coltime1#)");
}

//SQLPreparedInsert_ButVarTableReaderAll_WithVariables
BUILDER_DEFINITION(SQLPreparedInsert_ButVarTableReaderAll_WithVariables)
bool SQLPreparedInsert_ButVarTableReaderAll_WithVariables::isExecutable() const
{
    return true;
}

void SQLPreparedInsert_ButVarTableReaderAll_WithVariables::buildSQLString()
{
    resolve("select * from but_var where colser = #colser#");
}

//Acccessor definitions
namespace query_acc
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedQueryBasicTest)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedQueryBasicTest)
        SQL_BUILDER_CREATE(SQLPreparedQuery_BasicTest)
        SQL_BUILDER_PUSH_BACK(SQLPreparedQuery_BasicTest)
    END_ACCESSOR_DEFINITION
}

namespace query_wrong_acc
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedQueryWrong)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedQueryWrong)
        SQL_BUILDER_CREATE(SQLPreparedQuery_Wrong)
        SQL_BUILDER_PUSH_BACK(SQLPreparedQuery_Wrong)
    END_ACCESSOR_DEFINITION    
}

namespace query_no_variable_acc
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedQueryNoVariable)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedQueryNoVariable)
        SQL_BUILDER_CREATE(SQLPreparedQuery_NoVariable)
        SQL_BUILDER_PUSH_BACK(SQLPreparedQuery_NoVariable)
    END_ACCESSOR_DEFINITION       
}

namespace single_aggregate_query_with_variable
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedSingleQueryWithVariable)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedSingleQueryWithVariable)
        SQL_BUILDER_CREATE(SQLPreparedSingleQuery_WithVariable)
        SQL_BUILDER_PUSH_BACK(SQLPreparedSingleQuery_WithVariable)
        
        ACCESS_METHOD(ACCESSMETHOD_PreparedAggregateQueryWithVariable)
        SQL_BUILDER_CREATE(SQLPreparedAggregateQuery_WithVariable)
        SQL_BUILDER_PUSH_BACK(SQLPreparedAggregateQuery_WithVariable)
    END_ACCESSOR_DEFINITION         
}

namespace write_acc
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedWriteBasicTest)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint;")

        ACCESS_METHOD(ACCESSMETHOD_PreparedWriteBasicTest)
        SQL_BUILDER_CREATE(SQLPreparedWrite_BasicTest)
        SQL_BUILDER_PUSH_BACK(SQLPreparedWrite_BasicTest)
    END_ACCESSOR_DEFINITION
}

 namespace accessor_prepared_read_butmastertable_all
 {
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedQueryButMasterTableReader_All)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedQueryButMasterTableReader_All)
        SQL_BUILDER_CREATE(SQLPreparedQuery_ButMasterTableReader_All)
        SQL_BUILDER_PUSH_BACK(SQLPreparedQuery_ButMasterTableReader_All)
    END_ACCESSOR_DEFINITION
 }

 namespace accessor_prepared_read_butmastertable_all_with_varaible
 {
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedQueryButMasterTableReader_All_WithVaraible)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ;")

        ACCESS_METHOD(ACCESSMETHOD_PreparedQueryButMasterTableReader_All_WithVaraible)
        SQL_BUILDER_CREATE(SQLPreparedQuery_ButMasterTableReader_All_WithVariable)
        SQL_BUILDER_PUSH_BACK(SQLPreparedQuery_ButMasterTableReader_All_WithVariable)
    END_ACCESSOR_DEFINITION
 }

namespace accessor_prepared_write_butvar_all_with_variables
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedWriteButVarTableWriter_All_WithVariables)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ; ")

        ACCESS_METHOD(ACCESSMETHOD_PreparedWriteButVarTableWriter_All_WithVariables)
        SQL_BUILDER_CREATE(SQLPreparedInsert_ButVarTableWriter_All_WithVariables)
        SQL_BUILDER_PUSH_BACK(SQLPreparedInsert_ButVarTableWriter_All_WithVariables)
    END_ACCESSOR_DEFINITION    
}

namespace accessor_prepared_read_written_butvar_all_with_variables
{
    BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_PreparedWriteButVarTableReader_All_WithVariables)
        PROPERTY_DESCRIPTION_LIST(
        "int32 colser; int32 colint; int16 colsmint; "
        "float64 colfl ; float32 colsmfl ; "
        "decimal coldec ; decimal colmon ; "
        "string  colch1  ; string   colch2 ; string colvch1 ; string  colvch2 ; "
        "string colvch3 ; string colvch4 ; string collvch ;"
        "date coldate ;"
        "datetime coldt1 ; datetime coldt2 ; time coltime1 ;")

        ACCESS_METHOD(ACCESSMETHOD_PreparedWriteButVarTableReader_All_WithVariables)
        SQL_BUILDER_CREATE(SQLPreparedInsert_ButVarTableReaderAll_WithVariables)
        SQL_BUILDER_PUSH_BACK(SQLPreparedInsert_ButVarTableReaderAll_WithVariables)
    END_ACCESSOR_DEFINITION      
}
    
//------------------------------------------------------------------------------      

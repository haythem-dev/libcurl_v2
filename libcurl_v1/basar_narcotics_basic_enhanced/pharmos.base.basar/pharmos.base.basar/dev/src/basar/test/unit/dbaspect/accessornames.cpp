#include <UnitTest++/UnitTest++.h>
#include <libbasardbaspect.h>

#include "accessornames.h"


    basar::ConstString ACCESSORNAME_ButMasterTableReader_Int                =  "Accessor_ButMasterTableReader_Int";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_All                =  "Accessor_ButMasterTableReader_All";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_All_2              =  "Accessor_ButMasterTableReader_All_2";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_All_3              =  "Accessor_ButMasterTableReader_All_3";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_All_no_results     =  "Accessor_ButMasterTableReader_All_no_results";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_All_one_result     =  "Accessor_ButMasterTableReader_All_one_result";
    basar::ConstString ACCESSORNAME_ButMasterTableReader_SomeValues         =  "Accessor_ButMasterTableReader_SomeValues";  
    basar::ConstString ACCESSORNAME_ButTable_Join_byColser_Int              =  "Accessor_ButTable_Join_byColser_Int";
    basar::ConstString ACCESSORNAME_DateTimeSelect                          =  "Accessor_DateTimeSelect";
    basar::ConstString ACCESSORNAME_TestInsertUpdate_InsertOne              =  "Accessor_TestInsertUpdate_InsertOne";
    basar::ConstString ACCESSORNAME_TestInsertUpdate_InsertList             =  "Accessor_TestInsertUpdate_InsertList";
    basar::ConstString ACCESSORNAME_TestInsertUpdate_InsertDuplicate        =  "Accessor_TestInsertUpdate_InsertDuplicate";
    basar::ConstString ACCESSORNAME_TestInsertUpdate_InsertListWithDuplicate =  "Accessor_TestInsertUpdate_InsertListWithDuplicate";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere              =  "Accessor_TestSqlBuilder_SelectWhere";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_nodate       =  "Accessor_TestSqlBuilder_SelectWhere_nodate";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_nofloat      =  "Accessor_TestSqlBuilder_SelectWhere_nofloat";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_likeTest1    =  "Accessor_TestSqlBuilder_SelectWhere_likeTest1";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_Date         =  "Accessor_TestSqlBuilder_SelectWhere_Date";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_DateTime     =  "Accessor_TestSqlBuilder_SelectWhere_DateTime";
   
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_testEndsWithMaskedString =  "Accessor_TestSqlBuilder_SelectWhere_testEndsWithMaskedString";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest1  =  "Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest1";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest2  =  "Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest2";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest3  =  "Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest3";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest4  =  "Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest4";
  
	basar::ConstString ACCESSORNAME_CICS_VSAM_DKPAR_UT_Reader_Lean           = "Accessor_CICS_VSAM_DKPAR_UT_Reader_Lean";
	basar::ConstString ACCESSORNAME_CICS_VSAM_DKPAR_UT_Successive_Statements = "Accessor_CICS_VSAM_DKPAR_UT_Successive_Statements";
	basar::ConstString ACCESSORNAME_ButTable_Successive_Statements           = "Accessor_ButTable_Successive_Statements";
	basar::ConstString ACCESSORNAME_ButTable_FixedStrings                    = "Accessor_ButTable_FixedStrings";
	basar::ConstString ACCESSORNAME_ButTable_Statement_Types                 = "Accessor_ButTable_Statement_Types";
	basar::ConstString ACCESSORNAME_ButProcs_Stored_Procs                    = "Accessor_ButProcs_Stored_Procs";

    basar::ConstString ACCESSORNAME_TestDelete_DeleteRow                     = "Accessor_ButTableVar_TestDelete_DeleteRow";

    basar::ConstString ACCESSORNAME_TestSqlBuilder_IntAndOneString_SelectWhere          = "Accessor_TestSqlBuilder_IntAndOneString_SelectWhere";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_EnsureTest                           = "Accessor_TestSqlBuilder_EnsureTest";

    basar::ConstString ACCESSORNAME_TestSqlBuilder_IntAndTwoStrings_OneInsert           = "Accessor_TestSqlBuilder_IntAndTwoStrings_OneInsert";

    basar::ConstString ACCESSORNAME_TestSqlBuilder_EnsureParameters                     = "Accessor_TestSqlBuilder_EnsureParameters";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator     = "Accessor_TestSqlBuilder_EnsureParameters_SelectYIterator";
    basar::ConstString ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly                 = "Accessor_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly";

	basar::ConstString ACCESSORNAME_TestDefaultParameter_SetAndGet = "Accessor_TestDefaultParameter_SetAndGet";

	basar::ConstString ACCESSORNAME_TestI18n                       = "Accessor_ButTable_I18n";

	basar::ConstString ACCESSORNAME_TestTimespan			   = "Accessor_ButTable_TimeSpan";

    basar::ConstString ACCESSORNAME_PreparedQueryBasicTest = "Accessor_query_basic";
    basar::ConstString ACCESSORNAME_PreparedQueryWrong = "Accessor_query_wrong";
    basar::ConstString ACCESSORNAME_PreparedQueryNoVariable = "Accessor_query_no_variable";
    basar::ConstString ACCESSORNAME_PreparedSingleQueryWithVariable = "Accessor_single_query_with_variable";
    basar::ConstString ACCESSORNAME_PreparedWriteBasicTest = "Accessor_write_basic";
    basar::ConstString ACCESSORNAME_PreparedQueryButMasterTableReader_All = "Accessor_query_buttable";
    basar::ConstString ACCESSORNAME_PreparedQueryButMasterTableReader_All_WithVaraible = "Accessor_query_buttable_variable";
    basar::ConstString ACCESSORNAME_PreparedWriteButVarTableWriter_All_WithVariables = "Accessor_write_butvar_variables";
    basar::ConstString ACCESSORNAME_PreparedWriteButVarTableReader_All_WithVariables = "Accessor_read_written_butvar_variables";
    
#include <UnitTest++/UnitTest++.h>
#include <libbasardbaspect.h>

#include "accessmethodnames.h"

    basar::ConstString ACCESSMETHOD_ButMasterTableReader_Int                 = "AccessMethod_ButMasterTableReader_Int";
    basar::ConstString ACCESSMETHOD_ButMasterTableReader_All                 = "AccessMethod_ButMasterTableReader_All";
    basar::ConstString ACCESSMETHOD_ButMasterTableReader_All_no_results      = "AccessMethod_ButMasterTableReader_All_no_results";
    basar::ConstString ACCESSMETHOD_ButMasterTableReader_All_one_result      = "AccessMethod_ButMasterTableReader_All_one_result";
    basar::ConstString ACCESSMETHOD_ButMasterTableReader_SomeValues          = "AccessMethod_ButMasterTableReader_SomeValues";

    basar::ConstString ACCESSMETHOD_ButTable_Join_byColser_Int               = "AccessMethod_ButTable_Join_byColser_Int";
    
    basar::ConstString ACCESSMETHOD_DateTimeUpdateDate                       = "AccessMethod_DateTimeUpdateDate";
    basar::ConstString ACCESSMETHOD_DateTimeSelect                           = "AccessMethod_DateTimeSelect";
    basar::ConstString ACCESSMETHOD_DateTimeSelect2Rows                      = "AccessMethod_DateTimeSelect2Rows";
    
    basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertOne               = "AccessMethod_TestInsertUpdate_InsertOne";
    basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertList              = "AccessMethod_TestInsertUpdate_InsertList";
    basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertDuplicate         = "AccessMethod_TestInsertUpdate_InsertDuplicate";
    basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate = "AccessMethod_TestInsertUpdate_InsertListWithDuplicate";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere               = "AccessMethod_TestSqlBuilder_SelectWhere";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_nodate        = "AccessMethod_TestSqlBuilder_SelectWhere_nodate";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat       = "AccessMethod_TestSqlBuilder_SelectWhere_nofloat";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_likeTest1     = "AccessMethod_TestSqlBuilder_SelectWhere_likeTest1";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_Date          = "AccessMethod_TestSqlBuilder_SelectWhere_Date";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_DateTime      = "AccessMethod_TestSqlBuilder_SelectWhere_DateTime";

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_testEndsWithMaskedString  = "AccessMethod_TestSqlBuilder_SelectWhere_testEndsWithMaskedString";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest1   = "AccessMethod_TestSqlBuilder_SelectWhere_markedStringSelectTest1";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest2   = "AccessMethod_TestSqlBuilder_SelectWhere_markedStringSelectTest2";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest3   = "AccessMethod_TestSqlBuilder_SelectWhere_markedStringSelectTest3";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest4   = "AccessMethod_TestSqlBuilder_SelectWhere_markedStringSelectTest4";
   
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Reader_Lean                       = "AccessMethod_CICS_VSAM_DKPAR_UT_Reader_Lean";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a    = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b    = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_c    = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_c";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_SelectCount = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_SelectCount";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a    = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b    = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b";
	basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Insert      = "AccessMethod_CICS_VSAM_DKPAR_UT_Successive_Statements_Insert";
	basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_a             = "AccessMethod_ButMaster_Successive_Statements_Select_a";
	basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_b             = "AccessMethod_ButMaster_Successive_Statements_Select_b";
	basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_c             = "AccessMethod_ButMaster_Successive_Statements_Select_c";
	basar::ConstString ACCESSMETHOD_ButVar_Successive_Statements_Update_a                = "AccessMethod_ButVar_Successive_Statements_Update_a";
	basar::ConstString ACCESSMETHOD_ButVar_Successive_Statements_Update_b                = "AccessMethod_ButVar_Successive_Statements_Update_b";

	basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_Select                           = "AccessMethod_ButMaster_StmtTypes_Select";
	basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_SingleQuery                      = "AccessMethod_ButMaster_StmtTypes_SingleQuery";
	basar::ConstString ACCESSMETHOD_ButVar_StmtTypes_Execute                             = "AccessMethod_ButVar_StmtTypes_Execute";
	basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_Aggreg                           = "AccessMethod_ButMaster_StmtTypes_Aggreg";
	basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_SelectWrong                      = "AccessMethod_ButMaster_StmtTypes_SelectWrong";
	basar::ConstString ACCESSMETHOD_ButVar_StmtTypes_ExecuteWrong                        = "AccessMethod_ButVar_StmtTypes_ExecuteWrong";
	basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_AggregWrong                      = "AccessMethod_ButMaster_StmtTypes_AggregWrong";

	basar::ConstString ACCESSMETHOD_ButProcs_ExecProc1                                   = "AccessMethod_ButProcs_ExecProc1";
    basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc2_WithVariable                      = "AccessMethod_ButProcs_ExecFunc2_WithVariable";
	basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc2                                   = "AccessMethod_ButProcs_ExecFunc2";
	basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc3                                   = "AccessMethod_ButProcs_ExecFunc3";

	basar::ConstString ACCESSMETHOD_ButTables_FixedStrings_Select_a                      = "AccessMethod_ButTables_FixedStrings_Select_a";

    basar::ConstString ACCESSMETHOD_TestDelete_DeleteRow                                 = "AccessMethod_ButTableVar_TestDelete_DeleteRow";
    basar::ConstString ACCESSMETHOD_TestDelete_InsertRow                                 = "AccessMethod_ButTableVar_TestDelete_InsertRow";
    basar::ConstString ACCESSMETHOD_TestDelete_SelectRow                                 = "AccessMethod_ButTableVar_TestDelete_SelectRow";
    
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere           = "AccessMethod_TestSqlBuilder_IntAndOneString_SelectWhere";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2         = "AccessMethod_TestSqlBuilder_IntAndOneString_SelectWhere_2";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3         = "AccessMethod_TestSqlBuilder_IntAndOneString_SelectWhere_3";
 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere           = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_SelectWhere";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2         = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3         = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert                = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_Insert";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly        = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_OneInsert             = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_OneInsert";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert_2              = "AccessMethod_TestSqlBuilder_IntAndTwoStrings_Insert_2";

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly                                                 = "AccessMethod_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters                                     = "AccessMethod_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters";
 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere                                           = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere                                  = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere                               = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere"; 

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert                                     = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert                            = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert                                   = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert                          = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert                       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert";   
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Insert                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Insert             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Insert                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Insert             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert"; 

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update                                     = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update                            = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update                                   = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update                          = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update                       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update";   
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Update                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Update             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Update                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Update             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update"; 

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete                                     = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete                            = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete                                   = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete                          = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete";
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete                       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete";   
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Delete                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Delete             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Delete                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Delete             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete"; 

                
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Insert                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Insert             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Insert                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Insert             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert"; 
              
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Update                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Update             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Update                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Update             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update"; 

    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete                         = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Delete                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Delete             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Delete                = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete       = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Delete             = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete    = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete"; 
    basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete = "AccessMethod_TestSqlBuilder_EnsureParameters_AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete"; 

    basar::ConstString ACCESSMETHOD_OneSqlbuilder_1  = "Accessmethod_oneSqlbuilder_1"; 
    basar::ConstString ACCESSMETHOD_OneSqlbuilder_2  = "Accessmethod_oneSqlbuilder_2"; 
    basar::ConstString ACCESSMETHOD_OneSqlbuilder_3  = "Accessmethod_oneSqlbuilder_3"; 
    basar::ConstString ACCESSMETHOD_MoreSqlbuilder_1 = "Accessmethod_moreSqlbuilder_1"; 

	basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectByString         = "AccessMethod_ButTables_I18n_SelectByString";
	basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectByInt            = "AccessMethod_ButTables_I18n_SelectByInt";
	basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectLow              = "AccessMethod_ButTables_I18n_SelectLow";
	basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectFormat           = "AccessMethod_ButTables_I18n_SelectFormat";

	basar::ConstString ACCESSMETHOD_ButTables_TimeSpan_Select			  = "AccessMethod_ButTables_TimeSpan_Select";
	basar::ConstString ACCESSMETHOD_ButTables_TimeSpan_Update			  = "AccessMethod_ButTables_TimeSpan_Update";
	
    basar::ConstString ACCESSMETHOD_PreparedQueryBasicTest                              = "AccessMethod_query_basic";
    basar::ConstString ACCESSMETHOD_PreparedQueryWrong                                  = "AccessMethod_query_wrong";
    basar::ConstString ACCESSMETHOD_PreparedQueryNoVariable                             = "AccessMethod_query_no_variable";
    basar::ConstString ACCESSMETHOD_PreparedSingleQueryWithVariable                     = "AccessMethod_single_query_with_variable";
    basar::ConstString ACCESSMETHOD_PreparedAggregateQueryWithVariable                  = "AccessMethod_aggregate_query_with_variable";
    basar::ConstString ACCESSMETHOD_PreparedWriteBasicTest                              = "AccessMethod_write_basic";
    basar::ConstString ACCESSMETHOD_PreparedQueryButMasterTableReader_All               = "AccessMethod_query_buttable";
    basar::ConstString ACCESSMETHOD_PreparedQueryButMasterTableReader_All_WithVaraible  = "AccessMethod_query_buttable_variable";
    basar::ConstString ACCESSMETHOD_PreparedWriteButVarTableWriter_All_WithVariables    = "AccessMethod_write_butvar_variables";
    basar::ConstString ACCESSMETHOD_PreparedWriteButVarTableReader_All_WithVariables    = "AccessMethod_read_written_butvar_variables";


 /*
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
        
#ifndef  __BASAR_TEST_UNIT_DBASPECT_ACCESSMETHODNAMES_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_ACCESSMETHODNAMES_H__ 

    extern basar::ConstString ACCESSMETHOD_ButMasterTableReader_Int;
    extern basar::ConstString ACCESSMETHOD_ButMasterTableReader_All                 ;
    extern basar::ConstString ACCESSMETHOD_ButMasterTableReader_All_no_results      ;
    extern basar::ConstString ACCESSMETHOD_ButMasterTableReader_All_one_result      ;
    extern basar::ConstString ACCESSMETHOD_ButMasterTableReader_SomeValues          ;

    extern basar::ConstString ACCESSMETHOD_ButTable_Join_byColser_Int               ;
    
    extern basar::ConstString ACCESSMETHOD_DateTimeUpdateDate                       ;
    extern basar::ConstString ACCESSMETHOD_DateTimeSelect                           ;
    extern basar::ConstString ACCESSMETHOD_DateTimeSelect2Rows                      ;
    
    extern basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertOne               ;
    extern basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertList              ;
    extern basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertDuplicate         ;
    extern basar::ConstString ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere               ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_nodate        ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat       ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_likeTest1     ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_Date          ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_DateTime      ;

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_testEndsWithMaskedString  ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest1   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest2   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest3   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest4   ;
   
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Reader_Lean                       ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_a    ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_b    ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Select_c    ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_SelectCount ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_a    ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Update_b    ;
	extern basar::ConstString ACCESSMETHOD_CICS_VSAM_DKPAR_UT_Successive_Statements_Insert      ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_a             ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_b             ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_Successive_Statements_Select_c             ;
	extern basar::ConstString ACCESSMETHOD_ButVar_Successive_Statements_Update_a                ;
	extern basar::ConstString ACCESSMETHOD_ButVar_Successive_Statements_Update_b                ;

	extern basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_Select                           ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_SingleQuery                      ;
	extern basar::ConstString ACCESSMETHOD_ButVar_StmtTypes_Execute                             ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_Aggreg                           ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_SelectWrong                      ;
	extern basar::ConstString ACCESSMETHOD_ButVar_StmtTypes_ExecuteWrong                        ;
	extern basar::ConstString ACCESSMETHOD_ButMaster_StmtTypes_AggregWrong                      ;

	extern basar::ConstString ACCESSMETHOD_ButProcs_ExecProc1                                   ;
    extern basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc2_WithVariable;
	extern basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc2                                   ;
	extern basar::ConstString ACCESSMETHOD_ButProcs_ExecFunc3                                   ;

	extern basar::ConstString ACCESSMETHOD_ButTables_FixedStrings_Select_a                      ;

    extern basar::ConstString ACCESSMETHOD_TestDelete_DeleteRow                                 ;
    extern basar::ConstString ACCESSMETHOD_TestDelete_InsertRow                                 ;
    extern basar::ConstString ACCESSMETHOD_TestDelete_SelectRow                                 ;
    
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere           ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2         ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3         ;
 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere           ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2         ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3         ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert                ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly        ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_OneInsert             ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert_2              ;

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly                                                 ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters                                     ;
 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere                                           ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere                                  ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere                               ; 

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert                                     ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert                            ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert                                   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert                          ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert                       ;   
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Insert                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Insert             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Insert                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Insert             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert ; 

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update                                     ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update                            ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update                                   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update                          ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update                       ;   
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Update                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Update             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Update                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Update             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update ; 

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete                                     ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete                            ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete                                   ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete                          ;
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete                       ;   
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UsePropTab_Delete                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UsePropTab_Delete             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithBlanks_Delete                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UseStringWithoutBlanks_Delete             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete ; 

                
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Insert                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Insert             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Insert                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Insert             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert ; 
              
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Update                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Update             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Update                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Update             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update ; 

    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete                         ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UsePropTab_Delete                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UsePropTab_Delete             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithBlanks_Delete                ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete       ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UseStringWithoutBlanks_Delete             ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete    ; 
    extern basar::ConstString ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete ; 

    extern basar::ConstString ACCESSMETHOD_OneSqlbuilder_1  ; 
    extern basar::ConstString ACCESSMETHOD_OneSqlbuilder_2  ; 
    extern basar::ConstString ACCESSMETHOD_OneSqlbuilder_3  ; 
    extern basar::ConstString ACCESSMETHOD_MoreSqlbuilder_1 ; 

	extern basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectByString         ;
	extern basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectByInt            ;
	extern basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectLow              ;
	extern basar::ConstString ACCESSMETHOD_ButTables_I18n_SelectFormat           ;
    
    //timespan access method names
	extern basar::ConstString ACCESSMETHOD_ButTables_TimeSpan_Select			;
	extern basar::ConstString ACCESSMETHOD_ButTables_TimeSpan_Update			;
    
    //prepared statenebt access method names
    extern basar::ConstString ACCESSMETHOD_PreparedQueryBasicTest;
    extern basar::ConstString ACCESSMETHOD_PreparedQueryWrong;
    extern basar::ConstString ACCESSMETHOD_PreparedQueryNoVariable;
    extern basar::ConstString ACCESSMETHOD_PreparedSingleQueryWithVariable;
    extern basar::ConstString ACCESSMETHOD_PreparedAggregateQueryWithVariable;
    extern basar::ConstString ACCESSMETHOD_PreparedWriteBasicTest;
    extern basar::ConstString ACCESSMETHOD_PreparedQueryButMasterTableReader_All;
    extern basar::ConstString ACCESSMETHOD_PreparedQueryButMasterTableReader_All_WithVaraible;
    extern basar::ConstString ACCESSMETHOD_PreparedWriteButVarTableWriter_All_WithVariables;
    extern basar::ConstString ACCESSMETHOD_PreparedWriteButVarTableReader_All_WithVariables;

#endif

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

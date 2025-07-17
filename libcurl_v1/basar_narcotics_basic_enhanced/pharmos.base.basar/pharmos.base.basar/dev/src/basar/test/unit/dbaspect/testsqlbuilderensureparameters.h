 /** $Id$                                          
  *                                                  
  * @file                                             
  * @author Anke Klink                      
  * @date 2008                            
  */    
                     
#ifndef  __BASAR_TEST_UNIT_DBASPECT_TESTSQLBUILDERENSUREPARAMETERS_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_TESTSQLBUILDERENSUREPARAMETERS_H__ 

BEGIN_QUERY_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_Select_YIteratorIntOnly)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_Select_YIteratorAllEnsureParameters)
END_BUILDER_DECLARATION
                                
BEGIN_QUERY_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere)
END_BUILDER_DECLARATION

// Insert:
BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION


// Update

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION

// Delete

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION

// UnsetAndInStateTest tests 

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert)
END_BUILDER_DECLARATION

// update 2

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update)
END_BUILDER_DECLARATION

// delete 2

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete)
END_BUILDER_DECLARATION

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

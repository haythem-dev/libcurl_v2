 /** $Id$                                          
  *                                                  
  * @file testsqlresolvequeries.h        
  * @brief ...                                       
  * @author Anke Klink                      
  * @date 2007                            
  */    

                                         
#ifndef  ___BASAR_TEST_UNIT_DBASPECT_TESTSQLINSERTUPDATE_H__ 
#define  ___BASAR_TEST_UNIT_DBASPECT_TESTSQLINSERTUPDATE_H__ 

 
// update:

BEGIN_WRITE_BUILDER_DECLARATION(DateTimeUpdateDate)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(DateTimeSelect)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(DateTimeSelect2Rows)
END_BUILDER_DECLARATION

// insert:

BEGIN_WRITE_BUILDER_DECLARATION(TestInsertUpdateInsert1)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestInsertUpdateInsert2)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestInsertUpdateInsertValues)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(TestInsertUpdate_InsertDuplicate)
END_BUILDER_DECLARATION



#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

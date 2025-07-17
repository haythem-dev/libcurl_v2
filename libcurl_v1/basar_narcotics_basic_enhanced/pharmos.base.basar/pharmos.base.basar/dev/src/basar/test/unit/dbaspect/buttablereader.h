/*
* @file buttablereader.h                                                                      
* @author Anke Klink                      
* @date 2007                            
*/    
#include <libbasardbaspect_macros.h>

#ifndef  ___BASAR_TEST_UNIT_DBASPECT_BUTTABLEREADER_H__ 
#define  ___BASAR_TEST_UNIT_DBASPECT_BUTTABLEREADER_H__ 

 
BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMasterTableReader_Int)
END_BUILDER_DECLARATION


BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMasterTableReader_All)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMasterTableReader_All_no_results)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMasterTableReader_All_one_result)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButTable_Join_byColser_Int)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMasterTableReader_SomeValues)
END_BUILDER_DECLARATION

// -----------------------------------------------------------------------------------------------------------------

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_a)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_b)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_c)
END_BUILDER_DECLARATION

BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_Single)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderUpdate_ButVar_a)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderUpdate_ButVar_b)
END_BUILDER_DECLARATION


BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderSetIsolation)
END_BUILDER_DECLARATION

BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(SQLBuilderAggregQuery_ButMaster_a)
END_BUILDER_DECLARATION

// write for query !
BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_cWrong)
END_BUILDER_DECLARATION

// query for exec immediately !
BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderSetIsolationWrong)
END_BUILDER_DECLARATION

// write for single select
BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderAggregQuery_ButMaster_aWrong)
END_BUILDER_DECLARATION

// select for single timespan
BEGIN_QUERY_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_SelectTimeSpan)
END_BUILDER_DECLARATION

// select for single timespan
BEGIN_WRITE_BUILDER_DECLARATION(SQLBuilderQuery_ButMaster_UpdateTimeSpan)
END_BUILDER_DECLARATION

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

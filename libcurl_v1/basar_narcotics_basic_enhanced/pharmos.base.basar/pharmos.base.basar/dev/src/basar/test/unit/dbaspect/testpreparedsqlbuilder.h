#ifndef __BASAR_TEST_UNIT_DBASPECT_TESTPREPAREDSQLBUILDER_H__ 
#define __BASAR_TEST_UNIT_DBASPECT_TESTPREPAREDSQLBUILDER_H__ 

#include "libbasardbaspect.h"

//common ut helpers
#include "testconnection.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessmethodnames.h"
#include "accessornames.h"


BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedQuery_BasicTest)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedQuery_Wrong)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedQuery_NoVariable)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_SINGLE_QUERY_BUILDER_DECLARATION(SQLPreparedSingleQuery_WithVariable)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_AGGREGATE_QUERY_BUILDER_DECLARATION(SQLPreparedAggregateQuery_WithVariable)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_WRITE_BUILDER_DECLARATION(SQLPreparedWrite_BasicTest)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedQuery_ButMasterTableReader_All)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedQuery_ButMasterTableReader_All_WithVariable)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_WRITE_BUILDER_DECLARATION(SQLPreparedInsert_ButVarTableWriter_All_WithVariables)
END_BUILDER_DECLARATION

BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(SQLPreparedInsert_ButVarTableReaderAll_WithVariables)
END_BUILDER_DECLARATION


//fixtures
class FixtureQuerySqlResolvTest
{
    public:
	FixtureQuerySqlResolvTest() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryBasicTest, connRef)),				    
				   yit   (ai.getPropertyTable().insert(basar::FOR_CLEAN))
	{
        yit.setInt32("colser", 1234);
	};

	~FixtureQuerySqlResolvTest()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yit;
};

//------------------------------------------------------------------------------

class FixtureQuerySqlResolvTestWrong
{
    public:
    FixtureQuerySqlResolvTestWrong() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryWrong, connRef)),				    
				   yit   (ai.getPropertyTable().insert(basar::FOR_CLEAN))
	{
        yit.setInt32("colser", 1234);
	};

	~FixtureQuerySqlResolvTestWrong()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yit;
}; 

//------------------------------------------------------------------------------

class FixtureWriteSqlResolvTest
{
    public:
	FixtureWriteSqlResolvTest() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedWriteBasicTest, connRef)),				    
				   yit   (ai.getPropertyTable().insert(basar::FOR_CLEAN))
	{
        yit.setInt32("colser", 1234);
        yit.setInt32("colint", 4321);
	};

	~FixtureWriteSqlResolvTest()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yit;
};

//------------------------------------------------------------------------------

class FixturePreparedButtableRead
{
    public:
	FixturePreparedButtableRead() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryButMasterTableReader_All, connRef))
	{
	};

	~FixturePreparedButtableRead()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
};

//------------------------------------------------------------------------------

class FixturePreparedNoVariable
{
    public:
	FixturePreparedNoVariable() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryNoVariable, connRef))
	{
	};

	~FixturePreparedNoVariable()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
};

//------------------------------------------------------------------------------
class FixturePreparedSingleQueryWithVariable
{
    public:
	FixturePreparedSingleQueryWithVariable() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedSingleQueryWithVariable, connRef)),
				   yitSearch   (ai.getPropertyTable().insert(basar::FOR_CLEAN))
	{
	    yitSearch.setInt32("colser", 2);
	    
	    yitSearch = yitSearch.cloneToUnboundedPropertyRow();
	};

	~FixturePreparedSingleQueryWithVariable()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yitSearch; 
};

//------------------------------------------------------------------------------

class FixturePreparedButtableReadWithVariable
{
    public:
	FixturePreparedButtableReadWithVariable() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai   (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryButMasterTableReader_All_WithVaraible, connRef)),
				   yitSearch   (ai.getPropertyTable().insert(basar::FOR_CLEAN))
	{
	    yitSearch.setInt32("colser", 2);
	};

	~FixturePreparedButtableReadWithVariable()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yitSearch;
};

//------------------------------------------------------------------------------

class FixturePreparedRoundtrip
{
    public:
	FixturePreparedRoundtrip() : connRef   ( basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef() ),
				   ai               (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai", ACCESSORNAME_PreparedQueryButMasterTableReader_All_WithVaraible, connRef)),
				   ai_write         (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai_write", ACCESSORNAME_PreparedWriteButVarTableWriter_All_WithVariables, connRef)),
				   ai_read_written  (basar::db::aspect::Manager::getInstance().createAccessorInstance("ai_read_written", ACCESSORNAME_PreparedWriteButVarTableReader_All_WithVariables, connRef)),
				   yitSearch        (ai.getPropertyTable().insert(basar::FOR_CLEAN)),
				   yitInsert        (ai_write.getPropertyTable().insert(basar::FOR_INSERT))
	{
	    yitSearch.setInt32("colser", 2);
	};

	~FixturePreparedRoundtrip()
	{
	}; 
	
	basar::db::aspect::ConnectionRef connRef;
    basar::db::aspect::AccessorInstanceRef ai;
    basar::db::aspect::AccessorInstanceRef ai_write;
    basar::db::aspect::AccessorInstanceRef ai_read_written;
    
    basar::db::aspect::AccessorPropertyTable_YIterator yitSearch;
    basar::db::aspect::AccessorPropertyTable_YIterator yitInsert;    
};

#endif // __BASAR_TEST_UNIT_DBASPECT_TESTPREPAREDSQLBUILDER_H__ 

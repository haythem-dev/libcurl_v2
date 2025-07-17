//----------------------------------------------------------------------------
/** $Id$
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql_exceptions.h"
#include "libbasarproperty_propertydescriptionlistref.h"

#include "testconnection.h"

//------------------------------------------------------------------------------
using basar::ConstString;

using basar::db::sql::DbException;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::DatabaseHintEnum;
using basar::db::aspect::NO_HINT;
using basar::db::aspect::HOLD_RECORDSET;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
ConstString ExceptAccInstance                   = "ExceptAccessorInstance";
ConstString ExceptAccessors                     = "ExceptAccessors";

ConstString ExceptAccInstanceCics               = "ExceptAccessorInstanceCics";
ConstString ExceptAccessorsCics                 = "ExceptAccessorsCics";


ConstString ExceptMethod_Query                  = "ExceptMethod_Query";
ConstString ExceptMethod_QuerySyntaxWrong       = "ExceptMethod_QuerySyntaxWrong";

ConstString ExceptMethod_SingleQuery            = "ExceptMethod_SingleQuery";
ConstString ExceptMethod_SingleQuerySyntaxWrong = "ExceptMethod_SingleQuerySyntaxWrong";

ConstString ExceptMethod_AggregQuery            = "ExceptMethod_AggregQuery";
ConstString ExceptMethod_AggregQuerySyntaxWrong = "ExceptMethod_AggregQuerySyntaxWrong";

ConstString ExceptMethod_Write                  = "ExceptMethod_Write";
ConstString ExceptMethod_WriteSyntaxWrong       = "ExceptMethod_WriteSyntaxWrong";


ConstString ExceptMethod_QueryCics                  = "ExceptMethod_QueryCics";
ConstString ExceptMethod_QuerySyntaxWrongCics       = "ExceptMethod_QuerySyntaxWrongCics";

ConstString ExceptMethod_SingleQueryCics            = "ExceptMethod_SingleQuery";
ConstString ExceptMethod_SingleQuerySyntaxWrongCics = "ExceptMethod_SingleQuerySyntaxWrongCics";

ConstString ExceptMethod_AggregQueryCics            = "ExceptMethod_AggregQueryCics";
ConstString ExceptMethod_AggregQuerySyntaxWrongCics = "ExceptMethod_AggregQuerySyntaxWrongCics";

ConstString ExceptMethod_WriteCics                  = "ExceptMethod_WriteCics";
ConstString ExceptMethod_WriteSyntaxWrongCics       = "ExceptMethod_WriteSyntaxWrongCics";

//------------------------------------------------------------------------------
BEGIN_QUERY_BUILDER_DECLARATION(ExceptBuild_Query)
END_BUILDER_DECLARATION
BEGIN_QUERY_BUILDER_DECLARATION(ExceptBuild_QuerySyntaxWrong)
END_BUILDER_DECLARATION

BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(ExceptBuild_SingleQuery)
END_BUILDER_DECLARATION
BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(ExceptBuild_SingleQuerySyntaxWrong)
END_BUILDER_DECLARATION

BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(ExceptBuild_AggregQuery)
END_BUILDER_DECLARATION
BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(ExceptBuild_AggregQuerySyntaxWrong)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(ExceptBuild_Write)
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION(ExceptBuild_WriteSyntaxWrong)
END_BUILDER_DECLARATION


BEGIN_QUERY_BUILDER_DECLARATION(ExceptBuild_QueryCics)
END_BUILDER_DECLARATION
BEGIN_QUERY_BUILDER_DECLARATION(ExceptBuild_QuerySyntaxWrongCics)
END_BUILDER_DECLARATION

BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(ExceptBuild_SingleQueryCics)
END_BUILDER_DECLARATION
BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(ExceptBuild_SingleQuerySyntaxWrongCics)
END_BUILDER_DECLARATION

BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(ExceptBuild_AggregQueryCics)
END_BUILDER_DECLARATION
BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(ExceptBuild_AggregQuerySyntaxWrongCics)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(ExceptBuild_WriteCics)
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION(ExceptBuild_WriteSyntaxWrongCics)
END_BUILDER_DECLARATION

//------------------------------------------------------------------------------
DONT_CHECK_PROPERTIES(ExceptBuild_Query)
BUILDER_DEFINITION(ExceptBuild_Query)
SQL_PATTERN(ExceptBuild_Query, "SELECT colser FROM but_master;")
DONT_CHECK_PROPERTIES(ExceptBuild_QuerySyntaxWrong)
BUILDER_DEFINITION(ExceptBuild_QuerySyntaxWrong)
SQL_PATTERN(ExceptBuild_QuerySyntaxWrong, "SELECT xyz FROM but_master;")

DONT_CHECK_PROPERTIES(ExceptBuild_SingleQuery)
BUILDER_DEFINITION(ExceptBuild_SingleQuery)
SQL_PATTERN(ExceptBuild_SingleQuery, "SELECT colser FROM but_master WHERE colser = 0;")
DONT_CHECK_PROPERTIES(ExceptBuild_SingleQuerySyntaxWrong)
BUILDER_DEFINITION(ExceptBuild_SingleQuerySyntaxWrong)
SQL_PATTERN(ExceptBuild_SingleQuerySyntaxWrong, "SELECT xyz FROM but_master WHERE colser = 0;")

DONT_CHECK_PROPERTIES(ExceptBuild_AggregQuery)
BUILDER_DEFINITION(ExceptBuild_AggregQuery)
SQL_PATTERN(ExceptBuild_AggregQuery, "SELECT min(colser) FROM but_master;")
DONT_CHECK_PROPERTIES(ExceptBuild_AggregQuerySyntaxWrong)
BUILDER_DEFINITION(ExceptBuild_AggregQuerySyntaxWrong)
SQL_PATTERN(ExceptBuild_AggregQuerySyntaxWrong, "SELECT min(xyz) FROM but_master;")

DONT_CHECK_PROPERTIES(ExceptBuild_Write)
BUILDER_DEFINITION(ExceptBuild_Write)
SQL_PATTERN(ExceptBuild_Write, "update but_master set colint = 3 where colsmint < 0;")
DONT_CHECK_PROPERTIES(ExceptBuild_WriteSyntaxWrong)
BUILDER_DEFINITION(ExceptBuild_WriteSyntaxWrong)
SQL_PATTERN(ExceptBuild_WriteSyntaxWrong, "update but_master set xyz = 3 where colser = 5;")


DONT_CHECK_PROPERTIES(ExceptBuild_QueryCics)
BUILDER_DEFINITION(ExceptBuild_QueryCics)
SQL_PATTERN(ExceptBuild_QueryCics, 
			"SELECT par_k_fil FROM VSAM.DKPAR_UT "
			"WHERE par_k_fil  = '01';")
DONT_CHECK_PROPERTIES(ExceptBuild_QuerySyntaxWrongCics)
BUILDER_DEFINITION(ExceptBuild_QuerySyntaxWrongCics)
SQL_PATTERN(ExceptBuild_QuerySyntaxWrongCics, 
			"SELECT par_k_fil FROM VSAM.DKPAR_UT_X "
			"WHERE par_k_fil  = '01';")

DONT_CHECK_PROPERTIES(ExceptBuild_SingleQueryCics)
BUILDER_DEFINITION(ExceptBuild_SingleQueryCics)
SQL_PATTERN(ExceptBuild_SingleQueryCics, 
			"SELECT par_k_fil FROM VSAM.DKPAR_UT "
			"WHERE par_k_fil  = '01' "
			"  AND par_k_name = 'PA29' "
		    "  AND par_k_nr   = '01';")
DONT_CHECK_PROPERTIES(ExceptBuild_SingleQuerySyntaxWrongCics)
BUILDER_DEFINITION(ExceptBuild_SingleQuerySyntaxWrongCics)
SQL_PATTERN(ExceptBuild_SingleQuerySyntaxWrongCics, 
			"SELECT par_k_fil FROM VSAM.DKPAR_UT_X "
			"WHERE par_k_fil  = '01' "
			"  AND par_k_name = 'PA29' "
		    "  AND par_k_nr   = '01';")

DONT_CHECK_PROPERTIES(ExceptBuild_AggregQueryCics)
BUILDER_DEFINITION(ExceptBuild_AggregQueryCics)
SQL_PATTERN(ExceptBuild_AggregQueryCics, 
			"SELECT count(*) FROM VSAM.DKPAR_UT "
			"WHERE par_k_fil  = '01';")
DONT_CHECK_PROPERTIES(ExceptBuild_AggregQuerySyntaxWrongCics)
BUILDER_DEFINITION(ExceptBuild_AggregQuerySyntaxWrongCics)
SQL_PATTERN(ExceptBuild_AggregQuerySyntaxWrongCics, 
			"SELECT count(*) FROM VSAM.DKPAR_UT_X "
			"WHERE par_k_fil  = '01';")

DONT_CHECK_PROPERTIES(ExceptBuild_WriteCics)
BUILDER_DEFINITION(ExceptBuild_WriteCics)
SQL_PATTERN(ExceptBuild_WriteCics, 
				"UPDATE VSAM.DKPAR_UT "
				"SET dummy = 'BasarUnitTest (successive test a)' "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = 'PA29' "
				"AND   par_k_nr   = '01';")
DONT_CHECK_PROPERTIES(ExceptBuild_WriteSyntaxWrongCics)
BUILDER_DEFINITION(ExceptBuild_WriteSyntaxWrongCics)
SQL_PATTERN(ExceptBuild_WriteSyntaxWrongCics, 
				"UPDATE VSAM.DKPAR_UT_X "
				"SET par_k_fil = 'BasarUnitTest (successive test a)' "
				"WHERE par_k_fil  = '01' "
				"AND   par_k_name = 'PA29' "
				"AND   par_k_nr   = '01';")

//------------------------------------------------------------------------------
namespace except_accessors
{
	BEGIN_ACCESSOR_DEFINITION(ExceptAccessors)

		PROPERTY_DESCRIPTION_LIST("int32 colser;");

		SQL_BUILDER_CREATE(ExceptBuild_Query)
		ACCESS_METHOD(ExceptMethod_Query)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_Query)
		SQL_BUILDER_CREATE(ExceptBuild_QuerySyntaxWrong)
		ACCESS_METHOD(ExceptMethod_QuerySyntaxWrong)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_QuerySyntaxWrong)

		SQL_BUILDER_CREATE(ExceptBuild_SingleQuery)
		ACCESS_METHOD(ExceptMethod_SingleQuery)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_SingleQuery)
		SQL_BUILDER_CREATE(ExceptBuild_SingleQuerySyntaxWrong)
		ACCESS_METHOD(ExceptMethod_SingleQuerySyntaxWrong)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_SingleQuerySyntaxWrong)

		SQL_BUILDER_CREATE(ExceptBuild_AggregQuery)
		ACCESS_METHOD(ExceptMethod_AggregQuery)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_AggregQuery)
		SQL_BUILDER_CREATE(ExceptBuild_AggregQuerySyntaxWrong)
		ACCESS_METHOD(ExceptMethod_AggregQuerySyntaxWrong)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_AggregQuerySyntaxWrong)

		SQL_BUILDER_CREATE(ExceptBuild_Write)
		ACCESS_METHOD(ExceptMethod_Write)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_Write)
		SQL_BUILDER_CREATE(ExceptBuild_WriteSyntaxWrong)
		ACCESS_METHOD(ExceptMethod_WriteSyntaxWrong)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_WriteSyntaxWrong)

	END_ACCESSOR_DEFINITION
}

namespace except_accessors_cics
{

	BEGIN_ACCESSOR_DEFINITION(ExceptAccessorsCics)

		PROPERTY_DESCRIPTION_LIST("string par_k_fil;")

		SQL_BUILDER_CREATE(ExceptBuild_QueryCics)
		ACCESS_METHOD(ExceptMethod_QueryCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_QueryCics)
		SQL_BUILDER_CREATE(ExceptBuild_QuerySyntaxWrongCics)
		ACCESS_METHOD(ExceptMethod_QuerySyntaxWrongCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_QuerySyntaxWrongCics)

		SQL_BUILDER_CREATE(ExceptBuild_SingleQueryCics)
		ACCESS_METHOD(ExceptMethod_SingleQueryCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_SingleQueryCics)
		SQL_BUILDER_CREATE(ExceptBuild_SingleQuerySyntaxWrongCics)
		ACCESS_METHOD(ExceptMethod_SingleQuerySyntaxWrongCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_SingleQuerySyntaxWrongCics)

		SQL_BUILDER_CREATE(ExceptBuild_AggregQueryCics)
		ACCESS_METHOD(ExceptMethod_AggregQueryCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_AggregQueryCics)
		SQL_BUILDER_CREATE(ExceptBuild_AggregQuerySyntaxWrongCics)
		ACCESS_METHOD(ExceptMethod_AggregQuerySyntaxWrongCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_AggregQuerySyntaxWrongCics)

		SQL_BUILDER_CREATE(ExceptBuild_WriteCics)
		ACCESS_METHOD(ExceptMethod_WriteCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_WriteCics)
		SQL_BUILDER_CREATE(ExceptBuild_WriteSyntaxWrongCics)
		ACCESS_METHOD(ExceptMethod_WriteSyntaxWrongCics)
		SQL_BUILDER_PUSH_BACK(ExceptBuild_WriteSyntaxWrongCics)

	END_ACCESSOR_DEFINITION
}

//------------------------------------------------------------------------------
SUITE(TestException)
{

//------------------------------------------------------------------------------
struct AccessorFix
{
	enum ExceptEnum
	{
		NOTHROW,
		THROW
	};

	bool exec(TestConnection::TestConnectionType type,
		      ConstString                        method, 
		      ExceptEnum                         except,
			  DatabaseHintEnum                   hint = NO_HINT)
	{
		const bool clearBeforeSelect = true;

		ConnectionRef       connect = TestConnection::createTestConnection(type);

		ConstString inst = (type == TestConnection::CICS) ? ExceptAccInstanceCics
			                                              : ExceptAccInstance;
		ConstString acc  = (type == TestConnection::CICS) ? ExceptAccessorsCics
			                                              : ExceptAccessors;

		AccessorInstanceRef	accInst = 
			Manager::getInstance().createAccessorInstance(inst,
			                                              acc,
														  connect,
														  ON_DEMAND_CACHING);
		ExecuteResultInfo   ret     = accInst.execute(method, 
													  clearBeforeSelect,
										              except == THROW,
													  hint);
        return !ret.hasError();
	}
};

//------------------------------------------------------------------------------
TEST_FIXTURE(AccessorFix, SqlGood)
{
	ConstString query  = ExceptMethod_Query;
	ConstString single = ExceptMethod_SingleQuery;
	ConstString aggreg = ExceptMethod_AggregQuery;
	ConstString write  = ExceptMethod_Write;
	ExceptEnum  except = THROW;

	try
	{
		TestConnection::TestConnectionType type = TestConnection::INFX;

		CHECK(exec(type, query , except));
		CHECK(exec(type, query , except, HOLD_RECORDSET));
		CHECK(exec(type, single, except));
		CHECK(exec(type, aggreg, except));
		CHECK(exec(type, write , except));

#ifdef HAS_ODBC

		type = TestConnection::ODBC;

		CHECK(exec(type, query , except));
		CHECK(exec(type, query , except, HOLD_RECORDSET));
		CHECK(exec(type, single, except));
		CHECK(exec(type, aggreg, except));
		CHECK(exec(type, write , except));

#endif

#ifdef HAS_CICS

		type = TestConnection::CICS;

		CHECK(exec(type, ExceptMethod_QueryCics      , except));
		CHECK(exec(type, ExceptMethod_SingleQueryCics, except));
		CHECK(exec(type, ExceptMethod_AggregQueryCics, except));
		CHECK(exec(type, ExceptMethod_WriteCics      , except));

#endif
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(AccessorFix, SqlBad_Except)
{
	ConstString query  = ExceptMethod_QuerySyntaxWrong;
	ConstString single = ExceptMethod_SingleQuerySyntaxWrong;
	ConstString aggreg = ExceptMethod_AggregQuerySyntaxWrong;
	ConstString write  = ExceptMethod_WriteSyntaxWrong;
	ExceptEnum  except = THROW;

	try
	{
		TestConnection::TestConnectionType type = TestConnection::INFX;

		CHECK_THROW(exec(type, query , except),                 DbException);
		CHECK_THROW(exec(type, query , except, HOLD_RECORDSET), DbException);
		CHECK_THROW(exec(type, single, except),                 DbException);
		CHECK_THROW(exec(type, aggreg, except),                 DbException);
		CHECK_THROW(exec(type, write , except),                 DbException);

#ifdef HAS_ODBC
		type = TestConnection::ODBC;

		CHECK_THROW(exec(type, query , except),                 DbException);
		CHECK_THROW(exec(type, query , except, HOLD_RECORDSET), DbException);
		CHECK_THROW(exec(type, single, except),                 DbException);
		CHECK_THROW(exec(type, aggreg, except),                 DbException);
		CHECK_THROW(exec(type, write , except),                 DbException);
#endif
#ifdef HAS_CICS

		type = TestConnection::CICS;

		CHECK_THROW(exec(type, ExceptMethod_QuerySyntaxWrongCics      , except), DbException);
		CHECK_THROW(exec(type, ExceptMethod_SingleQuerySyntaxWrongCics, except), DbException);
		CHECK_THROW(exec(type, ExceptMethod_AggregQuerySyntaxWrongCics, except), DbException);
		CHECK_THROW(exec(type, ExceptMethod_WriteSyntaxWrongCics      , except), DbException);

#endif
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(AccessorFix, SqlBad_ErrorCode)
{
	ConstString query  = ExceptMethod_QuerySyntaxWrong;
	ConstString single = ExceptMethod_SingleQuerySyntaxWrong;
	ConstString aggreg = ExceptMethod_AggregQuerySyntaxWrong;
	ConstString write  = ExceptMethod_WriteSyntaxWrong;

	ExceptEnum  except = NOTHROW;

	try
	{
		TestConnection::TestConnectionType type = TestConnection::INFX;

		CHECK( !exec(type, query , except));
		CHECK( !exec(type, query , except, HOLD_RECORDSET));
		CHECK( !exec(type, single, except));
		CHECK( !exec(type, aggreg, except));
		CHECK( !exec(type, write , except));

#ifdef HAS_ODBC
		type = TestConnection::ODBC;

		CHECK( !exec(type, query , except));
		CHECK( !exec(type, query , except, HOLD_RECORDSET));
		CHECK( !exec(type, single, except));
		CHECK( !exec(type, aggreg, except));
		CHECK( !exec(type, write , except));
#endif
#ifdef HAS_CICS

		type = TestConnection::CICS;

        CHECK( !exec(type, ExceptMethod_QuerySyntaxWrongCics      , except));
		CHECK( !exec(type, ExceptMethod_SingleQuerySyntaxWrongCics, except));
		CHECK( !exec(type, ExceptMethod_AggregQuerySyntaxWrongCics, except));

		CHECK( !exec(type, ExceptMethod_WriteSyntaxWrongCics      , except));

#endif
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
}	// SUITE

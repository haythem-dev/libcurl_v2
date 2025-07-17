#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"

// -----------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
SUITE(ResultsetClose)
{

// -----------------------------------------------------------------------------
#define SELCOL	"colint" 

class Fix
{
public:
	Fix() : 
	  sqlprepmulti ("select " SELCOL " from but_master where colsmint <=  ? order by colint;"),
	  sqlprepsingle("select " SELCOL " from but_master where colsmint  =  ?;"),
	  sqlmulti     ("select " SELCOL " from but_master where colsmint <= %d order by colint;"),
	  sqlsingle    ("select " SELCOL " from but_master where colsmint  = %d;"),
	  conns (TestConnection::createDefaultTestConnections()),
      itConn(conns.begin()),
	  itEnd (conns.end  ())
	{
		filter[0] = 2000;
		filter[1] = 2005;
	}

	ConstString sqlprepmulti;
	ConstString sqlprepsingle;
	ConstString sqlmulti;
	ConstString sqlsingle;

	TestConnection::TestConnectionMap                 conns;
    TestConnection::TestConnectionMap::      iterator itConn;
    TestConnection::TestConnectionMap::const_iterator itEnd;

	Int16     filter[2];
	VarString sql;

private:
	Fix & operator =(const Fix &);
};

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetMulti_dtor)
{
	for (; itConn != itEnd; ++itConn)
	{
 		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepmulti);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeQuery();
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetMulti_reset)
{
	for (; itConn != itEnd; ++itConn)
	{
		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepmulti);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeQuery();
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.reset();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetMulti_close)
{
	for (; itConn != itEnd; ++itConn)
	{
		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepmulti);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeQuery();
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.close();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetSingle_dtor)
{
	for (; itConn != itEnd; ++itConn)
	{
		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepsingle);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeSingleQuery();
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetSingle_reset)
{
	for (; itConn != itEnd; ++itConn)
	{
		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepsingle);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeSingleQuery();
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.reset();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, PrepStmtReused_RessetSingle_close)
{
	for (; itConn != itEnd; ++itConn)
	{
		PreparedStatementRef stmt = itConn->second.createPreparedStatement(sqlprepsingle);

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			stmt.setInt16(0, filter[i]);

			ResultsetRef res = stmt.executeSingleQuery();
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.close();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_RessetMulti_dtor)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlmulti, filter[i]);
			ResultsetRef res = stmt.executeQuery(sql);
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_RessetMulti_reset)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlmulti, filter[i]);
			ResultsetRef res = stmt.executeQuery(sql);
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.reset();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_RessetMulti_close)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlmulti, filter[i]);
			ResultsetRef res = stmt.executeQuery(sql);
			CHECK(res.isOpen());

			int cnt = 1;
			while (res.next())
			{
				CHECK_EQUAL(cnt + 100000, res.getInt32(SELCOL));
				++cnt;
			}

			int expect = (i == 0) ? 0 : 5;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.close();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_RessetSingle_dtor)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlsingle, filter[i]);
			ResultsetRef res = stmt.executeSingleQuery(sql);
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_ResseSingle_reset)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlsingle, filter[i]);
			ResultsetRef res = stmt.executeSingleQuery(sql);
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.reset();
		}
	}
}

// -----------------------------------------------------------------------------
TEST_FIXTURE(Fix, StmtReused_RessetSingle_close)
{
	for (; itConn != itEnd; ++itConn)
	{
		StatementRef stmt = itConn->second.createStatement();

		for (size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); ++i)
		{
			sql.format(sqlsingle, filter[i]);
			ResultsetRef res = stmt.executeSingleQuery(sql);
			CHECK(res.isOpen());

			while (res.next())
			{
				CHECK_EQUAL(100005, res.getInt32(SELCOL));
			}

			int expect = (i == 0) ? 0 : 1;
			CHECK_EQUAL(expect, (int)res.getFetchedRows());

			res.close();
		}
	}
}

// -----------------------------------------------------------------------------
}	// SUITE

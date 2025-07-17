//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   20.02.2008
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "testconnection.h"
#include "odbcfix.h"

#include <string.h>
#include <iostream>

//------------------------------------------------------------------------------
SUITE(SuccStmts)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;
using basar::Int32;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectInfx_OneInstance)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		StatementRef      stmt;
		ResultsetRef      result;
		ExecuteReturnInfo ret;

		// --- step 1

		stmt   = connect.createStatement();
		result = stmt.executeQuery(sqlSel);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());

		// --- step 2

		ret = stmt.execute(sqlUpd);
		CHECK      (!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
	
	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectInfx_TwoInstances)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}

		// --- step 2
		{
			StatementRef      stmt;
			ExecuteReturnInfo ret;

			stmt = connect.createStatement();
			ret  = stmt.execute(sqlUpd);
			CHECK      (!ret.hasError());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectInfx_TwoInstances_good)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
	CHECK(connect.isOpen());

	try
	{
        StatementRef stmtOuterScope;

		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());

            stmtOuterScope = stmt;
		}

		// --- step 1a
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#ifdef HAS_ODBC

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, UpdateAfterSelectODBC_OneInstance)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		StatementRef      stmt;
		ResultsetRef      result;
		ExecuteReturnInfo ret;

		// --- step 1

		stmt   = connect.createStatement();
		result = stmt.executeQuery(sqlSel);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());

		// --- step 2

		ret = stmt.execute(sqlUpd);
		CHECK      (!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
	
	connect.rollback();
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, UpdateAfterSelectODBC_TwoInstances)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}

		// --- step 2
		{
			StatementRef      stmt;
			ExecuteReturnInfo ret;

			stmt = connect.createStatement();
			ret  = stmt.execute(sqlUpd);
			CHECK      (!ret.hasError());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST_FIXTURE(OdbcFix, UpdateAfterSelectODBC_TwoInstances_good)
{
	static ConstString sqlSel = 

		"SELECT FIRST 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
	CHECK(connect.isOpen());

	try
	{
        StatementRef stmtOuterScope;

		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());

            stmtOuterScope = stmt;
		}

		// --- step 1a
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
// MS SQL
//------------------------------------------------------------------------------

TEST(UpdateAfterSelectMSSQLODBC_OneInstance)
{
	static ConstString sqlSel = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		StatementRef      stmt;
		ResultsetRef      result;
		ExecuteReturnInfo ret;

		// --- step 1

		stmt   = connect.createStatement();
		result = stmt.executeQuery(sqlSel);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		Int16     smint;
		VarString ch2;
		VarString vch2;

		while (result.next())
		{
			smint = result.getInt16 ("colsmint");
			ch2   = result.getString("colch2"  );
			vch2  = result.getString("colvch2" );

			++cnt;
		}

		CHECK_EQUAL(   1     , cnt);
		CHECK_EQUAL(2001     , smint);
		CHECK_EQUAL("COLCH21", ch2 .c_str());
		CHECK_EQUAL("COLCH21", vch2.c_str());

		// --- step 2

		ret = stmt.execute(sqlUpd);
		CHECK      (!ret.hasError());
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
	
	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectMSSQLODBC_TwoInstances)
{
	static ConstString sqlSel = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	static ConstString sqlUpd = 

		"UPDATE but_var "
		"SET    colch2 = 'a2345', colvch2 = 'b2345' "
		"WHERE  colch2 = 'COLCH21';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());
	connect.begin();

	try
	{
		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}

		// --- step 2
		{
			StatementRef      stmt;
			ExecuteReturnInfo ret;

			stmt = connect.createStatement();
			ret  = stmt.execute(sqlUpd);
			CHECK      (!ret.hasError());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

	connect.rollback();
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectMSSQLODBC_TwoInstances_good)
{
	static ConstString sqlSel = 

		"SELECT TOP 1 "
			"colsmint, "
			"colch2, "
			"colvch2 "
		"FROM   but_master "
		"ORDER BY colint;";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
	CHECK(connect.isOpen());

	try
	{
        StatementRef stmtOuterScope;

		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());

            stmtOuterScope = stmt;
		}

		// --- step 1a
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			Int16     smint;
			VarString ch2;
			VarString vch2;

			while (result.next())
			{
				smint = result.getInt16 ("colsmint");
				ch2   = result.getString("colch2"  );
				vch2  = result.getString("colvch2" );

				++cnt;
			}

			CHECK_EQUAL(   1     , cnt);
			CHECK_EQUAL(2001     , smint);
			CHECK_EQUAL("COLCH21", ch2 .c_str());
			CHECK_EQUAL("COLCH21", vch2.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#endif	// HAS_ODBC

//------------------------------------------------------------------------------
#ifdef HAS_CICS

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectCICS_OneInstance)
{
	static ConstString sqlSel = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	static ConstString sqlUpd = 

		"UPDATE vsam.dkpar_ut "
		"SET    dummy = 'BasarUnitTest (update after select test: g2345)' "
		"WHERE  par_k_name = '01';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	connect.open();
	CHECK(connect.isOpen());

	try
	{
		StatementRef      stmt;
		ResultsetRef      result;
		ExecuteReturnInfo ret;

		// --- step 1

		stmt   = connect.createStatement();
		result = stmt.executeQuery(sqlSel);

		CHECK(stmt.isOpen  ());	// stmt must be executed first!
		CHECK(result.isOpen());

		Int32     cnt = 0;
		VarString fil;
		VarString name;
		VarString nr;

		while (result.next())
		{
			fil  = result.getString("par_k_fil");
			name = result.getString("par_k_name"  );
			nr   = result.getString("par_k_nr" );

			++cnt;
		}

		CHECK_EQUAL(   1, cnt);

		CHECK_EQUAL("01", fil .c_str());
		CHECK_EQUAL("01", name.c_str());
		CHECK_EQUAL("03", nr  .c_str());

		CHECK( 0 != ::strcmp("01 " , fil .c_str()));
		CHECK( 0 != ::strcmp("01  ", name.c_str()));
		CHECK( 0 != ::strcmp("03 " , nr  .c_str()));

		// --- step 2

		ret = stmt.execute(sqlUpd);
		CHECK      (!ret.hasError());
		CHECK_EQUAL(2, (int)ret.m_affectedRows);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectCICS_TwoInstances)
{
	static ConstString sqlSel = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	static ConstString sqlUpd = 

		"UPDATE vsam.dkpar_ut "
		"SET    dummy = 'BasarUnitTest (update + select test: g2345)' "
		"WHERE  par_k_name = '01';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	connect.open();
	CHECK(connect.isOpen());

	try
	{
		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			VarString fil;
			VarString name;
			VarString nr;

			while (result.next())
			{
				fil  = result.getString("par_k_fil");
				name = result.getString("par_k_name"  );
				nr   = result.getString("par_k_nr" );

				++cnt;
			}

			CHECK_EQUAL(   1, cnt);

			CHECK_EQUAL("01", fil .c_str());
			CHECK_EQUAL("01", name.c_str());
			CHECK_EQUAL("03", nr  .c_str());

			CHECK( 0 != ::strcmp("01 " , fil .c_str()));
			CHECK( 0 != ::strcmp("01  ", name.c_str()));
			CHECK( 0 != ::strcmp("03 " , nr  .c_str()));
		}

		// --- step 2
		{
			StatementRef      stmt;
			ExecuteReturnInfo ret;

			stmt = connect.createStatement();
			ret  = stmt.execute(sqlUpd);
			CHECK      (!ret.hasError());
			CHECK_EQUAL(2, (int)ret.m_affectedRows);
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST(UpdateAfterSelectCICS_TwoInstances_Critical)
{
	static ConstString sqlSel = 

		"SELECT "
			"par_k_fil, "
			"par_k_name, "
			"par_k_nr "
		"FROM  vsam.dkpar_ut "
		"WHERE par_k_fil  = '01' "
		"AND   par_k_name = '01' "
		"AND   par_k_nr   = '03';";

	ConnectionRef connect = TestConnection::createTestConnection(TestConnection::CICS);
	connect.open();
	CHECK(connect.isOpen());

	try
	{
        StatementRef stmtOuterScope;

		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			VarString fil;
			VarString name;
			VarString nr;

			while (result.next())
			{
				fil  = result.getString("par_k_fil");
				name = result.getString("par_k_name"  );
				nr   = result.getString("par_k_nr" );

				++cnt;
			}

			CHECK_EQUAL(   1, cnt);

			CHECK_EQUAL("01", fil .c_str());
			CHECK_EQUAL("01", name.c_str());
			CHECK_EQUAL("03", nr  .c_str());

			CHECK( 0 != ::strcmp("01 " , fil .c_str()));
			CHECK( 0 != ::strcmp("01  ", name.c_str()));
			CHECK( 0 != ::strcmp("03 " , nr  .c_str()));

            stmtOuterScope = stmt;
		}
        	// --- step 1 a 
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();

			stmtOuterScope.close();

			result = stmt.executeQuery(sqlSel);

			CHECK(stmt.isOpen  ());	// stmt must be executed first!
			CHECK(result.isOpen());

			Int32     cnt = 0;
			VarString fil;
			VarString name;
			VarString nr;

			while (result.next())
			{
				fil  = result.getString("par_k_fil");
				name = result.getString("par_k_name"  );
				nr   = result.getString("par_k_nr" );

				++cnt;
			}

			CHECK_EQUAL(   1, cnt);

			CHECK_EQUAL("01", fil .c_str());
			CHECK_EQUAL("01", name.c_str());
			CHECK_EQUAL("03", nr  .c_str());

			CHECK( 0 != ::strcmp("01 " , fil .c_str()));
			CHECK( 0 != ::strcmp("01  ", name.c_str()));
			CHECK( 0 != ::strcmp("03 " , nr  .c_str()));
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#endif	// HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

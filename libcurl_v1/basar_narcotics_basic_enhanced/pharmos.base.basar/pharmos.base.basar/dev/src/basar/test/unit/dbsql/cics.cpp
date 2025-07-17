#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasardbsql_manager.h"
#include "libbasarcmnutil.h"

#include "utdbsqlhelper.h"
#include "testconnection.h"

#include <iostream>

using basar::db::sql::Manager;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ConnectionRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::InvalidParameterException;
using basar::InvalidStateException;

// general:
using basar::ConstString;
using basar::UInt32;
using basar::Int32;
using basar::Float32;
using basar::Float64;
using basar::VarString;
using basar::Decimal;
using basar::Date;
using basar::Time;

// test:
using basar::test::unit::dbsql::TestConnection;

#define TESTDBSQLCICS
#ifdef TESTDBSQLCICS
#ifdef HAS_CICS

//------------------------------------------------------------------------------
extern bool        g_RUNSHORT;
static const Int32 numberOfRowsPAR_K_FIL01 = 3;

//------------------------------------------------------------------------------

SUITE(Cics)
{
	//------------------------------------------------------------------------------
	class Fixture
	{
	public:
		Fixture()  
        { 
            init(); 
        };
		~Fixture() {};

		bool init()
		{
			// random
			::sprintf(fx_randName, "%4.4x", fx_rd);

			// SQL statements
			fx_sqlins.format(	"INSERT INTO vsam.dkpar_ut "
								"(par_k_fil, par_k_name, par_k_nr, dummy, test_lint) "
								"VALUES "
								"('%s', '%s', '%s', '%s', %d);", 
								fx_FIL,
								fx_randName, 
								fx_NR,
								fx_CLIENT_STRING,
								fx_MARK);

			fx_sqlsel.format(	"SELECT test_lint, "
				                       "SUBSTR(dummy, 1, 12) as clientstring, "
									   "HEX(SUBSTR(dummy, 1, 12)) as serverstring "
								"FROM vsam.dkpar_ut "
								"WHERE par_k_fil  = '%s' "
								"  AND par_k_name = '%s' "
								"  AND par_k_nr   = '%s';",
								fx_FIL,
								fx_randName,
								fx_NR);

			fx_sqlupd.format(	"UPDATE vsam.dkpar_ut "
				                "SET dummy = 'dummy' "
								"WHERE par_k_fil  = '%s' "
								"  AND par_k_nr   = '%s';",
								fx_FIL,
								fx_NR);

			fx_sqldel.format(	"DELETE FROM vsam.dkpar_ut "
								"WHERE par_k_fil  = '%s' "
								"  AND par_k_nr   = '%s';",
								fx_FIL,
								fx_NR);


			return true;
		};

		VarString::value_type   fx_randName[8];	// marks dkpar_ut.par_k_name
		VarString               fx_sqlins;
		VarString               fx_sqlsel;
		VarString               fx_sqlupd;
		VarString               fx_sqldel;

        static const Int32              fx_rd;
		static const Int32				fx_MARK;
		static ConstString				fx_FIL;
		static ConstString				fx_NR;

		static ConstString				fx_CLIENT_STRING;
		static ConstString				fx_SERVER_STRING_EN_CICS;
		static ConstString				fx_SERVER_STRING_BG_CICS;
	};

    const Int32 Fixture::fx_rd   = ::rand() % 0x10000;	// 4 digits
	const Int32	Fixture::fx_MARK = 1234567;
	ConstString	Fixture::fx_FIL	 = "03";
	ConstString	Fixture::fx_NR	 = "81";

	// Win CP1252: "ABc ÄÖüß ÇÏ÷"
	ConstString	Fixture::fx_CLIENT_STRING         = "\x41\x42\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7";	

#ifdef _WIN32
	ConstString	Fixture::fx_SERVER_STRING_EN_CICS = "C1C283404AE0D0A1406877E1";
	ConstString	Fixture::fx_SERVER_STRING_BG_CICS = "C1C28340BCBBB0DD40FADCB6";
#else
	ConstString	Fixture::fx_SERVER_STRING_EN_CICS = "C1C283404AE0D0A1406877E1";
	ConstString	Fixture::fx_SERVER_STRING_BG_CICS = "C1C28340BEAE549F40FEDD48";
#endif

	// -------------------------------------------------------------------------------------------------
	TEST_FIXTURE(Fixture, ResultSet_test)
	{
		try
		{
			ConnectionRef connRef = TestConnection::createTestConnection( TestConnection::CICS );
			REQUIRE CHECK( !connRef.isOpen() );
			connRef.open();
			REQUIRE CHECK( connRef.isOpen() );

			StatementRef statement = connRef.createStatement();

			// clean up before
			ExecuteReturnInfo execResult = statement.execute(fx_sqldel);

			basar::VarString sqlins;
			sqlins.format( "INSERT INTO vsam.dkpar_ut "
						   "(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw, test_lint, test_sint, test_float1, test_float2) "
						   "VALUES ('%s', '%s', '%s', '%s', %s, %s, %d, %d, %f, %f );"
						   , fx_FIL
						   , fx_randName
						   , fx_NR
						   , fx_CLIENT_STRING
						   , "123456789"
						   , "98.7654321"
						   , Int32(0x71234567)
						   , basar::Int16(0x7654)
						   , 1234.567f
						   , 987654.321098
			);

			//std::cout << sqlins << std::endl;
			execResult = statement.execute( sqlins );
			CHECK_EQUAL( 1, (int)execResult.m_affectedRows );

			basar::VarString sqlsel;
			sqlsel.format( "SELECT "
				           "  SUBSTR(dummy, 1, 12) as clientstring, "
						   "  HEX(SUBSTR(dummy, 1, 12)) as serverstring, "
						   "  test_dec_scale_0, dkpar_euihw, test_lint, test_sint, test_float1, test_float2 "
						   "FROM vsam.dkpar_ut "
						   "WHERE par_k_fil  = '%s' "
						   "  AND par_k_name = '%s' "
						   "  AND par_k_nr   = '%s';"
						   , fx_FIL
						   , fx_randName
						   , fx_NR
			);

			//std::cout << sqlsel << std::endl;
			ResultsetRef res = statement.executeSingleQuery( sqlsel );

			Int32 cnt = 0;
			while( res.next() )
			{
				++cnt;

				CHECK_EQUAL(fx_CLIENT_STRING, res.getString( "clientstring" ));
				CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString( "serverstring" ));

				const basar::Decimal test_dec_scale_0( 123456789 );
				const basar::Decimal d1 = res.getDecimal( "test_dec_scale_0" );
				CHECK_EQUAL( test_dec_scale_0, d1 );

				const basar::Decimal dkpar_euihw( "98,7654321" );
				const basar::Decimal d2 = res.getDecimal( "dkpar_euihw" );
				CHECK_EQUAL( dkpar_euihw, d2 );

				CHECK_EQUAL( Int32( 0x71234567 ), res.getInt32( "test_lint" ));
				CHECK_EQUAL( basar::Int16( 0x7654 ), res.getInt16( "test_sint" ));

				// if we try to read a large integer we should get the range check exception
				CHECK_THROW(res.getInt16("test_lint"), InvalidParameterException);

				Float32 tf1 = res.getFloat32("test_float1");
				CHECK_CLOSE(1234.567f, tf1, 0.001);

				// it is only a 32bit float on host side!
				Float64 tf2 = res.getFloat64("test_float2");
				CHECK_CLOSE(987654.3, tf2, 0.1);
			}

			CHECK_EQUAL(1, cnt);

			// clean up entries
			execResult = statement.execute( fx_sqldel );
			CHECK( 1 <= execResult.m_affectedRows );
		}
		catch( const basar::Exception& ex )
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK( ExceptionOccured );
		}
		catch( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK( ExceptionOccured );
		}
	}

    //------------------------------------------------------------------------------

    TEST(invalid_state_exception_on_get_without_calling_next_cics)
    {
        ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
        REQUIRE CHECK(!connRef.isOpen());
        connRef.open();
        try
        {
            REQUIRE CHECK(connRef.isOpen());

            StatementRef stmt = connRef.createStatement();

            ConstString statementString = "select par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw, test_lint, test_sint, test_float1, test_float2, test_date, test_time from vsam.dkpar_ut where par_k_fil = '03' and par_k_nr = 81";

            ResultsetRef res = stmt.executeQuery(statementString);

            REQUIRE CHECK(!res.isNull());

            CHECK_THROW(res.getString("par_k_fil"), basar::InvalidStateException);
            CHECK_THROW(res.getString("test_dec_scale_0"), basar::InvalidStateException);
            CHECK_THROW(res.getInt32("test_lint"), basar::InvalidStateException);
            CHECK_THROW(res.getInt16("test_sint"), basar::InvalidStateException);
            CHECK_THROW(res.getFloat32("test_float1"), basar::InvalidStateException);
            CHECK_THROW(res.getDate("test_date"), basar::InvalidStateException);
            CHECK_THROW(res.getDateTime("test_time"), basar::InvalidStateException);

        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured_B = false;
            CHECK(ExceptionOccured_B);
        }
        connRef.close();
    }

    //------------------------------------------------------------------------------
    TEST(testdbsqlcics_test_cics_types)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            REQUIRE CHECK(!connRef.isOpen());
            connRef.open();
            try
            {
                REQUIRE CHECK(connRef.isOpen());

                const basar::UInt32 expectedNumberOfCols = 12;

                StatementRef stmt = connRef.createStatement();

                ConstString statementString = "select par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw, test_lint, test_sint, test_float1, test_float2, test_date, test_time from vsam.dkpar_ut where par_k_fil = '03' and par_k_nr = 81";

                ResultsetRef res = stmt.executeQuery( statementString );

                CHECK( !res.isNull() );

                // check column names
                {
                    const basar::db::sql::CollColNames& colNames = res.getColumnNames();

                    //get nice columnNames from statementstring
                    basar::VarString columnNamesRaw(statementString);
                    columnNamesRaw = columnNamesRaw.substr(7, 139);
                    basar::cmnutil::CollBString columnNames;
                    columnNamesRaw.tokenizeAndTrim(columnNames, ",", basar::cmnutil::BString::ALL);

                    for (basar::UInt32 i = 0; i < expectedNumberOfCols; ++i)
                    {
                        CHECK_EQUAL(columnNames.at(i), colNames.at(i));
                    }
                }

                //check column types
                {
                    std::vector<basar::SupportedTypeEnum> expectedTypes;

                    //expected types from cics
                    expectedTypes.push_back( basar::STRING );
                    expectedTypes.push_back( basar::STRING );
                    expectedTypes.push_back( basar::STRING );
                    expectedTypes.push_back( basar::STRING );
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
                    expectedTypes.push_back( basar::INT32 );     // since SP20
#else
                    expectedTypes.push_back( basar::DECIMAL ); // until SP18
#endif // !_WIN32
                    expectedTypes.push_back( basar::DECIMAL );
                    expectedTypes.push_back( basar::INT32 );
                    expectedTypes.push_back( basar::INT16 );
                    expectedTypes.push_back( basar::FLOAT32 );
                    expectedTypes.push_back( basar::FLOAT32 );
                    expectedTypes.push_back( basar::DATE );
                    expectedTypes.push_back( basar::DATETIME );

                    const basar::db::sql::CollColTypes& colTypes = res.getColumnTypes();

                    for ( basar::UInt32 i = 0; i < expectedNumberOfCols; ++i )
                    {
                        CHECK_EQUAL( expectedTypes.at( i ), colTypes.at( i ) );
                    }
                }
            }
            catch (basar::Exception & ex)
            {
                std::cout << ex.what() << std::endl;
                const bool ExceptionOccured_B = false;
                CHECK(ExceptionOccured_B);
            }
            connRef.close();
            CHECK(!connRef.isOpen());
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
        catch ( ... )
        {
            std::cout << "unknown exception" << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
    }

	//------------------------------------------------------------------------------
    TEST(testdbsqlcics_test_commit_rollback)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            REQUIRE CHECK(!connRef.isOpen());
            connRef.open();
            try
            {
                REQUIRE CHECK(connRef.isOpen());
                // cics implementation has no transactions only autocommit:
                CHECK_THROW(connRef.begin()   , basar::InvalidStateException);
                CHECK(!connRef.isInTransaction());
                CHECK_THROW(connRef.commit()  , basar::InvalidStateException);
                CHECK_THROW(connRef.rollback(), basar::InvalidStateException);
            }
            catch (basar::Exception & ex)
            {
                std::cout << ex.what() << std::endl;
                const bool ExceptionOccured_B = false;
                CHECK(ExceptionOccured_B);
            }
            connRef.close();
            CHECK(!connRef.isOpen());
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
        catch ( ... )
        {
            std::cout << "unknown exception" << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
    }

    // -------------------------------------------------------------------------------------------------
    TEST(testdbsqlcics_parallelconnects)
    {
		DatabaseInfo  dbInfo[8];

		for (size_t j = 0; j < sizeof(dbInfo)/sizeof(dbInfo[0]); ++j)
		{
			if (j < (sizeof(dbInfo)/sizeof(dbInfo[0]))/2)
				dbInfo[j].setCICS(CICS_CONFIG_TEST, 
								  "", "", "", 
								  CICS_USER, CICS_PWD);
			else
				dbInfo[j].setCICS(CICS_CONFIG_WEST, 
								  CICS_HOST, CICS_PORT, CICS_START, 
								  CICS_USER, CICS_PWD);
		}

		ConnectionRef conn[sizeof(dbInfo)/sizeof(dbInfo[0])];

        size_t i = 0;

		try
		{
			for (i = 0; i < sizeof(conn)/sizeof(conn[0]); ++i)
			{
				conn[i] = Manager::getInstance().createConnect(dbInfo[i]);
			}

			for (i = 0; i < sizeof(conn)/sizeof(conn[0]); ++i)
			{
				conn[i].close();
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << ". i: " << (unsigned int)i << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch ( ... )
        {
            std::cout << "unknown exception. i: " << (unsigned int)i << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
	}

    // -------------------------------------------------------------------------------------------------
	TEST_FIXTURE(Fixture, ConnectByName_DefaultWest)
	{
		DatabaseInfo  dbInfo;
		dbInfo.setCICS(CICS_CONFIG_WEST, CICS_HOST, CICS_PORT, CICS_START, CICS_USER, CICS_PWD);

		try
		{
			ConnectionRef conn = Manager::getInstance().createConnect(dbInfo);
			CHECK      (0               <= conn.getDbInfo().connectionId    );
			CHECK_EQUAL(CICS_SESSION_NAME, conn.getDbInfo().getCICSName().c_str());

            StatementRef	  statement  = conn.createStatement();

			ExecuteReturnInfo execResult = statement.execute(fx_sqlins);
            CHECK_EQUAL(1, (int)execResult.m_affectedRows);

			ResultsetRef res = statement.executeSingleQuery(fx_sqlsel);

			Int32  cnt = 0;
            while ( res.next())
            {
                ++cnt;

				CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
				CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
				CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString("serverstring"));
            }

            CHECK_EQUAL(1, cnt);

			execResult = statement.execute(fx_sqldel);
            CHECK(1 <= execResult.m_affectedRows);
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

    // -------------------------------------------------------------------------------------------------
	TEST_FIXTURE(Fixture, ConnectByName_DefaultCyr)
	{
		DatabaseInfo  dbInfo;
		dbInfo.setCICS(CICS_CONFIG_CYR, CICS_HOST, CICS_PORT, CICS_START, CICS_USER, CICS_PWD);

		try
		{
			ConnectionRef conn = Manager::getInstance().createConnect(dbInfo);
			CHECK      (0               <= conn.getDbInfo().connectionId    );
			CHECK_EQUAL(CICS_SESSION_NAME, conn.getDbInfo().getCICSName().c_str());

            StatementRef	  statement  = conn.createStatement();

			ExecuteReturnInfo execResult = statement.execute(fx_sqlins);
            CHECK_EQUAL(1, (int)execResult.m_affectedRows);

			ResultsetRef res = statement.executeSingleQuery(fx_sqlsel);

			Int32  cnt = 0;
            while ( res.next())
            {
                ++cnt;

				CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
				CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
				CHECK_EQUAL(fx_SERVER_STRING_BG_CICS, res.getString("serverstring"));
            }

            CHECK_EQUAL(1, cnt);

			execResult = statement.execute(fx_sqldel);
            CHECK(1 <= execResult.m_affectedRows);
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

    // -------------------------------------------------------------------------------------------------
	TEST_FIXTURE(Fixture, ConnectBySlot)
	{
		DatabaseInfo  dbInfo;
		dbInfo.eConnectType = basar::CICS;
		dbInfo.user         = CICS_USER;
		dbInfo.passwd       = CICS_PWD;
		dbInfo.connectionId = 0;

		try
		{
			ConnectionRef conn = Manager::getInstance().createConnect(dbInfo);
			CHECK      (0               == conn.getDbInfo().connectionId    );
			CHECK_EQUAL(CICS_SESSION_NAME, conn.getDbInfo().getCICSName().c_str());

            StatementRef	  statement  = conn.createStatement();

			ExecuteReturnInfo execResult = statement.execute(fx_sqlins);
            CHECK_EQUAL(1, (int)execResult.m_affectedRows);

			ResultsetRef res = statement.executeSingleQuery(fx_sqlsel);

			Int32  cnt = 0;
            while ( res.next())
            {
                ++cnt;

				CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
				CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
				CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString("serverstring"));
            }

            CHECK_EQUAL(1, cnt);

			execResult = statement.execute(fx_sqldel);
            CHECK(1 <= execResult.m_affectedRows);
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

    // -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_2_open_statements_fail)
    {
		if (g_RUNSHORT)
			return;

		DatabaseInfo  dbInfo;
		dbInfo.setCICS(CICS_CONFIG_TEST, "", "", "", CICS_USER, CICS_PWD);

		ConnectionRef conn;

		try
		{
			conn = Manager::getInstance().createConnect(dbInfo);
			CHECK      (0               <= conn.getDbInfo().connectionId    );
			CHECK_EQUAL(CICS_SESSION_NAME, conn.getDbInfo().getCICSName().c_str());

			{
				// insert test row
				StatementRef      stmt       = conn.createStatement();
				ExecuteReturnInfo execResult = stmt.execute(fx_sqlins);
				CHECK_EQUAL(1, (int)execResult.m_affectedRows);

				ResultsetRef res = stmt.executeQuery(fx_sqlsel);

				Int32 cnt = 0;
				while ( res.next())
				{
					++cnt;

					CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
					CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
					CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString("serverstring"));

				    StatementRef upd = conn.createStatement();
					CHECK_THROW(upd.execute(fx_sqlupd), basar::db::sql::DbException);
				}

				CHECK_EQUAL(1, cnt);

				// delete test row
				stmt       = conn.createStatement();
				execResult = stmt.execute(fx_sqldel);
				CHECK(1 <= execResult.m_affectedRows);
			}

			conn.close();
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

    // -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_2_open_statements_success)
    {
		if (g_RUNSHORT)
			return;

		DatabaseInfo  dbInfo;
		dbInfo.setCICS(CICS_CONFIG_TEST, "", "", "", CICS_USER, CICS_PWD);

		ConnectionRef conn[2];

		try
		{
			conn[0] = Manager::getInstance().createConnect(dbInfo);
			conn[1] = Manager::getInstance().createConnect(dbInfo);

			{
				// insert test row
				StatementRef      stmt       = conn[0].createStatement();
				ExecuteReturnInfo execResult = stmt.execute(fx_sqlins);
				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
			
				ResultsetRef res = stmt.executeQuery(fx_sqlsel);

				Int32 cnt = 0;
				while ( res.next())
				{
					++cnt;

					CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
					CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
					CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString("serverstring"));

					StatementRef upd = conn[1].createStatement();
					execResult       = upd.execute(fx_sqlupd);
					CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				}

				CHECK_EQUAL(1, cnt);

				// delete test row
				stmt       = conn[0].createStatement();
				execResult = stmt.execute(fx_sqldel);
				CHECK(1 <= execResult.m_affectedRows);
			}

			conn[0].close();
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

	// -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_parallelops)
    {
		DatabaseInfo  dbInfo[8];

		for (size_t i = 0; i < sizeof(dbInfo)/sizeof(dbInfo[0]); ++i)
		{
			if (i < (sizeof(dbInfo)/sizeof(dbInfo[0]))/2)
				dbInfo[i].setCICS(CICS_CONFIG_TEST, 
								  "", "", "", 
								  CICS_USER, CICS_PWD);
			else
				dbInfo[i].setCICS(CICS_CONFIG_WEST, 
								  CICS_HOST, CICS_PORT, CICS_START, 
								  CICS_USER, CICS_PWD);
		}

		ConnectionRef conn[sizeof(dbInfo)/sizeof(dbInfo[0])];
	
		try
		{
			// open connection
			for (size_t i = 0; i < sizeof(conn)/sizeof(conn[0]); ++i)
			{
				conn[i] = Manager::getInstance().createConnect(dbInfo[i]);
				CHECK      (0               <= conn[i].getDbInfo().connectionId    );
				CHECK_EQUAL(CICS_SESSION_NAME, conn[i].getDbInfo().getCICSName().c_str());
			}

			{
				// insert test row
				StatementRef      statement  = conn[0].createStatement();
				ExecuteReturnInfo execResult = statement.execute(fx_sqlins);
				CHECK_EQUAL(1, (int)execResult.m_affectedRows);

				// select over all connections
				for (size_t i = 0; i < sizeof(conn)/sizeof(conn[0]); ++i)
				{
					statement  = conn[i].createStatement();
					ResultsetRef res = statement.executeSingleQuery(fx_sqlsel);

					Int32  cnt = 0;
					while ( res.next())
					{
						++cnt;

						CHECK_EQUAL(fx_MARK                 , res.getInt32 ("test_lint"   ));
						CHECK_EQUAL(fx_CLIENT_STRING        , res.getString("clientstring"));
						CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, res.getString("serverstring"));
					}

					CHECK_EQUAL(1, cnt);
				}

				// delete test row
				statement  = conn[0].createStatement();
				execResult = statement.execute(fx_sqldel);
				CHECK(1 <= execResult.m_affectedRows);
			}

			// close connections
			for (size_t i = 0; i < sizeof(conn)/sizeof(conn[0]); ++i)
			{
				conn[i].close();
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

    // -------------------------------------------------------------------------------------------------
    TEST(testdbsqlcics_statement_insertError)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);

            try
            {	
                // dupl insert with Statement
                {
                    connRef.open();
                    ConstString insertStm = "insert into VSAM.DKPAR_UT (PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
						                    "values ('01','01','03','BasarUnitTest (insert-update-test)')";
                    // dupl insert with Statement nothrow
                    {
                        basar::db::sql::StatementRef statement  = connRef.createStatement();
                        ExecuteReturnInfo            execResult = statement.execute(insertStm,false);
                        CHECK_EQUAL(0, (int)execResult.m_affectedRows);
                        CHECK(0 != execResult.getErrorMessage().length());
                        CHECK_EQUAL(546986254, execResult.getRawMainError());
                        CHECK_EQUAL(0, execResult.getRawSubError());
                        CHECK_EQUAL(basar::db::sql::DBErrorCode::INSERT_DUPL, execResult.m_error);      
                        statement.close();
              
                    }
                    connRef.close();
                    connRef.open();
                    // dupl insert with Statement throw
                    {
                        basar::db::sql::StatementRef statement = connRef.createStatement();
                        CHECK_THROW(statement.execute(insertStm ,true), basar::Exception);
                        statement.close();
                    }
                    connRef.close();
                }
                // nonsense statement
                {
                    connRef.open();
                    ConstString nonsenceStm = "this is no valid statement";
                    // nonsence statement nothrow
                    {
                        basar::db::sql::StatementRef statement =
                            connRef.createStatement();
                        ExecuteReturnInfo execResult = statement.execute(nonsenceStm,false);
                        CHECK_EQUAL(0, (int)execResult.m_affectedRows);
                        CHECK_EQUAL(basar::db::sql::DBErrorCode::NONSPECIFIC,execResult.m_error);      
                        statement.close();
                    }
                    connRef.close();
                    connRef.open();
                    // nonsence statement throw
                    {
                        basar::db::sql::StatementRef statement = 
                            connRef.createStatement();
                        CHECK_THROW(statement.execute(nonsenceStm ,true), basar::Exception);
                        statement.close();
                    }
                    connRef.close();
                }
                //// dupl insert with PreparedStatementRef
                //{
                //    connRef.open();
                //    ConstString insertPrepStm =  "insert into VSAM.DKPAR_UT (VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) values (?,?,?, ?)";
                //    // dupl insert with PreparedStatement no throw
                //    {
                //        basar::db::sql::PreparedStatementRef statement 
                //            = connRef.createPreparedStatement(insertPrepStm);
                //        statement.setString(0,"01");
                //        statement.setString(1,"01");
                //        statement.setString(2,"01");
                //        statement.setString(3,"BasarUnitTest (insert-update-test)");

                //        ExecuteReturnInfo	execResult = statement.execute(false);
                //        CHECK_EQUAL(0, execResult.m_affectedRows);
                //        CHECK_EQUAL(basar::db::sql::DBErrorCode::INSERT_DUPL,execResult.m_error);
                //        statement.close();
                //    }
                //    connRef.close();
                //    connRef.open();
                //    // dupl insert with PreparedStatement  throw
                //    {
                //        basar::db::sql::PreparedStatementRef statement 
                //            = connRef.createPreparedStatement(insertPrepStm);
                //        statement.setString(0,"01");
                //        statement.setString(1,"01");
                //        statement.setString(2,"02");
                //        statement.setString(3,"BasarUnitTest (insert-update-test)");

                //        CHECK_THROW(statement.execute(true), basar::Exception);
                //        statement.close();
                //    }
                //    connRef.close();
                //}

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

            connRef.close();
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

    // -------------------------------------------------------------------------------------------------
    TEST(testdbsqlcics_statement_executeAggregate_executeQuery)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);

            try
            {
                // open:  
                CHECK(!connRef.isOpen());

                //// transactions:
                connRef.open();

                basar::db::sql::StatementRef statement = connRef.createStatement();

                basar::Decimal dec = statement.executeAggregate("select count(*) from  VSAM.DKPAR_UT where PAR_K_FIL = '01'" );
                CHECK_EQUAL(numberOfRowsPAR_K_FIL01, dec.toLong32());

                // in order to realine the other tests:
                if (dec.toLong32() !=  numberOfRowsPAR_K_FIL01)
                {
                    std::cout << "Data Source may be different from ususal test source, please check" << std::endl;
                    std::cout << "The test exactly expects " << numberOfRowsPAR_K_FIL01 << " rows with PAR_K_FIL = '01'" << std::endl;
                    std::cout << "and changes these!" << std::endl;
                }


                ConstString stmt =	"SELECT "
					                   "par_k_fil, par_k_name, par_k_nr, test_dec_scale_0, dkpar_euihw, test_sint, test_lint "
									"FROM vsam.dkpar_ut "
									"WHERE par_k_fil = '01' "
									"ORDER BY par_k_nr;";
                ResultsetRef resultset  = statement.executeQuery(stmt);

				const char * RES_NR[] = 
				{
					"01",
					"02",
					"03"
				};

				Int32 cnt = 0;

                while (resultset.next())
                {
                    CHECK_EQUAL           (resultset.getString ("par_k_fil"       ), resultset.getString (0));
                    CHECK_EQUAL           (resultset.getString ("par_k_name"      ), resultset.getString (1));
                    CHECK_EQUAL           (resultset.getString ("par_k_nr"        ), resultset.getString (2));
                    CHECK_EQUAL           (resultset.getInt32  ("test_dec_scale_0"), resultset.getInt32  (3));
                    CHECK_EQUAL_SHOW_FUNCT(resultset.getDecimal("dkpar_euihw"     ), resultset.getDecimal(4), toString);
                    CHECK_EQUAL           (resultset.getInt16  ("test_sint"       ), resultset.getInt16  (5));
                    CHECK_EQUAL           (resultset.getInt32  ("test_lint"       ), resultset.getInt32  (6));

					CHECK_EQUAL           (RES_NR[cnt]                             , resultset.getString (2));

					++cnt;
                }

				CHECK_EQUAL(static_cast<UInt32>(numberOfRowsPAR_K_FIL01), resultset.getFetchedRows());

				CHECK(!resultset.next());
				CHECK_EQUAL(static_cast<UInt32>(numberOfRowsPAR_K_FIL01), resultset.getFetchedRows());

                CHECK_THROW(resultset.reopen(), InvalidStateException);

				statement.close();
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
            connRef.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}

    }

    // -------------------------------------------------------------------------------------------------

    TEST(testdbsqlcics_preparedstatement_executeQuery_notimplemented)
    {
        try
        {
            basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(
                TestConnection::CICS);

            // open:  
            CHECK(!connRef.isOpen());

            //// transactions:
            connRef.open();

            ConstString stmt =	"select PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DKPAR_EUIHW from VSAM.DKPAR_UT where PAR_K_FIL = '01' ";		
            basar::db::sql::PreparedStatementRef statement; 
			
			CHECK_THROW(statement = connRef.createPreparedStatement(stmt), InvalidStateException);
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
        catch ( ... )
        {
            std::cout << "unknown exception" << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

    }

    // -------------------------------------------------------------------------------------------------

    TEST(testdbsqlcics_update)
    {
        try
        {
            basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            connRef.open();
            try
            {
                basar::db::sql::StatementRef statement = connRef.createStatement();	
                basar::db::sql::ExecuteReturnInfo info1 = statement.execute("update VSAM.DKPAR_UT "
					                                                        "SET DUMMY = 'BasarUnitTest (update test 1) ' "
																			"where PAR_K_FIL  = '01' ");
                CHECK_EQUAL(numberOfRowsPAR_K_FIL01, (int)info1.m_affectedRows);

                basar::db::sql::ExecuteReturnInfo info2 = statement.execute("update VSAM.DKPAR_UT "
					                                                        "SET DUMMY = 'BasarUnitTest (update test 2) ' "
																			"where PAR_K_FIL  = '01' ");
                CHECK_EQUAL(numberOfRowsPAR_K_FIL01, (int)info2.m_affectedRows);
                statement.close();
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
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
        catch ( ... )
        {
            std::cout << "unknown exception" << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
    }

    // -------------------------------------------------------------------------------------------------

    TEST(testdbsqlcics_update_repeated)
    {
        try
        {
            basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            connRef.open();

            try
            {
                basar::db::sql::StatementRef statement = connRef.createStatement();	
				basar::db::sql::ExecuteReturnInfo info;

				const Int32 LIM = 20;
				ConstString SQL = "update VSAM.DKPAR_UT "
					              "SET DUMMY = 'BasarUnitTest (rep test %d) ' "
								  "where PAR_K_FIL  = '01' ";
				VarString   sqlstmt;

				for (Int32 i = 0; i < LIM; ++i)
				{
					sqlstmt.format(SQL, i);
					info = statement.execute(sqlstmt.c_str());
					CHECK_EQUAL(numberOfRowsPAR_K_FIL01, (int)info.m_affectedRows);
				}

                statement.close();
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
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
        catch ( ... )
        {
            std::cout << "unknown exception" << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
    }

    // -------------------------------------------------------------------------------------------------
	class InsertFix
	{
	public:
		VarString					 testname;
		ConnectionRef				 connect;
		StatementRef				 stmt;
		ExecuteReturnInfo::RowNumber rows;	// inserted -> to be deleted
		VarString                    sql;

		static ConstString FILNO;

		InsertFix() :
			connect  (TestConnection::createTestConnection(TestConnection::CICS_OPENED)),
			stmt     (connect.createStatement()),
			rows     (0)
		{
            testname.format("%4.4x", Fixture::fx_rd);
		}

		~InsertFix()
		{
			// cleanup
			VarString del;
			del.format(	
				"DELETE FROM vsam.dkpar_ut "
				"WHERE par_k_fil = '%s';",
				FILNO);

			ExecuteReturnInfo res = stmt.execute(del);
			// CHECK(!res.hasError());                 C4297: function assumed not to throw an exception but does
			// CHECK_EQUAL(rows, res.m_affectedRows);  C4297: function assumed not to throw an exception but does

			stmt.close();

			connect.close();
		}
	};

	ConstString InsertFix::FILNO = "03";

    // -------------------------------------------------------------------------------------------------

    TEST_FIXTURE(InsertFix, testdbsqlcics_insert)
    {
        try
        {	
			{
				sql.format(
					"select PAR_K_NAME from VSAM.DKPAR_UT "
					"where PAR_K_FIL='%s' and PAR_K_NR='01'",
					InsertFix::FILNO);

				ResultsetRef res = stmt.executeQuery(sql);

				int cnt = 0;

				while (res.next())
				{
					++cnt;
				}

				CHECK_EQUAL(0, cnt);
			}

			{
				sql.format( 
					"insert into VSAM.DKPAR_UT "
					"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
					"values "
					"('%s', '%s', '02', 'BasarUnitTest (insert test)');",
					InsertFix::FILNO,
					testname.c_str());

				ExecuteReturnInfo res = stmt.execute(sql);

				CHECK_EQUAL(1, (int)res.m_affectedRows);
				CHECK_EQUAL(0, res.m_error);

				rows = 1;
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

    // -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(InsertFix, testdbsqlcics_multipleinsert)
    {
        try
        {
			sql.format(
				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '%02d', 'BasarUnitTest (insert test)');",
				InsertFix::FILNO,
				testname.c_str(),
				1);

			ExecuteReturnInfo res = stmt.execute(sql);
            CHECK_EQUAL(1, (int)res.m_affectedRows);
            CHECK_EQUAL(0, res.m_error);


			sql.format(
				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '%02d', 'BasarUnitTest (insert test)');",
				InsertFix::FILNO,
				testname.c_str(),
				2);

            res = stmt.execute(sql.c_str());
            CHECK_EQUAL(1, (int)res.m_affectedRows);
            CHECK_EQUAL(0, res.m_error);

			rows = 2;
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

    // -------------------------------------------------------------------------------------------------
    // by factor 8-10 faster than separate transmissions
    TEST_FIXTURE(InsertFix, testdbsqlcics_multipleinsert_singlestatement)
    {
        try
        {
			sql.format(
				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '01', 'BasarUnitTest (insert test)');"

				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '02', 'BasarUnitTest (insert test)');"

				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '03', 'BasarUnitTest (insert test)');",

				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str());
            
			ExecuteReturnInfo res = stmt.execute(sql);

            CHECK_EQUAL(1, (int)res.m_affectedRows);
            CHECK_EQUAL(0, res.m_error);

			rows = 3;
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

    // -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(InsertFix, testdbsqlcics_MultipleInsertWithError_singlestatement)
    {
        try
        {
			sql.format(
				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '01', 'BasarUnitTest (insert test)');"

				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '02', 'BasarUnitTest (insert test)');"

			// -> !!!
				"CCCC into DDDD "	
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '03', 'BasarUnitTest (insert test)');",

				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str());

			ExecuteReturnInfo res = stmt.execute(sql);

            CHECK_EQUAL(1, (int)res.m_affectedRows);
            CHECK_EQUAL(0, res.m_error);

			rows = 2;
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

    // -------------------------------------------------------------------------------------------------
    TEST_FIXTURE(InsertFix, testdbsqlcics_MultipleInsertWithErrorDupl_singlestatement)
    {
        try
        {
			sql.format(
				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '01', 'BasarUnitTest (insert test)');"

				"insert into VSAM.DKPAR_UT "
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '02', 'BasarUnitTest (insert test)');"

			// -> !!! duplicated 
				"insert into VSAM.DKPAR_UT "	
				"(VSAM.DKPAR_UT.PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY ) "
				"values "
				"('%s', '%s', '02', 'BasarUnitTest (insert test)');",

				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str(),
				InsertFix::FILNO, testname.c_str());

			ExecuteReturnInfo res = stmt.execute(sql);

            CHECK_EQUAL(1, (int)res.m_affectedRows);
            CHECK_EQUAL(0, res.m_error);

			rows = 2;
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

	// -------------------------------------------------------------------------------------
    TEST(testdbsqlcics_statement_executeQuery_DecimalScale0)
    {
        static ConstString stmt =	"SELECT   par_k_name, par_k_nr, test_dec_scale_0, dkpar_euihw, "
			                                 "DECIMAL(   '1.11', 7, 2, '.') as expr0, "
											 "DECIMAL(   '0.99', 7, 2, '.') as expr1, "
											 "DECIMAL('2341.18', 9, 2, '.') as expr2 "
							        "FROM     vsam.dkpar_ut "
									"WHERE    par_k_fil = '01' "
									"ORDER BY par_k_name, par_k_nr;";

		static ConstString	resString[][2] = 
		{
			{ "PA29", "01" },
			{ "01"  , "02" },
			{ "01"  , "03" }
		};

		static const Int32 resInt[] = 
		{
			123456789,
				-4321,
			  -987654
		};

		static const Decimal resDec[] = 
		{
			Decimal(1.23),
			Decimal(0.00),
			Decimal(-23)
		};

		Decimal expr0,
			    expr1,
				expr2;
		expr0.fromNonLocString(   "1.11");
		expr1.fromNonLocString(   "0.99");
		expr2.fromNonLocString("2341.18");

		static const Int32 LIM = sizeof(resInt) / sizeof(resInt[0]);

        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

            StatementRef statement = connRef.createStatement();
            ResultsetRef resultset = statement.executeQuery(stmt);

			Int32 cnt = 0;


            while (resultset.next())
            {
				CHECK(LIM > cnt);

				if (LIM > cnt)
				{
					CHECK_EQUAL(resString[cnt][0], resultset.getString ("par_k_name"      )          );
					CHECK_EQUAL(resString[cnt][1], resultset.getString ("par_k_nr"        )          );
					CHECK_EQUAL(resInt   [cnt]   , resultset.getInt32  ("test_dec_scale_0")          );
					CHECK_EQUAL(resInt   [cnt]   , resultset.getDecimal("test_dec_scale_0").toInt32());
					CHECK      (resDec   [cnt]  == resultset.getDecimal("dkpar_euihw"     )          );

					if (1 == cnt)
						CHECK_EQUAL(resInt[cnt], resultset.getInt16("test_dec_scale_0"));
					else
						CHECK_THROW(resultset.getInt16("test_dec_scale_0"), InvalidParameterException);

					CHECK      (expr0           == resultset.getDecimal("expr0"           )          );
					CHECK      (expr1           == resultset.getDecimal("expr1"           )          );
					CHECK      (expr2           == resultset.getDecimal("expr2"           )          );
				}

				++cnt;
            }

            CHECK_EQUAL(LIM, cnt);
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
    }

	// -------------------------------------------------------------------------------------
    TEST(testdbsqlcics_statement_Query_DecimalScale0)
    {
		static ConstString stmt = "SELECT par_k_name, par_k_nr "
			                      "FROM   VSAM.DKPAR_UT "
								  "WHERE  par_k_fil = '01' AND test_dec_scale_0 = 123456789";		

        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			{
				StatementRef statement = connRef.createStatement();
				ResultsetRef resultset = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resultset.next())
				{
					CHECK_EQUAL("PA29", resultset.getString("par_k_name"));
					CHECK_EQUAL("01"  , resultset.getString("par_k_nr"  ));

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_Write_DecimalScale0)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			ConstString NAME = fx_randName;

			{
				static ConstString s = "INSERT INTO vsam.dkpar_ut "
										"(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw,"
										"test_lint, test_sint, test_float1, test_float2, test_date, test_time) "
										"VALUES "
										"('03', '%s', '03', 'BasarUnitTest (insert test)', -987654321, -34.567, "
										"8987782, -31210, 236.098, -78.123, '2009-12-23', '21.34.01');";
				VarString sql;
				sql.format(s, NAME);

	            StatementRef      statement  = connRef.createStatement();
				ExecuteReturnInfo execResult = statement.execute(sql);

				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				CHECK_EQUAL(0, execResult.m_error);
			}
				
			// check inserted values

			{
				static ConstString sql = "SELECT "
											"par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw, "
											"test_lint, test_sint, test_float1, test_float2, test_date, test_time "
										"FROM  vsam.dkpar_ut "
										"WHERE par_k_fil ='03' AND par_k_name = '%s' AND par_k_nr = '03'";
				VarString stmt;
				stmt.format(sql, NAME);

				StatementRef statement = connRef.createStatement();
				ResultsetRef resSet = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resSet.next())
				{
					CHECK_EQUAL("03"                         , resSet.getString(0));
					CHECK_EQUAL(NAME                         , resSet.getString(1));
					CHECK_EQUAL("03"                         , resSet.getString(2));
					CHECK_EQUAL("BasarUnitTest (insert test)", resSet.getString(3));

					CHECK_EQUAL(-987654321, resSet.getInt32  (4)          );
					CHECK_EQUAL(-987654321, resSet.getDecimal(4).toInt32());
					CHECK(Decimal(-34.567)  ==	resSet.getDecimal(5));

					CHECK_EQUAL(8987782                       , resSet.getInt32(6));
					CHECK_EQUAL(-31210                        , resSet.getInt16(7));
					CHECK_CLOSE(static_cast<Float32>( 236.098), resSet.getFloat32(8), 0.001);
					CHECK_CLOSE(static_cast<Float64>(-78.123) , resSet.getFloat64(9), 0.0005);
  
					CHECK(Date(2009, 12, 23)               == resSet.getDate(10));
					CHECK(Time(Time::NULL_DATE, 213401000) == resSet.getTime(11));

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_Write_DecimalScale0_1)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			ConstString NAME = fx_randName;

			{
				static ConstString s  = "INSERT INTO vsam.dkpar_ut "
										"(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw)"
										"VALUES "
										"('03', '%s', '04', 'BasarUnitTest (insert test 2)', 123456789, 1.56789);";

				VarString sql;
				sql.format(s, NAME);

	            StatementRef      statement  = connRef.createStatement();
				ExecuteReturnInfo execResult = statement.execute(sql);

				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				CHECK_EQUAL(0, execResult.m_error);
			}
				
			// check inserted values

			{
				static ConstString sql = "SELECT "
											"par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw "
										"FROM  vsam.dkpar_ut "
										"WHERE par_k_fil ='03' AND par_k_name = '%s' AND par_k_nr = '04'";
				VarString stmt;
				stmt.format(sql, NAME);

				StatementRef statement = connRef.createStatement();
				ResultsetRef resSet = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resSet.next())
				{
					CHECK_EQUAL("03"                                , resSet.getString(0));
					CHECK_EQUAL(NAME                                , resSet.getString(1));
					CHECK_EQUAL("04"                                , resSet.getString(2));
					CHECK_EQUAL("BasarUnitTest (insert test 2)", resSet.getString(3));

					CHECK_EQUAL(123456789, resSet.getInt32  (4)          );
					CHECK_EQUAL(123456789, resSet.getDecimal(4).toInt32());
					CHECK(Decimal(1.56789)  ==	resSet.getDecimal(5));

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_Write_DecimalScale0_2)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			ConstString NAME = fx_randName;

			{
				static ConstString s   = "INSERT INTO vsam.dkpar_ut "
										"(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw)"
										"VALUES "
										"('03', '%s', '05', 'BasarUnitTest (insert test 3)', 26789, 1.56789);";

				VarString sql;
				sql.format(s, NAME);

	            StatementRef      statement  = connRef.createStatement();
				ExecuteReturnInfo execResult = statement.execute(sql);

				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				CHECK_EQUAL(0, execResult.m_error);
			}
				
			// check inserted values

			{
				static ConstString sql = "SELECT "
											"par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw "
										"FROM  vsam.dkpar_ut "
										"WHERE par_k_fil ='03' AND par_k_name = '%s' AND par_k_nr = '05'";
				VarString stmt;
				stmt.format(sql, NAME);

				StatementRef statement = connRef.createStatement();
				ResultsetRef resSet = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resSet.next())
				{
					CHECK_EQUAL("03"                           , resSet.getString(0));
					CHECK_EQUAL(NAME                           , resSet.getString(1));
					CHECK_EQUAL("05"                           , resSet.getString(2));
					CHECK_EQUAL("BasarUnitTest (insert test 3)", resSet.getString(3));

					CHECK_EQUAL(26789, resSet.getInt32  (4)          );
					CHECK_EQUAL(26789, resSet.getDecimal(4).toInt32());
					CHECK(Decimal(1.56789)  ==	resSet.getDecimal(5));

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_multiexecutes_a)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			ConstString  NAME       = fx_randName;
            StatementRef statement  = connRef.createStatement();

			{
				static ConstString stmt = "SELECT par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw "
										  "FROM  vsam.dkpar_ut "
										  "WHERE par_k_fil ='01'";

				ResultsetRef resSet = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resSet.next())
				{
					CHECK_EQUAL("01"                           , resSet.getString(0));
					++cnt;
				}

				CHECK_EQUAL(3, cnt);
			}

			{
				static ConstString s   = "INSERT INTO vsam.dkpar_ut "
										"(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw)"
										"VALUES "
										"('03', '%s', '06', 'BasarUnitTest (insert test 4)', 46789, 2.56789);";

				VarString sql;
				sql.format(s, NAME);

				ExecuteReturnInfo execResult = statement.execute(sql);

				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				CHECK_EQUAL(0, execResult.m_error);
			}
				
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_multiexecutes_b)
    {
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			ConstString  NAME       = fx_randName;
            StatementRef statement  = connRef.createStatement();

			{
				static ConstString s   = "INSERT INTO vsam.dkpar_ut "
										"(par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw)"
										"VALUES "
										"('03', '%s', '07', 'BasarUnitTest (insert test 4)', 36789, 1.56789);";

				VarString sql;
				sql.format(s, NAME);

				ExecuteReturnInfo execResult = statement.execute(sql);

				CHECK_EQUAL(1, (int)execResult.m_affectedRows);
				CHECK_EQUAL(0, execResult.m_error);
			}
				
			// check inserted values

			{
				static ConstString sql = "SELECT "
											"par_k_fil, par_k_name, par_k_nr, dummy, test_dec_scale_0, dkpar_euihw "
										"FROM  vsam.dkpar_ut "
										"WHERE par_k_fil ='03' AND par_k_name = '%s' AND par_k_nr = '07'";
				VarString stmt;
				stmt.format(sql, NAME);

				ResultsetRef resSet = statement.executeQuery(stmt);

				Int32 cnt = 0;

				while (resSet.next())
				{
					CHECK_EQUAL("03"                           , resSet.getString(0));
					CHECK_EQUAL(NAME                           , resSet.getString(1));
					CHECK_EQUAL("07"                           , resSet.getString(2));
					CHECK_EQUAL("BasarUnitTest (insert test 4)", resSet.getString(3));

					CHECK_EQUAL(36789, resSet.getInt32  (4)          );
					CHECK_EQUAL(36789, resSet.getDecimal(4).toInt32());
					CHECK(Decimal(1.56789)  ==	resSet.getDecimal(5));

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
	TEST_FIXTURE(Fixture, testdbsqlcics_statement_cleanup)
	{
        try
        {
            ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
            CHECK(!connRef.isOpen());
            connRef.open();

			VarString sql;
			sql.format("DELETE FROM vsam.dkpar_ut "
				       "WHERE par_k_fil = '03' AND par_k_name = '%s';",
				       fx_randName);

            StatementRef      statement  = connRef.createStatement();
            ExecuteReturnInfo execResult = statement.execute(sql);
			CHECK(0 < execResult.m_affectedRows);
			CHECK_EQUAL(0, execResult.m_error);
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_Query_FetchBuffer)
    {
		// sort order for CICS host chars
		static char RES_NR_CHARS[] = 
		{
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
			'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'      
		};

		static const Int32 NO_CHARS = sizeof(RES_NR_CHARS);
		static const Int32 INSROWS  = 50;

		// try to force more than one tcACCESS resultset buffer
		static ConstString SEL =  "SELECT *, "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
			                      "'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', "
								  "'FILL FILL FILL FILL FILL FILL' "
			                      "FROM vsam.dkpar_ut "
								  "WHERE par_k_fil = '02' AND par_k_name = '%s' "
								  "ORDER BY par_k_nr; ";

		static ConstString INS =  "INSERT INTO vsam.dkpar_ut "
									"(par_k_fil, par_k_name, par_k_nr   , dummy                        , test_dec_scale_0, dkpar_euihw, "
									"test_lint , test_sint , test_float1, test_float2                  , test_date       , test_time  ) "
									"VALUES "
									"('02'     , '%s'      , '%c%c'   , 'BasarUnitTest (insert test)', -987654321      , -34.567    , "
									" 8987782  , -31210    , 236.098    , -78.123                      , '2009-12-23'    , '21.34.01' );";

		static ConstString DEL =  "DELETE FROM vsam.dkpar_ut "
			                      "WHERE par_k_fil = '02' AND par_k_name = '%s';";

		ConstString  NAME    = fx_randName;
		VarString    sql;

        ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
        CHECK(!connRef.isOpen());
        connRef.open();

        try
        {

			// insert INSROWS rows
			{
				StatementRef      statement = connRef.createStatement();
				ExecuteReturnInfo execResult;
				Int32             a, b;

				for (Int32 i = 0; i < INSROWS; ++i)
				{
					a = i / NO_CHARS;		// first  literal in expected "nr"
					b = i % NO_CHARS;		// second literal in expected "nr"

					sql.format(INS, NAME, RES_NR_CHARS[a], RES_NR_CHARS[b]);
					
					execResult = statement.execute(sql);

					CHECK_EQUAL(1, (int)execResult.m_affectedRows);
					CHECK_EQUAL(0, execResult.m_error);
				}
			}

			// select with more than 1 tcACCESS fetch buffer
			{
				sql.format(SEL, NAME);

				StatementRef statement = connRef.createStatement();
				ResultsetRef resultset = statement.executeQuery(sql);

				Int32     cnt = 0;
				VarString nr;
				VarString cmp;
				Int32     a, b;


				while (resultset.next())
				{
					a = cnt / NO_CHARS;		// first  literal in expected "nr"
					b = cnt % NO_CHARS;		// second literal in expected "nr"

					cmp.format("%c%c", RES_NR_CHARS[a], RES_NR_CHARS[b]);
					nr = resultset.getString(2);

					CHECK_EQUAL(cmp, nr);

					++cnt;
				}

				CHECK_EQUAL(INSROWS, cnt);
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}

		// delete inserted rows
		{
			sql.format(DEL, NAME);

			StatementRef      statement  = connRef.createStatement();
			ExecuteReturnInfo execResult = statement.execute(sql);

			CHECK(0 < execResult.m_affectedRows);
			CHECK_EQUAL(0, execResult.m_error);
		}
	}

	// -------------------------------------------------------------------------------------
    TEST_FIXTURE(Fixture, testdbsqlcics_statement_size_larger_32k)
    {
		// try to force more than one tcACCESS resultset buffer
		static ConstString SEL =  "SELECT *, ";
	
		static ConstString BLOWUP = " 'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL', ";
		static ConstString BLOWUP_END = " 'FILL FILL FILL FILL FILL FILL', 'FILL FILL FILL FILL FILL FILL' ";

		VarString    sql;

        ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::CICS);
        CHECK(!connRef.isOpen());
        connRef.open();

        try
        {
			// select with more than 1 tcACCESS fetch buffer
			{
				sql.append(SEL);

				while (sql.size() < 32768)
				{
					sql.append(BLOWUP);
				}
				sql.append(BLOWUP_END);



				StatementRef statement = connRef.createStatement();

				CHECK_THROW(ResultsetRef resultset = statement.executeQuery(sql), basar::db::sql::DbExceptionStatementLengthExceeded);

				CHECK_THROW(statement.execute(sql), basar::db::sql::DbExceptionStatementLengthExceeded);

				basar::db::sql::ExecuteReturnInfo eri = statement.execute(sql, false);

				CHECK_EQUAL(true, eri.hasError());
				CHECK_EQUAL(basar::db::sql::DBErrorCode::LENGHT_EXCEEDED, eri.getError());
			}
		}
        catch (basar::Exception & ex)
        {
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
        }
		catch ( ... )
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

// -------------------------------------------------------------------------------------
}       // SUITE

#endif	// HAS_CICS
#endif	// TESTDBSQLCICS


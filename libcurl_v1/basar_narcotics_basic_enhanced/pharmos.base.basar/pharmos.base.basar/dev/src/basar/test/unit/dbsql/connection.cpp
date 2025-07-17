#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasardbsql_manager.h"

#include "testconnection.h"
#include "testinfxconnection.h"
#include "odbcfix.h"

#include <iostream>
#include <set>
#include <stdio.h>


using basar::Int32;
using basar::ULong32;
using basar::VarString;
using basar::InvalidStateException;

using basar::db::sql::DbException;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::ConnectionListSize;
using basar::db::sql::Manager;

using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::TestInfxConnection;

//------------------------------------------------------------------------------
SUITE(Connection)
{

TEST(multipleINFXConnectionsWithLocaleChange)
{
    //in order to test a specific error, this test has to be the first - don't move it
    try
	{
		TestConnection::TestTypes types;

		types.push_back(TestConnection::INFX_BG);
		types.push_back(TestConnection::INFX_BG);
		
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);    
    }
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

TEST(multipleINFXConnectionsWithoutLocaleChange)
{
    try
	{
		TestConnection::TestTypes types;

		types.push_back(TestConnection::INFX);
		types.push_back(TestConnection::INFX);
		
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);    
    }
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

//-----------------------------------------------------------------

// ----------------------------------------------------------
TEST_FIXTURE(OdbcFix, testdbsqlconnectionref_reset_null)
{
	try
	{
		ConnectionRef connRef;
		ConnectionRef connRef2;
		CHECK(connRef.isNull());

		connRef2 = TestConnection::createTestConnection(TestConnection::INFX_BG);
		CHECK(!connRef2.isNull());
		connRef2.reset();
		CHECK(connRef2.isNull());

		connRef = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());

		connRef2 = TestConnection::createTestConnection(TestConnection::INFX);
		CHECK(!connRef2.isNull());
		connRef2.reset();
		CHECK(connRef2.isNull());

#ifdef HAS_ODBC
		// ODBC:
		connRef = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());

		connRef = TestConnection::createTestConnection(TestConnection::ODBC_BG);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());

		connRef = TestConnection::createTestConnection(TestConnection::ODBC);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());
		
		// MSSQL ODBC:
		connRef = TestConnection::createTestConnection(TestConnection::MSSQL_ODBC);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());
		
#endif

#ifndef _WIN32
		CHECK_THROW(connRef = TestConnection::createTestConnection(TestConnection::ODBC), basar::InvalidParameterException);
#endif

#ifdef HAS_CICS
		connRef = TestConnection::createTestConnection(TestConnection::CICS);
		CHECK(!connRef.isNull());
		connRef.reset();
		CHECK(connRef.isNull());
#endif
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

// ----------------------------------------------------------
TEST_FIXTURE(OdbcFix, testdbsqlconnectionref_open_begin_rollback_commit)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
		TestConnection::TestTypes            types;
        TestConnection::TestTypes::size_type correct = 0;

		types.push_back(TestConnection::INFX);
		types.push_back(TestConnection::INFX_BG);
#ifdef HAS_ODBC
		types.push_back(TestConnection::ODBC);
		types.push_back(TestConnection::ODBC_BG);
		
		types.push_back(TestConnection::MSSQL_ODBC);
#endif 

#ifdef HAS_CICS
		types.push_back(TestConnection::CICS);
		types.push_back(TestConnection::CICS_SLOT_0);
        correct = 2;    // CICS connection managed by TestConnection aren't opened by default
#endif

		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		CHECK_EQUAL(static_cast<ULong32> (types.                 size()), 
				    static_cast<ULong32> (Manager::getInstance().size()));
		CHECK_EQUAL(static_cast<ULong32> (types.size() - correct), 
				    static_cast<ULong32> (Manager::getInstance().sizeOpen()));

		ConnectionRef conn;	// Infx CDSK 3.70 Win issue with ODBC

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			eConnType = itConn->first;

			conn = itConn->second;

			// open:  
			switch (itConn->first)
			{
			case TestConnection::CICS:
			case TestConnection::CICS_SLOT_0:
				CHECK(!conn.isOpen());
				break;
			default:
				CHECK(conn.isOpen());
				break;
			}

			conn.close();
			CHECK(!conn.isOpen());

			conn.open();
			CHECK(conn.isOpen());

			// transactions:
			switch (itConn->first)
			{
			case TestConnection::CICS:
			case TestConnection::CICS_SLOT_0:
				CHECK_THROW(conn.begin   (), InvalidStateException);
				CHECK_THROW(conn.rollback(), InvalidStateException);
				CHECK_THROW(conn.commit  (), InvalidStateException);
				break;

			default:
				conn.begin();
				CHECK(conn.isInTransaction());
				conn.rollback();
				CHECK(!conn.isInTransaction());

				conn.begin();
				CHECK(conn.isInTransaction());
				conn.commit();
				CHECK(!conn.isInTransaction());

				CHECK_THROW(conn.commit(), DbException);
				break;
			}

			conn.close();
			CHECK(!conn.isInTransaction());
		}
	}
	catch (basar::Exception & ex)
	{
		std::cout << "connection type " << eConnType << std::endl;
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
	catch (...)
	{
		std::cout << "connection type " << eConnType << std::endl;
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

// ----------------------------------------------------------
TEST_FIXTURE (OdbcFix, testdbsqlconnectionref_2parallelconnects_and_1transaction)
{

	TestConnection::TestConnectionType eConnType1 = TestConnection::INFX;
	TestConnection::TestConnectionType eConnType2 = TestConnection::INFX;

	try
	{
		TestConnection::TestTypes types1;
		TestConnection::TestTypes types2;

		types1.push_back(TestConnection::INFX);
		types1.push_back(TestConnection::INFX_BG);
		types1.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION);
		types1.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION_BG);

		types2.push_back(TestConnection::INFX);
		types2.push_back(TestConnection::INFX_BG);
		types2.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION);
		types2.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION_BG);

#ifdef HAS_ODBC
		types1.push_back(TestConnection::ODBC);
		types1.push_back(TestConnection::ODBC_BG);

		types2.push_back(TestConnection::ODBC);
		types2.push_back(TestConnection::ODBC_BG);
		
		types1.push_back(TestConnection::MSSQL_ODBC);
		types2.push_back(TestConnection::MSSQL_ODBC);
#endif

#ifdef HAS_CICS
		// order is important: slot0 before CICS because CICS finds next free slot > 0 for 2 parallel connections 
		types2.push_back(TestConnection::CICS_SLOT_0_OPENED);
		types2.push_back(TestConnection::CICS_OPENED);
#endif

		TestConnection::TestConnectionMap conns1 = TestConnection::createTestConnections(types1);

		CHECK_EQUAL(static_cast<ULong32> (types1.size()), static_cast<ULong32> (Manager::getInstance().size    ()));
		CHECK_EQUAL(static_cast<ULong32> (types1.size()), static_cast<ULong32> (Manager::getInstance().sizeOpen()));

		for (TestConnection::TestConnectionMap::const_iterator itConn1 = conns1.begin();
				itConn1 != conns1.end();
				++itConn1)
		{
			eConnType1 = itConn1->first;


			TestConnection::TestConnectionMap conns2 = TestConnection::createTestConnections(types2);

			CHECK_EQUAL(static_cast<ULong32> (types1.size() + types2.size()), static_cast<ULong32> (Manager::getInstance().size    ()));
			CHECK_EQUAL(static_cast<ULong32> (types1.size() + types2.size()), static_cast<ULong32> (Manager::getInstance().sizeOpen()));


			for (TestConnection::TestConnectionMap::const_iterator itConn2 = conns2.begin();
				itConn2 != conns2.end();
				++itConn2)
			{
				eConnType2 = itConn2->first;

				ConnectionRef conn1 = itConn1->second;
				ConnectionRef conn2 = itConn2->second;

				CHECK(!conn1.isNull());
				CHECK(conn1 .isOpen());
				CHECK(!conn2.isNull());
				CHECK(conn2 .isOpen());

				// nested
				conn1.begin();

				StatementRef stmt2 = conn2.createStatement();
				StatementRef stmt1 = conn1.createStatement();

				// only for Informix combinations
				if ( ( (itConn1->first == TestConnection::INFX   ) || 
						(itConn1->first == TestConnection::INFX_BG)    
						) 
						&&
						( (itConn2->first == TestConnection::INFX                          ) || 
						(itConn2->first == TestConnection::INFX_BG                       ) ||
						(itConn2->first == TestConnection::INFX_CONCURRENT_TRANSACTION   ) ||
						(itConn2->first == TestConnection::INFX_CONCURRENT_TRANSACTION_BG) 
						) 
					)
				{
					CHECK_THROW(stmt2.execute("set isolation dirty read"), DbException);	// need "concurrent transaction"
				}
				else
				{
					if ((itConn2->first == TestConnection::CICS_OPENED       ) ||
						(itConn2->first == TestConnection::CICS_SLOT_0_OPENED)    )
					{
						ExecuteReturnInfo info2 = stmt2.execute("update vsam.dkpar_ut "
								                                "set test_lint = 0 "
																"where par_k_fil = 'ZZ'");
						CHECK_EQUAL(false, info2.hasError());
					}
					else
					{
						ExecuteReturnInfo info2;
						if (TestConnection::MSSQL_ODBC == itConn2->first)
						{
						    info2 = stmt2.execute("set transaction isolation level read uncommitted");
						}
						else
						{
						    info2 = stmt2.execute("set isolation dirty read");
						}
						CHECK_EQUAL(false, info2.hasError());
					}
				}

				ExecuteReturnInfo info1;
				
				if (TestConnection::MSSQL_ODBC == itConn1->first)
				{
				    info1 = stmt1.execute("set transaction isolation level read committed");
				}
				else
				{
				    info1 = stmt1.execute("set isolation committed read");
				}
				
				CHECK_EQUAL(false, info1.hasError());

				conn1.rollback();
			}
		}
	}
	catch (basar::Exception & ex)
	{
		std::cout << "connection type1 " << eConnType1 << " connection type2 " << eConnType2 << std::endl;
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
	catch (...)
	{
		std::cout << "connection type1 " << eConnType1 << " connection type2 " << eConnType2 << std::endl;
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

#ifdef HAS_CICS
// ----------------------------------------------------------
TEST(testdbsqlconnectionref_cics_open)
{
	try
	{
		std::vector<TestConnection::TestConnectionType> typevec;
		typevec.push_back(TestConnection::CICS);
		typevec.push_back(TestConnection::CICS_SLOT_0);

		TestConnection::TestConnectionMap  connRefs = TestConnection::createTestConnections(typevec);
		TestConnection::TestConnectionMap::const_iterator iter;

		std::set<Int32> slotlist;

		for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
		{	
			ConnectionRef connRef = iter->second;
			try
			{
				Int32 slot   = connRef.getDbInfo().connectionId;
				bool  exists = slotlist.end() != slotlist.find(slot);

				if (!exists)
					slotlist.insert(slot);

				CHECK(!connRef.isOpen());

				if (exists)
				{
					CHECK_THROW(connRef.open(), InvalidStateException);
					CHECK( !connRef.isOpen() );
				}
				else
				{
					// check time for tcACCESS allocate session
					{
						UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
						connRef.open();
					}
					CHECK(connRef.isOpen());
				}

				connRef.close();
				CHECK(!connRef.isOpen());

				if (exists)
				{
					CHECK_THROW(connRef.open(), InvalidStateException);
					CHECK( !connRef.isOpen() );
				}
				else
				{
					// check time for tcACCESS allocate session
					{
						UNITTEST_TIME_CONSTRAINT(5 * 1000);	// ms
						connRef.open();
					}
					CHECK(connRef.isOpen());
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
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

		for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
		{	
			ConnectionRef connRef = iter->second;
            connRef.close();
        }
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured2 = false;
		CHECK(ExceptionOccured2);
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}
#endif

// ----------------------------------------------------------
TEST(testdbsqlconnectionref_infx_getDBInfo)
{
	try
	{
		ConnectionRef connRef = TestInfxConnection::createDefaultConnectionRef();

		DatabaseInfo info  = connRef.getDbInfo();
		DatabaseInfo info2 = TestInfxConnection::getDefaultDataBaseInfo();

		CHECK_EQUAL(info.dbServer    , info2.dbServer);
		CHECK_EQUAL(info.database    , info2.database);
		CHECK_EQUAL(-2               , info2.connectionId);
		CHECK_EQUAL(info.connectionId, info2.connectionId);
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

// ----------------------------------------------------------
TEST(testdbsqlconnectionref_infx_invalidDbInfo)
{
    CHECK_THROW(
        TestInfxConnection::createInvalidConnectionRef(),
        DbException);
}

// ----------------------------------------------------------
TEST(testinfx_isolationlevels)
{
	try
	{
		ConnectionRef connect = TestInfxConnection::createDefaultConnectionRef();

		connect.setIsolationLevel(basar::INFX_DIRTYREAD                       );
		connect.setIsolationLevel(basar::INFX_DIRTYREAD                 , true);
		connect.setIsolationLevel(basar::INFX_DIRTYREAD_WITHWARNING           );
		connect.setIsolationLevel(basar::INFX_DIRTYREAD_WITHWARNING     , true);

		connect.setIsolationLevel(basar::INFX_COMITTEDREAD                    );
		connect.setIsolationLevel(basar::INFX_COMITTEDREAD              , true);
		connect.setIsolationLevel(basar::INFX_COMITTEDREAD_LASTCOMMITTED      );
		connect.setIsolationLevel(basar::INFX_COMITTEDREAD_LASTCOMMITTED, true);

		connect.setIsolationLevel(basar::INFX_CURSORSTABILITY                 );
		connect.setIsolationLevel(basar::INFX_CURSORSTABILITY           , true);

		connect.setIsolationLevel(basar::INFX_REPEATABLEREAD                  );

		connect.setLockModeWait  (basar::WAIT);
		connect.setLockModeWait  (basar::WAITSECS, 20);
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

// ----------------------------------------------------------
TEST_FIXTURE(OdbcFix, testIsValid)
{

	try
	{
		TestConnection::TestTypes types;
		types.push_back(TestConnection::INFX);

#ifdef HAS_ODBC
		types.push_back(TestConnection::ODBC);
		types.push_back(TestConnection::MSSQL_ODBC);
#endif 
#ifdef HAS_CICS
		types.push_back(TestConnection::CICS_OPENED);
#endif


		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef conn = itConn->second;

			CHECK(conn.isValid());

			conn.close();
			CHECK(!conn.isValid());
		}
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

// ----------------------------------------------------------
TEST_FIXTURE(OdbcFix, testParallelConnections)
{
	try
	{
		TestConnection::TestTypes types;

		types.push_back(TestConnection::INFX);
//		types.push_back(TestConnection::INFX_BG);
		types.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION);
//		types.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION_BG);

		types.push_back(TestConnection::INFX);
//		types.push_back(TestConnection::INFX_BG);
		types.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION);
//		types.push_back(TestConnection::INFX_CONCURRENT_TRANSACTION_BG);

#ifdef HAS_ODBC
		types.push_back(TestConnection::ODBC);
		types.push_back(TestConnection::ODBC_BG);

		types.push_back(TestConnection::ODBC);
		types.push_back(TestConnection::ODBC_BG);
		
		types.push_back(TestConnection::MSSQL_ODBC);
		types.push_back(TestConnection::MSSQL_ODBC);
#endif 

#ifdef HAS_CICS
		types.push_back(TestConnection::CICS_OPENED);
		types.push_back(TestConnection::CICS_OPENED);
		types.push_back(TestConnection::CICS_OPENED);
		types.push_back(TestConnection::CICS_OPENED);
		types.push_back(TestConnection::CICS_BG_OPENED);
#endif

		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		CHECK_EQUAL(static_cast<ULong32> (types.size()), static_cast<ULong32> (Manager::getInstance().size    ()));
		CHECK_EQUAL(static_cast<ULong32> (types.size()), static_cast<ULong32> (Manager::getInstance().sizeOpen()));

	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
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

// ----------------------------------------------------------------

class EnvFix
{
public:
	enum 
	{
		BEFORE = 0,
		AFTER,

		TOTAL
	};

	VarString client[TOTAL];
	VarString db    [TOTAL];

	EnvFix() : 	
		env_client("CLIENT_LOCALE"),
	    val_client("CLIENT_abc"   ),
	    env_db    ("DB_LOCALE"    ),
	    val_db    ("DB_abc"       )

	{
		// preset environment -> should be restored
		static char assign_client[512];
		static char assign_db    [512];

		sprintf(assign_client, "%s=%s", env_client, val_client);
		sprintf(assign_db    , "%s=%s", env_db    , val_db);
		
		CHECK_EQUAL(0, TestInfxConnection::putEnv(assign_client));
		CHECK_EQUAL(0, TestInfxConnection::putEnv(assign_db    ));
	}

	~EnvFix()
	{
		//CHECK_EQUAL(val_client, client[BEFORE]);  C4297: function assumed not to throw an exception but does
		//CHECK_EQUAL(val_db    , db    [BEFORE]);  C4297: function assumed not to throw an exception but does
	}

	void connect(TestConnection::TestConnectionType type)
	{
		client[BEFORE] = TestInfxConnection::getEnv(env_client);
		db    [BEFORE] = TestInfxConnection::getEnv(env_db    );

		ConnectionRef conn = TestConnection::createTestConnection(type);

		client[AFTER] = TestInfxConnection::getEnv(env_client);
		db    [AFTER] = TestInfxConnection::getEnv(env_db    );
	}

private:
	const char * const env_client;
	const char * const val_client;
	const char * const env_db;
	const char * const val_db;

	EnvFix & operator = (const EnvFix &);
};

//-----------------------------------------------------------------
TEST_FIXTURE(EnvFix, test_restore_infx_environment_west)
{
	connect(TestConnection::INFX);

	CHECK_EQUAL(client[BEFORE], client[AFTER]);
	CHECK_EQUAL(db    [BEFORE], db    [AFTER]);
}

//-----------------------------------------------------------------
TEST_FIXTURE(EnvFix, test_restore_infx_environment_cyr)
{
	connect(TestConnection::INFX_BG);

	CHECK_EQUAL(client[BEFORE], client[AFTER]);
	CHECK_EQUAL(db    [BEFORE], db    [AFTER]);
}

//-----------------------------------------------------------------
}	// SUITE

/*
* @file testconnection.cpp                                                                        
* @author Anke Klink                      
* @date 2007                            
*/    

#include "libbasardbsql.h" //first one, because it's the precompiled header - vs2010 issues warning (and would skip libbasarcmnutil.h)
#include "libbasarcmnutil.h"
                                    

#include "testconnection.h"
#include "testodbcconnection.h"
#include "testinfxconnection.h"
#include "testcicsconnection.h"
#include "testmssqlodbcconnection.h"

using basar::db::sql::ConnectionRef;
using basar::test::unit::dbsql::TestInfxConnection;
using basar::test::unit::dbsql::TestODBCConnection;
using basar::test::unit::dbsql::TestCICSConnection;

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{

				// -------------------------------------------------------------------------
				ConnectionRef TestConnection::createDefaultTestConnection()
				{
					return TestInfxConnection::createDefaultConnectionRef();
				}
				
				// -------------------------------------------------------------------------
				ConnectionRef  TestConnection::createTestConnection(TestConnection::TestConnectionType type)
				{
					ConnectionRef retval;
					switch (type)
					{
					case INFX:
						retval =  TestInfxConnection::createDefaultConnectionRef();
						break;
					case INFX_CONCURRENT_TRANSACTION:
						retval =  TestInfxConnection::createDefaultConnectionRef(true);
						break;
					case INFX_BG:
						retval =  TestInfxConnection::createBgConnectionRef();
						break;
					case INFX_CONCURRENT_TRANSACTION_BG:
						retval =  TestInfxConnection::createBgConnectionRef(true);
						break;

					case ODBC:
						retval = TestODBCConnection::createDefaultUserConnectionRef();
						break;
					case ODBC_BG:
						retval = TestODBCConnection::createBgConnectionRef();
						break;

                    case MSSQL_ODBC:
                        retval = TestMSSqlODBCConnection::createDefaultUserConnectionRef();
                        break;

					case CICS:
						retval = TestCICSConnection::createDefaultUserConnectionRef();
						break;
					case CICS_SLOT_0:
						retval = TestCICSConnection::createDefaultConnectionRefBySlot(0);
						break;

					case CICS_OPENED:
						retval = TestCICSConnection::createConnectionRef(CICS_CONFIG_WEST);
						break;
					case CICS_SLOT_0_OPENED:
						retval = TestCICSConnection::createConnectionRefBySlot(0);
						break;

					case CICS_BG_OPENED:
						retval = TestCICSConnection::createConnectionRef(CICS_CONFIG_CYR);
						break;

					default:
						retval =  createDefaultTestConnection();
					};
					return retval;

				}
				// -------------------------------------------------------------------------

				TestConnection::TestConnectionMap TestConnection::createTestConnections(const TestTypes & vec)
				{
					TestConnectionMap  retval;
					TestTypes::const_iterator iter;

					for (iter = vec.begin(); iter != vec.end(); ++iter)
					{
						retval.insert(TestConnectionMap::value_type(*iter, createTestConnection(*iter)));
					}

					return retval;
				}

				// -------------------------------------------------------------------------

				TestConnection::TestConnectionMap TestConnection::createDefaultTestConnections()
				{
					TestConnectionMap  retval;

					retval.insert(TestConnectionMap::value_type(INFX_BG, createTestConnection(INFX_BG)));
#ifdef HAS_ODBC
					retval.insert(TestConnectionMap::value_type(ODBC   , createTestConnection(ODBC   )));
					retval.insert(TestConnectionMap::value_type(ODBC_BG, createTestConnection(ODBC_BG)));
					
					retval.insert(TestConnectionMap::value_type(MSSQL_ODBC, createTestConnection(MSSQL_ODBC)));
#endif
					retval.insert(TestConnectionMap::value_type(INFX, createTestConnection(INFX)));

					return retval;
				}
				// -------------------------------------------------------------------------

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

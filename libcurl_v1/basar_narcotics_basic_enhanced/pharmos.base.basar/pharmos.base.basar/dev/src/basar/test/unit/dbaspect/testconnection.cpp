/*
* @file                                                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "libbasardbsql_databaseinfo.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbaspect
			{
				// ---------------------------------------------------------------------------------

				basar::db::aspect::ConnectionRef TestConnection::createDefaultConnectionRef()
				{
					return createTestConnection(INFX);
				}

				basar::db::aspect::ConnectionRef TestConnection::createTestConnection(TestConnectionType eType)
				{
					basar::db::aspect::ConnectionRef  connRef;
					try
					{
						basar::db::aspect::ConnectionRef::DatabaseInfo  dbInfo;

						switch (eType)
						{
						case CICS:
							dbInfo.setCICS(CICS_CONFIG_WEST, CICS_HOST, CICS_PORT, CICS_START, CICS_USER, CICS_PWD);
							break;

						case ODBC:
							dbInfo.setODBC(ODBC_CONFIG_WEST);
							break;

						case ODBC_BG:
							dbInfo.setODBC(ODBC_CONFIG_CYR);
							break;

						case INFX_CONCURR_TRANSACT_BG:
							dbInfo.setInfxConcurrent(INFX_DBSERVER, INFX_DB_CYR);
							break;

						case INFX_BG:
							dbInfo.setInfx(INFX_DBSERVER, INFX_DB_CYR);
							break;

						case INFX_CONCURR_TRANSACT:
							dbInfo.setInfxConcurrent(INFX_DBSERVER, INFX_DB_WEST);
							break;

						case INFX:
						default:
							dbInfo.setInfx(INFX_DBSERVER, INFX_DB_WEST);
							break;
						}

						connRef =  basar::db::aspect::Manager::getInstance().createConnect( dbInfo );
					}
					catch (basar::Exception & ex)
					{
						std::cout << ex.what() << std::endl;
						throw;
					}
					return connRef;
				}

				// -------------------------------------------------------------------------

				TestConnection::TestConnectionMap TestConnection::createTestConnections()
				{
					TestConnectionMap  retval;

					retval[INFX_BG] = createTestConnection(INFX_BG);
#ifdef HAS_ODBC
					retval[ODBC   ] = createTestConnection(ODBC   );
					retval[ODBC_BG] = createTestConnection(ODBC_BG);
#endif
					retval[INFX   ] = createTestConnection(INFX   );

					return retval;
				}
				// ---------------------------------------------------------------------------------

			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

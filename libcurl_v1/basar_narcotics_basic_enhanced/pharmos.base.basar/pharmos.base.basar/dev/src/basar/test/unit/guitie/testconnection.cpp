/** $Id$                                          
*                                                  
* @file                                                 
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    
#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "testconnection.h"

#include "libbasardbsql_databaseinfo.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_manager.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
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
						case INFX_CONCURR_TRANSACT:
							dbInfo.setInfxConcurrent(INFX_DBSERVER, INFX_DB, INFX_USER, INFX_PWD);
							break;

						case INFX:
						default:
							dbInfo.setInfx(INFX_DBSERVER, INFX_DB, INFX_USER, INFX_PWD);
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
				// ---------------------------------------------------------------------------------

			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

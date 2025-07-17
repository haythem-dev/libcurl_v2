/** $Id$                                          
*                                                  
* @file utdbsqlhelper.cpp                                                  
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasardbsql_manager.h"

#include <stdlib.h>
#include "testinfxconnection.h"
#include "testconnection.h"

#ifdef _WIN32
#include <sqlhdr.h>
#endif

using basar::db::sql::ConnectionRef;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::Manager;
using basar::db::sql::Connection;


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{
				// ---------------------------------------------------------------------------------

				basar::db::sql::DatabaseInfo TestInfxConnection::getInvalidDataBaseInfo()
				{
					DatabaseInfo  dbInfo;
					dbInfo.setInfx(ESQLC_DBSERVER_INVALID, "esistwarm21");

					return dbInfo;
				}


				basar::db::sql::DatabaseInfo TestInfxConnection::getDefaultDataBaseInfo()
				{
					DatabaseInfo  dbInfo;
					dbInfo.setInfx(ESQLC_DBSERVER, ESQLC_DB_WEST);

					return dbInfo;
				}
				// ---------------------------------------------------------------------------------

				basar::db::sql::DatabaseInfo TestInfxConnection::getBgDataBaseInfo()
				{
					DatabaseInfo  dbInfo;
					dbInfo.setInfx(ESQLC_DBSERVER, ESQLC_DB_CYR);

					return dbInfo;
				}
				// ---------------------------------------------------------------------------------

				DatabaseInfo TestInfxConnection::getDefaultDataBaseInfo(
					const char * const user,
					const char * const passwd, 
					bool               concurrentTransaction)
				{
					DatabaseInfo  dbInfo = getDefaultDataBaseInfo();
					dbInfo.user               = user;
					dbInfo.passwd             = passwd;
					dbInfo.concurrTransaction = concurrentTransaction;

					return dbInfo;
				}

				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestInfxConnection::createNewConnectionRef (
					const char * const user,
					const char * const passwd,
					bool               concurrentTransaction)
				{
					DatabaseInfo  dbInfo = getDefaultDataBaseInfo(user, passwd, concurrentTransaction);

#ifdef USE_PRINT_INFO
					printf("\n\n");
					printf("--- test_conn infx: ---------------------------------\n");
					printf("\n");
					printf("connect :\n");
					printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
					printf("  database : %s\n", dbInfo.database.c_str());
					printf("  user     : %s\n", dbInfo.user    .c_str());
					printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
					printf("  conn ID  : %d\n", dbInfo.connectionId);
					printf("  conn type: %d\n", dbInfo.eConnectType);
					printf("\n");
#endif
					ConnectionRef connRef = Manager::getInstance().createConnect(dbInfo);
					return connRef;
				}

				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestInfxConnection::createInvalidConnectionRef(bool concurrentTransaction)
				{
					DatabaseInfo dbinfo = getInvalidDataBaseInfo();
					dbinfo.concurrTransaction = concurrentTransaction;

					//this should throw
					return Manager::getInstance().createConnect(dbinfo);
				}


				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestInfxConnection::createDefaultConnectionRef(bool concurrentTransaction)
				{
					return createNewConnectionRef("", "", concurrentTransaction);		//infx
				}
				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestInfxConnection::createBgConnectionRef(bool concurrentTransaction)
				{
					DatabaseInfo dbinfo = getBgDataBaseInfo();
					dbinfo.concurrTransaction = concurrentTransaction;

					return Manager::getInstance().createConnect(dbinfo);
				}

				// ---------------------------------------------------------------------------------
				ConstBuffer TestInfxConnection::getEnv(const char * const env)
				{
					static ConstString empty = "";
					char             * pBuf  = const_cast <char *> (env);
					const char       * pRet;

				#ifdef _WIN32
					pRet = ifx_getenv(pBuf);
				#else
					pRet = getenv(pBuf);
				#endif

					return pRet ? pRet : empty;
				}

				// ---------------------------------------------------------------------------------
				int TestInfxConnection::putEnv(const char * const assign)
				{
					char * pBuf = const_cast <char *> (assign);

				#ifdef _WIN32
					return ifx_putenv(pBuf);
				#else
					return putenv(pBuf);
				#endif
				}
			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

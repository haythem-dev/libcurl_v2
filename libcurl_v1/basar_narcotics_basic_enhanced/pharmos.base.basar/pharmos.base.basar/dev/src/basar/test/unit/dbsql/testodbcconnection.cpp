/*
* @file utdbsqlhelper.cpp                                                  
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasardbsql_manager.h"


#include "testodbcconnection.h"
#include "testconnection.h"

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

				basar::db::sql::DatabaseInfo TestODBCConnection::getDefaultDataBaseInfo()
				{
					DatabaseInfo  dbInfo;

					dbInfo.setODBC(ODBC_DSN_WEST);

					return dbInfo;
				} 

				// ---------------------------------------------------------------------------------

				basar::db::sql::DatabaseInfo TestODBCConnection::getBgDataBaseInfo()
				{
					DatabaseInfo  dbInfo;
					dbInfo.setODBC(ODBC_DSN_CYR);

					return dbInfo;
				} 

				// ---------------------------------------------------------------------------------

				DatabaseInfo TestODBCConnection::getDefaultDataBaseInfo(
					const char * const user,
					const char * const passwd)
				{
					DatabaseInfo  dbInfo = getDefaultDataBaseInfo();
					dbInfo.user     = user;
					dbInfo.passwd   = passwd; 
					return dbInfo;
				}

				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestODBCConnection::createNewConnectionRef (
					const char * const user,
					const char * const passwd)
				{
					DatabaseInfo  dbInfo = TestODBCConnection::getDefaultDataBaseInfo(user, passwd);

#ifdef USE_PRINT_INFO
					printf("\n\n");
					printf("--- test_conn odbc: ---------------------------------\n");
					printf("\n");
					printf("connect :\n");
					printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
					printf("  database : %s\n", dbInfo.database.c_str());
					printf("  user     : %s\n", dbInfo.user    .c_str());
					printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
					printf("\n");
#endif
					return Manager::getInstance().createConnect(dbInfo);
				}
				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestODBCConnection::createDefaultUserConnectionRef()
				{
					return  createNewConnectionRef("", "");	
				}

				// ---------------------------------------------------------------------------------

				basar::db::sql::ConnectionRef TestODBCConnection::createBgConnectionRef()
				{
					DatabaseInfo dbinfo = getBgDataBaseInfo();

					return Manager::getInstance().createConnect(dbinfo);
				}
				// ---------------------------------------------------------------------------------

			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

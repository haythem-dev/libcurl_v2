/*
* @file utdbsqlhelper.cpp                                                  
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasardbsql_manager.h"

#include "testconnection.h"
#include "testcicsconnection.h"

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

				DatabaseInfo TestCICSConnection::getSlotDataBaseInfo(
					Int32 slotid,
					ConstString user,
					ConstString passwd)
				{
					DatabaseInfo dbInfo;
					dbInfo.eConnectType		= basar::CICS;	
					dbInfo.connectionId		= slotid;
					dbInfo.user				= user;
					dbInfo.passwd			= passwd; 
					return dbInfo;
				}

				// ---------------------------------------------------------------------------------

				DatabaseInfo TestCICSConnection::getDefaultDataBaseInfo(
					ConstString config,
					ConstString user,
					ConstString passwd)
				{
					DatabaseInfo dbInfo;
					dbInfo.setCICS(config, CICS_HOST, CICS_PORT, CICS_START, user, passwd);

					return dbInfo;
				}

				// ---------------------------------------------------------------------------------
				ConnectionRef TestCICSConnection::createNewConnectionRef (
					const DatabaseInfo &dbInfo)
				{
#ifdef USE_PRINT_INFO
					printf("\n\n");
					printf("--- test_conn CICS: ---------------------------------\n");
					printf("\n");
					printf("connect :\n");
					printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
					printf("  database : %s\n", dbInfo.database.c_str());
					printf("  user     : %s\n", dbInfo.user    .c_str());
					printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
					printf("\n");
#endif
					return Manager::getInstance().createConnect(dbInfo, false);
				}
				// ---------------------------------------------------------------------------------

				ConnectionRef TestCICSConnection::createNewConnectionRef (
					ConstString user,
					ConstString passwd)
				{

					return createNewConnectionRef( TestCICSConnection::getDefaultDataBaseInfo(CICS_CONFIG_WEST, user, passwd));

				}
				// ---------------------------------------------------------------------------------

				ConnectionRef TestCICSConnection::createNewConnectionRefBySlot (
					Int32 slotid,
					ConstString user,
					ConstString passwd)
				{
					return createNewConnectionRef( TestCICSConnection::getSlotDataBaseInfo(slotid, user, passwd));
				}
				// ---------------------------------------------------------------------------------

				ConnectionRef TestCICSConnection::createDefaultUserConnectionRef()
				{
					return TestCICSConnection::createNewConnectionRef(CICS_USER, CICS_PWD);	
				}
				// ---------------------------------------------------------------------------------
				ConnectionRef TestCICSConnection::createDefaultConnectionRef()
				{
					return createDefaultUserConnectionRef();
				}

				// ---------------------------------------------------------------------------------
				ConnectionRef  TestCICSConnection::createDefaultConnectionRefBySlot(Int32 slotid)
				{
					return createNewConnectionRefBySlot(slotid, CICS_USER, CICS_PWD);	
				}

				// ---------------------------------------------------------------------------------
				ConnectionRef TestCICSConnection::createConnectionRef(ConstString config)
				{
					return Manager::getInstance().createConnect(getDefaultDataBaseInfo(config, CICS_USER, CICS_PWD));
				}
				
				// ---------------------------------------------------------------------------------
				ConnectionRef  TestCICSConnection::createConnectionRefBySlot(Int32 slotid)
				{
					return Manager::getInstance().createConnect(getSlotDataBaseInfo(slotid, CICS_USER, CICS_PWD));
				}


				// ---------------------------------------------------------------------------------
			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

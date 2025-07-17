/*
* @file
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTODBCCONNECTION_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTODBCCONNECTION_H__ 

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{
				/*!
				* Testclass that creates a ODBC Connection,
				* with the default ODBC Database / User and Passwd
				* informations
				*/
				class TestODBCConnection
				{
				public:

					//! default DatabaseInfo
					static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo();

					//! Bulgarian DatabaseInfo
					static basar::db::sql::DatabaseInfo getBgDataBaseInfo();

					//! default DatabaseInfo with user/password
					static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo(
						const char * const user,
						const char * const password);

					//! returns a ConnectionRef Object with DatabaseInfo with user/password
					static basar::db::sql::ConnectionRef createNewConnectionRef (
						const char * const user,
						const char * const password);

					//! returns a ConnectionRef Object with default DatabaseInfo
					static basar::db::sql::ConnectionRef createDefaultUserConnectionRef();

					//! returns a ConnectionRef Object with default DatabaseInfo
					static basar::db::sql::ConnectionRef createBgConnectionRef();

				};		

			}
		}
	}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

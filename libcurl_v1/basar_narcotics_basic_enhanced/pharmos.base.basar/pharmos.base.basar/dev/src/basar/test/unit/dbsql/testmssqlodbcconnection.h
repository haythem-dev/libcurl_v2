

#ifndef BASAR_TEST_UNIT_DBSQL_TESTMSSQLODBCCONNECTION_H
#define BASAR_TEST_UNIT_DBSQL_TESTMSSQLODBCCONNECTION_H



namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace dbsql
            {

/*!
* Testclass that creates a ODBC Connection to a MSSql server
*/
                class TestMSSqlODBCConnection
                {
				public:

					//! default DatabaseInfo
					static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo();

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
				};	

            } // end namespace dbsql
        } // end namespace unit
    } // end namespace test
} // end namespace basar

#endif //BASAR_TEST_UNIT_DBSQL_TESTMSSQLODBCCONNECTION_H
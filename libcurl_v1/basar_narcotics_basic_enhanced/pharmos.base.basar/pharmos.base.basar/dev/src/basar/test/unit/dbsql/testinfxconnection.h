/*
* @file
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTINFXCONNECTION_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTINFXCONNECTION_H__ 

namespace basar
{
namespace test
{
namespace unit
{
namespace dbsql
{

/*!
* Testclass that creates a Infx Connection,
* with the default Infx Database / User and Passwd
* informations
*/
class TestInfxConnection
{
public:

	//! invalid DatabaseInfo
	static basar::db::sql::DatabaseInfo getInvalidDataBaseInfo();

	//! default DatabaseInfo
	static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo();
	
	//! Bulgarian DatabaseInfo
	static basar::db::sql::DatabaseInfo getBgDataBaseInfo();

	//! default DatabaseInfo with user/password
	static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo(
		const char * const user,
		const char * const password,
		bool               concurrentTransaction);

	//! returns a ConnectionRef Object with DatabaseInfo with user/password
	static basar::db::sql::ConnectionRef createNewConnectionRef (
		const char * const user,
		const char * const password,
		bool               concurrentTransaction);

	static basar::db::sql::ConnectionRef createInvalidConnectionRef(bool concurrentTransaction = false);

	//! returns a ConnectionRef Object with default DatabaseInfo 
	static basar::db::sql::ConnectionRef createDefaultConnectionRef(
		bool concurrentTransaction = false);

	//! returns a ConnectionRef Object with Bulgarian DatabaseInfo 
	static basar::db::sql::ConnectionRef createBgConnectionRef(
		bool concurrentTransaction = false);

	static ConstBuffer getEnv(const char * const env   );
	static int         putEnv(const char * const assign);
};				

}
}
}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

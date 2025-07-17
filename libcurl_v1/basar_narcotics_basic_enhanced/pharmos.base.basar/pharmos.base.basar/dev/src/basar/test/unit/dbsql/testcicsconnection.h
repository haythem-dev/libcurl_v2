/*
* @file
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTCICSCONNECTION_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_TESTCICSCONNECTION_H__ 

 
namespace basar
{
namespace test
{
namespace unit
{
namespace dbsql
{

/*!
* Testclass that creates a CICS Connection,
* with the default CICS Database / User and Passwd
* informations
*/
class TestCICSConnection
{
public:

	//! returns the Database Info for a special solt number
	static basar::db::sql::DatabaseInfo getSlotDataBaseInfo(
		Int32 slotid, 
		ConstString user  ,
		ConstString password  );

	//! returns the default Database Info named "BasarUnitTest"
	static basar::db::sql::DatabaseInfo getDefaultDataBaseInfo(
		ConstString config,
		ConstString user,
		ConstString password );

	//! returns a new ConnectionRef with the default Database Info
	static basar::db::sql::ConnectionRef createNewConnectionRef (
		ConstString user,
		ConstString password);

	//! returns a new ConnectionRef for a special solt number
	static basar::db::sql::ConnectionRef createNewConnectionRefBySlot (
		Int32 slotid  ,
		ConstString user  ,
		ConstString password );

	//! returns a new ConnectionRef for a DatabaseInfo
	static basar::db::sql::ConnectionRef createNewConnectionRef (
		const basar::db::sql::DatabaseInfo &);


	//! returns a new ConnectionRef with default Values 
	static basar::db::sql::ConnectionRef createDefaultUserConnectionRef();

	//! returns a new ConnectionRef with default Values 
	static basar::db::sql::ConnectionRef createDefaultConnectionRef();

	//! returns a new ConnectionRef with default Values for a given slot
	static basar::db::sql::ConnectionRef createDefaultConnectionRefBySlot(Int32 slotid = 0);

	//! returns a new opened ConnectionRef 
	static basar::db::sql::ConnectionRef createConnectionRef(ConstString config);

	//! returns a new opened ConnectionRef for a given slot
	static basar::db::sql::ConnectionRef createConnectionRefBySlot(Int32 slotid = 0);
};		

}
}
}
}

#endif 

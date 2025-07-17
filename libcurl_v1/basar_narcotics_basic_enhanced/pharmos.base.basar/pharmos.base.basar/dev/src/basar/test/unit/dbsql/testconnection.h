/*
* @file testconnection.h                                                                        
* @author Anke Klink                      
* @date 2007                            
*/    

#ifndef  ___BASAR_TEST_UNIT_DBSQL_TESTCONNECTION_H__ 
#define  ___BASAR_TEST_UNIT_DBSQL_TESTCONNECTION_H__ 

//---------------------------------------------------------------
#ifdef _WIN32
#define HAS_ODBC
#endif

#define HAS_CICS

//---------------------------------------------------------------
#define ESQLC_DBSERVER         "zdev21_tcp"
#define ESQLC_DBSERVER_INVALID "zdev21_tcp_invalid"
#define ESQLC_DB_WEST          "but21"
#define ESQLC_DB_CYR           "but21bg"

#ifdef _WIN64
#define ODBC_DSN_WEST          "zdev21_but21_x64"
#define ODBC_DSN_CYR           "zdev21_but21bg_x64"
#else
#define ODBC_DSN_WEST          "zdev21_but21_VS2010"
#define ODBC_DSN_CYR           "zdev21_but21bg_VS2010"
#endif

#define CICS_CONFIG_TEST		"BasarUnitTest"
#define CICS_CONFIG_TEST_CYR	"BasarUnitTest"

#define CICS_CONFIG_TIMEOUT     "BasarUnitTest.mintimeout"

#define CICS_CONFIG_WEST		"BasarDefault_west"
#define CICS_CONFIG_CYR			"BasarDefault_cyr"
#define CICS_HOST				"129.210.15.2"
#define CICS_PORT				"3102"
#define CICS_START				"TCAS"
#define CICS_USER				""
#define CICS_PWD				""
#define CICS_SESSION_NAME		"A02CICS2"

//---------------------------------------------------------------
namespace basar {
namespace test  {
namespace unit  {
namespace dbsql {

//! create new tailored connection Ref Object
class TestConnection
{
public:
	//! name of the Connection type
	enum TestConnectionType
	{
		INFX						  ,		//!< default INFX connection 
		INFX_CONCURRENT_TRANSACTION	  ,		//!< INFX connection with concurrent transaction
		INFX_BG						  ,		//!< INFX connection to Bulgarian DB
		INFX_CONCURRENT_TRANSACTION_BG,		//!< INFX connection to Bulgarian DB with concurrent transaction

		ODBC						  ,		//!< default ODBC Connection 
		ODBC_BG						  ,		//!< Bulgarian ODBC Connection 

        MSSQL_ODBC                    ,     //!< default MSSQL ODBC Connection
        
		// order is important: slot0 before CICS because CICS finds next free slot > 0 if 2 parallel connections are used
		CICS_SLOT_0					  ,		//!< CICS Connection on slot 0 (not yet opened)
		CICS_SLOT_0_OPENED			  ,		//!< opened CICS Connection on slot 0
		CICS						  ,		//!< default CICS Connection (not yet opened)
		CICS_OPENED                   ,		//!< default opened CICS Connection 
		CICS_BG_OPENED                      //!< Bulgarian mapped CICS Connection
	};

	//! map of ConnectionRef with TestConnectionType
	typedef std::multimap    <TestConnectionType, basar::db::sql::ConnectionRef> TestConnectionMap;

	typedef std::vector <TestConnectionType >                               TestTypes;

	//! returns one ConnectionRef tailored to the specifications of the given Type
	static  basar::db::sql::ConnectionRef createTestConnection(TestConnectionType);

	//! returns the default  ConnectionRef (INFX)
	static  basar::db::sql::ConnectionRef createDefaultTestConnection();

	//! returns a map of ConnectionRef Objects tailored to the specifications of the given Types
	static  TestConnectionMap createTestConnections(const TestTypes &);

	//! returns a default map of ConnectionRef Objects (INFX, INFX_BG, ODBC, ODBC_BG) on windows and only (INFX, INFX_BG) on unix 
	static TestConnectionMap createDefaultTestConnections();
};

}}}}    // namespaces

#endif  // GUARD

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

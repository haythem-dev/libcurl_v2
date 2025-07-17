/** $Id$                                          
*                                                  
* @file TestConnection.h                                                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBASPECT_TESTCONNECTION_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBASPECT_TESTCONNECTION_H__ 

 
// ---------------------------------------------------------------------------------
#ifdef _WIN32
#define HAS_ODBC
#endif

#define HAS_CICS

// ---------------------------------------------------------------------------------
#define INFX_DBSERVER		"zdev21_tcp"
#define INFX_DB_WEST		"but21"
#define INFX_DB_CYR			"but21bg"

#ifdef _WIN64
#define ODBC_CONFIG_WEST	"zdev21_but21_x64"
#define ODBC_CONFIG_CYR		"zdev21_but21bg_x64"
#else
#define ODBC_CONFIG_WEST	"zdev21_but21_VS2010"
#define ODBC_CONFIG_CYR		"zdev21_but21bg_VS2010"
#endif

#define CICS_CONFIG_WEST	"BasarDefault_west"
#define CICS_CONFIG_CYR		"BasarDefault_cyr"
#define CICS_HOST			"129.210.15.2"
#define CICS_PORT			"3102"
#define CICS_START			"TCAS"
#define CICS_USER			""
#define CICS_PWD			""

// ---------------------------------------------------------------------------------
namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbaspect
			{
				/**
				* Testclass that creates a Infx Connection,
				* with the default Infx Database / User and Passwd
				* informations
				*/
				class TestConnection
				{
				public:
					enum TestConnectionType
					{
						INFX,						//!< default INFX Connection 
						INFX_BG,					//!< Bulgarian INFX Connection 
						INFX_CONCURR_TRANSACT,		//!< INFX connection with concurrent transaction
						INFX_CONCURR_TRANSACT_BG,	//!< Bulgarian INFX connection with concurrent transaction
						ODBC,						//!< default ODBC Connection 
						ODBC_BG,					//!< Bulgarian ODBC Connection 
						CICS						//!< default CICS Connection 
					};

					typedef std::map <TestConnectionType, basar::db::aspect::ConnectionRef> TestConnectionMap;


					//! returns a Connection (INFX)
					static basar::db::aspect::ConnectionRef createDefaultConnectionRef();

					//! returns one ConnectionRef tailored to the specifications of the given Type
					static basar::db::aspect::ConnectionRef createTestConnection(TestConnectionType);

					//! returns map of ConnectionRef Objects (INFX, INFX_BG, ODBC, ODBC_BG) on windows and only (INFX, INFX_BG) on unix 
					static TestConnectionMap createTestConnections();
				};				
			}
		}
	}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

/** $Id$                                          
*                                                  
* @file TestConnection.h                                                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBASPECT_TESTCONNECTION_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBASPECT_TESTCONNECTION_H__ 


#define INFX_DBSERVER	"zdev21_tcp"
#define INFX_DB			"but21"
#define INFX_USER		""
#define INFX_PWD		""

 
namespace basar
{
	namespace db {
		namespace aspect {
			class ConnectionRef;
		}
	}

namespace test
{
namespace unit
{
namespace guitie
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
		INFX,					//!< default INFX Connection 
		INFX_CONCURR_TRANSACT	//!< INFX connection with concurrent transaction
	};

	//! returns a Connection (INFX)
	static basar::db::aspect::ConnectionRef createDefaultConnectionRef();

	//! returns one ConnectionRef tailored to the specifications of the given Type
	static basar::db::aspect::ConnectionRef createTestConnection(TestConnectionType);
};				

}
}
}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

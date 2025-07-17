//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGGERPOOL_LIBBASARLOGIN_H
#define GUARD_LOGGERPOOL_LIBBASARLOGIN_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//---------------------------------------------------------------------------
/*! \brief class for initialized logger objects
	\n final class
	\n throws no exceptions */
class LoggerPool
{
public:	
	static log4cplus::Logger &	loginAppl();		//!< logger for LoginAppl
	static log4cplus::Logger &	loginData();		//!< logger for Login Data
	
	static log4cplus::Logger &	loginConn();	    //!< logger for Login Connection
	static log4cplus::Logger &	checkLogin();		//!< logger for CheckLogin
	static log4cplus::Logger &	loginCrypt();		//!< logger for crypt password
	
	static log4cplus::Logger &	baseUserManagement();	//!< logger for Login Base User Management
	static log4cplus::Logger &	baseApplManagement();	//!< logger for Login Base Appication Management

	static log4cplus::Logger &	userManagement();	//!< logger for Login User Management
	static log4cplus::Logger &	applManagement();	//!< logger for Login Appication Management
	static log4cplus::Logger &	textManagement();	//!< logger for Login Text Management

	static log4cplus::Logger &	adLoginUserMgmt();  //!< logger for Login User Management With AD
	static log4cplus::Logger &	adLoginApplMgmt();  //!< logger for Login Application Management With AD
};

//----------------------------------------------------------------------------
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD

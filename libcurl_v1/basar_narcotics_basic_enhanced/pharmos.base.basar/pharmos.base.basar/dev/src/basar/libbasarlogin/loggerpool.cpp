//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login {

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//---------------------------------------------------------------------------
//! \brief get logging instance for LoginAppl
//! \return logging instance for LoginAppl
Logger & LoggerPool::loginAppl()
{
	static Logger log = Logging::getInstance("basar.login.LoginAppl");
	return log;
}

//! \brief get logging instance for LoginData
//! \return logging instance for LoginData
Logger & LoggerPool::loginData()
{
	static Logger log = Logging::getInstance("basar.login.LoginData");
	return log;
}

//! \brief get logging instance for LoginConn
//! \return logging instance for LoginConn
Logger & LoggerPool::loginConn()
{
	static Logger log = Logging::getInstance("basar.login.Connection");
	return log;
}

//! \brief get logging instance for CheckLogin
//! \return logging instance for CheckLogin
Logger & LoggerPool::checkLogin()
{
	static Logger log = Logging::getInstance("basar.login.CheckLogin");
	return log;
}

//! \brief get logging instance for Crypt
//! \return logging instance for Crypt
Logger & LoggerPool::loginCrypt()
{
	static Logger log = Logging::getInstance("basar.login.Crypt");
	return log;
}

//! \brief get logging instance for BaseUserManagement
//! \return logging instance for BaseUserManagement
Logger & LoggerPool::baseUserManagement()
{
	static Logger log = Logging::getInstance("basar.login.BaseUserManagement");
	return log;
}

//! \brief get logging instance for UserManagement
//! \return logging instance for UserManagement
Logger & LoggerPool::userManagement()
{
	static Logger log = Logging::getInstance("basar.login.UserManagement");
	return log;
}

//! \brief get logging instance for BaseApplManagement
//! \return logging instance for BaseApplManagement
Logger & LoggerPool::baseApplManagement()
{
	static Logger log = Logging::getInstance("basar.login.BaseApplManagement");
	return log;
}

//! \brief get logging instance for ApplManagement
//! \return logging instance for ApplManagement
Logger & LoggerPool::applManagement()
{
	static Logger log = Logging::getInstance("basar.login.ApplManagement");
	return log;
}

//! \brief get logging instance for TextManagement
//! \return logging instance for TextManagement
Logger & LoggerPool::textManagement()
{
	static Logger log = Logging::getInstance("basar.login.TextManagement");
	return log;
}

//! \brief get logging instance for ADLoginUserManagement
//! \return logging instance for ADLoginUserManagement
Logger & LoggerPool::adLoginUserMgmt()
{
	static Logger log = Logging::getInstance("basar.login.ADLoginUserMgmt");
	return log;
}

//! \brief get logging instance for ADLoginApplManagement
//! \return logging instance for ADLoginApplManagement
Logger & LoggerPool::adLoginApplMgmt()
{
	static Logger log = Logging::getInstance("basar.login.ADLoginApplMgmt");
	return log;
}

//----------------------------------------------------------------------------
}//login
}//basar

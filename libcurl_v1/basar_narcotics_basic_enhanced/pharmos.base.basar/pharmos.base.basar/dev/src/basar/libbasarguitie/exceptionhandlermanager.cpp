/** $Id$
*
* @file
* @author Anke Klink
* @date 2009
*/

#include "libbasarguitie_exceptionhandlermanager.h"

#include "loggerpool.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

// ----------------------------------------------------------
ExceptionHandlerManager::ExceptionHandlerManager( )
{
	m_exceptionsHandled = 0;
	setDefaultRetval(true, true, true);
}
// ----------------------------------------------------------
ExceptionHandlerManager::~ExceptionHandlerManager( )
{ 
}
// ----------------------------------------------------------
void ExceptionHandlerManager::setExceptionHandlerRef(const ExceptionHandlerRef & e)
{
	m_exceptionHandler = e;
}
// ----------------------------------------------------------
const ExceptionHandlerRef & ExceptionHandlerManager::getExceptionHandlerRef()  const
{
	return m_exceptionHandler;
}
// ----------------------------------------------------------

void ExceptionHandlerManager::setDefaultRetval(bool a, bool b, bool c)
{
	m_defaultRetvalBasarException = a;
	m_defaultRetvalSTDException = b;
	m_defaultRetvalUnknownException = c;	
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::setDefaultRetval to ( %s, %s, %s)",
			(a) ? "true" : "false",
			(b) ? "true" : "false",
			(c) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}
// ---------------------------------------------------------------------------------------
void ExceptionHandlerManager::setDefaultRetvalBasarException(bool  a)
{
	m_defaultRetvalBasarException = a; 
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::setDefaultRetvalBasarException to  %s ",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}
// ---------------------------------------------------------------------------------------
void ExceptionHandlerManager::setDefaultRetvalSTDException(bool a )
{
	m_defaultRetvalSTDException = a; 
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::setDefaultRetvalSTDException to  %s ",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}
// ---------------------------------------------------------------------------------------
void ExceptionHandlerManager::setDefaultRetvalUnknownException(bool  a)
{
	m_defaultRetvalUnknownException = a;
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::setDefaultRetvalUnknownException to  %s ",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}
// ---------------------------------------------------------------------------------------

bool ExceptionHandlerManager::getDefaultRetvalBasarException( )
{
	return m_defaultRetvalBasarException;
}
// ---------------------------------------------------------------------------------------
bool ExceptionHandlerManager::getDefaultRetvalSTDException( )
{
	return m_defaultRetvalSTDException;
}
// ---------------------------------------------------------------------------------------
bool ExceptionHandlerManager::getDefaultRetvalUnknownException( )
{
	return m_defaultRetvalUnknownException;
}
// ----------------------------------------------------------

basar::VarString ExceptionHandlerManager::writeFoundInfo(const ExceptionFoundInfo & found)  const
{
	basar::VarString retval;
	retval.format("found in: context (%s) , file (%s) line (%d)",  
		found.context      .c_str(), 
		found.file         .c_str(),
		found.line);
	return retval;
}
// ----------------------------------------------------------

bool ExceptionHandlerManager::handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool a) 
{
	m_exceptionsHandled++;
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::WARN_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleException  for BasarException: (%s) found in %s)",
			ex.what().c_str(),
			writeFoundInfo(found).c_str());
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::WARN_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	bool retval = m_exceptionHandler.handleException(ex, found, a);
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleException  for BasarException: returns %s (default=%s))",
			(retval) ? "true" : "false",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	return retval;
}
// ----------------------------------------------------------
bool ExceptionHandlerManager::handleException(const std::exception& ex, const ExceptionFoundInfo & found, bool a)  
{
	m_exceptionsHandled++;
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::WARN_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleException  for std::exception : (%s) found in %s)",
			ex.what(),
			writeFoundInfo(found).c_str());
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::WARN_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	bool retval = m_exceptionHandler.handleException(ex, found, a);
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleException  for std::exception: returns %s (default=%s))",
			(retval) ? "true" : "false",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	} 
	return retval;
}
// ----------------------------------------------------------
bool ExceptionHandlerManager::handleUnknownException(const ExceptionFoundInfo & found, bool a)   
{
	m_exceptionsHandled++;
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::WARN_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleUnknownException for unknown Exception found in %s)",
			writeFoundInfo(found).c_str());
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::WARN_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	bool retval = m_exceptionHandler.handleUnknownException( found,a );
	if (LoggerPool::exceptionHandlerManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format(" ExceptionHandlerManager::handleUnknownException : returns %s (default=%s))",
			(retval) ? "true" : "false",
			(a) ? "true" : "false");
		LoggerPool::exceptionHandlerManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	return retval;
}
// ---------------------------------------------------------------------------------------
bool ExceptionHandlerManager::handleException(const basar::Exception & ex, const ExceptionFoundInfo & foundInfo)  
{  
	return handleException(ex, foundInfo, m_defaultRetvalBasarException);
}
// ----------------------------------------------------------

bool ExceptionHandlerManager::handleException(const std::exception& ex, const ExceptionFoundInfo & foundInfo)  
{ 
	return handleException(ex, foundInfo, m_defaultRetvalSTDException);
}
// ----------------------------------------------------------
bool ExceptionHandlerManager::handleUnknownException(const ExceptionFoundInfo & foundInfo)   
{
	return  handleUnknownException( foundInfo,m_defaultRetvalUnknownException );
}
// ---------------------------------------------------------------------------------------
basar::Int32 ExceptionHandlerManager::getNumberOfExceptionHandled() const
{
	return m_exceptionsHandled;
}
// ---------------------------------------------------------------------------------------

void ExceptionHandlerManager::resetNumberOfExceptionHandled()
{
	m_exceptionsHandled = 0;
}
// ---------------------------------------------------------------------------------------
bool ExceptionHandlerManager::checkNoExceptionHandled() const
{
	return (0 == m_exceptionsHandled);
}
// ---------------------------------------------------------------------------------------
}
}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_logging.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

using log4cplus::Logger;

//---------------------------------------------------------------------------

//! \brief get logging instance for Locale
//! \return instance for Locale
Logger & LoggerPool::locale()
{
	static Logger log = Logging::getInstance("basar.cmnutil.Locale");
	return log;
}

//! \brief get logging instance for LocaleSwitch
//! \return instance for LocaleSwitch
Logger & LoggerPool::localeSwitch()
{
	static Logger log = Logging::getInstance("basar.cmnutil.LocaleSwitch");
	return log;
}

//! \brief get logging instance for ParameterList
//! \return instance for ParameterList
Logger & LoggerPool::paramList()
{
	static Logger log = Logging::getInstance("basar.cmnutil.ParameterList");
	return log;
}

//! \brief get logging instance for PsInterpreter
//! \return instance for PsInterpreter
Logger & LoggerPool::psInterpreter()
{
	static Logger log = Logging::getInstance("basar.cmnutil.PsInterpreter");
	return log;
}


//! \brief get logging instance for print tools
//! \return instance for print tools
Logger & LoggerPool::printTools()
{
	static Logger log = Logging::getInstance("basar.cmnutil.PrintTools");
	return log;
}

//! \brief get logging instance for raw print
//! \return instance for raw print
Logger & LoggerPool::rawPrint()
{
	static Logger log = Logging::getInstance("basar.cmnutil.RawPrint");
	return log;
}

//! \brief get logging instance for lpr print
//! \return instance for lpr print
Logger & LoggerPool::lpr()
{
    static Logger log = Logging::getInstance("basar.cmnutil.Lpr");
	return log;
}

//! \brief get logging instance for lpr print data
//! \return instance for print data
Logger & LoggerPool::lprData()
{
    static Logger log = Logging::getInstance("basar.cmnutil.Lpr.Data");
	return log;
}

//! \brief get logging instance for PortPrint 
//! \return instance for PortPrint
Logger & LoggerPool::portPrint()
{
    static Logger log = Logging::getInstance("basar.cmnutil.PortPrint");
	return log;
}

//! \brief get logging instance for PortPrint data
//! \return instance for PortPrint data
Logger & LoggerPool::portPrintData()
{
	static Logger log = Logging::getInstance("basar.cmnutil.PortPrint.Data");
	return log;
}

//! \brief get logging instance for MailMessage
//! \return instance for MailMessage
Logger & LoggerPool::mailMsg()
{
    static Logger log = Logging::getInstance("basar.cmnutil.MailMessage");
	return log;
}

//! \brief get logging instance for SMTP
//! \return instance for SMTP
Logger & LoggerPool::smtp()
{
    static Logger log = Logging::getInstance("basar.cmnutil.Smtp");
	return log;
}

//! \brief get logging instance for SMTP data
//! \return instance for SMTP data
Logger & LoggerPool::smtpData()
{
    static Logger log = Logging::getInstance("basar.cmnutil.Smtp.Data");
	return log;
}


//! \brief get logging instance for tcp socket
//! \return instance for tcp socket
Logger & LoggerPool::tcpSocket()
{
    static Logger log = Logging::getInstance("basar.cmnutil.TcpSocket");
	return log;
}


//! \brief get logging instance for piped process
//! \return instance for piped process
Logger & LoggerPool::pipedProcess()
{
	static Logger log  = Logging::getInstance("basar.cmnutil.PipedProcess");
	return log;
}

//! \brief get logging instance for remote task
//! \return instance for remote task
Logger & LoggerPool::remoteTask()
{
	static Logger log  = Logging::getInstance("basar.cmnutil.RemoteTask");
	return log;
}


//! \brief get logging instance for configuration
//! \return instance for configuration
Logger & LoggerPool::config()
{
    static Logger log = Logging::getInstance("basar.cmnutil.Config");
	return log;
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

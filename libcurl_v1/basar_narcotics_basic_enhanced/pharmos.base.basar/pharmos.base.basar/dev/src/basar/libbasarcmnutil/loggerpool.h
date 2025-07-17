//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_LOGGERPOOL_H
#define GUARD_LIBBASARCMNUTIL_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	locale       ();	//!< logger for Locale
	static log4cplus::Logger &	localeSwitch ();	//!< logger for LocaleSwitch
	static log4cplus::Logger &	paramList    ();	//!< logger for ParameterList

	static log4cplus::Logger &	psInterpreter();	//!< logger for ps interpreter
	static log4cplus::Logger &	psInterpreterOL();	//!< logger for ps interpreter OL
	static log4cplus::Logger &	printTools   ();	//!< logger for print tools
	static log4cplus::Logger &	rawPrint     ();	//!< logger for raw print
	static log4cplus::Logger &	lpr          ();	//!< logger for lpr print
	static log4cplus::Logger &	lprData      ();	//!< logger for lpr print
	static log4cplus::Logger &	portPrint    ();	//!< logger for PortPrint
	static log4cplus::Logger &	portPrintData();	//!< logger for PortPrint

	static log4cplus::Logger &	mailMsg      ();	//!< logger for MailMessage
	static log4cplus::Logger &	smtp         ();	//!< logger for SMTP
	static log4cplus::Logger &	smtpData     ();	//!< logger for SMTP data

	static log4cplus::Logger &	tcpSocket    ();	//!< logger for tcp socket

	static log4cplus::Logger &	pipedProcess ();	//!< logger for piped process
	static log4cplus::Logger &	remoteTask   ();	//!< logger for remote task

	static log4cplus::Logger &	config       ();	//!< logger for configuration
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

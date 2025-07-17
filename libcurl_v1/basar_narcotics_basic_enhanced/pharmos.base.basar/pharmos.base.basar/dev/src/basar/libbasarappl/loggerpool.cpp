//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar {
namespace appl{

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//---------------------------------------------------------------------------
// init order is important: first globalLog, then further Loggers

//! \brief get logging instance for event manager
//! \return logger instance for event manager
log4cplus::Logger & LoggerPool::eventManager()
{
	static Logger log = Logging::getInstance("basar.appl.SystemEventManager");
	return log;
}

//----------------------------------------------------------------------------
}//appl
}//basar

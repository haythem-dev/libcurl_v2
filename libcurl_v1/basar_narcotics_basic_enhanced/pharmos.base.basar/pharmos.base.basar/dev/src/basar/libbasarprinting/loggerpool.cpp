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
	namespace printing {

		using log4cplus::Logger;
		//! \brief get logging instance for PsInterpreter
//! \return instance for PsInterpreter
		Logger& LoggerPool::psInterpreterOL()
		{
			static Logger log = cmnutil::Logging::getInstance("basar.cmnutil.PsInterpreterOL");
			return log;
		}

		Logger& LoggerPool::InterpretPrintOL()
		{
			static Logger log = cmnutil::Logging::getInstance("basar.cmnutil.InterpretPrintOL");
			return log;
		}
	}
}
//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARPRINT_LOGGERPOOL_H
#define GUARD_LIBBASARPRINT_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace printing	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	psInterpreterOL();	//!< logger for ps interpreter OL
	static log4cplus::Logger &	InterpretPrintOL();	//!< logger for interpret print OL
};

//----------------------------------------------------------------------------
}	// namespace printing
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

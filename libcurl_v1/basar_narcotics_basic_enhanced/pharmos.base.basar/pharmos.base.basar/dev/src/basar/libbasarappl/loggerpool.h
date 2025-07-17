//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_LOGGERPOOL_H
#define GUARD_LIBBASARAPPL_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace appl	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger & eventManager();	//!< logger for event manager
};

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

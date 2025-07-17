//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Roland Kiefert
 *  \date   09.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUIQT_LOGGERPOOL_H
#define GUARD_LIBBASARGUIQT_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar {
namespace gui	{
namespace qt	{

//----------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	prnPrevCallb  ();	//!< logger for Print Preview callback methods
	static log4cplus::Logger &	prnPrevMethods();	//!< logger for Print Preview methods
	static log4cplus::Logger &	prnPicList    ();	//!< logger for Print Preview picture list
	static log4cplus::Logger &  prnPrevOLMethods();	//!< logger for Print Preview methods
};

//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

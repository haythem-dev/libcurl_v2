//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   15.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_LOGGERPOOL_H
#define GUARD_LIBBASARGUITIE_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	eventHandler();				//!< logger for EventHandler
	static log4cplus::Logger &	transformer();				//!< logger for several transformers
	static log4cplus::Logger &	matcher();					//!< logger for matcher
	static log4cplus::Logger &	guiPropTab();				//!< logger for GuiPropertyTable
	static log4cplus::Logger &	managerImpl();				//!< logger for ManagerImpl
	static log4cplus::Logger &	qtMessageHandler();			//!< logger for QTMessages
	static log4cplus::Logger &	viewConnPtr();			    //!< logger for ViewConnPtr
	static log4cplus::Logger &	exceptionHandlerManager();	//!< logger for exception handler manager
};

//------------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

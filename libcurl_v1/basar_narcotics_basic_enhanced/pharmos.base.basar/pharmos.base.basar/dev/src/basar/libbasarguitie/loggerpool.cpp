//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   15.11.2005
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"

#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//---------------------------------------------------------------------------
//! \brief get logging instance for EventHandler
//! \return logging instance for EventHandler
log4cplus::Logger & LoggerPool::eventHandler()
{
	static Logger log = Logging::getInstance("basar.gui.tie.EventHandler");
	return log;
}

//! \brief get logging instance for several transformers
//! \return logging instance for several transformers
log4cplus::Logger & LoggerPool::transformer()
{
	static Logger log = Logging::getInstance("basar.gui.tie.Transformer");
	return log;
}

//! \brief get logging instance for matcher
//! \return logging instance for Matcher
log4cplus::Logger & LoggerPool::matcher()
{
	static Logger log = Logging::getInstance("basar.gui.tie.Matcher");
	return log;
}

//! \brief get logging instance for GuiPropertyTable
//! \return logging instance for GuiPropertyTable
log4cplus::Logger & LoggerPool::guiPropTab()
{
	static Logger log = Logging::getInstance("basar.gui.tie.GuiPropertyTable");
	return log;
}

//! \brief get logging instance for ManagerImpl
//! \return logging instance for ManagerImpl
log4cplus::Logger & LoggerPool::managerImpl()
{
	static Logger log = Logging::getInstance("basar.gui.tie.ManagerImpl");
	return log;
}

//! \brief get logging instance for QTMessageHandler
//! \return logging instance for QTMessageHandler
log4cplus::Logger & LoggerPool::qtMessageHandler()
{
	static Logger log = Logging::getInstance("basar.gui.tie.QTMessageHandler");
	return log;
}

//! \brief get logging instance for ViewConnPtr
//! \return logging instance for ViewConnPtr
log4cplus::Logger & LoggerPool::viewConnPtr()
{
	static Logger log = Logging::getInstance("basar.gui.tie.ViewConnPtr");
	return log;
}

//! \brief get logging instance for ExceptionHandlerManager
//! \return logging instance for ExceptionHandlerManager
log4cplus::Logger & LoggerPool::exceptionHandlerManager()
{
	static Logger log = Logging::getInstance("basar.gui.tie.ExceptionHandlerManager");
	return log;
}

//------------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Roland Kiefert
 *  \date   09.12.2005
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui	{
namespace qt	{

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//! common logger prefix in libbasarguiqt
#define LOGGER_BASARPREFIX	"basar.gui.qt."

//---------------------------------------------------------------------------
//! \brief get logging instance for Print Preview callback methods
//! \return logging instance for Print Preview callback methods
log4cplus::Logger & LoggerPool::prnPrevCallb()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPREFIX "PrintPreview.Callback");
	return log;
}

//! \brief get logging instance for Print Preview methods
//! \return logging instance for Print Preview methods
log4cplus::Logger & LoggerPool::prnPrevMethods()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPREFIX "PrintPreview.Methods");
	return log;
}

//! \brief get logging instance for Print Preview picture list
//! \return logging instance for Print Preview picture list
log4cplus::Logger & LoggerPool::prnPicList()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPREFIX "PrintPreview.PicList");
	return log;
}

//! \brief get logging instance for Print Preview methods
//! \return logging instance for Print Preview methods
log4cplus::Logger& LoggerPool::prnPrevOLMethods()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPREFIX "PrintPreviewOL.Methods");
	return log;
}


//------------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------

#include "commonheader.h"
#include "loggerpool.h"

using basar::cmnutil::Logging;

//---------------------------------------------------------------------------

//! \brief get main logging instance 
Logger LoggerPool::loggernarctransfer	= Logging::getInstance("narctransfer");

//! \brief get logging instance for Parameter
Logger LoggerPool::loggerUseCases		= Logging::getInstance("narctransfer.useCases");

//! \brief get logging instance for Domain Modules
Logger LoggerPool::loggerDomModules		= Logging::getInstance("narctransfer.domMod");

//---------------------------------------------------------------------------

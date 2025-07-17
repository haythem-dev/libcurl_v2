//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "loggerpool.h"
#include <log4cplus/configurator.h>


//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//---------------------------------------------------------------------------
// init order is important: first globalLog, then further Loggers

//! \brief get logging instance for Domain Modules
log4cplus::Logger LoggerPool::loggerDomModules	= basar::cmnutil::Logging::getInstance( "narcotics.DomMod" );

/*************************************************************************************************/

} //namespace narcotics
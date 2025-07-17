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
#include "version.h"
#include <log4cplus/configurator.h>


//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//---------------------------------------------------------------------------
// init order is important: first globalLog, then further Loggers

//! \brief get logging instance for Narcotics
log4cplus::Logger LoggerPool::loggerNarcotics	= basar::cmnutil::Logging::getInstance( "narcotics" );

//! \brief get logging instance for View Connectors
log4cplus::Logger LoggerPool::loggerViewConn	= basar::cmnutil::Logging::getInstance( "narcotics.ViewConn" );

//! \brief get logging instance for Use Cases
log4cplus::Logger LoggerPool::loggerUseCases	= basar::cmnutil::Logging::getInstance( "narcotics.UseCases" );

//! \brief get logging instance for Domain Modules
log4cplus::Logger LoggerPool::loggerDomModules	= basar::cmnutil::Logging::getInstance( "narcotics.DomMod" );

//! \brief get logging instance for Debugging
log4cplus::Logger LoggerPool::loggerDebug = basar::cmnutil::Logging::getInstance( "narcotics.Debug" );

/*************************************************************************************************/

//---------------------------------------------------------------------------
bool LoggerPool::initLogging( const basar::Int16 branchno )
{
	QString configFile = QString("%1").arg(getenv("MARATHON_LOG_CONFIGDIR"));
	configFile += "\\";
	configFile += PROGRAM_NAME;
	configFile += ".cfg"; 

	basar::VarString branch;
	branch.format("NARCOTICS_BRANCH=%02d", branchno);

	basar::VarString date;
	date.format("NARCOTICS_DATE=%ld", basar::cmnutil::Date::getCurrent().getDate());
	
	// setting env var %NARCOTICS_BRANCH%
	_putenv(branch.c_str());
	_putenv(date.c_str());

	log4cplus::PropertyConfigurator::doConfigure(configFile.toStdString());

	QString msg = QString("Logging initialized with config file %1").arg(configFile);
	BLOG_DEBUG(loggerNarcotics, msg.toStdString());

	return true;
}

} //namespace narcotics
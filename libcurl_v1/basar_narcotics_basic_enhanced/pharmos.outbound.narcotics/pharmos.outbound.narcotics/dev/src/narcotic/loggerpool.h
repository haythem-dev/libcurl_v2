//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects in narcotics
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NARCOTICS_LOGGERPOOL_H
#define GUARD_NARCOTICS_LOGGERPOOL_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger	loggerNarcotics;					//!< logger for Narcotics
	static log4cplus::Logger	loggerViewConn;						//!< logger for view connectors	
	static log4cplus::Logger	loggerUseCases;						//!< logger for use cases
	static log4cplus::Logger	loggerDomModules;					//!< logger for domain modules
	static log4cplus::Logger	loggerDebug;						//!< logger for Debugging

	static bool initLogging( const basar::Int16 branchno );	
};

} //namespace narcotics

#endif	//GUARD_NARCOTICS_LOGGERPOOL_H
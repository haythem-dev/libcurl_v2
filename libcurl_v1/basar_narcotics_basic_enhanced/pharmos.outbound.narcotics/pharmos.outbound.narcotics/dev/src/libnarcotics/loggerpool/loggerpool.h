//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects in narcotics
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBNARCOTICS_LOGGERPOOL_H
#define GUARD_LIBNARCOTICS_LOGGERPOOL_H

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
	static log4cplus::Logger	loggerDomModules;					//!< logger for domain modules

	static bool initLogging( const basar::Int16 branchno );	
};

} //namespace narcotics

#endif	//GUARD_NARCOTICS_LOGGERPOOL_H
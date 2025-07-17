//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGGERPOOL_H
#define GUARD_LOGGERPOOL_H

//----------------------------------------------------------------------------
using log4cplus::Logger;

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
		static Logger	loggernarctransfer;				//!< main logger
		static Logger	loggerUseCases;					//!< logger for UseCases
		static Logger	loggerDomModules;				//!< logger for DomainModules
};

#endif	//GUARD_LOGGERPOOL_H
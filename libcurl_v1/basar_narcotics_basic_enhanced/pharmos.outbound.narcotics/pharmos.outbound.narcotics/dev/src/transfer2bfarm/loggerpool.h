//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGGERPOOL_H
#define GUARD_LOGGERPOOL_H

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger	getLoggerTransferBfarm();		//!< main logger
	static log4cplus::Logger	getLoggerMissingVoucher();		//!< log missing vouchers to logfile
	static log4cplus::Logger	getLoggerSurplusVoucher();		//!< log surplus vouchers to logfile
};

//---------------------------------------------------------------------------

#endif	//GUARD_LOGGERPOOL_H

//---------------------------------------------------------------------------

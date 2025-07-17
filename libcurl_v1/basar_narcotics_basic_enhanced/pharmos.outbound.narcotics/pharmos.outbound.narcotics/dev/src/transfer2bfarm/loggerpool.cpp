//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "loggerpool.h"

//---------------------------------------------------------------------------

//! \brief get main logging instance 
log4cplus::Logger LoggerPool::getLoggerTransferBfarm()	{ return basar::cmnutil::Logging::getInstance(LOG4C_NARCOTICSMAIN); }
log4cplus::Logger LoggerPool::getLoggerMissingVoucher() { return basar::cmnutil::Logging::getInstance(LOG4C_NARCOTICSMAIN+".missingvoucher"); }
log4cplus::Logger LoggerPool::getLoggerSurplusVoucher() { return basar::cmnutil::Logging::getInstance(LOG4C_NARCOTICSMAIN+".surplusvoucher"); }
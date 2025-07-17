//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Bjoern Bischof
 *  \date   03.01.2006
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"

#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar    {
namespace property {

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//! common logger prefix in libbasardbsql
#define LOGGER_BASARPROPERTY	"basar.property."

//---------------------------------------------------------------------------
//! \brief get logging instance for Algorithm
//! \return logging instance for Algorithm
Logger & LoggerPool::algorithm				 ()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPROPERTY "Algorithm");
	return log;
}
//! \brief get logging instance for PropertyDescriptionList	
//! \return logging instance for PropertyDescriptionList
Logger & LoggerPool::propertyDescriptionList ()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPROPERTY "PropertyDescriptionList");
	return log;
}
//! \brief get logging instance for PropertyTable_XIterator
//! \return logging instance for PropertyTable_XIterator
Logger & LoggerPool::propertyTable_XIterator ()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPROPERTY "PropertyTable_XIterator");
	return log;
}
//! \brief get logging instance for PropertyTable_YIterator
//! \return logging instance for PropertyTable_YIterator
Logger & LoggerPool::propertyTable_YIterator ()
{
	static Logger log = Logging::getInstance(LOGGER_BASARPROPERTY "PropertyTable_YIterator");
	return log;
}

//------------------------------------------------------------------------------
}	// namespace property
}	// namespace basar

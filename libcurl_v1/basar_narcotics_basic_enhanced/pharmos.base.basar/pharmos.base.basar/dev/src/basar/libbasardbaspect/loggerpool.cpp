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
namespace basar  {
namespace db     {
namespace aspect {

//---------------------------------------------------------------------------
using log4cplus::Logger;
using basar::cmnutil::Logging;

//---------------------------------------------------------------------------
//! \brief get logging instance for loggerSQLStringBuilderImpl
//! \return logging instance for loggerSQLStringBuilderImpl
Logger & LoggerPool::sqlStringBuilderImpl()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.SQLStringBuilderImpl" );
	return log;
}

//! \brief get logging instance for loggerAccessMethod
//! \return logging instance for loggerAccessMethod
Logger & LoggerPool::accessMethod()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.AccessMethod" );
	return log;
}

//! \brief get logging instance for loggerAccessor
//! \return logging instance for loggerAccessor
Logger & LoggerPool::accessor()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.Accessor" );
	return log;
}

//! \brief get logging instance for loggerAptYIterator
//! \return logging instance for loggerAptYIterator
Logger & LoggerPool::aptYIterator()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.AccessorPropTblYIt" );
	return log;
}
//! \brief get logging instance for manager
//! \return logging instance for manager
Logger & LoggerPool::manager()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.Manager" );
	return log;
}

//! \brief get logging instance for init'ion
//! \return logging instance for init'ion
Logger & LoggerPool::init()
{
	static Logger log = Logging::getInstance( "basar.db.aspect.Init" );
	return log;
}


//------------------------------------------------------------------------------
}	// namespace aspect
}	// namespace db
}	// namespace basar

#ifndef GUARD_LIBBASARDBASPECT_LOGGERPOOL_H
#define GUARD_LIBBASARDBASPECT_LOGGERPOOL_H
//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  pool for logger objects
 *  \author Bjoern Bischof
 *  \date   03.01.2005
 */
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
#include <log4cplus/logger.h>

//------------------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
namespace aspect {

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger & sqlStringBuilderImpl();	//!< logger for SQLStringBuilderImpl
	static log4cplus::Logger & accessMethod();			//!< logger for access method
	static log4cplus::Logger & accessor();				//!< logger for accessor
	static log4cplus::Logger & aptYIterator();			//!< logger for accessor property table yiterator
	static log4cplus::Logger & manager();			//!< logger for DBAspect manager
	static log4cplus::Logger & init();				    //!< logger for DBAspect init'ion
};

//------------------------------------------------------------------------------
}	// namespace aspect
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD_LIBBASARDBASPECT_LOGGERPOOL_H

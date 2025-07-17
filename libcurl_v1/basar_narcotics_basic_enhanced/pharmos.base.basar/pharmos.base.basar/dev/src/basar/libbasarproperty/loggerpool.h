#ifndef GUARD_LIBBASARPROPERTY_LOGGERPOOL_H
#define GUARD_LIBBASARPROPERTY_LOGGERPOOL_H
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
namespace basar    {
namespace property {

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	algorithm();				//!< logger for algorithm.cpp		
	static log4cplus::Logger &	propertyDescriptionList();	//!< logger for propertydescriptionlist.cpp		
	static log4cplus::Logger &	propertyTable_XIterator();	//!< logger for PropertyTable_XIterator.cpp		
	static log4cplus::Logger &	propertyTable_YIterator();	//!< logger for PropertyTable_YIterator.cpp		
};

//------------------------------------------------------------------------------
}	// namespace property
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD_LIBBASARPROPERTY_LOGGERPOOL_H

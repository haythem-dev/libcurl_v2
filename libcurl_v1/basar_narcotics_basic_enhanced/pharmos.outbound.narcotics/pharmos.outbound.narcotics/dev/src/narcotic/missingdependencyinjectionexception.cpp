//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief		exception for missing dependency injection
 *  \author		Steffen Heinlein
 *  \date		22.06.2012
 *  \version	00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include "missingdependencyinjectionexception.h"

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//--------------------------------------------------------------------------------------------------//
	MissingDependencyInjectionException::MissingDependencyInjectionException( const basar::ExceptInfo& sInfo )
		: BasarException( sInfo, "MissingDependancyInjectionException" )
	{}

	//--------------------------------------------------------------------------------------------------//
	MissingDependencyInjectionException::~MissingDependencyInjectionException()
	{}

} // namespace infrastructure
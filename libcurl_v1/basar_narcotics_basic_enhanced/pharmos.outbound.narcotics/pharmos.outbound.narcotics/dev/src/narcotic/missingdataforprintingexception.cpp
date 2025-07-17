//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief		exception for missing remote procedure
 *  \author		Steffen Heinlein
 *  \date		22.06.2012
 *  \version	00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include "missingdataforprintingexception.h"

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{

	//--------------------------------------------------------------------------------------------------//
	MissingDataForPrintingException::MissingDataForPrintingException( const basar::ExceptInfo& sInfo )
		: BasarException( sInfo, "MissingDataForPrintingException" )
	{}

	//--------------------------------------------------------------------------------------------------//
	MissingDataForPrintingException::~MissingDataForPrintingException()
	{}

} // namespace util
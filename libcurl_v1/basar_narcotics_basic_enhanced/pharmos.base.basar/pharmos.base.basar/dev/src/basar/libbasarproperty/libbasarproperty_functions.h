#ifndef GUARD_LIBBASARPROPERTY_FUNCTION_H
#define GUARD_LIBBASARPROPERTY_FUNCTION_H
//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace function
 *  \author Bloern Bischof
 *  \date   17.11.2005
 */
//------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------//
// dll export and import macros
//--------------------------------------------------------------------------------------//
#ifndef LIBBASARPROPERTY_API
	#ifdef _WIN32
		#ifdef LIBBASARPROPERTY_EXPORTS
			#define LIBBASARPROPERTY_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARPROPERTY_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARPROPERTY_API
	#endif 
#endif 

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace property	{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARPROPERTY_API bool               init                ();
    
//----------------------------------------------------------------------------
/*! \brief package's version \n     no-throw		*/
LIBBASARPROPERTY_API ConstBuffer		getVersion			();

/*! \brief programname, version and buildtime info \n     no-throw		*/
LIBBASARPROPERTY_API ConstBuffer		getFullVersion		();
//----------------------------------------------------------------------------

}	// namespace property
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD_LIBBASARPROPERTY_FUNCTION_H

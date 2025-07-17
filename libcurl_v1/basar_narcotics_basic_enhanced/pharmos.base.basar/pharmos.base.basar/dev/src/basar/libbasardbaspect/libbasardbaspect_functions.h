#ifndef GUARD_LIBBASARDBASPECT_FUNCTION_H
#define GUARD_LIBBASARDBASPECT_FUNCTION_H
//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace function
 *  \author Roland Kiefert
 *  \date   07.09.2005
 */
//------------------------------------------------------------------------------

#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
namespace basar  {
namespace db	 {
namespace aspect {

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARDBASPECT_API bool               init                ();
    
/*! \brief package's version
 	\n     no-throw		*/
LIBBASARDBASPECT_API ConstBuffer		getVersion			();

/*! \brief programname, version and buildtime info
    \n     no-throw		*/
LIBBASARDBASPECT_API ConstBuffer		getFullVersion		();

}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

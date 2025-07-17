//----------------------------------------------------------------------------
/*! \file
 *  \brief  functions for version interface for application utilities
 *  \author Thomas Hörath
 *  \date   15.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_FUNCTIONS_H
#define GUARD_LIBBASARAPPL_FUNCTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARAPPL_API
	#ifdef _WIN32
		#ifdef LIBBASARAPPL_EXPORTS
			#define LIBBASARAPPL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARAPPL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARAPPL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
//! namespace for application-component
namespace appl
{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARAPPL_API bool          init                ();

//----------------------------------------------------------------------------
/*! \brief package's version
 	\n     no-throw		*/
LIBBASARAPPL_API ConstBuffer		getVersion			();

/*! \brief programname, version and buildtime info
    \n     no-throw		*/
LIBBASARAPPL_API ConstBuffer		getFullVersion		();

//----------------------------------------------------------------------------
}	//namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------
/*! \file
 *  \brief  functions for version interface for login component
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARLOGIN_FUNCTIONS_H
#define GUARD_LIBBASARLOGIN_FUNCTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARLOGIN_API
	#ifdef _WIN32
		#ifdef LIBBASARLOGIN_EXPORTS
			#define LIBBASARLOGIN_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARLOGIN_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARLOGIN_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
//! namespace for login-component
namespace login
{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARLOGIN_API bool              init                ();
    
//----------------------------------------------------------------------------
/*! \brief package's version
 	\n     no-throw		*/
LIBBASARLOGIN_API ConstBuffer		getVersion			();

/*! \brief programname, version and buildtime info
    \n     no-throw		*/
LIBBASARLOGIN_API ConstBuffer		getFullVersion		();

//----------------------------------------------------------------------------
}	//namespace login
}	// namespace basar

//----------------------------------------------------------------------------
#endif

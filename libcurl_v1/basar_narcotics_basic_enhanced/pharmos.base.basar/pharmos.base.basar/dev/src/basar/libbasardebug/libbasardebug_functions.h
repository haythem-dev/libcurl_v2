//----------------------------------------------------------------------------
/*! \file
 *  \brief  functions for version interface for basar debug
 *  \author Thomas Hörath
 *  \date   23.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDEBUG_FUNCTIONS_H
#define GUARD_LIBBASARDEBUG_FUNCTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARDEBUG_API
	#ifdef _WIN32
		#ifdef LIBBASARDEBUG_EXPORTS
			#define LIBBASARDEBUG_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDEBUG_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDEBUG_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace debug
{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARDEBUG_API bool				init                ();

//----------------------------------------------------------------------------
/*! \brief package's version
 	\n     no-throw		*/
LIBBASARDEBUG_API ConstBuffer		getVersion			();

/*! \brief programname, version and buildtime info
    \n     no-throw		*/
LIBBASARDEBUG_API ConstBuffer		getFullVersion		();

//----------------------------------------------------------------------------
}	//namespace debug
}	// namespace basar

//----------------------------------------------------------------------------
#endif

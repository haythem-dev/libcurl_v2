//----------------------------------------------------------------------------
/*! \file
 *  \brief  functions for version interface for common utilities
 *  \author Thomas Hörath
 *  \date   15.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_FUNCTIONS_H
#define GUARD_LIBBASARCMNUTIL_FUNCTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
//! namespace for cmnutil-component
namespace cmnutil
{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco) */
LIBBASARCMNUTIL_API bool        init                ();

//----------------------------------------------------------------------------
/*! \brief package's version */
LIBBASARCMNUTIL_API ConstBuffer	getVersion	  ();

/*! \brief programname, version and buildtime info */
LIBBASARCMNUTIL_API ConstBuffer	getFullVersion();

//----------------------------------------------------------------------------
//! \brief get executable's absolute path and name (native: e.g Windows backslashes)
LIBBASARCMNUTIL_API	VarString	getExecPath	  ();

//! \brief  get executable's filename
LIBBASARCMNUTIL_API VarString   getExecName   (bool full = false    //!< with/without extension
                                              );

//! \brief  get executable's absolute directory name (native: e.g Windows backslashes)
LIBBASARCMNUTIL_API VarString   getExecDir    ();

//! \brief  get library's absolute directroy name (in case it matters, only libbbasarcmnutil)
LIBBASARCMNUTIL_API VarString   getLibDir     ();

//----------------------------------------------------------------------------
}	//namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif

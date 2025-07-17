//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace function
 *  \author Roland Kiefert
 *  \date   06.12.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUIQT_FUNCTIONS_H
#define GUARD_LIBBASARGUIQT_FUNCTIONS_H

//------------------------------------------------------------------------------
#ifndef LIBBASARGUIQT_API
	#ifdef _WIN32
		#ifdef LIBBASARGUIQT_EXPORTS
			#define LIBBASARGUIQT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARGUIQT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARGUIQT_API
	#endif 
#endif 

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace gui	{
//! namespace for Qt component
namespace qt    {

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco) */
LIBBASARGUIQT_API bool				init                ();

//----------------------------------------------------------------------------
/*! \brief package's version */
LIBBASARGUIQT_API ConstString		getVersion			();

//----------------------------------------------------------------------------
/*! \brief programname, version and buildtime info */
LIBBASARGUIQT_API ConstString		getFullVersion		();

//------------------------------------------------------------------------------
}	// qt
}	// gui
}	// basar

//------------------------------------------------------------------------------
#endif	// GUARD

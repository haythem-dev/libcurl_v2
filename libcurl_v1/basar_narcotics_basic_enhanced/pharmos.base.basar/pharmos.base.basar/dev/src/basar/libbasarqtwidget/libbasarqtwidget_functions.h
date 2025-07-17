//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace functions
 *  \author Michael Eichenlaub
 *  \date   23.02.2006
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_FUNCTIONS_H
#define GUARD_LIBBASARQTWIDGET_FUNCTIONS_H

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace gui   {
namespace widget{

//------------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARQTWIDGET_API
	#ifdef _WIN32
		#ifdef LIBBASARQTWIDGET_EXPORTS
			#define LIBBASARQTWIDGET_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARQTWIDGET_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARQTWIDGET_API
	#endif
#endif

//------------------------------------------------------------------------------
// version

//----------------------------------------------------------------------------
//! \brief init library (avoid static init order fiasco)
LIBBASARQTWIDGET_API bool			init                ();

//! \brief package's version
LIBBASARQTWIDGET_API ConstString	getVersion			();	

//! \brief programname, version and buildtime info
LIBBASARQTWIDGET_API ConstString	getFullVersion		();

//------------------------------------------------------------------------------
}	// namespace widget
}	// namespace gui
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

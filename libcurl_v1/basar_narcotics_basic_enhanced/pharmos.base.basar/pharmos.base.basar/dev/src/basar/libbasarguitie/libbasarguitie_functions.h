//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace function
 *  \author Roland Kiefert
 *  \date   19.09.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_FUNCTIONS_H
#define GUARD_LIBBASARGUITIE_FUNCTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARGUITIE_API
	#ifdef _WIN32
		#ifdef LIBBASARGUITIE_EXPORTS
			#define LIBBASARGUITIE_API __declspec(dllexport)        //!< dll exported
		#else
			#define LIBBASARGUITIE_API __declspec(dllimport)        //!< dll imported
		#endif
	#else
		#define LIBBASARGUITIE_API
	#endif
#endif	// LIBBASARGUITIE_API

//----------------------------------------------------------------------------
#include "libbasarguitie_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
//! namespace for gui
namespace gui	{
//! namespace for Qt tie
namespace tie   {

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARGUITIE_API bool						init                ();

//----------------------------------------------------------------------------
/*! \brief package's version */
LIBBASARGUITIE_API ConstString				getVersion			();

/*! \brief programmname, version and buildtime info */
LIBBASARGUITIE_API ConstString				getFullVersion		();

/*! \brief match Qt's Widget Return Value into Basar's WidgetReturnEnum */
LIBBASARGUITIE_API const WidgetReturnEnum	getWidgetReturnType	(basar::Int32 retCode	//!< Qt's return code which will be matched
																 );
//----------------------------------------------------------------------------
}	// tie
}	// gui
}	// basar

//------------------------------------------------------------------------------
#endif	// GUARD

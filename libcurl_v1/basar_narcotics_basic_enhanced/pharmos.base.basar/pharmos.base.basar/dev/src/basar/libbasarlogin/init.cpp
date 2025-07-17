//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------

#include "libbasarlogin_functions.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarproperty_functions.h"
#include "libbasardbsql_functions.h"
#include "libbasardbaspect_functions.h"

#ifdef _WIN32
#include "libbasarguitie_functions.h"
#endif

/*----------------------------------------------------------------------------*/
namespace basar {
namespace login {

//---------------------------------------------------------------------------
//! \return	dummy value for static init
//! \throw  no-throw
//!
bool init()
{
	static bool done = false;
	
	if (done)
		return true;

	basar::cmnutil   ::init();
	basar::property  ::init();
	basar::db::sql   ::init();
	basar::db::aspect::init();
#ifdef _WIN32	
	basar::gui::tie  ::init();
#endif	

	done = true;
		
	return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib =init();

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------

#include "libbasarappl_functions.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarproperty_functions.h"
#include "libbasardbaspect_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace appl
{

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
	basar::db::aspect::init();
		
	done = true;
		
	return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib =init();

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar



//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------

#include "libbasarproperty_functions.h"

#include "libbasardebug_functions.h"
#include "libbasarcmnutil_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace property
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
		
	basar::debug  ::init();
	basar::cmnutil::init();
		
	done = true;
		
	return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace property
}	// namespace basar



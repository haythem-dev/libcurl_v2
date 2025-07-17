//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_functions.h"

#include "libbasardebug_functions.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_locale.h"

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace cmnutil
{

//---------------------------------------------------------------------------
//! \return	dummy value for static init
//!
bool init()
{
	static bool done = false;
	
	if (done)
		return true;

	// --- depends on
	basar::debug::init();

		
	// --- cmnutil stuff

	// basar logging object for initializing logging framework: 
	// read configuration etc.
	static Logging globalLog;
	
	// set locale, Informix environment
    Locale::initSystemSettings();

	
	done = true;
		
	return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

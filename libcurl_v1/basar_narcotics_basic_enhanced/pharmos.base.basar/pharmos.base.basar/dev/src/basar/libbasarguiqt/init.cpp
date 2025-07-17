//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------


#include "libbasarguiqt_functions.h"

#include "libbasardebug_functions.h"
#include "libbasarcmnutil_functions.h"
#include "libbasarproperty_functions.h"
#include "libbasardbsql_functions.h"
#include "libbasardbaspect_functions.h"
#include "libbasarappl_functions.h"
#include "libbasarguitie_functions.h"
#include "libbasarlogin_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar {
namespace gui   {
namespace qt    {

//---------------------------------------------------------------------------
//! \return dummy value for static init
//! \throw  no-throw
//!
bool init()
{
    static bool done = false;

    if (done)
        return true;
        
	basar::debug     ::init();
	basar::cmnutil   ::init();
	basar::property  ::init();
	basar::db::sql   ::init();
	basar::db::aspect::init();
	basar::appl      ::init();
	basar::gui::tie  ::init();
	basar::login     ::init();

	done = true;

    return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace
}   // namespace
}   // namespace


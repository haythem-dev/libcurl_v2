//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------


#include "libbasarguitie_functions.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarproperty_functions.h"
#include "libbasardbsql_functions.h"
#include "libbasardbaspect_functions.h"
#include "libbasarqtwidget_functions.h"
#include "libbasarappl_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar {
namespace gui   {
namespace tie   {

//---------------------------------------------------------------------------
//! \return dummy value for static init
//! \throw  no-throw
//!
bool init()
{
    static bool done = false;

    if (done)
        return true;
        
	basar::cmnutil    ::init();
	basar::property   ::init();
	basar::db::sql    ::init();
	basar::db::aspect ::init();
	basar::gui::widget::init();
	basar::appl       ::init();
	
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


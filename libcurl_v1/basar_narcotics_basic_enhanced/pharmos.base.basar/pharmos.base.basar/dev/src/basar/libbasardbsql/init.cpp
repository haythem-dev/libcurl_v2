//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------


#include "libbasardbsql_functions.h"
#include "libbasarcmnutil_functions.h"

/*----------------------------------------------------------------------------*/
namespace basar {
namespace db    {
namespace sql   {

//---------------------------------------------------------------------------
//! \return dummy value for static init
//! \throw  no-throw
//!
bool init()
{
    static bool done = false;

    if (done)
        return true;
        
	basar::cmnutil::init();

    done = true;

    return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace sql
}   // namespace db
}   // namespace basar

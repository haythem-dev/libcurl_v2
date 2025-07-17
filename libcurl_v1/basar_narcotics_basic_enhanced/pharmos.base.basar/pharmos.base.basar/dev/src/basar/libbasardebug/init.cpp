//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------


#include "libbasardebug_functions.h"

#include "checks.h"

/*----------------------------------------------------------------------------*/
namespace basar {
namespace debug {

//---------------------------------------------------------------------------
//! \return dummy value for static init
//! \throw  no-throw
//!
bool init()
{
    static bool done = false;

    if (done)
        return true;

	// instantiates memory check object
	check();
        
    done = true;

    return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace
}   // namespace


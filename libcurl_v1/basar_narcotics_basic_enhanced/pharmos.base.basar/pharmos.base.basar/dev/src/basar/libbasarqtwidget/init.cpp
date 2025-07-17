//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------


#include "libbasarqtwidget_functions.h"

#include "libbasarcmnutil_functions.h"

#pragma warning (push)
#pragma warning(disable: 4127 4512)
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#pragma warning (pop)

/*----------------------------------------------------------------------------*/
namespace basar  {
namespace gui    {
namespace widget {

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
	
	//add working dir to qt's lib path
	QCoreApplication::instance()->addLibraryPath(QDir::currentPath());

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

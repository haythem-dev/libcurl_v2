//----------------------------------------------------------------------------
/*! \file
 *  \brief  static initialization
 *  \author Michael Eichenlaub
 *  \date   21.01.2013
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_functions.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarproperty_functions.h"
#include "libbasardbsql_functions.h"
#include "loggerpool.h"

/*----------------------------------------------------------------------------*/
namespace basar  {
namespace db     {
namespace aspect {

//---------------------------------------------------------------------------
//! \return	dummy value for static init
//! \throw  no-throw
//!
bool init()
{
	static bool done = false;
	
	if (done)
		return true;
		
	basar::cmnutil ::init();
	basar::property::init();
	basar::db::sql ::init();
		
	done = true;
		
	log4cplus::Logger log = LoggerPool::init();
	
	BLOG_TRACE(log, "basar::db::aspect::init() finished");
		
	return true;
}

//---------------------------------------------------------------------------
//! init library object -> avoid static init order fiasco
static bool initlib = init();

//----------------------------------------------------------------------------
}	// namespace aspect
}	// namespace db
}	// namespace basar

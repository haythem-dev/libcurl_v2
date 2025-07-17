//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: namespace functions
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_FUNCTIONS_H
#define GUARD_LIBBASARDBSQL_FUNCTIONS_H

//------------------------------------------------------------------------------
#include "libbasardbsql_api.h"
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
/*! \brief init library (avoid static init order fiasco)
    \n     no-throw     */
LIBBASARDBSQL_API bool          init                ();

//------------------------------------------------------------------------------
// version

LIBBASARDBSQL_API ConstBuffer	getVersion			();	//!< package's version			\n no-throw
LIBBASARDBSQL_API ConstBuffer	getFullVersion		();	//!< programname, version and buildtime info	\n no-throw

//------------------------------------------------------------------------------
// profiling

LIBBASARDBSQL_API UInt32		getDbLibFetchTime	();	//!< get summarized fetch time (in ms) per opened statement within DB library (e.g. Informix client sdk)

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

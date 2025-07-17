//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_H
#define GUARD_LIBBASARDBSQL_H

//------------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARDBSQL_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARDBSQL_H
#endif
#endif

//------------------------------------------------------------------------------
#include "libbasardbsql_definitions.h"

#include "libbasardbsql_functions.h"

#include "libbasardbsql_sqldiagnosticsref.h"
#include "libbasardbsql_resultsetref.h"
#include "libbasardbsql_insertsetref.h"
#include "libbasardbsql_statementref.h"
#include "libbasardbsql_preparedstatementref.h"
#include "libbasardbsql_connectionref.h"
#include "libbasardbsql_databaseinfo.h"

#include "libbasardbsql_exceptions.h"

#include "libbasardbsql_managerimpl.h"
#include "libbasardbsql_manager.h"

//------------------------------------------------------------------------------
#endif	// GUARD

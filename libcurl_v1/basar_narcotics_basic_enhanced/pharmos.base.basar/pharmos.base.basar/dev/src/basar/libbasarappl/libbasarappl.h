//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface for common utilities
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_H
#define GUARD_LIBBASARAPPL_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARAPPL_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARAPPL_H
#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarappl_functions.h"
		
#include "libbasarappl_systemeventmanager.h"
#include "libbasarappl_syseventexecuter.hpp"
#include "libbasarappl_eventmacros.h"

//----------------------------------------------------------------------------
#endif


//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface for basar login
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARLOGIN_H
#define GUARD_LIBBASARLOGIN_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARLOGIN_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARLOGIN_H
#endif
#endif

//------------------------------------------------------------------------------
#include "libbasarlogin_definitions.h"
#include "libbasarlogin_functions.h"
#include "libbasarlogin_manager.h"
#include "libbasarlogin_exceptions.h"

#endif //GUARD

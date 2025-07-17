//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Roland Kiefert
 *  \date   19.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_H
#define GUARD_LIBBASARGUITIE_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARGUITIE_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARGUITIE_H
#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

#include "libbasarguitie_definitions.h"
#include "libbasarguitie_functions.h"
#include "libbasarguitie_exceptions.h"
#include "libbasarguitie_manager.h"
#include "libbasarguitie_transformer.h"
#include "libbasarguitie_matcher.h"
#include "libbasarguitie_propertytable.h"
#include "libbasarguitie_msgboxes.h"
#include "libbasarguitie_iviewconnector.h"
#include "libbasarguitie_viewconnptr.hpp"
#include "libbasarguitie_exceptionhandler.h"
#include "libbasarguitie_exceptionhandlerref.h"
#include "libbasarguitie_exceptionhandlermanager.h"
#include "libbasarguitie_exceptionhandlermacros.h"

//----------------------------------------------------------------------------
#endif

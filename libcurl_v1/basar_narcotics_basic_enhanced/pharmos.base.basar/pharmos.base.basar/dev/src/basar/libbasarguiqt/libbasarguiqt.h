//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Roland Kiefert
 *  \date   06.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUIQT_H
#define GUARD_LIBBASARGUIQT_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARGUIQT_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARGUIQT_H
#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarguiqt_functions.h"

#include "libbasarguiqt_manager.h"
#include "libbasarguiqt_statusline.h"
#include "libbasarguiqt_mainbackground.h"
#include "libbasarguiqt_about.h"
#include "libbasarguiqt_printpreview.h"

#endif

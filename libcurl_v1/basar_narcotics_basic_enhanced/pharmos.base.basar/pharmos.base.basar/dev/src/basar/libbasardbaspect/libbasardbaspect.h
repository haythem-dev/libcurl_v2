#ifndef GUARD_LIBBASARDBASPECT_H
#define GUARD_LIBBASARDBASPECT_H
//---------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARDBASPECT_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARDBASPECT_H
#endif
#endif


//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_definitions.h"
#include "libbasardbaspect_exceptions.h"
#include "libbasardbaspect_functions.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorexecutelistener.h"
#include "libbasardbaspect_sqlstringbuilder.h"
#include "libbasardbaspect_macros.h"
#include "libbasardbaspect_logmacros.h"

#endif

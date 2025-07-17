//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Roland Kiefert 
 *  \date   06.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_H
#define GUARD_LIBBASARQTWIDGET_H

//------------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARQTWIDGET_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARQTWIDGET_H
#endif
#endif

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"

#include "libbasarqtwidget_functions.h"
#include "libbasarqtwidget_tablewidget.h"
#include "libbasarqtwidget_multicombobox.h"
#include "libbasarqtwidget_datetimeedit.h"
#include "libbasarqtwidget_dateedit.h"
#include "libbasarqtwidget_timeedit.h"
#include "libbasarqtwidget_basarwidgets.h"
#include "libbasarqtwidget_integersignalemitter.h"
#include "libbasarqtwidget_i18n.h"

//------------------------------------------------------------------------------
#endif //GUARD
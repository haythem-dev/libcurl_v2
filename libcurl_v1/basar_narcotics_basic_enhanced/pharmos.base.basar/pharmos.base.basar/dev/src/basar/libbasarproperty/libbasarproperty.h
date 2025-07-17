#ifndef GUARD_LIBBASARPROPERTY_H
#define GUARD_LIBBASARPROPERTY_H
//--------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface for property
 *  \author Björn Bischof
 *  \date   15.07.2005
 */
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
// displays stage of compiling, so not fulfilled precompile conditions can be detected
//--------------------------------------------------------------------------------------//
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARPROPERTY_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARPROPERTY_H
#endif
#endif


//--------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------//
#include "libbasarproperty_definitions.h"
#include "libbasarproperty_functions.h"
#include "libbasarproperty_exceptions.h"
#include "libbasarproperty_supportedstate.h"
#include "libbasarproperty_propertytype.h"
#include "libbasarproperty_propertystate.h"
#include "libbasarproperty_propertystateset.h"
#include "libbasarproperty_propertydescriptionlist_iterator.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarproperty_propertytableref.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertytable_yiterator.h"
#include "libbasarproperty_propertytablesnapshotref.h"
#include "libbasarproperty_algorithm.h"
#include "libbasarproperty_propertydescription.h"

//--------------------------------------------------------------------------------------------//
#endif



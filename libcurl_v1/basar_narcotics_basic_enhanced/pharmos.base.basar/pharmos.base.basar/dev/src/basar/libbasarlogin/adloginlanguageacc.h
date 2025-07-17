//----------------------------------------------------------------------------
/*! \file	adloginlanguageacc.h
 *  \brief  string builder declaration for language-accessor
 *  \author Thomas Hörath
 *  \date   25.03.2014
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_ADLOGINLANGUAGE_H
#define GUARD_ACC_ADLOGINLANGUAGE_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for language-accessor
namespace acc_adloginlanguage
{

//---------------------------------------------------------------------------
//! class for selecting language from pploginlanguage \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelLanguage)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelAppLanguage)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} // acc_language

//----------------------------------------------------------------------------
#endif	// GUARD

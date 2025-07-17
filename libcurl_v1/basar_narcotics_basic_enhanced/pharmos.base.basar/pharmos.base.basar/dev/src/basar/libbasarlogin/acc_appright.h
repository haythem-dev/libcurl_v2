//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for appright-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_APPRIGHT_H
#define GUARD_ACC_APPRIGHT_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for application right-accessor
namespace acc_appright
{

//---------------------------------------------------------------------------
//! class for selecting rights from ppappright (for all areas and modules to one appl and user (and his groups) \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelAppRights)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelAppRightsRegion)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} // acc_appright

//----------------------------------------------------------------------------
#endif	// GUARD

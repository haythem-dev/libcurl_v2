#ifndef GUARD_ACC_ADLOGINAPPRIGHT_H
#define GUARD_ACC_ADLOGINAPPRIGHT_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for application right-accessor
namespace acc_adLoginAppRight
{

//---------------------------------------------------------------------------
//! class for selecting rights from ppappright (for all areas and modules to one appl and user (and his groups) \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelAppRights)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(SelAppUserRights)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} // acc_appright

//----------------------------------------------------------------------------
#endif	// GUARD

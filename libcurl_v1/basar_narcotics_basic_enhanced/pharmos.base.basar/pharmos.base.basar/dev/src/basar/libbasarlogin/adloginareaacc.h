#ifndef GUARD_ACC_ADLOGINAREA_H
#define GUARD_ACC_ADLOGINAREA_H

//-----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for area-accessor
namespace acc_adloginarea
{

//---------------------------------------------------------------------------
//! class for selecting branch from ppbranch \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelArea)
END_BUILDER_DECLARATION

//! class for selecting areaid \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelAreaID)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting global area for UADM \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelectGlobalArea)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting region from ppregion \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelRegion)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting areaname to unique key (branchid/regionid) \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelAreaName)
END_BUILDER_DECLARATION

//----------------------------------------------------------------------------
} // acc_area

//----------------------------------------------------------------------------
#endif	// GUARD

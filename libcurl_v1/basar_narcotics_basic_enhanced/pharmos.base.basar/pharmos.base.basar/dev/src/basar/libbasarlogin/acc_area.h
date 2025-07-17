//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for area-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_AREA_H
#define GUARD_ACC_AREA_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for area-accessor
namespace acc_area
{

//---------------------------------------------------------------------------
//! class for selecting branch from ppbranch \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelBranch)
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

//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for area-accessor + registration of area-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_area.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_area
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelBranch )

    ENSURE_PROPERTIES_ARE_SET( SelBranch, "applicationid;" )

    SQL_PATTERN( SelBranch,
                 "SELECT DISTINCT b.branchid as areaid, b.name as areaname "
                 "FROM ppbranch b, ppcorappbranchreg c "
                 "WHERE c.applicationid=#applicationid# and c.branchid=b.branchid "
                 "ORDER BY b.name" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelRegion )

    ENSURE_PROPERTIES_ARE_SET( SelRegion, "applicationid;" )

    SQL_PATTERN( SelRegion,
                 "SELECT DISTINCT r.regionid as areaid, r.name as areaname "
                 "FROM ppregion r, ppcorappbranchreg c "
                 "WHERE c.applicationid=#applicationid# and c.regionid=r.regionid "
                 "ORDER BY r.name" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAreaName )

    ENSURE_PROPERTIES_ARE_SET( SelAreaName, "areaid;" )

    SQL_PATTERN( SelAreaName,
                 "SELECT regionid as areaid, name as areaname "
                 "FROM ppregion "
                 "WHERE regionid=#areaid# "
                 "UNION "
                 "SELECT branchid as areaid, name as areaname "
                 "FROM ppbranch "
                 "WHERE branchid=#areaid#" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "Area" )

    PROPERTY_DESCRIPTION_LIST( "int16 areaid; string areaname;" )

    ACCESS_METHOD( domMod::area::lit::SELECT_BRANCH_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelBranch )
    SQL_BUILDER_PUSH_BACK( SelBranch )

    ACCESS_METHOD( domMod::area::lit::SELECT_AREA_BY_ID )
    SQL_BUILDER_CREATE( SelAreaName )
    SQL_BUILDER_PUSH_BACK( SelAreaName )

    ACCESS_METHOD( domMod::area::lit::SELECT_REGION_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelRegion )
    SQL_BUILDER_PUSH_BACK( SelRegion )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_area


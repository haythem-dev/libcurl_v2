#include "adloginareaacc.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adloginarea
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelArea )

    ENSURE_PROPERTIES_ARE_SET( SelArea, "applicationid;" )

    SQL_PATTERN( SelArea,
                 "SELECT DISTINCT ar.areaid as areaid ,ar.areano as areano,ar.areaname as areaname "
                 "FROM area ar, appareamap aam "
                 "WHERE aam.applicationid = #applicationid# and aam.areaid = ar.areaid "
                 "ORDER BY ar.areaname" )

//---------------------------------------------------------------------------

    BUILDER_DEFINITION( SelectGlobalArea )

    DONT_CHECK_PROPERTIES( SelectGlobalArea )

    SQL_PATTERN( SelectGlobalArea,
                 "SELECT areaname, areaid, areano "
                 " FROM area where areano = 0 " )
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
                 "SELECT areaid, areaname, areano "
                 "FROM area "
                 "WHERE areano=#areaid#" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAreaID )

    ENSURE_PROPERTIES_ARE_SET( SelAreaID, "areano;" )

    SQL_PATTERN( SelAreaID,
                 "SELECT areaid, areaname, areano "
                 "FROM area "
                 "WHERE areano=#areano#" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginArea" )

    PROPERTY_DESCRIPTION_LIST( "int32 areaid; int32 areano; string areaname; " )

    ACCESS_METHOD( domMod::area::lit::SELECT_AREA_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelArea )
    SQL_BUILDER_PUSH_BACK( SelArea )

    ACCESS_METHOD(  domMod::area::lit::SELECT_AREA_ID_BY_AREA_NO )
    SQL_BUILDER_CREATE( SelAreaID )
    SQL_BUILDER_PUSH_BACK( SelAreaID )

    ACCESS_METHOD( domMod::area::lit::SELECT_GLOBAL_AREA )
    SQL_BUILDER_CREATE( SelectGlobalArea )
    SQL_BUILDER_PUSH_BACK( SelectGlobalArea )

    ACCESS_METHOD( domMod::area::lit::SELECT_AREA_BY_ID )
    SQL_BUILDER_CREATE( SelAreaName )
    SQL_BUILDER_PUSH_BACK( SelAreaName )

    ACCESS_METHOD( domMod::area::lit::SELECT_REGION_BY_APPLICATION_ID )
    SQL_BUILDER_CREATE( SelRegion )
    SQL_BUILDER_PUSH_BACK( SelRegion )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_area


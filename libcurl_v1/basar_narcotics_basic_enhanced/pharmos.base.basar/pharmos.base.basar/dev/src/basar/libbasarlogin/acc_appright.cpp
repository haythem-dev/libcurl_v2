//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for apprigth-accessor + registration of apprigth-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_appright.h"

#include "usermgmt_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_appright
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAppRights )

    bool SelAppRights::isExecutable() const
    {
        if ( !isContainedAndSet( "branchregionid" ) )
        {
            return arePropertiesSet( "applicationid; userid;" );
        }

        return false;
    }

    SQL_PATTERN( SelAppRights,
                 "SELECT groupid, userid, applicationid, moduleid, branchregionid, rightname "
                 "FROM ppappright "
                 "WHERE applicationid=#applicationid# and userid=#userid# "
                 "UNION "
                 "SELECT groupid, userid, applicationid, moduleid, branchregionid, rightname "
                 "FROM ppappright r "
                 "WHERE groupid IN "
                 "(SELECT c.groupid "
                 "FROM ppcorappusergroup c "
                 "WHERE c.applicationid=#applicationid# and c.userid=#userid#) "
                 "and applicationid=#applicationid# " )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAppRightsRegion )

    ENSURE_PROPERTIES_ARE_SET( SelAppRightsRegion, "applicationid; userid; branchregionid;" )

    SQL_PATTERN( SelAppRightsRegion,
                 "SELECT groupid, userid, applicationid, moduleid, branchregionid, rightname "
                 "FROM ppappright "
                 "WHERE applicationid=#applicationid# and userid=#userid# and branchregionid = #branchregionid# "
                 "UNION "
                 "SELECT groupid, userid, applicationid, moduleid, branchregionid, rightname "
                 "FROM ppappright r "
                 "WHERE groupid IN "
                 "(SELECT c.groupid "
                 "FROM ppcorappusergroup c "
                 "WHERE c.applicationid=#applicationid# and c.userid=#userid#) "
                 "and applicationid=#applicationid# and branchregionid = #branchregionid# " )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "AppRight" )

    PROPERTY_DESCRIPTION_LIST( "int16 groupid; int16 userid; int16 applicationid; "
                               "int16 moduleid; string rightname; int32 branchregionid;" )

    ACCESS_METHOD( domMod::appRight::lit::SELECT_RIGHTS_BY_PATTERN )
    SQL_BUILDER_CREATE( SelAppRightsRegion )
    SQL_BUILDER_PUSH_BACK( SelAppRightsRegion )
    SQL_BUILDER_CREATE( SelAppRights )
    SQL_BUILDER_PUSH_BACK( SelAppRights )


    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_appright

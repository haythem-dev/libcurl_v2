#include "adloginuseracc.h"

#include "usermgmt_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adLoginUser
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelUserID )

    ENSURE_PROPERTIES_ARE_SET( SelUserID, "userid;" )

    SQL_PATTERN( SelUserID,
                 "SELECT userid , username, firstname as forename, lastname as surname, emailuser, isactiv as activ, "
                 "invloginattempts, areaid, usertype "
                 "FROM user "
                 "WHERE userid = #userid#" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SaveUserData )

    ENSURE_PROPERTIES_ARE_SET( SaveUserData, "userid;activ;" )

    SQL_PATTERN( SaveUserData,
                 "UPDATE user "
                 "SET  isactiv = #activ#, invloginattempts = #invloginattempts# "
                 "WHERE userid=#userid#" )

//---------------------------------------------------------------------------

    BUILDER_DEFINITION( SelUserData )

    ENSURE_PROPERTIES_ARE_SET( SelUserData, "username;" )

    SQL_PATTERN( SelUserData,
                 "SELECT userid , username, firstname as forename, lastname as surname, emailuser, isactiv as activ, "
                 "invloginattempts, areaid, usertype "
                 "FROM user "
                 "WHERE UPPER( username ) = UPPER ( '#username#' )" )

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginUser" )

    PROPERTY_DESCRIPTION_LIST( " string username; string password; string surname; string forename; int32 userid; int32 activ;"
                               "string description; string mailuser; string emailuser; int16 changepw; int32 lastlogin; "
                               " string ntuser; int32 lastpwchange; int32 datevalidfrom; int32 invloginattempts; int32 areaid; "
                               " int32 usertype " )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_USER_BY_ID )
    SQL_BUILDER_CREATE( SelUserID )
    SQL_BUILDER_PUSH_BACK( SelUserID )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_ID_BY_NAME )
    SQL_BUILDER_CREATE( SelUserData )
    SQL_BUILDER_PUSH_BACK( SelUserData )

    ACCESS_METHOD( domMod::userManagement::lit::SAVE_USER_DATA )
    SQL_BUILDER_CREATE( SaveUserData )
    SQL_BUILDER_PUSH_BACK( SaveUserData )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_user


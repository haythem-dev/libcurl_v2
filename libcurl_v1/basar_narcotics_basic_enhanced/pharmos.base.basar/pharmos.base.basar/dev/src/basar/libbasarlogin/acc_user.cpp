//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "acc_user.h"

#include "usermgmt_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_user
{

//---------------------------------------------------------------------------
using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelUserID )

    ENSURE_PROPERTIES_ARE_SET( SelUserID, "username;" )

    SQL_PATTERN( SelUserID,
                 "SELECT userid, username, password, surname, forename, description, mailuser, "
                 "emailuser, changepw, activ, lastlogin, ntuser, lastpwchange "
                 "FROM ppuser "
                 "WHERE username='#username#'" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelUserData )

    ENSURE_PROPERTIES_ARE_SET( SelUserData, "userid;" )

    SQL_PATTERN( SelUserData,
                 "SELECT userid, username, password, surname, forename, description, mailuser, "
                 "emailuser, changepw, activ, lastlogin, ntuser, lastpwchange "
                 "FROM ppuser "
                 "WHERE userid=#userid#" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelNumOfPrevPw )

    ENSURE_PROPERTIES_ARE_SET( SelNumOfPrevPw, "userid;" )

    SQL_PATTERN( SelNumOfPrevPw,
                 "SELECT count(*) "
                 "FROM pppasswordprevious "
                 "WHERE userid=#userid#" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelMinDateInPwHistory )

    ENSURE_PROPERTIES_ARE_SET( SelMinDateInPwHistory, "userid;" )

    SQL_PATTERN( SelMinDateInPwHistory,
                 "SELECT min(datevalidfrom) "
                 "FROM pppasswordprevious "
                 "WHERE userid=#userid#" )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( InsertPrevPw )

    ENSURE_PROPERTIES_ARE_SET( InsertPrevPw, "userid; password; datevalidfrom;" )

    SQL_PATTERN( InsertPrevPw,
                 "INSERT INTO pppasswordprevious (userid, password, datevalidfrom) "
                 "VALUES (#userid#, '#password#', #datevalidfrom#) "
               )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( DeletePrevPw )

    ENSURE_PROPERTIES_ARE_SET( DeletePrevPw, "userid; datevalidfrom;" )

    SQL_PATTERN( DeletePrevPw,
                 "DELETE FROM pppasswordprevious "
                 "WHERE userid = #userid# and datevalidfrom = #datevalidfrom# "
               )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SaveUserData )

    ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE( SaveUserData,
            "userid; password; changepw; activ; lastlogin; lastpwchange;",
            "UPDATE" )

    SQL_PATTERN( SaveUserData,
                 "UPDATE ppuser "
                 "SET  password='#password#', changepw=#changepw#, activ=#activ#, lastlogin=#lastlogin#, lastpwchange=#lastpwchange# "
                 "WHERE userid=#userid#" )


//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "User" )

    PROPERTY_DESCRIPTION_LIST( "int32 userid; string username; string password; string surname; string forename; "
                               "string description; string mailuser; string emailuser; int16 changepw; "
                               "int16 activ; int32 lastlogin; string ntuser; int32 lastpwchange; int32 datevalidfrom;" )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_ID_BY_NAME )
    SQL_BUILDER_CREATE( SelUserID )
    SQL_BUILDER_PUSH_BACK( SelUserID )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_USER_BY_ID )
    SQL_BUILDER_CREATE( SelUserData )
    SQL_BUILDER_PUSH_BACK( SelUserData )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_NUMBER_OF_PREV_PASSWORDS )
    SQL_BUILDER_CREATE( SelNumOfPrevPw )
    SQL_BUILDER_PUSH_BACK( SelNumOfPrevPw )

    ACCESS_METHOD( domMod::userManagement::lit::SELECT_MIN_DATE_IN_PASSWORD_HISTORY )
    SQL_BUILDER_CREATE( SelMinDateInPwHistory )
    SQL_BUILDER_PUSH_BACK( SelMinDateInPwHistory )

    ACCESS_METHOD( domMod::userManagement::lit::SAVE_PREV_PASSWORD )
    SQL_BUILDER_CREATE( InsertPrevPw )
    SQL_BUILDER_PUSH_BACK( InsertPrevPw )

    ACCESS_METHOD( domMod::userManagement::lit::DELETE_PREV_PASSWORD )
    SQL_BUILDER_CREATE( DeletePrevPw )
    SQL_BUILDER_PUSH_BACK( DeletePrevPw )

    ACCESS_METHOD( domMod::userManagement::lit::SAVE_USER_DATA )
    SQL_BUILDER_CREATE( SaveUserData )
    SQL_BUILDER_PUSH_BACK( SaveUserData )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_user


#include "adloginpreviousacc.h"

#include "application_definitions.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarproperty_propertystateset.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

//----------------------------------------------------------------------------
namespace acc_adloginprevious
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelPrevious )

    ENSURE_PROPERTIES_ARE_SET( SelPrevious, "applicationid; globaluser;" )

    SQL_PATTERN( SelPrevious,
                 "SELECT ud.applicationid, ud.userid,u.username as globaluser,ud.languagemapid as languagemapid, ud.areaid as areaid, "
                 "ar.areano as areano, lg.languageid as languageid "
                 "FROM userdefault ud ,language lg,user u, area ar "
                 "WHERE ud.applicationid=#applicationid# and UPPER ( u.username ) = UPPER('#globaluser#') "
                 "and ud.languagemapid = lg.languagemapid and ud.userid = u.userid and ar.areaid = ud.areaid " )

// new functions for default data.
//---------------------------------------------------------------------------
    BUILDER_DEFINITION( DeletePreviousLogin )

    ENSURE_PROPERTIES_ARE_SET( DeletePreviousLogin, "userid; applicationid;" )

    SQL_PATTERN( DeletePreviousLogin,
                 "DELETE from currentuserlogin "
                 "WHERE userid=#userid# and applicationid = #applicationid#" )
//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SaveDefaults )

    ENSURE_PROPERTIES_ARE_SET( SaveDefaults, "userid; applicationid;languagemapid;areaid;" )

    //SQL_PATTERN( SaveDefaults,
    //             "INSERT INTO userdefault (applicationid, userid, languagemapid, areaid) "
    //             "VALUES (#applicationid#, #userid#, #languagemapid#, #areaid#)" )

    SQL_PATTERN( SaveDefaults,
                 "merge into userdefault ud "
                 "using (select first 1 #userid# as userid, #applicationid# as applicationid from systables) org "
                 "on ud.userid = org.userid and ud.applicationid = org.applicationid "
                 "when matched then update set ud.languagemapid = #languagemapid#, ud.areaid = #areaid# "
                 "when not matched then insert ( ud.userid, ud.applicationid, ud.languagemapid, ud.areaid ) values (#userid#, #applicationid#, #languagemapid#, #areaid#) " )

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SaveCurrentLogin )

    ENSURE_PROPERTIES_ARE_SET( SaveCurrentLogin, "userid; applicationid; logintime; areaid;" )

    //SQL_PATTERN( SaveCurrentLogin,
    //             "INSERT INTO currentuserlogin (userid, applicationid, logintime, areaid ) "
    //             "VALUES (#userid#, #applicationid#, '#logintime#' , #areaid# ) " )

    SQL_PATTERN( SaveCurrentLogin,
                "merge into currentuserlogin cul "
                "using (select first 1 #userid# as userid, #applicationid# as applicationid from systables) cl "
                "on cul.userid = cl.userid and cul.applicationid = cl.applicationid "
                "when matched then update set cul.logintime = current, cul.areaid = #areaid# "
                "when not matched then insert (cul.userid, cul.applicationid, cul.logintime, cul.areaid) values (#userid#, #applicationid#, current, #areaid# )" )


//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginPrevious" )

    PROPERTY_DESCRIPTION_LIST( "int32 applicationid; string globaluser; int32 userid; string languageid;int32 areaid;int32 areano; int32 branchregionid; "
                               " string languagename; string areaname;string logintime;int32 languagemapid" )

    ACCESS_METHOD( domMod::previous::lit::SELECT_PREVIOUS_BY_PATTERN )
    SQL_BUILDER_CREATE( SelPrevious )
    SQL_BUILDER_PUSH_BACK( SelPrevious )

    ACCESS_METHOD( domMod::previous::lit::SAVE_PREVIOUS )

    SQL_BUILDER_CREATE( SaveCurrentLogin )
    SQL_BUILDER_PUSH_BACK( SaveCurrentLogin )

    SQL_BUILDER_CREATE( SaveDefaults )
    SQL_BUILDER_PUSH_BACK( SaveDefaults )

    ACCESS_METHOD( domMod::previous::lit::DELETE_PREVIOUS )
    SQL_BUILDER_CREATE( DeletePreviousLogin )
    SQL_BUILDER_PUSH_BACK( DeletePreviousLogin )

    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_previous

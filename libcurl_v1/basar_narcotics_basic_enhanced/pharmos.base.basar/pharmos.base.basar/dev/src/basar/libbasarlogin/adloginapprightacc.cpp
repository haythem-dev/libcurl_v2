#include "adloginapprightacc.h"

#include "loggerpool.h"

#include "usermgmt_definitions.h"

#include "libbasarcmnutil_logging.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_accessmethod.h"

#include <iomanip>

//----------------------------------------------------------------------------
namespace acc_adLoginAppRight
{

//---------------------------------------------------------------------------
    using basar::db::aspect::SQLStringBuilder;

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAppRights )

    bool SelAppRights::isExecutable() const
    {
        using basar::login::LoggerPool;

        bool executeable = arePropertiesSet( "applicationid; userid; areaid;" );

        BLOG_DEBUG_STREAM( LoggerPool::adLoginUserMgmt(), "StringBuilder SelAppRights' excuteability: <" << std::boolalpha << executeable << ">" );

        return executeable;
    }

    void SelAppRights::buildSQLString()
    {
        using basar::login::LoggerPool;

        static const basar::VarString constStmt = "SELECT  rg.userid as userid, rg.groupid groupid,rg.applicationid as applicationid, "
                "rg.moduleid as moduleid, rg.areaid as areaid, rg.rightid as rightid, amr.rightname as rightname "
                "FROM rightsgrant rg, appmoduleright amr "
                "WHERE rg.applicationid=#applicationid# and rg.userid=#userid# and rg.areaid = #areaid# and rg.rightid=amr.rightid "
                "UNION "
                "SELECT distinct rg.userid as userid ,agu.groupid as groupid,agu.applicationid as applicationid , "
                "rg.moduleid as moduleid ,rg.areaid as areaid,rg.rightid as rightid, amr.rightname as rightname "
                "FROM appgroupuser agu, rightsgrant rg, appmoduleright amr "
                "WHERE rg.applicationid = agu.applicationid and rg.groupid = agu.groupid and rg.rightid=amr.rightid and "
                "rg.applicationid =#applicationid# and rg.areaid = #areaid# and agu.userid =#userid# ";

        resolve( constStmt );

        BLOG_DEBUG_STREAM( LoggerPool::adLoginUserMgmt(), "StringBuilder SelAppRights' sql-string: <" << getSQLString() << ">" );
    }

//---------------------------------------------------------------------------
    BUILDER_DEFINITION( SelAppUserRights )

    bool SelAppUserRights::isExecutable() const
    {
        using basar::login::LoggerPool;

        bool executeable = arePropertiesSet( "applicationid; userid; " );

        BLOG_DEBUG_STREAM( LoggerPool::adLoginUserMgmt(), "StringBuilder SelAppUserRights' excuteability: <" << std::boolalpha << executeable << ">" );

        return executeable;
    }

    void SelAppUserRights::buildSQLString()
    {
        using basar::login::LoggerPool;

        static const basar::VarString constStmt = "SELECT rg.userid as userid, rg.groupid groupid,rg.applicationid as applicationid, "
                "am.moduleno::INTEGER as moduleid, a.areano as areaid, rg.rightid as rightid, amr.rightname as rightname "
                "FROM rightsgrant rg join appmoduleright amr on rg.rightid=amr.rightid join appmodule am on am.moduleid = amr.moduleid join area a on rg.areaid = a.areaid "
                "WHERE rg.applicationid=#applicationid# and rg.userid=#userid# "
                "UNION "
                "SELECT distinct rg.userid as userid, agu.groupid as groupid, agu.applicationid as applicationid, am.moduleno::INTEGER as moduleid, a.areano as areaid, "
                "rg.rightid as rightid, amr.rightname as rightname "
                "FROM appgroupuser agu join rightsgrant rg on rg.groupid = agu.groupid join appmoduleright amr "
                "on rg.rightid=amr.rightid join appmodule am on am.moduleid = amr.moduleid join area a on rg.areaid = a.areaid "
                "WHERE rg.applicationid = #applicationid# and agu.userid = #userid# ";

        resolve( constStmt );

        BLOG_DEBUG_STREAM( LoggerPool::adLoginUserMgmt(), "StringBuilder SelAppRights' sql-string: <" << getSQLString() << ">" );
    }

//---------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION( "ADLoginAppRight" )

    PROPERTY_DESCRIPTION_LIST( "int32 groupid; int32 userid; int32 applicationid; "
                               "int32 moduleid; string rightname; int32 areaid; int32 rightid;" )

    ACCESS_METHOD( domMod::appRight::lit::SELECT_RIGHTS_BY_PATTERN )
    SQL_BUILDER_CREATE( SelAppRights )
    SQL_BUILDER_PUSH_BACK( SelAppRights )
    SQL_BUILDER_CREATE( SelAppUserRights )
    SQL_BUILDER_PUSH_BACK( SelAppUserRights )


    END_ACCESSOR_DEFINITION

//----------------------------------------------------------------------------
}//acc_appright

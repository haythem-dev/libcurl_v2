//----------------------------------------------------------------------------
/*! \file
 *  \brief  domain module for user specific handling
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "usermanagementdm.h"

#include "libbasardbaspect_manager.h"
#include "loggerpool.h"
#include "libbasarlogin_definitions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace domMod
{

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    UserManagement::UserManagement() : BaseUserManagement()
    {
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    UserManagement::~UserManagement()
    {

    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    void UserManagement::init( const basar::db::aspect::ConnectionRef activeConn )
    {
        m_UserACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_UserInst",      "User",     activeConn );
        m_AppRightACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_AppRightInst",  "AppRight", activeConn );

        if ( basar::login::LoggerPool::userManagement().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
        {
            basar::VarString msg = "usermanagement initialized.";
            basar::login::LoggerPool::userManagement().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
        }
    }

//----------------------------------------------------------------------------
}//domMod

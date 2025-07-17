#include "adloginusermgmtdm.h"

#include "usermgmt_definitions.h"

#include "libbasardbaspect_manager.h"
#include "loggerpool.h"
#include "libbasarlogin_definitions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

using namespace basar::login;
using namespace log4cplus;

//----------------------------------------------------------------------------
namespace domMod
{

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    ADLoginUserMgmt::ADLoginUserMgmt() : BaseUserManagement()
    {
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    ADLoginUserMgmt::~ADLoginUserMgmt()
    {
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    void ADLoginUserMgmt::init( const basar::db::aspect::ConnectionRef activeConn )
    {
        m_UserACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_UserInst",      "ADLoginUser",      activeConn );
        m_AppRightACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_AppRightInst",  "ADLoginAppRight", activeConn );

        BLOG_DEBUG( LoggerPool::baseApplManagement(), "usermanagement initialized." );
    }

    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ADLoginUserMgmt::findUserRightsByPattern( const basar::cmnutil::ParameterList& where )
    {
        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_AppRightACC.execute( domMod::appRight::lit::SELECT_RIGHTS_BY_PATTERN, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findUserRightsByPattern: " << e.what() );

            throw;
        }

        return m_AppRightACC.getPropertyTable();
    }

//----------------------------------------------------------------------------
}//domMod

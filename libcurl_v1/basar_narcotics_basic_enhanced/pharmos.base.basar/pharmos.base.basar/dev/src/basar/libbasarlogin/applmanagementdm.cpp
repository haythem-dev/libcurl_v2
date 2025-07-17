//----------------------------------------------------------------------------
/*! \file
 *  \brief  domain module for application specific handling
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#include "applmanagementdm.h"

#include "application_definitions.h"

#include "libbasardbaspect_manager.h"
#include "loggerpool.h"
#include "libbasarlogin_exceptions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

using namespace basar::login;
using namespace log4cplus;

//----------------------------------------------------------------------------
namespace domMod
{

//----------------------------------------------------------------------------
    ApplManagement::ApplManagement()
    {
    }

//----------------------------------------------------------------------------
    ApplManagement::~ApplManagement()
    {

    }

//----------------------------------------------------------------------------
    void ApplManagement::init( const basar::db::aspect::ConnectionRef activeConn )
    {
        basar::db::aspect::CachingPolicyEnum eCachePolicyFull       = basar::db::aspect::FULL_CACHING;
        basar::db::aspect::CachingPolicyEnum eCachePolicyOnDemand   = basar::db::aspect::ON_DEMAND_CACHING;

        m_ApplACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance
                          ( "Acc_ApplInst",        "Appl",     activeConn, eCachePolicyOnDemand );
        m_LanguageACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance
                          ( "Acc_LanguageInst",    "Language", activeConn, eCachePolicyOnDemand );
        m_AreaACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance
                          ( "Acc_AreaInst",        "Area",     activeConn, eCachePolicyFull );
        m_PreviousACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance
                          ( "Acc_PreviousInst",    "Previous", activeConn, eCachePolicyOnDemand );
        m_ParaACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance
                          ( "Acc_ParaInst",        "Para",     activeConn, eCachePolicyOnDemand );

        BLOG_DEBUG( LoggerPool::applManagement(), "applmanagement initialized." );
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ApplManagement::findApplicationLanguage ( basar::db::aspect::AccessorPropertyTable_YIterator )
    {
        return m_LanguageACC.getPropertyTable();
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ApplManagement::findAreaByPattern   ( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        try
        {
            m_AreaACC.execute( domMod::area::lit::SELECT_BRANCH_BY_APPLICATION_ID, yit );
            m_AreaACC.execute( domMod::area::lit::SELECT_REGION_BY_APPLICATION_ID, yit, false );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::applManagement(), "exception during findAreaByPattern: " << e.what() );

            throw;
        }

        return m_AreaACC.getPropertyTable();
    }

//----------------------------------------------------------------------------
    /*! \throw not implemented exception
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef ApplManagement::findGlobalArea()
    {
        return BaseApplManagement::findGlobalArea();
    }

//----------------------------------------------------------------------------
    /*! \brief update / insert single previous data
        \n throws exception */
    basar::BULong ApplManagement::savePrevious( const basar::db::aspect::AccessorPropertyTable_YIterator previousData  )
    {
        return BaseApplManagement::savePrevious( previousData );
    }

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
}//domMod

#include "adloginapplmgmtdm.h"

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
    ADLoginApplMgmt::ADLoginApplMgmt()
    {
    }

//----------------------------------------------------------------------------
    ADLoginApplMgmt::~ADLoginApplMgmt()
    {

    }

//----------------------------------------------------------------------------
    void ADLoginApplMgmt::init( const basar::db::aspect::ConnectionRef activeConn )
    {
        basar::db::aspect::CachingPolicyEnum eCachePolicyFull       = basar::db::aspect::FULL_CACHING;
        basar::db::aspect::CachingPolicyEnum eCachePolicyOnDemand   = basar::db::aspect::ON_DEMAND_CACHING;

        m_ApplACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_ApplInst",      "ADLoginAppl",      activeConn, eCachePolicyOnDemand );
        m_LanguageACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_LanguageInst",  "ADLoginLanguage",  activeConn, eCachePolicyOnDemand );
        m_AreaACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_AreaInst",      "ADLoginArea",      activeConn, eCachePolicyFull );
        m_PreviousACC   = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_PreviousInst",  "ADLoginPrevious",  activeConn, eCachePolicyOnDemand );
        m_ParaACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_ParaInst",      "ADLoginPara",      activeConn, eCachePolicyOnDemand );

        BLOG_DEBUG( LoggerPool::adLoginApplMgmt(), "adloginapplmgmt initialized." );
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ADLoginApplMgmt::findAreaByPattern  ( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        try
        {
            m_AreaACC.execute( domMod::area::lit::SELECT_AREA_BY_APPLICATION_ID, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::adLoginApplMgmt(), "exception during findAreaByPattern: " << e.what() );
            throw;
        }

        return m_AreaACC.getPropertyTable();
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ADLoginApplMgmt::findGlobalArea()
    {
        try
        {
            m_AreaACC.execute( domMod::area::lit::SELECT_GLOBAL_AREA );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::adLoginApplMgmt(), "exception during findGlobalArea: " << e.what() );

            throw;
        }

        return m_AreaACC.getPropertyTable();
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef
    ADLoginApplMgmt::findApplicationLanguage    ( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        try
        {
            m_LanguageACC.execute( domMod::language::lit::SELECT_APPLICATION_LANGUAGE, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::adLoginApplMgmt(), "exception during findApplicationLanguage: " << e.what() );

            throw;
        }

        return m_LanguageACC.getPropertyTable();
    }

    basar::BULong ADLoginApplMgmt::savePrevious ( const basar::db::aspect::AccessorPropertyTable_YIterator previousData )
    {
        m_PreviousACC.beginTransaction();

        basar::BULong affRows = 0;

        try
        {
            affRows = m_PreviousACC.execute( domMod::previous::lit::SAVE_PREVIOUS, previousData ).getAffectedRows();
        }
        catch ( basar::Exception& e )
        {
            BLOG_ERROR_STREAM( LoggerPool::adLoginApplMgmt(), "exception during findApplicationLanguage: " << e.what() );

            throw;
        }

        m_PreviousACC.commitTransaction();

        return affRows;
    }

//----------------------------------------------------------------------------
}//domMod

#include "baseapplmgmtdm.h"
#include "libbasardbaspect_manager.h"
#include "configuration.h"

#include "application_definitions.h"

#include "adloginapplmgmtdm.h"
#include "applmanagementdm.h"
#include "libbasarlogin_exceptions.h"
#include "loggerpool.h"

using namespace basar::login;
using namespace log4cplus;

//----------------------------------------------------------------------------
namespace domMod
{

//----------------------------------------------------------------------------


    BaseApplManagement::BaseApplManagement()
    {
    }

    BaseApplManagement::~BaseApplManagement()
    {
    }

    void BaseApplManagement::shutdown()
    {
        m_ApplACC.reset();
        m_LanguageACC.reset();
        m_AreaACC.reset();
        m_PreviousACC.reset();
        m_ParaACC.reset();
    }

    basar::db::aspect::AccessorPropertyTableRef BaseApplManagement::findGlobalArea()
    {
        throw basar::cmnutil::BasarNotImplementedException( basar::ExceptInfo( "BaseApplManagement::findGlobalArea", "Method not implemented", __FILE__, __LINE__ ) );
    }

    basar::db::aspect::AccessorPropertyTable_YIterator  BaseApplManagement::findApplIDByName
    ( basar::I18nString& rApplName )
    {
        try
        {
            checkApplNameLength( rApplName );

            rApplName.upper();
            rApplName.trim();

            basar::I18nString Where = "applicationname=";
            Where.append( rApplName );
            Where.append( ";" );
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( Where );

            m_ApplACC.execute( domMod::application::lit::SELECT_APPLICATION_ID_BY_PATTERN, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findApplIDByName: " << e.what() );

            throw;
        }

        return m_ApplACC.getPropertyTable().begin();
    }

    /*! \brief check length of application name
        \n InvalidApplNameException */
    void BaseApplManagement::checkApplNameLength( const basar::I18nString& rApplName )
    {
        if ( rApplName.length() > LENGTH_APPNAME || rApplName.empty() )
        {
            basar::VarString msg;
            msg.format( "ApplName <%s> may not be empty or longer than 30 characters!", rApplName.c_str() );
            throw basar::login::InvalidApplNameException(
                basar::ExceptInfo( "basar.login.CheckAppName::checkLength()",
                                   msg.c_str(), __FILE__, __LINE__ ) );
        }
    }

// language
    /*! \brief select languages by pattern (parameterlist)
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef BaseApplManagement::findLanguageByPattern( const basar::cmnutil::ParameterList& where )
    {
        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_LanguageACC.execute( domMod::language::lit::SELECT_LANGUAGE_BY_APPLICATION_ID, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findLanguageByPattern: " << e.what() );

            throw;
        }

        return m_LanguageACC.getPropertyTable();
    }

    /*! \brief select languages by pattern (yiterator)
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef         BaseApplManagement::findLanguageByPattern
    ( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        try
        {
            m_LanguageACC.execute( domMod::language::lit::SELECT_LANGUAGE_BY_APPLICATION_ID, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findLanguageByPattern: " << e.what() );

            throw;
        }

        return m_LanguageACC.getPropertyTable();
    }

    const basar::db::aspect::AccessorPropertyTableRef   BaseApplManagement::getLanguagePropTab      () const
    {
        return m_LanguageACC.getPropertyTable();
    }

// area
    /*! \brief select areas by pattern (parameterlist)
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef         BaseApplManagement::findAreaByPattern
    ( const basar::cmnutil::ParameterList& where )
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

        return findAreaByPattern( yit );
    }

    const basar::db::aspect::AccessorPropertyTableRef   BaseApplManagement::getAreaPropTab          () const
    {
        return m_AreaACC.getPropertyTable();
    }

    /*! \brief select single area by ID
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseApplManagement::findAreaNameByID        ( const basar::Int32 areaID )
    {
        try
        {
            //acc is using areano in case of ad login
            basar::I18nString Where;
            Where.format( "areaid=%d;", areaID );

            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( Where );

            m_AreaACC.execute( domMod::area::lit::SELECT_AREA_BY_ID, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception in findAreaNameByID: " << e.what() );

            throw;
        }

        return m_AreaACC.getPropertyTable().begin();
    }

// previous
    /*! \brief select single previous data by pattern (parameterlist)
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseApplManagement::findPreviousByPattern
    ( const basar::cmnutil::ParameterList& where )
    {
        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_PreviousACC.execute( domMod::previous::lit::SELECT_PREVIOUS_BY_PATTERN, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findPreviousByPattern: " << e.what() );

            throw;
        }

        return m_PreviousACC.getPropertyTable().begin();
    }

    /*! \brief select single previous data by pattern (yiterator)
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseApplManagement::findPreviousByPattern
    ( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        try
        {
            m_PreviousACC.execute( domMod::previous::lit::SELECT_PREVIOUS_BY_PATTERN, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findPreviousByPattern: " << e.what() );

            throw;
        }

        return m_PreviousACC.getPropertyTable().begin();
    }

    /*! \brief update / insert single previous data
        \n no-throw */
    basar::BULong BaseApplManagement::savePrevious( const basar::db::aspect::AccessorPropertyTable_YIterator previousData )
    {
        return m_PreviousACC.execute( domMod::previous::lit::SAVE_PREVIOUS, previousData ).getAffectedRows();
    }

    /*! \brief get empty yiterator, marked for insert in accessor-previous-propertytable
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseApplManagement::addEmptyPrevious()
    {
        return m_PreviousACC.getPropertyTable().insert( basar::FOR_INSERT );
    }

// parameter
    /*! \brief select parameters by application Name
        \return basar::db::aspect::AccessorPropertyTableRef containing applications parameters
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef         BaseApplManagement::findParaByApplID        ( const basar::Int32 applID )
    {
        try
        {
            basar::I18nString where;
            where.format( "applicationid=%d;", applID );
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_ParaACC.execute( domMod::parameter::lit::SELECT_PARAMETER_BY_APPLICATION_ID, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findParaByApplID: " << e.what() );

            throw;
        }

        return m_ParaACC.getPropertyTable();
    }

    /*! \brief select parameters by application Name
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef BaseApplManagement::findParaByApplName( basar::I18nString sApplName )
    {
        try
        {
            checkApplNameLength( sApplName );

            basar::I18nString Where = "applicationname=";
            Where.append( sApplName );
            Where.append( ";" );

            basar::db::aspect::AccessorPropertyTable_YIterator yit =
                basar::db::aspect::Manager::getInstance().createAccessorYIterator( Where );

            m_ParaACC.execute( domMod::parameter::lit::SELECT_PARAMETER_BY_APPLICATION_NAME, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseApplManagement(), "exception during findParaByApplName: " << e.what() );

            throw;
        }

        return m_ParaACC.getPropertyTable();
    }

    /*! \brief get property table for parameters
        \n no-throw */
    const basar::db::aspect::AccessorPropertyTableRef   BaseApplManagement::getParaPropTab          () const
    {
        return m_ParaACC.getPropertyTable();
    }
//----------------------------------------------------------------------------
}//domMod

//----------------------------------------------------------------------------
/*! \file
 *  \brief  domain module for text specific handling
 *  \author Thomas Hörath
 *  \date   23.12.2005
 */
//----------------------------------------------------------------------------


#include "textmanagementdm.h"

#include "application_definitions.h"

#include "libbasardbaspect_manager.h"
#include "loggerpool.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace domMod {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
TextManagement::TextManagement() 
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
TextManagement::~TextManagement()
{
}

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    void TextManagement::init( const basar::db::aspect::ConnectionRef activeConn )
    {
        m_TextACC       = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_TextInst",      "Text",     activeConn );

        BLOG_DEBUG( basar::login::LoggerPool::textManagement(), "textmanagement initialized." );
    }

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void TextManagement::shutdown()
{
	m_TextACC.reset();
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void TextManagement::setApplID	(const basar::Int16		applID)
{
	m_ApplID = applID;

        BLOG_DEBUG_STREAM( basar::login::LoggerPool::textManagement(), "ApplID set to <" << m_ApplID << ">." );
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    void TextManagement::setModID ( const basar::Int16       moduleID )
    {
        m_ModuleID = moduleID;

        BLOG_DEBUG_STREAM( basar::login::LoggerPool::textManagement(), "ModuleID set to <" << m_ModuleID << ">." );
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw */
    void TextManagement::setLanguageID  ( const basar::I18nString& languageID )
    {
        m_LanguageID = languageID;

        BLOG_DEBUG_STREAM( basar::login::LoggerPool::textManagement(), "LangID set to <" << m_LanguageID << ">." );
    }

//----------------------------------------------------------------------------
    /*! \throw no-throw
        \return iterator, containing result of single select */
    basar::db::aspect::AccessorPropertyTable_YIterator
    TextManagement::findTextByRefText   ( const basar::I18nString& rRefText )
    {
        try
        {
            basar::I18nString Where;
            Where.format( "applicationid=%d; moduleid=%d;", m_ApplID, m_ModuleID );
            basar::cmnutil::ParameterList pl( Where );
            pl.push_back( "languageid", m_LanguageID );
            pl.push_back( "reftext", rRefText );

            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( pl );

            m_TextACC.execute( domMod::text::lit::SELECT_TEXT_BY_PATTERN, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( basar::login::LoggerPool::textManagement(), "exception during findTextByRefText: " << e.what() );

            throw;
        }

        return m_TextACC.getPropertyTable().begin();
    }

//----------------------------------------------------------------------------
}//domMod

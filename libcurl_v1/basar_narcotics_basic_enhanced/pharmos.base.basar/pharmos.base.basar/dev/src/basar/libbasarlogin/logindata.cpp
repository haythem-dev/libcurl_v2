//----------------------------------------------------------------------------
/*! \file
 *  \brief  object holding all login-specific data
 *  \author Thomas Hörath
 *  \date   13.12.2005
 */
//----------------------------------------------------------------------------
#include "logindata.h"

#include "libbasarlogin_definitions.h"
#include "loggerpool.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
        /*! \throw  no-throw
            \return pointer to LoginData itself */
        boost::shared_ptr< LoginData > LoginData::create()
        {
            boost::shared_ptr< LoginData > t = boost::shared_ptr< LoginData >( new LoginData );
            return t;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginData::LoginData() : m_LanguageID ( STD_LANGUAGE )
        {
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginData::~LoginData()
        {
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
//! \param applRights todo
//! \param ADLogin todo
        void LoginData::insertRights( basar::db::aspect::AccessorPropertyTableRef applRights, const bool ADLogin )
        {
            I18nString corModBranchRight;

            for ( db::aspect::AccessorPropertyTable_YIterator yit = applRights.begin();
                    !yit.isEnd();
                    ++yit )
            {
                if ( ADLogin )
                {
                    corModBranchRight = formatRightEntry( yit.getInt32 ( "moduleid" ),
                                                          yit.getString( "rightname" ),
                                                          yit.getInt32 ( "areaid" ) );
                }
                else
                {
                    corModBranchRight = formatRightEntry( yit.getInt16 ( "moduleid" ),
                                                          yit.getString( "rightname" ),
                                                          yit.getInt32 ( "branchregionid" ) );
                }

                m_CollRights.insert( CollRights::value_type( corModBranchRight ) );
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if right exists
            \return otherwise returns false */
        bool LoginData::findRight ( const basar::Int32        moduleID,
                                    const basar::I18nString& rRight,
                                    const basar::Int32        areaID ) const
        {
            I18nString corModAreaRight = formatRightEntry( moduleID, rRight, areaID );

            CollRights::const_iterator collIter = m_CollRights.find ( corModAreaRight );

            if ( collIter  == m_CollRights.end() )
            {
                if ( LoggerPool::loginData().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "right <%s> not found for user <%s>, appl <%s>.",
                                corModAreaRight.c_str(),
                                m_UserName     .c_str(),
                                m_ApplName     .c_str() );
                    LoggerPool::loginData().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                return false; //no right found
            }

            return true;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginData::insertParameters( basar::db::aspect::AccessorPropertyTableRef parameters )
        {
            I18nString key;
            I18nString value;

            m_CollPara.clear();

            for ( db::aspect::AccessorPropertyTable_YIterator yit = parameters.begin();
                    !yit.isEnd();
                    ++yit )
            {
                key = formatParaKey( yit.getString( "para1" ),
                                     yit.getString( "para2" ),
                                     yit.getString( "parabez" ) );
                value = yit.getString( "paravalue" );

                m_CollPara.insert( CollParamter::value_type( key, value ) );
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginData::findParameter ( const basar::I18nString& rPara1,
                                        const basar::I18nString& rPara2,
                                        const basar::I18nString& rParaName,
                                        basar::I18nString& value    ) const
        {
            I18nString key = formatParaKey( rPara1, rPara2, rParaName );

            CollParamter::const_iterator collIter = m_CollPara.find ( key );

            if ( collIter  == m_CollPara.end() )
            {
                if ( LoggerPool::loginData().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "parameter <%s> not found for appl <%s>.",
                                key       .c_str(),
                                m_ApplName.c_str() );
                    LoggerPool::loginData().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                return false; //no parameter found
            }
            else
            {
                value = collIter->second;
                return true;
            }
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
        void LoginData::insertBranches( basar::db::aspect::AccessorPropertyTableRef branches, const bool ADLogin )
        {
            for ( db::aspect::AccessorPropertyTable_YIterator yit = branches.begin();
                    !yit.isEnd();
                    ++yit )
            {
                if ( ADLogin )
                {
                    m_CollBranches.insert( CollBranches::value_type( yit.getInt32 ( "areaid"  ),
                                           yit.getString( "areaname" ) ) );
                }
                else
                {

                    m_CollBranches.insert( CollBranches::value_type( yit.getInt16 ( "areaid"  ),
                                           yit.getString( "areaname" ) ) );
                }
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return all areas for the current application */
        const CollBranches& LoginData::getAllAreas() const
        {
            return m_CollBranches;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return all rights for the current user */
        const CollRights& LoginData::getAllRights() const
        {
            return m_CollRights;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return key for parameter collection */
        const basar::I18nString LoginData::formatParaKey( const basar::I18nString& rPara1,
                const basar::I18nString& rPara2,
                const basar::I18nString& rParaName
                                                        ) const
        {
            basar::I18nString paraKey = rPara1;
            paraKey.append( rPara2 );
            paraKey.append( rParaName );
            paraKey.upper();
            return paraKey;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return key for right collection */
        const basar::I18nString LoginData::formatRightEntry ( const basar::Int32        moduleID,
                const basar::I18nString& rRight,
                const basar::Int32        areaID
                                                            ) const
        {
            I18nString rightEntry;

            rightEntry.format( "%d.%d.", moduleID, areaID );
            rightEntry.append( rRight );
            rightEntry.upper();

            return rightEntry;
        }

//---------------------------------------------------------------------------
        bool LoginData::hasUserRightsForCurrentArea() const
        {
            //make area "key"
            std::ostringstream os;

            os << '.' << m_AreaID << '.';

            const basar::I18nString key( os.str() );

            bool found = false;

            for ( CollRights::const_iterator it = m_CollRights.begin(); it != m_CollRights.end(); ++it )
            {
                if ( std::string::npos != it->find( key ) )
                {
                    found = true;
                    break;
                }
            }

            return found;
        }

//---------------------------------------------------------------------------
        bool LoginData::hasUserAnyRights() const
        {
            return ( false == m_CollRights.empty() );
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
        void LoginData::insertUserData( const basar::db::aspect::AccessorPropertyTable_YIterator userData, const bool ADLogin )
        {
            m_UserID    = userData.getInt32 ( "userid" );
            m_UserName  = userData.getString( "username" );
            m_SurName   = userData.getString( "surname" );
            m_ForeName  = userData.getString( "forename" );
            m_Email     = userData.getString( "emailuser" );

            if ( ADLogin )
            {
                m_UserType  = basar::login::UserTypeEnum( userData.getInt32( "usertype" ) );
            }

            else
            {
                m_Mail      = userData.getString( "mailuser" );
                m_NtUser    = userData.getString( "ntuser" );
                m_CryptPW   = userData.getString( "password" );
                m_Description = userData.getString( "description" );
            }
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
        void LoginData::insertApplData( const basar::db::aspect::AccessorPropertyTable_YIterator applData )
        {
            m_ApplID    = applData.getInt32 ( "applicationid" );
            m_ApplName  = applData.getString( "applicationname" );
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
        void LoginData::insertLangAreaData ( const basar::db::aspect::AccessorPropertyTable_YIterator languageAreaData, const bool ADLogin )
        {
            if ( ADLogin )
            {
                m_LanguageMapID = languageAreaData.getInt32( "languagemapid" );
                m_AreaIDAD = languageAreaData.getInt32  ( "areaid" );
                m_AreaID = static_cast<basar::Int16>( languageAreaData.getInt32( "areano" ) );
            }
            else
            {
                m_AreaID = languageAreaData.getInt16    ( "branchregionid" );
            }

            m_LanguageID    = languageAreaData.getString( "languageid" );
            m_LanguageName  = languageAreaData.getString( "languagename" );
            m_AreaName      = languageAreaData.getString( "areaname" );
        }

//----------------------------------------------------------------------------
    }//login
}//basar

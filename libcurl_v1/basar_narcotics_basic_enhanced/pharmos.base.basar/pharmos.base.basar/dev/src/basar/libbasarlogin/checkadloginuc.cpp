#include "checkadloginuc.h"

#include "loginappl.h"
#include "crypt.h"
#include "loggerpool.h"

#include <string.h>

#ifdef _WIN32
#include "adusercheckerwindows.h"

#include <boost/scoped_array.hpp>

#include <windows.h>
#include <winldap.h>
#include <ntldap.h>

#define SECURITY_WIN32

#include <security.h>
#include <secext.h>

#else
#include <ldap.h>
#include "adusercheckerunix.h"
#endif

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
        namespace useCase
        {

//----------------------------------------------------------------------------
            /*! \param config configuration object
                \throw no-throw */
            CheckADLogin::CheckADLogin( Configurationptr config ) : CheckLoginBase( config ), m_SingleSignOnRequested( false )
            {
                /*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
                m_ADUserChecker = IADUserChecker::IADUserCheckerPtr( new ADUserCheckerWindows( getConfig() ) );
#else
                m_ADUserChecker = IADUserChecker::IADUserCheckerPtr( new ADUserCheckerUnix( getConfig() ) );
#endif

            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            CheckADLogin::~CheckADLogin()
            {
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw
                \retval true if login is ok
                \return otherwise returns false */
            bool CheckADLogin::run()
            {
                m_UserIdOrPasswordEmpty = false;
                m_hasRightsForSeletedArea = true;

                bool ret;

                basar::I18nString Where = "username=";
                Where.append( getAppl()->m_currentUser.getString( "username" ) );
                Where.append( ";" );

                basar::cmnutil::ParameterList pl( Where );

                getAppl()->m_currentUser = getAppl()->m_UserDM->findUserIDByPattern( pl );

                if ( getAppl()->m_currentUser.isEnd() == true )
                {
                    if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "no userid found to <%s>", Where.c_str() );
                        LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }

                    return false; //userId doesn't exist.
                }

                basar::I18nString usrName;

                if ( m_SingleSignOnRequested )
                {
                    ret = m_ADUserChecker->isAccountActive( getUserLogonName() );
                }
                else
                {
                    usrName = getAppl()->m_currentUser.getString( "username" );

                    // Check if userid or password field empty.
                    if ( CheckLoginBase::getPassword().empty() || usrName.empty() )
                    {
                        m_UserIdOrPasswordEmpty = true;
                        return false; //
                    }

                    basar::cmnutil::CollBString domainUser = usrName.tokenize( "\\" );

                    ret = m_ADUserChecker->doLogin( domainUser, CheckLoginBase::getPassword() );
                }

                //only check rights if user passed authentication
                if ( true == ret )
                {
                    if ( getAppl()->m_currentUser.getInt32( "activ" ) == ACCOUNT_INACTIV )
                    {
                        if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                        {
                            VarString msg;
                            msg.format( "account to <%s> is inactiv.", Where.c_str() );
                            LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                        }

                        return false; // user inactiv
                    }

                    return hasUserRight();
                }

                //auth against AD failed
                return ret;
            }

            void CheckADLogin::setLoginMode( bool singleSignOnRequested )
            {
                m_SingleSignOnRequested = singleSignOnRequested;
            }

            bool CheckADLogin::isUserIdOrPasswordEmpty()
            {
                return m_UserIdOrPasswordEmpty;
            }

            bool CheckADLogin::isRightAvailable()
            {
                return m_hasRightsForSeletedArea;
            }

            bool CheckADLogin::hasUserRight()
            {
                I18nString Where;

                Where.format( "applicationid=%d; userid=%d; areaid = %d", getAppl()->m_currentAppl.getInt32( 0 ),
                              getAppl()->m_currentUser.getInt32( "userid" ), getAppl()->m_currentLangArea.getInt32( "areaid" ) );

                cmnutil::ParameterList pl( Where );

                getAppl()->m_currentRights = getAppl()->m_UserDM->findRightsByPattern( pl );

                basar::VarString applicationName = getAppl()->m_currentAppl.getString( "applicationname" );

                if ( applicationName.compare( USER_MGMT_APP_NAME ) != 0 )
                {
                    getAppl()->m_currentRights = getAppl()->m_UserDM->findRightsByPattern( pl );

                    if ( getAppl()->m_currentRights.begin().isEnd() )
                    {
                        m_hasRightsForSeletedArea = false;
                    }
                }

                getAppl()->m_currentRights = getAppl()->m_UserDM->findUserRightsByPattern( pl );

                return m_hasRightsForSeletedArea;
            }

            basar::VarString CheckADLogin::getUserLogonName() const
            {
                basar::VarString userLogonName;

                /*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
                {
                    EXTENDED_NAME_FORMAT NameFormat = NameSamCompatible;

                    boost::scoped_array<char> name( new char[MAX_PATH] );

                    ULONG cchDN = MAX_PATH;

                    if ( 0 == ::GetUserNameEx( NameFormat, name.get(), &cchDN ) )
                    {
                        //todo: unable to retrieve user name
                    }

                    userLogonName.assign( name.get() );
                }

                basar::cmnutil::CollBString domainUser = userLogonName.tokenize( "\\" );

                userLogonName = domainUser.back();
#endif
                return userLogonName;
            }

//----------------------------------------------------------------------------
        } //useCase
    } //login
} //basar
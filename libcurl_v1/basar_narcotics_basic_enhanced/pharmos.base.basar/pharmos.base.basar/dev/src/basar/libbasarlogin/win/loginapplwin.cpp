//----------------------------------------------------------------------------
/*! \file
 *  \brief  all win-specific functions implemented of LoginAppl
 *  \author Thomas Hörath
 *  \date   26.02.2006
 */
//----------------------------------------------------------------------------

#include "loginapplwin.h"

#include "logindata.h"
#include "loggerpool.h"
#include "logindlgvc.h"
#include "changepwdlgvc.h"

#include "libbasarproperty_propertystate.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarguitie_msgboxes.h"
#include "libbasarlogin_exceptions.h"
#include "libbasarcmnutil_logging.h"
#include "libbasardbaspect_logmacros.h"

#define VC_EXTRALEAN            //!< less Visual C headers
#define WIN32_LEAN_AND_MEAN     //!< less Windows headers
#include <windows.h>

#define SECURITY_WIN32

#include <security.h>

#include <secext.h>


//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginApplWin::LoginApplWin() : m_UpdateForBackup( false ), m_UserIdFound( false )
        {
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginApplWin::~LoginApplWin()
        {
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::init ()
        {
            m_UpdateForBackup = false;
            m_UserIdFound = false;
            LoginAppl::init();
        }

        void LoginApplWin::shutdown()
        {
            m_TextDM.shutdown();
            LoginAppl::shutdown();
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::clear ()
        {
            if ( m_IsDialog )
            {
                m_currentText.clear();
                m_currentPrevious.clear();

                if ( !m_currentAreas.isNull() )
                {
                    m_currentAreas.clear();
                }

                m_currentAreas.reset();

                if ( !m_currentLanguages.isNull() )
                {
                    m_currentLanguages.clear();
                }

                m_currentLanguages.reset();
            }

            LoginAppl::clear();
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if login was successful
            \return otherwise returns false */
        bool LoginApplWin::interactiveLogin( basar::I18nString& rApplName,
                                             const basar::I18nString& rWindowTitle,
                                             const basar::Int16 MaxTrials )
        {
            //store given window title
            m_rWindowTitle = rWindowTitle;

            try
            {
                init();

                m_IsDialog = true;

                m_pLoginDlgVC = boost::shared_ptr<viewConn::LoginDlg>( new viewConn::LoginDlg );

                //----- connect ----
                Connection::Connectionptr connPtr = getConnection();
                connPtr->connect();

                m_ApplDM->init( connPtr->getActiveConn() );
                m_UserDM->init( connPtr->getActiveConn() );
                m_TextDM.init( connPtr->getActiveConn() );
                m_TextDM.setApplID      ( APPLID_BASARLOGIN );
                m_TextDM.setModID       ( MODID_BASARLOGIN );
                m_TextDM.setLanguageID  ( STD_LANGUAGE );

                //----- check ----
                m_currentAppl = m_ApplDM->findApplIDByName( rApplName );

                if ( m_currentAppl.isEnd() )
                {
                    ConstString func = "basar.login.LoginApplWin.showDlg";
                    VarString reason;
                    reason.format( "application name <%s> does not exist!", rApplName.c_str() );
                    throw InvalidApplNameException( ExceptInfo( func, reason.c_str(), __FILE__, __LINE__ ) );
                }

                //----- save prev ----
                savePrevData();

                //----- previous -----
                managePrevious();

                //----- init Login Dialog -----
                m_pLoginDlgVC->init( m_ApplDM->getAreaPropTab(), m_ApplDM->getLanguagePropTab(), rWindowTitle, getConfiguration()->getLoginType() );

                //----- areas + languages -----
                if ( ( USER_MGMT_APP_NAME == rApplName ) && getConfiguration()->isADLogin() )
                {
                    m_currentAreas      = m_ApplDM->findGlobalArea();
                }
                else
                {
                    m_currentAreas      = m_ApplDM->findAreaByPattern( m_currentAppl );
                }

                m_currentLanguages      = m_ApplDM->findLanguageByPattern( m_currentAppl );

                if ( m_currentLanguages.empty() || m_currentAreas.empty() )
                {
                    ConstString func = "basar.login.LoginApplWin.showDlg";
                    VarString reason;
                    reason.format( "no area or language for application <%s> specified!", rApplName.c_str() );
                    throw InvalidStateException( ExceptInfo( func, reason.c_str(), __FILE__, __LINE__ ) );
                }

                //----- user -----
                if ( m_PreviousLoginExists )
                {
                    if ( getConfiguration()->isADLogin() )
                    {
                        m_currentUser = m_UserDM->findUserByUsername( m_currentPrevious.getString( "globaluser" ) );
                        BLOG_YIT( LoggerPool::loginAppl(), m_currentUser, "LoginApplWin::interactiveLogin()" );
                    }
                    else
                    {
                        m_currentUser = m_UserDM->findUserByID( m_currentPrevious.getInt16( "userid" ) );
                    }

                    //---- translate languagenames for gui ----
                    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_currentLanguages.begin();

                    while ( yit.isEnd() != true )
                    {
                        yit.setString( "languagename", getLabel( yit.getString( "languagename" ).c_str() ) );
                        ++yit;
                    }
                }
                else
                {
                    m_currentUser = m_UserDM->addEmptyUser();

                    //in AD case, default to users logon name in case no previous data was found
                    if ( getConfiguration()->isADLogin() )
                    {
                        m_currentUser.setString( "username", getUserName() );
                    }
                    else
                    {
                        m_currentUser.setString( "username", "");
                    }
                }

                m_pLoginDlgVC->matchFromLoginInfo( m_currentUser, m_currentPrevious, getConfiguration()->getLoginType() );
                m_LoginTrials = MaxTrials;

                //QApplication has to be instantiated before calling this method!
                m_pLoginDlgVC->show();

                if ( false == getConfiguration()->isADLogin() )
                {
                    updateBackup();
                }

                m_pLoginDlgVC.reset();

                if ( m_LoginSuccessful == false )
                {
                    m_pDataCurrent = m_pDataPrevious;
                }
                else
                {
                    m_pDataCurrent->insertBranches( m_currentAreas, getConfiguration()->isADLogin() );

                    setI18n();      // basar settings, possibly Qt settings

                    BLOG_INFO_STREAM( LoggerPool::loginAppl(), "user <" << m_pDataCurrent->m_UserName << "> logged in for application <" << m_pDataCurrent->m_ApplName << "> interactively." );
                }

                shutdown();

            }
            catch ( basar::Exception& e )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginAppl(), "got exception: " << e.what() );
                shutdown();
                throw;
            }
            catch ( ... )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginAppl(), "got exception" );
                shutdown();
                throw;
            }

            return m_LoginSuccessful;
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void    LoginApplWin::manageLoginFailed ()
        {
            if ( getConfiguration()->isADLogin() )
            {
                if ( m_CheckLoginUC->isUserIdOrPasswordEmpty() )
                {
                    basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), "Username or password Cannot be Empty. \nPlease try again.", m_rWindowTitle.c_str() );
                    return;
                }

                m_UserIdFound = m_currentUser.isEnd() != true;
                basar::Int32 invLoginAttemps = 0;

                if ( m_UserIdFound )
                {
                    BLOG_YIT( LoggerPool::loginAppl(), m_currentUser, "LoginApplWin::manageLoginFailed ()" );

                    basar::I18nString username;
                    username = m_currentUser.getString( "username" );
                    invLoginAttemps = m_currentUser.getInt32( "invloginattempts" ) + 1;
                }

                if ( m_UserIdFound )
                {
                    if ( !m_CheckLoginUC->isRightAvailable() )
                    {
                        basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), "User has no right for selected area", m_rWindowTitle.c_str() );
                    }
                    else if ( m_currentUser.getInt32( "activ" ) == ACCOUNT_ACTIV )
                    {
                        // login dlg further on
                        basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT USERNAME OR PASSWORD" ).c_str(), m_rWindowTitle.c_str() );
                    }
                    else
                    {
                        basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "ACCOUNT_INACTIV" ).c_str(), m_rWindowTitle.c_str() );
                        m_pLoginDlgVC->hide();
                    }

                    if ( m_currentUser.getInt32( "activ" ) == ACCOUNT_ACTIV )
                    {
                        m_currentUser.setInt32( "invloginattempts", invLoginAttemps );
                        m_UserDM->saveUser( m_currentUser );
                    }
                }
                else
                {
                    // login dlg further on
                    basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT USERNAME OR PASSWORD" ).c_str(), m_rWindowTitle.c_str() );
                    m_currentUser.clear();
                    m_currentUser = m_UserDM->addEmptyUser();
                }

                //set account inactiv
                if ( m_UserIdFound && m_currentUser.getInt32( "activ" ) == ACCOUNT_ACTIV && m_currentUser.getInt32( "invloginattempts" ) > m_LoginTrials )
                {
                    m_currentUser.setInt32( "activ", ACCOUNT_INACTIV ); //also if already backup-connect!
                    m_UserDM->saveUser( m_currentUser );
                    m_UpdateForBackup = true;
                    basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT_USERNAME_MAXTRIES_OVER" ).c_str(), m_rWindowTitle.c_str() );
                    basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "ACCOUNT_DEACTIVATED" ).c_str(), m_rWindowTitle.c_str() );

                    BLOG_INFO_STREAM( LoggerPool::loginAppl(), "account for user <" << m_currentUser.getString( "username" ) << "> deactivated because number of login trials reached." );

                    m_pLoginDlgVC->hide();
                }
            }
            else
            {
                --m_LoginTrials;

                m_UserIdFound = m_currentUser.isEnd() != true;

                if ( m_LoginTrials > 0 )
                {
                    if ( m_UserIdFound )
                    {
                        if ( m_currentUser.getInt16( "activ" ) == ACCOUNT_ACTIV )
                        {
                            // login dlg further on
                            basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT USERNAME OR PASSWORD" ).c_str(), m_rWindowTitle.c_str() );
                        }
                        else
                        {
                            basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "ACCOUNT_INACTIV" ).c_str(), m_rWindowTitle.c_str() );
                            m_pLoginDlgVC->hide();
                        }
                    }
                    else
                    {
                        // login dlg further on
                        basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT USERNAME OR PASSWORD" ).c_str(), m_rWindowTitle.c_str() );

                        m_currentUser.clear();
                        m_currentUser = m_UserDM->addEmptyUser();
                    }
                }
                else
                {
                    basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "INCORRECT_USERNAME_MAXTRIES_OVER" ).c_str(), m_rWindowTitle.c_str() );

                    //set account inactiv
                    if ( m_UserIdFound )
                    {
                        m_currentUser.setInt16( "activ", ACCOUNT_INACTIV ); //also if already backup-connect!
                        m_UserDM->saveUser( m_currentUser );
                        m_UpdateForBackup = true;
                        basar::gui::tie::warningMsgBox( m_pLoginDlgVC.get(), getLabel( "ACCOUNT_DEACTIVATED" ).c_str(), m_rWindowTitle.c_str() );

                        BLOG_INFO_STREAM( LoggerPool::loginAppl(), "account for user <" << m_currentUser.getString( "username" ) << "> deactivated because number of login trials reached." );
                    }

                    m_pLoginDlgVC->hide();
                }
            }
        }

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \param singleSignOnRequested - bool indicating if login with current user is requested (= true )
        void LoginApplWin::handleLogin( bool singleSignOnRequested )
        {

            BLOG_YIT( LoggerPool::loginAppl(), m_currentUser, "LoginApplWin::handleLogin - before match" );

            m_pLoginDlgVC->matchToLoginInfo( m_currentUser, m_currentPrevious, getConfiguration()->getLoginType() );

            BLOG_YIT( LoggerPool::loginAppl(), m_currentUser, "LoginApplWin::handleLogin - after match" );

            m_currentLangArea = m_currentPrevious;
            m_TextDM.setLanguageID  ( m_currentPrevious.getString( "languageid" ) ); //for messages

            m_CheckLoginUC->setPassword( m_pLoginDlgVC->getPassword() );
            m_CheckLoginUC->setLoginMode( singleSignOnRequested );
            m_LoginSuccessful = m_CheckLoginUC->run(); //usecase

            if ( m_LoginSuccessful )
            {
                manageLoginSucceeded();
                m_pLoginDlgVC->hide();
            }
            else
            {
                manageLoginFailed();
                m_pLoginDlgVC->clearPasswordField();
            }
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::handlePwChanged()
        {
            basar::I18nString repeatedPW;
            m_pChangePwDlgVC->matchToLoginInfo( m_currentUser, repeatedPW );
            basar::I18nString newPW = m_currentUser.getString( "password" );

            if ( newPW != repeatedPW                            ||
                    repeatedPW.empty()                              ||
                    repeatedPW.length() > PASSWORD_MAX_LENGTH       ||
                    repeatedPW.length() < PASSWORD_MIN_LENGTH       ||
                    false == m_ChangePwUC.isPwComplex( repeatedPW )   ||
                    false == m_ChangePwUC.run() )
            {
                //change pw dialog further on
                basar::gui::tie::warningMsgBox( m_pChangePwDlgVC.get(), getLabel( "CHANGEPW_INCORRECT" ).c_str(), m_rWindowTitle.c_str() );
            }
            else
            {
                basar::gui::tie::infoMsgBox( m_pChangePwDlgVC.get(), getLabel( "PASSWORD CHANGED" ).c_str(), m_rWindowTitle.c_str() );

                BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "password changed for user <" << m_currentUser.getString( "username" ) << ">" );

                //close change pw dialog
                m_pChangePwDlgVC->hide();
            }
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void    LoginApplWin::manageCancel      ()
        {
            m_pLoginDlgVC->hide();

            BLOG_DEBUG( LoggerPool::loginAppl(), "login canceled." );
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::handleCancel()
        {
            manageCancel();
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::managePrevious()
        {
            basar::VarString globalUser = getUserName();
            globalUser.lower();

            I18nString WherePrev;
            WherePrev.format( "applicationid=%d; globaluser=%s;", m_currentAppl.getInt32( 0 ), globalUser.c_str() );
            basar::cmnutil::ParameterList pl( WherePrev );

            m_currentPrevious = m_ApplDM->findPreviousByPattern( pl );

            if ( m_currentPrevious.isEnd() )
            {
                m_currentPrevious = m_ApplDM->addEmptyPrevious();
                m_currentPrevious.setString( "globaluser", globalUser );
                m_currentPrevious.setString( "languageid", m_pDataCurrent->m_LanguageID ); //known from default-init or m_pDataPrevious
                m_PreviousLoginExists = false;

                if ( getConfiguration()->isADLogin() )
                {
                    m_currentPrevious.setInt32( "applicationid", static_cast<basar::Int32>( m_currentAppl.getInt32( 0 ) ) );
                }
                else
                {
                    m_currentPrevious.setInt16( "applicationid", static_cast<basar::Int16>( m_currentAppl.getInt32( 0 ) ) );
                }

                BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "no previous login data found for <" << WherePrev << ">." );
            }
            else
            {
                BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "previous login data found for <" << WherePrev << ">." );

                m_PreviousLoginExists = true;
            }

            // setLocale for I18nString
            basar::I18nString language = m_currentPrevious.getString( "languageid" );

            BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "locale = <" << language << ">." );

            // init TextManagement for reading mutlilang-textes later
            m_TextDM.setLanguageID  ( language );

            m_BtOk = getLabel( "BTOK" );
            //m_BtOk = getLabel("Info");
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return true if user pressed ok on shown dialog, false otherwise */
        bool LoginApplWin::managePwChanging()
        {
            bool result = true;

            if ( getConnection()->isBackup() == false )
            {
                m_pChangePwDlgVC = boost::shared_ptr<viewConn::ChangePwDlg>( new viewConn::ChangePwDlg );

                m_pChangePwDlgVC->init();

                if ( basar::gui::tie::WIDGET_CANCELED == m_pChangePwDlgVC->show() )
                {
                    result = false;
                }

                m_pChangePwDlgVC.reset();
            }

            return result;
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void    LoginApplWin::manageLoginSucceeded()
        {
            //write data to logindata-obj
            storeLoginData();

            //save loginprevious
            if ( getConfiguration()->isADLogin() )
            {
                m_currentPrevious.setInt32( "userid", m_currentUser.getInt32( "userid" ) );

                //Setting login time and saving user defaults.
                //why string and not datetime?
                m_currentPrevious.setString( "logintime", basar::DateTime().toString( "%Y-%m-%d %H:%M:%S" ) );

                m_currentUser.setInt32( "invloginattempts", 0 );
            }
            else
            {
                m_currentPrevious.setInt16( "userid", static_cast<basar::Int16>( m_currentUser.getInt32( "userid" ) ) );

                if ( m_currentUser.getState( "password" ) == basar::SS_UPDATE )
                {
                    m_UpdateForBackup = true;
                }

                m_currentUser.setInt32( "lastlogin", basar::cmnutil::DateTime::getCurrent().getDate() );
            }

            m_ApplDM->savePrevious( m_currentPrevious );

            // saving changepw, password, lastlogin, activ, lastpwchange
            m_UserDM->saveUser( m_currentUser );

            if ( LoggerPool::loginAppl().isEnabledFor( log4cplus::INFO_LOG_LEVEL ) )
            {
                VarString msg;

                if ( getConfiguration()->isADLogin() )
                {
                    //also log areano?
                    msg.format( "user <%s> logged in for application <%s>, area <%d (%d) - %s> via dialog.",
                                m_currentUser.getString( "username" ).c_str(),
                                m_currentAppl.getString( "applicationname" ).c_str(),
                                m_currentPrevious.getInt32( "areano" ),
                                m_currentPrevious.getInt32( "areaid" ),
                                m_currentPrevious.getString( "areaname" ).c_str() );
                    LoggerPool::loginAppl().forcedLog( log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
                else
                {
                    msg.format( "user <%s> logged in for application <%s>, area <%d - %s> via dialog.",
                                m_currentUser.getString( "username" ).c_str(),
                                m_currentAppl.getString( "applicationname" ).c_str(),
                                m_currentPrevious.getInt16( "branchregionid" ),
                                m_currentPrevious.getString( "areaname" ).c_str() );
                    LoggerPool::loginAppl().forcedLog( log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::updateBackup()
        {
            if ( getConnection()->isBackup() == false && m_UserIdFound && m_UpdateForBackup )
            {
                getConnection()->connectBackup();

                // saving changepw, password, lastlogin, activ, lastpwchange
                property::PropertyTable_XIterator xit = m_currentUser.begin();

                while ( xit != m_currentUser.end() )
                {
                    if ( xit.getState() != basar::SS_UNSET )
                    {
                        xit.setPropertyState( basar::SS_UPDATE );
                    }

                    ++xit;
                }

                m_UserDM->saveUserBackup( m_currentUser, getConnection()->getActiveConn() );

                BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "user <" << m_currentUser.getInt32( "userid" ) << "> updated on backup-machine." );
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  NoTextFoundException
            \return multilang text for labels / messages */
        const basar::I18nString LoginApplWin::getLabel( basar::I18nString refText )
        {
            basar::I18nString retText =  refText;
            refText.upper();
            refText.trim();

            m_currentText = m_TextDM.findTextByRefText( refText );

            if ( m_currentText.isEnd() != true )
            {
                return getFormattedLabel( m_currentText.getString( "text" ) );
            }

            BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "no text found for reftext <" << refText << ">, language <" << m_currentPrevious.getString( "languageid" ) << "> in pptext." );

            // search for english text
            m_TextDM.setLanguageID( STD_LANGUAGE );
            m_currentText = m_TextDM.findTextByRefText( refText );

            if ( m_currentText.isEnd() != true )
            {
                m_TextDM.setLanguageID( m_currentPrevious.getString( "languageid" ) ); //reset language to selected
                return getFormattedLabel( m_currentText.getString( "text" ) );
            }

            BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "No matching text found (selected + standard language) in pptext to reftext <" << refText << ">" );

            //return refText if neither translation was found
            return refText;

        }


//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return formatted text */
        const basar::I18nString LoginApplWin::getFormattedLabel( basar::I18nString text )
        {
            I18nString            result;
            I18nString::size_type posEnd  = text.length();
            I18nString            pattern = "\\n";
            I18nString::size_type posPat  = text.find( pattern, 0 );

            while ( posPat < posEnd && posPat >= 0 )
            {
                result += text.left( pattern );
                result.append( "\n" );
                text = text.right( pattern );
                posPat = text.find( pattern, ++posPat );
            }

            result += text;

            return result;
        }

//----------------------------------------------------------------------------
        basar::VarString LoginApplWin::getUserName() const
        {
            char globalUser[256] = {0};
            DWORD dwSize = sizeof( globalUser );

            if ( getConfiguration()->isADLogin() )
            {
                //EXTENDED_NAME_FORMAT NameFormat = NameDnsDomain;
                //EXTENDED_NAME_FORMAT NameFormat = NameSamCompatible;
                ::GetUserNameEx( NameSamCompatible, globalUser, &dwSize );
            }
            else
            {
                ::GetUserName( globalUser, &dwSize );
            }

            return globalUser;
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginApplWin::storeLoginData()
        {
            m_UserIdFound = true;

            // Login-Data in m_pDataCurrent
            m_pDataCurrent->insertUserData      ( m_currentUser, getConfiguration()->isADLogin() );
            m_pDataCurrent->insertApplData      ( m_currentAppl );
            m_pDataCurrent->insertLangAreaData  ( m_currentPrevious, getConfiguration()->isADLogin() );

            //m_pDataCurrent to Coll of successful logins
            if ( insertLoginDataColl() == true ) // new login data -> read rights + parameter
            {
                storeRightsParameters();
            }
        }

//---------------------------------------------------------------------------
} // login
} // basar


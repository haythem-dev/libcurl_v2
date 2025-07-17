//----------------------------------------------------------------------------
/*! \file
 *  \brief  all common functions (win + unix) implemented of LoginAppl
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#include "loginappl.h"

#include "logindata.h"
#include "loggerpool.h"
#include "libbasarcmnutil_locale.h"
#include "libbasarlogin_exceptions.h"

/*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
#include "loginapplwin.h"
#include "libbasarqtwidget_i18n.h"
#else
#include "loginapplunix.h"
#endif

//----------------------------------------------------------------------------
namespace basar {
namespace login {

//----------------------------------------------------------------------------
        /*! \throw  no-throw
            \return pointer to LoginAppl itself */
        LoginAppl::LoginApplSharedPtr LoginAppl::create()
        {
            /*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
            LoginApplSharedPtr t = LoginApplSharedPtr( new LoginApplWin );
#else
            LoginApplSharedPtr t = LoginApplSharedPtr( new LoginApplUnix );
#endif

            return t;
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginAppl::LoginAppl() : m_PreviousLoginExists( false ), m_LoginSuccessful( false ),
            m_IsDialog( true ), m_RequestedLoginMode( LOGIN_MODE_DEFAULT ), m_Configuration( new Configuration() )
        {
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::init ()
        {
            m_PreviousLoginExists = false;
            m_LoginSuccessful = false;
            m_IsDialog = true;

            if ( NULL == m_ConnectionPtr.get() )
            {
                m_ConnectionPtr         = basar::login::Connection::Connectionptr       ( new Connection ( m_Configuration ) );
            }

            m_UserMgmtDMFactoryPtr      = UserMgmtDMFactory::UserMgmtDMFactoryPtr       ( new UserMgmtDMFactory     ( m_Configuration ) );
            m_ApplMgmtDMFactoryPtr      = ApplMgmtDMFactory::ApplMgmtDMFactoryPtr       ( new ApplMgmtDMFactory     ( m_Configuration ) );
            m_CheckLoginUCFactoryPtr    = CheckLoginUCFactory::CheckLoginUCFactoryPtr   ( new CheckLoginUCFactory   ( m_Configuration ) );

            m_UserDM                    = m_UserMgmtDMFactoryPtr->getUserManagementDM();
            m_ApplDM                    = m_ApplMgmtDMFactoryPtr->getApplManagementDM();
            m_CheckLoginUC              = m_CheckLoginUCFactoryPtr->getCheckLoginUC();
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::shutdown ()
        {
            try
            {
                clear();

                if ( NULL != m_ApplDM.get() )
                {
                    m_ApplDM->shutdown();
                }

                if ( NULL != m_UserDM.get() )
                {
                    m_UserDM->shutdown();
                }

                getConnection()->disconnect();
            }
            catch ( basar::Exception& e )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginAppl(), "got exception during shutdown: " << e.what() );
                throw;
            }
            catch ( ... )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginAppl(), "got exception during shutdown" );
                throw;
            }
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::clear ()
        {
            m_currentAreaSingle.clear();
            m_currentUser.clear();
            m_currentAppl.clear();
            m_currentLangArea.clear();

            if ( !m_currentRights.isNull() )
            {
                m_currentRights.clear();
                m_currentRights.reset();
            }

            if ( !m_currentPara.isNull() )
            {
                m_currentPara.clear();
                m_currentPara.reset();
            }
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        LoginAppl::~LoginAppl()
        {
            shutdown();
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if no previous login exists
            \return otherwise returns false */
        bool LoginAppl::isFirstLogin() const
        {
            return !m_PreviousLoginExists;
        }

//----------------------------------------------------------------------------
        /*! \throw  InvalidParameterException */
        void LoginAppl::storeRightsParameters()
        {
            // read rights + saving in m_pDataCurrent
            std::ostringstream where;

            //default login mode
            where << "applicationid=" << m_currentAppl.getInt32( 0 ) << "; userid=" << m_currentUser.getInt32( "userid" );

            if ( LOGIN_MODE_REGION_ONLY == m_RequestedLoginMode )
            {
                if ( false == m_Configuration->isADLogin() )
                {
                    where << "; branchregionid=" <<  getAreaID();
                }
                else
                {
                    where << "; areaid=" << getAreaIDAD();
                }
            }

            if ( LOGIN_MODE_REGION_ONLY != m_RequestedLoginMode && LOGIN_MODE_DEFAULT != m_RequestedLoginMode )
            {
                std::ostringstream os;
                os << "Encountered unknown requested login mode: <" << m_RequestedLoginMode << ">";

                throw InvalidParameterException( ExceptInfo( "basar.login.LoginApplWin::storeLoginData", os.str(), __FILE__, __LINE__ ) );
            }

            // read rights + saving in m_pDataCurrent
            cmnutil::ParameterList pl( where.str() );
            m_currentRights = m_UserDM->findRightsByPattern( pl );

            m_pDataCurrent->insertRights( m_currentRights, m_Configuration->isADLogin() );

            // read parameter + saving in m_pDataCurrent
            m_currentPara = m_ApplDM->findParaByApplID( m_currentAppl.getInt32( 0 ) );
            m_pDataCurrent->insertParameters( m_currentPara );
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if logindata-obj already exists in collection
            \return otherwise returns false */
        bool LoginAppl::insertLoginDataColl()
        {
            I18nString key;
            generateLoginDataCollKey( key );

            CollLoginData::const_iterator collIter;

            collIter = m_CollLoginData.find ( key );

            if ( collIter  != m_CollLoginData.end() ) //key found
            {
                if ( m_pDataCurrent != collIter->second )
                {
                    //area + language from former login
                    if ( m_Configuration->isADLogin() )
                    {
                        collIter->second->m_AreaIDAD = m_pDataCurrent->m_AreaIDAD;

                        if ( collIter->second->m_AreaIDAD != m_pDataCurrent->m_AreaIDAD )
                        {
                            collIter->second->m_AreaName = m_pDataCurrent->m_AreaName;
                        }
                    }
                    else
                    {
                        collIter->second->m_AreaID = m_pDataCurrent->m_AreaID;

                        if ( collIter->second->m_AreaID != m_pDataCurrent->m_AreaID )
                        {
                            collIter->second->m_AreaName = m_pDataCurrent->m_AreaName;
                        }
                    }

                    collIter->second->m_LanguageID = m_pDataCurrent->m_LanguageID;

                    if ( collIter->second->m_LanguageID != m_pDataCurrent->m_LanguageID )
                    {
                        collIter->second->m_LanguageName = m_pDataCurrent->m_LanguageName;
                    }

                    m_pDataCurrent = collIter->second;
                }

                BLOG_DEBUG_STREAM( LoggerPool::loginAppl(), "login data for user <" << m_pDataCurrent->m_UserName << ">, application <" << m_pDataCurrent->m_ApplName << "> already exists." );

                return false; // no insert to collection, data already exist; don't read rights and parameters again
            }
            else
            {
                m_CollLoginData.insert( CollLoginData::value_type( key, m_pDataCurrent ) );
                return true; // read rights and parameters
            }
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::eraseLoginDataColl()
        {
            I18nString key;
            generateLoginDataCollKey( key );

            m_CollLoginData.erase( key );
        }

//----------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::generateLoginDataCollKey( I18nString& rKey )
        {
            //<applName>%<userName>
            rKey = m_pDataCurrent->m_ApplName;
            rKey.append( "%" );
            rKey.append( m_pDataCurrent->m_UserName );
        }

//----------------------------------------------------------------------------
        /*! \throw  InvalidParameterException */
        void LoginAppl::setI18n()
        {
            using cmnutil::Locale;

            I18nString  paraValue;
            I18nString  paraCond;
            paraCond.itos( getAreaID() );

            // --- query basar locale
            Locale::LanguageEnum eLang = Locale::LOCALE_DEFAULT;

            if ( getParaValue( paraCond, PARAM_BASAR_LOCALE, paraValue ) ) // parameter found
            {
                eLang = Locale::getLocaleByString( paraValue );

                if ( Locale::LOCALE_INVALID == ( eLang ) )
                {
                    ConstString func = "basar.login.LoginAppl.setI18n";
                    VarString reason;
                    reason.format( "parameter %s=\"%s\" not valid!", PARAM_BASAR_LOCALE, paraValue.c_str() );

                    throw InvalidParameterException( ExceptInfo( func, reason, __FILE__, __LINE__ ) );
                }
            }

            // --- query basar codepage
            Locale::CodepageEnum eCodepage = Locale::getPhoenixCodepage( eLang );

            if ( getParaValue( paraCond, PARAM_BASAR_CODEPAGE, paraValue ) ) // parameter found
            {
                eCodepage = Locale::getCodepageByString( paraValue );

                if ( Locale::CP_INVALID == eCodepage )
                {
                    ConstString func = "basar.login.LoginAppl.setI18n";
                    VarString reason;
                    reason.format( "parameter %s=\"%s\" not valid!", PARAM_BASAR_CODEPAGE, paraValue.c_str() );

                    throw InvalidParameterException( ExceptInfo( func, reason, __FILE__, __LINE__ ) );
                }
            }

            // --- configure basar settings
            /*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
            gui::widget::I18n::setI18n( eLang, eCodepage );
#else
            Locale::setLocale  ( eLang );
            Locale::setCodepage( eCodepage );
#endif
        }

//---------------------------------------------------------------------------
//! \throw  no-throw
//! \return basar::login::Connection::Connectionptr - shared pointer to connection object
        basar::login::Connection::Connectionptr LoginAppl::getConnection() const
        {
            if ( NULL == m_ConnectionPtr.get() )
            {
                m_ConnectionPtr = basar::login::Connection::Connectionptr( new Connection ( m_Configuration ) );
            }

            return m_ConnectionPtr;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw */
        void LoginAppl::savePrevData ( const VarString& languageID /* = "en"*/ )
        {
            m_pDataPrevious = m_pDataCurrent;
            m_pDataCurrent  = LoginData::create();

            if ( !languageID.empty() )
            {
                m_pDataCurrent->m_LanguageID = languageID;
            }
            else if ( m_pDataPrevious != 0 )
            {
                m_pDataCurrent->m_LanguageID = m_pDataPrevious->m_LanguageID;
                m_pDataCurrent->m_LanguageName = m_pDataPrevious->m_LanguageName;
            }
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if login was successful
            \return otherwise returns false */
        bool LoginAppl::nonInteractiveLogin ( I18nString& rApplName,
                                              const I18nString&     rUsername,
                                              const I18nString&     rCryptPW,
                                              Int32               areaID, // -> uadm2 db-property: areano
                                              ConstString         languageID )
        {
            ConstString func = "basar.login.LoginAppl.withoutDlg";

            try
            {
                init();

                basar::VarString langId( languageID );
                langId.upper();

                m_IsDialog = false;

                //----- check rApplName ----
                m_ApplDM->checkApplNameLength( rApplName );

                //----- save prev ----
                savePrevData( langId );

                //----- set data (without iterator, because no connection yet) ----
                m_pDataCurrent->m_ApplName = rApplName;
                m_pDataCurrent->m_UserName = rUsername;
                m_pDataCurrent->m_CryptPW  = rCryptPW;

                if ( false == m_Configuration->isADLogin() )
                {
                    m_pDataCurrent->m_AreaID   = static_cast<basar::Int16>( areaID );
                }
                else
                {
                    m_pDataCurrent->m_AreaIDAD   = areaID;
                }

                if ( insertLoginDataColl() == false && rCryptPW == m_pDataCurrent->m_CryptPW )
                {
                    // rights + parameters already read, password ok
                    m_LoginSuccessful = true;
                }
                else
                {
                    // new login
                    //----- connect ----
                    basar::login::Connection::Connectionptr connPtr = getConnection();
                    connPtr->connect();

                    //----- init Domain Modules ----
                    m_ApplDM->init( connPtr->getActiveConn() );
                    m_UserDM->init( connPtr->getActiveConn() );

                    //----- check ----
                    m_currentAppl = m_ApplDM->findApplIDByName( rApplName );

                    if ( m_currentAppl.isEnd() )
                    {
                        VarString reason;
                        reason.format( "application name <%s> does not exist!", rApplName.c_str() );
                        throw InvalidApplNameException( ExceptInfo( func, reason, __FILE__, __LINE__ ) );
                    }

                    //----- area ----
                    if ( m_Configuration->isADLogin() && USER_MGMT_APP_NAME == rApplName )
                    {
                        m_currentAreaSingle = m_ApplDM->findGlobalArea().begin();
                    }
                    else
                    {
                        m_currentAreaSingle = m_ApplDM->findAreaNameByID( areaID );
                    }

                    if ( m_currentAreaSingle.isEnd() )
                    {
                        VarString reason;
                        reason.format( "area <%d> does not exist!", areaID );
                        throw InvalidAreaException( ExceptInfo( func, reason, __FILE__, __LINE__ ) );
                    }

                    m_pDataCurrent->m_AreaName = m_currentAreaSingle.getString( "areaname" );

                    db::aspect::AccessorPropertyTable_YIterator yitLang;

                    if ( m_Configuration->isADLogin() )
                    {
                        m_currentAppl.setString( "languageid" , langId );

                        yitLang = m_ApplDM->findApplicationLanguage( m_currentAppl ).begin();

                        if ( yitLang.isEnd() )
                        {
                            std::ostringstream os;
                            os << "language <" << langId << "> does not exist or not mapped to application";

                            throw InvalidStateException( ExceptInfo( func, os.str(), __FILE__, __LINE__ ) );
                        }

                        m_currentLangArea = m_currentAreaSingle;
                    }

                    //---- user ----
                    m_currentUser   = m_UserDM->addEmptyUser();
                    m_currentUser.setString( "username", rUsername );

                    //?????
                    // Reading UADM parameters from DB
                    //m_currentPara = m_ApplDM->findParaByApplName( USER_MGMT_APP_NAME );
                    //m_pDataCurrent->insertParameters( m_currentPara );

                    m_CheckLoginUC->setPassword( rCryptPW );
                    m_CheckLoginUC->setLoginMode( false );
                    m_LoginSuccessful = m_CheckLoginUC->run();

                    if ( m_LoginSuccessful )
                    {
                        m_pDataCurrent->insertUserData      ( m_currentUser, m_Configuration->isADLogin() );
                        m_pDataCurrent->insertApplData      ( m_currentAppl );
                        storeRightsParameters();

                        if ( m_Configuration->isADLogin() )
                        {
                            // Saving User Defaults
                            m_currentLangArea = m_ApplDM->addEmptyPrevious();
                            m_currentLangArea.setInt32( "applicationid" , m_currentAppl.getInt32( "applicationid" ) );
                            m_currentLangArea.setInt32( "languagemapid" , yitLang.getInt32( "languagemapid" ) );
                            m_currentLangArea.setInt32( "areaid" , m_currentAreaSingle.getInt32( "areaid" ) );
                            m_currentLangArea.setInt32( "userid", m_currentUser.getInt32( "userid" ) );

                            // login time set.
                            m_currentLangArea.setString( "logintime", basar::DateTime().toString( "%Y-%m-%d %H:%M:%S" ) );
                            m_ApplDM->savePrevious( m_currentLangArea );
                        }

                        m_pDataCurrent->insertBranches( m_ApplDM->findAreaByPattern( m_currentAppl ), m_Configuration->isADLogin() );
                    }
                }

                if ( m_LoginSuccessful == false )
                {
                    eraseLoginDataColl();
                    m_pDataCurrent = m_pDataPrevious;
                }
                else
                {
                    setI18n();      // basar settings, possibly Qt settings

                    BLOG_INFO_STREAM( LoggerPool::loginAppl(), "user <" << m_pDataCurrent->m_UserName << "> logged in for application <" << m_pDataCurrent->m_ApplName << ">, area <" << areaID << "> non-interactively." );
                }
            }
            catch ( basar::Exception& e )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginAppl(), "got exception: " << e.what() );
                shutdown();
                throw;
            }
            catch ( ... )
            {
                BLOG_ERROR( LoggerPool::loginAppl(), "got exception" );
                shutdown();
                throw;
            }

            shutdown();

            return m_LoginSuccessful;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if login via dialog
            \return otherwise returns false */
        bool LoginAppl::isDialog () const
        {
            return m_IsDialog;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return applicationid */
        Int32   LoginAppl::getApplID        ()  const
        {
            return m_pDataCurrent->m_ApplID;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return applicationname */
        const I18nString& LoginAppl::getApplName       ()  const
        {
            return m_pDataCurrent->m_ApplName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return languageid */
        const I18nString LoginAppl::getLanguageID   ()  const
        {
            I18nString langUpper = m_pDataCurrent->m_LanguageID;
            langUpper.upper();
            return langUpper;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return languagename */
        const I18nString&   LoginAppl::getLanguageName  ()  const
        {
            return m_pDataCurrent->m_LanguageName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaid */
        Int16   LoginAppl::getAreaID        ()  const
        {
            return m_pDataCurrent->m_AreaID;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaid */
        Int32   LoginAppl::getAreaIDAD      ()  const
        {
            return m_pDataCurrent->m_AreaIDAD;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return areaname */
        const I18nString&   LoginAppl::getAreaName      ()  const
        {
            return m_pDataCurrent->m_AreaName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return userid */
        Int32   LoginAppl::getUserID        ()  const
        {
            return m_pDataCurrent->m_UserID;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return username */
        const I18nString&   LoginAppl::getUserName      ()  const
        {
            return m_pDataCurrent->m_UserName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return username */
        basar::login::UserTypeEnum  LoginAppl::getUserType      ()  const
        {
            return m_pDataCurrent->m_UserType;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return description */
        const I18nString&   LoginAppl::getDescription   ()  const
        {
            return m_pDataCurrent->m_Description;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return surname */
        const I18nString&   LoginAppl::getSurName       ()  const
        {
            return m_pDataCurrent->m_SurName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return forename */
        const I18nString&   LoginAppl::getForeName      ()  const
        {
            return m_pDataCurrent->m_ForeName;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return mail */
        const I18nString&   LoginAppl::getMail          ()  const
        {
            return m_pDataCurrent->m_Mail;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return email */
        const I18nString&   LoginAppl::getEmail     ()  const
        {
            return m_pDataCurrent->m_Email;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return ntuser */
        const I18nString&   LoginAppl::getNtUser        ()  const
        {
            return m_pDataCurrent->m_NtUser;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return crypted password */
        const I18nString&   LoginAppl::getCryptPW       ()  const
        {
            return m_pDataCurrent->m_CryptPW;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if right exists
            \return otherwise returns false */
        bool LoginAppl::isLegitimated   ( Int32              moduleID,
                                          const I18nString& rRight,
                                          Int32              areaID
                                        ) const
        {
            return m_pDataCurrent->findRight( moduleID, rRight, areaID );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if right exists
            \return otherwise returns false */
        bool LoginAppl::isLegitimated   ( const Int32        moduleID,
                                          const I18nString& rRight
                                        )
        {
            //if ( m_Configuration->isADLogin() )
            //{
            //    return isLegitimated( moduleID, rRight, m_pDataCurrent->m_AreaIDAD, false );
            //}
            //else
            //{
            return isLegitimated( moduleID, rRight, m_pDataCurrent->m_AreaID );
            //}
        }


//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginAppl::getParaValue    ( const I18nString& rPara1,
                                          const I18nString& rPara2,
                                          const I18nString& rParaName,
                                          I18nString&        rParaValue
                                        ) const
        {
            return m_pDataCurrent->findParameter( rPara1, rPara2, rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginAppl::getParaValue    ( const I18nString& rPara1,
                                          const I18nString& rParaName,
                                          I18nString&        rParaValue
                                        ) const
        {
            I18nString noVal = "0";
            return getParaValue( rPara1, noVal, rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \retval true if parameter exists
            \return otherwise returns false */
        bool LoginAppl::getParaValue    ( const I18nString& rParaName,
                                          I18nString&        rParaValue
                                        ) const
        {
            I18nString noVal = "0";
            return getParaValue( noVal, noVal, rParaName, rParaValue );
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with branch numbers */
        const CollBranchesWithRight LoginAppl::getBranchesWithRight( const Int16 moduleID, const I18nString& rRight ) const
        {
            CollBranchesWithRight   collBranchesWithRight;

            const CollBranches& collAreas = m_pDataCurrent->getAllAreas();

            CollBranches::const_iterator itAreas = collAreas.begin();
            CollBranches::const_iterator itAreasEnd = collAreas.end();

            while ( itAreas != itAreasEnd )
            {
                if ( isLegitimated( moduleID, rRight, itAreas->first ) )
                {
                    collBranchesWithRight.insert( CollBranchesWithRight::value_type( itAreas->first, itAreas->second ) );
                }

                ++itAreas;
            }

            return collBranchesWithRight;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with branch names */
        const CollBranchesWithRightOrderedByName
        LoginAppl::getBranchesWithRightOrderedByName( const Int16 moduleID, const I18nString& rRight ) const
        {
            CollBranchesWithRightOrderedByName  collBranchesWithRight;

            const CollBranches& collAreas = m_pDataCurrent->getAllAreas();

            CollBranches::const_iterator itAreas = collAreas.begin();
            CollBranches::const_iterator itAreasEnd = collAreas.end();

            while ( itAreas != itAreasEnd )
            {
                if ( isLegitimated( moduleID, rRight, itAreas->first ) )
                {
                    collBranchesWithRight.insert( CollBranchesWithRightOrderedByName::value_type( itAreas->second, itAreas->first ) );
                }

                ++itAreas;
            }

            return collBranchesWithRight;
        }

//---------------------------------------------------------------------------
        /*! \throw  no-throw
            \return collection with branch numbers */
        const CollRightsForModAndArea LoginAppl::getRightsByModuleAndArea( Int32 moduleID, Int32 areaID ) const
        {
            CollRightsForModAndArea collRightsForModAndArea;

            const CollRights& collRights = m_pDataCurrent->getAllRights();

            CollRights::const_iterator itRights = collRights.begin();
            CollRights::const_iterator itEndRights = collRights.end();

            const I18nString formatted = m_pDataCurrent->formatRightEntry( moduleID, "", areaID );

            while ( itRights != itEndRights )
            {
                I18nString temp = *itRights;

                if ( temp.length() > formatted.length() )
                {
                    temp = temp.substr( 0, formatted.length() );

                    if ( std::string::npos != temp.find( formatted ) )
                    {
                        collRightsForModAndArea.push_back( ( *itRights ).right( temp ) );
                    }
                }

                ++itRights;
            }

            return collRightsForModAndArea;
        }

//---------------------------------------------------------------------------

        bool LoginAppl::hasUserAnyRights() const
        {
            return m_pDataCurrent->hasUserAnyRights();
        }

//---------------------------------------------------------------------------

        bool LoginAppl::hasUserRightsForCurrentArea() const
        {
            return m_pDataCurrent->hasUserRightsForCurrentArea();
        }

//---------------------------------------------------------------------------
//! \throw      no-throw
        void LoginAppl::setConnectionConfig ( const VarString& strHostMain,
                                              const VarString& strHostBackup,
                                              const VarString& strDbMain,
                                              const VarString& strDbBackup  )
        {
            basar::login::Connection::Connectionptr connPtr = getConnection();
            connPtr->setHostname( strHostMain );
            connPtr->setHostnameBackup( strHostBackup );
            connPtr->setDatabase( strDbMain );
            connPtr->setDatabaseBackup( strDbBackup );
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     main host name */
        const VarString LoginAppl::getHostMain()        const
        {
            return getConnection()->getHostname();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     backup host name */
        const VarString LoginAppl::getHostBackup()      const
        {
            return getConnection()->getHostnameBackup();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     main database name */
        const VarString LoginAppl::getDbMain()          const
        {
            return getConnection()->getDatabase();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
            \return     backup database name */
        const VarString LoginAppl::getDbBackup()        const
        {
            return getConnection()->getDatabaseBackup();
        }

//---------------------------------------------------------------------------
        /*! \throw      no-throw
        */
        void LoginAppl::setLoginMode( LoginModeEnum loginMode )
        {
            m_RequestedLoginMode = loginMode;
        }

//---------------------------------------------------------------------------
//! \throw      no-throw
//! \return LoginConfigurationPtr - shared ptr to configuration data
        Configurationptr LoginAppl::getConfiguration() const
        {
            return m_Configuration;
        }

//---------------------------------------------------------------------------
} // login
} // basar

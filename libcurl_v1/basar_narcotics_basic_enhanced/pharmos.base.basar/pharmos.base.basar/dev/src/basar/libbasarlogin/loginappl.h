//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface login
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LOGINAPPL_H
#define GUARD_LOGINAPPL_H

#include <map>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "libbasarlogin_definitions.h"

#include "checkloginbaseuc.h"
#include "checkloginuc.h"
#include "checkadloginuc.h"

#include "connection.h"
#include "configuration.h"
#include "configurationptr.h"

#include "baseapplmgmtdm.h"
#include "adloginapplmgmtdm.h"
#include "applmanagementdm.h"

#include "baseusermgmtdm.h"
#include "adloginusermgmtdm.h"
#include "usermanagementdm.h"

#include "usermgmtdmfactory.h"
#include "applmgmtdmfactory.h"

#include "checkloginucfactory.h"

#include "loginusertype.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
        namespace useCase
        {
            class ChangePw;
            class CheckLoginBase;
            class CheckLogin;
            class CheckADLogin;
        }

        class LoginData;

// --- typedef section ---------------------------------------------------

//! \brief definition for login data collection
//! \n     key  : string in format applname%username
//! \n     value: weak pointer to implementation object
        typedef std::map < I18nString, boost::shared_ptr < LoginData > >        CollLoginData;

//----------------------------------------------------------------------------
/*! \brief  implementation of login.

            This is a base class.
            throws NoTextFoundException */
        class LoginAppl
        {
                friend class useCase::ChangePw;
                friend class useCase::CheckLoginBase;
                friend class useCase::CheckLogin;
                friend class useCase::CheckADLogin;

            public:
                typedef boost::shared_ptr< LoginAppl >    LoginApplSharedPtr;   //!< shared pointer to LoginAppl

                /*! \brief return shared pointer to this by casting weak -> shared
                    \n no-throw */
                LoginApplSharedPtr getThis( );

                /*! \brief create a LoginAppl.
                    \n     no-throw */
                static LoginApplSharedPtr create();

                /*! \brief destructor
                    \n     no-throw */
                virtual ~LoginAppl();

                /*! \brief  start login by calling login-dialog, which is created modal.
                    \return true if login via login-dialog was successful, false otherwise
                    \n      no-throw */
                virtual bool interactiveLogin( I18nString& rApplName,       //!< unique applicationname to start login for
                                               const I18nString& rWindowTitle,    //!< window title to show in login dialog
                                               const Int16        maxTrials       //!< number of login trials
                                             ) = 0;

                /*! \brief  start login without calling login-dialog
                    \n      no-throw */
                bool nonInteractiveLogin( I18nString&    rApplName, //!< unique applicationname to start login for
                                          const I18nString&   rUsername,  //!< name of user to login
                                          const I18nString&   rCryptPW,   //!< password according to username
                                          Int32               areaID,     //!< areaid for login
                                          ConstString         languageID  //!< language can be set because no selection without login dialog
                                        );

                /*! \brief  return applicationid of current login
                    \n      no-throw */
                Int32               getApplID       ()  const;

                /*! \brief  return applicationname of current login
                    \n      no-throw */
                const I18nString&   getApplName     ()  const;

                /*! \brief  return languageid of current login
                    \n      no-throw */
                const I18nString    getLanguageID   ()  const; // default: Languageid in UPPER-Case

                /*! \brief  return languagename of current login
                    \n      no-throw */
                const I18nString&   getLanguageName ()  const;

                /*! \brief  return areaid of current login
                    \n      no-throw */
                Int16               getAreaID       ()  const;

                /*! \brief  return areaid of current login
                    \n      no-throw */
                Int32               getAreaIDAD     ()  const;

                /*! \brief  return areaname of current login
                    \n      no-throw */
                const I18nString&   getAreaName     ()  const;

                /*! \brief  return userid of current login
                    \n      no-throw */
                Int32               getUserID       ()  const;

                /*! \brief  return username of current login
                    \n      no-throw */
                const I18nString&   getUserName     ()  const;

                /*! \brief  return description of current login
                    \n      no-throw */
                const I18nString&   getDescription  ()  const;

                /*! \brief  return surname of current login
                    \n      no-throw */
                const I18nString&   getSurName      ()  const;

                /*! \brief  return forename of current login
                    \n      no-throw */
                const I18nString&   getForeName     ()  const;

                /*! \brief  return mail of current login
                    \n      no-throw */
                const I18nString&   getMail         ()  const;

                /*! \brief  return email of current login
                    \n      no-throw */
                const I18nString&   getEmail        ()  const;

                /*! \brief  return ntuser of current login
                    \n      no-throw */
                const I18nString&   getNtUser       ()  const;

                /*! \brief  return crypted of current login
                    \n      no-throw */
                const I18nString&   getCryptPW      ()  const;



                /*! \brief  return usertype of current login
                    \n      no-throw */
                basar::login::UserTypeEnum      getUserType     ()  const;

                /*! \brief  check if right exists for current login
                    \n      no-throw */
                bool isLegitimated  ( Int32              moduleID,  //!< module in which right is asked for
                                      const I18nString& rRight,     //!< rightname to ask for
                                      Int32              areaID     //!< areaid in which right is asked for
                                    ) const;

                /*! \brief  check if right exists for current login
                    \n      no-throw */
                bool isLegitimated  ( const Int32        moduleID,  //!< module in which right is asked for
                                      const I18nString& rRight       //!< rightname to ask for
                                    );


                /*! \brief  ask for parameter of current login
                    \n      no-throw */
                bool getParaValue   ( const I18nString& rPara1,     //!< 1st condition of parameter
                                      const I18nString& rPara2,      //!< 2nd condition of parameter
                                      const I18nString& rParaName,   //!< name of parameter to search for
                                      I18nString&        rParaValue  //!< value of parameter = result
                                    ) const;

                /*! \brief  ask for parameter of current login
                    \n      no-throw */
                bool getParaValue   ( const I18nString& rPara1,     //!< 1st condition of parameter
                                      const I18nString& rParaName,   //!< name of parameter to search for
                                      I18nString&        rParaValue  //!< value of parameter = result
                                    ) const;

                /*! \brief  ask for parameter of current login
                    \n      no-throw */
                bool getParaValue   ( const I18nString& rParaName,  //!< name of parameter to search for
                                      I18nString&        rParaValue  //!< value of parameter = result
                                    ) const;

                /*! \brief  ask for collection containing all branches (branchno) in which the given user right is granted.
                    \n      no-throw */
                const CollBranchesWithRight getBranchesWithRight( const Int16        moduleID,  //!< module in which right is asked for
                        const I18nString& rRight       //!< rightname to ask for
                                                                ) const;

                /*! \brief  ask for collection containing all branches (ordered by name) in which the given user right is granted.
                    \n      no-throw */
                const CollBranchesWithRightOrderedByName
                getBranchesWithRightOrderedByName( const Int16        moduleID, //!< module in which right is asked for
                                                   const I18nString& rRight      //!< rightname to ask for
                                                 ) const;


                /*! \brief  ask for collection containing all rightnames of given module and area in which are granted to the current user.
                    \n      no-throw */
                const CollRightsForModAndArea getRightsByModuleAndArea( Int32       moduleID,           //!< module in which right is asked for
                        Int32       areaID      //!< area in which right is asked for
                                                                      ) const;
                //! \brief ask whether or not user has any rights for current application
                //! \return true if user has any rights for current application, false otherwise
                bool    hasUserAnyRights() const;

                bool hasUserRightsForCurrentArea() const;

                /*! \brief  check if there is an entry in pploginprevious
                    \n      no-throw */
                bool    isFirstLogin() const;

                /*! \brief  check if login happened via dialog or without
                    \n      no-throw */
                bool    isDialog () const;

                /*! \brief  handle gui-event ok button pressed in login dlg
                    \n      no-throw */
                virtual void    handleLogin ( bool loginWithADCredentials = false ) = 0;

                /*! \brief  handle gui-event ok button pressed in change pw dlg
                    \n      no-throw */
                virtual void    handlePwChanged() = 0;

                /*! \brief  handle gui-event cancel button pressed
                    \n      no-throw */
                virtual void    handleCancel() = 0;

                /*! \brief  call change password dialog and manage input check
                    \n      no-throw */
                virtual bool    managePwChanging() = 0;

                /*! \brief  return multilang-text from pptext to reftext
                    \return multilang text for labels / messages
                    \n      NoTextFoundException */
                virtual const I18nString    getLabel( I18nString refText //!< reference text to search label / message in
                                                    ) = 0;

                /*! \brief set connection configuration for login, if not default values should be used.
                    \n     no-throw */
                void setConnectionConfig    ( const VarString& strHostMain, //!< main host name
                                              const VarString& strHostBackup,    //!< backup host name
                                              const VarString& strDbMain,        //!< main database name
                                              const VarString& strDbBackup       //!< backup database name
                                            );

                /*! \brief get main host name, to which login is connected.
                    \n     no-throw */
                const VarString getHostMain()       const;

                /*! \brief get backup host name, to which login is connected.
                    \n     no-throw */
                const VarString getHostBackup()     const;

                /*! \brief get main database name, to which login is connected.
                    \n     no-throw */
                const VarString getDbMain()         const;

                /*! \brief get backup database name, to which login is connected.
                    \n     no-throw */
                const VarString getDbBackup()       const;

                /*! \brief set login mode
                    \n     no-throw */
                void setLoginMode( LoginModeEnum loginMode //!< requested login mode
                                 );

                Configurationptr getConfiguration() const;


            protected:
                LoginAppl               ();                         //!< forbidden constructor
                LoginAppl               ( const LoginAppl& rCopy ); //!< forbidden copy constructor
                LoginAppl& operator =  ( const LoginAppl& rCopy );  //!< forbidden assignment operator


                bool            m_PreviousLoginExists;  //!< has there been a previous login ?
                bool            m_LoginSuccessful;      //!< was login sucessful ?
                bool            m_IsDialog;             //!< is login via dialog ?

                LoginModeEnum   m_RequestedLoginMode;   //!< storage for reguested login mode, defaults to LOGIN_MODE_DEFAULT (see libbasarlogin_definitions.h)

                /*! \brief  initialize members
                    \n      no-throw */
                virtual void init();

                /*! \brief  clear members
                    \n      no-throw */
                virtual void clear();

                /*! \brief  shutdown domain modules
                    \n      no-throw */
                virtual void shutdown();

                /*! \brief  save login data for previous entry
                    \n      no-throw */
                void    savePrevData        ( const VarString& languageID = "en"//!< language to set (e.g. "en" / "de")
                                            );

                /*! \brief  save rights + parameters in logindata-obj
                    \n      InvalidParameterException */
                void    storeRightsParameters();

                /*! \brief  add login data-obj to collection of successful logins
                    \n      no-throw */
                bool    insertLoginDataColl ();

                /*! \brief  erase login data-obj from collection of successful logins
                    \n      no-throw */
                void    eraseLoginDataColl  ();

                /*! \brief  generate key for logindata-collection
                    \n      no-throw */
                void    generateLoginDataCollKey( I18nString& rKey //!< key for logindata-collection = result
                                                );

                /*! \brief  set i18n settings for Qt codec, Qt locale, basar locale depending on retrieved login parameters
                    \n      throw InvalidParameterException */
                void    setI18n             ();


                basar::login::Connection::Connectionptr getConnection() const;

//-----------------------------
// YIterators / PropertyTables
//-----------------------------
                db::aspect::AccessorPropertyTable_YIterator m_currentAreaSingle;//!< iterator to single area
                db::aspect::AccessorPropertyTable_YIterator m_currentUser;      //!< iterator to current user data
                db::aspect::AccessorPropertyTable_YIterator m_currentAppl;      //!< iterator to selected AppID for ApplName
                db::aspect::AccessorPropertyTableRef        m_currentRights;    //!< list of rights a user has in an application (userrights + grouprights)
                db::aspect::AccessorPropertyTableRef        m_currentPara;      //!< list of parameters of an application
                db::aspect::AccessorPropertyTable_YIterator m_currentLangArea;  //!< yit for language & area.

//----------------------
// LoginData
//----------------------
                CollLoginData                           m_CollLoginData;    //!< collection with login data objects

                boost::shared_ptr<LoginData>            m_pDataCurrent;     //!< pointer to current data object
                boost::shared_ptr<LoginData>            m_pDataPrevious;    //!< pointer to previous data object

//----------------------
// DomainModules
//----------------------
                boost::shared_ptr<domMod::BaseApplManagement> m_ApplDM;
                boost::shared_ptr<domMod::BaseUserManagement> m_UserDM;

//----------------------
// UseCases
//----------------------
                boost::shared_ptr<useCase::CheckLoginBase>          m_CheckLoginUC;             //!< use case class for checking user login

//----------------------
//Factories
//----------------------
                CheckLoginUCFactory::CheckLoginUCFactoryPtr         m_CheckLoginUCFactoryPtr;   //!< shared pointer for CheckLoginUCFactory

                UserMgmtDMFactory::UserMgmtDMFactoryPtr             m_UserMgmtDMFactoryPtr;     //!< shared pointer for UserMgmtDMFactory

                ApplMgmtDMFactory::ApplMgmtDMFactoryPtr             m_ApplMgmtDMFactoryPtr;     //!< shared pointer for ApplMgmtDMFactory




            private:
//----------------------
// Utilities
//----------------------

                mutable basar::login::Connection::Connectionptr             m_ConnectionPtr;            //!< class for connecting to host / db
                Configurationptr                                            m_Configuration; //!< shared ptr to class holding configuration
        };

//----------------------------------------------------------------------------
} // login
} // basar

#endif

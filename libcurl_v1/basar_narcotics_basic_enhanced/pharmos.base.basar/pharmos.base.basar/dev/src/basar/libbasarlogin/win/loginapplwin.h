//----------------------------------------------------------------------------
/*! \file
 *  \brief  login implementation class for windows (with dialogs)
 *  \author Thomas Hörath
 *  \date   26.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGINAPPLWIN_H
#define GUARD_LOGINAPPLWIN_H

#include "loginappl.h"
#include "changepwuc.h"
#include "textmanagementdm.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace login	{

        namespace viewConn
        {
            class LoginDlg;
            class ChangePwDlg;
        }

//----------------------------------------------------------------------------
//! login implementation class \n final class
        class LoginApplWin : public LoginAppl
        {
                friend class LoginAppl;    //!< for construction from LoginAppl::create()

            public:
                /*! \brief destructor
                    \n     no-throw */
                virtual ~LoginApplWin();

                /*! \brief  start login by calling login-dialog, which is created modal.
                    \n      no-throw */
                virtual bool interactiveLogin( basar::I18nString& rApplName, //!< unique applicationname to start login for
                                               const basar::I18nString& rWindowTitle, //!< window title to show in login dialog
                                               const basar::Int16 maxTrials           //!< number of login trials
                                             );

                /*! \brief  handle gui-event ok button pressed in login dlg
                    \n      no-throw */
                virtual void    handleLogin( bool singleSignOnRequested = false );

                /*! \brief  handle gui-event ok button pressed in change pw dlg
                    \n      no-throw */
                virtual void    handlePwChanged();

                /*! \brief  handle gui-event cancel button pressed
                    \n      no-throw */
                virtual void    handleCancel();

                /*! \brief  call change password dialog and manage input check
                    \n      no-throw */
                virtual bool    managePwChanging();

                /*! \brief  return multilang-text from pptext to reftext
                    \n      NoTextFoundException */
                virtual const basar::I18nString getLabel( basar::I18nString refText //!< reference text to search in label / message
                                                        );


            private:
                LoginApplWin                ();                             //!< forbidden constructor
                LoginApplWin                ( const LoginApplWin& rCopy );  //!< forbidden copy constructor
                LoginApplWin& operator =   ( const LoginApplWin& rCopy );   //!< forbidden assignment operator

                basar::Int32        m_LoginTrials;      //!< number of login trials still available
                basar::I18nString   m_BtOk;             //!< text on ok-buttons
                basar::I18nString   m_rWindowTitle;     //!< given title of login window(s)
                bool                m_UpdateForBackup;  //!< is an update on backup necessary ?
                bool                m_UserIdFound;      //!< was userid found for username ?

                /*! \brief  initialize members
                    \n      no-throw */
                virtual void    init        ();

                /*! \brief  de-initialize members
                    \n      no-throw */
                virtual void shutdown();

                /*! \brief  clear members
                    \n      no-throw */
                virtual void    clear       ();

                /*! \brief  manage error input in login dialog
                    \n      no-throw */
                void    manageLoginFailed   ();

                /*! \brief  manage cancellation of login
                    \n      no-throw */
                void    manageCancel        ();

                /*! \brief  manage behaviour of previous login data
                    \n      no-throw */
                void    managePrevious      ();

                /*! \brief  manage sucessful input in login dialog
                    \n      no-throw */
                void    manageLoginSucceeded();

                /*! \brief  update backup db
                    \n      no-throw */
                void    updateBackup        ();

                /*! \brief  save login data in logindata-obj
                    \n      no-throw */
                void    storeLoginData      ();

                /*! \brief  return formatted multilang-text (for '\n' in messageboxes)
                    \n      no-throw */
                const basar::I18nString getFormattedLabel( basar::I18nString text //!< text to format
                                                         );

                basar::VarString getUserName() const;

//-----------------------------
// YIterators / PropertyTables
//-----------------------------
                basar::db::aspect::AccessorPropertyTable_YIterator  m_currentText;      //!< iterator to current text data
                basar::db::aspect::AccessorPropertyTable_YIterator  m_currentPrevious;  //!< iterator to previous login data
                basar::db::aspect::AccessorPropertyTableRef         m_currentAreas;     //!< list of areas
                basar::db::aspect::AccessorPropertyTableRef         m_currentLanguages; //!< list of languages

//----------------------
// DomainModules
//----------------------
                domMod::TextManagement      m_TextDM;               //!< domain module class for text access

//----------------------
// UseCases
//----------------------
                useCase::ChangePw           m_ChangePwUC;           //!< use case class for handling changed password

//----------------------
// ViewConnectors
//----------------------
                boost::shared_ptr<viewConn::LoginDlg>       m_pLoginDlgVC;      //!< view connector class for login dialog
                boost::shared_ptr<viewConn::ChangePwDlg>    m_pChangePwDlgVC;   //!< view connector class for change password dialog

        };

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar

#endif	// GUARD

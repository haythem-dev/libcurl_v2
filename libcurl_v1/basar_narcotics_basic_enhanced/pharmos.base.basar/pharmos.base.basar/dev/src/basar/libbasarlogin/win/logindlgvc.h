//----------------------------------------------------------------------------
/*! \file
 *  \brief  view connector class to login dialog
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------
#pragma once
#ifndef libbasarlogin_win_logindlgvc_h
#define libbasarlogin_win_logindlgvc_h

#include "libbasar_definitions.h"
#include "libbasarguitie_matcher.h"

#include "logintype.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4127 4481 4512)
#endif // _MSC_VER

#include <QtWidgets/QDialog>
#include "ui_logindlg.h"

#ifdef _MSC_VER
#pragma warning (pop)
#endif // _MSC_VER

namespace basar {

namespace db
{
    namespace aspect
    {
        class AccessorPropertyTableRef;
    } // namespace aspect
} // namespace db

namespace login {
namespace viewConn {

//---------------------------------------------------------------------------
/*! \brief class to connect login dialog
    \n final class
    \n throws no exceptions */
class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    /*! \brief std-constructor
        \n no-throw */
    LoginDlg(QWidget* parent = 0);  //!< parent from which dialog is called

    /*! \brief std-destructor
        \n no-throw */
    ~LoginDlg();

    /*! \brief initialize dialog (frames, matchers, labels, ...)
        \n no-throw */
    void init(
          const basar::db::aspect::AccessorPropertyTableRef propTabArea     //!< property table containing areas
        , const basar::db::aspect::AccessorPropertyTableRef propTabLanguage //!< property table containing languages
        , const basar::I18nString& rWindowTitle                             //!< window title
        , loginType::LoginTypeEnum loginType                                //!< login type
    );

    /*! \brief match from accessors to gui
        \n no-throw */
    void matchFromLoginInfo(
          const basar::db::aspect::AccessorPropertyTable_YIterator userData //!< yiterator containing user data
        , const basar::db::aspect::AccessorPropertyTable_YIterator prevData //!< yiterator containing previous data
        , loginType::LoginTypeEnum loginType                                //!< login type
    );

    /*! \brief match from gui to accessors
        \n no-throw */
    void matchToLoginInfo(
          basar::db::aspect::AccessorPropertyTable_YIterator userData //!< yiterator containing user data
        , basar::db::aspect::AccessorPropertyTable_YIterator prevData //!< yiterator containing previous data
        , loginType::LoginTypeEnum loginType                          //!< login type
    );

    /*! \brief show dialog
        \n no-throw */
    basar::Int32 show();

    /*! \brief hide dialog
        \n no-throw */
    void hide();
    void clearPasswordField();

    /*! \brief close dialog
        \n no-throw */
    bool shutdown();

    basar::VarString getUserName() const;
    basar::VarString getPassword() const;

private:
    Ui::logindlgClass m_uiLoginDlg;                 //!< user interface login dialog

    basar::gui::tie::MatcherRef m_matComboArea;     //!< matcher of area-combobox
    basar::gui::tie::MatcherRef m_matComboLanguage; //!< matcher of language-combobox

    QString m_Username;

private:
    void closeEvent(QCloseEvent *event);

private slots:
    /*! \brief handle ok button pressed
        \n no-throw */
    void on_okBt_clicked();

    /*! \brief handle cancel button pressed
        \n no-throw */
    void on_cancelBt_clicked();

    void on_cboWindowsLogin_stateChanged( int state );

    void on_password_textEdited( const QString& );
};

} // namespace viewConn
} // namespace login
} // namespace basar

#endif // !libbasarlogin_win_logindlgvc_h

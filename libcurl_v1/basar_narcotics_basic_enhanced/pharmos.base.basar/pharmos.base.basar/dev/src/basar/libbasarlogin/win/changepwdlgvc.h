//----------------------------------------------------------------------------
/*! \file
 *  \brief  view connector class to change password dialog
 *  \author Thomas Hörath
 *  \date   21.12.2005
 */
//----------------------------------------------------------------------------
#pragma once
#ifndef libbasarlogin_win_changepwdlgvc_h
#define libbasarlogin_win_changepwdlgvc_h

#include "libbasarguitie_definitions.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127 4481 4512)
#endif // _MSC_VER

#include <QtWidgets/QDialog>
#include "ui_changepwdlg.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

namespace basar {

namespace db
{
    namespace aspect
    {
        class AccessorPropertyTable_YIterator;
    } // namespace aspect
} // namespace db

namespace login {
namespace viewConn {

//---------------------------------------------------------------------------
/*! \brief class to connect login dialog
    \n final class
    \n throws no exceptions */
class ChangePwDlg : public QDialog
{
    Q_OBJECT

public:
    /*! \brief std-constructor
        \n no-throw */
    ChangePwDlg(QDialog* parent = 0); //!< parent from which dialog is called

    /*! \brief std-destructor
        \n no-throw */
    ~ChangePwDlg();

    /*! \brief show dialog
        \n no-throw */
    basar::gui::tie::WidgetReturnEnum show();

    /*! \brief hide dialog
        \n no-throw */
    void hide();

    /*! \brief close dialog
        \n no-throw */
    bool shutdown();

    /*! \brief initialize dialog (frames, matchers, labels, ...)
        \n no-throw */
    void init();

    /*! \brief match from gui to accessors
        \n no-throw */
    void matchToLoginInfo(
          const basar::db::aspect::AccessorPropertyTable_YIterator userData //!< yiterator containing user data
        , basar::I18nString& rRepeatedPW                                    //!< string to take repeated password for comparing later
    );

private:
    Ui::ChangePwDlgClass m_uiChangePwDlg; //!< user interface change password dialog

private:
    void closeEvent(QCloseEvent *event);

private slots:
    /*! \brief handle ok button pressed
        \n no-throw */
    void on_okBt_clicked();

    /*! \brief handle esc button pressed
        \n no-throw */
    void on_cancelBt_clicked();
};

} // namespace viewConn
} // namespace login
} // namespace basar

#endif // !libbasarlogin_win_changepwdlgvc_h

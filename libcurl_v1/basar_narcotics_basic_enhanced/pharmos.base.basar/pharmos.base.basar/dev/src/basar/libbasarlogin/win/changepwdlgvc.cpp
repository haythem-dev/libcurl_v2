//----------------------------------------------------------------------------
/*! \file
 *  \brief  all functions implemented for change password dialog
 *  \author Thomas Hörath
 *  \date   121.12.2005
 */
//----------------------------------------------------------------------------

#include "changepwdlgvc.h"

#include "libbasarguitie_manager.h"
#include "libbasarguitie_functions.h"
#include "libbasarlogin_manager.h"
#include "loginappl.h"

namespace basar {
namespace login {
namespace viewConn {

//---------------------------------------------------------------------------
/*! \throw  no-throw */
ChangePwDlg::ChangePwDlg(QDialog* parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint )
{}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
ChangePwDlg::~ChangePwDlg()
{}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void ChangePwDlg::init()
{
    m_uiChangePwDlg.setupUi(this);
    setFixedSize(size());

    // window title
    setWindowTitle(basar::login::Manager::getInstance().getAppl()->getLabel("CHANGEPW").c_str());

    //labels + buttons
    m_uiChangePwDlg.ID_ST_NEW_PW        ->setText(basar::login::Manager::getInstance().getAppl()->getLabel("PW1").c_str());
    m_uiChangePwDlg.ID_ST_NEW_PW_REPEAT ->setText(basar::login::Manager::getInstance().getAppl()->getLabel("PW2").c_str());

    m_uiChangePwDlg.okBt                ->setText(basar::login::Manager::getInstance().getAppl()->getLabel("BTOK").c_str());

    m_uiChangePwDlg.cancelBt            ->setShortcut(Qt::Key_Escape);
    m_uiChangePwDlg.cancelBt            ->setVisible(false);
}

//---------------------------------------------------------------------------
/*! \throw  no-throw
    \return exec-result from Qt */
basar::gui::tie::WidgetReturnEnum ChangePwDlg::show()
{
    return basar::gui::tie::getWidgetReturnType(exec());
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void ChangePwDlg::hide()
{
    return QDialog::accept();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw
    \return close-result from Qt */
bool ChangePwDlg::shutdown()
{
    return close();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void ChangePwDlg::on_okBt_clicked()
{
    Manager::getInstance().getAppl()->handlePwChanged();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void ChangePwDlg::on_cancelBt_clicked()
{
    QDialog::reject();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void ChangePwDlg::matchToLoginInfo(
      const basar::db::aspect::AccessorPropertyTable_YIterator userData
    , basar::I18nString& rRepeatedPW
)
{
    basar::gui::tie::Manager::getInstance().MatchToIterator( this, userData );
    rRepeatedPW = m_uiChangePwDlg.pw_repeated->text().toLocal8Bit().constData();
}

//---------------------------------------------------------------------------
void ChangePwDlg::closeEvent( QCloseEvent* event )
{
    on_cancelBt_clicked();
    QDialog::closeEvent(event);
}

} // namespace viewConn
} // namespace login
} // namespace basar

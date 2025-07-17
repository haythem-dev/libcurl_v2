//----------------------------------------------------------------------------
/*! \file
*  \brief  all functions implemented for login dialog
*  \author Thomas Hörath
*  \date   15.12.2005
*/
//----------------------------------------------------------------------------

#include "logindlgvc.h"

#include "loginappl.h"
#include "logintype.h"

#include "libbasarguitie_manager.h"
#include "libbasarlogin_manager.h"

namespace basar {
namespace login {
namespace viewConn {

//---------------------------------------------------------------------------
/*! \throw  no-throw */
LoginDlg::LoginDlg( QWidget* parent )
    : QDialog( parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint )
{}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
LoginDlg::~LoginDlg()
{}

//---------------------------------------------------------------------------
void LoginDlg::closeEvent( QCloseEvent* event )
{
    on_cancelBt_clicked();
    QDialog::closeEvent(event);
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::on_okBt_clicked()
{
    this->blockSignals( true );

    bool signleSignOnRequested = false;

    if ( Qt::Checked == m_uiLoginDlg.cboWindowsLogin->checkState() )
    {
        signleSignOnRequested = true;
    }

    Manager::getInstance().getAppl()->handleLogin( signleSignOnRequested );

    this->blockSignals( false );
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::on_cancelBt_clicked()
{
    this->blockSignals( true );

    Manager::getInstance().getAppl()->handleCancel();

    this->blockSignals( false );
}

//---------------------------------------------------------------------------

void LoginDlg::on_password_textEdited(const QString &)
{
    this->blockSignals( true );

    if ( false == m_uiLoginDlg.okBt->isDefault() )
    {
        m_uiLoginDlg.okBt->setDefault(true);
    }

    this->blockSignals( false );
}

//---------------------------------------------------------------------------

void LoginDlg::on_cboWindowsLogin_stateChanged( int state )
{
    this->blockSignals( true );

    if (Qt::Checked == state)
    {
        m_uiLoginDlg.username->setText( m_Username );
        m_uiLoginDlg.username->setEnabled( false );
        m_uiLoginDlg.password->setEnabled( false );
    }
    else
    {
        m_uiLoginDlg.username->setEnabled( true );
        m_uiLoginDlg.password->setEnabled( true );

        QString tempUsername = m_uiLoginDlg.username->text();

        if ( tempUsername.contains('\\') )
        {
            //select username part of login
            m_uiLoginDlg.username->setSelection( tempUsername.indexOf('\\') + 1, tempUsername.length() );
        }

        m_uiLoginDlg.username->setFocus();
    }

    this->blockSignals( false );
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::init(
      const basar::db::aspect::AccessorPropertyTableRef propTabArea
    , const basar::db::aspect::AccessorPropertyTableRef propTabLanguage
    , const basar::I18nString& rWindowTitle
    , loginType::LoginTypeEnum loginType
)
{
    m_uiLoginDlg.setupUi( this );
    setFixedSize(size());

    // Creating matchers of comboboxes with AccessorPropertyTableRefs
    m_matComboLanguage = basar::gui::tie::Manager::getInstance().createMatcher( m_uiLoginDlg.frLanguage, propTabLanguage );
    m_matComboArea     = basar::gui::tie::Manager::getInstance().createMatcher( m_uiLoginDlg.frArea, propTabArea );

    // window title
    basar::I18nString title = basar::login::Manager::getInstance().getAppl()->getLabel( "LOGIN" );
    title.append( " - " );
    title.append( rWindowTitle );
    setWindowTitle( QString::fromLocal8Bit( title.c_str() ) );

    // focus
    if ( basar::login::Manager::getInstance().getAppl()->isFirstLogin() )
    {
        m_uiLoginDlg.username->setFocus();
    }

    if ( loginType::LOGINTYPE_AD == loginType )
    {
        m_uiLoginDlg.ID_ST_AREA     ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "Area" ).c_str() ) );
        m_uiLoginDlg.ID_ST_USERNAME ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "UserID" ).c_str() ) );

        m_uiLoginDlg.cboWindowsLogin->setChecked( true );
    }
    else
    {
        m_uiLoginDlg.ID_ST_AREA     ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "BRANCH" ).c_str() ) );
        m_uiLoginDlg.ID_ST_USERNAME ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "USERNAME" ).c_str() ) );

        m_uiLoginDlg.cboWindowsLogin->setVisible( false );
    }

    // labels + buttons
    m_uiLoginDlg.ID_ST_LANGUAGE ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "LANGUAGE" ).c_str() ) );
    m_uiLoginDlg.ID_ST_PASSWORD ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "PASSWORD" ).c_str() ) );

    m_uiLoginDlg.cancelBt       ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "CANCEL" ).c_str() ) );
    m_uiLoginDlg.okBt           ->setText( QString::fromLocal8Bit( basar::login::Manager::getInstance().getAppl()->getLabel( "OK" ).c_str() ) );
}

//---------------------------------------------------------------------------
/*! \throw  no-throw
    \return exec-result from Qt */
basar::Int32 LoginDlg::show()
{
    return exec();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::hide()
{
    return QDialog::hide();
}

//---------------------------------------------------------------------------
void LoginDlg::clearPasswordField()
{
    m_uiLoginDlg.password->clear();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw
    \return close-result from Qt */
bool LoginDlg::shutdown()
{
    m_matComboArea.reset();
    m_matComboLanguage.reset();

    return close();
}

//---------------------------------------------------------------------------
basar::VarString LoginDlg::getUserName() const
{
    return m_uiLoginDlg.username->text().toLocal8Bit().constData();
}

//---------------------------------------------------------------------------
basar::VarString LoginDlg::getPassword() const
{
    return m_uiLoginDlg.password->text().toLocal8Bit().constData();
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::matchFromLoginInfo(
      const basar::db::aspect::AccessorPropertyTable_YIterator userData
    , const basar::db::aspect::AccessorPropertyTable_YIterator prevData
    , loginType::LoginTypeEnum loginType
)
{
    m_matComboArea.RightToLeft();
    m_matComboLanguage.RightToLeft();

    // Username match manually
    m_uiLoginDlg.username->setText( QString::fromLocal8Bit( userData.getString( "username" ).c_str() ).toLower() );
    m_uiLoginDlg.password->clear(); // don't set password to gui !

    // mark default entries from previous login
    if ( basar::login::Manager::getInstance().getAppl()->isFirstLogin() == false )
    {
        // store username
        m_Username = m_uiLoginDlg.username->text();

        // set focus to password field
        m_uiLoginDlg.password->setFocus();

        // Area
        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matComboArea.getRight().begin();

        if ( loginType::LOGINTYPE_AD == loginType )
        {
            basar::Int32 areaID = prevData.getInt32( "areaid" ); //uadms internal id

            while ( yit.isEnd() != true && yit.getInt32( "areaid" ) != areaID )
            {
                ++yit;
            }
        }
        else
        {
            basar::Int16 areaID = prevData.getInt16( "branchregionid" );

            while ( yit.isEnd() != true && yit.getInt16( "areaid" ) != areaID )
            {
                ++yit;
            }
        }

        if ( yit.isEnd() )
        {
            yit = m_matComboArea.getRight().begin();
        }


        basar::Int32 index = m_uiLoginDlg.areaname->findText( QString::fromLocal8Bit( yit.getString( "areaname" ).c_str() ) );
        m_uiLoginDlg.areaname->setCurrentIndex( index );

        // Language
        yit = m_matComboLanguage.getRight().begin();
        basar::I18nString  langID = prevData.getString( "languageid" );

        while ( yit.isEnd() != true && yit.getString( "languageid" ) != langID )
        {
            ++yit;
        }

        if ( yit.isEnd() )
        {
            yit = m_matComboLanguage.getRight().begin();
        }

        index = m_uiLoginDlg.languagename->findText( QString::fromLocal8Bit( yit.getString( "languagename" ).c_str() ) );
        m_uiLoginDlg.languagename->setCurrentIndex( index );
    }
}

//---------------------------------------------------------------------------
/*! \throw  no-throw */
void LoginDlg::matchToLoginInfo(
      basar::db::aspect::AccessorPropertyTable_YIterator userData
    , basar::db::aspect::AccessorPropertyTable_YIterator prevData
    , loginType::LoginTypeEnum loginType
)
{
    // set username
    I18nString userName = m_uiLoginDlg.username->text().toLocal8Bit().constData();

    userData.setString( "username", userName );

    I18nString area = m_uiLoginDlg.areaname->currentText().toLocal8Bit().constData();
    I18nString language = m_uiLoginDlg.languagename->currentText().toLocal8Bit().constData();

    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matComboArea.getRight().begin();

    while ( yit.isEnd() != true && yit.getString( "areaname" ) != area )
    {
        ++yit;
    }

    if ( loginType::LOGINTYPE_AD == loginType )
    {
        prevData.setInt32 ( "areaid", yit.getInt32( "areaid" ) );
        prevData.setInt32 ( "areano", yit.getInt32( "areano" ) );
    }
    else
    {
        prevData.setInt16 ( "branchregionid", yit.getInt16( "areaid" ) );
    }

    prevData.setString( "areaname",       area );

    yit = m_matComboLanguage.getRight().begin();

    while ( yit.isEnd() != true && yit.getString( "languagename" ) != language )
    {
        ++yit;
    }

    prevData.setString( "languageid",    yit.getString( "languageid" ) );
    prevData.setString( "languagename",  language );

    if ( loginType::LOGINTYPE_AD == loginType )
    {
        prevData.setInt32 ( "languagemapid", yit.getInt32( "languagemapid" ) );
    }
}

} // namespace viewConn
} // namespace login
} // namespace basar

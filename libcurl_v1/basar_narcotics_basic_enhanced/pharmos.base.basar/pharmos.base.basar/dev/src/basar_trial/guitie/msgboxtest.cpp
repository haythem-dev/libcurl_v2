
#include "msgboxtest.h"

using namespace basar::gui::tie;

MsgBoxTest::MsgBoxTest(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);

	ui.cbType->addItem( "infoMsgBox" );
	ui.cbType->addItem( "warningMsgBox" );
	ui.cbType->addItem( "criticalMsgBox" );
	ui.cbType->addItem( "questionMsgBox" );
	ui.cbType->addItem( "infoMsgBox (convenience)" );
	ui.cbType->addItem( "warningMsgBox (convenience)" );
	ui.cbType->addItem( "criticalMsgBox (convenience)" );
	ui.cbType->addItem( "questionMsgBox (convenience)" );
	ui.cbType->setCurrentIndex( 0 );

	ui.leMessage->setText( "My message text" );
	ui.leTitle->setText( "My window title" );
}

MsgBoxTest::~MsgBoxTest()
{
}

void MsgBoxTest::on_btShowThis_clicked()
{
	showMessageBox( this );
}

void MsgBoxTest::on_btShowNull_clicked()
{
	showMessageBox( NULL );
}

void MsgBoxTest::on_cbType_currentIndexChanged( int index )
{
	ui.groupBoxButtons->setDisabled( index > 3 );
}

void MsgBoxTest::showMessageBox( QWidget * parent )
{
	MessageBoxButtonEnum ret = ButtonNoButton;
	MessageBoxButtons buttons = getButtons();
	switch( ui.cbType->currentIndex() )
	{
		case 0:
		{
			ret = infoMsgBox( parent, ui.leMessage->text().toAscii().constData(), buttons, ButtonNoButton, ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 1:
		{
			ret = warningMsgBox( parent, ui.leMessage->text().toAscii().constData(), buttons, ButtonNoButton, ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 2:
		{
			ret = criticalMsgBox( parent, ui.leMessage->text().toAscii().constData(), buttons, ButtonNoButton, ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 3:
		{
			ret = questionMsgBox( parent, ui.leMessage->text().toAscii().constData(), buttons, ButtonNoButton, ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 4:
		{
			ret = infoMsgBox( parent, ui.leMessage->text().toAscii().constData(), ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 5:
		{
			ret = warningMsgBox( parent, ui.leMessage->text().toAscii().constData(), ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 6:
		{
			ret = criticalMsgBox( parent, ui.leMessage->text().toAscii().constData(), ui.leTitle->text().toAscii().constData() );
			break;
		}
		case 7:
		{
			ret = questionMsgBox( parent, ui.leMessage->text().toAscii().constData(), ui.leTitle->text().toAscii().constData() );
			break;
		}
		default:
		{
			criticalMsgBox(	this, "Invalid type!" );
			break;
		}
	}
	ui.leResult->setText( getResultText( ret ) );
}

const MessageBoxButtons MsgBoxTest::getButtons() const
{
	MessageBoxButtons ret = ButtonNoButton;

	ret |= ui.cbButtonOk->isChecked() ? ButtonOk : 0;
	ret |= ui.cbButtonOpen->isChecked() ? ButtonOpen : 0;
	ret |= ui.cbButtonSave->isChecked() ? ButtonSave : 0;
	ret |= ui.cbButtonCancel->isChecked() ? ButtonCancel : 0;
	ret |= ui.cbButtonClose->isChecked() ? ButtonClose : 0;
	ret |= ui.cbButtonDiscard->isChecked() ? ButtonDiscard : 0;
	ret |= ui.cbButtonApply->isChecked() ? ButtonApply : 0;
	ret |= ui.cbButtonReset->isChecked() ? ButtonReset : 0;
	ret |= ui.cbButtonRestoreDefaults->isChecked() ? ButtonRestoreDefaults : 0;
	ret |= ui.cbButtonHelp->isChecked() ? ButtonHelp : 0;
	ret |= ui.cbButtonSaveAll->isChecked() ? ButtonSaveAll : 0;
	ret |= ui.cbButtonYes->isChecked() ? ButtonYes : 0;
	ret |= ui.cbButtonYesToAll->isChecked() ? ButtonYesToAll : 0;
	ret |= ui.cbButtonNo->isChecked() ? ButtonNo : 0;
	ret |= ui.cbButtonNoToAll->isChecked() ? ButtonNoToAll : 0;
	ret |= ui.cbButtonAbort->isChecked() ? ButtonAbort : 0;
	ret |= ui.cbButtonRetry->isChecked() ? ButtonRetry : 0;
	ret |= ui.cbButtonIgnore->isChecked() ? ButtonIgnore : 0;

	return ret;
}

const QString MsgBoxTest::getResultText( MessageBoxButtonEnum button ) const
{
	switch( button )
	{
		case ButtonNoButton:		return "ButtonNoButton"; break;
		case ButtonOk:				return "ButtonOk"; break;
		case ButtonOpen:			return "ButtonOpen"; break;
		case ButtonSave:			return "ButtonSave"; break;
		case ButtonCancel:			return "ButtonCancel"; break;
		case ButtonClose:			return "ButtonClose"; break;
		case ButtonDiscard:			return "ButtonDiscard"; break;
		case ButtonApply:			return "ButtonApply"; break;
		case ButtonReset:			return "ButtonReset"; break;
		case ButtonRestoreDefaults: return "ButtonRestoreDefaults"; break;
		case ButtonHelp:			return "ButtonHelp"; break;
		case ButtonSaveAll:			return "ButtonSaveAll"; break;
		case ButtonYes:				return "ButtonYes"; break;
		case ButtonYesToAll:		return "ButtonYesToAll"; break;
		case ButtonNo:				return "ButtonNo"; break;
		case ButtonNoToAll:			return "ButtonNoToAll"; break;
		case ButtonAbort:			return "ButtonAbort"; break;
		case ButtonRetry:			return "ButtonRetry"; break;
		case ButtonIgnore:			return "ButtonIgnore"; break;
		default:					return "invalid!"; break;
	}
}


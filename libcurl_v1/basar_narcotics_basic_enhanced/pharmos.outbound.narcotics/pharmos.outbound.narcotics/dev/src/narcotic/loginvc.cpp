//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher
 *  \date   01.09.2010
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "loginvc.h"
#include "loggerpool.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-----------------------------------------------------------------------------------------------//
// usings
//-----------------------------------------------------------------------------------------------//
using basar::VarString;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-----------------------------------------------------------------------------------------------//
// class definition
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	parent passed window pointer this dialog belongs to.
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////

LoginVC::LoginVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
LoginVC::~LoginVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void LoginVC::wireEvents()
{
	QObject::connect(	btnLogout,	SIGNAL( clicked()				),			
						this,		SLOT  (	onBtnLogout_clicked()	));	
	QObject::connect(	btnCancel,	SIGNAL( clicked()				),			
						this,		SLOT  (	onBtnCancel_clicked()	));	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void LoginVC::init()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	userid->setReadOnly( true );
	username->setReadOnly( true );
	btnLogout->setEnabled( true );
	btnCancel->setEnabled( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return int-value of the methode QDialog::exec()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum LoginVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void LoginVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return bool-value of the method QDialog::close()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool LoginVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return close();
}

//---------------------------------------------------------------------------------------------------------------------//
// match methods
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LoginVC::matchCurrentSession(basar::Int32 userID, basar::I18nString userName)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchCurrentSession of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::I18nString userIDValue;
	userIDValue.itos( userID );
	userid->setText( QString::fromLocal8Bit(userIDValue.c_str()) );
	username->setText( QString::fromLocal8Bit(userName.c_str()) );
}

//---------------------------------------------------------------------------------------------------------------------//
// private slots
//---------------------------------------------------------------------------------------------------------------------//
void LoginVC::onBtnCancel_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnexit_clicked of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

//---------------------------------------------------------------------------------------------------------------------//
void LoginVC::onBtnLogout_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnlogout_clicked of LoginVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// decision how to continue is delegated to user
	basar::gui::tie::MessageBoxButtonEnum BT_NO = 
	basar::gui::tie::questionMsgBox( this,
				QApplication::translate( "LoginVC", "How do you want to continue after logging out? Login again?" ).toLocal8Bit().data(),
				basar::gui::tie::ButtonYes | basar::gui::tie::ButtonCancel );
				
	if( BT_NO == basar::gui::tie::ButtonYes ) // Relogin
	{
		// after Relogin has been fired the application won't return into loginvc
		basar::appl::SystemEventManager::getInstance().fire( "Relogin" );
		// close loginVC
		setCursor( Qt::WaitCursor );
		close();
	}
	else if( BT_NO == basar::gui::tie::ButtonCancel ){} // Cancel
}

//---------------------------------------------------------------------------------------------------------------------//
void LoginVC::clearSessionDetails()
{
	userid->setText( "" );
	username->setText( "" );
}

} //namespace viewConn
} //namespace narcotics
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of main application (exe) for narcotics
 *  \author	Thomas Hörath
 *  \date   28.06.2006
 */
//------------------------------------------------------------------------------
// include section
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "mainwindow.h"
#include "narcoticsappl.h"
#include "definitions.h"
#include "version.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
bool basar_init()
{
	narcotics::LoggerPool::initLogging( basar::login::Manager::getInstance().getAreaID() ); //after login, so that branchno is known
	
	basar::I18nString branch, user;
	basar::I18nString hostVal;			
	basar::I18nString databaseVal;
	basar::I18nString dokuVal;

	branch.format( "%d", basar::login::Manager::getInstance().getAreaID() );
	user = basar::login::Manager::getInstance().getUserName();
	
	if( basar::login::Manager::getInstance().getParaValue( branch, user, UADM_HOST, hostVal ) == false )
	{
		// no user-specific host parameter found (for individual testing!)
		basar::login::Manager::getInstance().getParaValue( branch, UADM_HOST, hostVal );
	}
	
	if( basar::login::Manager::getInstance().getParaValue( branch, user, UADM_DB, databaseVal ) == false )
	{
		// no user-specific database parameter found (for individual testing!)
		basar::login::Manager::getInstance().getParaValue( branch, UADM_DB, databaseVal );
	}
	
	basar::login::Manager::getInstance().getParaValue( UADM_DOKU, dokuVal );
	
	basar::InitStruct initStruct;
	initStruct.database = databaseVal;
	initStruct.host		= hostVal;
	initStruct.dokulink = dokuVal;
	basar::gui::qt::Manager::getInstance().init( initStruct );
	return true;
}

//----------------------------------------------------------------------------
bool init()
{
	narcotics::LoggerPool::initLogging(99); //branchno still not known
	
	basar::I18nString appl = "NARCOTICS";
	basar::I18nString title = QApplication::translate( "NarcoticsAppl", "Narcotics" ).toLocal8Bit().data();

	bool loginSuccess = basar::login::Manager::getInstance().interactiveLogin( appl,title, 5 );

	if( loginSuccess )
	{
		basar_init();
		
		if( !narcotics::NarcoticsAppl::getInstance().init() )
		{
			return false;
		}

		return true;
	}
	else
	{
		basar::gui::tie::infoMsgBox( 0, QApplication::translate( "NarcoticsAppl", "Login to Narcotics failed!" ).toLocal8Bit().data(), QApplication::translate("NarcoticsAppl", "Narcotics").toLocal8Bit().constData() );
		return false;
	}
}

//----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	int ret = 0;	
	QApplication a( argc, argv );

	try
	{
		if( init() )
		{
			mainwindow w;
		
			a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
			do 
			{
				w.resetRelogin();
				w.setStatusline();
				w.resetContributionVoucherPrintProcess();
				w.show();
				ret = a.exec();
			} while( ( w.isToRelogin() ) && ( init() == true ) );			
		}
	}
	catch( const basar::Exception & except )
	{
		basar::VarString msg;
		msg.format( "\n"
			"=== basar exception caught ===========================================================\n"
			"\n"
			"%s\n", 
			except.what().c_str() );
		basar::gui::tie::warningMsgBox( 0, msg.c_str() );
	}
	catch( const std::exception & )
	{
		basar::gui::tie::warningMsgBox( 0, "=== std exception caught ====" );
	}
	catch(...)
	{
		basar::gui::tie::warningMsgBox( 0, "=== unknown exception caught ====" );
	}

	narcotics::NarcoticsAppl::clear();
	basar::login::Manager::clear();
	basar::db::aspect::Manager::clear();

	return ret;
}
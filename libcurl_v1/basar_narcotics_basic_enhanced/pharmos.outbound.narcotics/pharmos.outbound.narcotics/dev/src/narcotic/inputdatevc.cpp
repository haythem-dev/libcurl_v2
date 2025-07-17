//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Roland Kiefert
 *  \date   04.12.2006
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "inputdatevc.h"
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
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-----------------------------------------------------------------------------------------------//
// class definition
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
// c'tor
///////////////////////////////////////////////////////////////////////////////////////////////////
InputDateVc::InputDateVc( QWidget *parent )
: QDialog( parent )
{
	ui.setupUi(this);
	wireEvents();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
InputDateVc::~InputDateVc()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::wireEvents()
{
	QObject::connect( this,			SIGNAL( finished( int ) ),	this, SLOT( closeDialog()		 ));
	QObject::connect( ui.btnClose,	SIGNAL( clicked()		),	this, SLOT( onBtnClose_clicked() ));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::closeDialog()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "closeDialog of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::init()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initGuiCtrls();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::initGuiCtrls()
{
	ui.SearchDate->setDate( QDate::currentDate() );
	ui.SearchDate->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );	
	ui.SearchDate->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::setDate( basar::cmnutil::DateTime dt )
{
	QDate qdt;
	qdt.setDate(dt.getDate()/10000, 
			  (dt.getDate()%10000)/100, 
			   dt.getDate()%100);
	ui.SearchDate->setDate( qdt );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum InputDateVc::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool InputDateVc::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void InputDateVc::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSelection of InputDateVc.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	searchIt.setString( constants::FROMDATE, ui.SearchDate->date().toString("yyyyMMdd").toLocal8Bit().constData() );
	searchIt.setString( constants::TODATE,	 ui.SearchDate->date().toString("yyyyMMdd").toLocal8Bit().constData() );
}

} //namespace viewConn
} //namespace narcotics
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "differenceinstockvc.h"
#include "comparebookingsvc.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

using basar::VarString;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-----------------------------------------------------------------------------------------------//
DifferenceInStockVC::DifferenceInStockVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

//-----------------------------------------------------------------------------------------------//
DifferenceInStockVC::~DifferenceInStockVC()
{
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::wireEvents()
{
	QObject::connect(	this,						SIGNAL( finished( int )						), 
						this,						SLOT  (	onBtnClose_clicked()				));
	QObject::connect(	btnUpdateDiffQtyInStock,	SIGNAL( clicked()							), 
						this,						SLOT  (	onBtnUpdateDiffQtyInStock_clicked()	));
	QObject::connect(	btnShowCompareBookings,		SIGNAL( clicked()							), 
						this,						SLOT  (	onBtnShowCompareBookings_clicked()	));
	QObject::connect(	btnClose,					SIGNAL( clicked()							), 
						this,						SLOT  (	onBtnClose_clicked()				));
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::init(basar::db::aspect::AccessorPropertyTableRef diffQtyInStock )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initGuiCtrls();
	m_matDiffQtyInStock	= basar::gui::tie::Manager::getInstance().createMatcher( grpDifference, diffQtyInStock );
	std::list<VarString> attribute;
	attribute.push_back( constants::ARTICLECODE );
	attribute.push_back( constants::ARTICLENAME );
	attribute.push_back( "creationdate"		);
	attribute.push_back( "diffqtyinstock"	);
	m_matDiffQtyInStock.transformProperties( tableWidget->horizontalHeaderLabels(), attribute );
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::initGuiCtrls()
{
	tableWidget->clear();
	QStringList list;
	list << tr("ArticleNo.") << tr("ArticleName") << tr("Date") << tr("Quantity");
	tableWidget->setHorizontalHeaderLabels( list );
	tableWidget->hideVerticalHeader();
	for ( int i = 0; i < tableWidget->columnCount(); i++ )
	{
		tableWidget->setEditableColumn( i, false );
	}
}

//-----------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum DifferenceInStockVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

//-----------------------------------------------------------------------------------------------//
bool DifferenceInStockVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matDiffQtyInStock.reset();
	return close();
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::matchToData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToData of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	tableWidget->clearContext();
	m_matDiffQtyInStock.RightToLeft( 0, tableWidget->rowCount() );
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::onBtnShowCompareBookings_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnShowCompareBookings_clicked of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	//CompareBookings
	basar::appl::SystemEventManager::getInstance().fire( "ShowComparingBookings" ); 
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::onBtnUpdateDiffQtyInStock_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnUpdateDiffQtyInStock_clicked of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	tableWidget->clearContext();
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "UpdateDiffQtyInStock" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

//-----------------------------------------------------------------------------------------------//
void DifferenceInStockVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of DifferenceInStockVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

} //namespace viewConn
} //namespace narcotics
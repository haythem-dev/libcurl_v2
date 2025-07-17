//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "halfyearlyreportvc.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
HalfYearlyReportVC::HalfYearlyReportVC( QWidget *parent )
: QDialog(parent)
{
    setupUi(this);
	wireEvents();
}

//-------------------------------------------------------------------------------------------------//
HalfYearlyReportVC::~HalfYearlyReportVC()
{
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::wireEvents()
{
	QObject::connect(	btnPrint,		SIGNAL( clicked()				), 
						this,			SLOT  (	onBtnPrint_clicked()	));
	QObject::connect(	btnSearch,		SIGNAL( clicked()				), 
						this,			SLOT  (	onBtnSearch_clicked()	));
	QObject::connect(	btnClose,		SIGNAL( clicked()				), 
						this,			SLOT  (	onBtnClose_clicked()	));
	QObject::connect(	btnDelete,		SIGNAL( clicked()				), 
						this,			SLOT  (	onBtnDelete_clicked()	));
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::initGuiCtrls()
{
	tableWidget->clear();
	basar::cmnutil::DateTime dateFrom;
	basar::cmnutil::DateTime dateTo;
	basar::Int16 year = static_cast<basar::Int16>(dateFrom.getDate()/10000);
	basar::VarString line;
	basar::Int16 count = 6;
	if( (dateFrom.getDate() % 10000) > 700 )	
	{
		dateFrom.setDate( year, 7, 1 );
		dateTo.setDate( year, 12, 31 );
		line.format( "%s - %s", dateFrom.toStrDate().c_str(), dateTo.toStrDate().c_str() );
		cbReportPeriod->addItem( QString::fromLocal8Bit(line.c_str()) );
	}
	else
	{
		dateFrom.setDate( year, 1, 1 );	
		dateTo.setDate( year, 6, 30 );
		line.format( "%s - %s", dateFrom.toStrDate().c_str(), dateTo.toStrDate().c_str() );
		cbReportPeriod->addItem( QString::fromLocal8Bit(line.c_str()) );
	}
	if( (dateFrom.getDate() % 10000) > 700 )
	{
		dateFrom.setDate( year, 1, 1 );	
		dateTo.setDate( year, 6, 30 );
		line.format( "%s - %s", dateFrom.toStrDate().c_str(), dateTo.toStrDate().c_str() );
		cbReportPeriod->addItem( QString::fromLocal8Bit(line.c_str()) );
		--count;
	}
	--year;
	bool firstHalf = false;
	for( ; count>0; count-- )
	{
		if( firstHalf )
		{
			dateFrom.setDate( year, 1, 1 );	
			dateTo.setDate( year, 6, 30 );
			line.format( "%s - %s", dateFrom.toStrDate().c_str(), dateTo.toStrDate().c_str() );
			--year;
		}
		else 
		{
			dateFrom.setDate(year, 7, 1 );	
			dateTo.setDate( year, 12, 31 );
			line.format( "%s - %s", dateFrom.toStrDate().c_str(), dateTo.toStrDate().c_str() );
		}
		cbReportPeriod->addItem( QString::fromLocal8Bit(line.c_str()) );
		firstHalf = !firstHalf;
	}
	m_ReportTypeGroup.addButton( obtC1,0 );
	m_ReportTypeGroup.addButton( obtC2,1 );
	m_ReportTypeGroup.addButton( obtC4,2 );
	obtC1->setChecked( true );
	tableWidget->hideVerticalHeader();
	tableWidget->setDisabled( true );
	btnDelete->hide();
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::init( basar::db::aspect::AccessorPropertyTableRef openOrders )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initGuiCtrls();
	m_matData.reset();
	m_openOrder = openOrders;
}

//-------------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum HalfYearlyReportVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

//-------------------------------------------------------------------------------------------------//
bool HalfYearlyReportVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matData.reset();
	return close();
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::matchFromPeriod( basar::DateTime & from, basar::DateTime & to, ReportTypeEnum & reportType )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromPeriod of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QString dates = cbReportPeriod->currentText();
	QStringList listdates = dates.split( "-", QString::SkipEmptyParts );
	basar::VarString date = listdates.last().trimmed().toLocal8Bit().data();
	to.fromStrDate( date );
	date = listdates.first().trimmed().toLocal8Bit().data();
	from.fromStrDate(date);
	reportType = ReportTypeEnum( m_ReportTypeGroup.checkedId() + 1 );
	m_matData.reset();
	tableWidget->clear();
	tableWidget->setDisabled( false );
	btnPrint->setDisabled( false );
	QStringList lstCaptions;
	if( reportType == C4Report )
	{
		tableWidget->setColumnCount(7);
		lstCaptions	<< tr("Article-No.")	<< tr("Article")		<< tr("Date")		
					<< tr("Order-No.")		<< tr("Supplier-No.")	<< tr("Supplier")		<< tr("QtyOrdered");
		
		tableWidget->setHorizontalHeaderLabels( lstCaptions );
		for( int i = 0; i < tableWidget->columnCount(); i++ )
		{
			tableWidget->setEditableColumn( i, false );
		}
		tableWidget->resizeColumnsToContents();
		m_matData = basar::gui::tie::Manager::getInstance().createMatcher( frame, m_openOrder );
		std::list<basar::VarString> attributeNames;
		attributeNames.push_back( constants::ARTICLECODE		);
		attributeNames.push_back( constants::ARTICLENAME		);
		attributeNames.push_back( "orderdate"					);
		attributeNames.push_back( constants::PURCHASEORDERNO	);
		attributeNames.push_back( constants::SUPPLIERNO			);
		attributeNames.push_back( constants::SUPPLIERNAME		);
		attributeNames.push_back( "qtyorder"					);
		m_matData.transformProperties( tableWidget->horizontalHeaderLabels(), attributeNames );
		btnDelete->show();
		btnPrint->show();
	}
	else 
	{
		btnPrint->hide();
		btnDelete->hide();
		tableWidget->setDisabled( true );
	}
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::matchToData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToData of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	tableWidget->clearContext();
	m_matData.RightToLeft( 0, tableWidget->rowCount() );
	tableWidget->resizeColumnsToContents();
}

//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator HalfYearlyReportVC::getCurrentAccessorIterator()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getCurrentAccessorIterator of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matData.getSelectedRight();
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::SystemEventManager::getInstance().fire( "ShowHalfYearlyReport" );
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::onBtnPrint_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnPrint_clicked of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::SystemEventManager::getInstance().fire( "PrintHalfYearlyReport" );
	setCursor( Qt::ArrowCursor );
}

//-------------------------------------------------------------------------------------------------//
void HalfYearlyReportVC::onBtnDelete_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnDelete_clicked of HalfYearlyReportVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::SystemEventManager::getInstance().fire( "DeleteHalfYearlyReport" );
	setCursor( Qt::ArrowCursor );
}

} //namespace viewConn
} //namespace narcotics
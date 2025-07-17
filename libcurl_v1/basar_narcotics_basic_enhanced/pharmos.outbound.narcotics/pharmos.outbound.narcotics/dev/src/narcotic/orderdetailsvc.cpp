//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Marco Köppendörfer
 *  \date   16.10.2012
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "orderdetailsvc.h"
#include "loggerpool.h"
#include "narcoticsappl.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// !\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
OrderDetailsVC::OrderDetailsVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
	initGuiCtrls();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// !\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
OrderDetailsVC::~OrderDetailsVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// !\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::wireEvents()
{
	QObject::connect( btnClose,	SIGNAL(	clicked()			 ),	
					  this,		SLOT  (	onBtnClose_clicked() ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	withSearchCriteria regulates if the dialog is initialized with or without the search criterias.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::initGuiCtrls()
{
	//--------------------------------------------------------//
	// disable the order specific data frame
	//--------------------------------------------------------//
	setDisabled_grpOrderData( true );

	transactiondate->setDisplayFormat( "dd.MM.yyyy" );
	transactiontime->setDisplayFormat( "hh:mm:ss"	);
    
	//--------------------------------------------------------//
	//	disable the narcotic bookings specific data frame 
	//	'grpNarcoticBookingsData'
	//--------------------------------------------------------//
	setDisabled_grpNarcoticsBookingsData( true );

	//--------------------------------------------------------//
	// initialization QTableWidget
	//--------------------------------------------------------//
	initTableWidget();

	//--------------------------------------------------------//
	// init of original attributes for QTableWidget headings
	//--------------------------------------------------------//
	if( m_OriginalAttributeNames.empty() )
	{
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONDATE		 );
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONTIME		 );
		m_OriginalAttributeNames.push_back( constants::ARTICLENAME			 );
		m_OriginalAttributeNames.push_back( constants::ARTICLECODE			 );
		m_OriginalAttributeNames.push_back( constants::PHARMAFORM			 );
		m_OriginalAttributeNames.push_back( constants::PACKAGEUNIT			 ); // ToDo: packageunit per article from "artikelzentral" ("einheit") is used at the moment
		m_OriginalAttributeNames.push_back( constants::QTYTRANSACTION		 );
		m_OriginalAttributeNames.push_back( constants::CONTRIBUTIONVOUCHERNO );
        m_OriginalAttributeNames.push_back( constants::USERNAME              );
        m_OriginalAttributeNames.push_back( constants::PRINTED_DATE          );
        m_OriginalAttributeNames.push_back( constants::PRINTED_TIME          );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::initTableWidget()
{
	QStringList lstCaptions;
	lstCaptions	<< tr("Date")			    << tr("Time")		
				<< tr("Name")			    << tr("ArticleNo")	
				<< tr("Pharmaform")		    << tr("Packageunit")	
                << tr("Quantity")		    << tr("ContributionVoucherNo")
                << tr("UserName")           << tr("PrintedDate")
				<< tr("PrintedTime");
    
	tableWidget->setColumnCount(lstCaptions.size());
	tableWidget->clear();
	tableWidget->setHorizontalHeaderLabels( lstCaptions );
	tableWidget->hideVerticalHeader();
	tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	tableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::setDisabled_grpOrderData( bool disable )
{
	grpOrderData->setDisabled( disable );

	lblOrderNo->setDisabled( disable );
	purchaseorderno->setDisabled( disable );
	//prevent purchaseorderno from being editable as it is part of the identificator of an order
	purchaseorderno->setReadOnly( true );
    
	lblBGANo->setDisabled( disable );
	customersupplierno->setDisabled( disable );
    customersupplierno->setReadOnly( true );

	lblDate->setDisabled( disable );
	transactiondate->setDisabled( disable );
	//prevent transactiondate from being editable as it is part of the identificator of an order
	transactiondate->setReadOnly( true );

	lblLocation->setDisabled( disable );
	customersupplierlocation->setDisabled( disable );
	customersupplierlocation->setReadOnly( true );

	lblName->setDisabled( disable );
	customersuppliername->setDisabled( disable );
	customersuppliername->setReadOnly( true );
    
	lblTime->setDisabled( disable );
	transactiontime->setDisabled( disable );
    transactiontime->setReadOnly( true );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::setDisabled_grpNarcoticsBookingsData( bool disable )
{
// these buttons shall only be disabled if an entry within the tableWidget has already been selected
	tableWidget->setDisabled( disable );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\param	articleNarcoticsData is the AccessorPropertyTableRef which will be connected 
			to the article specific frame via matcher.
//!	\param	narcoticsBookings is the AccessorPropertyTableRef which will be connected 
			to the query result of the narcotic bookings specific table widget.
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::setData( basar::db::aspect::AccessorPropertyTableRef bookingsCatalog )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of OrderDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	using basar::gui::tie::Manager;
	// creation of the matcher
    m_matOrderData.reset();
	m_matOrderData				= Manager::getInstance().createMatcher( grpOrderData, bookingsCatalog );
    
    m_matNarcoticBookings.reset();
	m_matNarcoticBookings		= Manager::getInstance().createMatcher( tableWidget, bookingsCatalog );

	m_matNarcoticBookings.transformProperties( tableWidget->horizontalHeaderLabels(), m_OriginalAttributeNames );

// Error when trying to insert date and time from accpropertytable into Gui QDate and QTime fields
	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().data() );
	m_matNarcoticBookings.push_back( dateTransformer );

    VariableDateTransformer printedDateTransformer( constants::PRINTED_DATE, tr("PrintedDate").toLocal8Bit().data() );
	m_matNarcoticBookings.push_back( printedDateTransformer );

	VariableTimeTransformer printedTimeTransformer( constants::PRINTED_TIME, tr("PrintedTime").toLocal8Bit().data() );
	m_matNarcoticBookings.push_back( printedTimeTransformer );

	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr("Time").toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( timeTransformer );
}

//-----------------------------------------------------------------------------------------------//
void OrderDetailsVC::colorRows()
{
    basar::Int32 rows = tableWidget->rowCount();
    basar::db::aspect::AccessorPropertyTableRef probTab = m_matNarcoticBookings.getRight();
    basar::db::aspect::AccessorPropertyTable_YIterator yit = probTab.begin();

    for( int i = 0; !yit.isEnd() && i < rows ; i++, yit++ )
    {
        if( yit.contains( constants::ACKNOWLEDGEMENTSTATUS ) )
        {
            if( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) == constants::IN_PRINT ){
                tableWidget->setBackgroundColorRow( i, Qt::red );
            }
            else{
                tableWidget->setBackgroundColorRow( i, Qt::white );
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------//
void OrderDetailsVC::matchToData()
{
    m_matOrderData.RightToLeft();
    tableWidget->setRowCount( static_cast<basar::Int32>( m_matNarcoticBookings.getRight().size() ) );
    m_matNarcoticBookings.RightToLeft();

    colorRows();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	int returns the result (DialogCode) of a modally shown dialog
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum OrderDetailsVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of OrderDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

    setDisabled_grpOrderData( false );
    setDisabled_grpNarcoticsBookingsData( false );

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	bool is always return at the time.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool OrderDetailsVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of OrderDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matOrderData.reset();
	m_matNarcoticBookings.reset();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderDetailsVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of OrderDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

//-----------------------------------------------------------------------------------------------//
void OrderDetailsVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

} //namespace viewConn
} //namespace narcotics
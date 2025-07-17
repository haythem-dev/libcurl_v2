//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher
 *  \date   24.03.2010
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "resolveprintingerrorsvc.h"
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

ResolvePrintingErrorsVC::ResolvePrintingErrorsVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
ResolvePrintingErrorsVC::~ResolvePrintingErrorsVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::wireEvents()
{
	QObject::connect(	btnShowDetails,	SIGNAL( clicked()							 ),	
						this,			SLOT  (	onBtnShowDetails_clicked()			 ));
	QObject::connect(	btnSolve,		SIGNAL( clicked()							 ),	
						this,			SLOT  (	onBtnSolve_clicked()				 ));
	QObject::connect(	btnClose,		SIGNAL(	clicked()							 ),	
						this,			SLOT  (	onBtnClose_clicked()				 ));
	QObject::connect(	tablewidget,	SIGNAL(	itemSelectionChanged()				 ),	
						this,			SLOT  (	onTablewidget_itemSelectionChanged() ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::init()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initGuiCtrls();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::setData( basar::db::aspect::AccessorPropertyTableRef table )
{
    m_matTableWidget.reset();
    m_matTableWidget	= basar::gui::tie::Manager::getInstance().createMatcher( gBox, table );
	std::list<VarString> attribute;
    attribute.push_back( "customersuppliername" );
	attribute.push_back( "customersupplierno" );
	attribute.push_back( "purchaseorderno" );
    attribute.push_back( "contributionvoucherno" );
	m_matTableWidget.transformProperties( tablewidget->horizontalHeaderLabels(), attribute );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return int-value of the methode QDialog::exec()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum ResolvePrintingErrorsVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return bool-value of the method QDialog::close()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool ResolvePrintingErrorsVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matTableWidget.reset();

	return close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::initGuiCtrls()
{
	QStringList list;
    list << tr("customersuppliername") << tr("customersupplierno") << tr("purchaseorderno") << tr("contributionvoucherno") ;
	tablewidget->setHorizontalHeaderLabels( list );
	tablewidget->hideVerticalHeader();
	for( int i=0; i< tablewidget->rowCount(); i++ )
	{
		tablewidget->setEditableRow( i, false );
	}
	tablewidget->resizeColumnsToContents();
	tablewidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
	
	/////////////////////////////////////////////////////////////////////////
	// gBox data
	/////////////////////////////////////////////////////////////////////////
	gBox->setDisabled( false );

	/////////////////////////////////////////////////////////////////////////
	// buttons
	/////////////////////////////////////////////////////////////////////////
	btnClose->setEnabled( true );
	btnShowDetails->setEnabled( false );
    btnSolve->setEnabled( false );
}

//---------------------------------------------------------------------------------------------------------------------//
// match methods
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::matchToWidget()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToGroup of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	tablewidget->clearContext();
	const basar::Int32 matchedHits = m_matTableWidget.RightToLeft( 0, tablewidget->rowCount() );
	tablewidget->resizeColumnsToContents();
    tablewidget->setRowCount( matchedHits );

    colorRows();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool ResolvePrintingErrorsVC::isInPrintBookingAlreadyTransfered( basar::db::aspect::AccessorPropertyTable_YIterator selectedBooking )
{
    if( !selectedBooking.isNull() && !selectedBooking.isEnd() )
    {
        //use printeddate if != 0, transactiondate otherwise

        basar::cmnutil::Date transactionDateToCheck;

        if( selectedBooking.isContainedAndSet( constants::PRINTED_DATE ) && 
			selectedBooking.getInt32( constants::PRINTED_DATE ) > 0 )
		{
            transactionDateToCheck.setDate( selectedBooking.getInt32( constants::PRINTED_DATE ) );
        }
        else
		{
            transactionDateToCheck.setDate( selectedBooking.getInt32( constants::TRANSACTIONDATE ) );
        }

        if( transactionDateToCheck.getDate() <= getLastSundaysDate().getDate() ){
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
const basar::cmnutil::Date ResolvePrintingErrorsVC::getLastSundaysDate()
{
    basar::cmnutil::Date date;
    //sunday => dayOfWeek == 0
    date.addDays( date.getDayOfWeek() * -1 );
    return date;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::colorRows()
{
    basar::Int32 rows = tablewidget->rowCount();
    basar::db::aspect::AccessorPropertyTableRef probTab = m_matTableWidget.getRight();
    basar::db::aspect::AccessorPropertyTable_YIterator yit = probTab.begin();

    for( int i = 0; !yit.isEnd() && i < rows ; i++, yit++ )
    {
        if( isInPrintBookingAlreadyTransfered( yit ) ){
            tablewidget->setTextColorRow( i, Qt::lightGray );
        }
        else{
            //set default color
            tablewidget->setTextColorRow( i, Qt::black );
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentArticle is the AccessorPropertyTable_YIterator which is going to be used to search for narcotic group details
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator ResolvePrintingErrorsVC::getCurrentSelection()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "ResolvePrintingErrorsVC::getCurrentSelection()";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matTableWidget.getCurrentRight();
	// if a correct selection has been cancelled or no selection has been executed at all, yit has to be resetted
	if( yit.isEnd() ){
		yit.clear();
	}

	return yit;
}

//---------------------------------------------------------------------------------------------------------------------//
// private slots
//---------------------------------------------------------------------------------------------------------------------//
void ResolvePrintingErrorsVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

//---------------------------------------------------------------------------------------------------------------------//
void ResolvePrintingErrorsVC::onBtnSolve_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSolve_clicked of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "ResolvePrintingErrors" );
	setCursor( Qt::ArrowCursor );

	//if (false == ret.isOK())
	//{
	//	basar::gui::tie::warningMsgBox( this, ret.message.c_str() );
	//}
}

//---------------------------------------------------------------------------------------------------------------------//
void ResolvePrintingErrorsVC::onBtnShowDetails_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnShowDetails_clicked of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "DisplayOrderDetailsRequested" );
}

//-----------------------------------------------------------------------------------------------//
// ctrl events
//-----------------------------------------------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResolvePrintingErrorsVC::onTablewidget_itemSelectionChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTablewidget_itemSelectionChanged of ResolvePrintingErrorsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

    btnShowDetails->setDisabled( tablewidget->selectedRanges().isEmpty() );

    bool disabled = tablewidget->selectedRanges().isEmpty();
    disabled |= hasSelectedBookingAlreadyBeenTransfered();

    btnSolve->setDisabled( disabled );
}

//---------------------------------------------------------------------------------------------------------------------//
bool ResolvePrintingErrorsVC::hasSelectedBookingAlreadyBeenTransfered()
{
    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matTableWidget.getCurrentRight();

    return isInPrintBookingAlreadyTransfered( yit );
}

} //namespace viewConn
} //namespace narcotics
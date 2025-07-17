//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// c'tor
///////////////////////////////////////////////////////////////////////////////////////////////////
CompareBookingsVC::CompareBookingsVC( QWidget *parent )
: QDialog( parent ), m_invokeContextDiffQtyInStock( false )
{
	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// d'tor
///////////////////////////////////////////////////////////////////////////////////////////////////
CompareBookingsVC::~CompareBookingsVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::wireEvents()
{
	QObject::connect(	cboArticleNos,		SIGNAL( currentIndexChanged( QString )					),
						this,				SLOT  (	onCboArticleNos_currentIndexChanged( QString )	));
	QObject::connect(	txtArticleNoSearch,	SIGNAL( textChanged( QString )							),
						this,				SLOT  (	onTxtArticleNoSearch_textChanged( QString )		));
	QObject::connect(	btnClose,			SIGNAL( clicked()										),
						this,				SLOT  (	onBtnClose_clicked()							));
	QObject::connect(	btnDelete,			SIGNAL( clicked()										),
						this,				SLOT  (	onBtnDelete_clicked()							));
	QObject::connect(	btnRebuild,			SIGNAL( clicked()										),
						this,				SLOT  (	onBtnRebuild_clicked()							));
	QObject::connect(	btnSearch,			SIGNAL( clicked()										),
						this,				SLOT  (	onBtnSearch_clicked()							));
	QObject::connect(	this,				SIGNAL( finished( int )									),
						this,				SLOT  (	closeDialog()									));
	QObject::connect(	narcotics,			SIGNAL( itemSelectionChanged()							),
						this,				SLOT  (	itemSelectedIsChange()							));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::init( AccessorPropertyTableRef dataTable,		AccessorPropertyTableRef narcoticsTable,
							  AccessorPropertyTableRef originalTable,	AccessorPropertyTableRef articleDiff,
							  AccessorPropertyTableRef articleDiffArtNos, bool invokeContextDiffQtyInStock		)
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "init of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	initGuiCtrls();
	createMatcher( dataTable, narcoticsTable, originalTable, articleDiff, articleDiffArtNos );
	setResultsInTable( invokeContextDiffQtyInStock );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::initGuiCtrls()
{
	initSearchFrame();
	initArticleDataFrame();
	initNarcoticDataFrame();
	initBasarTableWidgets();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::initSearchFrame()
{
	cboArticleNos->clear();
	cboArticleNos->hide();

	txtArticleNoSearch->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,8}"), this ) );
	txtArticleNoSearch->setText("");
	txtArticleNoSearch->setFocus();
	
	lblFromSearch->hide();
	lblToSearch->hide();

	dtFromSearch->hide();
	dtFromSearch->setDate( QDate::currentDate() );
	dtFromSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtFromSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	dtToSearch->hide();
	dtToSearch->setDate( QDate::currentDate() );
	dtToSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtToSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	btnSearch->setDisabled( true );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::initArticleDataFrame()
{
	articlecode->setText("");
	articlepackageunit->setText("");
	articlename->setText("");
	articlepharmaform->setText("");
	diffqtyinstock->setText("");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::initNarcoticDataFrame()
{
	group->setText("");
	name->setText("");
	packageunit->setText("");
	concentration->setText("");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::initBasarTableWidgets()
{
	QStringList lstCaptions;
	lstCaptions	<< tr("Date") << tr("BGA-No.") << tr("TT") << tr("Order-No.") << tr("Quantity") << tr("QtyInStock");

	narcotics->setHorizontalHeaderLabels( lstCaptions );
	narcotics->hideVerticalHeader();
	for( int i = 0; i < narcotics->columnCount(); i++ )
	{
		narcotics->setEditableColumn( i, false );
	}
	narcotics->clearContext();
	narcotics->resizeColumnsToContents();
	narcotics->setEditTriggers( QAbstractItemView::NoEditTriggers );
	
	original->setHorizontalHeaderLabels( lstCaptions );
	original->hideVerticalHeader();
	for( int i = 0; i < original->columnCount(); i++ )
	{
		original->setEditableColumn( i, false );
	}
	original->clearContext();
	original->resizeColumnsToContents();
	original->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::createMatcher(	AccessorPropertyTableRef dataTable,		AccessorPropertyTableRef narcoticsTable,
										AccessorPropertyTableRef originalTable,	AccessorPropertyTableRef articleDiff,
										AccessorPropertyTableRef articleDiffArtNos )
{
	m_matArticleNosDiffQtyInStock = basar::gui::tie::Manager::getInstance().createMatcher( grpSearchCriteria, articleDiffArtNos );
	basar::cmnutil::ParameterList pl( "articlecode=cboArticleNos;" );
	basar::gui::tie::PropertyNameTransformer pnt( pl );
	m_matArticleNosDiffQtyInStock.push_back( pnt );

	m_matNarcoticsData = basar::gui::tie::Manager::getInstance().createMatcher( fraArticleNarcoticData, dataTable );
	m_matNarcoticBookings = basar::gui::tie::Manager::getInstance().createMatcher( grpNarcoticBookingsData, narcoticsTable );

	std::list<VarString> originalAttributeNames;
	originalAttributeNames.push_back( constants::TRANSACTIONDATE	);
	originalAttributeNames.push_back( constants::CUSTOMERSUPPLIERNO );
	originalAttributeNames.push_back( constants::TRANSACTIONTYPE	);
	originalAttributeNames.push_back( constants::PURCHASEORDERNO	);
	originalAttributeNames.push_back( constants::QTYTRANSACTION		);
	originalAttributeNames.push_back( constants::QTYINSTOCK			);

	m_matNarcoticBookings.transformProperties( narcotics->horizontalHeaderLabels(), originalAttributeNames );

	m_matOriginalBookings = basar::gui::tie::Manager::getInstance().createMatcher( grpOriginalicBookingsData, originalTable );
	m_matOriginalBookings.transformProperties( original->horizontalHeaderLabels(), originalAttributeNames );

	m_matArticleDifference = basar::gui::tie::Manager::getInstance().createMatcher( fraArticleNarcoticData, articleDiff );

	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr( "Date" ).toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( dateTransformer );
	m_matOriginalBookings.push_back( dateTransformer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::setResultsInTable( bool invokeContextDiffQtyInStock )
{
	m_invokeContextDiffQtyInStock = invokeContextDiffQtyInStock;
	if( invokeContextDiffQtyInStock )
	{
		basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchArticleNoWithDiffQtyInStock" );
		txtArticleNoSearch->hide();	

		if( ret.ret == basar::appl::HANDLER_INFO ){
			basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		}

		cboArticleNos->show();

		lblFromSearch->hide();
		dtFromSearch->hide();
		lblToSearch->hide();
		dtToSearch->hide();
		if( 0 != cboArticleNos->count() )
		{
			cboArticleNos->setCurrentIndex( 0 );
			btnSearch->setDisabled( false );
			btnSearch->move( 210, 18 );
			basar::appl::SystemEventManager::getInstance().fire( "SearchCompareBookings" );
		}
	} 
	else 
	{
		txtArticleNoSearch->show();	
		cboArticleNos->hide();
		
		lblFromSearch->show();
		dtFromSearch->show();
		lblToSearch->show();
		dtToSearch->show();
		btnSearch->move( 510, 18 );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum CompareBookingsVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CompareBookingsVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matNarcoticsData.reset();
	m_matNarcoticBookings.reset();
	m_matOriginalBookings.reset();
	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CompareBookingsVC::getCurrentAccessorIterator()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getCurrentAccessorIterator of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matNarcoticBookings.getCurrentRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CompareBookingsVC::isSelected()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "isSelected of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return btnSearch->isEnabled();
}

//---------------------------------------------------------------------------------------------------------------------------//
//	match methods
//---------------------------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchToArticleNosWithDiffQtyInStock()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleNosWithDiffQtyInStock of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matArticleNosDiffQtyInStock.RightToLeft();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSelection of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_invokeContextDiffQtyInStock ){
		searchIt.setString( constants::ARTICLECODE, cboArticleNos->currentText().toLocal8Bit().constData() );
	} 
	else
	{
		searchIt.setString( constants::ARTICLECODE, txtArticleNoSearch->text().toLocal8Bit().constData()				);
		searchIt.setString( constants::FROMDATE,	dtFromSearch->date().toString("yyyyMMdd").toLocal8Bit().constData() );
		searchIt.setString( constants::TODATE,		dtToSearch->date().toString("yyyyMMdd").toLocal8Bit().constData()	);	
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchToNarcoticData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticData of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matNarcoticsData.RightToLeft();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchToArticleDifference()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleDifference of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	diffqtyinstock->setText( "0" );
	m_matArticleDifference.RightToLeft();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchToNarcoticBookings()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticBookings of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	narcotics->clearContext();
	m_matNarcoticBookings.RightToLeft( 0, narcotics->rowCount() );
	narcotics->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::matchToOriginalBookings()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToOriginalBookings of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	original->clearContext();
	m_matOriginalBookings.RightToLeft( 0, original->rowCount() );
	original->resizeColumnsToContents();
}

//-------------------------------------------------------------------------------------------------------------------//
//	ctrl events
//-------------------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( dtFromSearch->date() > dtToSearch->date() )
	{
		basar::gui::tie::infoMsgBox( this,
			tr("To date must be greater than or equal to from date!").toLocal8Bit().constData(),
			tr("Narcotics").toLocal8Bit().constData() );
		return;
	}

	setCursor( Qt::WaitCursor );
	if (basar::appl::SystemEventManager::getInstance().fire( "SearchCompareBookings" ).ret == basar::appl::HANDLER_ERROR)
	{
		narcotics->clearContext();
		narcotics->clearContext();
	}
	setCursor( Qt::ArrowCursor );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onBtnRebuild_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnRebuild_clicked of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::SystemEventManager::getInstance().fire( "RebuildNarcoticsBooking" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onBtnDelete_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnDelete_clicked of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( basar::gui::tie::questionMsgBox( this, 
		tr("Do you really want to delete this booking?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		basar::appl::SystemEventManager::getInstance().fire( "DeleteNarcoticsBooking" );
		setCursor( Qt::ArrowCursor );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onTxtArticleNoSearch_textChanged( const QString& text )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtArticleNoSearch_textChanged of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setDisabled( text.isEmpty() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::closeDialog()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "closeDialog of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::itemSelectedIsChange()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "itemSelectedIsChange of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnDelete->setDisabled( narcotics->selectedRanges().empty() || m_matNarcoticBookings.getCurrentRight().isEnd() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CompareBookingsVC::onCboArticleNos_currentIndexChanged( const QString& text )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onCboArticleNos_currentIndexChanged of CompareBookingsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	text;
	if (basar::appl::SystemEventManager::getInstance().fire( "SearchCompareBookings" ).ret == basar::appl::HANDLER_ERROR)
	{
		narcotics->clearContext();
		narcotics->clearContext();
	}
}

} //namespace viewConn
} //namespace narcotics
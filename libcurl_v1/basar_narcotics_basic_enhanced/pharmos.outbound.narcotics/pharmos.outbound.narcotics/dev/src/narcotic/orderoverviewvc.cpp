//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Bjoern Bischof
 *  \date   17.02.2006
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"

#include "narcoticstransformer.h"
#include "checkboxtabletransformer.h"
#include "orderoverviewvc.h"
#include "loggerpool.h"
#include "narcoticsappl.h"

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
// !\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
OrderOverviewVC::OrderOverviewVC( QWidget *parent )
:	QDialog( parent ), 
	m_DetailVCShown( false ), 
	m_BackupNo( "0" ), 
	m_currentLineBgaNo( 0 ), 
	m_currentLineLineCode( 0 ), 
	m_orderHeadDataChanged( false ),
	m_CurrentAckStatus( 0 )
{
	setupUi(this);
	wireEvents();
	initGuiCtrls();
}

//-----------------------------------------------------------------------------------------------//
OrderOverviewVC::~OrderOverviewVC()
{
}
//-----------------------------------------------------------------------------------------------//

void OrderOverviewVC::wireEvents()
{
	QObject::connect(	transactiontime,			SIGNAL( dateTimeChanged( const QDateTime& )			),
						this,						SLOT  ( onTransactiontime_changed()					));
	QObject::connect(	transactiondate,			SIGNAL( dateChanged( const QDate& )					),
						this,						SLOT  ( onTransactiondate_changed( const QDate& )	));
	QObject::connect(	cboBgaNo,					SIGNAL( currentIndexChanged( int )					),
						this,						SLOT  ( currentIndexOfBgaNoChanged( int )			));
	QObject::connect(	cboLineCode,				SIGNAL( currentIndexChanged( int )					),
						this,						SLOT  ( currentIndexOfLineCodeChanged( int )		));
	QObject::connect(	type,						SIGNAL( currentIndexChanged( int )					),
						this,						SLOT  ( onType_changed( int )						));
	QObject::connect(	customersupplierno,			SIGNAL( editingFinished()							),
						this,						SLOT  ( onCustomersupplierno_editingFinished()		));
	QObject::connect(	tableWidget,				SIGNAL( itemSelectionChanged()						),
						this,						SLOT  ( onTableWidget_itemSelectionChanged()		));
	QObject::connect(	btnEdit,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnEdit_clicked()							));
	QObject::connect(	btnSearch,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnSearch_clicked()						));
	QObject::connect(	btnClose,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnClose_clicked()						));
	QObject::connect(	btnSaveOrderChanges,		SIGNAL( clicked()									),
						this,						SLOT  ( onBtnSaveOrderChanges_clicked()				));
	QObject::connect(	btnDelete,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnDelete_clicked()						));
	QObject::connect(	btnCancelOrderPositions,	SIGNAL( clicked()									),
						this,						SLOT  ( onBtnCancelOrderPositions_clicked()			));
	QObject::connect(	btnCancelFullOrder,			SIGNAL( clicked()									),
						this,						SLOT  ( onBtnCancelFullOrder_clicked()				));
	QObject::connect(	btnPrintDisolveReceipt,		SIGNAL( clicked()									),
						this,						SLOT  ( onBtnPrintDisolveReceipt_clicked()			));
	QObject::connect(	orderNoSearch,				SIGNAL( textChanged( QString )						),
						this,						SLOT  ( onOrderNoSearch_textChanged( QString )		));
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::initCboBgaNo()
{
	cboBgaNo->clearContext();

	if( m_BgaOriginalAttributeNames.empty() )
	{	
		m_BgaOriginalAttributeNames.push_back( constants::CUSTOMERSUPPLIERNO			);
		m_BgaOriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_NAME		);
		m_BgaOriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_LOCATION	);
	}

	cboBgaNo->setColumnCount( 3 );
	cboBgaNo->setHorizontalHeaderItem( 0, tr("BGA")  );
	cboBgaNo->setHorizontalHeaderItem( 1, tr("Name") );
	cboBgaNo->setHorizontalHeaderItem( 2, tr("Location") );

	cboBgaNo->setMinimumHeightList( 250 );
    cboBgaNo->setMinimumWidthList( 600 );
    //cboBgaNo->setAutoCompletion( true );
    cboBgaNo->completer();
	cboBgaNo->resizeColumnsToContents();
	cboBgaNo->resizeRowsToContents();
	cboBgaNo->clearEditText();
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::setLineCodeToSelectedTransactionType( int index )
{
	cboLineCode->blockSignals(true);
	cboLineCode->clearContext();
	m_lineCodes.clear();

	QString cboText;
	basar::VarString transType;

	switch (index) 
	{ 
	case INDEX_GI:
		cboText = tr( "Incoming Delivery");
		transType = TransactionType::GOODSIN_DELIVERY;
		break;

	case INDEX_CUST_RETURN:
		cboText = tr( "Return (customer)");
		transType = TransactionType::CUSTOMER_RETURNS;
		break;

	case INDEX_DESTRUCTION:
		cboText = tr( "Destruction");
		transType = TransactionType::DESTRUCTION;
		break;

	default:
		break;
	}

	cboLineCode->addItem( 0, 0, cboText			);
	cboLineCode->addItem( 0, 1, transType.c_str()	);
	m_lineCodes.push_back( transType		);

	cboLineCode->blockSignals(false);

	disableSaveOrderButton();
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::setLineCodeToSupplReturn()
{
	cboLineCode->blockSignals(true);
	cboLineCode->clearContext();
	m_lineCodes.clear();

	cboLineCode->addItem( 0, 0, tr( "Return (supplier)" )			);
	cboLineCode->addItem( 0, 1, TransactionType::SUPPLIER_RETURNS	);
	m_lineCodes.push_back( TransactionType::SUPPLIER_RETURNS		);

	cboLineCode->addItem( 1, 0, tr( "Destruction" )					);
	cboLineCode->addItem( 1, 1, TransactionType::DESTRUCTION		);
	m_lineCodes.push_back( TransactionType::DESTRUCTION				);

	cboLineCode->blockSignals(false);

	disableSaveOrderButton();
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::setLineCodeToSellOf()
{
	cboLineCode->blockSignals(true);
	cboLineCode->clearContext();
	m_lineCodes.clear();

	cboLineCode->addItem( 0, 0, tr( "Sell Off" )					);
	cboLineCode->addItem( 0, 1, TransactionType::CUSTOMER_ORDER		);
	m_lineCodes.push_back( TransactionType::CUSTOMER_ORDER			);

	cboLineCode->addItem( 1, 0, tr( "Return (supplier)" )			);
	cboLineCode->addItem( 1, 1, TransactionType::SUPPLIER_RETURNS	);
	m_lineCodes.push_back( TransactionType::SUPPLIER_RETURNS		);

	cboLineCode->addItem( 2, 0, tr( "Destruction" )					);
	cboLineCode->addItem( 2, 1, TransactionType::DESTRUCTION		);
	m_lineCodes.push_back( TransactionType::DESTRUCTION				);

	cboLineCode->blockSignals(false);

	disableSaveOrderButton();
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::initCboLineCode()
{
	cboLineCode->setColumnCount( 2 );
	cboLineCode->setHorizontalHeaderItem( 0, tr( "Transactionname" )		);
	cboLineCode->setHorizontalHeaderItem( 1, tr( "Transactiontype" )		);

	setLineCodeToSellOf();
	
	cboLineCode->resizeColumnsToContents();
	cboLineCode->resizeRowsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	withSearchCriteria regulates if the dialog is initialized with or without the search criterias.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::initGuiCtrls( bool withSearchCriteria )
{
    //--------------------------------------------------------//
	//	prepare the search data frame with default values
	//	'grpSearchData'
	//--------------------------------------------------------//
	if( withSearchCriteria )
	{
		if( 0 == orderNoSearch->validator() ){
			orderNoSearch->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,7}"), this )  );
		}
		orderNoSearch->setText("");
		orderNoSearch->setFocus();

		dtSearch->setDate( QDate::currentDate() );
		dtSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );	
		dtSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

		btnSearch->setDisabled( true );
	}

	//--------------------------------------------------------//
	// disable the order specific data frame
	//--------------------------------------------------------//
	setDisabled_grpOrderData( true );

	// set validator for customersupplierno as user will now be able to change its value
	customersupplierno->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,7}"), this )  );

	QColor color( "red" ); 
	QPalette palette = purchaseorderno->palette();
	palette.setColor( QPalette::Text, color );
	purchaseorderno->setPalette( palette );
    purchaseorderno->setText( "" );
	purchaseorderno->setReadOnly( true );

	palette = customersuppliername->palette();
	palette.setColor( QPalette::Text, color );
	customersuppliername->setPalette( palette );
	customersuppliername->setText( "" );

	palette = customersupplierno->palette();
	palette.setColor( QPalette::Text, color );
	customersupplierno->setPalette( palette );
	customersupplierno->setText( "" );

	palette = customersupplierlocation->palette();
	palette.setColor( QPalette::Text, color );
	customersupplierlocation->setPalette( palette );
	customersupplierlocation->setText( "" );

	palette = customersuppliercip->palette();
	palette.setColor(QPalette::Text, color);
	customersuppliercip->setPalette(palette);
	customersuppliercip->setText("");

	transactiondate->setDisplayFormat( "dd.MM.yyyy" );
	transactiontime->setDisplayFormat( "hh:mm:ss" );
	
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
		m_OriginalAttributeNames.push_back(	constants::SELECTION		);
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONDATE	);
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONTIME	);
		m_OriginalAttributeNames.push_back( constants::ARTICLENAME		);
		m_OriginalAttributeNames.push_back( constants::ARTICLECODE		);
		m_OriginalAttributeNames.push_back( constants::PHARMAFORM		);
		m_OriginalAttributeNames.push_back( constants::PACKAGEUNIT		); // ToDo: packageunit per article from "artikelzentral" ("einheit") is used at the moment
		m_OriginalAttributeNames.push_back( constants::QTYTRANSACTION	);
		m_OriginalAttributeNames.push_back( constants::QTYINSTOCK		);
	}

	initCboLineCode();

	initCboBgaNo();

	//--------------------------------------------------------//
	// dialog buttons
	//--------------------------------------------------------//
	// button btnSaveOrderChanges may not be activated before a successful search
	// and changes to order specific data have been conducted
	disableSaveOrderButton();

	orderNoSearch->installEventFilter(this);
	dtSearch->installEventFilter(this);
	type->installEventFilter(this);

	// REFACTORING remove code behind these buttons
	btnEdit->setVisible( false );
	btnDelete->setVisible( false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::initTableWidget()
{
	QStringList lstCaptions;
	lstCaptions	<< tr("Cancel")			<< tr("Date")			<< tr("Time")		
				<< tr("Name")			<< tr("ArticleNo")	
				<< tr("Pharmaform")		<< tr("Packageunit")	
				<< tr("Quantity")		<< tr("QtyInStock");
	
	tableWidget->setColumnCount( lstCaptions.size() );
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
void OrderOverviewVC::setDisabled_grpOrderData( bool disable )
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

	lblLocation->setDisabled( disable );
	customersupplierlocation->setDisabled( disable );
	customersupplierlocation->setReadOnly( true );

	lblName->setDisabled( disable );
	customersuppliername->setDisabled( disable );
	customersuppliername->setReadOnly( true );

	lblCip->setDisabled(disable);
	customersuppliercip->setDisabled(disable);
	customersuppliercip->setReadOnly(true);
	
	lblTime->setDisabled( disable );
	transactiontime->setDisabled( disable );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::setDisabled_grpNarcoticsBookingsData( bool disable )
{
// these buttons shall only be enabled if an entry within the tableWidget has already been selected
	btnEdit->setDisabled( true );
	btnDelete->setDisabled( true );

	btnCancelOrderPositions->setDisabled( true );
	btnCancelFullOrder->setDisabled( true );
	// CR-14D140 nach Umsetzung beide Buttons "*stornieren" umbenennen nach "*löschen"

	btnPrintDisolveReceipt->setDisabled( true );
	
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
void OrderOverviewVC::init( basar::db::aspect::AccessorPropertyTableRef bookingsCatalog, basar::db::aspect::AccessorPropertyTableRef narcoticData )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	using basar::gui::tie::Manager;
	// creation of the matcher
	
	m_matOrderData			= Manager::getInstance().createMatcher( grpOrderData, bookingsCatalog );
	m_matSupplier			= Manager::getInstance().createMatcher( cboBgaNo, bookingsCatalog );
	m_matNarcoticBookings	= Manager::getInstance().createMatcher( tableWidget, bookingsCatalog );
	m_matNarcoticData		= Manager::getInstance().createMatcher( m_RecordEditVC->grpNarcoticData, narcoticData );
	m_matNarcoticBookings.transformProperties( tableWidget->horizontalHeaderLabels(), m_OriginalAttributeNames );

// Error when trying to insert date and time from accpropertytable into Gui QDate and QTime fields
	CheckBoxTableTransformer checkBoxTransformer( tableWidget, tr("Cancel").toLocal8Bit().constData(),
		constants::SELECTION, constants::SELECTION_ON, constants::SELECTION_OFF);
	m_matNarcoticBookings.push_back( checkBoxTransformer );

	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().data() );
	m_matNarcoticBookings.push_back( dateTransformer );

	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr("Time").toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( timeTransformer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
/*! \param	text completely passed content of the QTextEdit field 'txtArticleNoSearch'.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onOrderNoSearch_textChanged( const QString& text )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_orderNoSearch_textChanged of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setDisabled( text.isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	int returns the result (DialogCode) of a modally shown dialog
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum OrderOverviewVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//-----------------------------------------------------------------------------------------------//
basar::VarString OrderOverviewVC::getCurrentLineCode()
{
	QString currentLineCode = cboLineCode->itemText( cboLineCode->currentIndex(), 1 );
	basar::VarString lc = currentLineCode.toStdString().c_str();
	return lc;
}

//-----------------------------------------------------------------------------------------------//
bool OrderOverviewVC::orderHeaderDataChanged()
{
	return m_orderHeadDataChanged;
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::currentIndexOfBgaNoChanged( int i )
{
    if( m_currentLineBgaNo == i ){
		return;
	}
	
	m_currentLineBgaNo = i;
    cboBgaNo->setCurrentIndex( i );
	enableSaveOrderButton();
	m_orderHeadDataChanged = true;
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::currentIndexOfLineCodeChanged( int i )
{
	if( m_currentLineLineCode == i ){
		return;
	}

	m_currentLineLineCode = i;

	if ( INDEX_SUPPL_RETURN == type->currentIndex() )
		++i; // increase index by 1, because "a" is not contained in cb for current type "d"
	
	// wenn aktueller transactiontype a ist, darf dieser geändert werden auf d oder z
	// wenn aktueller transactiontype d ist, darf dieser geändert werden auf z
	if ( INDEX_SELLOFF		== type->currentIndex() ||
		 INDEX_SUPPL_RETURN == type->currentIndex() )
	{
		//transactiontype a
		if( i == 0 )
		{
			AccessorPropertyTable_YIterator currentOrder = m_matOrderData.getRight().begin(); // keine Änderung des Empfängers - nur den aktuellen Kunden des Auftrags in Cb anzeigen!
			this->matchToOrderData( currentOrder );
		}
		//transactiontype d
		else if( i == 1 )
		{
			setCursor( Qt::WaitCursor );
			basar::appl::SystemEventManager::getInstance().fire( "SearchNarcoticsSuppliersForOrderOverView" ); // alle unsere Lieferanten zur Auswahl in Cb!
			setCursor( Qt::ArrowCursor );
		}
		//transactiontype z
		else if ( i == 2 )
		{
			// die eine für Vernichtung vorgesehene IDF in Cb anzeigen!
			setCursor( Qt::WaitCursor );
			basar::appl::SystemEventManager::getInstance().fire( "FindBranchData" );
			setCursor( Qt::ArrowCursor );
		}
	}
		
	m_orderHeadDataChanged = true;
	enableSaveOrderButton();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	bool is always return at the time.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool OrderOverviewVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matOrderData.reset();
	m_matNarcoticBookings.reset();
	m_matNarcoticData.reset();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onType_changed( int index )
{
	if( index == INDEX_SELLOFF )
	{
		setLineCodeToSellOf();
		cboLineCode->setEnabled( true );
	}
	else if( index == INDEX_SUPPL_RETURN )
	{
		setLineCodeToSupplReturn();
		cboLineCode->setEnabled( true );
	}
	else
	{
		setCursor( Qt::WaitCursor );
		cboLineCode->setEnabled( false );
		setLineCodeToSelectedTransactionType(index);
		setCursor( Qt::ArrowCursor );
	}

	setDisabled_grpOrderData( true );
    //todo: re-init gui, new search necessary - better? set flag (no new search), if set, buttons don't enable
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initTableWidget();
	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchOrderBooking" );
	setCursor( Qt::ArrowCursor );
	
	// disable SaveButton again if it has already been enabled because of changes having been applied to transactiontime or 
	// customersupplierno belonging to former searches
	disableSaveOrderButton();

	btnPrintDisolveReceipt->setEnabled( false );

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		tableWidget->clearContents();
		tableWidget->setDisabled( true );
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		setDisabled_grpOrderData( true );
	}
	else
	{	
		onTransactiontype_changed( 0 );
		// backup customersupplierno for rollback actions
		m_BackupNo = customersupplierno->text();

		if( type->currentIndex() == INDEX_DESTRUCTION	)
			btnPrintDisolveReceipt->setEnabled( true );
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnEdit_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnEdit_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::SystemEventManager::getInstance().fire( "UsedBooking" ); 

	//Search ArticleNarcoticDataForTransaction
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchArticleNarcoticDataForTransaction" ); 
	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		// if no article and narcotic group specific data has been found the corresponding frames in DetailVC have to be disabled
		m_RecordEditVC->setDisabled_grpNarcoticData( true );
		m_RecordEditVC->setDisabled_grpArticleData( true );

		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	m_DetailVCShown = true;
	m_RecordEditVC->matchToBookingData( m_matNarcoticBookings.getCurrentRight() );

	// Gui Field packageunit of m_RecordEditVC has to be rematched afterwards as it doesn't contain the correct value but
	// the value of articlepackageunit
	m_RecordEditVC->matchToPackageunit( m_matNarcoticData.getCurrentRight() );

	m_RecordEditVC->show();
	m_DetailVCShown = false;

	//refresh anyway - eliminates phantom values in m_RecordEditVC (edit e.g. quantity resulting in neg. stock and canceling save, next edit of record will have old, unsaved quantity)
	initTableWidget();

	onBtnSearch_clicked();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnPrintDisolveReceipt_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnPrintDisolveReceipt_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret;
	ret = basar::appl::SystemEventManager::getInstance().fire( "PrintDissolveReceipt" );
	
	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnCancelFullOrder_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnCancelFullOrder_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret;

	if( basar::gui::tie::questionMsgBox( this, 
		tr("Do you really want to cancel this order?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		ret = basar::appl::SystemEventManager::getInstance().fire( "CancelFullOrder" );
		setCursor( Qt::ArrowCursor );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	
	if( ret.ret == basar::appl::HANDLER_OK )
		initGuiCtrls();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnCancelOrderPositions_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnCancelOrderPositions_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::appl::EventReturnStruct ret;

	if( basar::gui::tie::questionMsgBox( this, 
		tr("Do you really want to cancel all selected bookings?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		ret = basar::appl::SystemEventManager::getInstance().fire( "CancelBookings" );
		setCursor( Qt::ArrowCursor );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	if( ret.ret == basar::appl::HANDLER_OK ){
		ret = basar::appl::SystemEventManager::getInstance().fire( "SearchOrderBooking" );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnDelete_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnDelete_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	basar::appl::EventReturnStruct ret;

	if( basar::gui::tie::questionMsgBox( this, 
		tr("Do you really want to delete this booking?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		ret = basar::appl::SystemEventManager::getInstance().fire( "DeleteBooking" );
		setCursor( Qt::ArrowCursor );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onBtnSaveOrderChanges_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSaveOrderChanges_clicked of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SaveOrderChanges" );
	setCursor( Qt::ArrowCursor );
	bool saved = true;

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		saved = false;
	}
	else if( ret.ret == basar::appl::HANDLER_ERROR )
	{
		basar::gui::tie::warningMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		saved = false;
	}

	if( saved )
	{
		setDisabled_grpOrderData( true );
		disableSaveOrderButton();
		m_BackupNo = customersupplierno->text();
	}
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::selectTransactionTypeInCboLineCode( basar::VarString lineCode )
{
	//as multicombobox provides no rowcount property
	if( !m_lineCodes.empty() )
	{
		std::list<basar::VarString>::iterator it = m_lineCodes.begin();
		for( basar::Int32 i = 0; it != m_lineCodes.end(); i++, it++ )
		{
			if( *it == lineCode )
			{
				cboLineCode->selectRow(i);
				cboLineCode->showRow(i);
				cboLineCode->setCurrentIndex(i);
				return;
			}			
		}
	}
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::matchToCustomerSuppliers( basar::db::aspect::AccessorPropertyTableRef supplierData )
{	
	cboBgaNo->clearContext();
	
	if( !supplierData.isNull() && !supplierData.empty() )
	{
		std::list<basar::VarString> originalAttributes;
		//that crap changes all the naming from dm to dm ->
		//workaround, because changing all the namin is to dangerous(will crash only during runtime) no time for testing the whole application
		originalAttributes.push_back( constants::NARCOTICSSUPPLIERNO );
		originalAttributes.push_back( constants::SUPPLIERNAME );
		originalAttributes.push_back( constants::SUPPLIERLOCATION );

		m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cboBgaNo, supplierData );	
		m_matSupplier.transformProperties( cboBgaNo->horizontalHeaderLabels(), originalAttributes );
		m_matSupplier.RightToLeft();

		cboBgaNo->resizeColumnsToContents();

		// select customersupplier of current order !!!
		if ( INDEX_SUPPL_RETURN == type->currentIndex() || 
			 INDEX_GI			== type->currentIndex() )
		{
			cboBgaNo->setCurrentIndex( cboBgaNo->findText( customersupplierno->text() ) );
		}
	}
}

//-----------------------------------------------------------------------------------------------//
BgaNoData OrderOverviewVC::matchFromCustomerSuppliers()
{
	BgaNoData bgaNoData;

	if( m_orderHeadDataChanged )
	{
		bgaNoData.bgaNo		= cboBgaNo->itemText( cboBgaNo->currentIndex(),0 ).toInt();
		bgaNoData.name		= cboBgaNo->itemText( cboBgaNo->currentIndex(),1 ).toStdString().c_str();
		bgaNoData.location	= cboBgaNo->itemText( cboBgaNo->currentIndex(),2 ).toStdString().c_str();
		bgaNoData.orderType = getCurrentLineCode();
	}

	return bgaNoData;
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::matchToCustomerSuppliers( basar::db::aspect::AccessorPropertyTable_YIterator supplierData )
{	
	using namespace basar::db::aspect;

	cboBgaNo->clearContext();

	m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cboBgaNo, supplierData.getPropertyTable() );
	m_matSupplier.transformProperties( cboBgaNo->horizontalHeaderLabels(), m_BgaOriginalAttributeNames );
	m_matSupplier.RightToLeft( 0, 1 );

	cboBgaNo->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToOrderData( basar::db::aspect::AccessorPropertyTable_YIterator orderDataIt )
{
	using namespace basar::db::aspect;

	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToOrderData of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setDisabled_grpOrderData( false );
	m_matOrderData.RightToLeft();
	
	matchToCustomerSuppliers( orderDataIt );

	if( orderDataIt.contains( constants::TRANSACTIONTYPE ) )
	{
		basar::VarString transactionType = orderDataIt.getString( constants::TRANSACTIONTYPE );
		selectTransactionTypeInCboLineCode( transactionType );
	}

	m_CurrentAckStatus = orderDataIt.getInt16(constants::ACKNOWLEDGEMENTSTATUS);
	

// the matching of date and time fields has to be done manually as ssuk 01.05.01 doesn't support automatical
// matching into and out of qdateedit and qtimeedit fields
	transactiontime->blockSignals( true );
	transactiontime->setTime( QTime(int( orderDataIt.getInt32(constants::TRANSACTIONTIME)/10000), 
									int( ( orderDataIt.getInt32(constants::TRANSACTIONTIME)%10000) / 100), 
									int( orderDataIt.getInt32(constants::TRANSACTIONTIME)%100 ) ) );
	transactiontime->blockSignals(false);


	transactiondate->setDate( QDate( int( orderDataIt.getInt32(constants::TRANSACTIONDATE)/10000 ), 
										int( ( orderDataIt.getInt32(constants::TRANSACTIONDATE)%10000) / 100 ), 
										int( orderDataIt.getInt32(constants::TRANSACTIONDATE)%100 ) ) );

	transactiondate->setDisplayFormat( "dd.MM.yyyy" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchFromOrderData( basar::db::aspect::AccessorPropertyTable_YIterator orderDataIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromOrderData of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::Int32 newTransactionDate = this->matchFromTransactionDate();
    basar::Int32 newTransactionTime = this->matchFromTransactionTime();

	AccessorPropertyTable_YIterator customerSupplierData;
	
	basar::Int32	 customerSupplierNo = 0;
	basar::VarString customerSupplierName;
	basar::VarString customerSupplierLocation;
	basar::VarString customerSupplierStreet;
	basar::VarString customerSupplierCip;
	basar::VarString transactionType;

	transactionType = getCurrentLineCode();

	if( m_orderHeadDataChanged )
	{
		customerSupplierData = m_matSupplier.getCurrentRight();

		if(  customerSupplierData.isNull()												|| 
			!customerSupplierData.isContainedAndSet( constants::NARCOTICSSUPPLIERNO )	||
			!customerSupplierData.isContainedAndSet( constants::SUPPLIERNAME )			||
			!customerSupplierData.isContainedAndSet( constants::SUPPLIERLOCATION )		||
			!customerSupplierData.isContainedAndSet( constants::POSTCODE ) )
		{
			if ( customerSupplierData.isNull()								||
				!customerSupplierData.isContainedAndSet(  constants::BGANO )	)
			{
				customerSupplierData = m_matOrderData.getCurrentRight();

				customerSupplierNo		 = customerSupplierData.getInt32(  constants::CUSTOMERSUPPLIERNO );
				customerSupplierName	 = customerSupplierData.getString( constants::CUSTOMER_SUPPLIER_NAME );
				customerSupplierLocation = customerSupplierData.getString( constants::CUSTOMER_SUPPLIER_LOCATION );

				if( customerSupplierData.isContainedAndSet( constants::CUSTOMER_SUPPLIER_STREET ) ){
					customerSupplierStreet = customerSupplierData.getString( constants::CUSTOMER_SUPPLIER_STREET );
				}

				if (customerSupplierData.isContainedAndSet(constants::CUSTOMER_SUPPLIER_CIP)) 
				{
					customerSupplierCip = customerSupplierData.getString(constants::CUSTOMER_SUPPLIER_CIP);
				}
			}
			else
			{
				customerSupplierNo		 = customerSupplierData.getInt32(  constants::BGANO );
				customerSupplierName	 = customerSupplierData.getString( constants::COMPANYNAME );
				customerSupplierLocation = customerSupplierData.getString( constants::LOCATION );

				if( customerSupplierData.isContainedAndSet( constants::STREET ) ){
					customerSupplierStreet = customerSupplierData.getString( constants::STREET );
				}

				if (customerSupplierData.isContainedAndSet(constants::POSTCODE)) 
				{
					customerSupplierCip = customerSupplierData.getString(constants::POSTCODE);
				}
			}
		}
		else
		{
			customerSupplierNo		 = customerSupplierData.getInt32(  constants::NARCOTICSSUPPLIERNO	);
			customerSupplierName	 = customerSupplierData.getString( constants::SUPPLIERNAME			);
			customerSupplierLocation = customerSupplierData.getString( constants::SUPPLIERLOCATION		);

            if( customerSupplierData.isContainedAndSet( constants::STREET ) ){
				customerSupplierStreet = customerSupplierData.getString( constants::STREET );
			}

			if (customerSupplierData.isContainedAndSet( constants::POSTCODE ) ) 
			{
				customerSupplierCip = customerSupplierData.getString( constants::POSTCODE );
			}
		}
	}

	while( !orderDataIt.isEnd() )
	{
		basar::Int32 originalTransactionDate = orderDataIt.getInt32( constants::TRANSACTIONDATE );
        basar::Int32 originalTransactionTime = orderDataIt.getInt32( constants::TRANSACTIONTIME );

		m_matOrderData.LeftToRight( m_matOrderData.getLeft().begin(), orderDataIt );

// the matching of transactiontime has to be done manually as ssuk 01.05.01 doesn't support automatical
// matching into and out of qdateedit and qtimeedit fields
		orderDataIt.setInt32( constants::TRANSACTIONTIME, this->matchFromTransactionTime() );		

		if( m_orderHeadDataChanged )
		{
			orderDataIt.setInt32(  constants::CUSTOMERSUPPLIERNO,			customerSupplierNo		 );
			orderDataIt.setString( constants::CUSTOMER_SUPPLIER_NAME,		customerSupplierName	 );
			orderDataIt.setString( constants::CUSTOMER_SUPPLIER_LOCATION,	customerSupplierLocation );
			orderDataIt.setString( constants::CUSTOMER_SUPPLIER_CIP,        customerSupplierCip      );
			orderDataIt.setString( constants::NEWTRANSACTIONTYPE,			transactionType			 );

			if( customerSupplierData.isContainedAndSet( constants::STREET ) ){
				orderDataIt.setString( constants::CUSTOMER_SUPPLIER_STREET, customerSupplierStreet );
			}
		}

		if( newTransactionDate != originalTransactionDate )
		{
			orderDataIt.setInt32( constants::NEWTRANSACTIONDATE, newTransactionDate );
			orderDataIt.setInt32( constants::TRANSACTIONDATE, originalTransactionDate );
		}

        if( newTransactionTime != originalTransactionTime )
        {
            orderDataIt.setInt32( constants::NEWTRANSACTIONTIME, newTransactionTime );
			orderDataIt.setInt32( constants::TRANSACTIONTIME, originalTransactionTime );           
        }
		
		orderDataIt++;
	}

// at this point transactiondate doesn't have to be matched manually as it can never be changed 
// and therefore must have the same value in gui and in corresponding propertyTable
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToNarcoticBookings( basar::Int32 tableWidgetRows )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticBookings of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString s;
	for( int i = 0; i < tableWidget->columnCount(); i++ )
	{
		s = "";
		s = tableWidget->horizontalHeaderItem( i )->text().toLocal8Bit().data();
	}

	if( false == m_DetailVCShown )
	{
		setDisabled_grpNarcoticsBookingsData( false );
		initTableWidget();
		tableWidget->setRowCount( tableWidgetRows );
		m_matNarcoticBookings.RightToLeft();
		tableWidget->resizeColumnsToContents();
		tableWidget->selectRow(0);
		tableWidget->setFocus();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchFromNarcoticBookings()
{
	m_matNarcoticBookings.LeftToRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	searchIt represents the AccessorPropertyTable_YIterator which search values are set to.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSelection of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( !orderNoSearch->text().isEmpty() ){
		searchIt.setString( constants::PURCHASEORDERNO,	orderNoSearch->text().toLocal8Bit().constData()	);
	}
	searchIt.setString( constants::TRANSACTIONDATE,	dtSearch->date().toString("yyyyMMdd").toLocal8Bit().constData() );
	searchIt.setString( constants::FROMDATE, dtSearch->date().toString( "yyyyMMdd" ).toLocal8Bit().constData() );
	searchIt.setString( constants::TODATE,	 dtSearch->date().toString( "yyyyMMdd" ).toLocal8Bit().constData()   );

	basar::I18nString transactiontype = type->currentText().toLocal8Bit().constData();

	if( transactiontype == tr("Sale").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::CUSTOMER_ORDER );
	}

	if( transactiontype == tr("Received Goods").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::GOODSIN_DELIVERY );
	}

	if( transactiontype == tr("Customer Returns").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::CUSTOMER_RETURNS );
	}

	if( transactiontype == tr("Supplier Returns").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::SUPPLIER_RETURNS );
	}

	if( transactiontype == tr("Destruction").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::DESTRUCTION );
	}



}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentBooking is the AccessorPropertyTable_YIterator which is assigned dialog values to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchFromCurrentBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromCurrentBooking of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchFromCurrentBooking( currentBooking );
	} 
	else{
		m_matNarcoticBookings.LeftToRight( m_matNarcoticBookings.getCurrentLeft(), currentBooking );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	basar::Int32
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::Int32 OrderOverviewVC::matchFromCustomerSupplierNo()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromCustomerSupplierNo of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return atoi( customersupplierno->text().toLocal8Bit().constData() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	basar::Int32
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::Int32 OrderOverviewVC::matchFromTransactionTime()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromTransactionTime of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::Int32 newTransactionTime =	transactiontime->time().hour()	* 10000	+
										transactiontime->time().minute()* 100	+
										transactiontime->time().second();
	return newTransactionTime;
}

//-----------------------------------------------------------------------------------------------//
basar::Int32 OrderOverviewVC::matchFromTransactionDate()
{	
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromTransactionDate of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::Int32 newTransactionDate =	transactiondate->date().year()  * 10000	+
										transactiondate->date().month() * 100	+
										transactiondate->date().day();
	
	return newTransactionDate;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	AccessorPropertyTable_YIterator contains new values for location and name of the customer whose customersupplierno has been changed in the dialog.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToCustomerData( AccessorPropertyTable_YIterator customerData )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCustomerData of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if(  ( customerData.contains( constants::CUSTOMERNO ) ) && 
		!( customerData.getState( constants::CUSTOMERNO ) == basar::SS_UNSET ) )
	{
		basar::I18nString customerNoValue;
		customerNoValue.itos( customerData.getInt32( constants::CUSTOMERNO ) );
		customersupplierno->setText( QString::fromLocal8Bit(customerNoValue.c_str()) );
	}

	if(  ( customerData.contains( constants::CUSTOMERNAME ) ) &&	
		!( customerData.getState( constants::CUSTOMERNAME ) == basar::SS_UNSET ) )
	{
		customersuppliername->setText( QString::fromLocal8Bit(customerData.getString( constants::CUSTOMERNAME ).c_str()) );
	}

	if(  ( customerData.contains( constants::CUSTOMERLOCATION ) ) &&	
		!( customerData.getState( constants::CUSTOMERLOCATION ) == basar::SS_UNSET ) )
	{
		customersupplierlocation->setText( QString::fromLocal8Bit(customerData.getString( constants::CUSTOMERLOCATION ).c_str()) );
	}

	if ((customerData.contains(constants::CUSTOMERLOCATION)) &&
		!(customerData.getState(constants::CUSTOMERLOCATION) == basar::SS_UNSET))
	{
		customersuppliercip->setText(QString::fromLocal8Bit(customerData.getString(constants::CUSTOMERLOCATION).c_str()));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	AccessorPropertyTable_YIterator contains new values for location and name of the supplier whose customersupplierno has been changed in the dialog.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToSupplierData( AccessorPropertyTable_YIterator supplierData )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToSupplierData of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if(  ( supplierData.contains( constants::NARCOTICSSUPPLIERNO ) ) &&	
		!( supplierData.getState( constants::NARCOTICSSUPPLIERNO ) == basar::SS_UNSET ) )
	{
		basar::I18nString supplierNoValue;
		supplierNoValue.itos( supplierData.getInt32( constants::NARCOTICSSUPPLIERNO ) );
		customersupplierno->setText( QString::fromLocal8Bit(supplierNoValue.c_str()) );
	}

	if(  ( supplierData.contains( constants::SUPPLIERNAME ) ) &&	
		!( supplierData.getState( constants::SUPPLIERNAME ) == basar::SS_UNSET ) )
	{
		customersuppliername->setText( QString::fromLocal8Bit(supplierData.getString( constants::SUPPLIERNAME ).c_str()) );
	}

	if(  ( supplierData.contains( constants::SUPPLIERLOCATION ) ) &&	
		!( supplierData.getState( constants::SUPPLIERLOCATION ) == basar::SS_UNSET ) )
	{
		customersupplierlocation->setText( QString::fromLocal8Bit(supplierData.getString(constants::SUPPLIERLOCATION).c_str()) );
	}

	if ((supplierData.contains(constants::SUPPLIERCIP)) &&
		!(supplierData.getState(constants::SUPPLIERCIP) == basar::SS_UNSET))
	{
		customersuppliercip->setText(QString::fromLocal8Bit(supplierData.getString(constants::SUPPLIERCIP).c_str()));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToBranch( basar::db::aspect::AccessorPropertyTable_YIterator branch )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToBranch of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	if( m_DetailVCShown ){
		m_RecordEditVC->matchToBranch( branch );
	}
	else
	{
		cboBgaNo->clearContext();

		std::list<basar::VarString> originalAttributes;
		//that crap changes all the naming from dm to dm ->
		//workaround, because changing all the namin is to dangerous(will crash only during runtime) no time for testing the whole application
		originalAttributes.push_back( constants::BGANO );
		originalAttributes.push_back( constants::COMPANYNAME );
		originalAttributes.push_back( constants::LOCATION );

		m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cboBgaNo, branch.getPropertyTable() );	
		m_matSupplier.transformProperties( cboBgaNo->horizontalHeaderLabels(), originalAttributes );
		m_matSupplier.RightToLeft(0,1);

		cboBgaNo->resizeColumnsToContents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator OrderOverviewVC::getCurrentAccessorIterator()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getCurrentAccessorIterator of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matNarcoticBookings.getCurrentRight(); //getSelectedRight()
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onTableWidget_itemSelectionChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTableWidget_itemSelectionChanged of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	// enabling of buttons "delete" and "empty" as an item of the tableWidget is now selected
	
	btnEdit->setDisabled(	tableWidget->selectedRanges().isEmpty() || m_matNarcoticBookings.getCurrentRight().isEnd() || 
							m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED );
	btnDelete->setDisabled( tableWidget->selectedRanges().isEmpty() || m_matNarcoticBookings.getCurrentRight().isEnd() || 
							m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED);

	btnCancelFullOrder->setDisabled(	tableWidget->selectedRanges().isEmpty() || m_matNarcoticBookings.getCurrentRight().isEnd() || 
										(m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) != TransactionType::CUSTOMER_ORDER &&
										 m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) != TransactionType::SUPPLIER_RETURNS) ||
										m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED);
	btnCancelOrderPositions->setDisabled(	tableWidget->selectedRanges().isEmpty() || m_matNarcoticBookings.getCurrentRight().isEnd() || 
											(m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) != TransactionType::CUSTOMER_ORDER &&
											 m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) != TransactionType::SUPPLIER_RETURNS) ||
											m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onCustomersupplierno_editingFinished()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_customersupplierno_editingFinished of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// Search customersuppliername and customersupplierlocation for changed customersupplierno
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchNameAndLocation" ); 

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		// rollback of customersupplierno
		customersupplierno->setText( m_BackupNo );
		// show message that no entry has been found
		if( ret.message != "" ){
			basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		}
	} 
	else{
		// Enable SaveOrderChanges-Button for saving the changes made to order specific data
		enableSaveOrderButton();
	}
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::onTransactiondate_changed( const QDate& newTransactionDate )
{
	newTransactionDate;

	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "checkTimeChanges of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveOrderButton();
}

//-----------------------------------------------------------------------------------------------//
void OrderOverviewVC::onTransactiontype_changed( int index )
{	
	index;
	basar::I18nString transactiontype = type->currentText().toLocal8Bit().constData();

	if( transactiontype == tr("Sale").toLocal8Bit().constData() ||
		transactiontype == tr("Supplier Returns").toLocal8Bit().constData() ||
		transactiontype == tr("Destruction").toLocal8Bit().constData()		){
		transactiondate->setEnabled( false );
	}

	if( transactiontype == tr("Received Goods").toLocal8Bit().constData() ||
		transactiontype == tr("Customer Returns").toLocal8Bit().constData()	){
		transactiondate->setEnabled( true );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::onTransactiontime_changed()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "checkTimeChanges of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

// check if changes to transactiontime really have been applied
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "CheckTimeChanges" ); 

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		if( ret.message != "" ){
			basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		}
	} 
	else{
		// Enable SaveOrderChanges-Button for saving the changes made to order specific data
		enableSaveOrderButton();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\Enable SaveOrderChanges-Button for saving the changes made to oder specific data
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::enableSaveOrderButton()
{
	if (  m_CurrentAckStatus != constants::DELIVERY_NOTE_COPY_CREATED )	
	{
		m_orderHeadDataChanged = true;
		btnSaveOrderChanges->setEnabled( true );
	}
	else
		btnSaveOrderChanges->setEnabled( false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\Disable SaveOrderChanges-Button 
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::disableSaveOrderButton()
{
	btnSaveOrderChanges->setDisabled( true );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\clear and disable table widget
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::clearTableWidget()
{
	tableWidget->clearContents();
	tableWidget->setDisabled( true );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCustomer of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToCustomer( customer );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	supplier PropTab
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToSuppliers( basar::db::aspect::AccessorPropertyTableRef aptNarcSupp )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCboNarcoticsSupplier of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToSuppliers( aptNarcSupp );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	supplier PropTab
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewVC::matchToArticleNarcoticData( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleNarcoticData of OrderOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_RecordEditVC->matchToArticleNarcoticData( yit );
}

//-------------------------------------------------------------------------------------------------//
bool OrderOverviewVC::eventFilter( QObject *obj, QEvent *event )
{
	if (event->type() != QEvent::KeyPress)
		return false;

	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

	switch (keyEvent->key())
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
		{
			if (obj == orderNoSearch || obj == dtSearch || obj == type || obj == btnSearch)
			{
				onBtnSearch_clicked();
				return true;
			}
		}
	default:
		break;
	}
	return false;
}

} //namespace viewConn
} //namespace narcotics
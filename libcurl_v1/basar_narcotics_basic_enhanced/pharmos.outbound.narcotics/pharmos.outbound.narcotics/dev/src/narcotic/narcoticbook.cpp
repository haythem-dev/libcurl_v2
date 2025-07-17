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
#include "valuerowcolortransformer.hpp"
#include "narcoticbook.h"
#include "loggerpool.h"
#include "narcoticsappl.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

//-----------------------------------------------------------------------------------------------//
// class definition
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
// !\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticBookVC::NarcoticBookVC( QWidget *parent )
: QDialog( parent ), m_DetailVCShown( false ), m_DisplayStatus( NORMAL )
{
	setupUi(this);
	initGuiCtrls();
	wireEvents();
	createLayout();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticBookVC::~NarcoticBookVC()
{
	if (m_RecordEditVC.get() != NULL)
	{
		m_RecordEditVC->shutdown();
		m_RecordEditVC.reset();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticBookVC::isNewProcess()
{
	return narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess();	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::wireEvents()
{
	QObject::connect(	btnPrintList,			SIGNAL( clicked()								),
						this,					SLOT  (	onBtnPrintList_clicked()				));
	QObject::connect(	btnPrintDeliveryReceipt,SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnPrintDeliveryReceipt_clicked()		));	
	QObject::connect(	btnPrintDissolveReceipt,SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnPrintDissolveReceipt_clicked()		));	
	QObject::connect(	btnRestore,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnRestore_clicked()					));	
	QObject::connect(	btnCreate,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnCreate_clicked()					));	
	QObject::connect(	btnDelete,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnDelete_clicked()					));	
	QObject::connect(	btnCancellation,		SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnCancellation_clicked()				));	
	QObject::connect(	btnEdit,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnEdit_clicked()						));	
	QObject::connect(	tableWidget,			SIGNAL( itemSelectionChanged()					), 
						this,					SLOT  (	onTableWidget_itemSelectionChanged()	));	
	QObject::connect(	btnSearchInternalAccounts, SIGNAL( clicked()							), 
						this,					SLOT  (	onBtnSearchInternalAccounts_clicked()	));	
	QObject::connect(	btnSearchIncomplete,	SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnSearchIncomplete_clicked()			));	
	QObject::connect(	btnSearchDoubleBookings,SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnSearchDoubleBookings_clicked()		));	
	QObject::connect(	txtArticleNoSearch,		SIGNAL( textChanged( QString )					), 
						this,					SLOT  (	onTxtArticleNoSearch_textChanged( QString ) ));	
	QObject::connect(	txtArticleNameSearch,	SIGNAL( textChanged( QString )					), 
						this,					SLOT  (	onTxtArticleNameSearch_textChanged( QString ) ));	
	QObject::connect(	btnSearch,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnSearch_clicked()					));	
	QObject::connect(	btnClose,				SIGNAL( clicked()								), 
						this,					SLOT  (	onBtnClose_clicked()					));	
	QObject::connect(	tableWidget,			SIGNAL( itemPressed( QTableWidgetItem* )			),
						this,					SLOT  (	onTableWidget_itemPressed( QTableWidgetItem* ) ));	
	QObject::connect(	optArticleNo,			SIGNAL( toggled( bool )							), 
						this,					SLOT  (	buttonGroupStateChanged()				));	
	QObject::connect(	optArticleName,			SIGNAL( toggled( bool )							), 
						this,					SLOT  (	buttonGroupStateChanged()				));	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::createLayout()
{
	QGridLayout* searchLayout = new QGridLayout(grpSearchData);
	searchLayout->addWidget(optArticleNo,			0, 0);
	searchLayout->addWidget(optArticleName,			1, 0);
	searchLayout->addWidget(txtArticleNoSearch,		0, 1);
	searchLayout->addWidget(txtArticleNameSearch,	1, 1);
	searchLayout->addWidget(lblFromSearch,			0, 2, 1, 1, Qt::AlignRight);
	searchLayout->addWidget(dtFromSearch,			0, 3);
	searchLayout->addWidget(lblToSearch,			0, 4, 1, 1, Qt::AlignRight);
	searchLayout->addWidget(dtToSearch,				0, 5);
	searchLayout->addWidget(btnSearch,				0, 6, 1, 1, Qt::AlignRight);

	QHBoxLayout* narcGroupLayout = new QHBoxLayout(grpNarcoticData);
	narcGroupLayout->addWidget(lblBtmGroup);
	narcGroupLayout->addWidget(group);
	narcGroupLayout->addWidget(lblNarcoticGroupName);
	narcGroupLayout->addWidget(name);
	narcGroupLayout->addWidget(lblNarcoticGroupPackageUnit);
	narcGroupLayout->addWidget(packageunit);
	narcGroupLayout->addWidget(lblConcentration);
	narcGroupLayout->addWidget(concentration);

	QGridLayout* articleDataLayout = new QGridLayout(grpArticleData);
	articleDataLayout->addWidget(lblArticleNo,				0, 0);
	articleDataLayout->addWidget(articlecode,				0, 1);
	articleDataLayout->addWidget(lblArticleName,			0, 2);
	articleDataLayout->addWidget(articlename,				0, 3, 1, 5);
	articleDataLayout->addWidget(lblArticlePackageUnit,		1, 0);
	articleDataLayout->addWidget(articlepackageunit,		1, 1);

	articleDataLayout->addWidget(lblPharmaForm,				1, 2);
	articleDataLayout->addWidget(articlepharmaform,			1, 3);

	articleDataLayout->addWidget(lblStorageLocation,		1, 4);
	articleDataLayout->addWidget(articlestocklocation,		1, 5);
	
	articleDataLayout->addWidget(lblInitialInventory,		1, 6);
	articleDataLayout->addWidget(qtyinstock,				1, 7);

	QVBoxLayout* hiddenLayout = new QVBoxLayout(fraArticleNarcoticData);
	hiddenLayout->setContentsMargins(0, 0, 0, 0);
	hiddenLayout->addWidget(grpNarcoticData);
	hiddenLayout->addWidget(grpArticleData);

	//! \todo FN Layout grpNarcoticBookingsData
	QHBoxLayout* listButtonLayout = new QHBoxLayout();
	listButtonLayout->addWidget(btnCreate);
	listButtonLayout->addWidget(btnEdit);
	listButtonLayout->addWidget(btnDelete);
	listButtonLayout->addStretch();
	listButtonLayout->addWidget(btnCancellation);
	listButtonLayout->addStretch();
	listButtonLayout->addWidget(btnPrintDissolveReceipt);
	listButtonLayout->addStretch();
	listButtonLayout->addWidget(btnPrintDeliveryReceipt, 0, Qt::AlignRight);

	QVBoxLayout* listLayout = new QVBoxLayout(grpNarcoticBookingsData);
	listLayout->addWidget(tableWidget);
	listLayout->addLayout(listButtonLayout);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(btnSearchInternalAccounts);
	buttonLayout->addWidget(btnSearchIncomplete);
	buttonLayout->addWidget(btnSearchDoubleBookings);
	buttonLayout->addWidget(btnPrintList);
	buttonLayout->addWidget(btnRestore);
	buttonLayout->addStretch();
	buttonLayout->addWidget(btnClose);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(grpSearchData);
	mainLayout->addWidget(fraArticleNarcoticData);
	mainLayout->addWidget(grpNarcoticBookingsData);
	mainLayout->addLayout(buttonLayout);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	withSearchCriteria regulates if the dialog is initialized with or without the search criterias.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::initGuiCtrls()
{
	//--------------------------------------------------------//
	//	prepare the search data frame with default values
	//	'grpSearchData'
	//--------------------------------------------------------//
	if ( 0 == txtArticleNoSearch->validator() ){
		txtArticleNoSearch->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,8}" ), this ) );
	}
	txtArticleNoSearch->setText("");
	txtArticleNoSearch->setDisabled( true );
	txtArticleNameSearch->setText("");
	txtArticleNameSearch->setDisabled( true );

	dtFromSearch->setDate( QDate::currentDate() );
	dtFromSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtFromSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	dtToSearch->setDate( QDate::currentDate() );
	dtToSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtToSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	btnSearch->setDisabled( true );

	btnCancellation->setEnabled( isNewProcess() && btnDelete->isEnabled() );	
		
	//--------------------------------------------------------//
	// invisible QFrame 'fraArticleNarcoticData'
	//--------------------------------------------------------//
	QColor color( "red" ); 
	QPalette palette = group->palette();
	palette.setColor( QPalette::Text, color );
	group->setPalette( palette );
	group->setText( "" );

	palette = name->palette();
	palette.setColor( QPalette::Text, color );
	name->setPalette( palette );
	name->setText( "" );

	palette = packageunit->palette();
	palette.setColor( QPalette::Text, color );
	packageunit->setPalette( palette );
	packageunit->setText( "" );

	palette = concentration->palette();
	palette.setColor( QPalette::Text, color );
	concentration->setPalette( palette );
	concentration->setText( "" );

	///////////////////////////////////////////////////////////
	// disable the article specific data frame
	///////////////////////////////////////////////////////////
	color.setNamedColor( "black" );

	palette = articlecode->palette();
	palette.setColor( QPalette::Text, color );
	articlecode->setPalette( palette );
	articlecode->setText("");

	palette = articlepackageunit->palette();
	palette.setColor( QPalette::Text, color );
	articlepackageunit->setPalette( palette );
	articlepackageunit->setText("");

	palette = articlename->palette();
	palette.setColor( QPalette::Text, color );
	articlename->setPalette( palette );
	articlename->setText("");

	palette = articlepharmaform->palette();
	palette.setColor( QPalette::Text, color );
	articlepharmaform->setPalette( palette );
	articlepharmaform->setText("");

	palette = articlestocklocation->palette();
	palette.setColor( QPalette::Text, color );
	articlestocklocation->setPalette( palette );
	articlestocklocation->setText("");

	palette = qtyinstock->palette();
	palette.setColor( QPalette::Text, color );
	qtyinstock->setPalette( palette );
	qtyinstock->setText("");

	//--------------------------------------------------------//
	//	disable the narcotic bookings specific data frame 
	//	'grpNarcoticBookingsData'
	//--------------------------------------------------------//
	btnCreate->setDisabled( true );
	btnEdit->setDisabled( true );
	btnDelete->setDisabled( true );
	btnCancellation->setEnabled( isNewProcess() && btnDelete->isEnabled() );
	btnPrintDissolveReceipt->setDisabled( true );
	btnPrintDeliveryReceipt->setDisabled( true );

	// REFACTORING remove code behind these buttons
	btnPrintDissolveReceipt->setVisible( false );
	btnPrintDeliveryReceipt->setVisible( false );
	btnSearchDoubleBookings->setVisible( false );
	// CR-14D140 nach Umsetzung "Stornieren-Bt" entfernen: btnCancellation->setVisible( false );
	
	//--------------------------------------------------------//
	// initialization QTableWidget
	//--------------------------------------------------------//
	initTableWidget();

	//--------------------------------------------------------//
	// init of original attributes for QTableWidget headings
	//--------------------------------------------------------//
	if( m_OriginalAttributeNames.empty() )
	{
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONDATE );
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONTIME );
		m_OriginalAttributeNames.push_back( constants::CUSTOMERSUPPLIERNO );
		m_OriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_NAME );
		m_OriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_STREET);
		m_OriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_CIP);
		m_OriginalAttributeNames.push_back( constants::CUSTOMER_SUPPLIER_LOCATION );
		m_OriginalAttributeNames.push_back( constants::TRANSACTIONTYPE );
		m_OriginalAttributeNames.push_back( constants::PURCHASEORDERNO );
		m_OriginalAttributeNames.push_back( constants::QTYTRANSACTION );
		m_OriginalAttributeNames.push_back( constants::QTYINSTOCK );
		m_OriginalAttributeNames.push_back( constants::CONTRIBUTIONVOUCHERNO );
		m_OriginalAttributeNames.push_back( constants::ACKNOWLEDGEMENTSTATUS );
	}

	//--------------------------------------------------------//
	// dialog buttons
	//--------------------------------------------------------//
	btnSearchInternalAccounts->setDisabled( false );
	btnRestore->setDisabled( false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::initTableWidget()
{
	QStringList lstCaptions;
	lstCaptions	<< tr("Date")		<< tr("Time")		
				<< tr("Bga-No.")	<< tr("Name")
				<< tr("Street")		<< tr("CIP")
				<< tr("Location")
				<< tr("Record Type")	<< tr("Order-No.")	
				<< tr("Quantity")		<< tr("QtyInStock")
				<< tr("ContributionVoucherNo")
				<< tr("AcknowledgementStatus");
	
	tableWidget->clear();
	tableWidget->setHorizontalHeaderLabels( lstCaptions );
	tableWidget->hideVerticalHeader();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\param	articleNarcoticsData is the AccessorPropertyTableRef which will be connected 
			to the article specific frame via matcher.
//!	\param	narcoticsBookings is the AccessorPropertyTableRef which will be connected 
			to the query result of the narcotic bookings specific table widget.
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::init( basar::db::aspect::AccessorPropertyTableRef articleNarcoticsData, 
						   basar::db::aspect::AccessorPropertyTableRef narcoticsBookings, 
						   basar::db::aspect::AccessorPropertyTableRef initialInventory )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "init of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	using basar::gui::tie::Manager;
	// creation of the matcher
	m_matNarcoticArticleBook	= Manager::getInstance().createMatcher( fraArticleNarcoticData, articleNarcoticsData );
	m_matInitialInventory		= Manager::getInstance().createMatcher( fraArticleNarcoticData, initialInventory );
	m_matNarcoticBookings		= Manager::getInstance().createMatcher( grpNarcoticBookingsData, narcoticsBookings );

	m_matNarcoticBookings.transformProperties( tableWidget->horizontalHeaderLabels(), m_OriginalAttributeNames );
	
	SignTransformer signTransformer;
	m_matNarcoticBookings.push_back( signTransformer );

	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr( "Date" ).toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( dateTransformer );
	
	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr( "Time" ).toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( timeTransformer );

	AcknowledgementStatusTransformer statusTransformer;
	m_matNarcoticBookings.push_back(statusTransformer);

	std::map<basar::Int16, QColor> valueColorMap;
	valueColorMap.insert(std::map<basar::Int16, QColor>::value_type((basar::Int16)constants::DELIVERY_NOTE_COPY_CREATED, QColor(Qt::red)));
	ValueRowColorTransformer<basar::Int16> rowColorTransformer(constants::ACKNOWLEDGEMENTSTATUS, valueColorMap, tableWidget);
	m_matNarcoticBookings.push_back(rowColorTransformer);

	m_DisplayStatus = NORMAL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
/*! \param	text completely passed content of the QTextEdit field 'txtArticleNoSearch'.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onTxtArticleNoSearch_textChanged( const QString& text )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onTxtArticleNoSearch_textChanged of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	btnSearch->setDisabled( text.isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
/*! \param	text completely passed content of the QTextEdit field 'txtArticleNameSearch'.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onTxtArticleNameSearch_textChanged( const QString& text )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onTxtArticleNameSearch_textChanged of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	btnSearch->setDisabled( text.isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	int returns the result (DialogCode) of a modally shown dialog
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum NarcoticBookVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "show of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	bool is always return at the time.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticBookVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "shutdown of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	m_matNarcoticArticleBook.reset();
	m_matInitialInventory.reset();
	m_matNarcoticBookings.reset();
	initGuiCtrls();
	close();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "hide of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	return QWidget::hide();
}

//-----------------------------------------------------------------------------------------------//
// ctrl events
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onTableWidget_itemPressed( QTableWidgetItem * )
{
	//	article specific data are not shown yet 
	//	--> it could happen if the internal accounts are shown in the dialog
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchArticleNarcoticData" );

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		btnDelete->setDisabled( true );
		btnCancellation->setDisabled( true );
		btnEdit->setDisabled( true );
		
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onTableWidget_itemSelectionChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onTableWidget_itemSelectionChanged of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	checkButtonsActivation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( btnSearch->isEnabled() == false ){
		return;
	}

	if( dtFromSearch->date() > dtToSearch->date() )
	{
		basar::gui::tie::infoMsgBox( this,
			tr( "To date must be greater than or equal to from date!" ).toLocal8Bit().constData(),
			tr( "Narcotics" ).toLocal8Bit().constData() );
		return;
	}

	initTableWidget();
	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchBookingCatalog" );
	setCursor( Qt::ArrowCursor );
	m_DisplayStatus = NORMAL;

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\throw	no-throw  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnPrintList_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnPrintList_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintBookingCatalog" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\throw	no-throw  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnPrintDeliveryReceipt_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnPrintDeliveryReceipt_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintDeliveryReceipt" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\throw	no-throw  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnPrintDissolveReceipt_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnPrintDissolveReceipt_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintDissolveReceipt" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnSearchDoubleBookings_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "NarcoticBookVC::onBtnSearchDoubleBookings_clicked() triggered.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}
    
    using namespace basar::appl;

	setCursor( Qt::WaitCursor );
	initTableWidget();
	SystemEventManager::getInstance().fire( "SearchDoubleBookingsRequested" );
	m_DisplayStatus = DOUBLE_BOOKINGS;
	setCursor( Qt::ArrowCursor );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnSearchIncomplete_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnSearchIncomplete_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	setCursor( Qt::WaitCursor );
	initTableWidget();
	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "DisplayIncompeteBookings" );
		
	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		setCursor( Qt::ArrowCursor );
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	else if( ret.ret == basar::appl::HANDLER_ERROR )
	{
		setCursor( Qt::ArrowCursor );
		basar::gui::tie::criticalMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	m_DisplayStatus = INCOMPLETE_BOOKINGS;
	setCursor( Qt::ArrowCursor );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnSearchInternalAccounts_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnSearchInternalAccounts_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	setCursor( Qt::WaitCursor );
	initTableWidget();
	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "DisplayInternalBookings" );
		
	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		setCursor( Qt::ArrowCursor );
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	else if( ret.ret == basar::appl::HANDLER_ERROR )
	{
		setCursor( Qt::ArrowCursor );
		basar::gui::tie::criticalMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	m_DisplayStatus = INTERNAL_ACCOUNTS;
	setCursor( Qt::ArrowCursor );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnClose_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnCreate_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnCreate_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	m_DetailVCShown = true;
	m_RecordEditVC->initGuiCtrls();

	m_RecordEditVC->matchToArticleNarcoticData( m_matNarcoticArticleBook.getRight().begin() );
	basar::appl::SystemEventManager::getInstance().fire( "CreateBooking" );

	m_RecordEditVC->show();
	m_DetailVCShown = false;

	//	Here it is not allowed to check against any values saved on the detail dialog, 
	//	because the method 'handleCreateBooking()' clears the AccessorPropertyTable and inserts only one new YIterator. 
	//	If the detail dialog is closed and the new search is not started old hits will displayed not 
	//	corresponding with the current (new manipulated by method 'handleCreateBooking') AccessorPropertyTable.
	//	So it is always essential to do a new search to provide the AccessorPropertyTable with the right datasets.
	initTableWidget();

	if( m_DisplayStatus == NORMAL ) {
		onBtnSearch_clicked();
	}
	else if( m_DisplayStatus == INCOMPLETE_BOOKINGS ) {
		onBtnSearchIncomplete_clicked();
	}
	else if( m_DisplayStatus == INTERNAL_ACCOUNTS ) {
		onBtnSearchInternalAccounts_clicked();
	}
	else if( m_DisplayStatus == DOUBLE_BOOKINGS ) {
		onBtnSearchDoubleBookings_clicked();
	}
}

//-----------------------------------------------------------------------------------------------//
basar::gui::tie::ViewConnPtr<CreateProcessNarcoticsBookingVC> NarcoticBookVC::getRecordEditVC()
{
	return m_RecordEditVC;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnEdit_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnEdit_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	basar::appl::SystemEventManager::getInstance().fire( "UsedBooking" ); 

	AccessorPropertyTable_YIterator yit = m_matNarcoticArticleBook.getCurrentRight();
	
	m_RecordEditVC->matchToArticleNarcoticData( yit );

	m_DetailVCShown = true;

	AccessorPropertyTable_YIterator yitCurrentNarcoticBookings = m_matNarcoticBookings.getCurrentRight();

	if( yit.isContainedAndSet( "packageunit" ) ){
		yitCurrentNarcoticBookings.setString( "packageunit",yit.getString( "packageunit" ) );
	}

	m_RecordEditVC->matchToBookingData( yitCurrentNarcoticBookings );
	m_RecordEditVC->show();
	m_DetailVCShown = false;

	if( true == m_RecordEditVC->m_DatasetSaved )
	{
		initTableWidget();
		if( m_DisplayStatus == NORMAL ){
			onBtnSearch_clicked();
		}
		else if( m_DisplayStatus == INCOMPLETE_BOOKINGS ){
			onBtnSearchIncomplete_clicked();
		}
		else if( m_DisplayStatus == INTERNAL_ACCOUNTS ){
			onBtnSearchInternalAccounts_clicked();
		}
		else if( m_DisplayStatus == DOUBLE_BOOKINGS ) {
			onBtnSearchDoubleBookings_clicked();
		}
	}
}

//-----------------------------------------------------------------------------------------------//
void NarcoticBookVC::onBtnCancellation_clicked()
{
	basar::appl::EventReturnStruct ret;

	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnCancellation_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( basar::gui::tie::questionMsgBox( this, 
		tr( "Do you really want to cancel this order? It will also be deleted!! Please note that at least one printprocess must be running!" ).toLocal8Bit().constData(),
		tr( "Narcotics" ).toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );

		ret = basar::appl::SystemEventManager::getInstance().fire( "CancelBooking" );
		
		if( !ret.isHandlerOK() ){
			basar::gui::tie::infoMsgBox( this,
				ret.message.append( tr(" Deleting will not start!!" ).toLocal8Bit().constData() ).c_str(),
				tr("Narcotics").toLocal8Bit().constData());
		}		
		else{
			ret = basar::appl::SystemEventManager::getInstance().fire( "DeleteBooking" );
		}
		
		if( !ret.isHandlerOK() ){
			basar::gui::tie::infoMsgBox( this, 
				ret.message.append( tr(" Deleting hasn't been successful!" ).toLocal8Bit().constData() ).c_str(),
				tr("Narcotics").toLocal8Bit().constData());
		}		
		else{
			ret = basar::appl::SystemEventManager::getInstance().fire( "RegisterOrderForPrinting" );
		}
		
		setCursor( Qt::ArrowCursor );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	else
	{
		// call search method to refresh table widget
		if( m_DisplayStatus == NORMAL ) {
			onBtnSearch_clicked();
		}
		else if( m_DisplayStatus == INCOMPLETE_BOOKINGS ) {
			onBtnSearchIncomplete_clicked();
		}
		else if( m_DisplayStatus == INTERNAL_ACCOUNTS ) {
			onBtnSearchInternalAccounts_clicked();
		}
		else if( m_DisplayStatus == DOUBLE_BOOKINGS ) {
			onBtnSearchDoubleBookings_clicked();
		}
	}
//	ret = basar::appl::SystemEventManager::getInstance().fire( "SearchBookingCatalog" );

//	if( ret.ret == basar::appl::HANDLER_INFO ){
//		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
//	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnDelete_clicked()
{
	basar::appl::EventReturnStruct ret;

	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnDelete_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( basar::gui::tie::questionMsgBox( this, 
		tr( "Do you really want to delete this order?" ).toLocal8Bit().constData(),
		tr( "Narcotics" ).toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		ret = basar::appl::SystemEventManager::getInstance().fire( "DeleteBooking" );
		setCursor( Qt::ArrowCursor );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	else
	{
		// call search method to refresh table widget
		if( m_DisplayStatus == NORMAL ) {
			onBtnSearch_clicked();
		}
		else if( m_DisplayStatus == INCOMPLETE_BOOKINGS ) {
			onBtnSearchIncomplete_clicked();
		}
		else if( m_DisplayStatus == INTERNAL_ACCOUNTS ) {
			onBtnSearchInternalAccounts_clicked();
		}
		else if( m_DisplayStatus == DOUBLE_BOOKINGS ) {
			onBtnSearchDoubleBookings_clicked();
		}
		//// call search method to refresh table widget
		//basar::appl::SystemEventManager::getInstance().fire( "SearchBookingCatalog" );

		//if ( ret.ret == basar::appl::HANDLER_INFO ){
		//	basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
		//}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::onBtnRestore_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "onBtnRestore_clicked of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	basar::appl::SystemEventManager::getInstance().fire( "RebuildBooking" );
	if( m_DisplayStatus == NORMAL ) {
		onBtnSearch_clicked();
	}
	else if( m_DisplayStatus == INCOMPLETE_BOOKINGS ) {
		onBtnSearchIncomplete_clicked();
	}
	else if( m_DisplayStatus == INTERNAL_ACCOUNTS ) {
		onBtnSearchInternalAccounts_clicked();
	}
	else if( m_DisplayStatus == DOUBLE_BOOKINGS ) {
		onBtnSearchDoubleBookings_clicked();
	}
}

//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
// match methods
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToBranch( basar::db::aspect::AccessorPropertyTable_YIterator branch )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToBranch of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}
	
	if( m_DetailVCShown ){
		m_RecordEditVC->matchToBranch( branch );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNarcoticArticleData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNarcoticArticleData of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	m_matNarcoticArticleBook.RightToLeft();
	btnCreate->setDisabled( false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNarcoticArticleData4InitialInventory()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNarcoticArticleData4InitialInventory of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	m_matInitialInventory.RightToLeft();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNarcoticBookings()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNarcoticBookings of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	basar::VarString s;
	for( int i = 0; i < tableWidget->columnCount(); i++ )
	{
		s = "";
		s = tableWidget->horizontalHeaderItem( i )->text().toLocal8Bit().data();
	}

	if( false == m_DetailVCShown )
	{
		initTableWidget();
		m_matNarcoticBookings.RightToLeft( 0, tableWidget->rowCount() );
		tableWidget->resizeColumnsToContents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentBooking is the AccessorPropertyTable_YIterator which is assigned dialog values to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToCurrentBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToCurrentBooking of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToBookingData( currentBooking );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentBooking is the AccessorPropertyTable_YIterator which is assigned dialog values to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNewBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNewBooking of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	m_RecordEditVC->matchToNewBooking( currentBooking );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	supplier represents the AccessorPropertyTable_YIterator which values are set to sub dialog.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNarcoticCustomerSupplierName( basar::db::aspect::AccessorPropertyTable_YIterator supplier )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNarcoticCustomerSupplierName of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToNarcoticCustomerSupplierName( supplier );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	yitBranch
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToSuppliers( basar::db::aspect::AccessorPropertyTableRef aptNarcSupp )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToCboNarcoticsSupplier of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToSuppliers( aptNarcSupp );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchCurrentBgaNo( basar::Int32& bgano )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchCurrentBgaNo of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchCurrentBgaNo( bgano );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	searchIt represents the AccessorPropertyTable_YIterator which search values are set to.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchFromSelection of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( !txtArticleNoSearch->text().isEmpty() ){
		searchIt.setString( constants::ARTICLECODE,	txtArticleNoSearch->text().toLocal8Bit().constData() );
	}

	if( !txtArticleNameSearch->text().isEmpty() )
	{
		basar::I18nString searchname = txtArticleNameSearch->text().toLocal8Bit().constData();
		searchname.upper();
		searchIt.setString( constants::ARTICLENAME,	searchname );
	}

	searchIt.setString( constants::FROMDATE, dtFromSearch->date().toString( "yyyyMMdd" ).toLocal8Bit().constData() );
	searchIt.setString( constants::TODATE,	 dtToSearch->date().toString( "yyyyMMdd" ).toLocal8Bit().constData()   );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	searchIt represents the AccessorPropertyTable_YIterator which search values are set to.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchFromNarcoticSupplierNo( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchFromNarcoticSupplierNo of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchFromNarcoticCustomerSupplierNo( searchIt );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentBooking is the AccessorPropertyTable_YIterator which is assigned dialog values to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchFromCurrentBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchFromCurrentBooking of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchFromCurrentBooking( currentBooking );
	} 
	else {
		m_matNarcoticBookings.LeftToRight( m_matNarcoticBookings.getCurrentLeft(), currentBooking );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	true if the query result is shown in the dialog immediately.
	\ret	false otherwise
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticBookVC::isSearchDone()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "isSearchDone of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	return 0 != tableWidget->rowCount();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator NarcoticBookVC::getCurrentAccessorIterator()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "getCurrentAccessorIterator of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	return m_matNarcoticBookings.getCurrentRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToCustomer of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToCustomer( customer );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToNarcoticData( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToNarcoticData of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

	if( m_DetailVCShown ){
		m_RecordEditVC->matchToNarcoticData( currentBooking );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::matchToArticleNo(basar::I18nString articleNoValue)
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "matchToArticleNo of NarcoticBookVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	}

// enable articleNo checkbox to set value for field txtArticleNoSearch.
	optArticleName->setChecked( false );
	optArticleNo->setChecked( true );
	txtArticleNoSearch->setText( QString::fromLocal8Bit(articleNoValue.c_str()) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::buttonGroupStateChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::ConstString msg = "buttonGroupStateChanged of NarcoticCategoryVC.";
		LoggerPool::loggerViewConn.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
	} 

	if( optArticleNo->isChecked() )
	{
		txtArticleNoSearch->setDisabled( false );
		txtArticleNoSearch->setFocus();
	}
	else 
	{
		txtArticleNoSearch->setDisabled( true );
		txtArticleNoSearch->setText( "" );
	}

	if( optArticleName->isChecked() )
	{
		txtArticleNameSearch->setDisabled( false );
		txtArticleNameSearch->setFocus();
	}
	else
	{
		txtArticleNameSearch->setDisabled( true );
		txtArticleNameSearch->setText( "" );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkButtonsActivation()
{
	checkIfDeleteButtonAllowedToBeActive();
	checkIfCancellationButtonAllowedToBeActive();
	checkIfEditButtonAllowedToBeActive();
	checkIfPrintDissolveReceiptButtonAllowedToBeActive();
	checkIfPrintDeliveryReceiptButtonAllowedToBeActive();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkIfDeleteButtonAllowedToBeActive()
{
	if( tableWidget->selectedRanges().isEmpty() || 
		m_matNarcoticBookings.getCurrentRight().isEnd() ||
		m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED )
	{
		btnDelete->setDisabled( true );
	}
	else{
		btnDelete->setDisabled( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkIfCancellationButtonAllowedToBeActive()
{
	if( ( isNewProcess() && btnDelete->isEnabled() ) &&
		( m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER ||
		  m_matNarcoticBookings.getCurrentRight().getString(constants::TRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS ) &&
		  m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) != constants::DELIVERY_NOTE_COPY_CREATED )
	{
		btnCancellation->setEnabled( true );
	}
	else{
		btnCancellation->setEnabled( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkIfEditButtonAllowedToBeActive()
{
	if( tableWidget->selectedRanges().isEmpty() || 
		m_matNarcoticBookings.getCurrentRight().isEnd() ||
		m_matNarcoticBookings.getCurrentRight().getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::DELIVERY_NOTE_COPY_CREATED )
	{
		btnEdit->setDisabled( true );
	}
	else{
		btnEdit->setDisabled( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkIfPrintDissolveReceiptButtonAllowedToBeActive()
{
	if( tableWidget->selectedRanges().isEmpty() ||
		m_matNarcoticBookings.getCurrentRight().isNull() ||
		m_matNarcoticBookings.getCurrentRight().isEnd() ||
		false == m_matNarcoticBookings.getCurrentRight().isContainedAndSet( constants::TRANSACTIONTYPE ) ||
		m_matNarcoticBookings.getCurrentRight().getString( constants::TRANSACTIONTYPE ) != TransactionType::DESTRUCTION )
	{
		btnPrintDissolveReceipt->setDisabled( true );
	}
	else{
		btnPrintDissolveReceipt->setDisabled( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticBookVC::checkIfPrintDeliveryReceiptButtonAllowedToBeActive()
{
	if( false == narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess() ||
		( tableWidget->selectedRanges().isEmpty() ||
		  m_matNarcoticBookings.getCurrentRight().isNull() ||
		  m_matNarcoticBookings.getCurrentRight().isEnd() ||
		  false == m_matNarcoticBookings.getCurrentRight().isContainedAndSet( constants::TRANSACTIONTYPE ) ||
		  !( m_matNarcoticBookings.getCurrentRight().getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER ||
		  m_matNarcoticBookings.getCurrentRight().getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS ) 
		) )
	{
		btnPrintDeliveryReceipt->setDisabled( true );
	}
	else{
		btnPrintDeliveryReceipt->setDisabled( false );
	}
}

} // namespace viewConn
} // namespace narcotics
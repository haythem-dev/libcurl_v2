//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "contributionvouchervc.h"
#include "loggerpool.h"
#include "contrvouchernoacknowledgement.h"
#include "deliverynotecopyenter.h"

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#pragma warning (pop)

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
using basar::db::aspect::AccessorPropertyTableRef;

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
ContributionVoucherVC::ContributionVoucherVC( QWidget *parent ) : QDialog( parent ),
	m_Initialized( false )
{
	setupUi(this);
    connectSignals();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
ContributionVoucherVC::~ContributionVoucherVC()
{
	m_Initialized = false;
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::connectSignals()
{
	QObject::connect(	rdBtnPurchaseOrderNo,		SIGNAL( toggled( bool )							),	
						this,						SLOT  (	onRdBtnPurchaseOrderNo_toggled()		));
	QObject::connect(	rdBtnContributionVoucherNo,	SIGNAL( toggled( bool )							),	
						this,						SLOT  (	onRdBtnContributionVoucherNo_toggled()	));
	QObject::connect(	rdBtnTimespan,				SIGNAL( toggled( bool )							),	
						this,						SLOT  (	onRdBtnTimespan_toggled()				));
	QObject::connect(	btnDeliveryNoteCopy,		SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnDeliveryNoteCopy_clicked()			));
	QObject::connect(	btnOpenEmptyDeliveryNote,	SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnOpenEmptyDeliveryNote_clicked()	));
	QObject::connect(	btnReprintDeliverynote,		SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnReprintDeliverynote_clicked()		));
	QObject::connect(	btnClose,					SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnClose_clicked()					));
	QObject::connect(	btnAcknowledgementReceived,	SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnAcknowledgementReceived_clicked()	));
	QObject::connect(	btnAcknowledgementPrinted,	SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnAcknowledgementPrinted_clicked()	));
	QObject::connect(	btnSearch,					SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnSearch_clicked()					));
	QObject::connect(	btnOpenEmptyContributionVoucher,	SIGNAL( clicked()						),	
						this,						SLOT  (	onBtnOpenEmptyContributionVoucher_clicked()	));
	QObject::connect(	tblAcknowledgement,			SIGNAL( itemSelectionChanged()					),	
						this,						SLOT  (	checkButtonState()						));
	QObject::connect(	btnPrintList,				SIGNAL( clicked()								),	
						this,						SLOT  (	onBtnPrintList_clicked()				));
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::closeEvent( QCloseEvent* event )
{
    shutdown();
    event->accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::initGuiCtrls()
{
	// set textfield validator
	if( 0 == leContributionVoucherNo->validator() ){
        leContributionVoucherNo->setValidator( new QRegExpValidator(QRegExp("\\d{16}"), this) );
	}

	if( 0 == leTour->validator() ){
        leTour->setValidator( new QRegExpValidator(QRegExp("\\d{6}"), this) );
	}

	if( 0 == lePurchaseOrderNo->validator() ){
		lePurchaseOrderNo->setValidator( new QRegExpValidator(QRegExp("\\d{7}"), this) );
	}

    initCboAcknowledgementStatus();

    clearGuiCtrlsPurchaseOrderNo();
    clearGuiCtrlsContributionVoucherNo();
    clearGuiCtrlsTimespan();

    toggleGuiCtrlsPurchaseOrderNo( false );
    toggleGuiCtrlsContributionVoucherNo( false );
    toggleGuiCtrlsTimespan( false );

    // initialize table widget
	initTableWidgetAcknowledgement();

	//buttons
    btnAcknowledgementReceived->setDisabled( true );
	btnAcknowledgementPrinted->setDisabled( true );
    btnDeliveryNoteCopy->setDisabled( true );
    btnReprintDeliverynote->setDisabled( true );
    btnPrintList->setDisabled( true );

	// REFACTORING remove code behind these buttons
	btnOpenEmptyDeliveryNote->setVisible( false );
	btnOpenEmptyContributionVoucher->setVisible( false );
	lblempty->setVisible( false );
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::initCboAcknowledgementStatus()
{
    
    cboAcknowledgementStatus->setColumnCount( 2 );
    cboAcknowledgementStatus->hideHorizontalHeader();
    cboAcknowledgementStatus->hideVerticalHeader();

    cboAcknowledgementStatus->clearContext();
    
    cboAcknowledgementStatus->addItem( 0, 1, QString::number(constants::INVALID) );
    cboAcknowledgementStatus->addItem( 0, 0, tr("All") );

    cboAcknowledgementStatus->addItem( 1, 1, QString::number(constants::PRINTED) );
    cboAcknowledgementStatus->addItem( 1, 0, tr("Printed") );
    
    cboAcknowledgementStatus->addItem( 2, 1, QString::number(constants::ACKNOWLEDGEMENT_RECEIVED) );
    cboAcknowledgementStatus->addItem( 2, 0, tr("Acknowledgement received") );

    cboAcknowledgementStatus->addItem( 3, 1, QString::number(constants::CHANGED_ACKNOWLEDGEMENT) );
    cboAcknowledgementStatus->addItem( 3, 0, tr("Changed Acknowledgement") );

    cboAcknowledgementStatus->addItem( 4, 1, QString::number(constants::DELIVERY_NOTE_COPY_CREATED) );
    cboAcknowledgementStatus->addItem( 4, 0, tr("Delivery Note Copy created") );
    
    cboAcknowledgementStatus->hideColumn( 1 );

    cboAcknowledgementStatus->setMinimumWidthList( 300 );
    cboAcknowledgementStatus->resizeColumnsToContents();
    
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onRdBtnPurchaseOrderNo_toggled()
{
    //enable own gui ctls, disable others
    toggleGuiCtrls();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::toggleGuiCtrls()
{
    clearGuiCtrlsPurchaseOrderNo();
    clearGuiCtrlsContributionVoucherNo();
    clearGuiCtrlsTimespan();
    toggleGuiCtrlsPurchaseOrderNo( rdBtnPurchaseOrderNo->isChecked() );
    toggleGuiCtrlsContributionVoucherNo( rdBtnContributionVoucherNo->isChecked() );
    toggleGuiCtrlsTimespan( rdBtnTimespan->isChecked() );
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onRdBtnContributionVoucherNo_toggled()
{
    toggleGuiCtrls();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onRdBtnTimespan_toggled()
{
    toggleGuiCtrls();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::toggleGuiCtrlsPurchaseOrderNo( bool state )
{
    lePurchaseOrderNo->setEnabled( state );
    dtPurchaseOrderNo->setEnabled( state );
	dtPurchaseOrderNo->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtPurchaseOrderNo->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	if (state == true)
		lePurchaseOrderNo->setFocus();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::toggleGuiCtrlsContributionVoucherNo( bool state )
{
    leContributionVoucherNo->setEnabled( state );
    dtContributionVoucher->setEnabled( state );
	dtContributionVoucher->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtContributionVoucher->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	if (state == true)
		leContributionVoucherNo->setFocus();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::toggleGuiCtrlsTimespan( bool state )
{
    dtTimespanFrom->setEnabled( state );
	dtTimespanFrom->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtTimespanFrom->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

    dtTimespanTo->setEnabled( state );
	dtTimespanTo->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtTimespanTo->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

    cboAcknowledgementStatus->setEnabled( state );
	leTour->setEnabled( state );   

	if (state == true)
		dtTimespanFrom->setFocus();
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::clearGuiCtrlsPurchaseOrderNo()
{
    lePurchaseOrderNo->clear();
    dtPurchaseOrderNo->setDate( QDate::currentDate() );
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::clearGuiCtrlsContributionVoucherNo()
{
    leContributionVoucherNo->clear();
    dtContributionVoucher->setDate( QDate::currentDate() );
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::clearGuiCtrlsTimespan()
{
    QDate from = QDate::currentDate();    
    dtTimespanFrom->setDate( from.addMonths(-1) ); 
    dtTimespanTo->setDate( QDate::currentDate() );
	dtTimespanTo->setTime( QTime(23, 59, 59) );
    cboAcknowledgementStatus->setCurrentIndex( 0 );
	leTour->clear();
}

//-----------------------------------------------------------------------------------------------//
bool ContributionVoucherVC::areSearchTimespanParametersValid()
{
    //check if both dates are at least today
    if( dtTimespanFrom->date() > QDate::currentDate() )
    {
        //inform user
        basar::gui::tie::warningMsgBox( this, tr("from Date is in the futur").toLocal8Bit().constData() );
        return false;
    }

    if( dtTimespanTo->date() > QDate::currentDate() )
    {
        //inform user
        basar::gui::tie::warningMsgBox( this, tr("to Date is in the futur").toLocal8Bit().constData() );
        return false;
    }

    //check if date from is before date to
    if( dtTimespanFrom->date() > dtTimespanTo->date() )
    {
        //inform user
        basar::gui::tie::warningMsgBox( this, tr("from Date later than to date").toLocal8Bit().constData() );
        return false;
    }

    //check if time from is before time to
    if( dtTimespanFrom->date() == dtTimespanTo->date() && dtTimespanFrom->time() > dtTimespanTo->time())
    {
        //inform user
        basar::gui::tie::warningMsgBox( this, tr("from Date later than to date").toLocal8Bit().constData() );
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------------------------//
bool ContributionVoucherVC::areSearchPurchaseOrderParametersValid()
{
    //check if puchase order no is set
    if( 0 == lePurchaseOrderNo->text().length() )
    {
        basar::gui::tie::warningMsgBox( this, tr("no purchaseorderno entered").toLocal8Bit().constData() );
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------//
bool ContributionVoucherVC::areSearchContributionVoucherParametersValid()
{
    //check if contribution voucher no is set
    if( 0 == leContributionVoucherNo->text().length() )
    {
        basar::gui::tie::warningMsgBox( this, tr("no contributionvoucherno entered").toLocal8Bit().constData() );
        return false;
    }

    //check if contribution voucher no is either exactly 8 or exactly 16 digits long
    QRegExp rex = QRegExp("\\d{8}|\\d{16}");

    if( !rex.exactMatch( leContributionVoucherNo->text() ) )
    {
        basar::gui::tie::warningMsgBox( this, tr("please enter 8 digit or 16 digit contribution voucher no").toLocal8Bit().constData() );
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------//
bool ContributionVoucherVC::areSearchParametersValid()
{
    bool retVal = false;
    
    if( rdBtnContributionVoucherNo->isChecked() )
    {
        //do checks
        retVal = areSearchContributionVoucherParametersValid();
    }

    if( rdBtnPurchaseOrderNo->isChecked() )
    {
        //do checks
        retVal = areSearchPurchaseOrderParametersValid();
    }

    if( rdBtnTimespan->isChecked() )
    {
        //do checks
        retVal = areSearchTimespanParametersValid();
    }

    return retVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::initTableWidgetAcknowledgement()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "initTableWidgetAcknowledgement of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	tblAcknowledgement->clear();

	tblAcknowledgement->setHorizontalHeaderLabels( m_lstLables );
	tblAcknowledgement->setColumnCount( m_lstLables.size() );
	tblAcknowledgement->hideVerticalHeader();
	tblAcknowledgement->resizeColumnsToContents();

	for( int col = 0; col < tblAcknowledgement->columnCount(); col++ ){
		tblAcknowledgement->setEditableColumn( col, false );	
	}
	tblAcknowledgement->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	void
	\param	narcoticsBookings is the AccessorPropertyTable necessary to create the matcher
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::initHeaderLables( QStringList& lstLables, std::list<basar::VarString>& lstOriginalAttributes )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "initHeaderLables of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( !m_Initialized )
	{
		lstLables.push_back            ( tr("PurchaseOrderNo")					);
		lstOriginalAttributes.push_back( constants::PURCHASEORDERNO				);
	    
		lstLables.push_back            ( tr("Date")								);
		lstOriginalAttributes.push_back( constants::TRANSACTIONDATE				);
	    
		lstLables.push_back            ( tr("CustomerNo")						);
		lstOriginalAttributes.push_back( constants::CUSTOMERSUPPLIERNO			);

		lstLables.push_back            ( tr("Name")								);
		lstOriginalAttributes.push_back( constants::CUSTOMER_SUPPLIER_NAME		);

		lstLables.push_back            ( tr("Location")							);
		lstOriginalAttributes.push_back( constants::CUSTOMER_SUPPLIER_LOCATION	);

		lstLables.push_back            ( tr("Tour")   							);
        lstOriginalAttributes.push_back( constants::TOURID 						);

		lstLables.push_back            ( tr("ContributionVoucherNo")			);
		lstOriginalAttributes.push_back( constants::CONTRIBUTIONVOUCHERNO		);

		lstLables.push_back            ( tr("UserName")             			);
		lstOriginalAttributes.push_back( constants::USERNAME            		);

        lstLables.push_back            ( tr("AcknowledgementStatus")   			);
        lstOriginalAttributes.push_back( constants::ACKNOWLEDGEMENTSTATUS  		);

		lstLables.push_back            ( tr("PrintedDate")          			);
        lstOriginalAttributes.push_back( constants::PRINTED_DATE        		);

		lstLables.push_back            ( tr("PrintedTime")          			);
        lstOriginalAttributes.push_back( constants::PRINTED_TIME        		);

        lstLables.push_back            ( tr("TelNo")							);
		lstOriginalAttributes.push_back( constants::TELNO						);

		lstLables.push_back            ( tr("Quickdialing")						);
		lstOriginalAttributes.push_back( constants::QUICKDIALING				);

		lstLables.push_back            ( tr("FaxNo")							);
		lstOriginalAttributes.push_back( constants::FAXNO						);

		m_Initialized = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	void
	\param	narcoticsBookings is the AccessorPropertyTable necessary to create the matcher
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::init( AccessorPropertyTableRef acknowledgements )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initHeaderLables( m_lstLables, m_OriginalAttributeNames );
	initGuiCtrls();
	createMatchers( acknowledgements );
    tblAcknowledgement->resizeColumnsToContents();
    checkButtonState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	void
	\param	narcoticsBookings is the AccessorPropertyTable necessary to create the matcher
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::createMatchers( AccessorPropertyTableRef openAcknowledgements )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "createMatchers of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	using basar::gui::tie::Manager;
	
	// initialize transformers
	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().data() );
    VariableDateTransformer printedDateTransformer( constants::PRINTED_DATE, tr("PrintedDate").toLocal8Bit().data() );
    VariableTimeTransformer printedTimeTransformer( constants::PRINTED_TIME, tr("PrintedTime").toLocal8Bit().data() );

    AcknowledgementStatusTransformer statusTransformer;

	m_matAcknowledgements = Manager::getInstance().createMatcher( grpAcknowledgements, openAcknowledgements );
	m_matAcknowledgements.transformProperties( tblAcknowledgement->horizontalHeaderLabels(), m_OriginalAttributeNames );
	m_matAcknowledgements.push_back( dateTransformer );
    m_matAcknowledgements.push_back( printedDateTransformer );
    m_matAcknowledgements.push_back( printedTimeTransformer );
    m_matAcknowledgements.push_back( statusTransformer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	int indicates the value returned by a modal dialog. 
			Value '1' represents QDialog::Accepted and value '0' represents QDialog::Rejected.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum ContributionVoucherVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true by default
			element of the erased one.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
bool ContributionVoucherVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matAcknowledgements.reset();
	m_OriginalAttributeNames.clear();
	m_lstLables.clear();

    return true;
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnReprintDeliverynote_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnReprintDeliverynote_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "ReprintDeliveryNote" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

    if( ret.ret == basar::appl::HANDLER_ERROR ){
        basar::gui::tie::warningMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
    }
	
    if( areSearchParametersValid() ){
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
    else{
        ret = basar::appl::SystemEventManager::getInstance().fire( "RefreshAcknowledgement" );
    }
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onBtnPrintList_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnPrintList_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintAcknowledgementList" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	if( ret.ret == basar::appl::HANDLER_ERROR ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnAcknowledgementReceived_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnreceived_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "MarkAsReceived" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	if( ret.ret == basar::appl::HANDLER_ERROR ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

    if( areSearchParametersValid() ){
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
    else{
        ret = basar::appl::SystemEventManager::getInstance().fire( "RefreshAcknowledgement" );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnAcknowledgementPrinted_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnPrinted_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "MarkAsPrinted" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	if( ret.ret == basar::appl::HANDLER_ERROR ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

    if( areSearchParametersValid() ){
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
    else{
        ret = basar::appl::SystemEventManager::getInstance().fire( "RefreshAcknowledgement" );
    }
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onBtnOpenEmptyContributionVoucher_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnOpenEmptyContributionVoucher_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "OpenEmptyContributionVoucherDelCopy" );	
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}	
}

//-----------------------------------------------------------------------------------------------//
void ContributionVoucherVC::onBtnOpenEmptyDeliveryNote_clicked()
{	
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnOpenEmptyDeliveryNote_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "OpenEmptyDeliveryNote" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

    if( areSearchParametersValid() ){
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
    else{
        ret = basar::appl::SystemEventManager::getInstance().fire( "RefreshAcknowledgement" );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnDeliveryNoteCopy_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnDeliveryNoteCopy_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintDeliveryNoteCopy" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	if( areSearchParametersValid() ){
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
    else{
        ret = basar::appl::SystemEventManager::getInstance().fire( "RefreshAcknowledgement" );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnsearch_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
    basar::appl::EventReturnStruct ret;

    //done: call sanity checking
    if( areSearchParametersValid() ){
	    tblAcknowledgement->clearContext(); 
        ret = basar::appl::SystemEventManager::getInstance().fire( "SearchAcknowledgements" );
    }
	
	if( !m_matAcknowledgements.getRight().isNull() && !m_matAcknowledgements.getRight().empty() )
	{
		tblAcknowledgement->selectRow( 0 );
        tblAcknowledgement->setFocus();
	}
	
	setCursor( Qt::ArrowCursor );

	if ( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnclose_clicked of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	no param.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::matchFromOpenAcknowledgements()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromOpenAcknowledgements of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	tblAcknowledgement->clearContext();
	m_matAcknowledgements.RightToLeft( 0, tblAcknowledgement->rowCount() );

	if( tblAcknowledgement->rowCount() != 0 ){
		tblAcknowledgement->setDisabled( false );
	}
	else{
		tblAcknowledgement->setDisabled( true );
	}
	tblAcknowledgement->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	no param.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::matchToSearch( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToSearch of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	//! \todo FN matcher?
    
    //check which part has to be matched
    if( rdBtnContributionVoucherNo->isChecked() )
    {
        if( 8 == leContributionVoucherNo->text().length() )
        {
            searchIt.setInt32( constants::CONTRIBUTIONVOUCHERNO, leContributionVoucherNo->text().toInt() );
            searchIt.setInt32( constants::TRANSACTIONDATE,		 dtContributionVoucher->date().toString("yyyyMMdd").toInt() );
        }
        else if( 16 == leContributionVoucherNo->text().length() ){
            searchIt.setString( constants::SCANNED_CONTRIBUTION_VOUCHER_NO, leContributionVoucherNo->text().toLocal8Bit().constData() );
        }
        else{
            this->error( this, tr("Entered value has wrong format (neither 8 nor 16 numbers)!").toLocal8Bit().constData() );
        }
    }

    if( rdBtnPurchaseOrderNo->isChecked() )
    {
        //match searchIt
        searchIt.setInt32( constants::PURCHASEORDERNO,  lePurchaseOrderNo->text().toInt() );
        searchIt.setInt32( constants::TRANSACTIONDATE,  dtPurchaseOrderNo->date().toString("yyyyMMdd").toInt() );
    }

    if( rdBtnTimespan->isChecked() )
    {
        //match searchIt
        searchIt.setInt32( constants::FROMDATE,	dtTimespanFrom->date().toString("yyyyMMdd").toInt() );
	    searchIt.setInt32( constants::TODATE,	dtTimespanTo->date().toString("yyyyMMdd").toInt() );
		searchIt.setInt32( constants::FROMTIME, dtTimespanFrom->time().toString("hhmmss").toInt() );
		searchIt.setInt32( constants::TOTIME,   dtTimespanTo->time().toString("hhmmss").toInt() );
        
        if( cboAcknowledgementStatus->currentIndex() > 0 ){
            searchIt.setInt16( constants::ACKNOWLEDGEMENTSTATUS, cboAcknowledgementStatus->itemText( cboAcknowledgementStatus->currentIndex(), 1 ).toShort() );
        }
        
		if( 0 < leTour->text().length() )
        {
			searchIt.setString( constants::TOURID, leTour->text().toLocal8Bit().data() );
		}
    }

	if (cbOrderByTour->checkState() == Qt::Checked)
		searchIt.setInt16( constants::ORDERBYTOUR, 2 );
	else if (cbOrderByTour->checkState() == Qt::PartiallyChecked)
		searchIt.setInt16( constants::ORDERBYTOUR, 1 );
	else
		searchIt.setInt16( constants::ORDERBYTOUR, 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	no param.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::matchFromSearch()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSearch of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
    tblAcknowledgement->clearContext();

    m_matAcknowledgements.RightToLeft( 0, tblAcknowledgement->rowCount() );

    if( tblAcknowledgement->rowCount() > 0 ){
		tblAcknowledgement->setDisabled( false );
    }
    else{
        tblAcknowledgement->setDisabled( true );
    }

    tblAcknowledgement->resizeColumnsToContents();
    checkButtonState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator representing the YIterator which is worked on.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator ContributionVoucherVC::getFirstSelectedContributionVoucher()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getFirstSelectedContributionVoucher of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matAcknowledgements.getSelectedRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator ContributionVoucherVC::getNextSelectedContributionVoucher()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getNextSelectedContributionVoucher of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matAcknowledgements.getNextSelectedRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef ContributionVoucherVC::getCurrentDisplayedData() const
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getCurrentDisplayedData of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

    return m_matAcknowledgements.getRight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ContributionVoucherVC::checkButtonState()
{ 
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "checkButtonState of ContributionVoucherVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	int selRows = tblAcknowledgement->selectionModel()->selectedRows().size();
	int ackStatus = constants::INVALID;
    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matAcknowledgements.getSelectedRight();

    if (false == yit.isNull() && false == yit.isEnd() && yit.isContainedAndSet(constants::ACKNOWLEDGEMENTSTATUS))
	{
		ackStatus = yit.getInt16(constants::ACKNOWLEDGEMENTSTATUS);
	}

	// Lieferscheindoppel erstellen: es darf nur GENAU eine Zeile markiert sein, mit Status 2 oder 3
	btnDeliveryNoteCopy->setEnabled(selRows == 1 && (ackStatus == constants::ACKNOWLEDGEMENT_RECEIVED || ackStatus == constants::PRINTED));

	// Lieferschein nachdrucken: mindestens eine Zeile markiert
	btnReprintDeliverynote->setEnabled(selRows > 0 && false == yit.isEnd());

	// Empfangsbestätigung eingegangen / widerrufen: Alle markierten Zeilen im entsprechenden Status
	btnAcknowledgementReceived->setEnabled(selRows > 0 && ackStatus == constants::PRINTED);
	btnAcknowledgementPrinted->setEnabled(selRows > 0 && ackStatus == constants::ACKNOWLEDGEMENT_RECEIVED);
	while (false == yit.isNull() && false == yit.isEnd() && yit.isContainedAndSet(constants::ACKNOWLEDGEMENTSTATUS))
	{
		if (yit.getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::PRINTED)
			btnAcknowledgementPrinted->setEnabled(false);
		if (yit.getInt16(constants::ACKNOWLEDGEMENTSTATUS) == constants::ACKNOWLEDGEMENT_RECEIVED)
			btnAcknowledgementReceived->setEnabled(false);
		yit = m_matAcknowledgements.getNextSelectedRight();
	}
	// Liste drucken: Mindestens eine Zeile vorhanden
	btnPrintList->setEnabled(false == m_matAcknowledgements.getRight().begin().isEnd());
}

} // namespace viewConn
} // namespace narcotics
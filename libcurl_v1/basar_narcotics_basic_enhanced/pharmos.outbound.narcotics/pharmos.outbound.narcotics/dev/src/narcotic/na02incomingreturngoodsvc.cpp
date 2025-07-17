//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "na02incomingreturngoodsvc.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

using basar::VarString;
using basar::Int32;
using basar::Int16;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::gui::tie::PropertyNameTransformer;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
IncomingReturnGoodsVC::IncomingReturnGoodsVC( QWidget *parent )
: QDialog( parent ),m_existingBooking( true )
{
	setupUi(this);
	wireEvents();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
IncomingReturnGoodsVC::~IncomingReturnGoodsVC()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::wireEvents()
{
	QObject::connect(	cb_transactiontype,		SIGNAL(	currentIndexChanged( int )				),
						this,					SLOT  (	change_cb_transactiontype( int )		));
	QObject::connect(	articlecode,			SIGNAL(	editingFinished()						),
						this,					SLOT  (	change_articleno()						));
	QObject::connect(	customersupplierno,		SIGNAL(	editingFinished()						),
						this,					SLOT  (	change_customersupplierno()				));
	QObject::connect(	cb_customersupplierno,	SIGNAL(	currentIndexChanged( int )				),
						this,					SLOT  (	change_cb_customersupplierno( int )		));
	QObject::connect(	btnNext,				SIGNAL(	clicked()								),
						this,					SLOT  (	onBtnNext_clicked()						));
	QObject::connect(	btnCancel,				SIGNAL(	clicked()								),
						this,					SLOT  (	onBtnCancel_clicked()					));
	QObject::connect(	btnClose,				SIGNAL(	clicked()								),
						this,					SLOT  (	onBtnClose_clicked()					));
	QObject::connect(	purchaseorderno,		SIGNAL(	textChanged( QString )					),
						this,					SLOT  (	onPurchaseorderno_textChanged( QString )));
	QObject::connect(	articlename,			SIGNAL(	textChanged( QString )					),
						this,					SLOT  (	onArticlename_textChanged( QString )	));
	QObject::connect(	packageunit,			SIGNAL(	textChanged( QString )					),
						this,					SLOT  (	onPackageunit_textChanged( QString )	));
	QObject::connect(	qtytransaction,			SIGNAL(	textChanged( QString )					),
						this,					SLOT  (	onQtytransaction_textChanged( QString )	));
	QObject::connect(	btnCancel,				SIGNAL(	textChanged( QString )					),
						this,					SLOT  (	onPharmaform_textChanged( QString )		));
	QObject::connect(	transactiondate,		SIGNAL(	dateChanged()							),
						this,					SLOT  (	onTransactiondate_dateChanged()			));
	QObject::connect(	transactiontime,		SIGNAL(	timeChanged()							),
						this,					SLOT  (	onTransactiontime_timeChanged()			));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::init( basar::db::aspect::AccessorPropertyTableRef supplierTable )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initGuiCtrls();

	m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cb_customersupplierno, supplierTable );

	if( m_OrigHeaderAttrList.empty() )
	{
		m_OrigHeaderAttrList.push_back( "narcoticssupplierno"	);
		m_OrigHeaderAttrList.push_back( "suppliername"			);
	}

	m_matSupplier.transformProperties( cb_customersupplierno->horizontalHeaderLabels(), m_OrigHeaderAttrList );
	m_matSupplier.RightToLeft();
	cb_customersupplierno->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::initGuiCtrls()
{
	cb_customersupplierno->setColumnCount( 2 );
	cb_customersupplierno->setHorizontalHeaderItem( 0, tr("BGA") );
	cb_customersupplierno->setHorizontalHeaderItem( 1, tr("Name") );
    cb_customersupplierno->setMinimumWidthList( 400 );
	customersupplierno->hide();
	customersupplierno->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,7}"), this) );

	cb_transactiontype->clear();
	cb_transactiontype->addItem( tr("Incoming Delivery") );
	cb_transactiontype->addItem( tr("Return") );

	purchaseorderno->setText("");
	purchaseorderno->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,5}"), this ) );
	customersuppliername->setText("");
	customersupplierlocation->setText("");
	customersupplierstreet->setText("");
	customersuppliercip->setText("");
	articlecode->setText("");
	articlecode->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,8}"), this) );
	articleno->setText("");
	articleno->setVisible( false );
	articlename->setText("");
	packageunit->setText("");
	pharmaform->setText("");
	qtytransaction->setText("");
	qtytransaction->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,4}"), this ) );
	customersupplierno->setText("");
	transactiondate->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	transactiondate->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	// disable save buttons
	btnClose->setDisabled( true );
	btnNext->setDisabled( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum IncomingReturnGoodsVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool IncomingReturnGoodsVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "reset of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matSupplier.reset();
	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::matchToBooking(AccessorPropertyTable_YIterator newBooking)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToBooking of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( newBooking.getState(constants::TRANSACTIONTYPE) != basar::SS_UNSET )
	{
		// set state of combobox cb_customersupplierno
		basar::I18nString supplierNoValue;
		supplierNoValue.itos( newBooking.getInt32(constants::CUSTOMERSUPPLIERNO) );
		QString qSuppliernoValue = QString::fromLocal8Bit(supplierNoValue.c_str());
		QString ff;
		for( int row = 0; row < cb_customersupplierno->count(); row++ )
		{
			ff = cb_customersupplierno->itemText( row, 0 );
			if( qSuppliernoValue == ff )
			{
				cb_customersupplierno->setCurrentIndex( row );
				break;
			}
		}
	}

	basar::gui::tie::Manager::getInstance().MatchFromIterator( this, newBooking );
	if( newBooking.getState(constants::TRANSACTIONTYPE) == basar::SS_UNSET )
	{
		cb_transactiontype->setCurrentIndex( 0 );
		change_cb_customersupplierno( 0 );
		transactiondate->setDate( QDate::currentDate() );
		transactiontime->setTime( QTime::currentTime() );
	}
	else
	{
		if( newBooking.getString( constants::TRANSACTIONTYPE ) == TransactionType::GOODSIN_DELIVERY ){
			cb_transactiontype->setCurrentIndex( 0 );
		}
		else {
			cb_transactiontype->setCurrentIndex( 1 );
		}

        transactiontime->setTime( QTime( int( newBooking.getInt32( constants::TRANSACTIONTIME ) / 10000 ),
                                         int((newBooking.getInt32( constants::TRANSACTIONTIME ) % 10000 ) / 100 ),
											  newBooking.getInt32( constants::TRANSACTIONTIME ) % 100) );
        transactiontime->setDisplayFormat( "hh:mm:ss" );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::matchFromBooking( AccessorPropertyTable_YIterator newBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromBooking of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::gui::tie::Manager::getInstance().MatchToIterator( this, newBooking );

	if( newBooking.isContainedAndSet( constants::ARTICLECODE ) ){
		newBooking.setString( constants::ARTICLECODE, removeLeadingZeros( newBooking.getString( constants::ARTICLECODE ) ) );
	}

	basar::DateTime date;
	QDate qdate = transactiondate->date();
	date.setDate( static_cast<Int16>(qdate.day()), static_cast<Int16>(qdate.month()), static_cast<Int16>(qdate.year()) );
	// whenever new incoming deliveries are created or already existing ones changed current date is always set
	//newBooking.setInt32( constants::TRANSACTIONDATE, date.getCurrent().getDate() );

	QTime qtime = transactiontime->time();
	date.setTime( qtime.hour() * 10000000 + qtime.minute() * 100000 + qtime.second() * 1000 );
	newBooking.setInt32( constants::TRANSACTIONTIME,date.getTime() / 1000 );

	if( cb_transactiontype->currentIndex() == 0 ){
		newBooking.setString( constants::TRANSACTIONTYPE, TransactionType::GOODSIN_DELIVERY );
	}
	else{
		newBooking.setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_RETURNS );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::clearDialog()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "clearDialog of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	transactiondate->setDate( QDate::currentDate() );
	transactiontime->setTime( QTime::currentTime() );
	articlecode->clear();
	qtytransaction->clear();
	articlename->clear();
	packageunit->clear();
	pharmaform->clear();
	customersuppliername->clear();
	customersupplierlocation->clear();
	customersupplierstreet->clear();
	customersuppliercip->clear();
	customersupplierno->clear();
	purchaseorderno->clear();
	cb_transactiontype->setCurrentIndex( 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnClose_clicked of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SaveIncomingGoodsList" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onBtnCancel_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnCancel_clicked of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onBtnNext_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_btnNext_clicked of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SaveIncomingGoodsList" );
	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	clearDialog();
	basar::appl::SystemEventManager::getInstance().fire( "NewBookingIncomingGood" );
	setCursor( Qt::ArrowCursor );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::change_cb_transactiontype( int index )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "change_cb_transactiontype of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	if( index == 0 )
	{
		customersupplierno->hide();
		cb_customersupplierno->show();
	}
	else
	{
		customersupplierno->show();
		cb_customersupplierno->hide();
	}
	setCursor( Qt::ArrowCursor );

	// check if save buttons can be enabled
	enableSaveButtons();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::change_cb_customersupplierno( int index )
{
    index;
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "change_cb_customersupplierno of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	setCursor( Qt::WaitCursor );

	customersuppliername->setText( QString::fromLocal8Bit(m_matSupplier.getCurrentRight().getString( "suppliername" ).c_str()) );

	basar::VarString no;
	no.format( "%ld",m_matSupplier.getCurrentRight().getInt32("narcoticssupplierno") );
	customersupplierno->setText( QString::fromLocal8Bit(no.c_str()) );

	customersupplierlocation->setText( QString::fromLocal8Bit( m_matSupplier.getCurrentRight().getString( "supplierlocation" ).c_str() ));
	customersupplierstreet->setText(QString::fromLocal8Bit(m_matSupplier.getCurrentRight().getString("street").c_str()));
	customersuppliercip->setText(QString::fromLocal8Bit(m_matSupplier.getCurrentRight().getString("postcode").c_str()));

	setCursor( Qt::ArrowCursor );

	// check if save buttons can be enabled
	enableSaveButtons();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::change_customersupplierno()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "change_customersupplierno of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	// reinitialize fields for customer data
	clearContentsCustomerData();
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "ReadCustomerDataIncomingGood" );
	setCursor( Qt::ArrowCursor );

	// check if save buttons can be enabled
	enableSaveButtons();

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::change_articleno()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "change_articleno of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );

	// reinitialize fields for article data
	clearContentsArticleData();
	basar::appl::EventReturnStruct ret;

	if( !articlecode->text().isEmpty() ){
		ret = basar::appl::SystemEventManager::getInstance().fire( "ReadArticleDataIncomingGood" );
	}
	setCursor( Qt::ArrowCursor );

	enableSaveButtons();

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::clearContentsArticleData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "clearContentsArticleData of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	articlename->setText( "" );
	packageunit->setText( "" );
	pharmaform->setText( "" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::clearContentsCustomerData()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "clearContentsCustomerData of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	customersuppliername->setText( "" );
	customersupplierlocation->setText( "" );
	customersupplierstreet->setText("");
	customersuppliercip->setText("");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::enableSaveButtons()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "enableSaveButtons of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// enabling of buttons "close" und "next" if fields are completely filled
	// additionally, in order to enable button "next" subdialog must have been opened by "btnRecordItem"
	if( !m_existingBooking )
	{
		btnNext->setDisabled( (purchaseorderno->text().isEmpty()) || (articlecode->text().isEmpty()) || (articlename->text().isEmpty()) ||
				(packageunit->text().isEmpty()) || (qtytransaction->text().isEmpty()) || (customersuppliername->text().isEmpty()) ||
				(customersupplierlocation->text().isEmpty()) || (customersupplierstreet->text().isEmpty()) ||  
			    (customersuppliercip->text().isEmpty()) || (customersupplierno->text().isEmpty()) );
	}
	else{
		btnNext->setDisabled( true );
	}

	btnClose->setDisabled( (purchaseorderno->text().isEmpty()) || (articlecode->text().isEmpty()) || (articlename->text().isEmpty()) ||
			(packageunit->text().isEmpty()) || (qtytransaction->text().isEmpty()) || (customersuppliername->text().isEmpty()) ||
			(customersupplierlocation->text().isEmpty()) || (customersupplierstreet->text().isEmpty()) || 
			(customersuppliercip->text().isEmpty()) || (customersupplierno->text().isEmpty()) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onPurchaseorderno_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_purchaseorderno_textChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onArticlename_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_articlename_textChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onPackageunit_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_packageunit_textChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onQtytransaction_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_qtytransaction_textChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onPharmaform_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_pharmaform_textChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onTransactiondate_dateChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_transactiondate_dateChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::onTransactiontime_timeChanged()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "on_transactiontime_timeChanged of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	enableSaveButtons();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingReturnGoodsVC::enableButtonStateNext( bool enable )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "enableButtonStateNext of IncomingReturnGoodsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_existingBooking = !enable;
}

} //namespace viewConn
} //namespace narcotics
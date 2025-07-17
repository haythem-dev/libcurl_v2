//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Bjoern Bischof
 *  \date   07.04.2006
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "createprocessnarcoticsbooking.h"
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
using basar::db::aspect::AccessorPropertyTableRef;

//-----------------------------------------------------------------------------------------------//
// class definition
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	parent passed window pointer this dialog belongs to.
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
CreateProcessNarcoticsBookingVC::CreateProcessNarcoticsBookingVC( QWidget* parent )
: QDialog( parent ), m_cboLC( 0 ), m_LoadedCompleted( false ), m_bCboLoaded( false ), 
  m_bInit( false ), m_bBranchLoaded( false ), m_MatchingDone( false )
{
	setupUi(this);
	init();
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! throw	return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::wireEvents()
{
	QObject::connect(	customersupplierno,		SIGNAL( textChanged( QString )						), 
						this,					SLOT  ( onCustomersupplierno_textChanged( QString )	));
	QObject::connect(	m_cboLC2,				SIGNAL( currentIndexChanged( int )					), 
						this,					SLOT  ( currentIndexChangedOfLineCode( int )		));
	QObject::connect(	cboCustomerSupplierNo,	SIGNAL( currentIndexChanged( int )					), 
						this,					SLOT  ( currentIndexChangedOfCustomerSupplier( int )));
	QObject::connect(	btnClose,				SIGNAL( clicked()									), 
						this,					SLOT  ( onBtnClose_clicked()						));
	QObject::connect(	btnClose,				SIGNAL( clicked()									), 
						this,					SLOT  ( onBtnClose_clicked()						));
	QObject::connect(	btnSave,				SIGNAL( clicked()									), 
						this,					SLOT  ( onBtnSave_clicked()							));
	QObject::connect(	customersupplierno,		SIGNAL( editingFinished()							), 
						this,					SLOT  ( onCustomersupplierno_editingFinished()		));
	QObject::connect(	customersuppliername,	SIGNAL( textChanged( QString )						), 
						this,					SLOT  ( onCustomersuppliername_textChanged( QString )));
	QObject::connect(	m_cboLC2,				SIGNAL( currentIndexChanged( int )					), 
						this,					SLOT  ( onM_cboLC2_currentIndexChanged( int )		));
	QObject::connect(	qtytransaction,			SIGNAL( textChanged( QString )						), 
						this,					SLOT  ( onQtytransaction_textChanged( QString )		));
	QObject::connect(	purchaseorderno,		SIGNAL( textChanged( QString )						), 
						this,					SLOT  ( onPurchaseorderno_textChanged( QString )	));
	QObject::connect(	transactiondate,		SIGNAL( dateChanged(const QDate&)					), 
						this,					SLOT  ( onTransactiondate_dateChanged(const QDate&)	));
	QObject::connect(	transactiontime,		SIGNAL( timeChanged(const QTime&)					), 
						this,					SLOT  ( onTransactiontime_timeChanged(const QTime&)	));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::init()
{
	/////////////////////////////////////////////////////////////////////////
	// group narcotic data
	/////////////////////////////////////////////////////////////////////////
	setDisabled_grpNarcoticData( false );

	/////////////////////////////////////////////////////////////////////////
	// group article data
	/////////////////////////////////////////////////////////////////////////
	setDisabled_grpArticleData( false );

	/////////////////////////////////////////////////////////////////////////
	// group record edit pos
	/////////////////////////////////////////////////////////////////////////
	initGuiCtrls();

	/////////////////////////////////////////////////////////////////////////
	// buttons
	/////////////////////////////////////////////////////////////////////////
	btnSave->setDisabled( true );

	//---------------------------------------------------//
	//	with every display of the dialog it must register 
	//	if a narcotic booking has been changed or not to decide a new start 
	//	of the search in the master dialog.
	//---------------------------------------------------//
	m_DatasetSaved = false;

	if( m_OriginalAttributeNames.empty() )
	{	
		m_OriginalAttributeNames.push_back( constants::NARCOTICSSUPPLIERNO	);
		m_OriginalAttributeNames.push_back( constants::SUPPLIERNAME			);
		m_OriginalAttributeNames.push_back( constants::SUPPLIERLOCATION		);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::loadCombo()
{
	if( false == m_bCboLoaded )
	{
		m_LoadedCompleted = false;		
		basar::appl::SystemEventManager::getInstance().fire( "SearchNarcoticsSupplier" );
		m_LoadedCompleted = true;
		m_bCboLoaded = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
CreateProcessNarcoticsBookingVC::~CreateProcessNarcoticsBookingVC()
{
	if( 0 != m_cboLC )
	{
		delete m_cboLC;
		m_cboLC = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \param	LINE_CODE represents the line code (transactiontype) of the current narcotics booking
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::chooseLineCodeDescription( const basar::VarString& LINE_CODE )
{
	QString qLineCode = QString::fromLocal8Bit(LINE_CODE.c_str());
	QString ff;
	int row;

	for( row = 0; row < m_cboLC2->count(); row++ )
	{
		ff = m_cboLC2->itemText( row, 1 );
		if( qLineCode == ff )
		{
			m_cboLC2->setCurrentIndex( row );
			return;
		}
	}	
	m_cboLC2->setCurrentIndex( row );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::initGuiCtrls()
{
	grpRecordEditPos->setDisabled( false );
	
	purchaseorderno->setDisabled( false );
	purchaseorderno->setText( "" );
	purchaseorderno->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,5}"), this) );
	
	customersupplierno->setDisabled( false );
	customersupplierno->setText( "" );
	customersupplierno->setVisible( true );
	customersupplierno->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,7}"), this) );

	customersuppliername->setDisabled( false );
	customersuppliername->setText( "" );
	customersuppliername->setDisabled( true );

	qtytransaction->setDisabled( false );
	qtytransaction->setText( "" );
	qtytransaction->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,7}"), this) );
	
	transactiondate->setDate( QDate::currentDate() );
	transactiondate->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );	
	transactiondate->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	transactiontime->setTime( QTime::currentTime() );

	cboCustomerSupplierNo->setColumnCount( 3 );
	cboCustomerSupplierNo->setHorizontalHeaderItem( 0, tr("BGA")  );
	cboCustomerSupplierNo->setHorizontalHeaderItem( 1, tr("Name") );
	cboCustomerSupplierNo->setHorizontalHeaderItem( 2, tr("Location") );

	cboCustomerSupplierNo->setMinimumHeightList( 250 );
	cboCustomerSupplierNo->setVisible( false );
	cboCustomerSupplierNo->resizeColumnsToContents();
	cboCustomerSupplierNo->clearEditText();
	
	if( false == m_bInit )
	{
		m_cboLC2->setColumnCount( 2 );
		m_cboLC2->setHorizontalHeaderItem( 0, tr( "Transactionname" )		);
		m_cboLC2->setHorizontalHeaderItem( 1, tr( "Transactiontype" )		);
		m_cboLC2->addItem( 0, 0, tr( "Sell Off" )							);
		m_cboLC2->addItem( 0, 1, TransactionType::CUSTOMER_ORDER			);
		m_cboLC2->addItem( 1, 0, tr( "Incoming Delivery" )					);
		m_cboLC2->addItem( 1, 1, TransactionType::GOODSIN_DELIVERY			);
		m_cboLC2->addItem( 2, 0, tr( "Return" )								);
		m_cboLC2->addItem( 2, 1, TransactionType::CUSTOMER_RETURNS			);
		m_cboLC2->addItem( 3, 0, tr( "Return (supplier)" )					);
		m_cboLC2->addItem( 3, 1, TransactionType::SUPPLIER_RETURNS			);
		m_cboLC2->addItem( 4, 0, tr( "stock difference plus" )				);
		m_cboLC2->addItem( 4, 1, TransactionType::STOCK_DIFFERENCE_INCREASE	);	
		m_cboLC2->addItem( 5, 0, tr( "stock difference minus" )				);
		m_cboLC2->addItem( 5, 1, TransactionType::STOCK_DIFFERENCE_DECREASE	);
		m_cboLC2->addItem( 6, 0, tr( "Elimination" )						);
		m_cboLC2->addItem( 6, 1, TransactionType::DESTRUCTION				);
		m_cboLC2->addItem( 7, 0, ""											);
		m_cboLC2->addItem( 7, 1, ""											);

		m_bInit = true;
	}
	m_cboLC2->setCurrentIndex( m_cboLC2->count()-1 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \param	disable controls the state of the gui ctrils
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::setDisabled_grpNarcoticData( bool disable )
{
	grpNarcoticData->setDisabled( disable );
	
	group->setDisabled( true );
	group->setText( "" );
	name->setDisabled( true );
	name->setText( "" );
	packageunit->setDisabled( true );
	packageunit->setText( "" );
	concentration->setDisabled( true );
	concentration->setText( "" );
	narcfactor->setDisabled( true );
	narcfactor->setText( "" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \param	disable controls the state of the gui ctrils
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::setDisabled_grpArticleData( bool disable )
{
	grpArticleData->setDisabled( disable );
	
	articlecode->setDisabled( true );
	articlecode->setText( "" );
	articlepackageunit->setDisabled( true );
	articlepackageunit->setText( "" );
	articlename->setDisabled( true );
	articlename->setText( "" );
	articlepharmaform->setDisabled( true );
	articlepharmaform->setText( "" );
	articlestocklocation->setDisabled( true );
	articlestocklocation->setText( "" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return int-value of the methode QDialog::exec()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum CreateProcessNarcoticsBookingVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_DatasetSaved = false;	

	return basar::gui::tie::getWidgetReturnType( exec() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return bool-value of the method QDialog::close()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CreateProcessNarcoticsBookingVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// reset member
	m_MatchingDone = false;
	return close();
}

//---------------------------------------------------------------------------------------------------------------------//
// match methods
//---------------------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \param	branch passed AccessorPropertyTable_YIterator with branch specific data.\n 
//			It is set to an equivalent member.
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToBranch( basar::db::aspect::AccessorPropertyTable_YIterator branch )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		basar::ConstString msg = "matchToBranch of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_Branch = branch;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \param	aptNarcSupp passed AccessorPropertyTableRef, neccessary for creation of the local \n
//			narcotics supplier matcher. This method is called only one time, so the matcher could be local.
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToSuppliers( AccessorPropertyTableRef aptNarcSupp )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToSuppliers of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cboCustomerSupplierNo, aptNarcSupp );
	m_matSupplier.transformProperties( cboCustomerSupplierNo->horizontalHeaderLabels(), m_OriginalAttributeNames );
	m_matSupplier.RightToLeft();

	int cntColumn = cboCustomerSupplierNo->columnCount();
	int cntRows = cboCustomerSupplierNo->count();

	for( int i = 0; i < cntColumn; i++ ){
		cboCustomerSupplierNo->addItem( cntRows, i , " " );
	}
	
	cboCustomerSupplierNo->resizeColumnsToContents();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	currentBooking passed AccessorPropertyTable_YIterator for matching the current narcotics booking data.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToBookingData( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToBookingData of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	/* 
	----------------------------------------------------------------------------
	status	| allowed to change in	| effect on supplier combo
	----------------------------------------------------------------------------
	a		| 	d, y, z				| alle Lieferanten x; y; a (vorblenden)
	b		| 	-					| b (vorblenden)
	c		| 	x					| x; c (vorblenden)
	d		| 	z					| alle Lieferantren + z; d (vorblenden)
	x		| 	-					| x (vorblenden)
	y		| 	-					| y (vorblenden)
	z		| 	d					| alle Lieferanten; z (vorblenden)
	----------------------------------------------------------------------------
	*/

	QString strBgaNo = cboCustomerSupplierNo->currentText();
	const int iBgaNo = strBgaNo.toInt();
	if( iBgaNo != currentBooking.getInt32("customersupplierno") ){
		cboCustomerSupplierNo->setCurrentIndex( cboCustomerSupplierNo->count()-1 );
	}

	basar::gui::tie::Manager::getInstance().MatchFromIterator( this, currentBooking );
	chooseLineCodeDescription( currentBooking.getString( constants::TRANSACTIONTYPE ) );

	transactiontime->setTime( QTime( int( currentBooking.getInt32( constants::TRANSACTIONTIME ) / 10000 ), 
                                     int((currentBooking.getInt32( constants::TRANSACTIONTIME ) % 10000 ) / 100 ), 
                                     int( currentBooking.getInt32( constants::TRANSACTIONTIME ) % 100 ) ) );
	transactiontime->setDisplayFormat( "hh:mm:ss" );

	basar::Int32 startInternalBookingsNo = getStartInternalBookingsNo();
	if( -1 == startInternalBookingsNo ){
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	bgano passed int (basar::Int32&) reference for returning the current bga no to the caller.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchCurrentBgaNo( basar::Int32& bgano )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchCurrentBgaNo of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	bgano = atoi( customersupplierno->text().toLocal8Bit().data() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	narcoticArticleData passed AccessorPropertyTable_YIterator for displaying \n
//			the article specific data of the current narcotic booking.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToArticleNarcoticData( AccessorPropertyTable_YIterator narcoticArticleData )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleNarcoticData of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::gui::tie::Manager::getInstance().MatchFromIterator( this, narcoticArticleData );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	currentBooking will be filled with values of narcotic data specific frame
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToNarcoticData( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticData of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::Decimal narcPotency, narcFactor; 
	narcPotency.fromString(		concentration->text().toLocal8Bit().constData() );
	narcFactor.fromString(		narcfactor->text().toLocal8Bit().constData() );
	currentBooking.setString(	constants::NARCGROUP,		group->text().toLocal8Bit().constData()			);
	currentBooking.setString(	constants::NARCNAME,		name->text().toLocal8Bit().constData()			);
	currentBooking.setString(	constants::NARCUNIT,		packageunit->text().toLocal8Bit().constData()	);
	currentBooking.setDecimal(	constants::NARCPOTENCY,		narcPotency									);
	currentBooking.setDecimal(	constants::NARCFACTOR,		narcFactor									);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::Int32 CreateProcessNarcoticsBookingVC::getStartInternalBookingsNo()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getStartInternalBookingsNo of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::I18nString bgaNo;
	if( false == basar::login::Manager::getInstance().getParaValue( UADM_START_INTERNAL_BOOKINGS_NO, bgaNo ) )
	{
		basar::gui::tie::criticalMsgBox( this,
			QApplication::translate("CreateProcessNarcoticsBookingVC", 
									"No parameter data set entry for 'START_INTERNAL_BOOKINGS_NO' found!").toLocal8Bit().data() );
		return -1;
	}

	return atoi( bgaNo.c_str() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	i not used in this context
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::currentIndexChangedOfCustomerSupplier( int i )
{
	i; // avoid compiler warning;
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "currentIndexChangedOfCustomerSupplier of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// update the edit field behind (not always visible) the comboBox
	customersupplierno->setText( cboCustomerSupplierNo->currentText() );
	customersuppliername->setText( cboCustomerSupplierNo->itemText( cboCustomerSupplierNo->currentIndex(), 1 ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::db::aspect::AccessorPropertyTable_YIterator CreateProcessNarcoticsBookingVC::getBranch()
{
	if( false == m_bBranchLoaded )
	{
		basar::appl::SystemEventManager::getInstance().fire( "FindBranchData" );
		m_bBranchLoaded = true;
	}

	return m_Branch;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	i not used in this context
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::currentIndexChangedOfLineCode( int i )
{
	i; // avoid compiler warning;
	if (LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::ConstString msg = "currentIndexChangedOfLineCode of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QString lc; lc = m_cboLC2->itemText( m_cboLC2->currentIndex(), 1 );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// internal account
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	basar::VarString custsuppno;
	if( ( TransactionType::STOCK_DIFFERENCE_INCREASE == lc ) ||
		( TransactionType::STOCK_DIFFERENCE_DECREASE == lc ) ||
		( TransactionType::DESTRUCTION == lc ) )
	{
		// make combobox invisible and the the customersupplier edit field visible
		cboCustomerSupplierNo->setVisible( false );
		customersupplierno->setVisible( true );	
		custsuppno.format( "%ld", getBranch().getInt32( constants::BGANO ) );
		customersupplierno->setText( QString::fromLocal8Bit(custsuppno.c_str()) );
		customersuppliername->setText( m_cboLC2->itemText( m_cboLC2->currentIndex(), 0 ) );
	} 
	else if( ( TransactionType::CUSTOMER_ORDER == lc ) || ( TransactionType::CUSTOMER_RETURNS == lc ) )
	{
		cboCustomerSupplierNo->setVisible( false );
		customersupplierno->setVisible( true );
		// clear customersuppliername and -no
		if( m_MatchingDone )
		{
			customersuppliername->setText("");
			customersupplierno->setText("");
		}
	} 
	else if( ( TransactionType::GOODSIN_DELIVERY == lc ) || ( TransactionType::SUPPLIER_RETURNS == lc ) )
	{
		cboCustomerSupplierNo->setVisible( true );
		customersupplierno->setVisible( false );
		
		// important to save the customersupplierno in an extra variable, because after the 'loadCombo' it is empty
		QString sn = customersupplierno->text();
		// load narcotics suppliers
		loadCombo();
		
		// display current supplier
		int idx;
		if( -1 == ( idx = cboCustomerSupplierNo->findText( sn ) ) )
		{
			// messageBox
			std::ostringstream os;
			os << QApplication::translate("CreateProcessNarcoticsBookingVC", "The current supplier (supplier no.: ").toLocal8Bit().data() 
			   << sn.toLocal8Bit().data()
			   << QApplication::translate(	"CreateProcessNarcoticsBookingVC", 
											" ) was not found among the narcotics suppliers!").toLocal8Bit().data();
			basar::gui::tie::infoMsgBox( this, os.str().c_str(), tr("Narcotics").toLocal8Bit().constData() );
			cboCustomerSupplierNo->clearEditText();
			customersuppliername->setText("");
		} 
		else 
		{
			cboCustomerSupplierNo->setCurrentIndex( idx );
			customersupplierno->setText("");
			customersuppliername->setText( cboCustomerSupplierNo->itemText( cboCustomerSupplierNo->currentIndex(), 1 ) );
		}

		if( m_MatchingDone )
		{
			customersuppliername->setText("");
			customersupplierno->setText("");
		}
	}

	m_MatchingDone = true;
	btnSave->setDisabled( !savingAllowed() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	oss passed std::ostringstream& which the data are assigned to.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::toStream( std::ostringstream& oss )
{
	oss		<< "purchaseorderno: >"			<< purchaseorderno->text().toLocal8Bit().data()					<< "<, "
			<< "qtytransaction: >"			<< qtytransaction->text().toLocal8Bit().data()					<< "<, "
			<< "m_cboLC2: >"				<< m_cboLC2->currentText().toLocal8Bit().data()					<< "<, "
			<< "customersupplierno: >"		<< customersupplierno->text().toLocal8Bit().data()				<< "<, "
			<< "customersuppliername: >"	<< customersuppliername->text().toLocal8Bit().data()			<< "<, "
			<< "transactiondate: >"			<< transactiondate->text().toLocal8Bit().data()					<< "<, "
			<< "transactiontime: >"			<< transactiontime->text().toLocal8Bit().data()					<< "<, "
			<< "cboCustomerSupplierNo: >"	<< cboCustomerSupplierNo->currentText().toLocal8Bit().data()	<< "<;"; 		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	currentBooking passed AccessorPropertyTable_YIterator representing the current narcotics booking\n
//			which the current data in the dialog are assigned to.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchFromCurrentBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromCurrentBooking of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
#ifndef NDEBUG
	std::ostringstream oss;
	toStream( oss );
	currentBooking.toStream( oss );
#endif

	basar::gui::tie::Manager::getInstance().MatchToIterator( this, currentBooking );

	int i = m_cboLC2->currentIndex();
	QString txt  = m_cboLC2->itemText( i, 1 );
	currentBooking.setString( constants::TRANSACTIONTYPE, txt.toLocal8Bit().data() );
	currentBooking.setInt32( constants::TRANSACTIONTIME, transactiontime->time().hour() * 10000 +
												transactiontime->time().minute() * 100 +
												transactiontime->time().second() );

    transactiontime->setTime( QTime( int(  currentBooking.getInt32( constants::TRANSACTIONTIME ) / 10000 ), 
                                     int( (currentBooking.getInt32( constants::TRANSACTIONTIME ) % 10000 ) / 100), 
                                     int(  currentBooking.getInt32( constants::TRANSACTIONTIME) % 100 ) ) );
    transactiontime->setDisplayFormat( "hh:mm:ss" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	currentBooking passed AccessorPropertyTable_YIterator representing a new narcotics booking\n
//			which the current data in the dialog are assigned to.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToNewBooking( AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNewBooking of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::gui::tie::Manager::getInstance().MatchFromIterator( this, currentBooking );
// needed to prevent transactiondate and time from being set to maxvalue
	if(	currentBooking.contains( constants::TRANSACTIONDATE ) && 
	  ( currentBooking.getState( constants::TRANSACTIONDATE ) == basar::SS_UNSET ) )
	{
		transactiondate->setDate( QDate::currentDate() );
	}

	if(	currentBooking.contains( constants::TRANSACTIONTIME ) && 
	  ( currentBooking.getState( constants::TRANSACTIONTIME ) == basar::SS_UNSET ) )
	{
		transactiontime->setTime( QTime::currentTime() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	searchIt passed AccessorPropertyTable_YIterator for searching a new customer or supplier\n
//			with the here set customersupplierno and transactiontype.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchFromNarcoticCustomerSupplierNo( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromNarcoticCustomerSupplierNo of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	searchIt.setString( constants::CUSTOMERSUPPLIERNO,	customersupplierno->text().toLocal8Bit().constData() );
	searchIt.setString( constants::TRANSACTIONTYPE,		
						m_cboLC2->itemText( m_cboLC2->currentIndex(), 1 ).toLocal8Bit().constData() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	supplier passed AccessorPropertyTable_YIterator which the sutomersuppliername is displayed of.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToNarcoticCustomerSupplierName( AccessorPropertyTable_YIterator supplier )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticCustomerSupplierName of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	customersuppliername->setText( QString::fromLocal8Bit(supplier.getString( "customersuppliername" ).c_str()) );
}

//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
// ctrl events
//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onBtnSave_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSave_clicked of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SaveBooking" );
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
	
	if( false == m_DatasetSaved ){
		m_DatasetSaved = saved;
	}

	shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	str not used here
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onCustomersupplierno_textChanged( const QString& str )
{
	str;
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onCustomersupplierno_textChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( customersupplierno->isModified() )
	{
		QString lc; lc = m_cboLC2->itemText( m_cboLC2->currentIndex(), 1 );
		if( !( ( TransactionType::STOCK_DIFFERENCE_INCREASE == lc ) ||
			   ( TransactionType::STOCK_DIFFERENCE_DECREASE == lc ) ||
			   ( TransactionType::DESTRUCTION == lc )				) )
		{
			customersuppliername->setText( "" );
		}
	}
	btnSave->setDisabled( !savingAllowed() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::loadCustomer()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "loadCustomer of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	//	this method is declared as a slot of this dialog and will be invoked if the event 'returnPressed()'
	//	is emitted of the line edit ctrl 'customersupplierno'.
	QString lc = m_cboLC2->itemText( m_cboLC2->currentIndex(), 1 );
	/////////////////////////////////////////////////////////////////////////
	// internal account
	/////////////////////////////////////////////////////////////////////////

	basar::appl::EventReturnStruct ret; 	
    if( (TransactionType::CUSTOMER_ORDER == lc) || (TransactionType::CUSTOMER_RETURNS == lc) ){
		ret = basar::appl::SystemEventManager::getInstance().fire( "FindCustomer" );		
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	customer passed object which namne is set to the customersuppliername line edit.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onCustomersupplierno_editingFinished()
{
	if( customersupplierno->text() != "" ){
		loadCustomer();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	customer passed object which namne is set to the customersuppliername line edit.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCustomer of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	customersuppliername->setText( QString::fromLocal8Bit(customer.getString(constants::CUSTOMERNAME).c_str()) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	packageunit for narcotic group is passed within the iterator to put it into packageunit line edit.
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::matchToPackageunit( basar::db::aspect::AccessorPropertyTable_YIterator narcoticDataIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToPackageunit of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( !narcoticDataIt.isEnd() )
	{
		if( narcoticDataIt.contains( constants::PACKAGEUNIT ) && 
			!( narcoticDataIt.getState( constants::PACKAGEUNIT ) == basar::SS_UNSET ) )
		{
			packageunit->setText( QString::fromLocal8Bit(narcoticDataIt.getString( constants::PACKAGEUNIT ).c_str()) );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if one of the search criteria is set (orderno, articleno, articlename, supplierno or suppliername)
//!	\retval false otherwise
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CreateProcessNarcoticsBookingVC::savingAllowed()
{	
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "savingAllowed of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	bool isSavinAllowed = ( ( !purchaseorderno->text().isEmpty() ) && ( !qtytransaction->text().isEmpty() ) && 
							( !m_cboLC2->currentText().isEmpty() ) && ( !customersuppliername->text().isEmpty() ) && 
							( !customersupplierno->text().isEmpty() || !cboCustomerSupplierNo->currentText().isEmpty() ) &&
							( !purchaseorderno->text().isEmpty() ) && ( !group->text().isEmpty() ) && ( !name->text().isEmpty() ) && 
							( !packageunit->text().isEmpty() ) && ( !concentration->text().isEmpty() ) && ( !narcfactor->text().isEmpty() ) && 
							( !articlecode->text().isEmpty() ) && ( !articlepackageunit->text().isEmpty() ) && ( !articlename->text().isEmpty() ) &&
							( !articlestocklocation->text().isEmpty() ) );

	return isSavinAllowed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onPurchaseorderno_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onPurchaseorderno_textChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onQtytransaction_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onQtytransaction_textChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onM_cboLC2_currentIndexChanged( int index )
{
	index;
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onM_cboLC2_textChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onCustomersuppliername_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onCustomersuppliername_textChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onTransactiondate_dateChanged(const QDate&)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTransactiondate_dateChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CreateProcessNarcoticsBookingVC::onTransactiontime_timeChanged(const QTime&)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTransactiontime_timeChanged of CreateProcessNarcoticsBookingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSave->setEnabled( savingAllowed() );
}

} // namespace viewConn
} // namespace narcotics
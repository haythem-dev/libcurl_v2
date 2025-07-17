//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "transactiondatavc.h"
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
TransactionDataVC::TransactionDataVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
TransactionDataVC::~TransactionDataVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::wireEvents()
{
	QObject::connect( checkBoxArticle,		SIGNAL( stateChanged( int )  ), this, SLOT( combineSearchCriteria()					));	
	QObject::connect( checkBoxCustomer,		SIGNAL( stateChanged( int )  ), this, SLOT( combineSearchCriteria()					));	
	QObject::connect( checkBoxSupplier,		SIGNAL( stateChanged( int )  ), this, SLOT( combineSearchCriteria()					));	
	QObject::connect( checkBoxArticleName,	SIGNAL( stateChanged( int )  ), this, SLOT( activateSearchForArticleName()			));	
	QObject::connect( checkBoxCustomerName,	SIGNAL( stateChanged( int )  ), this, SLOT( activateSearchForCustomerName()			));	
	QObject::connect( checkBoxSupplierName,	SIGNAL( stateChanged( int )  ), this, SLOT( activateSearchForSupplierName()			));

	QObject::connect( btnClose,				SIGNAL( clicked()			 ), this, SLOT( onBtnClose_clicked()					));
	QObject::connect( chkWithDate,			SIGNAL( stateChanged( int )	 ), this, SLOT( onChkWithDate_stateChanged( int )		));
	QObject::connect( txtArticleCode,		SIGNAL( textChanged(QString) ), this, SLOT( onTxtArticleCode_textChanged(QString)	));
	QObject::connect( txtArticleName,		SIGNAL( textChanged(QString) ), this, SLOT( onTxtArticleName_textChanged(QString)	));
	QObject::connect( txtBgaNo,				SIGNAL( textChanged(QString) ), this, SLOT( onTxtBgaNo_textChanged(QString)			));
	QObject::connect( txtCustomerName,		SIGNAL( textChanged(QString) ), this, SLOT( onTxtCustomerName_textChanged(QString)	));
	QObject::connect( txtSupplierNo,		SIGNAL( textChanged(QString) ), this, SLOT( onTxtSupplierNo_textChanged(QString)	));
	QObject::connect( txtSupplierName,		SIGNAL( textChanged(QString) ), this, SLOT( onTxtSupplierName_textChanged(QString)	));
	QObject::connect( btnSearch,			SIGNAL( clicked()			 ), this, SLOT( onBtnSearch_clicked()					));
	QObject::connect( btnPrint,				SIGNAL( clicked()			 ), this, SLOT( onBtnPrint_clicked()					));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	void
	\param	narcoticsBookings is the AccessorPropertyTable necessary to create the matcher
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::init( AccessorPropertyTableRef narcoticsBookings )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initList();
	initGuiCtrls();

	using basar::gui::tie::Manager;
	// creation of the matcher
	m_matNarcoticBookings = Manager::getInstance().createMatcher( grpTransactionData, narcoticsBookings );
	m_matNarcoticBookings.transformProperties( m_lstCaptionsPZN, m_OriginalAttributeNamesPZN );

	m_AccessorPropertyTableRef = narcoticsBookings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::initList()
{
	// bga no specific column headers
	m_lstCaptionsBgaNo.push_back( tr( "Date" ).toLocal8Bit().data()				);
	m_lstCaptionsBgaNo.push_back( tr( "Article No." ).toLocal8Bit().data()		);
	m_lstCaptionsBgaNo.push_back( tr( "Package Unit" ).toLocal8Bit().data()		);
	m_lstCaptionsBgaNo.push_back( tr( "Article Name" ).toLocal8Bit().data()		);
	m_lstCaptionsBgaNo.push_back( tr( "PharmaForm" ).toLocal8Bit().data()		);
	m_lstCaptionsBgaNo.push_back( tr( "Record Type" ).toLocal8Bit().data()		);
	m_lstCaptionsBgaNo.push_back( tr( "Quantity" ).toLocal8Bit().data()			);
	m_lstCaptionsBgaNo.push_back( tr( "PurchaseOrderNo" ).toLocal8Bit().data()	);

	m_OriginalAttributeNamesBgaNo.push_back( constants::TRANSACTIONDATE		);
	m_OriginalAttributeNamesBgaNo.push_back( constants::ARTICLECODE			);
	m_OriginalAttributeNamesBgaNo.push_back( constants::PACKAGEUNIT			);
	m_OriginalAttributeNamesBgaNo.push_back( constants::ARTICLENAME			);
	m_OriginalAttributeNamesBgaNo.push_back( constants::PHARMAFORM			);
	m_OriginalAttributeNamesBgaNo.push_back( constants::TRANSACTIONTYPE		);
	m_OriginalAttributeNamesBgaNo.push_back( constants::QTYTRANSACTION		);
	m_OriginalAttributeNamesBgaNo.push_back( constants::PURCHASEORDERNO		);

	// pzn specific column headers
	m_lstCaptionsPZN.push_back( tr( "Date").toLocal8Bit().data()			   );
	m_lstCaptionsPZN.push_back( tr( "Bga-No").toLocal8Bit().data()			   );
	m_lstCaptionsPZN.push_back( tr( "Customer/Supplier Name").toLocal8Bit().data()	   );
	m_lstCaptionsPZN.push_back( tr( "Customer/Supplier Location").toLocal8Bit().data()	   );
	m_lstCaptionsPZN.push_back( tr( "Record Type").toLocal8Bit().data()		   );
	m_lstCaptionsPZN.push_back( tr( "Purchase Order No.").toLocal8Bit().data() );
	m_lstCaptionsPZN.push_back( tr( "Quantity").toLocal8Bit().data()		   );
	m_lstCaptionsPZN.push_back( tr( "Tel No.").toLocal8Bit().data()			   );
	m_lstCaptionsPZN.push_back( tr( "Quick Dialing").toLocal8Bit().data()	   );

	m_OriginalAttributeNamesPZN.push_back( constants::TRANSACTIONDATE			);
	m_OriginalAttributeNamesPZN.push_back( constants::CUSTOMERSUPPLIERNO		);
	m_OriginalAttributeNamesPZN.push_back( constants::CUSTOMER_SUPPLIER_NAME	);
	m_OriginalAttributeNamesPZN.push_back( constants::CUSTOMER_SUPPLIER_LOCATION);
	m_OriginalAttributeNamesPZN.push_back( constants::TRANSACTIONTYPE			);
	m_OriginalAttributeNamesPZN.push_back( constants::PURCHASEORDERNO			);
	m_OriginalAttributeNamesPZN.push_back( constants::QTYTRANSACTION			);
	m_OriginalAttributeNamesPZN.push_back( constants::TELNO						);
	m_OriginalAttributeNamesPZN.push_back( constants::QUICKDIALING				);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::initGuiCtrls()
{
	checkBoxArticle->setChecked( false );
	checkBoxCustomer->setChecked( false );
	checkBoxSupplier->setChecked( false );
	cbAlsoLockedCust->setChecked( false ); //default: don´t show locked customers

	if( 0 == txtBgaNo->validator() ){
		txtBgaNo->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,7}"), this) );
	}

	if( 0 == txtArticleCode->validator() ){
		txtArticleCode->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,8}"), this) );
	}
	
	if( 0 == txtSupplierNo->validator() ){
		txtSupplierNo->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,7}"), this) );
	}

	// disable specific frames containing search criteria
	setDisabled_frameArticleSearch ( true );
	setDisabled_frameCustomerSearch( true );
	setDisabled_frameSupplierSearch( true );

	grpFromTo->setDisabled( true );
	dtFrom->setDate( QDate::currentDate() );
	dtFrom->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtFrom->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	dtTo->setDate( QDate::currentDate() );
	dtTo->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtTo->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	
	btnSearch->setDisabled( true );
	btnPrint->setDisabled(  true );

	// qtablewidget
	tblTransData->setDisabled( true );
	tblTransData->clear();
	tblTransData->hideVerticalHeader();
	
	tblTransData->setColumnCount( static_cast<int>(m_lstCaptionsPZN.size()) );
	QStringList lst;
	std::list<basar::VarString>::const_iterator it = m_lstCaptionsPZN.begin();
	for( ; it != m_lstCaptionsPZN.end(); it++ ){
		lst << QString::fromLocal8Bit((*it).c_str());
	}
	tblTransData->setHorizontalHeaderLabels( lst );
	
	for( int col = 0; col < tblTransData->columnCount(); col++ ){
		tblTransData->setEditableColumn( col, false );	
	}

	// prevent cells of table tblTransData from being editable
	tblTransData->setEditTriggers( QAbstractItemView::NoEditTriggers );

	cbOrdertype->setCurrentIndex( 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	int indicates the value returned by a modal dialog. 
			Value '1' represents QDialog::Accepted and value '0' represents QDialog::Rejected.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum TransactionDataVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true by default
			element of the erased one.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
bool TransactionDataVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matNarcoticBookings.reset();
	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\param	searchIt represents the AccessorPropertyTable_YIterator that is search criteria set to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSelection of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	bool articleNoSet		= false;
	bool articleNameSet		= false;
	bool bgaNoSet			= false;
	bool customerNameSet	= false;
	bool supplierNoSet		= false;
	bool supplierNameSet	= false;

	searchIt.setString( constants::SHOW_LOCKED_CUST, "0" );
	
	if( !txtArticleCode->text().isEmpty() )
	{
		searchIt.setString( constants::ARTICLECODE, txtArticleCode->text().toLocal8Bit().constData() );
		articleNoSet = true;
	}
	
	if( !txtArticleName->text().isEmpty() )
	{
		basar::I18nString searchname = txtArticleName->text().toLocal8Bit().constData();
		searchname.upper();
		searchIt.setString( constants::ARTICLENAME, searchname );
		articleNameSet = true;
	}

	if( !txtBgaNo->text().isEmpty() )
	{
		searchIt.setString( constants::CUSTOMERSUPPLIERNO, txtBgaNo->text().toLocal8Bit().constData() );
		//\! todo Why should not be differentiated here between customer's and supplier's transactiontype.
		// attention:	here the attribute 'customersuppliername' of the search iterator is misused, 
		//				because more than one transactiontype has to used in the sql clause.	
		searchIt.setString( constants::TRANSACTIONTYPE, TransactionType::CUSTOMER_ORDER );
		searchIt.setString( constants::CUSTOMER_SUPPLIER_NAME, TransactionType::CUSTOMER_RETURNS );
		bgaNoSet = true;
	} 

	if( !txtCustomerName->text().isEmpty() )
	{
		basar::I18nString searchname = txtCustomerName->text().toLocal8Bit().constData();
		searchname.upper();
		searchIt.setString( constants::CUSTOMERNAME, searchname );
		customerNameSet = true;

		if (cbAlsoLockedCust->isChecked())
			searchIt.setString( constants::SHOW_LOCKED_CUST, "1" );
	}

	if( !txtSupplierNo->text().isEmpty() )
	{
			searchIt.setString( constants::CUSTOMERSUPPLIERNO, txtSupplierNo->text().toLocal8Bit().constData() );
			searchIt.setString( constants::TRANSACTIONTYPE, TransactionType::GOODSIN_DELIVERY );
			searchIt.setString( constants::CUSTOMER_SUPPLIER_NAME, "d" );
			supplierNoSet = true;
	}

	if( !txtSupplierName->text().isEmpty() )
	{
		basar::I18nString searchname = txtSupplierName->text().toLocal8Bit().constData();
		searchname.upper();
		searchIt.setString( constants::SUPPLIERNAME, searchname );
		supplierNameSet = true;
	}

	using basar::gui::tie::Manager;
	m_matNarcoticBookings.reset();
    QStringList lst;
    std::list<basar::VarString>::const_iterator it;
    std::list<basar::VarString>* pList         = &m_lstCaptionsPZN;
    std::list<basar::VarString>* pListOriginal = &m_OriginalAttributeNamesPZN;

	basar::I18nString transactiontype = cbOrdertype->currentText().toLocal8Bit().constData();

// ToDo: maybe the use of only one caption-headerlabelcombination must be revised
	if( bgaNoSet || customerNameSet || supplierNoSet || supplierNameSet || transactiontype == tr("Destruction").toLocal8Bit().constData())
	{	
		pList         = &m_lstCaptionsBgaNo;
        pListOriginal = &m_OriginalAttributeNamesBgaNo;
	}	
	else 
	{
        pList         = &m_lstCaptionsPZN;
        pListOriginal = &m_OriginalAttributeNamesPZN;
	}

    it = pList->begin();
	for( ; it != pList->end(); it++ ){
		lst << QString::fromLocal8Bit((*it).c_str());
	}
	tblTransData->setColumnCount( static_cast<int>( pList->size() ) );
	tblTransData->setHorizontalHeaderLabels( lst );

    m_matNarcoticBookings = Manager::getInstance().createMatcher( grpTransactionData, m_AccessorPropertyTableRef );
	
    m_matNarcoticBookings.transformProperties( *pList, *pListOriginal );

	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().constData() );
	m_matNarcoticBookings.push_back( dateTransformer );

    // search criteria 'with date'
	if( chkWithDate->isChecked() )
	{
		searchIt.setString( constants::FROMDATE, dtFrom->date().toString( "yyyyMMdd" ).toLocal8Bit().constData() );
		searchIt.setString( constants::TODATE,	 dtTo->date().toString( "yyyyMMdd" ).toLocal8Bit().constData()	 );
	}

	if( transactiontype == tr("Sale").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::CUSTOMER_ORDER );
	}
	else if( transactiontype == tr("Received Goods").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::GOODSIN_DELIVERY );
	}
	else if( transactiontype == tr("Customer Returns").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::CUSTOMER_RETURNS );
	}
	else if( transactiontype == tr("Supplier Returns").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::SUPPLIER_RETURNS );
	}
	else if( transactiontype == tr("Destruction").toLocal8Bit().constData() ){
		searchIt.setString( constants::TRANSACTIONTYPE,	TransactionType::DESTRUCTION );
	}
	else{
		searchIt.setPropertyState( constants::TRANSACTIONTYPE,	basar::SS_UNSET	);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::matchToNarcoticBookings()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticBookings of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	tblTransData->setDisabled( false );
	tblTransData->clearContext();
	m_matNarcoticBookings.RightToLeft( 0, tblTransData->rowCount() );
	tblTransData->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::matchToArticleNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleNo of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// disable checkBoxName in order to clear it and enable txtArticleNo which can therefore now be filled with
	// passed articleNoValue.
	checkBoxArticleName->setChecked( false );
	if( yit.isContainedAndSet( constants::ARTICLECODE ) ){
		txtArticleCode->setText( QString::fromLocal8Bit(yit.getString( constants::ARTICLECODE ).c_str() ));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::matchToCustomerNo( basar::I18nString customerNoValue )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCustomerNo of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	checkBoxCustomerName->setChecked( false );
	txtBgaNo->setText( QString::fromLocal8Bit(customerNoValue.c_str()) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::matchToSupplierNo( basar::I18nString supplierNoValue )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToSupplierNo of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	checkBoxSupplierName->setChecked( false );
	txtSupplierNo->setText( QString::fromLocal8Bit(supplierNoValue.c_str()) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( chkWithDate->isChecked() && ( dtFrom->date() > dtTo->date() ) )
	{
		basar::gui::tie::infoMsgBox( this,
			tr("To date must be greater than or equal to from date!").toLocal8Bit().constData(),
			tr("Narcotics").toLocal8Bit().constData() );
		return;
	}

	setCursor( Qt::WaitCursor );	
	tblTransData->clearContext();
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchGoodsTransaction" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
	}
	btnPrint->setEnabled( isPrintAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onBtnPrint_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnPrint_clicked of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "PrintGoodsTransaction" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtBgaNo_textChanged( const QString& )
{ 
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtBgaNo_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
	btnPrint->setEnabled ( isPrintAllowed()  );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtCustomerName_textChanged( const QString& )
{ 
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtCustomerName_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtArticleCode_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtArticleNo_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
	btnPrint->setEnabled ( isPrintAllowed()  );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtArticleName_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtArticleName_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtSupplierNo_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtSupplierNo_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
	btnPrint->setEnabled ( isPrintAllowed()  );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onTxtSupplierName_textChanged( const QString& )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onTxtSupplierName_textChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	iToggle is used to signalise if the CheckBox is set or not.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onChkWithDate_stateChanged( int iToggle )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onChkWithDate_stateChanged of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	btnSearch->setEnabled( isSearchAllowed() );
	btnPrint->setEnabled ( isPrintAllowed()  );

	bool enable = false;
	if( iToggle == Qt::Checked ){
		enable = true;
	}
	
	grpFromTo->setEnabled( enable );
	lblFrom->setEnabled	 ( enable );
	lblTo->setEnabled	 ( enable );
	dtFrom->setEnabled	 ( enable );
	dtTo->setEnabled	 ( enable );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if one of the search criteria is set (articleno, customerno/supplierno or date)
//!	\retval false otherwise
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool TransactionDataVC::isSearchAllowed()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "isSearchAllowed of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return	( !txtArticleCode->text().isEmpty()  )	|| 
			( !txtArticleName->text().isEmpty()  )	|| 
			( !txtBgaNo->text().isEmpty()		 )	|| 
			( !txtCustomerName->text().isEmpty() )	|| 
			( !txtSupplierNo->text().isEmpty()	 )	|| 
			( !txtSupplierName->text().isEmpty() )	|| 
			(  chkWithDate->isChecked()			 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if one of the search criteria is set (articleno, customerno/supplierno)
//!	\retval false otherwise
//!	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool TransactionDataVC::isPrintAllowed()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "isPrintAllowed of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return !m_matNarcoticBookings.getRight().empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::setDisabled_frameArticleSearch( bool disable )
{
	bool searchByName = checkBoxArticleName->isChecked();

	if ( true == disable )
	{	
		checkBoxArticleName->setDisabled( true );
		checkBoxArticleName->setCheckState( Qt::Unchecked );
		txtArticleName->setDisabled( true );
		txtArticleName->setText( "" );
		lblArticleNo->setDisabled( true );
		txtArticleCode->setDisabled( true );
		txtArticleCode->setText( "" );
	}
	else
	{
		checkBoxArticleName->setDisabled( false );
		lblArticleNo->setDisabled( false );		

		if ( true == searchByName )
		{
			txtArticleCode->setText( "" );
			txtArticleCode->setDisabled( true );
			txtArticleName->setDisabled( false );
			txtArticleName->setFocus();
		}
		else
		{
			txtArticleCode->setDisabled( false );
			txtArticleName->setDisabled( true );
			txtArticleName->setText( "" );
			txtArticleCode->setFocus();
			
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::setDisabled_frameCustomerSearch( bool disable )
{
	bool searchByName = checkBoxCustomerName->isChecked();

	if ( true == disable )
	{
		lblBgaNo->setDisabled( true );
		checkBoxCustomerName->setCheckState( Qt::Unchecked );
		checkBoxCustomerName->setDisabled( true );
		txtBgaNo->setDisabled( true );
		cbAlsoLockedCust->setDisabled( true );
		txtBgaNo->setText( "" );
		txtCustomerName->setDisabled( true );
		txtCustomerName->setText( "" );
	}
	else
	{
		checkBoxCustomerName->setDisabled( false );
		lblBgaNo->setDisabled( false );
		cbAlsoLockedCust->setDisabled( false );

		if ( true == searchByName )
		{
			txtBgaNo->setText( "" );
			txtBgaNo->setDisabled( true );
			txtCustomerName->setDisabled( false );
			txtCustomerName->setFocus();
		}
		else
		{
			txtBgaNo->setDisabled( false );
			txtCustomerName->setDisabled( true );
			txtCustomerName->setText( "" );
			txtBgaNo->setFocus();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	disable switches the control to active/inactive.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::setDisabled_frameSupplierSearch( bool disable )
{
	bool searchByName = checkBoxSupplierName->isChecked();

	if ( true == disable )
	{
		lblSupplierNo->setDisabled( true );
		checkBoxSupplierName->setCheckState( Qt::Unchecked );
		checkBoxSupplierName->setDisabled( true );
		txtSupplierNo->setDisabled( true );
		txtSupplierNo->setText( "" );
		txtSupplierName->setDisabled( true );
		txtSupplierName->setText( "" );
	}
	else
	{
		checkBoxSupplierName->setDisabled( false );
		lblSupplierNo->setDisabled( false );
		
		if ( true == searchByName )
		{
			txtSupplierNo->setText( "" );
			txtSupplierNo->setDisabled( true );
			txtSupplierName->setDisabled( false );
			txtSupplierName->setFocus();
		}
		else
		{
			txtSupplierNo->setDisabled( false );
			txtSupplierName->setDisabled( true );
			txtSupplierName->setText( "" );
			txtSupplierNo->setFocus();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::activateSearchForArticleName()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "activateSearchForArticleName of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	} 

// decide whether a search for articlename or articleno is valid
	if( checkBoxArticleName->isChecked() )
	{
		txtArticleCode->setDisabled( true );
		txtArticleName->setDisabled( false );
		txtArticleCode->setText( "" );
		txtArticleName->setFocus();
	}
	else 
	{
		txtArticleCode->setDisabled( false );
		txtArticleName->setDisabled( true );
		txtArticleCode->setFocus();
		txtArticleName->setText( "" );		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::activateSearchForSupplierName()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "activateSearchForSupplierName of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	} 

// decide whether a search for articlename or articleno is valid
	if( checkBoxSupplierName->isChecked() )
	{
		txtSupplierName->setDisabled( false );
		txtSupplierNo->setDisabled( true );
		txtSupplierName->setFocus();
		txtSupplierNo->setText( "" );
	}
	else 
	{
		txtSupplierName->setDisabled( true );
		txtSupplierNo->setDisabled( false );
		txtSupplierName->setText( "" );
		txtSupplierNo->setFocus();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::activateSearchForCustomerName()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "activateSearchForCustomerName of TransactionDataVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	} 

// decide whether a search for articlename or articleno is valid
	if( checkBoxCustomerName->isChecked() )
	{
		txtBgaNo->setDisabled( true );
		txtCustomerName->setDisabled( false );
		txtBgaNo->setText( "" );
		txtCustomerName->setFocus();
	}
	else 
	{
		txtBgaNo->setDisabled( false );
		txtCustomerName->setDisabled( true );
		txtBgaNo->setFocus();
		txtCustomerName->setText( "" );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void TransactionDataVC::combineSearchCriteria()
{
    basar::ConstString fun = "TransactionDataVC::combineSearchCriteria()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );
    
	if( checkBoxArticle->isChecked() ){
		setDisabled_frameArticleSearch( false );
	}
	else{
		setDisabled_frameArticleSearch( true );
	}

	if( checkBoxCustomer->isChecked() )
	{
		setDisabled_frameCustomerSearch( false );
	// disable search for supplier
		checkBoxSupplier->setDisabled( true );
		setDisabled_frameSupplierSearch( true );
	}
	else 
	{
		checkBoxSupplier->setDisabled( false );
		setDisabled_frameCustomerSearch( true );
	}

	if( checkBoxSupplier->isChecked() )
	{
		setDisabled_frameSupplierSearch( false );
	// disable search for customer
		checkBoxCustomer->setDisabled( true );
		setDisabled_frameCustomerSearch( true );
	}
	else 
	{
		checkBoxCustomer->setDisabled( false );
		setDisabled_frameSupplierSearch( true );
	}

	btnPrint->setEnabled( isPrintAllowed() );
}

} //namespace viewConn
} //namespace narcotics
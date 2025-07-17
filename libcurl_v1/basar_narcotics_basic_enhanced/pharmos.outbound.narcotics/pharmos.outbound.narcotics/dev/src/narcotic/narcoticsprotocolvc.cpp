//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "narcoticsprotocolvc.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//--------------------------------------------------------------------------------------------------------------//
//! Constructor of the View Connector "Narcotics Protocol".
/*! A Translator for the dialog is installed. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
NarcoticsProtocolVC::NarcoticsProtocolVC( QWidget *parent )
: QDialog( parent )
{
    setupUi(this);
	wireEvents();
	initGuiCtrls();
}

//--------------------------------------------------------------------------------------------------------------//
//! Destructor of the View Connector "Narcotics Protocol".
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
NarcoticsProtocolVC::~NarcoticsProtocolVC()
{
}

//--------------------------------------------------------------------------------------------------------------//
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::wireEvents()
{
	QObject::connect(	btnClose,		SIGNAL(	clicked()							),	
						this,			SLOT  (	onBtnClose_clicked()				));
	QObject::connect(	btnPrint,		SIGNAL(	clicked()							),	
						this,			SLOT  (	onBtnPrint_clicked()				));
	QObject::connect(	btnSearch,		SIGNAL(	clicked()							),	
						this,			SLOT  (	onBtnSearch_clicked()				));
	QObject::connect(	twCatalog,		SIGNAL(	itemSelectionChanged()				),	
						this,			SLOT  (	onTwCatalog_itemSelectionChanged()	));
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that shows the dialog for Narcotic Changes.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum NarcoticsProtocolVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that hides the dialog for Narcotic Changes.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that closes and destroys the dialog for Narcotic Changes.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
bool NarcoticsProtocolVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matProtocol.reset();
	m_matCatalog.reset();
	return true;
}

//--------------------------------------------------------------------------------------------------------------//
//! Action that is executed when the Search button is clicked.
/*! The function calls the handles for narcprotocol and narctransactioncatalog. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( dtFromSearch->date() > dtToSearch->date() )
	{
		basar::gui::tie::infoMsgBox( this,
			tr("To date must be greater than or equal to from date!").toLocal8Bit().constData(),
			tr("Narcotics").toLocal8Bit().constData() );
		return;
	}

	twCatalog->clearContext();
	twProtocol->clearContext();

	setCursor( Qt::WaitCursor );	
	
	basar::appl::EventReturnType ret = basar::appl::SystemEventManager::getInstance().fire( "SearchCatalog" );
	
	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().data() );
    }

	setCursor( Qt::ArrowCursor );
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that sets values in the Search Iterator.
/*! The values come from the entries from the user on the Gui. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::matchFromFirstSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromFirstSelection of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( !txtArticleNoSearch->text().isEmpty() ){
		searchIt.setString( constants::ARTICLECODE, txtArticleNoSearch->text().toLocal8Bit().constData() );
	}

	if( !txtUserSearch->text().isEmpty() ){
		searchIt.setString( constants::USERNAME, txtUserSearch->text().toLocal8Bit().constData() );
	}

	if( !txtAttributeSearch->text().isEmpty() )
	{
		ProtocolTransformer pProtocolTransformer;
		pProtocolTransformer.LeftToRight( txtAttributeSearch->text().toLocal8Bit().constData(), searchIt );
	}

	searchIt.setString( constants::FROMDATE, dtFromSearch->date().toString("yyyyMMdd").toLocal8Bit().constData() );
	searchIt.setString( constants::TODATE, dtToSearch->date().toString("yyyyMMdd").toLocal8Bit().constData() );
}

//--------------------------------------------------------------------------------------------------------------//
//! Initialization of the Matchers and Transformers.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::init( basar::db::aspect::AccessorPropertyTableRef protocolTable, basar::db::aspect::AccessorPropertyTableRef catalogTable )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matCatalog = basar::gui::tie::Manager::getInstance().createMatcher( grpTransCatData, catalogTable );

	std::list<basar::VarString> originalAttributeNamesCat;
	originalAttributeNamesCat.push_back( constants::BRANCHNO		);
	originalAttributeNamesCat.push_back( constants::ARTICLECODE		);
	originalAttributeNamesCat.push_back( constants::TRANSACTIONDATE	);
	originalAttributeNamesCat.push_back( constants::TRANSACTIONTIME	);
	originalAttributeNamesCat.push_back( "customersupplierno"		);
	originalAttributeNamesCat.push_back( constants::TRANSACTIONTYPE	);
	originalAttributeNamesCat.push_back( constants::PURCHASEORDERNO	);
	originalAttributeNamesCat.push_back( "qtytransaction"			);
	originalAttributeNamesCat.push_back( "qtyinstock"				);

	m_matCatalog.transformProperties( twCatalog->horizontalHeaderLabels(), originalAttributeNamesCat );
	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().constData() );
	m_matCatalog.push_back( dateTransformer );
	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr("Time").toLocal8Bit().constData() );
	m_matCatalog.push_back( timeTransformer );
	SignTransformer signTransformer;
	m_matCatalog.push_back( signTransformer );
	
	m_matProtocol = basar::gui::tie::Manager::getInstance().createMatcher( grpNarcoticsProtocolData, protocolTable );

	std::list<basar::VarString> originalAttributeNames;
	originalAttributeNames.push_back( "username"		 );
	originalAttributeNames.push_back( "protocoldate"	 );
	originalAttributeNames.push_back( "protocoltime"	 );
	originalAttributeNames.push_back( "activitytype"	 );
	originalAttributeNames.push_back( "changedattribute" );
	originalAttributeNames.push_back( "valueold"		 );
	originalAttributeNames.push_back( "valuenew"		 );

	m_matProtocol.transformProperties( twProtocol->horizontalHeaderLabels(), originalAttributeNames );

	VariableDateTransformer pDateTransformer( constants::PROTOCOLDATE, tr("Date").toLocal8Bit().constData() );
	m_matProtocol.push_back( pDateTransformer );
	
	VariableTimeTransformer pTimeTransformer( constants::PROTOCOLTIME, tr("Time").toLocal8Bit().constData() );
	m_matProtocol.push_back( pTimeTransformer );

	ProtocolTransformer pProtocolTransformer;
	m_matProtocol.push_back( pProtocolTransformer );
}

//--------------------------------------------------------------------------------------------------------------//
//! Initialization of the Dialog NarcoticsProtocol.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::initGuiCtrls()
{
	txtArticleNoSearch->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,8}"), this)  );
	txtArticleNoSearch->setText("");
	txtArticleNoSearch->setFocus();

	txtUserSearch->setText("");
	txtAttributeSearch->setText("");

	dtFromSearch->setDate( QDate::currentDate() );
	dtFromSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtFromSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	dtToSearch->setDate( QDate::currentDate() );
	dtToSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );
	dtToSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	txtUserSearch->setMaxLength( 20 );
	txtUserSearch->setText("");

	//--------------------------------------//
	// narcotic booking
	//--------------------------------------//
	QStringList CatalogCaptions;
	CatalogCaptions	<< tr("BranchNo")		<< tr("ArticleNo")		<< tr("Date")		<< tr("Time")		
					<< tr("Bga-No.")		<< tr("Type")			<< tr("Order-No.")	<< tr("Quantity")		
					<< tr("QtyInStock");

	twCatalog->setHorizontalHeaderLabels( CatalogCaptions );
	twCatalog->hideVerticalHeader();
	twCatalog->clearContext();

	//--------------------------------------//
	// logged bookings
	//--------------------------------------//
	QStringList TWCaptions;
	TWCaptions	<< tr("Name")		<< tr("Date")		<< tr("Time")		<< tr("Activity")	
				<< tr("Attribute")	<< tr("old value")	<< tr("new value");

	twProtocol->setHorizontalHeaderLabels( TWCaptions );
	twProtocol->hideVerticalHeader();
	twProtocol->clearContext();
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that matches data from the Accessor to the Gui.
/*! The data is written into the Protocol-TableWidget. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::matchToProtocol()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToProtocol of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	twProtocol->clearContext();
	m_matProtocol.RightToLeft( 0, twProtocol->rowCount() );
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that matches data from the Accessor to the Gui.
/*! The data is written into the TransactionCatalog-TableWidget. throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::matchToCatalog()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCatalog of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matCatalog.RightToLeft( 0, twCatalog->rowCount() );
	twCatalog->setCurrentCell( 0, 1 );
}

//--------------------------------------------------------------------------------------------------------------//
//! Function that returns the current Y-Iterator.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator NarcoticsProtocolVC::getCatalogId()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getCatalogId of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_matCatalog.getCurrentRight();
}

//--------------------------------------------------------------------------------------------------------------//
//! The dialog is closed and destroyed when the button "Close" is clicked.
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

//--------------------------------------------------------------------------------------------------------------//
//! data found for search criteria will be shown in print preview
/*! throw no-throw. */
//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::onBtnPrint_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnPrint_clicked of NarcoticsProtocolVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnType ret = basar::appl::SystemEventManager::getInstance().fire( "PrintProtocol" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().data() );
	}
}

//--------------------------------------------------------------------------------------------------------------//
void NarcoticsProtocolVC::onTwCatalog_itemSelectionChanged()
{
	basar::appl::EventReturnType ret;
	if( false == m_matCatalog.getCurrentRight().isEnd() ){
		ret = basar::appl::SystemEventManager::getInstance().fire( "SearchProtocol" );
	}

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().data() );
	}
}

} //namespace viewConn
} //namespace narcotics
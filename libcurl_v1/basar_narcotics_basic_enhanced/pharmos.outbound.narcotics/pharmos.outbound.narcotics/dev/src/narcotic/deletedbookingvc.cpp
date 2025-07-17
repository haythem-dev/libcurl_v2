//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "acknowledgementstatustransformer.h"
#include "valuerowcolortransformer.hpp"
#include "deletedbookingvc.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

//-----------------------------------------------------------------------------------------------//
DeletedBookingVC::DeletedBookingVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
	createLayout();
}

//-----------------------------------------------------------------------------------------------//
DeletedBookingVC::~DeletedBookingVC()
{
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::wireEvents()
{
	QObject::connect(	articleNoSearch,	SIGNAL( textChanged( QString )						),			
						this,				SLOT  (	onArticleNoSearch_textChanged( QString )	));	
	QObject::connect(	btnSearch,			SIGNAL( clicked()									),			
						this,				SLOT  (	onBtnSearch_clicked()						));	
	QObject::connect(	btnClose,			SIGNAL( clicked()									),			
						this,				SLOT  (	onBtnClose_clicked()						));	
	QObject::connect(	btnRestore,			SIGNAL( clicked()									),			
						this,				SLOT  (	onBtnRestore_clicked()						));	
	QObject::connect(	tableWidget,		SIGNAL( itemSelectionChanged()						),			
						this,				SLOT  (	onTableWidget_itemSelectionChanged()		));	
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::createLayout()
{
	QHBoxLayout* searchLayout = new QHBoxLayout(grpSearch);
	searchLayout->addWidget(lbArticleNo);
	searchLayout->addWidget(articleNoSearch);
	searchLayout->addStretch(2);
	searchLayout->addWidget(lbFrom);
	searchLayout->addWidget(dtFromSearch);
	searchLayout->addStretch(1);
	searchLayout->addWidget(lbTo);
	searchLayout->addWidget(dtToSearch);
	searchLayout->addStretch(7);
	searchLayout->addWidget(btnSearch);

	QVBoxLayout* deletedLayout = new QVBoxLayout(grpDeletedBookings);
	deletedLayout->addWidget(tableWidget);
	deletedLayout->addWidget(btnRestore, 0, Qt::AlignRight);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(grpSearch);
	mainLayout->addWidget(grpDeletedBookings);
	mainLayout->addWidget(btnClose, 0, Qt::AlignRight);
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::init( basar::db::aspect::AccessorPropertyTableRef deletedBookings )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::init()");

	initGuiCtrls();

	m_matDeletedBookings = basar::gui::tie::Manager::getInstance().createMatcher( grpDeletedBookings, deletedBookings );
	
	std::list<basar::VarString> attribute;
	attribute.push_back(constants::PURCHASEORDERNO);
	attribute.push_back(constants::ARTICLECODE);
	attribute.push_back(constants::ARTICLENAME);
	attribute.push_back(constants::PHARMAFORM);
	attribute.push_back(constants::PACKAGEUNIT);
	attribute.push_back(constants::QTYTRANSACTION);
	attribute.push_back(constants::TRANSACTIONTYPE);
	attribute.push_back(constants::CUSTOMERSUPPLIERNO);
	attribute.push_back(constants::CUSTOMER_SUPPLIER_NAME);
	attribute.push_back(constants::CUSTOMER_SUPPLIER_LOCATION);
	attribute.push_back(constants::TRANSACTIONDATE);
	attribute.push_back(constants::TRANSACTIONTIME);
	attribute.push_back(constants::CONTRIBUTIONVOUCHERNO);
	attribute.push_back(constants::ACKNOWLEDGEMENTSTATUS);
	m_matDeletedBookings.transformProperties( tableWidget->horizontalHeaderLabels(), attribute );

	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().data() );
	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr("Time").toLocal8Bit().data() );
    AcknowledgementStatusTransformer statusTransformer;

	std::map<basar::Int16, QColor> valueColorMap;
	valueColorMap.insert(std::map<basar::Int16, QColor>::value_type((basar::Int16)constants::DELIVERY_NOTE_COPY_CREATED, QColor(Qt::red)));
	ValueRowColorTransformer<basar::Int16> rowColorTransformer(constants::ACKNOWLEDGEMENTSTATUS, valueColorMap, tableWidget);

	m_matDeletedBookings.push_back(dateTransformer);
	m_matDeletedBookings.push_back(timeTransformer);
    m_matDeletedBookings.push_back(statusTransformer);
    m_matDeletedBookings.push_back(rowColorTransformer);

	articleNoSearch->setFocus();
}	

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::initGuiCtrls()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::initGuiCtrls()");

	QStringList list;
	list << tr("Order-No.") << tr("ArticleNo.") << tr("ArticleName") << tr("PharmaForm") << tr("PackageUnit") << tr("Quantity") 
		 << tr("TransactionType") << tr("BGA-No.") << tr("Name") << tr("Location") << tr("Date") << tr("Time") << tr("ContributionVoucherNo") << tr("AcknowledgementStatus");
	tableWidget->setHorizontalHeaderLabels( list );
	tableWidget->hideVerticalHeader();
	for( int i = 0; i < tableWidget->columnCount(); i++ )
	{
		tableWidget->setEditableColumn( i, false );
	}
	tableWidget->resizeColumnsToContents();

	dtFromSearch->setDate( QDate::currentDate() );
	dtFromSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );	
	dtFromSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );
	dtToSearch->setDate( QDate::currentDate() );
	dtToSearch->setMinimumDate( basarDateToQDate( constants::MINIMUM_SEARCH_DATE ) );	
	dtToSearch->setMaximumDate( basarDateToQDate( constants::MAXIMUM_SEARCH_DATE ) );

	btnSearch->setDisabled( true );
	btnRestore->setDisabled( true );
	articleNoSearch->setText("");
	articleNoSearch->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,8}"), this) );
}

//-----------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum DeletedBookingVC::show()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::show()");

	return basar::gui::tie::getWidgetReturnType(exec());
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::hide()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::hide()");

	return QWidget::hide();
}

//-----------------------------------------------------------------------------------------------//
bool DeletedBookingVC::shutdown()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::shutdown()");

	m_matDeletedBookings.reset();
	return close();
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::matchFromSelection(basar::db::aspect::AccessorPropertyTable_YIterator searchIt)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::matchFromSelection()");

	searchIt.setString( constants::ARTICLECODE,	articleNoSearch->text().toLocal8Bit().constData()					);
	searchIt.setString( constants::FROMDATE,	dtFromSearch->date().toString("yyyyMMdd").toLocal8Bit().constData() );
	searchIt.setString( constants::TODATE,		dtToSearch->date().toString("yyyyMMdd").toLocal8Bit().constData()	);
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::matchToDeletedBookings()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::matchToDeletedBookings()");

	tableWidget->clearContext();
	m_matDeletedBookings.RightToLeft( 0, tableWidget->rowCount() );
	tableWidget->resizeColumnsToContents();
}

//-----------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator DeletedBookingVC::getCurrentAccessorIterator( basar::Int32 position )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::getCurrentAccessorIterator()");

	basar::db::aspect::AccessorPropertyTable_YIterator currentPositionIt = m_matDeletedBookings.getRight().begin();
	while( !currentPositionIt.isEnd() && ( position != 0 ) )
	{
		position--;
		currentPositionIt++;
	}
	return currentPositionIt;
}

//-----------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator DeletedBookingVC::getSelectedFirstIterator()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::getSelectedFirstIterator()");

	return m_matDeletedBookings.getSelectedRight();
}

//-----------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator DeletedBookingVC::getSelectedNextAccIterator()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::getSelectedNextAccIterator()");

	return m_matDeletedBookings.getNextSelectedRight();
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::onBtnRestore_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::onBtnRestore_clicked()");

	setCursor( Qt::WaitCursor );	
	
	basar::gui::tie::infoMsgBox( this,
			tr("The contributionvoucher is not transmitted to BfarM!!").toLocal8Bit().constData(),
			tr("Narcotics").toLocal8Bit().constData() );
	
	basar::appl::SystemEventManager::getInstance().fire( "RestoreDeletedBookings" );
	setCursor( Qt::ArrowCursor );
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::onBtnClose_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::onBtnClose_clicked()");

	shutdown();
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::onBtnSearch_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::onBtnSearch_clicked()");

	if( dtFromSearch->date() > dtToSearch->date() )
	{
		basar::gui::tie::infoMsgBox( this,
			tr("To date must be greater than or equal to from date!").toLocal8Bit().constData(),
			tr("Narcotics").toLocal8Bit().constData() );
		return;
	}

	tableWidget->clearContext();
	
	setCursor( Qt::WaitCursor );	
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchDeletedBookings" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::onArticleNoSearch_textChanged( const QString& txt )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::onArticleNoSearch_textChanged()");

	btnSearch->setDisabled( txt.isEmpty() );
}

//-----------------------------------------------------------------------------------------------//
void DeletedBookingVC::onTableWidget_itemSelectionChanged()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "DeletedBookingVC::onTableWidget_itemSelectionChanged()");

	basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matDeletedBookings.getSelectedRight();
	bool enable = false;
	
	while (false == yit.isEnd())
	{
		if (yit.getInt16(constants::ACKNOWLEDGEMENTSTATUS) != constants::DELIVERY_NOTE_COPY_CREATED || yit.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS )
			enable = true;
		yit = m_matDeletedBookings.getNextSelectedRight();
	}
	btnRestore->setEnabled(enable);
}

//-----------------------------------------------------------------------------------------------//

} //namespace viewConn
} //namespace narcotics

//-----------------------------------------------------------------------------------------------//

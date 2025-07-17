//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "customermappingvc.h"
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
CustomerMappingVC::CustomerMappingVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
CustomerMappingVC::~CustomerMappingVC()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::wireEvents()
{
	QObject::connect( btnClose,				SIGNAL( clicked()			 ), this, SLOT( onBtnClose_clicked()					));
	QObject::connect( btnSearch,			SIGNAL( clicked()			 ), this, SLOT( onBtnSearch_clicked()					));
	QObject::connect( btnEdit,				SIGNAL( clicked()			 ), this, SLOT( onBtnEdit_clicked()						));
	QObject::connect( btnNew,				SIGNAL( clicked()			 ), this, SLOT( onBtnNew_clicked()						));
	QObject::connect( btnDelete,			SIGNAL( clicked()			 ), this, SLOT( onBtnDelete_clicked()					));
	QObject::connect( twCustomerMappings,	SIGNAL( itemDoubleClicked( QTableWidgetItem* )),this,	SLOT  (	onTablewidget_itemDoubleClicked( QTableWidgetItem* )));
	QObject::connect( twCustomerMappings,	SIGNAL( itemSelectionChanged()),this,SLOT  ( onTablewidget_itemSelectionChanged()	));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	void
	\param	narcoticsBookings is the AccessorPropertyTable necessary to create the matcher
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::init( AccessorPropertyTableRef customerMappings )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	initList();
	initGuiCtrls();

	using basar::gui::tie::Manager;
	// creation of the matcher
	m_matCustomerMappings = Manager::getInstance().createMatcher( twCustomerMappings, customerMappings );
	m_matCustomerMappings.transformProperties( m_lstCaptionsCustomerMappings, m_OriginalAttributeNamesCustomerMappings );

	PrintedCustomerAdressTransformer printedAdressTransformer;
	m_matCustomerMappings.push_back( printedAdressTransformer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::initList()
{
	m_lstCaptionsCustomerMappings.push_back( tr( "No ext." ).toLocal8Bit().data()				);
	m_lstCaptionsCustomerMappings.push_back( tr( "Name ext." ).toLocal8Bit().data()				);
	m_lstCaptionsCustomerMappings.push_back( tr( "No int." ).toLocal8Bit().data()				);
	m_lstCaptionsCustomerMappings.push_back( tr( "Name int." ).toLocal8Bit().data()				);
	m_lstCaptionsCustomerMappings.push_back( tr( "printed adress" ).toLocal8Bit().data()		);
	
	m_OriginalAttributeNamesCustomerMappings.push_back( constants::CUSTOMERNO_EXTERN		);
	m_OriginalAttributeNamesCustomerMappings.push_back( constants::NAME_EXTERN				);
	m_OriginalAttributeNamesCustomerMappings.push_back( constants::CUSTOMERNO_INTERN		);
	m_OriginalAttributeNamesCustomerMappings.push_back( constants::NAME_INTERN				);
	m_OriginalAttributeNamesCustomerMappings.push_back( constants::PRINTED_ADRESS			);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::initGuiCtrls()
{
	// qtablewidget
	twCustomerMappings->clear();
	twCustomerMappings->hideVerticalHeader();
	
	twCustomerMappings->setColumnCount( static_cast<int>(m_lstCaptionsCustomerMappings.size()) );
	QStringList lst;
	std::list<basar::VarString>::const_iterator it = m_lstCaptionsCustomerMappings.begin();
	for( ; it != m_lstCaptionsCustomerMappings.end(); it++ ){
		lst << QString::fromLocal8Bit((*it).c_str());
	}
	twCustomerMappings->setHorizontalHeaderLabels( lst );
	twCustomerMappings->resizeColumnsToContents();
	twCustomerMappings->setSelectionBehavior(QAbstractItemView::SelectRows);
	
	// prevent cells of table tblTransData from being editable
	twCustomerMappings->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	int indicates the value returned by a modal dialog. 
			Value '1' represents QDialog::Accepted and value '0' represents QDialog::Rejected.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum CustomerMappingVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	onBtnSearch_clicked();
	txtCustNo->setFocus();

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true by default
			element of the erased one.
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CustomerMappingVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matCustomerMappings.reset();
	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\param	searchIt represents the AccessorPropertyTable_YIterator that is search criteria set to
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::matchFromSelection( AccessorPropertyTable_YIterator searchIt )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromSelection of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( !txtCustNo->text().isEmpty() )
	{
		if (txtCustNo->text().toInt() > 0)			
			searchIt.setString( constants::CUSTOMERSUPPLIERNO, txtCustNo->text().toLocal8Bit().constData());
		else
		{
			searchIt.setString( constants::CUSTOMERNAME, txtCustNo->text().toUpper().toLocal8Bit().constData() );
		}
	} 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::matchToCustomerMappings()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNarcoticBookings of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	twCustomerMappings->clearContext();
	m_matCustomerMappings.RightToLeft( 0, twCustomerMappings->rowCount() );
	twCustomerMappings->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::matchToCustomerNo( basar::I18nString customerNoValue )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToCustomerNo of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	txtCustNo->setText( QString::fromLocal8Bit(customerNoValue.c_str()) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onBtnEdit_clicked()
{
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "EditCustomerMapping", m_matCustomerMappings.getSelectedRight() );

	if( ret.ret != basar::appl::HANDLER_OK )
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
	
	onBtnSearch_clicked();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onTablewidget_itemDoubleClicked( QTableWidgetItem* )
{
	if (btnEdit->isEnabled())
		onBtnEdit_clicked();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onTablewidget_itemSelectionChanged()
{
	btnEdit->setDisabled( twCustomerMappings->selectedItems().isEmpty() );
	btnDelete->setDisabled( twCustomerMappings->selectedItems().isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onBtnNew_clicked()
{
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "CreateCustomerMapping", m_matCustomerMappings.getSelectedRight() );

	if( ret.ret != basar::appl::HANDLER_OK )
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
	
	onBtnSearch_clicked();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onBtnDelete_clicked()
{
	if( basar::gui::tie::questionMsgBox( this, 
		tr("Do you really want to delete this mapping?").toLocal8Bit().constData(),	tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		setCursor( Qt::WaitCursor );
		basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "DeleteCustomerMapping", m_matCustomerMappings.getSelectedRight() );
		setCursor( Qt::ArrowCursor );

		if( ret.ret != basar::appl::HANDLER_OK )
		{
			basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
			twCustomerMappings->setFocus();
		}
		else
		{
			onBtnSearch_clicked();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onBtnSearch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSearch_clicked of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	setCursor( Qt::WaitCursor );	
	twCustomerMappings->clearContext();
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerMappings" );
	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
		txtCustNo->selectAll();
	}
	else
	{
		twCustomerMappings->setFocus();
		twCustomerMappings->selectRow(0);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */   
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of CustomerMappingVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	shutdown();
}



} //namespace viewConn
} //namespace narcotics
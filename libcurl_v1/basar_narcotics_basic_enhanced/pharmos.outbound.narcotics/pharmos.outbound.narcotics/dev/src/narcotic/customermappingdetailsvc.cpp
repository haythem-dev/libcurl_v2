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
#include "customermappingdetailsvc.h"
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
CustomerMappingDetailsVC::CustomerMappingDetailsVC( QWidget *parent )
:	QDialog( parent ), m_isCreation (false), m_SwitchAdress (false)
{
	setupUi(this);
	wireEvents();
}

//-----------------------------------------------------------------------------------------------//
CustomerMappingDetailsVC::~CustomerMappingDetailsVC()
{
}
//-----------------------------------------------------------------------------------------------//

void CustomerMappingDetailsVC::wireEvents()
{
	QObject::connect(	customernoextern,			SIGNAL( returnPressed()								),
						this,						SLOT  ( onCustomernoExtern_editingFinished()		));
	QObject::connect(	customernointern,			SIGNAL( returnPressed()								),
						this,						SLOT  ( onCustomernoIntern_editingFinished()		));
	QObject::connect(	btnSwitch,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnSwitch_clicked()						));
	QObject::connect(	btnClose,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnClose_clicked()						));
	QObject::connect(	btnSave,					SIGNAL( clicked()									),
						this,						SLOT  ( onBtnSave_clicked()							));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	withSearchCriteria regulates if the dialog is initialized with or without the search criterias.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::initGuiCtrls(  )
{
    if (m_isCreation)
	{
		customernoextern->setReadOnly(false);
		customernointern->setReadOnly(false);

		btnSwitch->setDisabled(true);

		customernoextern->setFocus();
	}
	else
	{
		customernoextern->setReadOnly(true);
		customernointern->setReadOnly(true);

		btnSwitch->setDisabled(false);

		btnSwitch->setFocus();
	}

	btnSave->setDisabled(true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::init( basar::db::aspect::AccessorPropertyTableRef customerData, const bool isCreation )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_isCreation = isCreation;

	initGuiCtrls();

	using basar::gui::tie::Manager;
	// creation of the matcher
	
	m_matCustomer			= Manager::getInstance().createMatcher( this, customerData );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	int returns the result (DialogCode) of a modally shown dialog
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum CustomerMappingDetailsVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	bool is always return at the time.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CustomerMappingDetailsVC::shutdown()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_matCustomer.reset();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::onBtnClose_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnClose_clicked of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::onBtnSwitch_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSwitch_clicked of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_SwitchAdress = !m_SwitchAdress;

	switchAdress();

	if (!m_isCreation)
		btnSave->setDisabled(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::matchFromCustExtern (basar::db::aspect::AccessorPropertyTable_YIterator searchIt)
{
	if (customernoextern->text().toInt() > 0)			
		searchIt.setString( constants::CUSTOMERSUPPLIERNO, customernoextern->text().toLocal8Bit().constData());
	else
	{
		searchIt.setString( constants::CUSTOMERNAME, customernoextern->text().toUpper().toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::matchFromCustIntern (basar::db::aspect::AccessorPropertyTable_YIterator searchIt)
{
	if (customernointern->text().toInt() > 0)			
		searchIt.setString( constants::CUSTOMERSUPPLIERNO, customernointern->text().toLocal8Bit().constData());
	else
	{
		searchIt.setString( constants::CUSTOMERNAME, customernointern->text().toUpper().toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//!	\param	no param.
//!	\throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::switchAdress()
{
	QPalette pal_printed;
	pal_printed.setColor(QPalette::Text, Qt::red);

	QFont font_printed;
    font_printed.setBold(true);
     	
	QPalette pal_skipped;
	pal_skipped.setColor(QPalette::Text, Qt::darkGray);

	QFont font_skipped;
    font_skipped.setBold(false);
    
	if (m_SwitchAdress)
	{
		name_extern->setPalette(pal_skipped);
		street_extern->setPalette(pal_skipped);
		cip_extern->setPalette(pal_skipped);
		location_extern->setPalette(pal_skipped);

		name_extern->setFont(font_skipped);
		street_extern->setFont(font_skipped);
		cip_extern->setFont(font_skipped);
		location_extern->setFont(font_skipped);


		name_intern->setPalette(pal_printed);
		street_intern->setPalette(pal_printed);
		cip_intern->setPalette(pal_printed);
		location_intern->setPalette(pal_printed);

		name_intern->setFont(font_printed);
		street_intern->setFont(font_printed);
		cip_intern->setFont(font_printed);
		location_intern->setFont(font_printed);
	}
	else
	{
		name_extern->setPalette(pal_printed);
		street_extern->setPalette(pal_printed);
		cip_extern->setPalette(pal_printed);
		location_extern->setPalette(pal_printed);

		name_extern->setFont(font_printed);
		street_extern->setFont(font_printed);
		cip_extern->setFont(font_printed);
		location_extern->setFont(font_printed);


		name_intern->setPalette(pal_skipped);
		street_intern->setPalette(pal_skipped);
		cip_intern->setPalette(pal_skipped);
		location_intern->setPalette(pal_skipped);

		name_intern->setFont(font_skipped);
		street_intern->setFont(font_skipped);
		cip_intern->setFont(font_skipped);
		location_intern->setFont(font_skipped);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::onBtnSave_clicked()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onBtnSave_clicked of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	setCursor( Qt::WaitCursor );
	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SaveCustomerMapping" );
	setCursor( Qt::ArrowCursor );
	
	if( ret.ret == basar::appl::HANDLER_ERROR )
	{
		basar::gui::tie::warningMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
	else
		onBtnClose_clicked();
}

//-----------------------------------------------------------------------------------------------//
void CustomerMappingDetailsVC::matchToCustomerData( basar::db::aspect::AccessorPropertyTable_YIterator yit  )
{	
	m_matCustomer.RightToLeft(m_matCustomer.getLeft().begin(), yit);

	if (yit.isContainedAndSet(constants::SWITCHADDRESSFLAG))
		m_SwitchAdress = yit.getInt16(constants::SWITCHADDRESSFLAG) == 1;
	else
		m_SwitchAdress = false;

	switchAdress();
}

//-----------------------------------------------------------------------------------------------//
void CustomerMappingDetailsVC::matchFromCustomerData( basar::db::aspect::AccessorPropertyTable_YIterator yit  )
{
	m_matCustomer.LeftToRight( m_matCustomer.getCurrentLeft(), yit );

	if (m_SwitchAdress)
		yit.setInt16(constants::SWITCHADDRESSFLAG, 1);
	else
		yit.setInt16(constants::SWITCHADDRESSFLAG, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::onCustomernoExtern_editingFinished()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onCustomernoExtern_editingFinished of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if (m_isCreation && !customernoextern->text().isEmpty())
	{
		basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerExtern", m_matCustomer.getCurrentRight() );

		customernoextern->setFocus();
			
		if( ret.ret != basar::appl::HANDLER_OK )
		{
			street_extern->clear();
			name_extern->clear();
			cip_extern->clear();
			location_extern->clear();
			basar::gui::tie::warningMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
			customernoextern->selectAll();
			return;
		}

		if (!name_intern->text().isEmpty()) 
		{
			btnSave->setDisabled(false);
			btnSwitch->setDisabled(false);
		}
		else
			customernointern->setFocus();	
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void CustomerMappingDetailsVC::onCustomernoIntern_editingFinished()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "onCustomernoIntern_editingFinished of CustomerMappingDetailsVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if (m_isCreation && !customernointern->text().isEmpty())
	{
		basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerIntern", m_matCustomer.getCurrentRight() );

		customernointern->setFocus();

		if( ret.ret != basar::appl::HANDLER_OK )
		{
			street_intern->clear();
			name_intern->clear();
			cip_intern->clear();
			location_intern->clear();
			basar::gui::tie::warningMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
			customernointern->selectAll();
			return;
		}

		if (!name_extern->text().isEmpty()) 
		{
			btnSave->setDisabled(false);
			btnSwitch->setDisabled(false);
		}
		else
			customernoextern->setFocus();	
	}
}

} //namespace viewConn
} //namespace narcotics
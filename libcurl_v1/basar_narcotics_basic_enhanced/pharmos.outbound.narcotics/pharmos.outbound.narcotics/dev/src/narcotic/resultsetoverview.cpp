//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author Benjamin Endlicher
 *  \date   24.03.2010
 */
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "resultsetoverview.h"
#include "loggerpool.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param	parent passed window pointer this dialog belongs to.
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
ResultSetOverviewVC::ResultSetOverviewVC( QWidget *parent )
: QDialog( parent ), m_Cancelled( false )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::ResultSetOverviewVC()");

	setupUi(this);
	wireEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
ResultSetOverviewVC::~ResultSetOverviewVC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::~ResultSetOverviewVC()");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::wireEvents()
{
	QObject::connect(	btnTakeOver,	SIGNAL( clicked()											),
						this,			SLOT  (	onBtnTakeOver_clicked()								));
	QObject::connect(	btnCancel,		SIGNAL( clicked()											),
						this,			SLOT  (	onBtnCancel_clicked()								));
	QObject::connect(	tablewidget,	SIGNAL( itemDoubleClicked( QTableWidgetItem* )				),
						this,			SLOT  (	onTablewidget_itemDoubleClicked( QTableWidgetItem* )));
	QObject::connect(	tablewidget,	SIGNAL( itemSelectionChanged()								),
						this,			SLOT  (	onTablewidget_itemSelectionChanged()				));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! return	void
//! throw	return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::init( basar::db::aspect::AccessorPropertyTableRef propTab,	 
							    basar::I18nString attribute1,
							    basar::I18nString attribute2,
							    basar::I18nString headerlabel1, 
							    basar::I18nString headerlabel2 )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::init()");

	QStringList list;
	std::list<basar::VarString> attribute;
	list << tr(headerlabel1.c_str()) << tr(headerlabel2.c_str());
	attribute.push_back( attribute1 );
	attribute.push_back( attribute2 );
	// PZN8 Geknödel
	if( attribute1 == constants::ARTICLECODE )
	{
		tablewidget->setColumnCount( 3 );
		tablewidget->setColumnHidden( 2, true );
		list.append( tr("PZNIntern") );
		attribute.push_back( constants::ARTICLENO );
	}
	else
	{
		tablewidget->setColumnCount( 2 );
	}
	// PZN8 Geknödel

	tablewidget->setHorizontalHeaderLabels( list );
	tablewidget->hideVerticalHeader();
	tablewidget->resizeColumnsToContents();
	tablewidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_matTableWidget = basar::gui::tie::Manager::getInstance().createMatcher( tablewidget, propTab );
	m_matTableWidget.transformProperties( tablewidget->horizontalHeaderLabels(), attribute );
	btnTakeOver->setEnabled( false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return int-value of the methode QDialog::exec()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum ResultSetOverviewVC::show()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::show()");

	return basar::gui::tie::getWidgetReturnType( exec() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::hide()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::hide()");

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return bool-value of the method QDialog::close()
//! \throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool ResultSetOverviewVC::shutdown()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::shutdown()");

	m_matTableWidget.reset();
	m_Cancelled = false;
	return close();
}

//---------------------------------------------------------------------------------------------------------------------//
// match methods
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::matchToWidget()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::matchToWidget()");

	tablewidget->clearContext();
	m_matTableWidget.RightToLeft( 0, tablewidget->rowCount() );
	tablewidget->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	currentArticle is the AccessorPropertyTable_YIterator which is going to be used to search for narcotic group details
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
basar::db::aspect::AccessorPropertyTable_YIterator ResultSetOverviewVC::getCurrentSelection()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::getCurrentSelection()");

	basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matTableWidget.getCurrentRight();

	// if a correct selection has been cancelled or no selection has been executed at all, yit has to be resetted
	if (m_Cancelled || yit.isEnd())
	{
		yit.clear();
	}
	return yit;
}

//---------------------------------------------------------------------------------------------------------------------//
// private slots
//---------------------------------------------------------------------------------------------------------------------//
void ResultSetOverviewVC::onBtnCancel_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::on_btnCancel_clicked()");

	m_Cancelled = true;
	reject();
}

//---------------------------------------------------------------------------------------------------------------------//
void ResultSetOverviewVC::onBtnTakeOver_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::on_btnTakeOver_clicked()");

	accept();
}

//-----------------------------------------------------------------------------------------------//
// ctrl events
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::onTablewidget_itemDoubleClicked( QTableWidgetItem* )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::on_tablewidget_itemDoubleClicked()");

	accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void ResultSetOverviewVC::onTablewidget_itemSelectionChanged()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "ResultSetOverviewVC::on_tablewidget_itemSelectionChanged()");

	btnTakeOver->setDisabled( tablewidget->selectedItems().isEmpty() );
}

} //namespace viewConn
} //namespace narcotics
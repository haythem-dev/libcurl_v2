#include "tablewidgetmatchandsizetestframe.h"
#include "buttablepropertydescriptionvectors.h"
#include "testconnection.h"
#include "libbasarqtwidget_integersignalemitter.h"

using basar_trial::guitie::TestConnection;

TableWidgetMatchAndSizeTestFrame::TableWidgetMatchAndSizeTestFrame(QWidget *parent)
: QWidget(parent)
{
	m_memoryPolicy  = basar::gui::tie::WITHOUT_MEMORY;
	m_cachingPolicy = basar::db::aspect::ON_DEMAND_CACHING;

	ui.setupUi( this );

	ui.comboCachingPolicy->blockSignals( true );
	ui.comboCachingPolicy->addItem( "ON_DEMAND_CACHING" );
	ui.comboCachingPolicy->addItem( "FULL_CACHING" );
	ui.comboCachingPolicy->addItem( "SINGLE_ROW_CACHING" );
	ui.comboCachingPolicy->addItem( "NO_CACHING" );
	ui.comboCachingPolicy->blockSignals( false );

	ui.comboMemoryPolicy->blockSignals( true );
	ui.comboMemoryPolicy->addItem( "WITHOUT_MEMORY" );
	ui.comboMemoryPolicy->addItem( "WITH_MEMORY" );
	ui.comboMemoryPolicy->blockSignals( false );

	ui.cbExceptions->setChecked( true );
	ui.cbExceptions->setCheckState( Qt::Checked );

	ui.tableWidget->setRowCount( 3 );
	ui.tableWidget->setColumnCount( 9 );
	ui.tableWidget->setWordWrap( true );

	setColumnHeaders();
}

TableWidgetMatchAndSizeTestFrame::~TableWidgetMatchAndSizeTestFrame()
{
	m_accessor.getPropertyTable().clear();
}

void TableWidgetMatchAndSizeTestFrame::on_comboCachingPolicy_currentIndexChanged( int caching )
{
	m_cachingPolicy = static_cast< basar::db::aspect::CachingPolicyEnum>( caching );
}

void TableWidgetMatchAndSizeTestFrame::on_comboMemoryPolicy_currentIndexChanged( int memory )
{
	m_memoryPolicy = static_cast<basar::gui::tie::MemoryPolicyEnum>( memory );
}

void TableWidgetMatchAndSizeTestFrame::on_cbExceptions_stateChanged( int state )
{
	bool ok = ( 0 != state );
	basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval( ok, ok, ok );
}

void TableWidgetMatchAndSizeTestFrame::on_pbClear_clicked()
{
	ui.tableWidget->clear();
	setColumnHeaders();
}

void TableWidgetMatchAndSizeTestFrame::on_pbMatch_clicked()
{
	ui.tableWidget->clearContext();
	ui.comboCachingPolicy->setCurrentIndex( m_cachingPolicy );
	ui.comboMemoryPolicy->setCurrentIndex( m_memoryPolicy );

	createAccessorAndMatch();
}

void TableWidgetMatchAndSizeTestFrame::on_pbResizeColumns_clicked()
{
//	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->setResizeColumnsToContents( true );
	ui.tableWidget->setFocus( Qt::MouseFocusReason );
}

void TableWidgetMatchAndSizeTestFrame::on_pbResizeRows_clicked()
{
//	ui.tableWidget->resizeRowsToContents();
	ui.tableWidget->setResizeRowsToContents( true );
	ui.tableWidget->setFocus( Qt::MouseFocusReason );
}

void TableWidgetMatchAndSizeTestFrame::createAccessorAndMatch()
{
	try 
	{
		if( false == m_accessor.isNull() )
		{
			m_accessor.reset();
		}
		static int counter = reinterpret_cast<int>( this );
		basar::VarString accInstanceName;
		accInstanceName.format( "Acc_StringCustomInstance%i", counter++ );
		basar::db::aspect::ConnectionRef conn = TestConnection::getBasarTestConnection();
		m_accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(
			accInstanceName,
			"Acc_StringCustom", 
			conn,
			m_cachingPolicy, 
			false
				);

		if( false == m_matcher.isNull() )
		{
			m_matcher.clear();
			m_matcher.reset();
		}

		m_matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tableWidget, m_accessor.getPropertyTable() );
		m_matcher.getLeft().setMemoryPolicy( m_memoryPolicy );

		basar::db::aspect::ExecuteResultInfo res = m_accessor.execute( "AccMethod_StringCustom", false );
		m_matcher.RightToLeft( 0, ui.tableWidget->rowCount() );
		ui.tableWidget->setFocus( Qt::MouseFocusReason );
	}
	catch( const basar::Exception & e )
	{
		QMessageBox::warning( this, "Exception", e.what().c_str(), QMessageBox::Ok );
	}
}

void TableWidgetMatchAndSizeTestFrame::setColumnHeaders()
{
	QStringList list;
	list << PROPDEF_COLVCH1.getName().c_str();
	list << PROPDEF_COLVCH2.getName().c_str();
	list << PROPDEF_COLVCH3.getName().c_str();
	list << PROPDEF_COLSER.getName().c_str();
	list << PROPDEF_COLSMINT.getName().c_str();
	list << PROPDEF_COLDEC.getName().c_str();
	list << PROPDEF_COLDATE.getName().c_str();
	list << PROPDEF_COLDT1.getName().c_str();
	list << PROPDEF_COLTIME1.getName().c_str();
	ui.tableWidget->setHorizontalHeaderLabels( list );
}

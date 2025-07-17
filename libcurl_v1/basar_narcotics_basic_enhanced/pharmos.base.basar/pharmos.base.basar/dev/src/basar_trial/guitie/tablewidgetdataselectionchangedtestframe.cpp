#include "tablewidgetdataselectionchangedtestframe.h"
#include "buttablepropertydescriptionvectors.h"
#include "testconnection.h"
#include "libbasarqtwidget_integersignalemitter.h"

using basar_trial::guitie::TestConnection;

// --------------------------------------------------------------------------------------

TableWidgetdataSelectionChangedTestFrame::TableWidgetdataSelectionChangedTestFrame(QWidget *parent)
: QWidget(parent)
{
	m_memoryPolicy  = basar::gui::tie::WITHOUT_MEMORY;
	m_cachingPolicy = basar::db::aspect::ON_DEMAND_CACHING;

	ui.setupUi(this);
	ui.comboBox->addItem("ON_DEMAND_CACHING");
	ui.comboBox->addItem("FULL_CACHING");
	ui.comboBox->addItem("SINGLE_ROW_CACHING");
	ui.comboBox->addItem("NO_CACHING");

	ui.comboBox_2->addItem("WITHOUT_MEMORY");
	ui.comboBox_2->addItem("WITH_MEMORY");

	ui.checkBox->setChecked(true);
	ui.checkBox->setCheckState(Qt::Checked);

	change();


	connect(ui.tableWidget, SIGNAL(dataSelectionChanged()), this, SLOT(dataSelectionChanged()));
}
// --------------------------------------------------------------------------------------

TableWidgetdataSelectionChangedTestFrame::~TableWidgetdataSelectionChangedTestFrame()
{
	m_accessor.getPropertyTable().clear();
}
// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::dataSelectionChanged()
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	QString text;
	QString help;
	help.setNum(m_times);
	text.append(help);
	text.append(". in Row no. ");
	help.setNum(row);
	text.append(help);
	text.append(" col no. ");
	help.setNum(col);
	text.append(help);
	ui.textEdit->append (text);
	++m_times;
}
// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::on_pushButton_clicked()
{
	try
	{
		if (m_matcher.isNull())
		{
			m_matcher = 
				basar::gui::tie::Manager::getInstance().createMatcher(ui.tableWidget, m_accessor.getPropertyTable());
			// test:
			m_matcher.getLeft().setMemoryPolicy(m_memoryPolicy);
		}
		m_accessor.execute("AccMethod_StringSelect");
		m_matcher.RightToLeft(0,ui.tableWidget->rowCount());
	}
	catch (basar::Exception & ex)
	{
		QMessageBox::warning(this, "Error", ex.what().c_str(), QMessageBox::Ok);
	}
	catch (...)
	{
		QMessageBox::warning(this, "Error", " - error - ", QMessageBox::Ok);
		throw;
	}
}
// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::on_pushButton_2_clicked()
{
	ui.textEdit->clear();
	ui.tableWidget->clear();
	m_times = 0;
}
// --------------------------------------------------------------------------------------


void TableWidgetdataSelectionChangedTestFrame::on_comboBox_currentIndexChanged(int caching)
{
	m_cachingPolicy = static_cast< basar::db::aspect::CachingPolicyEnum>(caching);
	change();
}
// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::change()
{

	ui.textEdit->clear();
	ui.tableWidget->clear();
	ui.comboBox->setCurrentIndex(m_cachingPolicy);
	ui.comboBox_2->setCurrentIndex(m_memoryPolicy);
	m_times = 0;
	try 
	{
		basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
		m_accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(
			"test",
			"Acc_StringSelect", 
			connection2,
			m_cachingPolicy, 
			true);
		ui.tableWidget->setRowCount(3);
		ui.tableWidget->setColumnCount(
			static_cast<int>(propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.size()));

		QStringList list;
		std::vector<basar::property::PropertyDescription>::const_iterator iter;
		for (iter = propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.begin(); iter != propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.end(); ++iter)
		{
			list << iter->getName().c_str();
		}
		ui.tableWidget->setHorizontalHeaderLabels(list);
		m_matcher.clear();
		m_matcher.reset();
	}
	catch (const basar::Exception& except)
	{
		except.what();
	}
}
// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::on_pushButton_3_clicked()
{
	qFatal("bye");
}

// --------------------------------------------------------------------------------------

void TableWidgetdataSelectionChangedTestFrame::on_comboBox_2_currentIndexChanged(int val)
{
	m_memoryPolicy = static_cast<basar::gui::tie::MemoryPolicyEnum>(val);
	change();
}
// --------------------------------------------------------------------------------------



void TableWidgetdataSelectionChangedTestFrame::on_checkBox_stateChanged(int ok)
{
	if (ok)
	{
		basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(true, true, true);
	}
	else
	{
		basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(false, false, false);

	}

}
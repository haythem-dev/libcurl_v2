#include "testguitie.h"
#include "testconnection.h"
#include "frame1.h"
#include "datetimetest.h"
#include "checkboxtest.h"
#include "msgboxtest.h"
#include "tablewidgetabsoluteheadertestframe1.h"
#include "tablewidgetabsoluteheadertestframe2.h"
#include "tablewidgetmultmatchtestframe.h"
#include "tablewidgetnocoltestframe.h"
#include "tablewidgetdataselectionchangedtestframe.h"
#include "tablewidgetmatchandsizetestframe.h"
#include "frame2.h"
#include "vctestframe.h"

testguitie::testguitie(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
    ui.setupUi(this);
    ui.tabWidget->removeTab ( 0) ;

    QWidget * tab;
    // tab = new Frame1(ui.tabWidget);
    // ui.tabWidget->addTab(tab, "orignal guitie Test");

	tab = new Frame2(ui.tabWidget);
    ui.tabWidget->addTab(tab, "test");

    tab = new DateTimeTest(ui.tabWidget);
    ui.tabWidget->addTab(tab, "Date Time Test");
    tab = new CheckBoxTest(ui.tabWidget);
    ui.tabWidget->addTab(tab, "CheckBox Test");
    tab = new MsgBoxTest(ui.tabWidget);
    ui.tabWidget->addTab(tab, "MessageBox Test");
ui.tabWidget->setCurrentWidget(tab);

	tab = new VCTestFrame(ui.tabWidget);
    ui.tabWidget->addTab(tab,   "VC Test");

    QTabWidget * twid2 = new QTabWidget(ui.tabWidget);
    ui.tabWidget->addTab(twid2, "TableWidget Test");
    
    {
        tab = new TableWidgetAbsoluteHeaderTestFrame1(twid2);
        twid2->addTab(tab,  "Absolute Header 1");
        tab = new TableWidgetAbsoluteHeaderTestFrame2(twid2);
        twid2->addTab(tab,   "Absolute Header 2");
        tab = new TableWidgetMultMatchTestFrame(twid2);
        twid2->addTab(tab,  "Multiple Matcher");
        tab = new TableWidgetNoColTestFrame(twid2);
        twid2->addTab(tab,   "no column");
		tab = new TableWidgetdataSelectionChangedTestFrame(twid2);
        twid2->addTab(tab,   "DataSelectionChangedTest");
		tab = new TableWidgetMatchAndSizeTestFrame(twid2);
        twid2->addTab(tab,   "Match and Size Test");
        twid2->setCurrentWidget(tab);
    }
}


testguitie::~testguitie()
{

}

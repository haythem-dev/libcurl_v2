#pragma warning (push)
#pragma warning(disable: 4127 4244 4273 4311 4312 4512 4800)
#include <Qtwidgets/qgridlayout.h>
#include <Qtwidgets/qmessagebox.h>
#pragma warning (pop)
#include "testguiqt.h"

testguiqt::testguiqt(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// create menu on your own

	m_Statusline.init(this);
	VarString value("r.kiefert");
	m_Statusline.setUser(value);
	value = "pps1cg";
	m_Statusline.setConnection(value);

	m_Background.init("testguiqt", "Version 01.00.00.01", basar::gui::qt::MainBackground::PHOENIXGROUP);
	QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(&m_Background);
	ui.centralWidget->setLayout(mainLayout);
}

testguiqt::~testguiqt()
{
	m_Statusline.clear();
}

void testguiqt::about()
{
   About dlg;
   dlg.show(this);
}

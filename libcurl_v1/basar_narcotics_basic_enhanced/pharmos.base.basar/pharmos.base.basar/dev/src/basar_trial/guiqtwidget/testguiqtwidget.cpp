#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4273 4311 4312 4512 4800)
#include <Qt/qgridlayout.h>
#include <Qt/qmessagebox.h>
#include <Qt/QMdiSubWindow.h>

#pragma warning (pop)
#include "Testguiqtwidget.h"
#include "TableWidgetTestDialog1.h"

Testguiqtwidget::Testguiqtwidget(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	m_area = 0;
	ui.setupUi(this);
	m_area = new QMdiArea(this);
	this->setCentralWidget(m_area);

    connect(ui.actionTableWiget_Test_1 , SIGNAL(triggered()),this, SLOT(on_tableWidgetTest1()));

}

Testguiqtwidget::~Testguiqtwidget()
{

}
 
void Testguiqtwidget::on_tableWidgetTest1()
{
  TableWidgetTestDialog1 * dialog = new TableWidgetTestDialog1(m_area);
  QMdiSubWindow * subWindow1 = m_area->addSubWindow ( dialog, 0) ;
  subWindow1->setAttribute(Qt::WA_DeleteOnClose);
  subWindow1->showMaximized();
  
  
}
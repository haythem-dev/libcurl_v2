#include "tablewidgettestdialog1.h"

TableWidgetTestDialog1::TableWidgetTestDialog1(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
    ui.tableWidget->setABCVerticalHeaderPolicy();
    ui.tableWidget_2->setABCVerticalHeaderPolicy();
    
}

TableWidgetTestDialog1::~TableWidgetTestDialog1()
{

}

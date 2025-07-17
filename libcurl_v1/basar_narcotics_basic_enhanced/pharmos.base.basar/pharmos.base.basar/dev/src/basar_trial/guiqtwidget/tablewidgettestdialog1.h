#ifndef TABLEWIDGETTESTDIALOG1_H
#define TABLEWIDGETTESTDIALOG1_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include <Qt/QDialog.h>
#include "ui_tablewidgettestdialog1.h"
#pragma warning (pop)

class TableWidgetTestDialog1 : public QDialog
{
    Q_OBJECT

public:
    TableWidgetTestDialog1(QWidget *parent = 0);
    ~TableWidgetTestDialog1();

private:
    Ui::TableWidgetTestDialog1Class ui;
};

#endif // TABLEWIDGETTESTDIALOG1_H

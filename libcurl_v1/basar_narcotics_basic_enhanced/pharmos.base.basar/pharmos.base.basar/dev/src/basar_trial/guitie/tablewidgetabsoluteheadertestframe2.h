#ifndef FRAME3_H
#define FRAME3_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_tablewidgetabsoluteheadertestframe2.h"
#pragma warning (pop)

class TableWidgetAbsoluteHeaderTestFrame2 : public QFrame 
{
    Q_OBJECT

public:
    TableWidgetAbsoluteHeaderTestFrame2(QWidget *parent = 0);
    ~TableWidgetAbsoluteHeaderTestFrame2();

private:
    Ui::TableWidgetAbsoluteHeaderTestFrame2Class ui;
};

#endif // FRAME3_H

#ifndef FRAME2_H
#define FRAME2_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include <QT/QShortcut.h>
#include "ui_tablewidgetabsoluteheadertestframe1.h"
#pragma warning (pop)
#include "selects.h"

class TableWidgetAbsoluteHeaderTestFrame1 : public QFrame
{
    Q_OBJECT

public:
    TableWidgetAbsoluteHeaderTestFrame1(QWidget *parent = 0);
    ~TableWidgetAbsoluteHeaderTestFrame1();

private:
    Ui::TableWidgetAbsoluteHeaderTestFrame1Class ui;
    MatcherRef m_matcher;
	AccessorInstanceRef m_accessor;
    int m_maxrows;

private slots:
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_button_useABC_2_clicked();
    void on_button_useABC_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pb_rows_clicked();
    void on_pb_tableWidget_clicked();
};

#endif // FRAME2_H

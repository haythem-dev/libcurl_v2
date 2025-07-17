#ifndef FRAME5_H
#define FRAME5_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_tablewidgetnocoltestframe.h"
#include "buttablepropertydescriptionvectors.h"
#pragma warning (pop)

class TableWidgetNoColTestFrame : public QFrame
{
    Q_OBJECT

public:
    TableWidgetNoColTestFrame(QWidget *parent = 0);
    ~TableWidgetNoColTestFrame();

private:
    Ui::TableWidgetNoColTestFrameClass ui;

    enum Which
    {
         TABLE_WIDGET_OK,
        TABLE_WIDGET_NOCOLNAMES,
        TABLE_WIDGET_NOCOLS
    };

    std::map<Which,MatcherRef> m_matcher;
    std::map<Which,AccessorInstanceRef> m_accessor;

    void select(MatcherRef & , AccessorInstanceRef &);
    void select(Which i);
    void initTableWidget(Which, BasarTableWidget &);
 
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
      void on_pushButton_clicked();
      void on_pushButton_All_clicked();
};

#endif // FRAME5_H

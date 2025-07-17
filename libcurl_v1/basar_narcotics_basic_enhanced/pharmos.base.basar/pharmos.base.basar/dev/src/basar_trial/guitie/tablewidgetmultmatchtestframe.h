#ifndef FRAME4_H
#define FRAME4_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_tablewidgetmultmatchtestframe.h"
#include "buttablepropertydescriptionvectors.h"
#include "tablewidgetmultmatchselectframe.h"
#pragma warning (pop)
 
class TableWidgetMultMatchTestFrame : public QFrame
{
    Q_OBJECT

public:
    TableWidgetMultMatchTestFrame(QWidget *parent = 0);
    ~TableWidgetMultMatchTestFrame();

private:

    void  setHorizontalHeader(BasarTableWidget & tableWidget, const PropertyDescriptionVector & vec);
    void  select(MatcherRef & matcher,  AccessorInstanceRef & acessor);
void  showNonsence( BasarTableWidget & tableWidget, const QString & text);

   
    Ui::TableWidgetMultMatchTestFrameClass ui;
 
    MatcherRef m_matcherOne;
    AccessorInstanceRef m_accessorOne;
    MatcherRef m_matcherReverse;
    AccessorInstanceRef m_accessorReverse;

  
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_button_shownonsence_clicked();
 
   
    void on_button_Select_clicked();

};

#endif // FRAME4_H

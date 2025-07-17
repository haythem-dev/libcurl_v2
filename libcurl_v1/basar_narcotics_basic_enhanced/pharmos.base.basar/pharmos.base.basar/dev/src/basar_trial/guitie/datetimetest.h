#ifndef DATETIMETEST_H
#define DATETIMETEST_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_datetimetest.h"
#pragma warning (pop)


//------------------------------------------------------------------------------
//------------------- Accessor and StringBuilder Definitions -------------------
//------------------------------------------------------------------------------

BEGIN_QUERY_BUILDER_DECLARATION(DateTimeSelect)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(DateTimeWriteDate)
END_BUILDER_DECLARATION



class DateTimeTest : public QFrame
{
    Q_OBJECT

public:
    DateTimeTest(QWidget *parent = 0);
    ~DateTimeTest();

private:
    Ui::DateTimeTestClass ui;
  
	MatcherRef m_matcher;
	AccessorInstanceRef m_accessor;

private slots:
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_9_clicked();
};

#endif // DATETIMETEST_H

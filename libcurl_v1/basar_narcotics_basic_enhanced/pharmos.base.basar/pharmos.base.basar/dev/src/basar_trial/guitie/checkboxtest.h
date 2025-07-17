#ifndef CHECKBOXTEST_H
#define CHECKBOXTEST_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_checkboxtest.h"
#pragma warning (pop)

BEGIN_QUERY_BUILDER_DECLARATION(CheckBoxSelect1)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(CheckBoxSelect2)
END_BUILDER_DECLARATION

BEGIN_QUERY_BUILDER_DECLARATION(CheckBoxSelect3)
END_BUILDER_DECLARATION


class CheckBoxTest : public QFrame
{
    Q_OBJECT

public:
    CheckBoxTest(QWidget *parent = 0);
    ~CheckBoxTest();

private:
    Ui::CheckBoxTestClass ui;

    MatcherRef m_matcherCB1;
    AccessorInstanceRef m_accessorCB1;

    MatcherRef m_matcherCB2;
    AccessorInstanceRef m_accessorCB2;

    MatcherRef m_matcherCB3;
    AccessorInstanceRef m_accessorCB3;


private slots:
    void on_pushButton_15_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();

};

#endif // CHECKBOXTEST_H

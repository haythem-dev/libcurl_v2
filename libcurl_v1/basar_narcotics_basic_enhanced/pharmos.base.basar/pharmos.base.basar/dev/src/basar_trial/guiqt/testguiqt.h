#ifndef TESTGUIQT_H
#define TESTGUIQT_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4311 4312 4512 4800)
#include "usings.h"
#include <QtWidgets/QMainwindow>
#include "ui_testguiqt.h"
#pragma warning (pop)

class testguiqt : public QMainWindow
{
    Q_OBJECT

public:
    testguiqt(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~testguiqt();

private slots:
    void about();

private:
    Ui::testguiqtClass ui;

	StatusLine m_Statusline;
	MainBackground m_Background;
};

#endif // TESTGUIQT_H

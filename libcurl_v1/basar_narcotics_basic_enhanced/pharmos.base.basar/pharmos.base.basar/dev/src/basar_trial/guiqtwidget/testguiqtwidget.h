#ifndef TESTGUIQT_H
#define TESTGUIQT_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include <Qt/qmainwindow.h>
#include "ui_testguiqtwidget.h"
#include <qt/QMdiArea.h>
#pragma warning (pop)

class Testguiqtwidget : public QMainWindow
{
    Q_OBJECT

public:
    Testguiqtwidget(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Testguiqtwidget();

private slots:
 

    void on_tableWidgetTest1();

private:
    Ui::TestguiqtwidgetClass ui;
	QMdiArea * m_area;

};

#endif // TESTGUIQT_H

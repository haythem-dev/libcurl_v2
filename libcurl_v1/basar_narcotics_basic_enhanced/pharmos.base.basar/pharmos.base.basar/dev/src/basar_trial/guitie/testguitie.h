#ifndef TESTQTTIE_H
#define TESTQTTIE_H


#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <Qt/qmainwindow.h>
#include "ui_testguitie.h"
#pragma warning (pop)

class testguitie : public QMainWindow
{
    Q_OBJECT

public:
    testguitie(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~testguitie();

private:
    Ui::testguitieClass ui;
	
};

#endif // TESTQTTIE_H

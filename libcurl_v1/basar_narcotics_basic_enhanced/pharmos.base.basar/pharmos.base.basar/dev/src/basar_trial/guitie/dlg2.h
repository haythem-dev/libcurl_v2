#ifndef DLG2_H
#define DLG2_H


#pragma warning (push)
#pragma warning(disable: 4311 4312 4127 4251 4512)
#include <Qt/qdialog.h>
#include "ui_dlg2.h"
#pragma warning (pop)


using namespace Ui;

class dlg2 : public QDialog, public dlg2Class
{
    Q_OBJECT

public:
    dlg2(QWidget *parent = 0);
    ~dlg2();
};

#endif // DLG2_H

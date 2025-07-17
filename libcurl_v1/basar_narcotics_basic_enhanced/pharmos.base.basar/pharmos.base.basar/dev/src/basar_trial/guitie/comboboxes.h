#ifndef COMBOBOXES_H
#define COMBOBOXES_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4311 4312 4512 4800)
#include "usings.h"
#include <Qt/qdialog.h>
#include <Qt/qtableview.h>
#include "ui_comboboxes.h"
#pragma warning (pop)

class comboboxes : public QDialog
{
    Q_OBJECT

public:
    comboboxes(QWidget *parent = 0);
    ~comboboxes();

private:
    Ui::comboboxesClass ui;

private slots:
	void on_btTest_clicked();
};

#endif // COMBOBOXES_H

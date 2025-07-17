#ifndef AAA_FRAME2_H
#define AAA_FRAME2_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QtGui/QFrame.h>
#include "ui_frame2.h"
#pragma warning (pop)

class Frame2 : public QFrame
{
	Q_OBJECT

public:
	Frame2(QWidget *parent = 0);
	~Frame2();

private:
	Ui::frame2Class ui;

private slots:
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
};

#endif // FRAME2_H

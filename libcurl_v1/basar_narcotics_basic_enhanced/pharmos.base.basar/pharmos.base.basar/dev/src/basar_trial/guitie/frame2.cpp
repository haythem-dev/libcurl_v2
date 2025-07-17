#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include "frame2.h"
#include "QTGui/QShortcut.h"
#pragma warning (pop)

Frame2::Frame2(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	QShortcut * a = new QShortcut(this);
	a->setKey(QKeySequence("F11"));
	connect(a, SIGNAL(activated() ), this, SLOT(on_pushButton_clicked()));
	connect(a, SIGNAL(activatedAmbiguously()), this, SLOT(on_pushButton_clicked()));

}

Frame2::~Frame2()
{

}


void Frame2::on_pushButton_clicked()
{
  static int one = 1;
  one++;
  QString num;
  num.setNum(one);
  for (int i=0; i<1000; ++i)
  {
	  ui.textEdit->append (num);
  }
}

void Frame2::on_pushButton_2_clicked()
{
  
}
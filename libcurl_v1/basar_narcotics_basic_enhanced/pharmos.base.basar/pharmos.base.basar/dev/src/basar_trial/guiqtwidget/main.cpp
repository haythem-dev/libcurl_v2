#pragma warning (push)
#pragma warning(disable: 4127 4311 4312 4512 4251) 
#include <Qt/qapplication.h>
#include <Qt/qpainter.h>
#include <Qt/qscrollarea.h>
#include <Qt/qprintengine.h>
#include <Qt/qpaintengine.h>
#include <Qt/qprintdialog.h>
#include <QtGui/qimagereader.h>
#include <QtGui/qimagewriter.h>
#include <Qt/qtranslator.h>
#pragma warning (pop)

#include "testguiqtwidget.h"


//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int ret = 0;
	

	QApplication a(argc, argv);
    Testguiqtwidget w;
		w.show();
		a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
		ret = a.exec();
	

   return ret;
}

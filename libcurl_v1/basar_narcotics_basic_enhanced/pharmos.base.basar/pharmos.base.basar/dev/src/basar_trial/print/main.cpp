#include "print.h"

#include <libbasarcmnutil_print.h>
#include <libbasarguiqt.h>

#include <QtWidgets/QApplication>

void preview()
{
	basar::gui::qt::PrintPreview preview(
        "../../basar_trial/print/form.ps",
        0, 
        "test for ghostscript: הצü");
	
	preview.setOutputFormats(
        static_cast<basar::gui::qt::OutputEnum>(
            basar::gui::qt::PDF | 
            basar::gui::qt::WIN_PRINT));
	
	preview.show();
}

void pdfprint()
{
    basar::cmnutil::InterpretPrint print;

    print.print("../../basar_trial/print/s1.pdf");
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    print w;
    //w.show();
    int ret = 0;
    
    //ret = a.exec();

    preview();
    //pdfprint();

    return ret;
}


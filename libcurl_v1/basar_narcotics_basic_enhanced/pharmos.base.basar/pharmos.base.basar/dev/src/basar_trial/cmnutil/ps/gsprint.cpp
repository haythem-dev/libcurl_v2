
#include <libbasarcmnutil.h>

#include <testcmnutil.h>

#include <printdata.h>

#include "screenprint.h"

#include "gsprint.h"

using basar::cmnutil::InterpretPrint;

#define DATA_PATH	"/marathon/ssuk/trunk/dev/src/basar_trial/cmnutil/print"
#define OUT_PATH	"/temp/ps/out"

//------------------------------------------------------------------------------
void print_win()
{
	InterpretPrint pr;

	// --- default case -------------
	pr.print(DATA_PATH "/colourswin.ps");

	// --- explicit dialog
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DIALOG);
	pr.print(DATA_PATH "/colourswin.ps");

	// --- default printer w/o dialog
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_STANDARD);
	pr.print(DATA_PATH "/colourswin.ps");

	// --- dedicated printer w/o name -> exception
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED);

	try
	{
		pr.print(DATA_PATH "/colourswin.ps");
	}
	catch (const basar::InvalidParameterException & except)
	{
		printf("exception %s\n", except.what().c_str());
	}

	// --- dedicated printer with name
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP - High Quality");
	pr.print(DATA_PATH "/colourswin.ps");

	// --- dedicated printer with name
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP - High Quality");
	pr.setPrintDpi	 (600);
	pr.setPrintBpp   (basar::cmnutil::InterpretPrint::PRINT_COLOR_24);
	pr.print(DATA_PATH "/colourswin.ps");


	// --- dedicated printer with name
	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP - High Quality");
	pr.setPrintBpp   (basar::cmnutil::InterpretPrint::PRINT_MONO);
	pr.setPrintDpi	 (150);
	pr.print(DATA_PATH "/colourswin.ps");
}

//------------------------------------------------------------------------------
void print_win2()
{
	gui_for_test::PrintData dataprovider1(DATA_PATH "/bookdata.txt",		// data that will be put in form
										  "NarcoticsBook.ps"  ,				// dynamic form file that expects data
										  "NarcoticsBook"     );			// form name

	gui_for_test::PrintData dataprovider2(DATA_PATH "/oceformular.data",	// data that will be put in form
										  "oceformular.ps"  ,				// dynamic form file that expects data
										  "OCEFormular"     );				// form name

	InterpretPrint pr;

	pr.setPrintDuplex(false);
	pr.setPrintPaperSize(basar::cmnutil::InterpretPrint::PS_A4);

	// --- default printer w/o dialog
	pr.setPrintSelect(	//basar::cmnutil::InterpretPrint::PRINTER_DIALOG);
						//basar::cmnutil::InterpretPrint::PRINTER_STANDARD);
						basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP");
						//basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "TallyGenicom Intelliprint 9035N (PS)");
						//basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "\\\\FU1S0001\\AE_Laser");

	//pr.print(dataprovider1, 1, 2);
	pr.print(DATA_PATH "/narclong.ps");
	//pr.print(DATA_PATH "/4pages_static.ps");
	//pr.print(DATA_PATH "/my/NarcoticsBook.ps");
	//pr.savePdf(DATA_PATH "/my/NarcoticsBook.ps", DATA_PATH "/my/NarcoticsBook.pdf");
}

//------------------------------------------------------------------------------
void print_pdf()
{
	InterpretPrint pdf;

	pdf.savePdf(DATA_PATH "/colourswin.ps", OUT_PATH "/colourswin.pdf");
}

//------------------------------------------------------------------------------
void print_preview()
{
}

//------------------------------------------------------------------------------
void test_gsprint()
{
	printf("\n");
	printf("--- test_gsprint ---------------------------\n");
	printf("\n");

	print_win2    ();
	//print_pdf    ();
	//print_preview();
}

void test_xxxx()
{
#ifdef TEST1
	{
		basar::cmnutil::InterpretPrint	pr1;
		basar::cmnutil::InterpretPrint	pr2;
	}

	{
		basar::cmnutil::InterpretPrint	pr1;
		basar::cmnutil::InterpretPrint	pr2;
	}
#endif	// TEST1

#define PRINTPATH	"/marathon/lib/basar/test/component/cmnutil/print/"

#ifndef TEST2
	{


		// 1.
		gui_for_test::PrintData dataprovider("colourswin.ps",				// static form file without data
											 "colours"      );				// form name

		basar::cmnutil::InterpretPrint prPdf;
/*
		prPdf.savePdf(PRINTPATH "colourswin.ps",							// static form file without data
			          PRINTPATH "colourswin_file.pdf");
		prPdf.savePdf(dataprovider, 
			          PRINTPATH "colourswin_pipe.pdf");

		prPdf.print  (dataprovider, 1, 1);
*/

		// 2.

/*
		gui_for_test::PrintData dataprovider2(PRINTPATH "oceformular.data",	// data that will be put in form
											  "oceformular.ps"  ,			// dynamic form file that expects data
											  "OCEFormular"     );			// form name
*/
		gui_for_test::PrintData dataprovider2(PRINTPATH "bookdata.txt",	// data that will be put in form
											  "NarcoticsBook.ps"  ,			// dynamic form file that expects data
											  "NarcoticsBook"     );			// form name

		basar::cmnutil::InterpretPrint prPdf2;

		//prPdf2.savePdf(dataprovider2, PRINTPATH "oceformular_pipe.pdf", 2, 19);
		//prPdf2.savePdf(PRINTPATH "oceformular.trigger",	PRINTPATH "oceformular_file.pdf");

		//prPdf2.setPrintBpp(InterpretPrint::PRINT_USER_SETTING);
		//prPdf2.setPrintDpi(InterpretPrint::PrintProp::DPI_USER_SETTING);
		//prPdf2.print  (PRINTPATH "oceformular.trigger");					// trigger + data

		//prPdf2.savePdf(dataprovider2, PRINTPATH "oceformular_pipe.pdf", 3, 3);
		//prPdf2.setPrintBpp(InterpretPrint::PRINT_COLOR_4);
		prPdf2.print  (dataprovider2, 3, 27);
		//prPdf2.print  (PRINTPATH "bookdata.trigger");
	}

#endif

#ifdef TEST3
	{
		// --- display: "preview"	

		gui_for_test::ScreenPrint screenprint;

		basar::cmnutil::InterpretPrint prDisp;

#ifdef DISP0
		printf("\ndisplay prDisp 0\n");

		gui_for_test::PrintData dataprovider0(PRINTPATH "bookdata.txt",		// data that will be put in form
											  "NarcoticsBook.ps"  ,			// dynamic form file that expects data
											  "NarcoticsBook"     );		// form name

		prDisp.display(dataprovider0, screenprint, 1, 60);		// bpp, dpi
		//prDisp.savePdf(dataprovider0, PRINTPATH "NarcoticsBook.pdf");

#endif


#ifdef DISP1
		printf("\ndisplay prDisp 1\n");

		gui_for_test::PrintData dataprovider4(PRINTPATH "oceformular.data",	// data that will be put in form
											  "oceformular.ps"  ,			// dynamic form file that expects data
											  "OCEFormular"     );			// form name

		prDisp.display(dataprovider4,
			           screenprint,
				   	   32,		// bit per pixel (color depth)
					   96);		// dpi

#endif

#ifndef DISP2

		printf("\ndisplay prDisp 2\n");

		prDisp.setDisplayBpp(InterpretPrint::DISP_MONO);
		prDisp.display(PRINTPATH "oceformular.trigger", screenprint);

#endif

#ifdef DISP3
		// ------
		printf("\ndisplay prDisp 3\n");

		gui_for_test::PrintData dataprovider5("colourswin.ps",		// static form file without data
											  "colours"      );		// form name


		prDisp.display(dataprovider5,
			           screenprint,
				   	   32,		// bit per pixel (color depth)
					   96);		// dpi
#endif

#ifdef DISP4
		printf("\ndisplay prDisp 4 \n");

		prDisp.display(PRINTPATH "colourswin.ps",
			            screenprint,
				   		32,		// bit per pixel (color depth)
						96);		// dpi
#endif
	}
#endif	// TEST1
}


//------------------------------------------------------------------------------

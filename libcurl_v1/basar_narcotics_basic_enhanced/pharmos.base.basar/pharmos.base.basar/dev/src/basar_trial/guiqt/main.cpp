#pragma warning (push)
#pragma warning(disable: 4127 4311 4312 4512) 
#include <QtWidgets/qapplication.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qscrollarea.h>
#include <QtPrintSupport/qprintengine.h>
#include <QtGui/qpaintengine.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QtGui/qimagereader.h>
#include <QtGui/qimagewriter.h>
#include <QtCore/qtranslator.h>
#pragma warning (pop)

#include "usings.h"
#include "printdata.h"
#include "testguiqt.h"


//----------------------------------------------------------------------------
bool basar_define()
{
	basar::DefinitionStruct def;
	def.copyright = "copyright by PHOENIX AG & Co KG";
	def.programDescription = "this is a test";
	def.version = "00.00.04.01";
	basar::gui::qt::Manager::getInstance().init(def);
	basar::DllStruct dll;
	dll.name = "libbasarcmnutil";
	dll.pkgVersion = basar::cmnutil::getVersion();
	basar::gui::qt::Manager::getInstance().initDll(dll);
	dll.name = "libbasarguiqt";
	dll.pkgVersion = basar::gui::qt::getVersion();
	return true;
}

bool dummybasar = basar_define();

//----------------------------------------------------------------------------
bool basar_init()
{
	basar::InitStruct initStruct;
	initStruct.database = "pps1g";
	initStruct.host = "pps1";
	initStruct.dokulink = "http://ntfuln1/doku/public/anwendung/kundendispo/index.html";
	basar::gui::qt::Manager::getInstance().init(initStruct);
	return true;
}

//----------------------------------------------------------------------------
class MyPrinter : public QPrinter
{
public:
	MyPrinter(){};
	~MyPrinter(){};

	void setEngines(QPrintEngine * pre, QPaintEngine * pae){
		QPrinter::setEngines(pre, pae);
	};
	
};

//----------------------------------------------------------------------------
void simple_printing()
{
	//MyPrinter *pPrinter = new MyPrinter();
	
	QPrinter *pPrinter = new QPrinter();
	
	//QPrintDialog printDlg(pPrinter);

	//printDlg.setEnabledOptions(QAbstractPrintDialog::PrintSelection | QAbstractPrintDialog::PrintPageRange | 
	//							QAbstractPrintDialog::PrintToFile | QAbstractPrintDialog::PrintCollateCopies);
	//printDlg.setEnabledOptions(QAbstractPrintDialog::PrintPageRange);
	
	pPrinter->setOrientation(QPrinter::Landscape); 
	
	// from - to
	/*printDlg.setPrintRange(QAbstractPrintDialog::PageRange);
	printDlg.setMinMax(1,3);
	printDlg.setFromTo(1,2);*/
	
	//if (printDlg.exec() != QDialog::Accepted)
    //  	return;

	pPrinter->setOutputFormat(QPrinter::PdfFormat);		
	pPrinter->setOutputFileName("C:/temp/printoutput.pdf");
	pPrinter->setOrientation(QPrinter::Landscape); // set orientation after setOutputFormat(QPrinter::PdfFormat), then it works also for PDF!
	
	QPainter paint;
	
	// --- paint to printer ---
	if ( !paint.begin( pPrinter ) )
		return;

	bool b;
	b = pPrinter->paintEngine()->isActive();

	paint.setPen(Qt::blue);
	QFont font("Helvetica", 10, QFont::Bold);
    paint.setFont(font);
	paint.drawText( 10, 10, "The Test Text");
//THQT after (physical) printing picture, no more output to printer, only empty page !
	QPixmap pic = QPixmap(":/image/exit.png");
	paint.drawPixmap(150,150,pic);
	

/* ------ Tests
	b = pPrinter->paintEngine()->isActive();
	QPainter *pPainter;
	pPainter = pPrinter->paintEngine()->painter();
	QPaintEngine::Type typ;
	typ = pPrinter->paintEngine()->type();
	b = pPrinter->paintEngine()->testDirty(QPaintEngine::AllDirty);
	b = pPrinter->paintEngine()->hasFeature(QPaintEngine::AllFeatures);
	//	QPaintEngine::DirtyFlags flags = pPrinter->paintEngine()->state();

	QPrinter::PrinterState state;
	state = pPrinter->printEngine()->printerState();*/

	/* //how it works on std-printer, creating new printer after each printed picture
	paint.end();
	delete pPrinter;

	pPrinter = new QPrinter();
	paint.begin( pPrinter );
-------- */


	paint.drawText( 100, 100, "2The Test Text2");
	pPrinter->newPage();
	paint.drawText( 10, 10, "The Second Page");
	paint.end();
	
	delete pPrinter;
}

#include <direct.h> // for getting current working directory for loading translation-file
	

//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int ret = 0;
	
{
	basar::I18nString appl = "TRANSORD";
	basar::I18nString title = "testguiqt";

	QApplication a(argc, argv);

//----------- Translator for preview / about ----------------------------------------------
	basar::I18nString language = "DE";//basar::login::Manager::getInstance().getLanguageID();
	language.lower();
	QString file = "libbasarguiqt_";
	file.append(language.data());
	
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH ); // current working directory
	QString cwd = buffer;
	cwd += "/../multilang";
	
	QTranslator tr_guiqt;
	if ( tr_guiqt.load( file, cwd ) == false )
	{
		// logging -> language xy not found, default english
	}
	else
	{
		// logging -> language xy loaded
	}
	QApplication::instance()->installTranslator(&tr_guiqt);

	try
	{

//----------- Background ----------------------------------------------
	testguiqt w;

	//bool loginSuccess = basar::login::Manager::getInstance().showDlg(appl,title,2);
/*
	if (!loginSuccess) // Anwendung, Fenstertitel, Login-Versuche
	{
		printf("\nLOGIN with dlg FAILED\n");
	}
	else
*/
	{
		basar_init();
		w.show();
		a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
		ret = a.exec();
	}

	{
		basar::login::Manager::clear();
		basar::db::aspect::Manager::clear();
	}


//----------- Picture formats ---------------------------------------------
/*	QStringList list1 = QPicture::inputFormatList();
	QList<QByteArray> list2 = QPictureIO::inputFormats();

	QList<QString>::iterator it1 = list1.begin();
	QString str;
	while ( it1 != list1.end() )
	{
		str = *it1;
		++it1;
	}

	QList<QByteArray>::iterator it2 = list2.begin();
	QByteArray str2;
	while ( it2 != list2.end() )
	{
		str2 = *it2;
		++it2;
	}

	list1 = QPicture::outputFormatList();
	list2 = QPictureIO::outputFormats();

	it1 = list1.begin();
	while ( it1 != list1.end() )
	{
		str = *it1;
		++it1;
	}

	it2 = list2.begin();
	while ( it2 != list2.end() )
	{
		str2 = *it2;
		++it2;
	}

	QList<QByteArray> list3 = QImageReader::supportedImageFormats();
	QList<QByteArray>::iterator it3 = list3.begin();
	QByteArray str3;
	while ( it3 != list3.end() )
	{
		str3 = *it3;
		++it3;
	}

	list3 = QImageWriter::supportedImageFormats();
	it3 = list3.begin();
	while ( it3 != list3.end() )
	{
		str3 = *it3;
		++it3;
	}
*/
//#ifdef PREVIEW
//----------- Print Preview ---------------------------------------------

#ifdef XXXXX
	basar::I18nString file  = "..\\..\\basar_trial\\guiqt\\print\\colourswin.ps";
	basar::I18nString file2 = "..\\..\\basar_trial\\guiqt\\print\\Stock_Count_List_data.ps";
	
	basar::gui::qt::PrintPreview * preview = new basar::gui::qt::PrintPreview(file);
	//basar::I18nString caption = "Caption";
	//basar::I18nString pdfPath = "C:/temp/printoutput.pdf";
	//basar::gui::qt::PrintPreview * preview = new basar::gui::qt::PrintPreview(file, 0, caption, QPrinter::Landscape, QPrinter::PdfFormat, pdfPath );
	preview->setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP - High Quality");
	preview->show();

	//preview->hide();
	delete preview;
#endif		
	
	PrintData data("Stock_Count_List.ps", "StockCountList",
					"..\\..\\basar_trial\\guiqt\\print\\stockdata.txt");
	
	/*PrintData data2("form_customer.ps", "customer",
					"C:\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\customer.dat");*/
	
	//PrintData data3("NarcHYR4.ps", "NarcHYR4", "C:\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\hyr4.txt");

	//PrintData data3("NarcoticsBook.ps", "NarcoticsBook", "\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\bookdata.txt");

		                                                 //"C:\\marathon\\1\\print\\bookdata.txt");
	//PrintData data3("colourswin.ps", "colours", "\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\bookdata.txt");
	//PrintData data3("NarcArticleToCust.ps", "NarcArticleToCust", "\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\arttocust.txt");
	
	
	basar::gui::qt::PrintPreview * preview2 = new basar::gui::qt::PrintPreview(data);
	preview2->show();
	delete preview2;

	{
		basar::cmnutil::InterpretPrint pr;
		pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED, "FreePDF XP");
		pr.print(data);
	}


	//basar::gui::qt::PrintPreview * preview3 = new basar::gui::qt::PrintPreview(data3, 0, "caption", QPrinter::Landscape, basar::gui::qt::PDF, "temp" );
	//basar::gui::qt::PrintPreview preview3;
	//preview3.setPrintData(data3);
	//preview3.setOutputFormats(static_cast<basar::gui::qt::OutputEnum>(basar::gui::qt::PDF | basar::gui::qt::PS_PRINT | basar::gui::qt::WIN_PRINT) );
	//preview3.setOutputFormats(basar::gui::qt::PREVIEW_ONLY );

	//preview3.setPrintBpp(basar::cmnutil::InterpretPrint::PRINT_USER_SETTING);
	//preview3.setPrintDpi(basar::cmnutil::InterpretPrint::PrintProp::DPI_USER_SETTING);

	//preview3.show();	

/*
	preview3.setTitle("my preview");
	//preview3.setOrientation(QPrinter::Landscape);
	preview3.setOutputFormat(QPrinter::PdfFormat);
	preview3.show();	
*/
	/*basar::gui::qt::PrintPreview preview4(data2);
	preview4.setOrientation(QPrinter::Landscape);
	preview4.show();

	preview3.setPrintData(file2);
	preview3.show();*/

	/*basar::I18nString testfile = "C:\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\col.ps";
	basar::I18nString testfile = "C:\\MARATHON\\lib\\basar\\test\\component\\guiqt\\print\\t.ps";
	basar::gui::qt::PrintPreview preview4(testfile);
	preview4.show();*/
	
//#endif

//----------- simple printing ---------------------------------------------
//	simple_printing();

	}
	catch (const basar::Exception & except)
	{
		VarString msg;
		msg.format("\n"
			       "=== exception caught ===========================================================\n"
				   "\n"
				   "%s\n", 
				   except.what().c_str());
		basar::gui::tie::warningMsgBox(0, msg.c_str());
	}
	catch (const std::exception &)
	{
		basar::gui::tie::warningMsgBox(0, "=== std exception caught ====");
	}
	catch (...)
	{
		basar::gui::tie::warningMsgBox(0, "=== unknown exception caught ====");
	}

}

	return ret;
}



//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for qt-printpreview
 *  \author Thomas Hörath 
 *  \date   17.01.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PRINTPREVIEWNEW_H
#define GUARD_PRINTPREVIEWNEW_H

//----------------------------------------------------------------------------
#include "libbasarguiqt_printpreview.h"
#include "libbasarcmnutil_print.h"
//----------------------------------------------------------------------------
#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4512 4800)
#include <QtWidgets/QDialog>
#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QMainWindow>
#pragma warning (pop)

//----------------------------------------------------------------------------
class QMenu;
class QMenuBar;
class QScrollArea;
class QComboBox;
class QLineEdit;
class QLabel;
class QToolBar;
class QVBoxLayout;
class QIntValidator;

namespace basar	{
namespace gui	{
namespace tie	{
	enum WidgetReturnEnum;
}}}

//----------------------------------------------------------------------------
namespace basar	{
namespace gui	{
namespace qt	{

//----------------------------------------------------------------------------
class PictureList;

//----------------------------------------------------------------------------
/*! \brief  Class to create a Print Preview in Qt - applications of basar. 

	Preview manager main form. Handles paging, zooming and so on.
	Final class. 
	Throws RuntimeException */
class PrintPreviewOL : public PrintPreview
{
public:
	/*! \brief Constructor of print preview class with psFile
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreviewOL(	const basar::I18nString & rPsFile,									//!< name of postscript file containing print job; if no path is given search is continued in default-paths
										QWidget *                 pParent      = 0,							//!< parent widget of preview window, default 0	
										const basar::I18nString & rTitle       = "",						//!< title to add to window title, default empty
										QPrinter::Orientation     orientation  = QPrinter::Portrait,		//!< current orientation, default portrait
										OutputEnum		   outputPossibilities = static_cast<OutputEnum>(WIN_PRINT | PDF),	//!< current output format, default native
										const basar::I18nString & rPdfPath     = ""							//!< path to save pdf-output to, default empty
										);

	/*! \brief Constructor of print preview class with PrintDataProvider
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreviewOL(	basar::cmnutil::IPrintDataProvider & rDataProvider,							//!< reference to data provider class
										QWidget *                            pParent      = 0,						//!< parent widget of preview window, default 0	
										const basar::I18nString&             rTitle       = "",						//!< title to add to window title, default empty
										QPrinter::Orientation                orientation  = QPrinter::Portrait,		//!< current orientation, default portrait
										OutputEnum				      outputPossibilities = static_cast<OutputEnum>(WIN_PRINT | PDF), //!< current output format, default native
										const basar::I18nString&             rPdfPath     = ""						//!< path to save pdf-output to, default empty
									);

	/*! \brief standard constructor of print preview class; preview can only be started if print data are set before calling show
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreviewOL();

	/*! \brief Destructor of print preview class
		\n     no-throw */
	LIBBASARGUIQT_API	~PrintPreviewOL();

	
	/*! \brief start up preview! print data has to be set!
		\n     RuntimeException */
	LIBBASARGUIQT_API	basar::gui::tie::WidgetReturnEnum	show	();

	//PrintPreviewNew(const basar::I18nString& rPsFile, QWidget* pParent, const basar::I18nString& rTitle, QPrinter::Orientation orientation, OutputEnum outputPossibilities, const basar::I18nString& rPdfPath);

	/*! \brief set path where pdf-file is stored after printing
		\n     no-throw */
	LIBBASARGUIQT_API	void setPdfPath	(const basar::I18nString& rPdfPath		//!< path where to store pdf
										);

	/*! \brief set server and printer, used for raw print
		\n     no-throw */
	LIBBASARGUIQT_API	void setRawPrintInfo(const basar::VarString& rServer,		//!< server for raw print
											 const basar::VarString& rPrinter = ""	//!< printer for raw print (if empty -> 9100, else lpr)
										);

	LIBBASARGUIQT_API	void setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
		const basar::I18nString& printerCfgName	//!< printer name)
	);

protected:
	cmnutil::RawPrint* _rawPrint;
	QWidget* _rParent;

private:
	QMainWindow* getMainWindow();
	void PrintPreviewOL::showPdf(const QString& fileName);
	void PrintPreviewOL::InitRawPrint(QWidget* rParent);

	basar::VarString		m_RwprServer;			//!< server for raw print
	basar::VarString		m_RwprPrinter;			//!< printer for raw print
};

//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar

#endif // GUARD_PRINTPREVIEWNEW_H

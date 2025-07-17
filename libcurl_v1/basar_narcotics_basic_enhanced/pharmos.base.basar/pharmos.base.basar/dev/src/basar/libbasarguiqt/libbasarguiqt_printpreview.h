//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for qt-printpreview
 *  \author Thomas Hörath 
 *  \date   17.01.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PRINTPREVIEW_H
#define GUARD_PRINTPREVIEW_H

//----------------------------------------------------------------------------
#ifndef LIBBASARGUIQT_API
	#ifdef _WIN32
		#ifdef LIBBASARGUIQT_EXPORTS
			#define LIBBASARGUIQT_API __declspec(dllexport)	//!< dll exported
		#else
		    #define LIBBASARGUIQT_API __declspec(dllimport)	//!< dll imported
	    #endif 
	#else 
		#define LIBBASARGUIQT_API
	#endif 
#endif

//----------------------------------------------------------------------------
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_print.h"

//----------------------------------------------------------------------------
#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4512 4800)
#include <QtWidgets/QDialog>
#include <QtPrintSupport/QPrinter>
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
//! OutputEnum defines all possible output formats for printing
enum OutputEnum {	
	PREVIEW_ONLY= 0x00,		//!< show only preview, no possibility to print what is seen
	PDF			= 0x01,		//!< allow pdf-generation of what is seen in preview		
	WIN_PRINT	= 0x02,		//!< allow windows-printing of what is seen in preview
	PS_PRINT	= 0x04		//!< allow postscript-printing of what is seen in preview
};

//----------------------------------------------------------------------------
/*! \brief  Class to create a Print Preview in Qt - applications of basar. 

	Preview manager main form. Handles paging, zooming and so on.
	Final class. 
	Throws RuntimeException */
class PrintPreview : public QDialog, 
	                        cmnutil::IScreenPrint
{
	Q_OBJECT // macro for connecting qt - slots

public:

    //! ZoomEnum defines all known zoom factors
    enum ZoomEnum		//! zoom factors
    {
	    ZOOM_FIRST = 0,			//!< first valid zoom value, must be in sync with combobox

	    ZOOM_25  = ZOOM_FIRST,	//!<  25 %
	    ZOOM_50,				//!<  50 %
	    ZOOM_75,				//!<  75 %
	    ZOOM_100,				//!< 100 %
	    ZOOM_125,				//!< 125 %
	    ZOOM_150,				//!< 150 %
	    ZOOM_200,				//!< 200 %

	    ZOOM_LAST = ZOOM_200,	//!< last valid zoom value
        ZOOM_INVALID            //!< invalid value (needed for initialization of member)
    };

	/*! \brief Constructor of print preview class with psFile
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreview(	const basar::I18nString & rPsFile,									//!< name of postscript file containing print job; if no path is given search is continued in default-paths
										QWidget *                 pParent      = 0,							//!< parent widget of preview window, default 0	
										const basar::I18nString & rTitle       = "",						//!< title to add to window title, default empty
										QPrinter::Orientation     orientation  = QPrinter::Portrait,		//!< current orientation, default portrait
										OutputEnum		   outputPossibilities = static_cast<OutputEnum>(WIN_PRINT | PDF),	//!< current output format, default native
										const basar::I18nString & rPdfPath     = ""							//!< path to save pdf-output to, default empty
										);

	/*! \brief Constructor of print preview class with PrintDataProvider
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreview(	basar::cmnutil::IPrintDataProvider & rDataProvider,							//!< reference to data provider class
										QWidget *                            pParent      = 0,						//!< parent widget of preview window, default 0	
										const basar::I18nString&             rTitle       = "",						//!< title to add to window title, default empty
										QPrinter::Orientation                orientation  = QPrinter::Portrait,		//!< current orientation, default portrait
										OutputEnum				      outputPossibilities = static_cast<OutputEnum>(WIN_PRINT | PDF), //!< current output format, default native
										const basar::I18nString&             rPdfPath     = ""						//!< path to save pdf-output to, default empty
									);

	/*! \brief standard constructor of print preview class; preview can only be started if print data are set before calling show
		\n     no-throw */
	LIBBASARGUIQT_API	PrintPreview();

	/*! \brief Destructor of print preview class
		\n     no-throw */
	LIBBASARGUIQT_API	~PrintPreview();

	
	/*! \brief start up preview! print data has to be set!
		\n     RuntimeException */
	LIBBASARGUIQT_API	basar::gui::tie::WidgetReturnEnum	show	();


	/*! \brief set IPrintDataProvider containing data to preview
		\n     no-throw */
	LIBBASARGUIQT_API	void setPrintData(basar::cmnutil::IPrintDataProvider  & rDataProvider	//!< reference to data provider class
										);

	/*! \brief set path of ps-file containing data to preview
		\n     throw no-throw */
	LIBBASARGUIQT_API	void setPrintData(const basar::I18nString& rPsFile		//!< name of postscript file containing print job; if no path is given search is continued in default-paths
										);

	/*! \brief set parent of preview dialog
		\n     no-throw */
	LIBBASARGUIQT_API	void setPreviewParent	(QWidget * pParent						//!< parent widget of preview window
										);

	/*! \brief set window title of preview dialog
		\n     no-throw */
	LIBBASARGUIQT_API	void setTitle	(const basar::I18nString& rTitle		//!< title to add to window title
										);

	/*! \brief set orientation of preview pictures
		\n     no-throw */
	LIBBASARGUIQT_API	void setOrientation	(QPrinter::Orientation orientation	//!< portrait or landscape
										);

	/*! \brief set format for the output when pressing print button
		\n     no-throw */
	LIBBASARGUIQT_API	void setOutputFormats(OutputEnum outputPossibilities//!< pdf / win-print / ps-print / none
										);

	/*! \brief set path where pdf-file is stored after printing
		\n     no-throw */
	LIBBASARGUIQT_API	void setPdfPath	(const basar::I18nString& rPdfPath		//!< path where to store pdf
										);

	/*! \brief set server and printer, used for raw print
		\n     no-throw */
	LIBBASARGUIQT_API	void setRawPrintInfo(const basar::VarString& rServer,		//!< server for raw print
											 const basar::VarString& rPrinter = ""	//!< printer for raw print (if empty -> 9100, else lpr)
										);

	//! set print color depth						\n no-throw
	LIBBASARGUIQT_API	void setPrintBpp(basar::cmnutil::InterpretPrint::PrintBppEnum eBpp		//!< print color depth in bpp
									    );
	//! set maximum print resolution				\n no-throw	
	LIBBASARGUIQT_API	void setPrintDpi(Int32 dpi												//!< maximum print resolution in dpi
										);
	//! select Windows printer						\n no-throw
	LIBBASARGUIQT_API	void setPrintSelect	(basar::cmnutil::InterpretPrint::PrintSelectEnum eSelect,			//!< printer selection 
											 const basar::VarString						   & rPrinterName = ""	//!< Windows printer name
											);
    //! set resolution of print preview (dpi of 150 is normally ok for preview)             \n no-throw
    LIBBASARGUIQT_API void setPreviewDpi( basar::Int32 previewDpi                   //!< preview resolution in dpi
                                        );
    //! set starting zoom factor (otherwise it's calculated)    \n no-throw
    LIBBASARGUIQT_API void setStartingPreviewZoomLevel( ZoomEnum startingPreviewZoomLevel //!< starting zoom factor requested
                                                      );

protected slots:
	/*! \brief slot for combobox select changed (zoom)
		\n     no-throw */
	void slotZoom( const QString & rZoom	//!< selected value in combobox
					);

	/*! \brief slot for button zoom +
		\n     no-throw */
	void zoomPlus();
	
	/*! \brief slot for button zoom -
		\n     no-throw */
	void zoomMinus();
	
	/*! \brief slot for button print
		\n     no-throw */
	void slotPrint();
	
	/*! \brief slot for button next page
		\n     no-throw */
	void slotNext();
	
	/*! \brief slot for button previous page
		\n     no-throw */
	void slotPrev();
	
	/*! \brief slot for button first page
		\n     no-throw */
	void slotFirst();
	
	/*! \brief slot for button last page
		\n     no-throw */
	void slotLast();
	
	/*! \brief slot for button go to page
		\n     no-throw */
	void slotGoTo();

	/*! \brief slot for combobox select changed (orientation)
		\n     no-throw */
	void slotOrientation(const QString & cbVal	//!< text value in combobox
						);

	/*! \brief slot for combobox select changed (output format)
		\n     no-throw */
	void slotOutputFormat(const QString & cbVal	//!< text value in combobox
						);

protected:
	QAction *m_pActionPrint;				//!< action of print button
	QAction *m_pActionFrst;					//!< action of button - go to first page
	QAction *m_pActionNext;					//!< action of button - go to next page
	QAction *m_pActionPrev;					//!< action of button - go to previous page
	QAction *m_pActionLast;					//!< action of button - go to last page

protected:
	/*! \brief start up of preview + qt-widget resources
		\n     no-throw */
	void init	( );
		
	/*! \brief fill output combo with available formats
		\n     no-throw */
	void fillOutputCb			( );
	
	/*! \brief paint current preview page on main widget
		\n     no-throw */
	void refreshPage			( );

	//! get number of pictures in list \n no-throw
	basar::Int32 getPageCount	() const;			

	//! compute zoom level so that it fits in scroll area \n no-throw
	void computeZoom				();

	/*! \brief zoom all pages
		\n     no-throw */
	void zoom					( );

	/*! \brief call display() from cmnutil::PrintPs with callbacks and reset iterator of picturelist
		\n     no-throw */
	void callDisplay();


	static const Int32		s_Dpi;				//!< dots per inch of picture
	static const basar::cmnutil::InterpretPrint::DisplayBppEnum	
							s_BitsPixel;		//!< bits per pixel of picture
	static const Float64	s_ZoomFactor[];		//!< zoom level factor, corresponds with ZoomEnum

	PictureList *			m_pPicList;				//!< collection of pictures to preview
	
	basar::Int32			m_PageNo;				//!< number of current picture on gui
	
	QPrinter::Orientation	m_Orientation;			//!< current orientation
	OutputEnum				m_OutputPossibilities;	//!< print output possibilities that user can select
	OutputEnum				m_OutputSelected;		//!< print output selected in m_pCbOutputFormat

	
	QScrollArea *			m_pScrollArea;			//!< widget's scroll area
	QComboBox	*			m_pCbZoom;				//!< combo box of zoom levels in toolbar
	QComboBox	*			m_pCbOrientation;		//!< combo box of orientation in toolbar
	QComboBox	*			m_pCbOutputFormat;		//!< combo box of output format in toolbar
	QLineEdit	*			m_pEdFrom;				//!< edit to input page where printing is started from
	QLabel		*			m_pLabelFrom;			//!< label "from" for edit description
	QLabel		*			m_pLabelTo;				//!< label "to" for edit description
	QLineEdit	*			m_pEdTo;				//!< edit to input page where printing should end
	QLabel		*			m_pPreviewPic;			//!< label containing one print page to preview
	QToolBar	*			m_pToolbar;				//!< toolbar of print preview
	QMenuBar	*			m_pMenuBar;				//!< main menu bar of print preview
	QMenu		*			m_pFileMenu;			//!< 1st part of menu bar
	QMenu		*			m_pNavigMenu;			//!< 2nd part of menu bar
	QVBoxLayout *			m_pMainLayout;			//!< main layout of print preview dialog
	QLabel		*			m_pStatusLine;			//!< label containing page status in toolbar	
	QIntValidator *         m_pPageValid;			//!< from/to page validator
	

	basar::Int32			m_Width;				//!< width of pages to preview
	basar::Int32			m_Height; 				//!< height of pages to preview
	basar::Int32			m_Raster;				//!< raster of pages to preview
	UInt8 *					m_pImage;				//!< name of image to create bitmap from

	QString					m_PdfPath;				//!< path to print pdf files to
	QString					m_WindowTitle;			//!< title to add to header line of preview dialog

	basar::I18nString					m_PsFilename;		//!< name of post script file containing print job
	basar::cmnutil::IPrintDataProvider*	m_pDataProv;		//!< reference to PrintDataProvider from cmnutil containing print job

	basar::Int32			m_IndexOfZoomCb;		//!< current index of selected zoom level in m_pCbZoom

	basar::VarString		m_RwprServer;			//!< server for raw print
	basar::VarString		m_RwprPrinter;			//!< printer for raw print

	basar::cmnutil::InterpretPrint::PrintProp
							m_PrintProperties;		//!< print properties

    basar::Int32            m_PreviewDpi;           //!< preview resolution in dpi, defaults to 60

    ZoomEnum                m_StartingPreviewZoom;  //!< starting zoom factor requested, defaults to ZOOM_INVALID
	
// --------------------------------------------------------------
// static callbacks for cmnutil
private:

	/*! \brief callback for cmnutil, called once directly after display() was called
		\n     no-throw */
	virtual basar::Int32		onOpen			();
	
	/*! \brief callback for cmnutil, called at the end of display()
		\n     no-throw */
	virtual basar::Int32		onClose			();
	
	/*! \brief callback for cmnutil, called twice after open() was called (correct values in 2nd call)
		\n     no-throw */
	virtual basar::Int32		onSize			(basar::Int32   width,	//!< width of pages to preview
												 basar::Int32   height, //!< height of pages to preview
												 basar::Int32   raster, //!< raster of pages to preview
												 basar::UInt8 * pImage	//!< path and name of picture to preview
												 );

	/*! \brief callback for cmnutil, called after size() was called the 2nd time;
				here, bitmaps are created and set to picturelist
		\n     RuntimeException */
	virtual basar::Int32		onPage			();
};

//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar

#endif // GUARD_PRINTPREVIEW_H

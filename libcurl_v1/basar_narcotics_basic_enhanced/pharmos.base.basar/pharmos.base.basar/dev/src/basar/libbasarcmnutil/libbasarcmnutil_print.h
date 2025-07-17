//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for print
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
 //----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_PRINT_H
#define GUARD_LIBBASARCMNUTIL_PRINT_H

//------------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
#ifdef _WIN32
#ifdef LIBBASARCMNUTIL_EXPORTS
#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
#else
#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
#endif 
#else 
#define LIBBASARCMNUTIL_API
#endif
#endif

//----------------------------------------------------------------------------
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include "libbasarcmnutil_definitions.h"
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar {
	namespace cmnutil {

		//----------------------------------------------------------------------------
		typedef CollStdIo		CollPrintRawInput;		//!< collection for raw input data to be printed


		//############################################################################
		//   useful functions
		//############################################################################

		//----------------------------------------------------------------------------
		//! \brief  collection with helper methods for printing
		//! \n final class
		class PrintTools
		{
			friend class PsInterpreter;	//!< only for basar internal use: calls private methods
			friend class RawPrint;		//!< only for basar internal use: calls private methods

		public:
			//! insert string with print data into queue \n no-throw
			LIBBASARCMNUTIL_API static void			insertStr2Coll(const VarString& input,				//!< string to be inserted in queue
				CollPrintRawInput& collPrintRawInput	//!< collection for raw input data to be printed
			);

		// JSI made public due to change over to the separate dll and wqe still need access to those methods
		public:

			//! gets postscript trigger lines				\n no-throw
			LIBBASARCMNUTIL_API static VarString						getTrigger(const VarString& formPath,		//!< form path: client/printer
				const VarString& formFileName,		//!< form file name -> postscript file
				const VarString& formName,		//!< form name
				Int32             fromPage,		//!< print from page
				Int32             toPage				//!< print to page
			);

			//! get client path with postscript forms		\n throw InvalidParameterException
			LIBBASARCMNUTIL_API  static VarString						getFormPathClient();
		};


		//############################################################################
		//   interfaces
		//############################################################################

		//----------------------------------------------------------------------------
		//! \brief provides raw print data (without layout)
		//! trigger will automatically be added by RawPrint resp. InterpretPrint
		//! \n interface class
		class IPrintDataProvider
		{
		public:
			//! \brief virtual destructor
			virtual			   ~IPrintDataProvider() {};

			//! \brief callback method: provides calling the printing instance with data that applies to form 
			//! \n pure virtual, must be implemented
			virtual void		onSendData(CollPrintRawInput& collPrintRawInput	//!< collection for raw input data to be printed
			) = 0;

			//! \brief callback method: provides calling printing instance with form path to ps file (-> trigger needs it)
			//! \n not pure virtual, default implemented; can be overwritten if ps file is stored on printer file system
			virtual	VarString	getFormPath() const { return ""; };
			//! \brief callback method: provides calling printing instance with form file name (-> trigger needs it)
			//! \n pure virtual, must be implemented
			virtual	VarString	getFormFileName() const = 0;
			//! \brief callback method: provides calling printing instance with form name (-> trigger needs it)
			//! \n pure virtual, must be implemented
			virtual	VarString	getFormName() const = 0;
		};

		//----------------------------------------------------------------------------
		/*! \ingroup DIFF_WIN_UNIX */
		//----------------------------------------------------------------------------
#ifdef _WIN32

//----------------------------------------------------------------------------
//! \brief screen display handling of interpreted printing documents
//! \n interface class
		class IScreenPrint
		{
		public:
			//! new screen device has been opened; first event of this device		\n pure virtual, must be implemented
			//! \return Int32
			virtual Int32		onOpen() = 0;
			//! screen device has been closed; last event from of this device		\n pure virtual, must be implemented
			//! \return Int32
			virtual Int32		onClose() = 0;
			//! screen device has been resized										\n pure virtual, must be implemented
			//! \return Int32
			virtual Int32		onSize(Int32   width,		//!< raster image width 
				Int32   height,	//!< raster image height
				Int32   raster,	//!< byte count of a raster image row	
				UInt8* pImage		//!< pointer to raster image
			) = 0;
			//! show screen device page												\n pure virtual, must be implemented
			//! \return Int32
			virtual Int32		onPage() = 0;
		};

		//----------------------------------------------------------------------------
#endif	// _WIN32


//############################################################################
//   exception handler
//############################################################################

//----------------------------------------------------------------------------
//! \brief LPD protocol exception class; thrown if errors with LPD protocol occur
//! \n final class
		class LPDException : public basar::Exception
		{
		public:
			//! constructs exception object and initialises exception type	\n no-throw
			LIBBASARCMNUTIL_API		LPDException(const ExceptInfo& sInfo	//!< exception info struct
			)
				: basar::Exception(sInfo, "LPDException")
			{};
		private:
			LPDException();							//!< forbidden
		};

		//############################################################################
		//   print handler
		//############################################################################

		//------------------------------------------------------------------------------------------------------------//
		//  return codes
		//------------------------------------------------------------------------------------------------------------//

		enum PrintRetCodeEnum				//!  printer's return values
		{
			PRINTER_SUCCESS = 0,	//!< printer communication OK

			PRINTER_OFFLINE = 1,	//!< printer is offline/down
			PRINTER_CONNREFUSED = 2,	//!< printer rejected attempts to connect: 
											//!< -> maybe too many client connections, so try again later
											PRINTER_CONNINPROGRESS = 3		//!< printer connect: socket is non-blocking, connection cannot completed immediately
		};


		//----------------------------------------------------------------------------
		//! \brief raw print handling
		//! \n print data will be sent to printer by lpr
		//! \n final class
		class RawPrint
		{
		public:
			//! constructor	\n no-throw
			LIBBASARCMNUTIL_API			RawPrint();
			//! destructor	\n no-throw
			LIBBASARCMNUTIL_API		   ~RawPrint();

			//! print raw print data straight to postscript printer; get data by callback method; use class PortPrint	\n no-throw
			LIBBASARCMNUTIL_API	PrintRetCodeEnum	print(const VarString& rServer,			//!< IP address or server name
				IPrintDataProvider& rDataProvider,	//!< provider for raw print data (trigger will automatically be added)
				Int32                 fromPage = -1,	//!< print from page
				Int32                 toPage = -1	//!< print to page
			);
			//! print raw print data straight to postscript printer; get data by callback method; use class Lpr			\n no-throw
			LIBBASARCMNUTIL_API	PrintRetCodeEnum	print(const VarString& rServer,			//!< IP address or server name
				const VarString& rPrinter,		//!< printer name (= queue name)
				IPrintDataProvider& rDataProvider,	//!< provider for raw print data (trigger will automatically be added)
				Int32                 fromPage = -1,	//!< print from page
				Int32                 toPage = -1	//!< print to page
			);

			//! print raw print data straight to postscript printer; get data by file; use tool lpr (child process		\n throw RuntimeException
			LIBBASARCMNUTIL_API	void				print(const VarString& rServer,			//!< IP address or server name
				const VarString& rPrinter,		//!< printer name (= queue name)
				const VarString& rInputFile		//!< (trigger + data) file name
			);

			//! set remote host's port if daemon doesn't listen to default port											\n no-throw
			LIBBASARCMNUTIL_API void				setPort(Int16                 port				//!< remote host's service port for socket connection
			);
		private:

			Int16									m_ServicePort;			//!< remote host's port if not default port
		};

		//----------------------------------------------------------------------------
		// forward declarations
		//----------------------------------------------------------------------------
		class PsInterpreter;
		class CollArgs;

		//----------------------------------------------------------------------------
		//! \brief postscript print handling
		//! \n print data + form (ps file ) will be interpreted by Ghostscript
		//! \n   - preview
		//! \n   - printer (specific to operating system)
		//! \n   - convert to pdf file
		//! \n final class
		class InterpretPrint
		{
		public:

			//! printer selection 
			enum PrintSelectEnum
			{
				PRINTER_DIALOG,							//!< create standard printer dialog: user can manually select printer, default printer is preselected
				PRINTER_STANDARD,						//!< select standard printer without dialog
				PRINTER_DEDICATED,						//!< select printer from Windows printer control panel. Windows printer name with spaces must be set!

				PRINTER_SELECT_DEFAULT = PRINTER_DIALOG	//!< default printer selection
			};

			//! display color depth
			enum DisplayBppEnum
			{
				DISP_MONO = 1,				//!<  1 bpp: monochrom
				DISP_COLOR_4 = 4,				//!<  4 bpp:      16 colors
				DISP_COLOR_8 = 8,				//!<  8 bpp:      256 colors
				DISP_COLOR_15 = 15,				//!< 15 bpp:    32786 colors 
				DISP_COLOR_16 = 16,				//!< 16 bpp:    65536 colors
				DISP_COLOR_32 = 32,				//!< 32 bpp: 16777216 colors (24 bit rgb)

				DISP_COLOR_DEFAULT = DISP_COLOR_4		//!< default color depth
			};

			//! print color depth
			enum PrintBppEnum
			{
				PRINT_USER_SETTING = 0,				//!<  printer settings (by user)

				PRINT_MONO = 1,				//!<  1 bpp: monochrom
				PRINT_COLOR_4 = 4,				//!<  4 bpp:      16 colors
				PRINT_COLOR_8 = 8,				//!<  8 bpp:      256 colors
				PRINT_COLOR_24 = 24,				//!< 24 bpp: 16777216 colors 

				PRINT_COLOR_DEFAULT = PRINT_MONO		//!< default color depth
			};

			//! paper size (see also wingdi.h)
			enum PaperSizeEnum
			{
				PS_Letter = 1,				//!< Letter 8 1/2 x 11 inches (values from wingdi.h)
				PS_A3 = 8,				//!< A3 297 x 420 mm          (values from wingdi.h)
				PS_A4 = 9,				//!< A4 210 x 297 mm          (values from wingdi.h)
				PS_A5 = 11,				//!< A5 148 x 210 mm          (values from wingdi.h)
				PS_A2 = 66,				//!< A2 420 x 594 mm          (values from wingdi.h)
				PS_A6 = 70,				//!< A6 105 x 148 mm          (values from wingdi.h)

				PS_DEFAULT = PS_A4				//!< default paper size
			};

			//! display properties
			class DisplayProp
			{
			public:
				//! c'tor	\n no-throw
				DisplayProp() : eBpp(DISP_COLOR_DEFAULT),
					dpi(DPI_DEFAULT),
					render(false)
				{};

				DisplayBppEnum		eBpp;							//!< color depth : bits per pixel
				Int32				dpi;							//!< resolution  : dots per inch
				bool				render;							//!< rendering   : interpolating, antialiasing

				static const Int32	DPI_DEFAULT = 75;				//!< default dpi	
			};

			//! print properties
			class PrintProp
			{
			public:
				//! c'tor	\n no-throw
				PrintProp() : eBpp(PRINT_COLOR_DEFAULT),
					papersize(PS_DEFAULT),
					eSelect(PRINTER_SELECT_DEFAULT),
					duplex(false),
					dpi(DPI_DEFAULT)
				{};

				PrintBppEnum		eBpp;							//!< color depth        : bits per pixel 
				PaperSizeEnum       papersize;                      //!< Windows defined paper size (wingdi.h)
				PrintSelectEnum		eSelect;						//!< printer selection
				bool                duplex;							//!< duplex printing
				Int32				dpi;							//!< maximum resolution : dots per inch
				VarString			printerName;					//!< Windows printer name (including spaces!)

				static const Int32	DPI_DEFAULT = 360;			//!< default dpi
				static const Int32  DPI_USER_SETTING = 0;			//!< printer setting (by user)
			};

			//----------------------------------------------------------------------------

			//! constructor: creates PsLib if necessary		\n no-throw
			LIBBASARCMNUTIL_API			InterpretPrint();
			//! destructor									\n no-throw
			LIBBASARCMNUTIL_API		   ~InterpretPrint();

			//! set print color depth						\n no-throw
			LIBBASARCMNUTIL_API void	setPrintBpp(PrintBppEnum		   eBpp					//!< print color depth in bpp
			);
			//! set maximum print resolution				\n no-throw	
			LIBBASARCMNUTIL_API void	setPrintDpi(Int32				   dpi					//!< maximum print resolution in dpi
			);
			//! set duplex printing							\n no-throw	
			LIBBASARCMNUTIL_API void	setPrintDuplex(bool				   duplex				//!< duplex printing
			);
			//! set Windows defined paper size				\n no-throw	
			LIBBASARCMNUTIL_API void	setPrintPaperSize(PaperSizeEnum		   size                 //!< paper size
			);
			//! select Windows printer						\n no-throw
			LIBBASARCMNUTIL_API void	setPrintSelect(PrintSelectEnum       eSelect,				//!< printer selection 
				const VarString& rPrinterName = ""	//!< Windows printer name
			);

			//! print ps data to windows printer			\n no-throw
			LIBBASARCMNUTIL_API void	print(IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! print ps data to windows printer			\n no-throw
			LIBBASARCMNUTIL_API void	print(const VarString& rInputFileName		//!< pdf file or postscript file (can include trigger + data)
			);

			//! convert ps to pdf and save file				\n no-throw
			LIBBASARCMNUTIL_API void	savePdf(IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				const VarString& rOutputFileName,		//!< pdf file to save (path + file name)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! convert ps to pdf and save file				\n RuntimeException when rInputFileName specifies non-existing file
			LIBBASARCMNUTIL_API void	savePdf(const VarString& rInputFileName,		//!< pdf file or postscript file (can include trigger + data)
				const VarString& rOutputFileName		//!< pdf file to save (path + file name)
			);

			//! set display color depth						\n no-throw
			LIBBASARCMNUTIL_API void	setDisplayBpp(DisplayBppEnum		   eBpp					//!< display color depth in bpp
			);
			//! set display resolution						\n no-throw
			LIBBASARCMNUTIL_API void	setDisplayDpi(Int32				   dpi					//!< display resolution in dpi
			);
			//! set rendering								\n no-throw
			LIBBASARCMNUTIL_API void	setDisplayRender(bool				   render				//!< display rendering
			);
#ifdef _WIN32
			//! display ps data (e.g. in print preview)		\n no-throw
			LIBBASARCMNUTIL_API void	display(IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				const IScreenPrint& rScreenPrint,		//!< reference to screen display handler
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! display ps data (e.g. in print preview)		\n no-throw
			LIBBASARCMNUTIL_API void	display(const VarString& rInputFileName,		//!< pdf file or postscript file (can include trigger + data)
				const IScreenPrint& rScreenPrint  		//!< reference to screen display handler
			);
#endif

		private:

			//--- typedef section ----------------------------------------------------

			typedef boost::shared_ptr < PsInterpreter >	PsInterpretSharedPtr;	//!< shared reference to postscript interpreter object
			typedef boost::weak_ptr   < PsInterpreter >	PsInterpretWeakPtr;		//!< weak reference to postscript interpreter object

			//--- method section  ----------------------------------------------------

			//! get display format value for ps interpreter \n throw RuntimeException
			static Int32			getDisplayFormat(const DisplayBppEnum bpp	//!< display color depth in bpp
			);

			//! get printer settings in postscript code	\n InvalidParameterException
			const VarString				getPrinterSettingPS() const;

			//! compose common options
			void                        insertCommonOptions(CollArgs& args //!< arguments container
			);
			//! compose print options
			void                        insertPrintOptions(CollArgs& args //!< arguments container
			);
#ifdef _WIN32
			//! compose display options
			void                        insertDisplayOptions(CollArgs& args,        //!< arguments container
				const IScreenPrint& rScreenPrint //!< reference to screen display handler
			);
#endif
			//! compose pdf options
			void                        insertPdfOptions(CollArgs& args,           //!< arguments container
				const VarString& rOutputFileName //!< pdf file to save (path + file name)
			);
			//! checks if passed inputFileName exists \n throw RuntimeException
			void                        checkInputFileExists(const basar::VarString& inputFileName //!< name of ps-file (plus optional path)
			) const;

			//--- member section  ----------------------------------------------------

			PsInterpretSharedPtr		m_pPsInterpreter;			//!< shared pointer to PsInterpreter object
			static PsInterpretWeakPtr	m_pWeakPsInterpreter;		//!< static(!) weak pointer to control single PsInterpreter object
			static boost::mutex         m_InterpreterGuard;         //!< static(!) mutex to control creation of PsInterpreter object

			DisplayProp					m_DisplayProperties;		//!< display properties
			PrintProp					m_PrintProperties;			//!< print   properties
		};

		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
	}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD

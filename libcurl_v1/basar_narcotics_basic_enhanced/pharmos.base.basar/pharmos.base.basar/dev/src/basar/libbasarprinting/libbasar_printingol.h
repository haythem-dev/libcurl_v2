//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for print
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
 //----------------------------------------------------------------------------

#ifndef GUARD_INTERPRETPRINTOL_H
#define GUARD_INTERPRETPRINTOL_H

//------------------------------------------------------------------------------
#ifndef LIBBASARPRINT_API
#ifdef _WIN32
#ifdef LIBBASARPRINT_EXPORTS
#define LIBBASARPRINT_API __declspec(dllexport)	//!< dll exported
#else
#define LIBBASARPRINT_API __declspec(dllimport)	//!< dll imported
#endif 
#else 
#define LIBBASARPRINT_API
#endif
#endif

//----------------------------------------------------------------------------
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include "libbasarcmnutil_print.h"
#include "libbasarcmnutil_definitions.h"
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"
//#include "ps/collargs.h"
class CollArg;
//----------------------------------------------------------------------------
namespace basar {
	namespace printing {

		//----------------------------------------------------------------------------
		// forward declarations
		//----------------------------------------------------------------------------
		class PsInterpreterOL;
//		class cmnutil::CollArgs;

		class PrintUtils
		{
		public:
			LIBBASARPRINT_API	static basar::VarString GetNameExtended(const basar::VarString& srcName, const basar::VarString& ext = ".ps");
			// internal best match interface to UADM parameter returns true if parameter was found and false if parameter could not be found
			// implcitily it will search via logon branch, area ID or Default parameter.
			LIBBASARPRINT_API   static bool getParaValue(const basar::I18nString& rParaName, basar::I18nString& rParaValue);
		};


		class InterpretPrintOL
		{
		public:

			LIBBASARPRINT_API			InterpretPrintOL();
			LIBBASARPRINT_API		   ~InterpretPrintOL();

			//! print ps data to windows printer			\n no-throw
			LIBBASARPRINT_API void	print(cmnutil::IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! print ps data to windows printer			\n no-throw
			LIBBASARPRINT_API void	print(const VarString& rInputFileName		//!< pdf file or postscript file (can include trigger + data)
			);

			//! convert ps to pdf and save file				\n no-throw
			LIBBASARPRINT_API void	savePdf(cmnutil::IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				const VarString& rOutputFileName,		//!< pdf file to save (path + file name)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! convert ps to pdf and save a copy of OL file				\n no-throw
			LIBBASARPRINT_API void	saveCopyPdf(cmnutil::IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				const VarString& rOutputFileName,		//!< pdf file to save (path + file name)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! convert ps to pdf and save file				\n RuntimeException when rInputFileName specifies non-existing file
			LIBBASARPRINT_API void	savePdf(const VarString& rInputFileName,		//!< pdf file or postscript file (can include trigger + data)
				const VarString& rOutputFileName		//!< pdf file to save (path + file name)
			);
			//! convert ps to pdf and save a copy of OLfile				\n RuntimeException when rInputFileName specifies non-existing file
			LIBBASARPRINT_API void	saveCopyPdf(const VarString& rInputFileName,		//!< pdf file or postscript file (can include trigger + data)
				const VarString& rOutputFileName		//!< pdf file to save (path + file name)
			);

			//! display ps data (e.g. in print preview)		\n no-throw
			LIBBASARPRINT_API void	display(cmnutil::IPrintDataProvider& rDataProvider,		//!< provider for raw print data (trigger will automatically be added)
				Int32                 fromPage = -1,		//!< print from page
				Int32                 toPage = -1		//!< print to page
			);
			//! display ps data (e.g. in print preview)		\n no-throw
			LIBBASARPRINT_API void	display(const VarString& rInputFileName		//!< pdf file or postscript file (can include trigger + data)
			);
			/*! \brief set server and printer, used for raw print
				\n     no-throw */
			LIBBASARPRINT_API	void setRawPrintInfo(const basar::VarString& rServer,		//!< server for raw print
				const basar::VarString& rPrinter = ""	//!< printer for raw print (if empty -> 9100, else lpr)
			);

			LIBBASARPRINT_API	void setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
				const basar::I18nString& printerCfgName	//!< printer name)
			);
		protected:
			cmnutil::RawPrint* _rawPrint;

		private:

			//--- typedef section ----------------------------------------------------

			typedef boost::shared_ptr < PsInterpreterOL >	PsInterpretOLSharedPtr;	//!< shared reference to postscript interpreter object
			typedef boost::weak_ptr   < PsInterpreterOL >	PsInterpretOLWeakPtr;		//!< weak reference to postscript interpreter object

			//--- member section  ----------------------------------------------------

			PsInterpretOLSharedPtr		m_pPsInterpreterOL;			//!< shared pointer to PsInterpreter object
			static PsInterpretOLWeakPtr	m_pWeakPsInterpreterOL;		//!< static(!) weak pointer to control single PsInterpreter object
			static boost::mutex         m_InterpreterOLGuard;         //!< static(!) mutex to control creation of PsInterpreter object

			basar::VarString m_Server;
			basar::VarString m_Printer;

			//--- method section  ----------------------------------------------------
			void InitRawPrint();

			//			//! compose pdf options
			void                        insertPdfOptions(cmnutil::CollArgs& args,           //!< arguments container
				const VarString& rOutputFileName //!< pdf file to save (path + file name)
			);
			//			//! checks if passed inputFileName exists \n throw RuntimeException
			void                        checkInputFileExists(const basar::VarString& inputFileName //!< name of ps-file (plus optional path)
			) const;

		};

		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
	}	// namespace printing
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD

//----------------------------------------------------------------------------
/*! \file
 *  \brief  ghostscript interpreter class
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
 //----------------------------------------------------------------------------

#ifndef GUARD_PSINTERPRETEROL_H
#define GUARD_PSINTERPRETEROL_H

//----------------------------------------------------------------------------

#ifdef _WIN32
#define VC_EXTRALEAN            //!< less Visual C headers
#define WIN32_LEAN_AND_MEAN     //!< less Windows headers
#include <windows.h>
#endif

#include <libbasar_definitions.h>
#include "libbasarcmnutil_print.h"
#include "libbasarcmnutil_bstring.h"
#include "../libbasarcmnutil/ps/collargs.h"
//#include "collargs.h"

/*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
#pragma warning (push)
#pragma warning( disable : 4244 )       // boost::datetime::miro_sec_time_clock: conversion from int to unsigned short
#endif

#include <boost/thread/mutex.hpp>

/*! \ingroup DIFF_WIN_UNIX */
#ifdef _WIN32
#pragma warning (pop)
#endif

#ifdef _AIX
typedef void* HINSTANCE;
#endif

//----------------------------------------------------------------------------
namespace basar
{
    namespace printing
    {

        //---------------------------------------------------------------------------
        class IPrintDataProvider;

        //---------------------------------------------------------------------------
        class PsInterpreterOL
        {
        public:

            //! caller device
            enum CallerDeviceEnum
            {
                CALLER_PDF,         //!< pdf     device
                CALLER_PRINT,       //!< print   device
                CALLER_DISPLAY      //!< display device
            };

            //! constructor: load shared library and create ps interpreter          \n no-throw
            PsInterpreterOL();
            //! destructor:  close ps interpreter and unload shared library         \n no-throw
            ~PsInterpreterOL();

            //! execute interpreting postscript data \n throw RuntimeException
            void                    execute(const cmnutil::CollArgs& args,     //!< argument strings
                CallerDeviceEnum eDevice   //!< caller device
            );

            //! set provider for raw print data (without layout)                    \n no-throw
            void                    setDataProvider(cmnutil::IPrintDataProvider& rDataProvider,        //!< raw print data provider
                Int32                fromPage,             //!< print from page
                Int32                toPage                //!< print to page
            );

            /*! \brief set server and printer, used for raw print
    \n     no-throw */
            void setRawPrintInfo(const basar::VarString& rServer,		//!< server for raw print
                const basar::VarString& rPrinter = ""	//!< printer for raw print (if empty -> 9100, else lpr)
            );

            void setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
                const basar::I18nString& printerCfgName);	//!< printer config name)

            cmnutil::RawPrint* GetRawPrint() { return _rawPrint; }
        private:

            //--- method section  ----------------------------------------------------

            //! standard input/output callback functions
            //class Stdio
            //{
            //public:
            //    //! stdinput callback function      \n no-throw
            //    static Int32 GSDLLCALL  onInput(void* pInterpreter,    //!< pointer to ps interpreter
            //        char* pBuf,             //!< input data buffer
            //        Int32        buflen            //!< buffer length
            //    );
            //    //! stdoutput callback function     \n no-throw
            //    static Int32 GSDLLCALL  onOutput(void* pInterpreter,    //!< pointer to ps interpreter
            //        const char* pBuf,              //!< output data buffer
            //        Int32        buflen            //!< buffer length
            //    );
            //    //! stderror callback function      \n no-throw
            //    static Int32 GSDLLCALL  onError(void* pInterpreter,    //!< pointer to ps interpreter
            //        const char* pBuf,              //!< error data buffer
            //        Int32        buflen            //!< buffer length
            //    );
            //};

            //! display callback functions
            class Display
            {
            public:
                //! new device has been opened; first event from this device                                \n no-throw
                static Int32            onOpen(void* pScreenPrint,           //!< screen display handler
                    void* pDevice                  //!< ignored: enhanced key for indentifying caller
                );

                //! device is about to be closed; device will not be closed until this function returns     \n no-throw
                static Int32            onPreclose(void* pScreenPrint,           //!< screen display handler
                    void* pDevice                  //!< ignored: enhanced key for indentifying caller
                );

                //! device has been closed; last event from this device                                     \n no-throw
                static Int32            onClose(void* pScreenPrint,           //!< screen display handler
                    void* pDevice                  //!< ignored: enhanced key for indentifying caller
                );

                //! device is about to be resized; resize will only occur if this function returns 0        \n no-throw
                static Int32            onPresize(void* pScreenPrint,           //!< screen display handler
                    void* pDevice,                 //!< ignored: enhanced key for indentifying caller
                    Int32  width,                  //!< raster image width
                    Int32  height,                 //!< raster image height
                    Int32  raster,                 //!< byte count of a row
                    UInt32 format                  //!< ignored
                );

                //! device has been resized                                                                 \n no-throw
                static Int32            onSize(void* pScreenPrint, //!< screen display handler
                    void* pDevice,       //!< ignored: enhanced key for indentifying caller
                    Int32           width,         //!< raster image width
                    Int32           height,        //!< raster image height
                    Int32           raster,        //!< byte count of a row
                    UInt32          format,        //!< ignored
                    unsigned char* pImage          //!< new pointer to raster image
                );

                //! flushpage                                                                               \n no-throw
                static Int32            onSync(void* pScreenPrint,           //!< screen display handler
                    void* pDevice                  //!< ignored: enhanced key for indentifying caller
                );

                //! show page; if you want to pause on showpage, then don't return immediately              \n no-throw
                static Int32            onPage(void* pScreenPrint,           //!< screen display handler
                    void* pDevice,            //!< ignored: enhanced key for indentifying caller
                    Int32  copies,           //!< ignored
                    Int32  flush                   //!< ignored
                );
            };

            //---------------------------

            PsInterpreterOL(const PsInterpreterOL& rCopy);         //!< forbidden
            PsInterpreterOL& operator =          (const PsInterpreterOL& rCopy);         //!< forbidden

            //--- method section  ----------------------------------------------------

            void                    clearError(); //!< clear stored error message and error state                     \n no-throw
            //! \brief clear input data queue   \n no-throw
            void                    clearQueue();
            //! initialize queue with trigger (values from IPrintDataProvider)  \n throw InvalidStateException
            void                    initQueue(Int32                fromPage,            //!< print from page
                Int32                toPage                //!< print to page
            );

            void InitRawPrint();


            //--- member section  ----------------------------------------------------
            cmnutil::IPrintDataProvider* m_pDataProvider;        //!< non constant reference to raw printing data provider

            bool                            m_PendingError;         //!< error occurred and must be handled
            VarString                       m_ErrorMsg;             //!< error message

            cmnutil::CollPrintRawInput               m_collRawInput;         //!< collection for raw input data to be printed
            cmnutil::RawPrint* _rawPrint;
            basar::VarString		m_RwprServer;			//!< server for raw print
            basar::VarString		m_RwprPrinter;			//!< printer for raw print
        };

        //----------------------------------------------------------------------------
    }   // namespace cmnutil
}   // namespace basar

//----------------------------------------------------------------------------
#endif  // GUARD

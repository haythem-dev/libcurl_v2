//----------------------------------------------------------------------------
/*! \file
 *  \brief  ghostscript interpreter class
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PSINTERPRETER_H
#define GUARD_PSINTERPRETER_H

//----------------------------------------------------------------------------

#ifdef _WIN32
#define VC_EXTRALEAN            //!< less Visual C headers
#define WIN32_LEAN_AND_MEAN     //!< less Windows headers
#include <windows.h>
#endif

#include <libbasar_definitions.h>
#include "../libbasarcmnutil_print.h"
#include "../libbasarcmnutil_bstring.h"
#include "collargs.h"

#include <iapi.h>

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
    namespace cmnutil
    {

//---------------------------------------------------------------------------
        class IPrintDataProvider;

//---------------------------------------------------------------------------
//! ghostscript interpreter class   \n final class
        class PsInterpreter
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
                PsInterpreter       ();
                //! destructor:  close ps interpreter and unload shared library         \n no-throw
                ~PsInterpreter       ();

                //! execute interpreting postscript data \n throw RuntimeException
                void                    execute             ( const CollArgs& args,     //!< argument strings
                        CallerDeviceEnum eDevice   //!< caller device
                                                            );

                //! set provider for raw print data (without layout)                    \n no-throw
                void                    setDataProvider     ( IPrintDataProvider& rDataProvider,        //!< raw print data provider
                        Int32                fromPage,             //!< print from page
                        Int32                toPage                //!< print to page
                                                            );

            private:

                //--- method section  ----------------------------------------------------

                //! standard input/output callback functions
                class Stdio
                {
                    public:
                        //! stdinput callback function      \n no-throw
                        static Int32 GSDLLCALL  onInput             ( void*        pInterpreter,    //!< pointer to ps interpreter
                                char*        pBuf,             //!< input data buffer
                                Int32        buflen            //!< buffer length
                                                                    );
                        //! stdoutput callback function     \n no-throw
                        static Int32 GSDLLCALL  onOutput            ( void*        pInterpreter,    //!< pointer to ps interpreter
                                const char* pBuf,              //!< output data buffer
                                Int32        buflen            //!< buffer length
                                                                    );
                        //! stderror callback function      \n no-throw
                        static Int32 GSDLLCALL  onError             ( void*        pInterpreter,    //!< pointer to ps interpreter
                                const char* pBuf,              //!< error data buffer
                                Int32        buflen            //!< buffer length
                                                                    );
                };

                //! display callback functions
                class Display
                {
                    public:
                        //! new device has been opened; first event from this device                                \n no-throw
                        static Int32            onOpen              ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice                  //!< ignored: enhanced key for indentifying caller
                                                                    );

                        //! device is about to be closed; device will not be closed until this function returns     \n no-throw
                        static Int32            onPreclose          ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice                  //!< ignored: enhanced key for indentifying caller
                                                                    );

                        //! device has been closed; last event from this device                                     \n no-throw
                        static Int32            onClose             ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice                  //!< ignored: enhanced key for indentifying caller
                                                                    );

                        //! device is about to be resized; resize will only occur if this function returns 0        \n no-throw
                        static Int32            onPresize           ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice,                 //!< ignored: enhanced key for indentifying caller
                                Int32  width,                  //!< raster image width
                                Int32  height,                 //!< raster image height
                                Int32  raster,                 //!< byte count of a row
                                UInt32 format                  //!< ignored
                                                                    );

                        //! device has been resized                                                                 \n no-throw
                        static Int32            onSize              ( void*           pScreenPrint, //!< screen display handler
                                void*           pDevice,       //!< ignored: enhanced key for indentifying caller
                                Int32           width,         //!< raster image width
                                Int32           height,        //!< raster image height
                                Int32           raster,        //!< byte count of a row
                                UInt32          format,        //!< ignored
                                unsigned char* pImage          //!< new pointer to raster image
                                                                    );

                        //! flushpage                                                                               \n no-throw
                        static Int32            onSync              ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice                  //!< ignored: enhanced key for indentifying caller
                                                                    );

                        //! show page; if you want to pause on showpage, then don't return immediately              \n no-throw
                        static Int32            onPage              ( void* pScreenPrint,           //!< screen display handler
                                void* pDevice     ,            //!< ignored: enhanced key for indentifying caller
                                Int32  copies      ,           //!< ignored
                                Int32  flush                   //!< ignored
                                                                    );
                };

                //---------------------------

                PsInterpreter       ( const PsInterpreter& rCopy );         //!< forbidden
                PsInterpreter&          operator =          ( const PsInterpreter& rCopy );         //!< forbidden

                void                    loadLib             ();     //!< load shared library                    \n throw NullReferenceException, InvalidParameterException
                void                    unloadLib           ();     //!< unload shared library                  \n throw NullReferenceException, InvalidParameterException

                void                    createInstance      ();     //!< create interpreter instance            \n throw RuntimeException, NullReferenceException
                void                    closeInstance       ();     //!< destroy interpreter instance           \n throw NullReferenceException

                void                    setStdIo            ();     //!< set stdin, stdout, stderr callbacks    \n throw RuntimeException
                void                    setDisplay          ();     //!< set display specific configuration     \n throw RuntimeException

                //--- method section  ----------------------------------------------------

                void                    clearError          (); //!< clear stored error message and error state                     \n no-throw
                //! \brief clear input data queue   \n no-throw
                void                    clearQueue          ();
                //! initialize queue with trigger (values from IPrintDataProvider)  \n throw InvalidStateException
                void                    initQueue           ( Int32                fromPage,            //!< print from page
                        Int32                toPage                //!< print to page
                                                            );


                //--- member section  ----------------------------------------------------

                HINSTANCE                       m_hLib;                 //!< shared library handle

                PFN_gsapi_revision              m_pfnRevision;          //!< function pointer to interpreter dll: version numbers and strings
                PFN_gsapi_new_instance          m_pfnNewInst;           //!< function pointer to interpreter dll: create new Ghostscript instance
                PFN_gsapi_delete_instance       m_pfnDelInst;           //!< function pointer to interpreter dll: destroy Ghostscript instance
                PFN_gsapi_set_stdio             m_pfnSetStdIo;          //!< function pointer to interpreter dll: set callback functions for stdio
                PFN_gsapi_set_display_callback  m_pfnSetDisplay;        //!< function pointer to interpreter dll: set display device callback structure
                PFN_gsapi_set_arg_encoding      m_pfnArgCoding;         //!< function pointer to interpreter dll: argument encoding
                PFN_gsapi_init_with_args        m_pfnInit;              //!< function pointer to interpreter dll: init Ghostscript interpreter
                PFN_gsapi_exit                  m_pfnExit;              //!< function pointer to interpreter dll: exit Ghostscript interpreter

                void*                           m_pInstance;            //!< Ghostscript interpreter instance
                IPrintDataProvider*             m_pDataProvider;        //!< non constant reference to raw printing data provider

                bool                            m_PendingError;         //!< error occurred and must be handled
                VarString                       m_ErrorMsg;             //!< error message

                CollPrintRawInput               m_collRawInput;         //!< collection for raw input data to be printed
        };

//----------------------------------------------------------------------------
    }   // namespace cmnutil
}   // namespace basar

//----------------------------------------------------------------------------
#endif  // GUARD

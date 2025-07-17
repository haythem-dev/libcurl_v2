//----------------------------------------------------------------------------
/*! \file
 *  \brief  ghostscript interpreter class
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
//----------------------------------------------------------------------------

#include "psinterpreter.h"

#include "../loggerpool.h"
#include "encodedargs.h"

#include <gdevdsp.h>
#include <ierrors.h>

#ifdef _AIX
#include <errno.h>
#include <dlfcn.h>
#endif

//----------------------------------------------------------------------------
namespace basar
{
    namespace cmnutil
    {

//------------------------------------------------------------------------------

//! \retval >0 number of characters read
//! \retval 0 for EOF
//! \retval -1 for error
//! \throw  no-throw
        Int32 PsInterpreter::Stdio::onInput ( void* pInterpreter, char* pBuf, Int32 buflen )
        {
            CollPrintRawInput&   rCollInput  =   ( static_cast< PsInterpreter* > ( pInterpreter ) )->m_collRawInput  ;
            IPrintDataProvider& rDataProv   = *( ( static_cast< PsInterpreter* > ( pInterpreter ) )->m_pDataProvider );
            Int32                queuedBytes =    static_cast< const Int32     > ( rCollInput.size() );

            if ( 0 == queuedBytes ) // refresh queue
            {
                rDataProv.onSendData( rCollInput );
                queuedBytes = static_cast < const Int32 > ( rCollInput.size() );
            }


#ifndef NDEBUG  // ---  call statistics ----------------------------------

            static int count = 0;   // number of calls
            static int bytes = 0;   // requested bytes by ps interpreter
            static int peak  = 0;   // maximum queue size

            ++count;
            bytes += buflen;

            if ( queuedBytes > peak )
            {
                peak = queuedBytes;
            }

            if ( 0 == queuedBytes ) // no more data
            {
                count = bytes = peak = 0;
            }

#endif // NDEBUG  --------------------------------------------------------


            if ( 0 == queuedBytes ) // no more data
            {
                return 0;
            }

            // send data to ps interpreter
            const Int32        copybytes = ( queuedBytes <= buflen ) ? queuedBytes : buflen;
            const char* const pEnd      = pBuf + copybytes;

            for ( ; pBuf < pEnd; ++pBuf )
            {
                *pBuf = rCollInput.front();     // copy char from queue to ps buffer
                rCollInput.pop();               // delete char from queue
            }

            return copybytes;
        }

//------------------------------------------------------------------------------
//! \return number of characters written
//! \throw  no-throw
        Int32 PsInterpreter::Stdio::onOutput ( void* pInterpreter, const char* pBuf, Int32 buflen )
        {
            PsInterpreter* pObj = static_cast<PsInterpreter*>( pInterpreter );

            pObj->m_PendingError = true;
            pObj->m_ErrorMsg    += pBuf;

            return buflen;
        }

//------------------------------------------------------------------------------
//! \return number of characters written
//! \throw no-throw
        Int32 PsInterpreter::Stdio::onError ( void* pInterpreter, const char* pBuf, Int32 buflen )
        {
            PsInterpreter* pObj = static_cast<PsInterpreter*>( pInterpreter );

            pObj->m_PendingError = true;
            pObj->m_ErrorMsg    += pBuf;

            return buflen;
        }


//------------------------------------------------------------------------------
//! \throw no-throw
//! \return return value of IScreenPrint::onOpen if pScreenPrint != null, zero otherwise
        Int32 PsInterpreter::Display::onOpen ( void* pScreenPrint, void* /* pDevice */ )

        {

            if ( pScreenPrint )
            {
#ifdef _WIN32
                IScreenPrint& rScreenPrint = *( static_cast<IScreenPrint*>( pScreenPrint ) );
                return rScreenPrint.onOpen();
#endif
            }

            return 0;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return always zero
        Int32 PsInterpreter::Display::onPreclose( void* /* pScreenPrint*/, void* /* pDevice */ )
        {
            return 0;   // do nothing - no thread synchonisation needed
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return return value of IScreenPrint::onClose if pScreenPrint != null, zero otherwise
        Int32 PsInterpreter::Display::onClose( void* pScreenPrint, void* /* pDevice */ )
        {

            if ( pScreenPrint )
            {
#ifdef _WIN32
                IScreenPrint& rScreenPrint = *( static_cast<IScreenPrint*>( pScreenPrint ) );
                return rScreenPrint.onClose();
#endif
            }

            return 0;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return always zero
        Int32 PsInterpreter::Display::onPresize( void* /* pScreenPrint */,
                void* /* pDevice      */,
                Int32  /* width        */,
                Int32  /* height       */,
                Int32  /* raster       */,
                UInt32 /* format       */ )
        {
            return 0; // do nothing - no thread synchonisation needed
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return return value of IScreenPrint::onSize if pScreenPrint != null, zero otherwise
        Int32 PsInterpreter::Display::onSize ( void*           pScreenPrint , void* /* pDevice */,
                                               Int32           width        ,
                                               Int32           height       ,
                                               Int32           raster       ,
                                               UInt32          /* format  */,
                                               unsigned char* pImage       )
        {
            if ( pScreenPrint )
            {
#ifdef _WIN32
                IScreenPrint& rScreenPrint = *( static_cast<IScreenPrint*>( pScreenPrint ) );
                return rScreenPrint.onSize( width, height, raster, pImage );
#endif
#ifdef _AIX

                if ( width && height && raster && pImage )
                {
                }

#endif
            }

            return 0;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return always zero
        Int32 PsInterpreter::Display::onSync( void* /* pScreenPrint */, void* /* pDevice   */ )
        {
            return 0; // do nothing - no thread synchonisation needed
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return return value of IScreenPrint::onPage if pScreenPrint != null, zero otherwise
        Int32 PsInterpreter::Display::onPage ( void* pScreenPrint ,
                                               void* /* pDevice */,
                                               Int32  /* copies  */,
                                               Int32  /* flush   */ )
        {
            if ( pScreenPrint )
            {
#ifdef _WIN32
                IScreenPrint& rScreenPrint = *( static_cast<IScreenPrint*>( pScreenPrint ) );
                return rScreenPrint.onPage();
#endif
            }

            return 0;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        PsInterpreter::PsInterpreter() :    m_hLib          ( 0 ),
            m_pfnRevision   ( 0 ),
            m_pfnNewInst    ( 0 ),
            m_pfnDelInst    ( 0 ),
            m_pfnSetStdIo   ( 0 ),
            m_pfnSetDisplay ( 0 ),
            m_pfnArgCoding  ( 0 ),
            m_pfnInit       ( 0 ),
            m_pfnExit       ( 0 ),
            m_pInstance     ( 0 ),
            m_pDataProvider ( 0 )
        {
            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "PsInterpreter(this = 0x%08X)",
                            this );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

            clearError    ();   // clear error msg, flag

            loadLib       ();   // library

        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        PsInterpreter::~PsInterpreter()
        {
            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "~PsInterpreter(this = 0x%08X)",
                            this );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

            if ( NULL != m_pInstance )
            {
                closeInstance();
            }

            unloadLib    ();
        }

//------------------------------------------------------------------------------
//! \throw NullReferenceException, InvalidParameterException
//!
        void PsInterpreter::loadLib()
        {
            static ConstString func = "basar.cmnutil.PsInterpreter.loadLib()";

            if ( ( HINSTANCE )0 != m_hLib )
            {
                throw NullReferenceException( ExceptInfo( func, "m_hLib != 0", __FILE__, __LINE__ ) );
            }

            static ConstString funcRev   = "gsapi_revision";
            static ConstString funcNewInst   = "gsapi_new_instance";
            static ConstString funcDelInst   = "gsapi_delete_instance";
            static ConstString funcStdIo     = "gsapi_set_stdio";
            static ConstString funcDisplay   = "gsapi_set_display_callback";
            static ConstString funcArgCoding = "gsapi_set_arg_encoding";
            static ConstString funcInit  = "gsapi_init_with_args";
            static ConstString funcExit  = "gsapi_exit";

#ifdef _AIX

            static ConstString libName = "libgs.so";

            m_hLib = ::dlopen( libName, RTLD_NOW );

            if ( NULL == m_hLib )
            {
                basar::VarString msg;

                msg.format("dlopen() failed with error <%d>", errno );

                if ( ENOEXEC == errno )
                {
                    //check dlerror 
                    msg.append(": ");
                    msg.append( dlerror() );
                }

                throw InvalidParameterException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
            }

            //__extension__ is used to suppress pedantic warning
            const char* pFunc;

            if ( ( 0 == ( m_pfnRevision   = __extension__ ( PFN_gsapi_revision            )::dlsym( m_hLib, ( pFunc = funcRev      ) ) ) ) ||
                    ( 0 == ( m_pfnNewInst    = __extension__ ( PFN_gsapi_new_instance        )::dlsym( m_hLib, ( pFunc = funcNewInst  ) ) ) ) ||
                    ( 0 == ( m_pfnDelInst    = __extension__ ( PFN_gsapi_delete_instance     )::dlsym( m_hLib, ( pFunc = funcDelInst  ) ) ) ) ||
                    ( 0 == ( m_pfnSetStdIo   = __extension__ ( PFN_gsapi_set_stdio           )::dlsym( m_hLib, ( pFunc = funcStdIo    ) ) ) ) ||
                    ( 0 == ( m_pfnSetDisplay = __extension__ ( PFN_gsapi_set_display_callback )::dlsym( m_hLib, ( pFunc = funcDisplay  ) ) ) ) ||
                    ( 0 == ( m_pfnArgCoding  = __extension__ ( PFN_gsapi_set_arg_encoding    )::dlsym( m_hLib, ( pFunc = funcArgCoding ) ) ) ) ||
                    ( 0 == ( m_pfnInit       = __extension__ ( PFN_gsapi_init_with_args      )::dlsym( m_hLib, ( pFunc = funcInit     ) ) ) ) ||
                    ( 0 == ( m_pfnExit       = __extension__ ( PFN_gsapi_exit                )::dlsym( m_hLib, ( pFunc = funcExit     ) ) ) )    )
            {
                unloadLib();

                VarString msg;
                msg.format( "dlsym(\"%s\") failed", pFunc );
                throw InvalidParameterException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "loadLib(this = 0x%08X): dlopen(%s) = 0x%08X; dlsym() successful",
                            this,
                            libName,
                            m_hLib );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

#endif
#ifdef _WIN32
            static ConstString libName =
#ifdef _WIN64
                "gsdll64.dll"
#else
                "gsdll32.dll"
#endif
            ;

            // try the system search path
            if ( ( HINSTANCE )HINSTANCE_ERROR > ( m_hLib = ::LoadLibrary( libName ) ) )
            {
                // TODO: add GetLastError() to exception output?
                throw InvalidParameterException( ExceptInfo( func, "LoadLibrary() failed", __FILE__, __LINE__ ) );
            }

            const char* pFunc;

            if ( ( 0 == ( m_pfnRevision   = ( PFN_gsapi_revision            )::GetProcAddress( m_hLib, ( pFunc = funcRev      ) ) ) ) ||
                    ( 0 == ( m_pfnNewInst    = ( PFN_gsapi_new_instance        )::GetProcAddress( m_hLib, ( pFunc = funcNewInst  ) ) ) ) ||
                    ( 0 == ( m_pfnDelInst    = ( PFN_gsapi_delete_instance     )::GetProcAddress( m_hLib, ( pFunc = funcDelInst  ) ) ) ) ||
                    ( 0 == ( m_pfnSetStdIo   = ( PFN_gsapi_set_stdio           )::GetProcAddress( m_hLib, ( pFunc = funcStdIo    ) ) ) ) ||
                    ( 0 == ( m_pfnSetDisplay = ( PFN_gsapi_set_display_callback )::GetProcAddress( m_hLib, ( pFunc = funcDisplay  ) ) ) ) ||
                    ( 0 == ( m_pfnArgCoding  = ( PFN_gsapi_set_arg_encoding    )::GetProcAddress( m_hLib, ( pFunc = funcArgCoding ) ) ) ) ||
                    ( 0 == ( m_pfnInit       = ( PFN_gsapi_init_with_args      )::GetProcAddress( m_hLib, ( pFunc = funcInit     ) ) ) ) ||
                    ( 0 == ( m_pfnExit       = ( PFN_gsapi_exit                )::GetProcAddress( m_hLib, ( pFunc = funcExit     ) ) ) )    )
            {
                unloadLib();

                VarString msg;
                msg.format( "GetProcAddress(\"%s\") failed", pFunc );
                throw InvalidParameterException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "loadLib(this = 0x%08X): LoadLibrary(%s) = 0x%08X; GetProcAddress() successful",
                            this,
                            libName,
                            m_hLib );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

#endif

        }

//------------------------------------------------------------------------------
//! \throw NullReferenceException, InvalidParameterException
//!
        void PsInterpreter::unloadLib()
        {
            static ConstString func = "basar.cmnutil.PsInterpreter.unloadLib()";

            if ( ( HINSTANCE )0 == m_hLib )
            {
                throw NullReferenceException( ExceptInfo( func, "m_hLib == 0", __FILE__, __LINE__ ) );
            }

#ifdef _WIN32

            if ( 0 == FreeLibrary( m_hLib ) )
            {
                throw InvalidParameterException( ExceptInfo( func, "FreeLibrary() failed", __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "unloadLib(this = 0x%08X): FreeLibrary(m_hLib = 0x%08X) successful",
                            this,
                            m_hLib );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

#endif
#ifdef _AIX

            if ( 0 != ::dlclose( m_hLib ) )
            {
                throw InvalidParameterException( ExceptInfo( func, "dlclose() failed", __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "unloadLib(this = 0x%08X): dlclose(m_hLib = 0x%08X) successful",
                            this,
                            m_hLib );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

#endif
            m_hLib = NULL;

        }

//------------------------------------------------------------------------------
//! \throw RuntimeException, NullReferenceException
//!
        void PsInterpreter::createInstance()
        {
            static ConstString func = "basar.cmnutil.PsInterpreter.createInstance()";

            if ( 0 != m_pInstance )
            {
                throw NullReferenceException( ExceptInfo( func, "m_pInstance != 0", __FILE__, __LINE__ ) );
            }

            if ( 0 > m_pfnNewInst( &m_pInstance, this ) ) // "this" pointer will be provided to stdio callback functions
            {
                throw RuntimeException( ExceptInfo( func, "creating Ghostscript instance failed", __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "createInstance(this = 0x%08X): create new instance(this = 0x%08X) = 0x%08X",
                            this,
                            this,
                            m_pInstance );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }
        }

//------------------------------------------------------------------------------
//! \throw NullReferenceException
//!
        void PsInterpreter::closeInstance()
        {
            if ( 0 == m_pInstance )
                throw NullReferenceException( ExceptInfo( "basar.cmnutil.PsInterpreter.closeInstance()",
                                              "m_pInstance == 0",
                                              __FILE__, __LINE__ ) );

            m_pfnDelInst( m_pInstance );

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "closeInstance(this = 0x%08X): delete instance(m_pInstance = 0x%08X) successful",
                            this,
                            m_pInstance );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }

            m_pInstance = 0;
        }

//------------------------------------------------------------------------------
//! \throw RuntimeException
//!
        void PsInterpreter::setStdIo()
        {
            if ( 0 > m_pfnSetStdIo ( m_pInstance, &Stdio::onInput ,
                                     &Stdio::onOutput,
                                     &Stdio::onError ) )
                throw RuntimeException( ExceptInfo( "basar.cmnutil.PsInterpreter.setStdIo()",
                                                    "setting Ghostscript stdio failed", __FILE__, __LINE__ ) );

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "setStdIo(this = 0x%08X, m_pInstance = 0x%08X)",
                            this,
                            m_pInstance );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }
        }

//----------------------------------------------------------------------------
//! \throw RuntimeException
//!      typical sequence of callbacks would be:
//! \n      1) open, presize, memalloc, size,
//! \n      2) presize, memfree, memalloc, size,
//! \n      3) sync, page, (repeated if more than 1 page)
//! \n      4) sync, sync,
//! \n      5) preclose, memfree, close
//! \n   caller should not access the image buffer:
//! \n      - before the first sync
//! \n      - between presize and size
//! \n      - after preclose
//! \n   if opening the device fails, you might see the following:
//! \n      open, presize, memalloc, memfree, close
        void PsInterpreter::setDisplay()
        {
            static display_callback display =
            {
                sizeof( display_callback ),
                DISPLAY_VERSION_MAJOR,
                DISPLAY_VERSION_MINOR,
                Display::onOpen,
                Display::onPreclose,
                Display::onClose,
                Display::onPresize,
                Display::onSize,
                Display::onSync,
                Display::onPage,
                NULL,                   // update
                NULL,                   // memalloc
                NULL,                   // memfree
                NULL,                   // separation
                NULL,                   // adjust_band_height
                NULL                    // rectangle_request
            };

            int ret;

            if ( 0 != ( ret = m_pfnSetDisplay( m_pInstance, &display ) ) )
            {
                VarString msg;
                msg.format( "set setDisplay(this = 0x%08X, m_pInstance = 0x%08X) failed with %d",
                            this,
                            m_pInstance,
                            ret );
                throw RuntimeException( ExceptInfo( "basar.cmnutil.PsInterpreter.setDisplay()",
                                                    msg,
                                                    __FILE__, __LINE__ ) );
            }

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "setDisplay(this = 0x%08X, m_pInstance = 0x%08X, display 0x%08X)",
                            this,
                            m_pInstance,
                            &display );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }
        }

//------------------------------------------------------------------------------
//! \throw RuntimeException
//!
        void PsInterpreter::execute(
            const CollArgs& args,
            CallerDeviceEnum eDevice  )
        {
            static ConstString func = "basar.cmnutil.PsInterpreter.execute()";

            createInstance();   // interpreter instance
            setStdIo      ();   // stdinput, stdoutput, stderror callbacks
#ifdef _WIN32
            setDisplay    ();   // set display callbacks
#endif

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;
                msg.format( "execute(this = 0x%08X): init'ing instance(m_pInstance = 0x%08X, \"%s\")",
                            this,
                            m_pInstance,
                            args.dumpArgs().c_str() );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }


            // --- set parameter encoding before execution
            const Int32 encoding = GS_ARG_ENCODING_UTF8;
            Int32 ret = m_pfnArgCoding( m_pInstance, encoding );

            if ( 0 != ret )
            {
                VarString msg;
                msg.format(
                    "set Ghostscript argument encoding (this = 0x%08X, m_pInstance = 0x%08X, "
                    "encoding = %d) failed with %d",
                    this,
                    m_pInstance,
                    encoding,
                    ret );
                throw RuntimeException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
            }


            // --- convert parameter to utf
            EncodedArgs encodedargs( args );


            // --- execute Ghostscript with utf-8 encoded arguments
            ret = m_pfnInit( m_pInstance, int(args.size()), encodedargs.get() );

            if ( ( 0 != ret ) && m_PendingError ) // message on stderror or stdout
            {
                // print: escaping printer setup dialog causes e_Fatal -> no exception
                //        but: other exception reasons with e_Fatal are not detected -> not yet solved
                if ( ( gs_error_Fatal != ret ) || ( PsInterpreter::CALLER_PRINT != eDevice ) )
                {
                    VarString msg;
                    msg.format(
                        "init Ghostscript(this = 0x%08X, m_pInstance = 0x%08X) via \"%s\" failed "
                        "with %d\n"
                        "%s",
                        this,
                        m_pInstance,
                        args.dumpArgs().c_str(),
                        ret,
                        m_ErrorMsg.c_str() );
                    throw RuntimeException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
                }
            }


            // --- exit on error
            if ( 0 != ( ret = m_pfnExit( m_pInstance ) ) )
            {
                VarString msg;
                msg.format( "exit Ghostscript(this = 0x%08X, m_pInstance = 0x%08X) failed with %d",
                            this,
                            m_pInstance,
                            ret );
                throw RuntimeException( ExceptInfo( func, msg, __FILE__, __LINE__ ) );
            }


            // --- cleanup on success
            clearError();
            clearQueue();
            m_pDataProvider = 0;

            closeInstance();

        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void PsInterpreter::setDataProvider( IPrintDataProvider& rDataProvider,
                                             Int32                fromPage     ,
                                             Int32                toPage       )
        {
            m_pDataProvider = &rDataProvider;
            initQueue( fromPage, toPage );  // init queue with trigger
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void PsInterpreter::clearError()
        {
            m_PendingError = false;

            m_ErrorMsg.clear();
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void PsInterpreter::clearQueue()
        {
            while ( !m_collRawInput.empty() )
            {
                m_collRawInput.pop();
            }
        }

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//!
        void PsInterpreter::initQueue( Int32 fromPage,
                                       Int32 toPage  )
        {
            if ( ! m_collRawInput.empty() )
                throw InvalidStateException( ExceptInfo( "basar.cmnutil.PsInterpreter.initQueue",
                                             "queue m_collRawInput not empty",
                                             __FILE__, __LINE__ ) );

            static const VarString formPathClient = PrintTools::getFormPathClient(); // need to compute only once, since it doesn't change

            VarString trigger = PrintTools::getTrigger( formPathClient                    ,
                                m_pDataProvider->getFormFileName(),
                                m_pDataProvider->getFormName    (),
                                fromPage                          ,
                                toPage                            );
            PrintTools::insertStr2Coll( trigger, m_collRawInput );

            if ( LoggerPool::psInterpreter().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                VarString msg;

                msg.format( "initQueue() inserts trigger lines:\n"
                            "%s",
                            trigger.c_str() );
                LoggerPool::psInterpreter().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
            }
        }

//----------------------------------------------------------------------------
    }   // cmnutil
}   // basar

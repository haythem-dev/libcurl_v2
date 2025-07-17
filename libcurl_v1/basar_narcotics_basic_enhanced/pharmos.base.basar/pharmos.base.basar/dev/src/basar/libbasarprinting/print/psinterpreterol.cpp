//----------------------------------------------------------------------------
/*! \file
 *  \brief  ghostscript OL replacement interpreter class
 *  \author Juri Skuratovski
 *  \date   27.05.2023
 */
 //----------------------------------------------------------------------------

#include "psinterpreterol.h"

#include "libbasarcmnutil_print.h"
#include "libbasarlogin_manager.h"
#include "libbasar_printingol.h"

#include "../loggerpool.h"
#include "ps/encodedargs.h"

//#include <gdevdsp.h>
//#include <ierrors.h>

#ifdef _AIX
#include <errno.h>
#include <dlfcn.h>
#endif

//----------------------------------------------------------------------------
namespace basar
{
    namespace printing
    {

        //------------------------------------------------------------------------------

//        //! \retval >0 number of characters read
//        //! \retval 0 for EOF
//        //! \retval -1 for error
//        //! \throw  no-throw
//        Int32 PsInterpreterOL::Stdio::onInput(void* pInterpreterOL, char* pBuf, Int32 buflen)
//        {
//            CollPrintRawInput& rCollInput = (static_cast<PsInterpreterOL*> (pInterpreterOL))->m_collRawInput;
//            IPrintDataProvider& rDataProv = *((static_cast<PsInterpreterOL*> (pInterpreterOL))->m_pDataProvider);
//            Int32                queuedBytes = static_cast<const Int32> (rCollInput.size());
//
//            if (0 == queuedBytes) // refresh queue
//            {
//                rDataProv.onSendData(rCollInput);
//                queuedBytes = static_cast <const Int32> (rCollInput.size());
//            }
//
//
//#ifndef NDEBUG  // ---  call statistics ----------------------------------
//
//            static int count = 0;   // number of calls
//            static int bytes = 0;   // requested bytes by ps interpreter
//            static int peak = 0;   // maximum queue size
//
//            ++count;
//            bytes += buflen;
//
//            if (queuedBytes > peak)
//            {
//                peak = queuedBytes;
//            }
//
//            if (0 == queuedBytes) // no more data
//            {
//                count = bytes = peak = 0;
//            }
//
//#endif // NDEBUG  --------------------------------------------------------
//
//
//            if (0 == queuedBytes) // no more data
//            {
//                return 0;
//            }
//
//            // send data to ps interpreter
//            const Int32        copybytes = (queuedBytes <= buflen) ? queuedBytes : buflen;
//            const char* const pEnd = pBuf + copybytes;
//
//            for (; pBuf < pEnd; ++pBuf)
//            {
//                *pBuf = rCollInput.front();     // copy char from queue to ps buffer
//                rCollInput.pop();               // delete char from queue
//            }
//
//            return copybytes;
//        }
//
//        //------------------------------------------------------------------------------
//        //! \return number of characters written
//        //! \throw  no-throw
//        Int32 PsInterpreterOL::Stdio::onOutput(void* pInterpreterOL, const char* pBuf, Int32 buflen)
//        {
//            PsInterpreterOL* pObj = static_cast<PsInterpreterOL*>(pInterpreterOL);
//
//            pObj->m_PendingError = true;
//            pObj->m_ErrorMsg += pBuf;
//
//            return buflen;
//        }

        //------------------------------------------------------------------------------
        //! \return number of characters written
        //! \throw no-throw
        //Int32 PsInterpreterOL::Stdio::onError(void* pInterpreterOL, const char* pBuf, Int32 buflen)
        //{
        //    PsInterpreterOL* pObj = static_cast<PsInterpreterOL*>(pInterpreterOL);

        //    pObj->m_PendingError = true;
        //    pObj->m_ErrorMsg += pBuf;

        //    return buflen;
        //}


        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return return value of IScreenPrint::onOpen if pScreenPrint != null, zero otherwise
        Int32 PsInterpreterOL::Display::onOpen(void* pScreenPrint, void* /* pDevice */)

        {

            if (pScreenPrint)
            {
#ifdef _WIN32
                cmnutil::IScreenPrint& rScreenPrint = *(static_cast<cmnutil::IScreenPrint*>(pScreenPrint));
                return rScreenPrint.onOpen();
#endif
            }

            return 0;
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return always zero
        Int32 PsInterpreterOL::Display::onPreclose(void* /* pScreenPrint*/, void* /* pDevice */)
        {
            return 0;   // do nothing - no thread synchonisation needed
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return return value of IScreenPrint::onClose if pScreenPrint != null, zero otherwise
        Int32 PsInterpreterOL::Display::onClose(void* pScreenPrint, void* /* pDevice */)
        {

            if (pScreenPrint)
            {
#ifdef _WIN32
                cmnutil::IScreenPrint& rScreenPrint = *(static_cast<cmnutil::IScreenPrint*>(pScreenPrint));
                return rScreenPrint.onClose();
#endif
            }

            return 0;
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return always zero
        Int32 PsInterpreterOL::Display::onPresize(void* /* pScreenPrint */,
            void* /* pDevice      */,
            Int32  /* width        */,
            Int32  /* height       */,
            Int32  /* raster       */,
            UInt32 /* format       */)
        {
            return 0; // do nothing - no thread synchonisation needed
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return return value of IScreenPrint::onSize if pScreenPrint != null, zero otherwise
        Int32 PsInterpreterOL::Display::onSize(void* pScreenPrint, void* /* pDevice */,
            Int32           width,
            Int32           height,
            Int32           raster,
            UInt32          /* format  */,
            unsigned char* pImage)
        {
            if (pScreenPrint)
            {
#ifdef _WIN32
                cmnutil::IScreenPrint& rScreenPrint = *(static_cast<cmnutil::IScreenPrint*>(pScreenPrint));
                return rScreenPrint.onSize(width, height, raster, pImage);
#endif
#ifdef _AIX

                if (width && height && raster && pImage)
                {
                }

#endif
            }

            return 0;
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return always zero
        Int32 PsInterpreterOL::Display::onSync(void* /* pScreenPrint */, void* /* pDevice   */)
        {
            return 0; // do nothing - no thread synchonisation needed
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //! \return return value of IScreenPrint::onPage if pScreenPrint != null, zero otherwise
        Int32 PsInterpreterOL::Display::onPage(void* pScreenPrint,
            void* /* pDevice */,
            Int32  /* copies  */,
            Int32  /* flush   */)
        {
            if (pScreenPrint)
            {
#ifdef _WIN32
                cmnutil::IScreenPrint& rScreenPrint = *(static_cast<cmnutil::IScreenPrint*>(pScreenPrint));
                return rScreenPrint.onPage();
#endif
            }

            return 0;
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //!
        PsInterpreterOL::PsInterpreterOL() : m_pDataProvider(0)
        {
            InitRawPrint();
            if (LoggerPool::psInterpreterOL().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;
                msg.format("PsInterpreterOL(this = 0x%08X)",
                    this);
                LoggerPool::psInterpreterOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

            }

            clearError();   // clear error msg, flag

            //loadLib();   // library

        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //!
        PsInterpreterOL::~PsInterpreterOL()
        {
            if (LoggerPool::psInterpreterOL().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;
                msg.format("~PsInterpreterOL(this = 0x%08X)",
                    this);
                LoggerPool::psInterpreterOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            //if (NULL != m_pInstance)
            //{
            //    closeInstance();
            //}

            //unloadLib();
        }

        //------------------------------------------------------------------------------
        //! \throw RuntimeException
        //!
        void PsInterpreterOL::execute(
            const cmnutil::CollArgs& args,
            CallerDeviceEnum eDevice)
        {
            // static ConstString func = "basar.cmnutil.PsInterpreterOL.execute()";

            if (LoggerPool::psInterpreterOL().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;
                msg.format("execute(this = 0x%08X), \"%s\")",
                    this,
                    args.dumpArgs().c_str());
                LoggerPool::psInterpreterOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                /// <summary>
                ///  JSI only to avoid warning as error due to not used eDevice!!
                /// </summary>
                /// <param name="args"></param>
                /// <param name="eDevice"></param>
                msg.format("CallerDevice:%d", eDevice);
                LoggerPool::psInterpreterOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            //// --- set parameter encoding before execution
            //const Int32 encoding = GS_ARG_ENCODING_UTF8;
            //Int32 ret = m_pfnArgCoding(m_pInstance, encoding);

            //if (0 != ret)
            //{
            //    VarString msg;
            //    msg.format(
            //        "set Ghostscript argument encoding (this = 0x%08X, m_pInstance = 0x%08X, "
            //        "encoding = %d) failed with %d",
            //        this,
            //        m_pInstance,
            //        encoding,
            //        ret);
            //    throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__));
            //}

            // send data to OL
            _rawPrint->print(m_RwprServer, m_RwprPrinter, *m_pDataProvider);
            
            /// JSI that's all, no code more at this place

            // --- cleanup on success
            clearError();
            clearQueue();
            m_pDataProvider = 0;
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //!
        void PsInterpreterOL::setDataProvider(cmnutil::IPrintDataProvider& rDataProvider,
            Int32                fromPage,
            Int32                toPage)
        {
            m_pDataProvider = &rDataProvider;
            initQueue(fromPage, toPage);  // init queue with trigger
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //!
        void PsInterpreterOL::clearError()
        {
            m_PendingError = false;

            m_ErrorMsg.clear();
        }

        //------------------------------------------------------------------------------
        //! \throw no-throw
        //!
        void PsInterpreterOL::clearQueue()
        {
            while (!m_collRawInput.empty())
            {
                m_collRawInput.pop();
            }
        }

        //------------------------------------------------------------------------------
        //! \throw  InvalidStateException
        //!
        void PsInterpreterOL::initQueue(Int32 fromPage,
            Int32 toPage)
        {
            if (!m_collRawInput.empty())
                throw InvalidStateException(ExceptInfo("basar.cmnutil.PsInterpreterOL.initQueue",
                    "queue m_collRawInput not empty",
                    __FILE__, __LINE__));

            //static const VarString formPathClient = cmnutil::PrintTools::getFormPathClient(); // need to compute only once, since it doesn't change

            VarString trigger = cmnutil::PrintTools::getTrigger("",
                m_pDataProvider->getFormFileName(),
                m_pDataProvider->getFormName(),
                fromPage,
                toPage);
            cmnutil::PrintTools::insertStr2Coll(trigger, m_collRawInput);

            if (LoggerPool::psInterpreterOL().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;

                msg.format("initQueue() inserts trigger lines:\n"
                    "%s",
                    trigger.c_str());
                LoggerPool::psInterpreterOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            /*! \throw no-throw */
        }

        void PsInterpreterOL::setRawPrintInfo(const basar::VarString& rServer, const basar::VarString& rPrinter)
        {
            m_RwprServer = rServer;
            m_RwprPrinter = rPrinter;
        }

        void PsInterpreterOL::InitRawPrint()
        {
            _rawPrint = new cmnutil::RawPrint();

            setRawPrintFromConfig("OL_PRINT_SERVER", "OL_PRINT_QUEUE");
        }

        void PsInterpreterOL::setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
            const basar::I18nString& printerCfgName)	//!< printer name)
        {
            basar::I18nString olServer;
            basar::printing::PrintUtils::getParaValue(serverCfgName, olServer);
            basar::I18nString olPrinter;
            basar::printing::PrintUtils::getParaValue(printerCfgName, olPrinter);
            
            setRawPrintInfo(olServer, olPrinter);
        }


        //----------------------------------------------------------------------------
    }   // cmnutil
}   // basar

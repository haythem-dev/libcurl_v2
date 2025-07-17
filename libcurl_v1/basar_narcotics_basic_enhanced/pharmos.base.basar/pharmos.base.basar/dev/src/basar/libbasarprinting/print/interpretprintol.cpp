//----------------------------------------------------------------------------
/*! \file
 *  \brief  postscript interpreting class via ObjectiveLune
 *  \author Juri SKuratovski
 *  \date   05.06.2023
 * based on interpritprint.cpp
 */
//----------------------------------------------------------------------------
#include "libbasar_printingol.h"
#include "psinterpreterol.h"

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include <inttypes.h>

#include "libbasarlogin_manager.h"
#include "ps/collargs.h"
#include "loggerpool.h"

//#include "../libbasarcmnutil_print.h"
//#include "../libbasarcmnutil_i18nstring.h"
//#include "psinterpreterol.h"

namespace fs = boost::filesystem;

//----------------------------------------------------------------------------
namespace basar
{
    namespace printing
    {

//------------------------------------------------------------------------------
//  init statics
//------------------------------------------------------------------------------
//! init static variable for controlling single object
#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4592) // warning C4592: 'm_pWeakPsInterpreter': symbol will be dynamically initialized (implementation limitation)
#endif // _WIN32
        InterpretPrintOL::PsInterpretOLWeakPtr InterpretPrintOL::m_pWeakPsInterpreterOL;
#ifdef _WIN32
#pragma warning( pop )
#endif // _WIN32

        boost::mutex InterpretPrintOL::m_InterpreterOLGuard;
//------------------------------------------------------------------------------
//! \throw  RuntimeException
//! \return display format value for postscript interpreter
        //Int32 InterpretPrintOL::getDisplayFormat ( const DisplayBppEnum bpp )
        //{
        //    // format
        //    Int32 fmt = DISPLAY_ALPHA_NONE | DISPLAY_UNUSED_LAST  | DISPLAY_LITTLEENDIAN | DISPLAY_TOPFIRST;

        //    switch ( bpp )
        //    {
        //        case DISP_COLOR_32:
        //            fmt |= DISPLAY_COLORS_RGB    | DISPLAY_DEPTH_8;
        //            break;

        //        case DISP_COLOR_16:
        //            fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_16 | DISPLAY_NATIVE_565;
        //            break;

        //        case DISP_COLOR_15:
        //            fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_16 | DISPLAY_NATIVE_555;
        //            break;

        //        case DISP_COLOR_8:
        //            fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_8;
        //            break;

        //        case DISP_COLOR_4:
        //            fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_4;
        //            break;

        //        case DISP_MONO:
        //            fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_1;
        //            break;

        //        default:
        //        {
        //            VarString msg;
        //            msg.format( "unknown display color depth %d", bpp );
        //            throw RuntimeException( ExceptInfo( "basar.cmnutil.InterpretPrint.getDisplayFormat()",
        //                                                msg,
        //                                                __FILE__, __LINE__ ) );
        //        }
        //        break;
        //    }

        //    return fmt;
        //}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
        InterpretPrintOL::InterpretPrintOL ()
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            if ( m_pWeakPsInterpreterOL.expired() )
            {
                m_pPsInterpreterOL     = PsInterpretOLSharedPtr( new PsInterpreterOL );
                m_pWeakPsInterpreterOL = m_pPsInterpreterOL;
            }
            else
            {
                m_pPsInterpreterOL     = PsInterpretOLSharedPtr( m_pWeakPsInterpreterOL );
            }
        }

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
        InterpretPrintOL::~InterpretPrintOL ()
        {
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrintOL::print( cmnutil::IPrintDataProvider& rDataProvider      ,
                                    Int32                fromPage /* = -1 */,
                                    Int32                toPage   /* = -1 */ )
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            m_pPsInterpreterOL->setDataProvider( rDataProvider, fromPage, toPage );

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            //insertPrintOptions ( args );

            //args.push_back( s_InputControl );

            m_pPsInterpreterOL->execute( args, PsInterpreterOL::CALLER_PRINT );
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrintOL::print( const VarString& rInputFileName )
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            //insertPrintOptions ( args );

            //args.push_back( s_InputFile );
            args.push_back( rInputFileName );

            m_pPsInterpreterOL->execute( args, PsInterpreterOL::CALLER_PRINT );
        }

        void InterpretPrintOL::setRawPrintInfo(const basar::VarString& rServer, const basar::VarString& rPrinter)
        {
            m_Server = rServer;
            m_Printer = rPrinter;
            m_pPsInterpreterOL->setRawPrintInfo(rServer, rPrinter);
        }

        void InterpretPrintOL::setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
            const basar::I18nString& printerCfgName)	//!< printer name)
        {
            basar::I18nString olServer;
            basar::printing::PrintUtils::getParaValue(serverCfgName, olServer);
            basar::I18nString olPrinter;
            basar::printing::PrintUtils::getParaValue(printerCfgName, olPrinter);
            setRawPrintInfo(olServer, olPrinter);
        }
        //------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrintOL::display( cmnutil::IPrintDataProvider& rDataProvider      ,
                                      Int32                fromPage /* = -1 */,
                                      Int32                toPage   /* = -1 */ )
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            m_pPsInterpreterOL->setDataProvider( rDataProvider, fromPage, toPage );

            cmnutil::CollArgs args;
            //insertCommonOptions ( args );
            //insertDisplayOptions( args, rScreenPrint );

            //args.push_back( s_InputControl );

            m_pPsInterpreterOL->execute( args, PsInterpreterOL::CALLER_DISPLAY );
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrintOL::display( const VarString&     rInputFileName)
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            cmnutil::CollArgs args;
            //insertCommonOptions ( args );
            //insertDisplayOptions( args, rScreenPrint );

            //args.push_back( s_InputFile );
            args.push_back( rInputFileName );

            m_pPsInterpreterOL->execute( args, PsInterpreterOL::CALLER_DISPLAY );
        }


//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrintOL::savePdf( cmnutil::IPrintDataProvider& rDataProvider      ,
                                      const VarString&     rOutputFileName    ,
                                      Int32                fromPage /* = -1 */,
                                      Int32                toPage   /* = -1 */ )
        {

            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            m_pPsInterpreterOL->setDataProvider( rDataProvider, fromPage, toPage );

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            insertPdfOptions   ( args, rOutputFileName );

            //args.push_back( s_InputControl );

            m_pPsInterpreterOL->execute( args, PsInterpreterOL::CALLER_PDF );

            // get temprary file and move/rename it to the target
            basar::I18nString sharedDir;
            basar::printing::PrintUtils::getParaValue("OL_PRINT_SHARED_DIR", sharedDir);
            basar::I18nString fName = rDataProvider.getFormFileName(); // getFormFileName();
            fs::path fPathName(fName.c_str());
            fPathName.replace_extension("pdf");

#ifdef _WIN32
            VarString fileName = VarString(sharedDir.c_str()) + "\\" + VarString(fPathName.string().c_str());
#else
            VarString fileName = VarString(sharedDir.c_str()) + "/" + VarString(fPathName.string().c_str());
#endif

            //wait for the file to be created by OL PrintServer before renaming it
            if (!fileName.empty())
            {
                // read timeout from uadm config
                basar::I18nString olPreviewTimeout;
                int previewTimeoutMs = 30 * 1000; // 30 s default timeout)
                if (basar::printing::PrintUtils::getParaValue("OL_PREVIEW_TIMEOUT", olPreviewTimeout))
                {
                    try
                    {
                        previewTimeoutMs = (int)olPreviewTimeout.stoi() * 1000;
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PREVIEW_TIMEOUT could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                    }
                }

                // read poll interval for file to be transmitted from OL
                basar::I18nString olPollInterval;
                int defCheckFileDelay = 1000; // ms - do not check to fast or soon, it would slow down the process
                if (basar::printing::PrintUtils::getParaValue("OL_PRINT_FILE_POLLINTERV_MS", olPollInterval))
                {
                    try
                    {
                        defCheckFileDelay = (int)olPollInterval.stoi();
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PRINT_FILE_POLLINTERV_MS could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        defCheckFileDelay = 1000; // back to default 1000 ms = 1s
                    }

                }
                int max_repeat = previewTimeoutMs / defCheckFileDelay;
                int counter = 0;
                while (!fs::exists(fileName.c_str()) && ++counter < max_repeat)
                {
                    VarString msg;
                    msg.format("%d: polling for OL PDF file: %s", counter, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    boost::this_thread::sleep(boost::posix_time::milliseconds(defCheckFileDelay));
                }
                if (counter < max_repeat && fs::exists(fileName.c_str()))
                {
                    VarString msg;
                    msg.format("Input PDF file: %s", fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                    msg.format("Output PDF file: %s", rOutputFileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    //fs::rename(fileName.c_str(), rOutputFileName.c_str());
                    try
                    {
                        boost::system::error_code ec;
                        fs::copy_file(fileName.c_str(), rOutputFileName.c_str(), fs::copy_option::overwrite_if_exists, ec);
                        if (ec.failed())
                        {
                            msg.format("Error copying file from %s to %s: %s", fileName.c_str(), rOutputFileName.c_str(), ec.message().c_str());
                            LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                        else
                        {
                            msg.format("File copied successfully from %s to %s", fileName.c_str(), rOutputFileName.c_str());
                            LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                        if (!fs::remove(fileName.c_str(), ec))
                        {
                            msg.format("Error removing temporary file %s: %s", fileName.c_str(), ec.message().c_str());
                            LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                    }
                    catch (const fs::filesystem_error& e)
                    {
                        msg.format("Error copying file from %s to %s: %s", fileName.c_str(), rOutputFileName.c_str(), e.what());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                    }
                }
                else
                {
                    VarString msg;
                    msg.format("%d ms / timeout reached - missing OL PDF file: %s", previewTimeoutMs, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }
        }

        void InterpretPrintOL::saveCopyPdf(cmnutil::IPrintDataProvider& rDataProvider,
            const VarString& rOutputFileName,
            Int32                fromPage /* = -1 */,
            Int32                toPage   /* = -1 */)
        {

            boost::mutex::scoped_lock lock(m_InterpreterOLGuard);

            m_pPsInterpreterOL->setDataProvider(rDataProvider, fromPage, toPage);

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            insertPdfOptions(args, rOutputFileName);

            //args.push_back( s_InputControl );

            m_pPsInterpreterOL->execute(args, PsInterpreterOL::CALLER_PDF);

            // get temprary file and move/rename it to the target
            basar::I18nString sharedDir;
            basar::printing::PrintUtils::getParaValue("OL_PRINT_SHARED_DIR", sharedDir);
            basar::I18nString fName = rDataProvider.getFormFileName(); // getFormFileName();
            fs::path fPathName(fName.c_str());
            fPathName.replace_extension("pdf");

#ifdef _WIN32
            VarString fileName = VarString(sharedDir.c_str()) + "\\" + VarString(fPathName.string().c_str());
#else
            VarString fileName = VarString(sharedDir.c_str()) + "/" + VarString(fPathName.string().c_str());
#endif
            //wait for the file to be created by OL PrintServer before renaming it
            if (!fileName.empty())
            {
                // read timeout from uadm config
                basar::I18nString olPreviewTimeout;
                int previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                if (basar::printing::PrintUtils::getParaValue("OL_PREVIEW_TIMEOUT", olPreviewTimeout))
                {
                    try
                    {
                        previewTimeoutMs = (int)olPreviewTimeout.stoi() * 1000;
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PREVIEW_TIMEOUT could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                    }
                }

                // read poll interval for file to be transmitted from OL
                basar::I18nString olPollInterval;
                int defCheckFileDelay = 1000; // ms - do not check to fast or soon, it would slow down the process
                if (basar::printing::PrintUtils::getParaValue("OL_PRINT_FILE_POLLINTERV_MS", olPollInterval))
                {
                    try
                    {
                        defCheckFileDelay = (int)olPollInterval.stoi();
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PRINT_FILE_POLLINTERV_MS could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        defCheckFileDelay = 1000; // back to default 1000 ms = 1s
                    }

                }
                int max_repeat = previewTimeoutMs / defCheckFileDelay;

                int counter = 0;
                while (!fs::exists(fileName.c_str()) && ++counter < max_repeat)
                {
                    VarString msg;
                    msg.format("%d: polling for OL PDF file: %s", counter, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    boost::this_thread::sleep(boost::posix_time::milliseconds(defCheckFileDelay));
                }
                if (counter < max_repeat && fs::exists(fileName.c_str()))
                {
                    VarString msg;
                    msg.format("Input PDF file: %s", fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                    msg.format("Output PDF file: %s", rOutputFileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    fs::copy_file(fileName.c_str(), rOutputFileName.c_str(), boost::filesystem::copy_option::overwrite_if_exists);
                }
                else
                {
                    VarString msg;
                    msg.format("%d ms / timeout reached - missing OL PDF file: %s", previewTimeoutMs, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }
        }
//------------------------------------------------------------------------------
//! \throw RuntimeException when rInputFileName specifies non-existing file
//!
        void InterpretPrintOL::savePdf( const VarString& rInputFileName ,
                                      const VarString& rOutputFileName )
        {
            boost::mutex::scoped_lock lock( m_InterpreterOLGuard );

            checkInputFileExists( rInputFileName );

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            insertPdfOptions   ( args, rOutputFileName );
            cmnutil::RawPrint* rawPrint = m_pPsInterpreterOL->GetRawPrint();
            rawPrint->print(m_Server, m_Printer, rInputFileName);

            /// JSI check if and how the input name will be used!
            /// should we expect renaming to uuid-file or file name with time stamp?
            // get temprary file and move/rename it to the target
            basar::I18nString sharedDir;
            basar::printing::PrintUtils::getParaValue("OL_PRINT_SHARED_DIR", sharedDir);
            basar::I18nString fName = rInputFileName; // getFormFileName();
            fs::path fPathName(fName.c_str());
            fPathName.replace_extension("pdf");

            VarString fileName = VarString(sharedDir.c_str()) + "\\" + VarString(fPathName.string().c_str());
            //wait for the file to be created by OL PrintServer before renaming it
            if (!fileName.empty())
            {
                // read timeout from uadm config
                basar::I18nString olPreviewTimeout;
                int previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                if (basar::printing::PrintUtils::getParaValue("OL_PREVIEW_TIMEOUT", olPreviewTimeout))
                {
                    try
                    {
                        previewTimeoutMs = (int)olPreviewTimeout.stoi() * 1000;
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PREVIEW_TIMEOUT could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                    }
                }

                // read poll interval for file to be transmitted from OL
                basar::I18nString olPollInterval;
                int defCheckFileDelay = 1000; // ms - do not check to fast or soon, it would slow down the process
                if (basar::printing::PrintUtils::getParaValue("OL_PRINT_FILE_POLLINTERV_MS", olPollInterval))
                {
                    try
                    {
                        defCheckFileDelay = (int)olPollInterval.stoi();
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PRINT_FILE_POLLINTERV_MS could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        defCheckFileDelay = 1000; // back to default 1000 ms = 1s
                    }

                }

                int max_repeat = previewTimeoutMs / defCheckFileDelay;
                int counter = 0;
                while (!fs::exists(fileName.c_str()) && ++counter < max_repeat)
                {
                    VarString msg;
                    msg.format("%d: polling for OL PDF file: %s", counter, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    boost::this_thread::sleep(boost::posix_time::milliseconds(defCheckFileDelay));
                }
                if (counter < max_repeat && fs::exists(fileName.c_str()))
                {
                    VarString msg;
                    msg.format("Input PDF file: %s", fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                    msg.format("Output PDF file: %s", rOutputFileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    //fs::rename(fileName.c_str(), rOutputFileName.c_str());
                    try
                    {
                        boost::system::error_code ec;
                        fs::copy_file(fileName.c_str(), rOutputFileName.c_str(), fs::copy_option::overwrite_if_exists, ec);
						if (ec.failed())
						{
							msg.format("Error copying file from %s to %s: %s", fileName.c_str(), rOutputFileName.c_str(), ec.message().c_str());
							LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
						}
                        else
                        {
							msg.format("File copied successfully from %s to %s", fileName.c_str(), rOutputFileName.c_str());
							LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                        if (!fs::remove(fileName.c_str(), ec))
                        {
							msg.format("Error removing temporary file %s: %s", fileName.c_str(), ec.message().c_str());
							LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                    }
					catch (const fs::filesystem_error& e)
					{
						msg.format("Error copying file from %s to %s: %s", fileName.c_str(), rOutputFileName.c_str(), e.what());
						LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
					}
                }
                else
                {
                    VarString msg;
                    msg.format("%d ms / timeout reached - missing OL PDF file: %s", previewTimeoutMs, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }            
        }

        void InterpretPrintOL::saveCopyPdf(const VarString& rInputFileName,
            const VarString& rOutputFileName)
        {
            boost::mutex::scoped_lock lock(m_InterpreterOLGuard);

            checkInputFileExists(rInputFileName);

            cmnutil::CollArgs args;
            //insertCommonOptions( args );
            insertPdfOptions(args, rOutputFileName);
            cmnutil::RawPrint* rawPrint = m_pPsInterpreterOL->GetRawPrint();
            rawPrint->print(m_Server, m_Printer, rInputFileName);

            /// JSI check if and how the input name will be used!
            /// should we expect renaming to uuid-file or file name with time stamp?
            // get temprary file and move/rename it to the target
            basar::I18nString sharedDir;
            basar::printing::PrintUtils::getParaValue("OL_PRINT_SHARED_DIR", sharedDir);
            basar::I18nString fName = rInputFileName; // getFormFileName();
            fs::path fPathName(fName.c_str());
            fPathName.replace_extension("pdf");

            VarString fileName = VarString(sharedDir.c_str()) + "\\" + VarString(fPathName.string().c_str());
            //wait for the file to be created by OL PrintServer before renaming it
            if (!fileName.empty())
            {
                // read timeout from uadm config
                basar::I18nString olPreviewTimeout;
                int previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                if (basar::printing::PrintUtils::getParaValue("OL_PREVIEW_TIMEOUT", olPreviewTimeout))
                {
                    try
                    {
                        previewTimeoutMs = (int)olPreviewTimeout.stoi() * 1000;
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PREVIEW_TIMEOUT could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                    }
                }

                // read poll interval for file to be transmitted from OL
                basar::I18nString olPollInterval;
                int defCheckFileDelay = 1000; // ms - do not check to fast or soon, it would slow down the process
                if (basar::printing::PrintUtils::getParaValue("OL_PRINT_FILE_POLLINTERV_MS", olPollInterval))
                {
                    try
                    {
                        defCheckFileDelay = (int)olPollInterval.stoi();
                    }
                    catch (...)
                    {
                        VarString msg;
                        msg.format("value of parameter OL_PRINT_FILE_POLLINTERV_MS could not be converted to int <%s>.", olPreviewTimeout.c_str());
                        LoggerPool::InterpretPrintOL().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                        defCheckFileDelay = 1000; // back to default 1000 ms = 1s
                    }

                }
                int max_repeat = previewTimeoutMs / defCheckFileDelay;
                int counter = 0;
                while (!fs::exists(fileName.c_str()) && ++counter < max_repeat)
                {
                    VarString msg;
                    msg.format("%d: polling for OL PDF file: %s", counter, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    boost::this_thread::sleep(boost::posix_time::milliseconds(defCheckFileDelay));
                }
                if (counter < max_repeat && fs::exists(fileName.c_str()))
                {
                    VarString msg;
                    msg.format("Input PDF file: %s", fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                    msg.format("Output PDF file: %s", rOutputFileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);

                    fs::copy_file(fileName.c_str(), rOutputFileName.c_str(), boost::filesystem::copy_option::overwrite_if_exists);
                }
                else
                {
                    VarString msg;
                    msg.format("%d ms / timeout reached - missing OL PDF file: %s", previewTimeoutMs, fileName.c_str());
                    LoggerPool::InterpretPrintOL().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }             
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setPrintBpp( PrintBppEnum eBpp )
        //{
        //    m_PrintProperties.eBpp = eBpp;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setPrintDpi( Int32 dpi )
        //{
        //    m_PrintProperties.dpi = dpi;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setPrintDuplex( bool duplex )
        //{
        //    m_PrintProperties.duplex = duplex;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setPrintPaperSize( PaperSizeEnum size )
        //{
        //    m_PrintProperties.papersize = size;
        //}

//------------------------------------------------------------------------------
//! \throw     no-throw
//! \attention escape backslashes for network printer in a C string!<br>
//!            e.g.: "\\\\FU1S0001\\AE_Laser"
        //void InterpretPrintOL::setPrintSelect( PrintSelectEnum   eSelect,
        //                                     const VarString& rPrinterName /* = "" */ )
        //{
        //    m_PrintProperties.eSelect     = eSelect;
        //    m_PrintProperties.printerName = rPrinterName;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setDisplayBpp( DisplayBppEnum eBpp )
        //{
        //    m_DisplayProperties.eBpp = eBpp;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setDisplayDpi( Int32 dpi )
        //{
        //    m_DisplayProperties.dpi = dpi;
        //}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        //void InterpretPrintOL::setDisplayRender( bool render )
        //{
        //    m_DisplayProperties.render = render;
        //}

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return VarString containing printer settings
        //const VarString InterpretPrintOL::getPrinterSettingPS () const
        //{
        //    // see ghostscript docu: devices - MS Windows printers
        //    static ConstString DEVICE_SETTINGS   = "mark /NoCancel true "                               // set progress dialog
        //                                           "/Duplex %s "                                   // set duplex printing
        //                                           "%s"                                            // optional properties
        //                                           "/UserSettings "
        //                                           "<< /Paper %d "                              // set paper size (wingdi.h)
        //                                           "/MaxResolution %d "                      // set maximum printer resolution
        //                                           ">> "
        //                                           "(mswinpr2) finddevice "                            // get printer device
        //                                           "putdeviceprops "                                   // save device properties
        //                                           "setdevice";                                        // set printer device

        //    // optional
        //    static ConstString BITS_PER_PIXEL    = "/BitsPerPixel %d ";                                 // set device depth to specified bits per pixel
        //    static ConstString STANDARD_PRINTER  = "/QueryUser 3 ";                                     // select default Windows printer without any user interaction
        //    static ConstString DEDICATED_PRINTER = "/OutputFile (%%printer%%%s) ";                      // specifies which printer should be used.
        //    // string should be of the form %printer%printer_name,
        //    // where the printer_name should be typed exactly as it appears
        //    // in Windows printer control panel, including spaces
        //    static ConstString DUPLEX_TRUE       = "true";                                              // duplex printing value for enabled
        //    static ConstString DUPLEX_FALSE      = "false";                                             // duplex printing value for disabled

        //    VarString options;

        //    switch ( m_PrintProperties.eSelect )
        //    {
        //        case PRINTER_STANDARD:
        //            options += STANDARD_PRINTER;
        //            break;

        //        case PRINTER_DEDICATED:
        //            if ( m_PrintProperties.printerName.empty() )
        //                throw InvalidParameterException( ExceptInfo( "basar.cmnutil.InterpretPrintOL::getPrinterSettingPS",
        //                                                 "/OutputFile: printername empty",
        //                                                 __FILE__, __LINE__ ) );

        //            {
        //                // workaround for Ghostscript's strange behaviour: gs eliminates backslash if not escaped
        //                VarString name = m_PrintProperties.printerName;
        //                name.exchange( '\\', "\\\\" );  // means : '\' -> '\\' (one replaced by two)

        //                options.format( DEDICATED_PRINTER, name.c_str() );
        //            }

        //            break;

        //        case PRINTER_DIALOG:
        //        default:
        //            break;
        //    }

        //    if ( PRINT_USER_SETTING != m_PrintProperties.eBpp )
        //    {
        //        VarString bpp;
        //        bpp.format( BITS_PER_PIXEL, m_PrintProperties.eBpp );

        //        options += bpp;
        //    }

        //    VarString code;
        //    code.format( DEVICE_SETTINGS,
        //                 m_PrintProperties.duplex ? DUPLEX_TRUE : DUPLEX_FALSE,
        //                 options.c_str(),
        //                 m_PrintProperties.papersize,
        //                 m_PrintProperties.dpi );

        //    return code;
        //}

//----------------------------------------------------------------------------
//! common GhostScript options, used by all cases
        //void InterpretPrintOL::insertCommonOptions( CollArgs& args )
        //{
        //    args.push_back( s_Dummy );
        //    args.push_back( s_LookForLibs );
        //    args.push_back( s_SearchPath );
        //    args.push_back( s_SaferMode );
        //    args.push_back( s_BatchMode );
        //    args.push_back( s_PauseMode );
        //    args.push_back( s_QuietMode );
        //    args.push_back( s_PromptMode );
        //}

//----------------------------------------------------------------------------
//! common GhostScript options, used by printing
        //void InterpretPrintOL::insertPrintOptions( CollArgs& args )
        //{
        //    VarString dispfmtdmy;
        //    dispfmtdmy.format( "%s%d", s_DisplayFormat, getDisplayFormat( DISP_MONO ) );

        //    VarString prnsetting;
        //    prnsetting.format( "%s"  , getPrinterSettingPS().c_str() );

        //    args.push_back( s_DisplayResolutionDummy );
        //    args.push_back( dispfmtdmy );
        //    args.push_back( s_InputCommand );
        //    args.push_back( prnsetting );
        //}

#ifdef _WIN32

//----------------------------------------------------------------------------
//! common GhostScript options, used by display
        //void InterpretPrintOL::insertDisplayOptions(
        //    CollArgs&            args,
        //    const IScreenPrint& rScreenPrint )
        //{
        //    VarString handle;
        //    handle.format( "%s%" PRIuPTR, s_DisplayHandle, &rScreenPrint );

        //    VarString resolution;
        //    resolution.format( "%s%d", s_DisplayResolution, m_DisplayProperties.dpi );

        //    VarString fmt;
        //    fmt.format( "%s%d", s_DisplayFormat, getDisplayFormat( m_DisplayProperties.eBpp ) );

        //    if ( m_DisplayProperties.render )
        //    {
        //        args.push_back( s_Interpolate );
        //        args.push_back( s_TextAlphaBits );
        //        args.push_back( s_GraphAlphaBits );
        //        args.push_back( s_AlignToPixels );
        //    }

        //    args.push_back( s_DeviceDisplay );
        //    args.push_back( handle );
        //    args.push_back( resolution );
        //    args.push_back( fmt );
        //}

#endif

//----------------------------------------------------------------------------

//! common GhostScript options, used by creating PDFs
        void InterpretPrintOL::insertPdfOptions(
            cmnutil::CollArgs&         args,
            const VarString& rOutputFileName )
        {
            args.push_back(rOutputFileName);
        }

//----------------------------------------------------------------------------
        void InterpretPrintOL::checkInputFileExists ( const basar::VarString& inputFileName ) const
        {
            boost::filesystem::path p( inputFileName.c_str() );

            if ( false == boost::filesystem::exists( p ) )
            {
                boost::filesystem::absolute( p );

                basar::VarString msg("No ps-file found at <");
                msg.append( p.string() );
                msg.append( ">" );

                throw RuntimeException( ExceptInfo( "basar.cmnutil.InterpretPrintOL::checkInputFileExists",
                                                 msg,
                                                 __FILE__, __LINE__ ) );
            }
        }

//----------------------------------------------------------------------------
    }   // cmnutil
}   // basar

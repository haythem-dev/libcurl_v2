//----------------------------------------------------------------------------
/*! \file
 *  \brief  postscript interpreting class via ghostscript
 *  \author Michael Eichenlaub
 *  \date   26.04.2006
 */
//----------------------------------------------------------------------------

#include "../libbasarcmnutil_print.h"
#include "../libbasarcmnutil_i18nstring.h"

#include "psinterpreter.h"
#include <gdevdsp.h>

#include <boost/filesystem.hpp>

#include <inttypes.h>

#ifdef _AIX
#include "getlibpath.h"

static basar::ConstBuffer getSearchPath()
{
    static std::string searchPath;

    if ( true == searchPath.empty() )
    {
        searchPath = "-I ";
        searchPath.append( getLibPath() );
        searchPath.append( "../ghostscript/lib" );
    }

    return searchPath.c_str();
}

#endif

//----------------------------------------------------------------------------
namespace basar
{
    namespace cmnutil
    {

//------------------------------------------------------------------------------
//  init statics
//------------------------------------------------------------------------------
        static ConstString s_Dummy                  = "-ignored";
        static ConstString s_LookForLibs            = "-P-";

#ifndef _AIX
        static ConstString s_SearchPath             = "-I" "../ghostscript/lib";
#else
        static ConstString s_SearchPath             = getSearchPath();
#endif

        static ConstString s_SaferMode              = "-dNOSAFER";
        static ConstString s_BatchMode              = "-dBATCH";
        static ConstString s_PauseMode              = "-dNOPAUSE";
        static ConstString s_QuietMode              = "-dQUIET";
        static ConstString s_PromptMode             = "-dNOPROMPT";

        static ConstString s_InputControl           = "-";
        static ConstString s_InputFile              = "-f";
        static ConstString s_InputCommand           = "-c";

        static ConstString s_DeviceDisplay          = "-sDEVICE=display";
        static ConstString s_DevicePdf              = "-sDEVICE=pdfwrite";
        static ConstString s_DeviceOutFile          = "-sOutputFile=";

        static ConstString s_DisplayResolution      = "-dDisplayResolution=";
        static ConstString s_DisplayResolutionDummy = "-dDisplayResolution=2";
        static ConstString s_DisplayHandle          = "-sDisplayHandle=";
        static ConstString s_DisplayFormat          = "-dDisplayFormat=";

        static ConstString s_Interpolate            = "-dDOINTERPOLATE";
        static ConstString s_TextAlphaBits          = "-dTextAlphaBits=4";
        static ConstString s_GraphAlphaBits         = "-dGraphicsAlphaBits=4";
        static ConstString s_AlignToPixels          = "-dAlignToPixels=0";

//! init static variable for controlling single object
#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4592) // warning C4592: 'm_pWeakPsInterpreter': symbol will be dynamically initialized (implementation limitation)
#endif // _WIN32
        InterpretPrint::PsInterpretWeakPtr InterpretPrint::m_pWeakPsInterpreter;
#ifdef _WIN32
#pragma warning( pop )
#endif // _WIN32

        boost::mutex InterpretPrint::m_InterpreterGuard;

//------------------------------------------------------------------------------
//! \throw  RuntimeException
//! \return display format value for postscript interpreter
        Int32 InterpretPrint::getDisplayFormat ( const DisplayBppEnum bpp )
        {
            // format
            Int32 fmt = DISPLAY_ALPHA_NONE | DISPLAY_UNUSED_LAST  | DISPLAY_LITTLEENDIAN | DISPLAY_TOPFIRST;

            switch ( bpp )
            {
                case DISP_COLOR_32:
                    fmt |= DISPLAY_COLORS_RGB    | DISPLAY_DEPTH_8;
                    break;

                case DISP_COLOR_16:
                    fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_16 | DISPLAY_NATIVE_565;
                    break;

                case DISP_COLOR_15:
                    fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_16 | DISPLAY_NATIVE_555;
                    break;

                case DISP_COLOR_8:
                    fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_8;
                    break;

                case DISP_COLOR_4:
                    fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_4;
                    break;

                case DISP_MONO:
                    fmt |= DISPLAY_COLORS_NATIVE | DISPLAY_DEPTH_1;
                    break;

                default:
                {
                    VarString msg;
                    msg.format( "unknown display color depth %d", bpp );
                    throw RuntimeException( ExceptInfo( "basar.cmnutil.InterpretPrint.getDisplayFormat()",
                                                        msg,
                                                        __FILE__, __LINE__ ) );
                }
                break;
            }

            return fmt;
        }

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
        InterpretPrint::InterpretPrint ()
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            if ( m_pWeakPsInterpreter.expired() )
            {
                m_pPsInterpreter     = PsInterpretSharedPtr( new PsInterpreter );
                m_pWeakPsInterpreter = m_pPsInterpreter;
            }
            else
            {
                m_pPsInterpreter     = PsInterpretSharedPtr( m_pWeakPsInterpreter );
            }
        }

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
        InterpretPrint::~InterpretPrint ()
        {
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::print( IPrintDataProvider& rDataProvider      ,
                                    Int32                fromPage /* = -1 */,
                                    Int32                toPage   /* = -1 */ )
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            m_pPsInterpreter->setDataProvider( rDataProvider, fromPage, toPage );

            CollArgs args;
            insertCommonOptions( args );
            insertPrintOptions ( args );

            args.push_back( s_InputControl );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_PRINT );
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::print( const VarString& rInputFileName )
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            CollArgs args;
            insertCommonOptions( args );
            insertPrintOptions ( args );

            args.push_back( s_InputFile );
            args.push_back( rInputFileName );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_PRINT );
        }

#ifdef _WIN32
//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::display( IPrintDataProvider& rDataProvider      ,
                                      const IScreenPrint& rScreenPrint       ,
                                      Int32                fromPage /* = -1 */,
                                      Int32                toPage   /* = -1 */ )
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            m_pPsInterpreter->setDataProvider( rDataProvider, fromPage, toPage );

            CollArgs args;
            insertCommonOptions ( args );
            insertDisplayOptions( args, rScreenPrint );

            args.push_back( s_InputControl );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_DISPLAY );
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::display( const VarString&     rInputFileName,
                                      const IScreenPrint& rScreenPrint  )
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            CollArgs args;
            insertCommonOptions ( args );
            insertDisplayOptions( args, rScreenPrint );

            args.push_back( s_InputFile );
            args.push_back( rInputFileName );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_DISPLAY );
        }

#endif

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::savePdf( IPrintDataProvider& rDataProvider      ,
                                      const VarString&     rOutputFileName    ,
                                      Int32                fromPage /* = -1 */,
                                      Int32                toPage   /* = -1 */ )
        {

            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            m_pPsInterpreter->setDataProvider( rDataProvider, fromPage, toPage );

            CollArgs args;
            insertCommonOptions( args );
            insertPdfOptions   ( args, rOutputFileName );

            args.push_back( s_InputControl );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_PDF );
        }

//------------------------------------------------------------------------------
//! \throw RuntimeException when rInputFileName specifies non-existing file
//!
        void InterpretPrint::savePdf( const VarString& rInputFileName ,
                                      const VarString& rOutputFileName )
        {
            boost::mutex::scoped_lock lock( m_InterpreterGuard );

            checkInputFileExists( rInputFileName );

            CollArgs args;
            insertCommonOptions( args );
            insertPdfOptions   ( args, rOutputFileName );

            args.push_back( s_InputFile );
            args.push_back( rInputFileName );

            m_pPsInterpreter->execute( args, PsInterpreter::CALLER_PDF );
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setPrintBpp( PrintBppEnum eBpp )
        {
            m_PrintProperties.eBpp = eBpp;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setPrintDpi( Int32 dpi )
        {
            m_PrintProperties.dpi = dpi;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setPrintDuplex( bool duplex )
        {
            m_PrintProperties.duplex = duplex;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setPrintPaperSize( PaperSizeEnum size )
        {
            m_PrintProperties.papersize = size;
        }

//------------------------------------------------------------------------------
//! \throw     no-throw
//! \attention escape backslashes for network printer in a C string!<br>
//!            e.g.: "\\\\FU1S0001\\AE_Laser"
        void InterpretPrint::setPrintSelect( PrintSelectEnum   eSelect,
                                             const VarString& rPrinterName /* = "" */ )
        {
            m_PrintProperties.eSelect     = eSelect;
            m_PrintProperties.printerName = rPrinterName;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setDisplayBpp( DisplayBppEnum eBpp )
        {
            m_DisplayProperties.eBpp = eBpp;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setDisplayDpi( Int32 dpi )
        {
            m_DisplayProperties.dpi = dpi;
        }

//------------------------------------------------------------------------------
//! \throw no-throw
//!
        void InterpretPrint::setDisplayRender( bool render )
        {
            m_DisplayProperties.render = render;
        }

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return VarString containing printer settings
        const VarString InterpretPrint::getPrinterSettingPS () const
        {
            // see ghostscript docu: devices - MS Windows printers
            static ConstString DEVICE_SETTINGS   = "mark /NoCancel true "                               // set progress dialog
                                                   "/Duplex %s "                                   // set duplex printing
                                                   "%s"                                            // optional properties
                                                   "/UserSettings "
                                                   "<< /Paper %d "                              // set paper size (wingdi.h)
                                                   "/MaxResolution %d "                      // set maximum printer resolution
                                                   ">> "
                                                   "(mswinpr2) finddevice "                            // get printer device
                                                   "putdeviceprops "                                   // save device properties
                                                   "setdevice";                                        // set printer device

            // optional
            static ConstString BITS_PER_PIXEL    = "/BitsPerPixel %d ";                                 // set device depth to specified bits per pixel
            static ConstString STANDARD_PRINTER  = "/QueryUser 3 ";                                     // select default Windows printer without any user interaction
            static ConstString DEDICATED_PRINTER = "/OutputFile (%%printer%%%s) ";                      // specifies which printer should be used.
            // string should be of the form %printer%printer_name,
            // where the printer_name should be typed exactly as it appears
            // in Windows printer control panel, including spaces
            static ConstString DUPLEX_TRUE       = "true";                                              // duplex printing value for enabled
            static ConstString DUPLEX_FALSE      = "false";                                             // duplex printing value for disabled

            VarString options;

            switch ( m_PrintProperties.eSelect )
            {
                case PRINTER_STANDARD:
                    options += STANDARD_PRINTER;
                    break;

                case PRINTER_DEDICATED:
                    if ( m_PrintProperties.printerName.empty() )
                        throw InvalidParameterException( ExceptInfo( "basar.cmnutil.InterpretPrint::getPrinterSettingPS",
                                                         "/OutputFile: printername empty",
                                                         __FILE__, __LINE__ ) );

                    {
                        // workaround for Ghostscript's strange behaviour: gs eliminates backslash if not escaped
                        VarString name = m_PrintProperties.printerName;
                        name.exchange( '\\', "\\\\" );  // means : '\' -> '\\' (one replaced by two)

                        options.format( DEDICATED_PRINTER, name.c_str() );
                    }

                    break;

                case PRINTER_DIALOG:
                default:
                    break;
            }

            if ( PRINT_USER_SETTING != m_PrintProperties.eBpp )
            {
                VarString bpp;
                bpp.format( BITS_PER_PIXEL, m_PrintProperties.eBpp );

                options += bpp;
            }

            VarString code;
            code.format( DEVICE_SETTINGS,
                         m_PrintProperties.duplex ? DUPLEX_TRUE : DUPLEX_FALSE,
                         options.c_str(),
                         m_PrintProperties.papersize,
                         m_PrintProperties.dpi );

            return code;
        }

//----------------------------------------------------------------------------
//! common GhostScript options, used by all cases
        void InterpretPrint::insertCommonOptions( CollArgs& args )
        {
            args.push_back( s_Dummy );
            args.push_back( s_LookForLibs );
            args.push_back( s_SearchPath );
            args.push_back( s_SaferMode );
            args.push_back( s_BatchMode );
            args.push_back( s_PauseMode );
            args.push_back( s_QuietMode );
            args.push_back( s_PromptMode );
        }

//----------------------------------------------------------------------------
//! common GhostScript options, used by printing
        void InterpretPrint::insertPrintOptions( CollArgs& args )
        {
            VarString dispfmtdmy;
            dispfmtdmy.format( "%s%d", s_DisplayFormat, getDisplayFormat( DISP_MONO ) );

            VarString prnsetting;
            prnsetting.format( "%s"  , getPrinterSettingPS().c_str() );

            args.push_back( s_DisplayResolutionDummy );
            args.push_back( dispfmtdmy );
            args.push_back( s_InputCommand );
            args.push_back( prnsetting );
        }

#ifdef _WIN32

//----------------------------------------------------------------------------
//! common GhostScript options, used by display
        void InterpretPrint::insertDisplayOptions(
            CollArgs&            args,
            const IScreenPrint& rScreenPrint )
        {
            VarString handle;
            handle.format( "%s%" PRIuPTR, s_DisplayHandle, &rScreenPrint );

            VarString resolution;
            resolution.format( "%s%d", s_DisplayResolution, m_DisplayProperties.dpi );

            VarString fmt;
            fmt.format( "%s%d", s_DisplayFormat, getDisplayFormat( m_DisplayProperties.eBpp ) );

            if ( m_DisplayProperties.render )
            {
                args.push_back( s_Interpolate );
                args.push_back( s_TextAlphaBits );
                args.push_back( s_GraphAlphaBits );
                args.push_back( s_AlignToPixels );
            }

            args.push_back( s_DeviceDisplay );
            args.push_back( handle );
            args.push_back( resolution );
            args.push_back( fmt );
        }

#endif

//----------------------------------------------------------------------------
//! common GhostScript options, used by creating PDFs
        void InterpretPrint::insertPdfOptions(
            CollArgs&         args,
            const VarString& rOutputFileName )
        {
            VarString paramPdf;
            paramPdf.format( "%s%s", s_DeviceOutFile, rOutputFileName.c_str() );

            args.push_back( s_DevicePdf );
            args.push_back( paramPdf );
        }

//----------------------------------------------------------------------------
        void InterpretPrint::checkInputFileExists ( const basar::VarString& inputFileName ) const
        {
            boost::filesystem::path p( inputFileName.c_str() );

            if ( false == boost::filesystem::exists( p ) )
            {
                boost::filesystem::absolute( p );

                basar::VarString msg("No ps-file found at <");
                msg.append( p.string() );
                msg.append( ">" );

                throw RuntimeException( ExceptInfo( "basar.cmnutil.InterpretPrint::checkInputFileExists",
                                                 msg,
                                                 __FILE__, __LINE__ ) );
            }
        }

//----------------------------------------------------------------------------
    }   // cmnutil
}   // basar

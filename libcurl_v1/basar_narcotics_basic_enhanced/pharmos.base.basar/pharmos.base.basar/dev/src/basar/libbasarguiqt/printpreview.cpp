//----------------------------------------------------------------------------
/*! \file
 *  \brief  print preview implementation (qt)
 *  \author Thomas Hörath
 *  \date   17.01.2006
 */
//----------------------------------------------------------------------------

#include "libbasarguiqt_printpreview.h"
#include "picturelist.h"

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_socket.h"
#include "libbasarguitie_functions.h"
#include "libbasarguitie_msgboxes.h"


#pragma warning (push)
#pragma warning(disable: 4244 4481 4512 4800)
#include <QtGui/QIntValidator>

#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QCombobox>
#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#pragma warning (pop)

//----------------------------------------------------------------------------
using basar::cmnutil::InterpretPrint;

//----------------------------------------------------------------------------
namespace basar
{
    namespace gui
    {
        namespace qt
        {

//----------------------------------------------------------------------------
            const basar::Int32                      PrintPreview::s_Dpi          = 60;
            const InterpretPrint::DisplayBppEnum    PrintPreview::s_BitsPixel    = InterpretPrint::DISP_COLOR_4;    // -> 16 colors
            const basar::Float64                    PrintPreview::s_ZoomFactor[] =
            {
                0.25,
                0.50,
                0.75,
                1.00,
                1.25,
                1.50,
                2.00
            };

//----------------------------------------------------------------------------
            /*! \throw no-throw
             *  \return 0 (always)
            */
// new device has been opened; first event from this device
            basar::Int32 PrintPreview::onOpen()
            {
                // reset piclist because after each display-call to PrintPs (e.g. after zooming)
                // all pages all calculated new and set to piclist again
                m_pPicList->reset();
                m_PageNo = 1;

                BLOG_TRACE( LoggerPool::prnPrevCallb(), "onOpen() - m_PageNo = 1; m_pPicList resetted." );

                return 0;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw
             *  \return 0 (always)
            */
// device has been closed; last event from this device
            basar::Int32 PrintPreview::onClose()
            {
                BLOG_TRACE( LoggerPool::prnPrevCallb(), "onClose()" );
                return 0;   // nothing to do here! do not delete piclist here!
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw
             *  \return 0 (always)
            */
// device has been resized; new pointer to raster returned in pImage
            basar::Int32 PrintPreview::onSize( basar::Int32 width , basar::Int32 height,
                                               basar::Int32 raster, UInt8*       pImage )
            {
                // save geometric data
                m_Width  = width;
                m_Height = height;
                m_Raster = raster;
                m_pImage = pImage;

                if ( LoggerPool::prnPrevCallb().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "onSize() - device has been resized:\n"
                                "  width = %9d\n"
                                "  height= %9d\n"
                                "  raster= %9d\n",
                                m_Width,
                                m_Height,
                                m_Raster );
                    LoggerPool::prnPrevCallb().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                return 0;
            }

//----------------------------------------------------------------------------
            /*! \throw RuntimeException
             *  \return 0 (always)
            */
// show page; if you want to pause on showpage, then don't return immediately
            basar::Int32 PrintPreview::onPage()
            {
                if ( LoggerPool::prnPrevCallb().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "onPage():\n"
                                "  bitmap size = %9d\n"
                                "  raster      = %9d\n"
                                "  width       = %9d\n"
                                "  height      = %9d",
                                m_Raster * m_Height,
                                m_Raster,
                                m_Width,
                                m_Height );
                    LoggerPool::prnPrevCallb().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                //  save in bitmap in picture list
                if ( !m_pPicList->append( m_Width, m_Height, m_Raster, m_pImage ) )
                    throw RuntimeException( ExceptInfo( "basar.gui.qt.PrintPreview.onPage()",
                                                        "appending bitmap into picture list failed",
                                                        __FILE__, __LINE__ ) );

                return 0;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreview::PrintPreview( const basar::I18nString& rPsFile,
                                        QWidget*                 pParent,
                                        const basar::I18nString& rTitle,
                                        QPrinter::Orientation    orientation,
                                        OutputEnum               outputPossibilities,
                                        const basar::I18nString& rPdfPath    )

                : QDialog( pParent, Qt::Dialog ),
                  m_pPicList( new PictureList( s_BitsPixel ) ),
                  m_PageNo ( 1 ),
                  m_Orientation( orientation ),
                  m_OutputPossibilities( outputPossibilities ),
                  m_Width ( 0 ),
                  m_Height( 0 ),
                  m_Raster( 0 ),
                  m_pImage( 0 ),
                  m_PdfPath ( QString::fromLocal8Bit( rPdfPath.c_str() ) ),
                  m_WindowTitle( QString::fromLocal8Bit( rTitle.c_str() ) ),
                  m_PsFilename( rPsFile ),
                  m_pDataProv( 0 ),
                  m_IndexOfZoomCb( ZOOM_100 ),
                  m_PreviewDpi( s_Dpi ),
                  m_StartingPreviewZoom( ZOOM_INVALID )
            {
                init();
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreview::PrintPreview( basar::cmnutil::IPrintDataProvider& rDataProvider,
                                        QWidget*                             pParent,
                                        const basar::I18nString&             rTitle,
                                        QPrinter::Orientation                orientation,
                                        OutputEnum                           outputPossibilities,
                                        const basar::I18nString&             rPdfPath    )

                : QDialog( pParent, Qt::Dialog ),
                  m_pPicList( new PictureList( s_BitsPixel ) ),
                  m_PageNo ( 1 ),
                  m_Orientation( orientation ),
                  m_OutputPossibilities( outputPossibilities ),
                  m_Width ( 0 ),
                  m_Height( 0 ),
                  m_Raster( 0 ),
                  m_pImage( 0 ),
                  m_PdfPath ( QString::fromLocal8Bit( rPdfPath.c_str() ) ),
                  m_WindowTitle ( QString::fromLocal8Bit( rTitle.c_str() ) ),
                  m_PsFilename( "" ),
                  m_pDataProv( &rDataProvider ),
                  m_IndexOfZoomCb( ZOOM_100 ),
                  m_PreviewDpi( s_Dpi ),
                  m_StartingPreviewZoom( ZOOM_INVALID )
            {
                init();
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreview::PrintPreview()

                : QDialog( 0, Qt::Dialog ),
                  m_pPicList( new PictureList( s_BitsPixel ) ),
                  m_PageNo ( 1 ),
                  m_Orientation( QPrinter::Portrait ),
                  m_OutputPossibilities( static_cast<OutputEnum>( WIN_PRINT | PDF ) ),
                  m_Width ( 0 ),
                  m_Height( 0 ),
                  m_Raster( 0 ),
                  m_pImage( 0 ),
                  m_PdfPath ( "" ),
                  m_WindowTitle ( "" ),
                  m_PsFilename( "" ),
                  m_pDataProv( 0 ),
                  m_IndexOfZoomCb( ZOOM_100 ),
                  m_PreviewDpi( s_Dpi ),
                  m_StartingPreviewZoom( ZOOM_INVALID )
            {
                init();
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreview::~PrintPreview()
            {
                BLOG_TRACE( LoggerPool::prnPrevMethods(), "destruction of printpreview." );

                delete m_pPicList;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPrintData( basar::cmnutil::IPrintDataProvider&   rDataProvider )
            {
                BLOG_TRACE( LoggerPool::prnPrevMethods(), "print data set via cmnutil::IPrintDataProvider." );

                m_PsFilename = "";
                m_pDataProv  = &rDataProvider;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPrintData( const basar::I18nString& rPsFile )
            {
                BLOG_TRACE( LoggerPool::prnPrevMethods(), "print data set via path to ps-file." );

                m_pDataProv = 0;
                m_PsFilename = rPsFile;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPreviewParent ( QWidget* pParent )
            {
                BLOG_TRACE( LoggerPool::prnPrevMethods(), "preview parent set." );

                this->setParent( pParent );
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setTitle ( const basar::I18nString& rTitle )
            {
                m_WindowTitle  = QString::fromLocal8Bit( rTitle.c_str() );

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview window title set to <%s>.", m_WindowTitle.toLocal8Bit().constData() );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setOrientation   ( QPrinter::Orientation orientation  )
            {
                m_Orientation = orientation;

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview orientation set to <%d>.", m_Orientation );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setOutputFormats( OutputEnum outputPossibilities )
            {
                m_OutputPossibilities = outputPossibilities;

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview - possible output formats set to <%d>.", m_OutputPossibilities );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                fillOutputCb();
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPdfPath   ( const basar::I18nString& rPdfPath )
            {
                m_PdfPath = QString::fromLocal8Bit( rPdfPath.c_str() );

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview default pdf path set to <%s>.", m_PdfPath.toLocal8Bit().constData() );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setRawPrintInfo( const basar::VarString& rServer, const basar::VarString& rPrinter )
            {
                m_RwprServer  = rServer;
                m_RwprPrinter = rPrinter;

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview raw print info set: server=<%s>, printer=<%s>.",
                                m_RwprServer.c_str(), m_RwprPrinter.c_str() );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPrintBpp( basar::cmnutil::InterpretPrint::PrintBppEnum eBpp )
            {
                m_PrintProperties.eBpp = eBpp;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPrintDpi( Int32 dpi )
            {
                m_PrintProperties.dpi = dpi;
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::setPrintSelect( basar::cmnutil::InterpretPrint::PrintSelectEnum eSelect,
                                               const basar::VarString&                         rPrinterName /* = "" */ )
            {
                m_PrintProperties.eSelect     = eSelect;
                m_PrintProperties.printerName = rPrinterName;
            }

            void PrintPreview::setPreviewDpi( basar::Int32 previewDpi )
            {
                m_PreviewDpi = previewDpi;
            }

            void PrintPreview::setStartingPreviewZoomLevel( ZoomEnum startingPreviewZoomLevel )
            {
                m_StartingPreviewZoom = startingPreviewZoomLevel;
            }

//----------------------------------------------------------------------------
            /*! \throw RuntimeException
                \return qt's widget return value matched to basar */
            basar::gui::tie::WidgetReturnEnum PrintPreview::show    ()
            {
                if ( m_pDataProv == 0 && m_PsFilename.empty() )
                {
                    throw RuntimeException( ExceptInfo( "basar.gui.qt.PrintPreview.show()",
                                                        "no data-provider or ps-file set for previewing!",
                                                        __FILE__, __LINE__ ) );
                }


                QString title = tr( "Print Preview  " );
                title.append( m_WindowTitle );
                setWindowTitle( title );

                m_pCbOrientation ->setCurrentIndex( m_Orientation );

                showMaximized();        // maximize window: from now on scroll area geometry is known

                if ( ZOOM_INVALID == m_StartingPreviewZoom )
                {
                    computeZoom  ();        // compute zoom level for fitting bitmap in scroll area
                }
                else
                {
                    //honor user's request
                    m_IndexOfZoomCb = m_StartingPreviewZoom;
                }

                try
                {
                    callDisplay  ();        // call postscript interpreter - this might throw
                }
                catch ( const basar::Exception& ex )
                {
                    std::ostringstream os;

                    os << std::endl;
                    os << "=== basar exception caught ===========================================================" << std::endl;
                    os << std::endl;
                    os << ex.what();
                    os << std::endl;
                    os << __FILE__;
                    os << ": ";
                    os << __LINE__;

                    basar::gui::tie::warningMsgBox( this, os.str().c_str() );
                }
                catch ( const std::exception& s_ex )
                {
                    std::ostringstream os;

                    os << std::endl;
                    os << "=== std exception caught =============================================================" << std::endl;
                    os << std::endl;
                    os << s_ex.what();
                    os << std::endl;
                    os << __FILE__;
                    os << ": ";
                    os << __LINE__;

                    basar::gui::tie::warningMsgBox( this, os.str().c_str() );
                }
                catch ( ... )
                {
                    std::ostringstream os;

                    os << std::endl;
                    os << "=== unknown exception caught =========================================================" << std::endl;
                    os << std::endl;
                    os << __FILE__;
                    os << ": ";
                    os << __LINE__;

                    basar::gui::tie::warningMsgBox( this, os.str().c_str() );
                }

                refreshPage  ();        // refresh window

                QString pg;
                pg.setNum( getPageCount() );
                m_pEdTo->setText( pg );

                if ( getPageCount() > 0 )
                {
                    m_pActionPrint->setEnabled( true );
                }
                else
                {
                    m_pActionPrint->setEnabled( false );
                }

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview started, pagecount = <%d>.", getPageCount() );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                m_pPageValid->setRange( 1, getPageCount() );

                m_pCbZoom->setCurrentIndex( m_IndexOfZoomCb );

                return basar::gui::tie::getWidgetReturnType( exec() );
            }


//----------------------------------------------------------------------------
            void PrintPreview::computeZoom()
            {
                // new algorithm: assume DIN A4: width = 8.27", height = 11.69"
                //                and compute dpi before time consuming ps interpreter is executed

                static const Float64 WIDTHA4  =  8.27;
                static const Float64 HEIGHTA4 = 11.69;
                const Int32          width    = static_cast<Int32>( m_PreviewDpi * ( ( m_Orientation == QPrinter::Portrait ) ? WIDTHA4  : HEIGHTA4 ) );
                const Int32          height   = static_cast<Int32>( m_PreviewDpi * ( ( m_Orientation == QPrinter::Portrait ) ? HEIGHTA4 : WIDTHA4 ) );

                // maximize
                while ( ( m_pScrollArea->width () >  static_cast<Int32>( width  * s_ZoomFactor[m_IndexOfZoomCb] ) ) &&
                        ( m_pScrollArea->height() >  static_cast<Int32>( height * s_ZoomFactor[m_IndexOfZoomCb] ) )   )
                {
                    if ( ZOOM_LAST == m_IndexOfZoomCb )
                    {
                        break;
                    }

                    ++m_IndexOfZoomCb;
                }

                // minmize
                while ( ( m_pScrollArea->width () <  static_cast<Int32>( width  * s_ZoomFactor[m_IndexOfZoomCb] ) ) ||
                        ( m_pScrollArea->height() <  static_cast<Int32>( height * s_ZoomFactor[m_IndexOfZoomCb] ) )   )
                {
                    if ( ZOOM_FIRST == m_IndexOfZoomCb )
                    {
                        break;
                    }

                    --m_IndexOfZoomCb;
                }


                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "computeDpi():\n"
                                "    dpi         = %5d, orient = %s\n"
                                "    zoom level  = %5d, factor = %g\n"
                                "    pic: width  = %5d, height = %5d\n"
                                "    gui: width  = %5d, height = %5d",
                                m_PreviewDpi,
                                ( m_Orientation == QPrinter::Portrait ) ? "portrait" : "landscape",
                                m_IndexOfZoomCb, s_ZoomFactor[m_IndexOfZoomCb],
                                width, height,
                                m_pScrollArea->width(), m_pScrollArea->height() );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::init()
            {
                assert( sizeof( s_ZoomFactor ) / sizeof( s_ZoomFactor[0] ) == ZOOM_LAST + 1 );

                m_pMenuBar  = new QMenuBar();
                m_pFileMenu = new QMenu( tr( "&File" ),       this );
                m_pNavigMenu = new QMenu( tr( "&Navigation" ), this );

                m_pToolbar  = new QToolBar( tr( "Toolbar" ), this );

// print
                m_pActionPrint = new QAction( QIcon( ":/image/print.png" ), tr( "P&rint..." ), this );
                m_pActionPrint->setToolTip( tr( "print file" ) );
                m_pActionPrint->setShortcut( Qt::Key_Enter );
                connect( m_pActionPrint, SIGNAL( triggered() ), this, SLOT( slotPrint() ) );

                m_pToolbar->addAction( m_pActionPrint );
                m_pFileMenu->addAction( m_pActionPrint );

// orientation combo
                m_pCbOrientation = new QComboBox( m_pToolbar );
                m_pCbOrientation->setEditable( false );
                m_pCbOrientation->addItem( tr( "Portrait" ) );
                m_pCbOrientation->addItem( tr( "Landscape" ) );
                m_pCbOrientation->setFocusPolicy( Qt::NoFocus );
                connect( m_pCbOrientation, SIGNAL( activated( const QString& ) ), SLOT( slotOrientation( const QString& ) ) );
                m_pToolbar->addWidget( m_pCbOrientation );

// output format combo
                m_pCbOutputFormat = new QComboBox( m_pToolbar );
                m_pCbOutputFormat->setEditable( false );

                fillOutputCb();

                m_pCbOutputFormat->setFocusPolicy( Qt::NoFocus );
                connect( m_pCbOutputFormat, SIGNAL( activated( const QString& ) ), SLOT( slotOutputFormat( const QString& ) ) );
                m_pToolbar->addWidget( m_pCbOutputFormat );
                m_pToolbar->addSeparator();

// from - to
                m_pLabelFrom = new QLabel( tr( "  from: " ), m_pToolbar );
                m_pToolbar->addWidget( m_pLabelFrom );

                m_pPageValid = new QIntValidator( 1, 1, this );

                m_pEdFrom = new QLineEdit( "1", m_pToolbar );
                m_pEdFrom->setMaximumWidth( 40 );
                m_pEdFrom->setValidator( m_pPageValid );
                m_pToolbar->addWidget( m_pEdFrom );

                m_pLabelTo = new QLabel( tr( "  to: " ), m_pToolbar );
                m_pToolbar->addWidget( m_pLabelTo );

                m_pEdTo = new QLineEdit( "1", m_pToolbar );
                m_pEdTo->setMaximumWidth( 40 );
                m_pEdTo->setValidator( m_pPageValid );
                m_pToolbar->addWidget( m_pEdTo );

                m_pToolbar->addSeparator();
                m_pFileMenu->addSeparator();

//zoom +
                QAction* pActionZP = new QAction( QIcon( ":/image/zoom_plus.png" ), tr( "&Zoom..." ), this );
                pActionZP->setToolTip( tr( "increase page" ) );
                pActionZP->setShortcut( Qt::Key_Plus );
                connect( pActionZP, SIGNAL( triggered() ), this, SLOT( zoomPlus() ) );

                m_pToolbar->addAction( pActionZP );
                m_pFileMenu->addAction( pActionZP );

//zoom -
                QAction* pActionZM = new QAction( QIcon( ":/image/zoom_minus.png" ), tr( "&Unzoom..." ), this );
                pActionZM->setToolTip( tr( "minimize page" ) );
                pActionZM->setShortcut( Qt::Key_Minus );
                connect( pActionZM, SIGNAL( triggered() ), this, SLOT( zoomMinus() ) );

                m_pToolbar->addAction( pActionZM );
                m_pFileMenu->addAction( pActionZM );

// zoom combo
                m_pCbZoom =  new QComboBox( m_pToolbar );
                m_pCbZoom->setEditable( false );
                m_pCbZoom->addItem( tr( " 25 %" ) );
                m_pCbZoom->addItem( tr( " 50 %" ) );
                m_pCbZoom->addItem( tr( " 75 %" ) );
                m_pCbZoom->addItem( tr( "100 %" ) );
                m_pCbZoom->addItem( tr( "125 %" ) );
                m_pCbZoom->addItem( tr( "150 %" ) );
                m_pCbZoom->addItem( tr( "200 %" ) );
                m_pCbZoom->setFocusPolicy( Qt::NoFocus );
                m_pCbZoom->setCurrentIndex( m_IndexOfZoomCb );
                connect( m_pCbZoom, SIGNAL( activated( const QString& ) ), SLOT( slotZoom( const QString& ) ) );
                m_pToolbar->addWidget( m_pCbZoom );

                m_pToolbar->addSeparator();
                m_pFileMenu->addSeparator();

//first page
                m_pActionFrst = new QAction( QIcon( ":/image/firstpage.png" ), tr( "&first page" ), this );
                m_pActionFrst->setToolTip( tr( "go to first page" ) );
                m_pActionFrst->setShortcut( Qt::CTRL + Qt::Key_Home );
                connect( m_pActionFrst, SIGNAL( triggered() ), this, SLOT( slotFirst() ) );

                m_pToolbar->addAction( m_pActionFrst );
                m_pNavigMenu->addAction( m_pActionFrst );

// prev page
                m_pActionPrev = new QAction( QIcon( ":/image/prevpage.png" ), tr( "&previous page" ), this );
                m_pActionPrev->setToolTip( tr( "go to previous page" ) );
                m_pActionPrev->setShortcut( Qt::CTRL + Qt::Key_Left );
                connect( m_pActionPrev, SIGNAL( triggered() ), this, SLOT( slotPrev() ) );

                m_pToolbar->addAction( m_pActionPrev );
                m_pNavigMenu->addAction( m_pActionPrev );

//next page
                m_pActionNext = new QAction( QIcon( ":/image/nextpage.png" ), tr( "&next page" ), this );
                m_pActionNext->setToolTip( tr( "go to next page" ) );
                m_pActionNext->setShortcut( Qt::CTRL + Qt::Key_Right );
                connect( m_pActionNext, SIGNAL( triggered() ), this, SLOT( slotNext() ) );

                m_pToolbar->addAction( m_pActionNext );
                m_pNavigMenu->addAction( m_pActionNext );

//last page
                m_pActionLast = new QAction( QIcon( ":/image/lastpage.png" ), tr( "&last page" ), this );
                m_pActionLast->setToolTip( tr( "go to last page" ) );
                m_pActionLast->setShortcut( Qt::CTRL + Qt::Key_End );
                connect( m_pActionLast, SIGNAL( triggered() ), this, SLOT( slotLast() ) );

                m_pToolbar->addAction( m_pActionLast );
                m_pNavigMenu->addAction( m_pActionLast );
                m_pNavigMenu->addSeparator();

// go to page
                QAction* pActionGo = new QAction( QIcon( ":/image/gotopage.png" ), tr( "&go to page..." ), this );
                pActionGo->setToolTip( tr( "go to specified page" ) );
                pActionGo->setShortcut( Qt::CTRL + Qt::Key_G );
                connect( pActionGo, SIGNAL( triggered() ), this, SLOT( slotGoTo() ) );

                m_pToolbar->addAction( pActionGo );
                m_pNavigMenu->addAction( pActionGo );
                m_pToolbar->addSeparator();

// exit
                QAction* pActionExit = new QAction( QIcon( ":/image/exit.png" ), tr( "&Exit" ), this );
                pActionExit->setToolTip( tr( "exit print preview" ) );
                pActionExit->setShortcut( Qt::Key_Escape );
                connect( pActionExit, SIGNAL( triggered() ), this, SLOT( close() ) );

                m_pToolbar->addAction( pActionExit );
                m_pFileMenu->addAction( pActionExit );
                m_pToolbar->addSeparator();

// status line
                m_pStatusLine = new QLabel( m_pToolbar );
                m_pToolbar->addWidget( m_pStatusLine );

// help edit for gui layout - not visible there
                QLineEdit* pEdHelp = new QLineEdit( m_pToolbar );
                pEdHelp->setEnabled( false );
                pEdHelp->setFrame( false );
                m_pToolbar->addWidget( pEdHelp );

// -----
                m_pMenuBar->addMenu( m_pFileMenu );
                m_pMenuBar->addMenu( m_pNavigMenu );

// ----
                m_pPreviewPic = new QLabel;
                m_pPreviewPic->setBackgroundRole( QPalette::Base );
                m_pPreviewPic->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
                m_pPreviewPic->setScaledContents( true );

// ----
                m_pScrollArea = new QScrollArea( this );
                m_pScrollArea->setBackgroundRole( QPalette::Dark );
                m_pScrollArea->setWidget( m_pPreviewPic );
                m_pScrollArea->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Ignored ) );

// ----
                m_pMainLayout = new QVBoxLayout();
                m_pMainLayout->setMenuBar( m_pMenuBar );
                m_pMainLayout->addWidget ( m_pToolbar,      0 );
                m_pMainLayout->addWidget ( m_pScrollArea,   1 );
                setLayout( m_pMainLayout );

// ----
                setWindowModality( Qt::ApplicationModal );

// ----

                //get available screen size

                QDesktopWidget* desktopWidget = QApplication::desktop();

                QRect screenRect = desktopWidget->availableGeometry( this );

                //set our geometry to screen size (dialog is shown maximized anyway)
                setGeometry( screenRect );

                BLOG_TRACE( LoggerPool::prnPrevMethods(), "print preview initialized!" );
            }

//----------------------------------------------------------------------------
            void PrintPreview::fillOutputCb()
            {
                m_pCbOutputFormat->clear();

                if ( m_OutputPossibilities == PREVIEW_ONLY )
                {
                    m_pCbOutputFormat->addItem( tr( "Preview only" ) );
                }
                else if ( m_OutputPossibilities == PDF )
                {
                    m_pCbOutputFormat->addItem( tr( "PDF-Generation" ) );
                }
                else if ( m_OutputPossibilities == WIN_PRINT )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (win)" ) );
                }
                else if ( m_OutputPossibilities == PS_PRINT )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (ps)" ) );
                }
                else if ( m_OutputPossibilities == ( PS_PRINT | PDF ) )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (ps)" ) );
                    m_pCbOutputFormat->addItem( tr( "PDF-Generation" ) );
                }
                else if ( m_OutputPossibilities == ( WIN_PRINT | PDF ) )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (win)" ) );
                    m_pCbOutputFormat->addItem( tr( "PDF-Generation" ) );
                }
                else if ( m_OutputPossibilities == ( WIN_PRINT | PDF | PS_PRINT ) )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (win)" ) );
                    m_pCbOutputFormat->addItem( tr( "Print (ps)" ) );
                    m_pCbOutputFormat->addItem( tr( "PDF-Generation" ) );
                }
                else if ( m_OutputPossibilities == ( WIN_PRINT | PS_PRINT ) )
                {
                    m_pCbOutputFormat->addItem( tr( "Print (win)" ) );
                    m_pCbOutputFormat->addItem( tr( "Print (ps)" ) );
                }

                m_pCbOutputFormat->setCurrentIndex( 0 );
                slotOutputFormat( "" );
            }

//----------------------------------------------------------------------------
            /* old way to get display caps; from now on bits per pixel is set to 4
            void PrintPreview::getDisplayCaps()
            {
                static ConstString func = "basar.gui.qt.PrintPreview.getDisplayCaps()";

                HDC screen = ::GetDC(0);

                if (!screen)
                    throw RuntimeException( ExceptInfo( func, "failure at getting DC", __FILE__, __LINE__ ) );

                m_Caps.bitspixel = ::GetDeviceCaps(screen, BITSPIXEL);
                m_Caps.planes    = ::GetDeviceCaps(screen, PLANES);
                m_Caps.dpi       = ::GetDeviceCaps(screen, LOGPIXELSY);

                m_Caps.depth     = m_Caps.bitspixel * m_Caps.planes;

                if (16 == m_Caps.bitspixel) // 64k colors detected -> check if there are really 32k
                {
                    ::HDC       mem = 0;
                    ::HBITMAP   bmpNew = 0;
                    ::HBITMAP   bmpOld = 0;
                    ::COLORREF  outColor = 0;

                    const ::COLORREF inColor  = 0x00202020; // 32 bit values -> overflow

                    static ConstString funcDC      = "creating compatible DC out of screen failed";
                    static ConstString funcBM      = "creating compatible bitmap out of screen failed";
                    static ConstString funcBMold   = "selecting old bitmap failed";
                    static ConstString funcCol     = "invalid color";
                    static ConstString funcDelBMold= "deleting old bitmap failed";
                    static ConstString funcDelBM   = "deleting new bitmap failed";
                    static ConstString funcDelDC   = "deleting DC failed";

                    const char * pFunc;

                    if ( 0 != (pFunc = funcDC)      &&  0           == (mem     = ::CreateCompatibleDC      (screen))       ||
                         0 != (pFunc = funcBM)      &&  0           == (bmpNew  = ::CreateCompatibleBitmap  (screen, 1, 1)) ||
                         0 != (pFunc = funcBMold)   &&  0           == (bmpOld  = (::HBITMAP)::SelectObject(mem, bmpNew))   ||
                         0 != (pFunc = funcCol)     &&  CLR_INVALID == (outColor = ::SetPixel(mem, 0, 0, inColor))          ||
                         0 != (pFunc = funcDelBMold)&&  0           == ::DeleteObject(bmpOld)                               ||
                         0 != (pFunc = funcDelBM)   &&  0           == ::DeleteObject(bmpNew)                               ||
                         0 != (pFunc = funcDelDC)   &&  0           == ::DeleteDC(mem)                                      )
                    {
                        throw RuntimeException( ExceptInfo( func, pFunc, __FILE__, __LINE__ ) );
                    }

                    const ::COLORREF cmpColor = 0x00212121; // result for 15 bits per pixel

                    if (cmpColor == outColor)
                        m_Caps.depth = 15 * m_Caps.planes;
                }

                if ( 0 == ::ReleaseDC(0, screen))
                    throw RuntimeException( ExceptInfo( func, "releasing DC failed", __FILE__, __LINE__ ) );

                if (LoggerPool::prnPrevMethods().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {
                    VarString msg;
                    msg.format("getDisplayCaps():\n"
                               "  bpp    = %5d\n"
                               "  planes = %5d\n"
                               "  depth  = %5d\n"
                               "  dpi    = %5d",
                               m_Caps.bitspixel,
                               m_Caps.planes,
                               m_Caps.depth,
                               m_Caps.dpi);
                    LoggerPool::prnPrevMethods().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }
            */

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::callDisplay()
            {
                InterpretPrint  print;

                print.setDisplayBpp   ( s_BitsPixel );
                print.setDisplayDpi   ( m_PreviewDpi );
                print.setDisplayRender( false );

                if ( m_PsFilename.empty() )
                {
                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "display called via DataProvider." );

                    print.display(  *m_pDataProv,   // data provider containing print data
                                    *this       );  // ref to preview-obj
                }
                else
                {
                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "display called via ps-file." );

                    print.display(  m_PsFilename,   // file containing ps-data
                                    *this       );  // ref to preview-obj
                }

                BLOG_TRACE( LoggerPool::prnPrevMethods(), "display call successful." );
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotZoom( const QString& )
            {
                m_IndexOfZoomCb = m_pCbZoom->currentIndex( );
                refreshPage( );
                m_pScrollArea->setFocus();

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "zoom set to index <%d>", m_IndexOfZoomCb );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotOrientation( const QString& )
            {
                if ( m_Orientation != m_pCbOrientation->currentIndex() )
                {
                    m_Orientation = static_cast<QPrinter::Orientation>( m_pCbOrientation->currentIndex() );
                    refreshPage();

                    if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "orientation set to index <%d>", m_Orientation );
                        LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotOutputFormat( const QString& )
            {
                if ( m_pCbOutputFormat->count() == 1 )
                {
                    m_OutputSelected = m_OutputPossibilities;
                }
                else if ( m_pCbOutputFormat->count() == 2 )
                {
                    if ( m_pCbOutputFormat->currentIndex() == 0 )
                    {
                        if ( m_OutputPossibilities == ( PDF | PS_PRINT ) )
                        {
                            m_OutputSelected = PS_PRINT;
                        }
                        else
                        {
                            m_OutputSelected = WIN_PRINT;
                        }
                    }
                    else
                    {
                        if ( m_OutputPossibilities == ( WIN_PRINT | PS_PRINT ) )
                        {
                            m_OutputSelected = PS_PRINT;
                        }
                        else
                        {
                            m_OutputSelected = PDF;
                        }
                    }
                }
                else
                {
                    if ( m_pCbOutputFormat->currentIndex() == 0 )
                    {
                        m_OutputSelected = WIN_PRINT;
                    }
                    else if ( m_pCbOutputFormat->currentIndex() == 1 )
                    {
                        m_OutputSelected = PS_PRINT;
                    }
                    else
                    {
                        m_OutputSelected = PDF;
                    }
                }

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "output format set to index <%d>", m_OutputSelected );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotPrint()
            {
                if ( !m_pActionPrint->isEnabled() )
                {
                    return;
                }

                m_pActionPrint->setEnabled( false );

                Int32 fromPage  = m_pEdFrom->text().isEmpty() ?              1 : m_pEdFrom->text().toInt();
                Int32 toPage    = m_pEdTo  ->text().isEmpty() ? getPageCount() : m_pEdTo  ->text().toInt();

                if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "printing started from preview; fromPage<%d> - toPage<%d>", fromPage, toPage );
                    LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }

                if ( m_OutputSelected == PDF )
                {
                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "trying to generate pdf-file." );

                    InterpretPrint  print;

                    if ( m_PdfPath.isEmpty() )
                    {
                        static ConstString ENV_HOME = "HOMEDRIVE";
                        m_PdfPath = QString::fromLocal8Bit( getenv( ENV_HOME ) );
                    }

                    m_PdfPath = QFileDialog::getSaveFileName(
                                    this,
                                    tr( "set file for pdf-output" ),
                                    m_PdfPath,
                                    "Adobe PDF (*.pdf)" );

                    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

                    if ( !m_PdfPath.contains( ".pdf" ) )
                    {
                        m_PdfPath.append( ".pdf" );
                    }

                    if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "path for pdf-generation: <%s>", m_PdfPath.toLocal8Bit().constData() );
                        LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }

                    if ( m_PsFilename.empty() )
                    {
                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "pdf-generation via DataProvider." );
                        print.savePdf( *m_pDataProv, m_PdfPath.toLocal8Bit().constData(), fromPage, toPage );
                    }
                    else
                    {
                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "pdf-generation via ps-file." );
                        print.savePdf( m_PsFilename, m_PdfPath.toLocal8Bit().constData() );
                    }

                    QApplication::restoreOverrideCursor();

                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "pdf-generation from preview successful." );
                }
                else if ( m_OutputSelected == WIN_PRINT )
                {
                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "trying to do win-printing from preview." );

                    InterpretPrint  print;

                    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

                    print.setPrintBpp   ( m_PrintProperties.eBpp );
                    print.setPrintDpi   ( m_PrintProperties.dpi );
                    print.setPrintSelect( m_PrintProperties.eSelect, m_PrintProperties.printerName );


                    if ( m_PsFilename.empty() )
                    {
                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "win-printing via DataProvider." );
                        print.print( *m_pDataProv, fromPage, toPage );
                    }
                    else
                    {
                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "win-printing via ps-file." );
                        print.print( m_PsFilename );
                    }

                    QApplication::restoreOverrideCursor();

                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "win-printing from preview successful." );
                }
                else if ( m_OutputSelected == PS_PRINT )
                {
                    try
                    {
                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "trying to do ps-printing from preview." );

                        basar::cmnutil::RawPrint rwpr;

                        QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

                        if ( m_PsFilename.empty() )
                        {
                            basar::cmnutil::PrintRetCodeEnum ret;

                            if ( m_RwprPrinter.empty() )
                            {
                                BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via DataProvider over Port 9100." );
                                ret = rwpr.print( m_RwprServer, *m_pDataProv, fromPage, toPage );
                            }
                            else
                            {
                                BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via DataProvider over lpr." );
                                ret = rwpr.print( m_RwprServer, m_RwprPrinter, *m_pDataProv, fromPage, toPage );
                            }

                            if ( ret == basar::cmnutil::PRINTER_OFFLINE )
                            {
                                BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via DataProvider -> printer offline!." );
                                basar::gui::tie::warningMsgBox( this, tr( "Your printer is offline / down!\nPlease put it on and try printing again." ).toLocal8Bit().constData() );
                            }
                            else if ( ret == basar::cmnutil::PRINTER_CONNREFUSED )
                            {
                                BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via DataProvider -> printer refused connecting attempt!." );
                                basar::gui::tie::warningMsgBox( this, tr( "Your printer rejected connecting attempts!\nMaybe too many clients tried to connect.\nPlease try printing again later." ).toLocal8Bit().constData() );
                            }
                            else  //--> ret == PRINTER_SUCCESS (nothing to do here)
                            {
                                BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via DataProvider -> NO printer error!." );
                            }
                        }
                        else
                        {
                            BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing via ps-file over lpr." );
                            rwpr.print( m_RwprServer, m_RwprPrinter, m_PsFilename );
                        }

                        QApplication::restoreOverrideCursor();

                        BLOG_TRACE( LoggerPool::prnPrevMethods(), "ps-printing from preview successful." );
                    }
                    catch ( basar::cmnutil::TcpSocketException& )
                    {
                        basar::gui::tie::criticalMsgBox( this, tr( "Error at printing!\nNo output was generated!\nPlease inform your data processing team for inspecting the log-file!\n(error with socket handling)" ).toLocal8Bit().constData() );
                    }
                    catch ( basar::cmnutil::LPDException& )
                    {
                        basar::gui::tie::criticalMsgBox( this, tr( "Error at printing!\nNo output was generated!\nPlease inform your data processing team for inspecting the log-file!\n(error with LPD protocol)" ).toLocal8Bit().constData() );
                    }
                }
                else
                {
                    basar::gui::tie::warningMsgBox( this, tr( "No printing or pdf-generating allowed!" ).toLocal8Bit().constData() );
                    BLOG_TRACE( LoggerPool::prnPrevMethods(), "no possibility for printing allowed." );
                }

                m_pActionPrint->setEnabled( true );
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotNext()
            {
                if ( m_PageNo < getPageCount() )
                {
                    ++m_PageNo;

                    refreshPage();
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotPrev()
            {
                if ( m_PageNo > 1 )
                {
                    --m_PageNo;

                    refreshPage();
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotFirst()
            {
                if ( m_PageNo > 1 )
                {
                    while ( m_PageNo != 1 )
                    {
                        --m_PageNo;
                    }

                    refreshPage();
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotLast()
            {
                if ( m_PageNo < getPageCount() )
                {
                    while ( m_PageNo != getPageCount() )
                    {
                        ++m_PageNo;
                    }

                    refreshPage();
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::slotGoTo()
            {
                bool         ok     = false;
                basar::Int32 pageNo = QInputDialog::getInt( this, tr( "go to page ..." ), tr( "page:" ), 1, 1, getPageCount(), 1, &ok );

                if ( ok )
                {
                    m_PageNo = pageNo;
                    refreshPage();

                    if ( LoggerPool::prnPrevMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "user directly selected page: <%d>", m_PageNo );
                        LoggerPool::prnPrevMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::refreshPage()
            {
                if ( 0 == getPageCount() )
                {
                    return;
                }

                QPixmap pm = m_pPicList->getPic( m_PageNo - 1, s_ZoomFactor[m_IndexOfZoomCb] );

                if ( m_Orientation == QPrinter::Portrait )
                {
                    m_pPreviewPic->setPixmap( pm );
                }
                else
                {
                    assert( QPrinter::Landscape == m_Orientation );

                    QMatrix matrix;
                    m_pPreviewPic->setPixmap( pm.transformed( matrix.rotate( 90.0 ) ) );
                }

                m_pPreviewPic->adjustSize();

                m_pStatusLine->setText( tr( "   page: %1 / %2" ).arg( m_PageNo ).arg( getPageCount() ) );

                m_pActionLast->setEnabled( true );
                m_pActionNext->setEnabled( true );
                m_pActionFrst->setEnabled( true );
                m_pActionPrev->setEnabled( true );

                if ( m_PageNo == getPageCount() )
                {
                    m_pActionLast->setEnabled( false );
                    m_pActionNext->setEnabled( false );
                }

                if ( m_PageNo == 1 )
                {
                    m_pActionFrst->setEnabled( false );
                    m_pActionPrev->setEnabled( false );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::zoomPlus()
            {
                if ( ZOOM_LAST == m_IndexOfZoomCb )
                {
                    basar::gui::tie::warningMsgBox( this, tr( "You already reached the maximum zoom level!" ).toLocal8Bit().constData() );
                    return;
                }

                m_pCbZoom->setCurrentIndex( ++m_IndexOfZoomCb );

                refreshPage( );
            }
//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreview::zoomMinus()
            {
                if ( ZOOM_FIRST == m_IndexOfZoomCb )
                {
                    basar::gui::tie::warningMsgBox( this, tr( "You already reached the minimum zoom level!" ).toLocal8Bit().constData() );
                    return;
                }

                m_pCbZoom->setCurrentIndex( --m_IndexOfZoomCb );

                refreshPage( );
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw
                \return number of pages */
            basar::Int32 PrintPreview::getPageCount () const
            {
                return m_pPicList->count();
            }

//----------------------------------------------------------------------------
        }   // namespace qt
    }   // namespace gui
}   // namespace bassar

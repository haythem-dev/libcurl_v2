//----------------------------------------------------------------------------
/*! \file
 *  \brief  print preview implementation (qt)
 *  \author Thomas Hörath
 *  \date   17.01.2006
 */
//----------------------------------------------------------------------------
#include <boost/filesystem.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 

#include "libbasar_printingol.h"
#include "libbasarguiqt_printpreviewol.h"
#include "picturelist.h"

#include "loggerpool.h"
#include "libbasarlogin_manager.h"
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
#include <ActiveQt/QAxWidget>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/QGuiApplication>

#pragma warning (pop)

//#define CLSID_InternetExplorer "{8856F961-340A-11D0-A96B-00C04FD705A2}"

namespace fs = boost::filesystem;

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
            /*! \throw no-throw */
            PrintPreviewOL::PrintPreviewOL( const basar::I18nString& rPsFile,
                                        QWidget*                 pParent,
                                        const basar::I18nString& rTitle,
                                        QPrinter::Orientation    orientation,
                                        OutputEnum               outputPossibilities,
                                        const basar::I18nString& rPdfPath    )

                : PrintPreview(rPsFile, pParent, rTitle, orientation, outputPossibilities, rPdfPath) 
            {
                InitRawPrint(pParent);
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreviewOL::PrintPreviewOL( basar::cmnutil::IPrintDataProvider& rDataProvider,
                                        QWidget*                             pParent,
                                        const basar::I18nString&             rTitle,
                                        QPrinter::Orientation                orientation,
                                        OutputEnum                           outputPossibilities,
                                        const basar::I18nString&             rPdfPath    )

                : PrintPreview(rDataProvider,
                    pParent,
                    rTitle,
                    orientation,
                    outputPossibilities,
                    rPdfPath)
            {
                InitRawPrint(pParent);
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreviewOL::PrintPreviewOL() :
                    PrintPreview()

            {
                InitRawPrint(nullptr);
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            PrintPreviewOL::~PrintPreviewOL()
            {
                BLOG_TRACE( LoggerPool::prnPrevOLMethods(), "destruction of printpreviewnew." );
            }

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreviewOL::setPdfPath   ( const basar::I18nString& rPdfPath )
            {
                m_PdfPath = QString::fromLocal8Bit( rPdfPath.c_str() );

                if ( LoggerPool::prnPrevOLMethods().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview default pdf path set to <%s>.", m_PdfPath.toLocal8Bit().constData() );
                    LoggerPool::prnPrevOLMethods().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            void PrintPreviewOL::setRawPrintInfo( const basar::VarString& rServer, const basar::VarString& rPrinter )
            {
                m_RwprServer  = rServer;
                m_RwprPrinter = rPrinter;

                if ( LoggerPool::prnPrevOLMethods().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                {
                    VarString msg;
                    msg.format( "preview raw print info set: server=<%s>, printer=<%s>.",
                                m_RwprServer.c_str(), m_RwprPrinter.c_str() );
                    LoggerPool::prnPrevOLMethods().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                }
            }

            void PrintPreviewOL::InitRawPrint(QWidget* rParent)
            {
                _rParent = rParent;
                _rawPrint = new cmnutil::RawPrint();

                setRawPrintFromConfig("OL_PRINT_SERVER", "OL_PRINT_QUEUE");
            }


            QMainWindow* PrintPreviewOL::getMainWindow()
            {
                foreach(QWidget * w, qApp->topLevelWidgets())
                    if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
                        return mainWin;
                return nullptr;
            }

//----------------------------------------------------------------------------
            /*! 
            * setRawPrintInfo must be called before Show() to set server/printer
            * setPdfPath mustz be called before show() to set path for the generating pdf preview
            */
            basar::gui::tie::WidgetReturnEnum PrintPreviewOL::show    ()
            {
                if ( m_pDataProv == 0 && m_PsFilename.empty() )
                {
                    throw RuntimeException( ExceptInfo( "basar.gui.qt.PrintPreview.show()",
                                                        "no data-provider or ps-file set for previewing!",
                                                        __FILE__, __LINE__ ) );
                }

                // send data to OL
                _rawPrint->print(m_RwprServer, m_RwprPrinter, *m_pDataProv);

                // get created pdf from the share
                // and display it

                basar::I18nString sharedDir;
                basar::printing::PrintUtils::getParaValue("OL_PRINT_SHARED_DIR", sharedDir);
                basar::I18nString fName = m_pDataProv->getFormFileName(); // getFormFileName();
                fs::path fPathName(fName.c_str());
                fPathName.replace_extension("pdf");

                QString fileName = QString(sharedDir.c_str()) + "\\" + QString(fPathName.string().c_str());
                showPdf(fileName);
                //return basar::gui::tie::getWidgetReturnType(exec());
                return basar::gui::tie::getWidgetReturnType(0);
            }

            void PrintPreviewOL::showPdf(const QString& fileName)
            {
                QAxWidget* pdf;
                pdf = new QAxWidget(_rParent);
                QString CLSID_InternetExplorer = QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}");
                if (!pdf->setControl(CLSID_InternetExplorer) )
                {
                    QMessageBox::critical(this, "Error", "Make sure you have Internet Explorer (and its ActiveX) installed!");
                }

                // resize wiget
                
                QWidget* pTopLevelWidget = window();
                /* Make no normalizeGeometry in case we are in manual resize mode or main window is maximized: */
                if (pTopLevelWidget->isMaximized())
                    return;

                /* Calculate client window offsets: */
                QRect frameGeo = pTopLevelWidget->frameGeometry();
                QRect geo = pTopLevelWidget->geometry();

                /* Get the best size w/o scroll bars: */
                QSize s = pTopLevelWidget->sizeHint();

                /* Resize the frame to fit the contents: */
                s -= pTopLevelWidget->size();
                frameGeo.setRight((int)(frameGeo.right()*0.8));
                frameGeo.setBottom((int)(frameGeo.bottom() * 0.8));

                frameGeo.moveCenter(QPoint(geo.right()/2, geo.bottom()/2));
                pdf->setGeometry(frameGeo);
                pdf->setWindowModality(Qt::ApplicationModal);
                //pdf->setWindowState(Qt::WindowFullScreen);
                if (!fileName.isEmpty())
                {
                    // wait until file is "completed"
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
                            msg.format("preview default tiemout could not be converted to int <%s>.", olPreviewTimeout.c_str());
                            LoggerPool::prnPrevOLMethods().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
                            previewTimeoutMs = 30 * 1000; // 30 ms default timeout)
                        }
                    }

                    const int defCheckFileDelay = 200; //ms
                    int max_repeat = previewTimeoutMs / defCheckFileDelay;
                    int counter = 0;
                    while (!fs::exists(fileName.toStdString()) && ++counter < max_repeat)
                    {
                        boost::this_thread::sleep(boost::posix_time::milliseconds(defCheckFileDelay));
                    }
                    if (counter < max_repeat)
                    {
                        pdf->setWindowTitle(fileName);
                        pdf->dynamicCall("Navigate(const QString&)", fileName);
                        //auto res = pdf->dynamicCall("setZoom(float)", ((int)m_StartingPreviewZoom)/100.0);
                    }
                    else
                    {
                        throw RuntimeException(ExceptInfo("basar.gui.qt.PrintPreviewOL.show()",
                            "no data (misssinf pdf file)!",
                            __FILE__, __LINE__));
                    }
                }

                pdf->show();
            }

//----------
			void PrintPreviewOL::setRawPrintFromConfig(const basar::I18nString& serverCfgName,		//OL_SERVER
				const basar::I18nString& printerCfgName)	//!< printer name)
			{
				basar::I18nString olServer;
                basar::printing::PrintUtils::getParaValue(serverCfgName, olServer);
				basar::I18nString olPrinter;
                basar::printing::PrintUtils::getParaValue(printerCfgName, olPrinter);
				
                setRawPrintInfo(olServer, olPrinter);
			}

        }   // namespace qt
    }   // namespace gui
}   // namespace bassar

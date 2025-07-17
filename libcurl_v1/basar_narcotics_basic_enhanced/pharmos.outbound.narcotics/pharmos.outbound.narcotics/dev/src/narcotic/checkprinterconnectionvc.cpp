#include "commonheader.h"
#include "checkprinterconnectionvc.h"
#include "loggerpool.h"

namespace narcotics
{
    namespace viewConn
    {

        using basar::VarString;
        using basar::db::aspect::AccessorPropertyTableRef;
        using basar::db::aspect::AccessorPropertyTable_YIterator;

        CheckPrinterConnectionVC::CheckPrinterConnectionVC(QWidget* parent) : QDialog(parent)
        {
            setupUi(this);
            connectSignals();
        }

        CheckPrinterConnectionVC::~CheckPrinterConnectionVC() { }

        void CheckPrinterConnectionVC::connectSignals()
        {
            QObject::connect(btnConnectionTest, SIGNAL(clicked()),
                this, SLOT(onBtnConnectionTest_clicked()));

            QObject::connect(btnTestPrint, SIGNAL(clicked()),
                this, SLOT(onBtnTestPrint_clicked()));
        }

        void CheckPrinterConnectionVC::injectPrinterIP(const basar::VarString& printerIP)
        {
            m_printerIP = printerIP;
        }

        void CheckPrinterConnectionVC::setPrinterConnectionResult(const basar::I18nString& printerConnectionResult)
        {
            lblConnectionResult->setText(printerConnectionResult.c_str());
        }

        void CheckPrinterConnectionVC::setTestPrintResult(const basar::I18nString& testPrintResult)
        {
            lblPrintTestResult->setText(testPrintResult.c_str());
        }

        basar::gui::tie::WidgetReturnEnum CheckPrinterConnectionVC::show()
        {
            if (LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                basar::ConstString msg = "show of CheckPrinterConnectionVC";
                LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            lblUadmPrinterIPResult->setText(m_printerIP.c_str());
            lblConnectionResult->setText(m_printerConnectionResult.c_str());

            return basar::gui::tie::getWidgetReturnType(exec());
        }

        void CheckPrinterConnectionVC::hide()
        {
            QWidget::hide();
        }

        bool CheckPrinterConnectionVC::shutdown()
        {
            lblConnectionResult->setText("");
            lblUadmPrinterIPResult->setText("");

            if (LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                basar::ConstString msg = "shutdown of CheckPrinterConnectionVC";
                LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
            }

            return close();
        }

        void CheckPrinterConnectionVC::onBtnConnectionTest_clicked()
        {
            lblConnectionResult->setText("");

            setCursor(Qt::WaitCursor);

            basar::appl::EventReturnStruct ret = 
                basar::appl::SystemEventManager::getInstance().fire("ConnectionTest");

            setCursor(Qt::ArrowCursor);

            if (ret.ret != basar::appl::HANDLER_OK)
                basar::gui::tie::infoMsgBox(this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
        }

        void CheckPrinterConnectionVC::onBtnTestPrint_clicked()
        {
            lblUadmPrinterIPResult->setText("");

            setCursor(Qt::WaitCursor);

            basar::appl::EventReturnStruct ret =
                basar::appl::SystemEventManager::getInstance().fire("TestPrint");

            setCursor(Qt::ArrowCursor);

            if (ret.ret != basar::appl::HANDLER_OK)
                basar::gui::tie::infoMsgBox(this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData());
        }

    }  //namespace viewConn
} //namespace narcotics
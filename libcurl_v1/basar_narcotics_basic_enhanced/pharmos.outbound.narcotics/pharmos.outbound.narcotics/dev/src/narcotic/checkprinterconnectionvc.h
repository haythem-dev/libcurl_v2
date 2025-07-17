#ifndef GUARD_CHECKPRINTERCONNECTIONVC_H
#define GUARD_CHECKPRINTERCONNECTIONVC_H

#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_checkprinterconnectionvc.h"
#pragma warning (pop)

namespace narcotics
{
namespace viewConn
{
    class CheckPrinterConnectionVC : public QDialog, public Ui::CheckPrinterConnectionClass,
        public boost::enable_shared_from_this<CheckPrinterConnectionVC>,
        public basar::gui::tie::IViewConnector
    {
        Q_OBJECT

        public:
            CheckPrinterConnectionVC(QWidget* parent = 0);
            ~CheckPrinterConnectionVC();

            void injectPrinterIP(const basar::VarString& printerIP);
            void setPrinterConnectionResult(const basar::I18nString& printerConnectionResult);
            void setTestPrintResult(const basar::I18nString& testPrintResult);

            basar::gui::tie::WidgetReturnEnum show();
            void hide();
            bool shutdown();

        private slots:
            void onBtnConnectionTest_clicked();
            void onBtnTestPrint_clicked();


        private:

            void connectSignals();

            basar::VarString m_printerIP;
            basar::I18nString m_printerConnectionResult;
    };

} //namespace viewConn
} //namespace narcotics

#endif // GUARD_CHECKPRINTERCONNECTIONVC_H
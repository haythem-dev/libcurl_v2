#ifndef GUARD_CHECKPRINTERCONNECTIONUC_H
#define GUARD_CHECKPRINTERCONNECTIONUC_H

#include "usecasecontroller.h"
#include "checkprinterconnectionvc.h"

namespace narcotics {
namespace useCase {

class CheckPrinterConnectionUC : public UseCaseController
{
public:
    CheckPrinterConnectionUC();
    ~CheckPrinterConnectionUC();

    virtual basar::appl::EventReturnType run();
    void registerEventHandlers(const bool forSave); //register = true / deregister = false
    void injectPrinterIP(const basar::I18nString& printerIP);

    SYSTEM_EVENT_HANDLER_DECLARATION(ConnectionTest)
    SYSTEM_EVENT_HANDLER_DECLARATION(TestPrint)

private: 
    CheckPrinterConnectionUC(const CheckPrinterConnectionUC&); // forbidden
    CheckPrinterConnectionUC& operator=(const CheckPrinterConnectionUC&); // forbidden

    bool ping(const std::string& ipAddress);

    basar::gui::tie::ViewConnPtr<viewConn::CheckPrinterConnectionVC> m_checkPrinterConnectionVC;
    basar::I18nString m_printerIP;

};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_CHECKPRINTERCONNECTIONUC_H
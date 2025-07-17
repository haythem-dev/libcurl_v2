#include "commonheader.h"
#include "checkprinterconnectionuc.h"
#include "narcoticsappl.h"
#include "loggerpool.h"
#include "testreceiptnoticeprintdata.h"

#include <cstdlib>
#include <iostream>


namespace narcotics {
namespace useCase {

    using basar::VarString;
    using basar::Int32;
    using basar::db::aspect::AccessorPropertyTable_YIterator;
    using basar::db::aspect::AccessorPropertyTableRef;

    CheckPrinterConnectionUC::CheckPrinterConnectionUC() { }
    CheckPrinterConnectionUC::~CheckPrinterConnectionUC() { }

    BEGIN_HANDLERS_REGISTRATION(CheckPrinterConnectionUC)
        SYSTEM_EVENT_HANDLER_REGISTRATION(CheckPrinterConnectionUC, ConnectionTest)
        SYSTEM_EVENT_HANDLER_REGISTRATION(CheckPrinterConnectionUC, TestPrint)
    END_HANDLERS_REGISTRATION()

    void CheckPrinterConnectionUC::injectPrinterIP(const basar::I18nString& printerIP)
    {
        m_printerIP = printerIP;
    }

    bool CheckPrinterConnectionUC::ping(const std::string& ipAddress)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string command = "ping -n 1 " + ipAddress;

        if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
        {
            // Failed to create process
            return false;
        }

        // Wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Get the exit code of the process
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // Clean up the process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return (exitCode == 0);
    }

    basar::appl::EventReturnType CheckPrinterConnectionUC::run()
    {
        m_checkPrinterConnectionVC->injectPrinterIP(m_printerIP);
        
        registerEventHandlers(true);

        m_checkPrinterConnectionVC->show();
        m_checkPrinterConnectionVC->shutdown();

        registerEventHandlers(false);

        return basar::appl::EventReturnStruct();
    }

    SYSTEM_EVENT_HANDLER_DEFINITION(CheckPrinterConnectionUC, ConnectionTest)
    {
        BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CheckPrinterConnectionUC::ConnectionTest");
        rSource;

        basar::VarString pingResult;
        if (ping(m_printerIP)) pingResult = QApplication::translate("CheckPrinterConnectionUC", "successful").toLocal8Bit().data();
        else pingResult = QApplication::translate("CheckPrinterConnectionUC", "unsuccessful").toLocal8Bit().data();

        m_checkPrinterConnectionVC->setPrinterConnectionResult(pingResult);

        return basar::appl::EventReturnStruct();
    }

    SYSTEM_EVENT_HANDLER_DEFINITION(CheckPrinterConnectionUC, TestPrint)
    {
        BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "CheckPrinterConnectionUC::TestPrint");
        rSource;

        narcotics::TestReceiptNoticePrintData testPrintData("BTMEmpfangsbest", "BTMEmpfangsbest");

        basar::cmnutil::RawPrint rawPrint;
        basar::cmnutil::PrintRetCodeEnum result = rawPrint.print(m_printerIP, "test", testPrintData);

        basar::VarString testPrintResult;
        if (result == basar::cmnutil::PrintRetCodeEnum::PRINTER_SUCCESS) testPrintResult = QApplication::translate("CheckPrinterConnectionUC", "successful").toLocal8Bit().data();
        else testPrintResult = QApplication::translate("CheckPrinterConnectionUC", "unsuccessful").toLocal8Bit().data();

        m_checkPrinterConnectionVC->setTestPrintResult(testPrintResult);

        return basar::appl::EventReturnStruct();
    }

} //namespace useCase
} //namespace narcotics
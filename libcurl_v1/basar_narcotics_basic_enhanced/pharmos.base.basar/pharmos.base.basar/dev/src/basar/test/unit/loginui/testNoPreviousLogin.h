#pragma once

#pragma warning(push)
#pragma warning(disable: 4127 4251 4481 4505 4512 4800)
#include <QtTest/QTest>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtCore/QTimer>
#pragma warning(pop)

#pragma warning(disable: 4505)
#pragma warning(disable: 4127)

#include <libbasarcmnutil.h>
#include <libbasarlogin.h>
#include <sstream>

#include "qtestcollection.h"
#include "helpers.h"

//requirement: if a user has never logged in into an application the following shall happen
//             - dialog has to show in the englisch language
//             - line edit 'username' has to be empty
//             - areas valid for the application have to be in the combobox 'area' and the first one must be selected
//             - line edit password has to be blank
//             - languages valid for the application have to be in the combobox 'langauge' and the first one must be selected
//             - title of the login dialog should read '[Identification] - [Appliation Name] with [Identification] being the translated to users language and [Application 
//               Name] being the string that is passed on call of interactiveLogin

class NoPreviousLogin : public QObject
{
        Q_OBJECT;

    public:
        NoPreviousLogin() : m_AppName("CODE_UI_TEST_APP"), m_DbConn(helpers::getDBConnection()), m_ApplicationId(-1)
        {

        }

    private:
        basar::I18nString m_AppName;
        basar::db::sql::ConnectionRef m_DbConn;
        basar::Int64 m_ApplicationId;

    private slots:

        //inits this test case
        void initTestCase()
        {
            try
            {
                auto stmt = m_DbConn.createStatement();

                //application
                std::ostringstream os;
                os << "insert into ppapplication (name, description) values ('" << m_AppName;
                os << "', 'test app' )";

                auto execRet = stmt.execute(os.str());

                m_ApplicationId = execRet.m_lastSerial.at(0);

                os.str("");

                //app language mapping
                os << "insert into ppcorapplang (languageid, applicationid) values ('de', " << m_ApplicationId << ")";

                execRet = stmt.execute(os.str());

                os.str("");

                //app area mapping
                os << "insert into ppcorappbranchreg (applicationid, regionid) values (" << m_ApplicationId << ", 2002)";

                execRet = stmt.execute(os.str());

            }
            catch(basar::Exception& e)
            {
                std::cout << "caught exception: " << e.what();
                throw;
            }
        };

        //cleans up this test case
        void cleanupTestCase()
        {
            auto stmt = m_DbConn.createStatement();

            //previous
            std::ostringstream os;

            //app language mapping
            os << "delete from ppcorapplang where applicationid = " << m_ApplicationId;
            auto execRet = stmt.execute(os.str());
            os.str("");

            //app area mapping
            os << "delete from ppcorappbranchreg where applicationid = " << m_ApplicationId;
            execRet = stmt.execute(os.str());
            os.str("");

            //application
            os << "delete from ppapplication where applicationid = " << m_ApplicationId;
            execRet = stmt.execute(os.str());

        };

        //called before each test function (=private slot)
        void init() {};

        //called after each test function (=private slot)
        void cleanup() {};

        void testNoPreviousLogin()
        {
            //as login is a modal dialog, our test code resides in a timer slot
            //note: the timer slot will be called from the test framework regardless -> make reentrant
            QTimer::singleShot(500, this, SLOT(checkTimeout()));

            basar::login::Manager::getInstance().interactiveLogin(m_AppName, "Tender login", 3);
        };

        void checkTimeout()
        {
            QWidget* dlg = QApplication::activeModalWidget();

            //as testframework calls all slots, we should check if we got a valid ptr
            if (dlg)
            {
                QLabel* area = dlg->findChild<QLabel*>(QString::fromLocal8Bit("ID_ST_AREA"));
                QLabel* username = dlg->findChild<QLabel*>(QString::fromLocal8Bit("ID_ST_USERNAME"));
                QLabel* language = dlg->findChild<QLabel*>(QString::fromLocal8Bit("ID_ST_LANGUAGE"));
                QLabel* password = dlg->findChild<QLabel*>(QString::fromLocal8Bit("ID_ST_PASSWORD"));

                QPushButton* btnOk = dlg->findChild<QPushButton*>(QString::fromLocal8Bit("okBt"));
                QPushButton* btnCancel = dlg->findChild<QPushButton*>(QString::fromLocal8Bit("cancelBt"));

                QComboBox* cboArea = dlg->findChild<QComboBox*>(QString::fromLocal8Bit("areaname"));
                QComboBox* cboLanguage = dlg->findChild<QComboBox*>(QString::fromLocal8Bit("languagename"));

                QLineEdit* leUserName = dlg->findChild<QLineEdit*>(QString::fromLocal8Bit("username"));
                QLineEdit* lePassword = dlg->findChild<QLineEdit*>(QString::fromLocal8Bit("password"));

                dlg->close();
                QCOMPARE(dlg->windowTitle(), QString::fromLocal8Bit("Identification - Tender login"));

                QCOMPARE(area->text(), QString::fromLocal8Bit("Area"));
                QCOMPARE(username->text(), QString::fromLocal8Bit("User"));
                QCOMPARE(language->text(), QString::fromLocal8Bit("Language"));
                QCOMPARE(password->text(), QString::fromLocal8Bit("Password"));

                QCOMPARE(btnOk->text(), QString::fromLocal8Bit("Login"));
                QCOMPARE(btnCancel->text(), QString::fromLocal8Bit("Cancel"));

                QCOMPARE(leUserName->text().isEmpty(), true);
                QCOMPARE(lePassword->text().isEmpty(), true);

                QCOMPARE(cboArea->currentText(), QString::fromLocal8Bit("Switzerland")); //make dynamic
                QCOMPARE(cboLanguage->currentText(), QString::fromLocal8Bit("German")); //make dynmic

            }
        }

};

ADD_TEST(NoPreviousLogin);

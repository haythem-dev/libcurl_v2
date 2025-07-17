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

#include <libbasarlogin.h>
#include <sstream>
#include <iostream>

#include "qtestcollection.h"
#include "helpers.h"

#pragma warning(disable: 4127)

//requirement: if a user has already logged in at least once and selected a different language than english the following should happen
//             - dialog has to show in the previously selected language
//             - login name of user has to be set in line edit 'username'
//             - previously selected area for that application has to be selected in combobox 'area' -> todo: add more areas to test this
//             - line edit password has to be blank
//             - combobox language has to be set to users previously selected language for that application
//             - title of the login dialog should read '[Identification] - [Appliation Name] with [Identification] being the translated to users language and [Application 
//               Name] being the string that is passed on call of interactiveLogin

class DELoginDialog : public QObject
{
        Q_OBJECT;

    public:
        DELoginDialog() : m_AppName("CODE_UI_TEST_APP"), m_DbConn(helpers::getDBConnection()), m_ApplicationId(-1), m_CurrentUser(helpers::getLoggedOnUserName())
        {

        }

    private:
        basar::I18nString m_AppName;
        basar::db::sql::ConnectionRef m_DbConn;
        basar::Int64 m_ApplicationId;
        basar::VarString m_CurrentUser;

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

                os.str("");

                //previous user
                os << "insert into pploginprevious (applicationid, globaluser, userid, languageid, branchregionid) values (";
                os << m_ApplicationId << ", ";
                os << "'" << m_CurrentUser.toUpper() << "',";
                os << "11288, 'de', 2002)";

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
            os << "delete from pploginprevious where applicationid = " << m_ApplicationId << " and ";
            os << "globaluser = '" << m_CurrentUser.toUpper() << "'";

            auto execRet = stmt.execute(os.str());

            os.str("");

            //app language mapping
            os << "delete from ppcorapplang where applicationid = " << m_ApplicationId;
            execRet = stmt.execute(os.str());
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

        void testDELoginDialog()
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

                auto stmt = m_DbConn.createStatement();

                std::ostringstream os;

                os << "select username from ppuser where userid = 11288";

                auto res = stmt.executeQuery(os.str());

                QCOMPARE(res.next(), true);

                basar::VarString userName = res.getString(0);

                dlg->close();
                QCOMPARE(dlg->windowTitle(), QString::fromLocal8Bit("Zugangsberechtigung - Tender login"));

                QCOMPARE(area->text(), QString::fromLocal8Bit("Gebiet"));
                QCOMPARE(username->text(), QString::fromLocal8Bit("Benutzerkennung"));
                QCOMPARE(language->text(), QString::fromLocal8Bit("Sprache"));
                QCOMPARE(password->text(), QString::fromLocal8Bit("Kennwort"));

                QCOMPARE(btnOk->text(), QString::fromLocal8Bit("Anmelden"));
                QCOMPARE(btnCancel->text(), QString::fromLocal8Bit("Abbrechen"));

                QCOMPARE(leUserName->text(), QString::fromLocal8Bit(userName.c_str()));
                QCOMPARE(lePassword->text().isEmpty(), true);

                QCOMPARE(cboArea->currentText(), QString::fromLocal8Bit("Switzerland")); //make dynamic
                QCOMPARE(cboLanguage->currentText(), QString::fromLocal8Bit("Deutsch")); //make dynmic

            }
        }

};

ADD_TEST(DELoginDialog);

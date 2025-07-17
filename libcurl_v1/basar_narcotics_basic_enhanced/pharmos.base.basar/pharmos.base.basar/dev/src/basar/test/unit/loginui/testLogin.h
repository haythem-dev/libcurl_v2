#pragma once

#pragma warning(push)
#pragma warning(disable: 4127 4251 4456 4481 4505 4512 4800)
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

//requirement: if a user has already logged in at least once and selected a different language than english the following should happen
//             - dialog has to show in the previously selected language
//             - login name of user has to be set in line edit 'username'
//             - previously selected area for that application has to be selected in combobox 'area' -> todo: add more areas to test this
//             - line edit password has to be blank
//             - combobox language has to be set to users previously selected language for that application
//             - title of the login dialog should read '[Identification] - [Appliation Name] with [Identification] being the translated to users language and [Application 
//               Name] being the string that is passed on call of interactiveLogin
//             if the user enters his accounts valid password and clicks ok the dialog should close and the call to interactiveLogin should return true


//pw: test -> crypt image: O(~%CY3)

class Login : public QObject
{
        Q_OBJECT;

    public:
        Login() : m_AppName("DEPENT"), m_DbConn(helpers::getDBConnection()), m_ApplicationId(-1), m_CurrentUser(helpers::getLoggedOnUserName()), m_UserId(-1), m_Password("test"),
            m_CryptedPassword("O(~%CY3)"), m_UserName("codedui_testuser")
        {

        }

    private:
        basar::I18nString m_AppName;
        basar::db::sql::ConnectionRef m_DbConn;
        basar::Int32 m_ApplicationId;
        const basar::VarString m_CurrentUser;
        basar::Int32 m_UserId;
        const basar::VarString m_Password;
        const basar::VarString m_CryptedPassword;
        const basar::VarString m_UserName;

    private slots:

        //inits this test case
        void initTestCase()
        {
            try
            {
                auto stmt = m_DbConn.createStatement();

                std::ostringstream os;

                //get application id of application
                os << "select applicationid from ppapplication where name = '" << m_AppName << "'";
                auto res = stmt.executeQuery(os.str());

                if (res.next())
                {
                    m_ApplicationId = res.getInt32(0);
                }
                else
                {
                    QWARN("retrieval of applicationid failed");
                }

                //user
                os.str("");

                os << "insert into ppuser (username, password) values ('"<< m_UserName << "','" << m_CryptedPassword << "')";

                auto execRet = stmt.execute(os.str());

                m_UserId = static_cast<basar::Int32>(execRet.m_lastSerial.at(0));

                os.str("");

                //previous user
                os << "insert into pploginprevious (applicationid, globaluser, userid, languageid, branchregionid) values (";
                os << m_ApplicationId << ", ";
                os << "'" << m_CurrentUser << "',";
                os << m_UserId << ", 'de', 6)";

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

            //previous user
            std::ostringstream os;
            os << "delete from pploginprevious where applicationid = " << m_ApplicationId << " and ";
            os << "globaluser = '" << m_CurrentUser << "'";

            auto execRet = stmt.execute(os.str());

            //user
            os.str("");

            os << "delete from ppuser where userid = " << m_UserId;
            execRet = stmt.execute(os.str());

        };

        //called before each test function (=private slot)
        void init() {};

        //called after each test function (=private slot)
        void cleanup() {};

        void testLogin()
        {
            //as login is a modal dialog, our test code resides in a timer slot
            //note: the timer slot will be called from the test framework regardless -> make reentrant
            QTimer::singleShot(500, this, SLOT(checkTimeout()));

            QCOMPARE(basar::login::Manager::getInstance().interactiveLogin(m_AppName, "Test Login", 3), true);
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

                QCOMPARE(dlg->windowTitle(), QString::fromLocal8Bit("Zugangsberechtigung - Test Login"));

                QCOMPARE(area->text(), QString::fromLocal8Bit("Gebiet"));
                QCOMPARE(username->text(), QString::fromLocal8Bit("Benutzerkennung"));
                QCOMPARE(language->text(), QString::fromLocal8Bit("Sprache"));
                QCOMPARE(password->text(), QString::fromLocal8Bit("Kennwort"));

                QCOMPARE(btnOk->text(), QString::fromLocal8Bit("Anmelden"));
                QCOMPARE(btnCancel->text(), QString::fromLocal8Bit("Abbrechen"));

                QCOMPARE(leUserName->text(), QString::fromLocal8Bit(m_UserName.c_str()));
                QCOMPARE(lePassword->text().isEmpty(), true);

                QCOMPARE(cboArea->currentText(), QString::fromLocal8Bit("Berlin")); //make dynamic
                QCOMPARE(cboLanguage->currentText(), QString::fromLocal8Bit("Deutsch")); //make dynmic

                //test login
                QTest::keyClicks(lePassword, QString::fromLocal8Bit(m_Password.c_str()));

                QTest::mouseClick(btnOk, Qt::LeftButton);

                QTRY_VERIFY(dlg->isHidden());
            }
        }

};

ADD_TEST(Login);

#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

//------------------------------------------------------------------------------
using basar::VarString;
using basar::ConstString;

using basar::cmnutil::MailMessage;
using basar::cmnutil::Smtp;

//------------------------------------------------------------------------------
extern bool g_RUNSHORT;

#define SMTP_SERVER_NAME "denu00smtpint.phoenix.loc"
#define SMTP_SERVER_IP   "10.156.80.21"
#define SMTP_FROM        "pharmos.basar.dev.test@phoenixgroup.eu"
#define SMTP_RCPT        "pharmos.basar.dev.mail@phoenixgroup.eu"

static ConstString SMTP_SUBJECT = "Subject Line: PHARMOS basar SMTP test";
static ConstString SMTP_BODY    = 
    "PHARMOS basar body line1\n"
    "PHARMOS basar body line2\n"
    "PHARMOS basar body line3";

//------------------------------------------------------------------------------
SUITE(Smtp)
{
    TEST(smtp_name_ok)
    {
#ifdef _WIN32
        Smtp smtp(SMTP_SERVER_NAME);

        Smtp::RetCodeEnum ret = smtp.connect();
        CHECK_EQUAL(Smtp::SMTP_SUCCESS, ret);

        MailMessage msg;
        msg.setFrom   (SMTP_FROM);
        msg.setTo     (SMTP_RCPT);
        msg.setSubject(SMTP_SUBJECT);
        msg.setBody   (SMTP_BODY);

        ret = smtp.send(msg);
        CHECK_EQUAL(Smtp::SMTP_SUCCESS, ret);

        smtp.disconnect();
#else
        // AIX: no DNS resolving available
        CHECK_THROW(
            Smtp(SMTP_SERVER_NAME),
            basar::cmnutil::TcpSocketException);
#endif
    }

    TEST(smtp_name_nonexisting_name)
    {
        CHECK_THROW(
            Smtp("invalid.mailgate.phoenix.loc"),
            basar::cmnutil::TcpSocketException);
    }

    TEST(smtp_ip_ok)
    {
        MailMessage msg;
        msg.setFrom   (SMTP_FROM);
        msg.setTo     (SMTP_RCPT);
        msg.setSubject(SMTP_SUBJECT);
        msg.setBody(SMTP_BODY);

        Smtp smtp(SMTP_SERVER_IP);

        Smtp::RetCodeEnum ret = smtp.connect();
        CHECK_EQUAL(Smtp::SMTP_SUCCESS, ret);

        ret = smtp.send(msg);
        CHECK_EQUAL(Smtp::SMTP_SUCCESS, ret);

        smtp.disconnect();
    }

    TEST(smtp_name_nonexisting_ip)
    {
        if (g_RUNSHORT)
            return;

        Smtp smtp("1.1.1.1");

        {
            Smtp::RetCodeEnum ret = smtp.connect();
            CHECK_EQUAL(Smtp::SMTP_OFFLINE, ret);
        }
    }
}


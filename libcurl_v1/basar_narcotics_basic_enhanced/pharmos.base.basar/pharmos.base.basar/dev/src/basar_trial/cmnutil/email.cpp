
#include <libbasarcmnutil.h>

#include "testcmnutil.h"

#include <libbasarcmnutil_email.h>


using basar::cmnutil::Smtp;
using basar::cmnutil::MailMessage;
using basar::cmnutil::Locale;


#ifndef __NET__
//#define SMTP_SERVER		"10.100.200.102" //"nu1s0001" 
#define SMTP_SERVER		"ntmaprz1" 
//#define SMTP_SERVER		"mailgate.phoenixgroup.eu"	// to use this server, your ip address must be registered
#define SMTP_RCPT		"m.eichenlaub@phoenixgroup.eu"
//#define SMTP_RCPT		"d.kapusta@phoenixgroup.eu"
#define SMTP_FROM       "tester@phoenix-ag.de"
#else
#define SMTP_SERVER		"finale"
#define SMTP_RCPT		"eichen@finale.localdomain"
#define SMTP_FROM       "tester@phoenix-ag.de"
#endif

//------------------------------------------------------------------------------
void test_default()
{
	printf("\n");
	printf("--- test_default ---------------------------\n");
	printf("\n");

	const UInt8 EURO = 0x80;
	VarString  body;
	body.format("\n"
				".\r\n"
				"€€€€ (CP1252 EURO = 0x80)\r\n"
		        "MEI's test long line VVVVVv.-v,-v.,vddsdssaaswqwiwiwiwiwSSSxcxxxXSHSASHAH AHAHAAAZAZAZAZAZAZAZAZAAZAöäüüpüööäöäöääöäöäöäöäöäöäö\r\n"
		        ".\n"
				"\n"
				"a.äöüß§@%c%c%c%c (CP 8859-15 EURO = 0xA4)\r\n"
		        "last line",
				EURO,
				EURO,
				EURO,
				EURO);

	MailMessage msg; //(basar::cmnutil::CROATIAN);

	msg.setFrom   (SMTP_FROM);
	msg.setTo     (SMTP_RCPT);
	//msg.setBCC     ("informix@finale.localdomain");
	//msg.setBCC    ("root@finale.localdomain");
	//msg.setBCC    ("michael_eichenlaub@web.de");
	msg.setSubject("Testmail for own SMTP class; Öffµe Tür döäaäöü german literals: €ÄÖÜ longlinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelonglinelongline");
	msg.setBody(body);


	Smtp smtp(SMTP_SERVER);

	if (Smtp::SMTP_SUCCESS != smtp.connect())
	{
		//assert(0);
		return;
	}

	if (Smtp::SMTP_SUCCESS != smtp.send(msg))
		printf("message not sent\n");

	smtp.disconnect();

}

//------------------------------------------------------------------------------
void test_de()
{
	printf("\n");
	printf("--- test_de ---------------------------\n");
	printf("\n");

	VarString  body, subj;

#ifdef WIN32
	ConstString inp = "ABC\xD6\xC4\xDC\xDF\x80";
#else
	ConstString inp = "ABC\xD6\xC4\xDC\xDF\xA4";
#endif

	subj.format("German Testmail for own SMTP class: <%s>", inp);

	body.format("\n"
				".\r\n"
				"<%s>\r\n"
		        "last line",
				inp);

	MailMessage msg;	// Locale::GERMAN_DE

	msg.setFrom   (SMTP_FROM);
	msg.setTo     (SMTP_RCPT);
	msg.setSubject(subj);
	msg.setBody   (body);


	Smtp smtp(SMTP_SERVER);

	if (Smtp::SMTP_SUCCESS != smtp.connect())
	{
		//assert(0);
		return;
	}

	if (Smtp::SMTP_SUCCESS != smtp.send(msg))
		printf("message not sent\n");

	smtp.disconnect();
}

//------------------------------------------------------------------------------
void test_html_de()
{
	printf("\n");
	printf("--- test_html_de ---------------------------\n");
	printf("\n");

	VarString  body, subj;

#ifdef WIN32
	ConstString inp = "ABC\xD6\xC4\xDC\xDF\x80";
#else
	ConstString inp = "ABC\xD6\xC4\xDC\xDF\xA4";
#endif

	subj.format("German HTML Testmail for own SMTP class: <%s>", inp);

	body =	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\">"
			"<html>"
			"<head>"
			"  <meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-15\">"
			"  <title>German HTML Testmail for own SMTP class</title>"
			"<style type=\"text/css\">"
			" #red  { font-size:2em; color:red; }"
			" #blue { font-size:1em; color:blue; }"
			"</style>"
			"</head>"
			"<body>"
			"<p id=\"red\">Gro&szlig;er roter Text</p>"
			"<p id=\"blue\">Kleiner blauer Text</p>"
			"Deutsche Umlaute: &Auml;&Ouml;&Uuml;&auml;&ouml;&uuml;&szlig;<br>"
			"<p>"
			"<table border=\"1\">"
			"<caption style=\"caption-side:bottom\">Tabelle Assoziationen</caption>"
			"<tr>"
			"<th>Berlin</th><th>Hamburg</th><th>M&uuml;nchen</th>"
			"</tr><tr>"
			"<td>Milj&ouml;h</td><td>Kiez</td><td>Bierdampf</td>"
			"</tr><tr>"
			"<td>Buletten</td><td>Frikadellen</td><td>Fleischpflanzerl</td>"
			"</tr></table>"
			"</p>"
			"<p><img src=\"http://www.google.de/intl/de_de/images/logo.gif\" alt=\"Google\"></p>"
			"<p><a href=\"http://www.google.de\">Link zu google.de</a></p>"
			"last text line"
			"</body></html>";

	MailMessage msg( Locale::GERMAN_DE );

	msg.setContentType( MailMessage::TEXT_SIMPLEHTML );
	msg.setFrom   (SMTP_FROM);
	msg.setTo     (SMTP_RCPT);
	msg.setSubject(subj);
	msg.setBody   (body);


	Smtp smtp(SMTP_SERVER);

	if (Smtp::SMTP_SUCCESS != smtp.connect())
	{
		//assert(0);
		return;
	}

	if (Smtp::SMTP_SUCCESS != smtp.send(msg))
		printf("message not sent\n");

	smtp.disconnect();
}

//------------------------------------------------------------------------------
void test_bg()
{
	printf("\n");
	printf("--- test_bg ---------------------------\n");
	printf("\n");

	VarString  body, subj;

#ifdef WIN32
	ConstString inp = "ABC\xEE\xE1\xF9\xE5\xF1\xF2\xE2\xEE";
#else
	ConstString inp = "ABC\xDE\xD1\xE9\xD5\xE1\xE2\xD2\xDE";
#endif

	subj.format("Bulgarian Testmail for own SMTP class: <%s>", inp);

	body.format("\n"
				".\r\n"
				"<%s>\r\n"
		        "last line",
				inp);

	MailMessage msg(Locale::BULGARIAN_BG);

	msg.setFrom   (SMTP_FROM);
	msg.setTo     (SMTP_RCPT);
	msg.setSubject(subj);
	msg.setBody   (body);


	Smtp smtp(SMTP_SERVER);

	if (Smtp::SMTP_SUCCESS != smtp.connect())
	{
		//assert(0);
		return;
	}

	if (Smtp::SMTP_SUCCESS != smtp.send(msg))
		printf("message not sent\n");

	smtp.disconnect();
}

//------------------------------------------------------------------------------
void test_bg2()
{
	printf("\n");
	printf("--- test_bg2 ---------------------------\n");
	printf("\n");

	VarString  body, subj;

#ifdef WIN32
	ConstString inp = "ABC\xEE\xE1\xF9\xE5\xF1\xF2\xE2\xEE";
#else
	ConstString inp = "ABC\xDE\xD1\xE9\xD5\xE1\xE2\xD2\xDE";
#endif

	subj.format("Testmail w/ Bulgarian Locale for own SMTP class: <%s>", inp);

	body.format("\n"
				".\r\n"
				"<%s>\r\n"
		        "last line",
				inp);

	Locale::setLocale(Locale::BULGARIAN_BG);

	MailMessage msg;

	msg.setFrom   (SMTP_FROM);
	msg.setTo     (SMTP_RCPT);
	msg.setSubject(subj);
	msg.setBody   (body);


	Smtp smtp(SMTP_SERVER);

	if (Smtp::SMTP_SUCCESS != smtp.connect())
	{
		//assert(0);
		return;
	}

	if (Smtp::SMTP_SUCCESS != smtp.send(msg))
		printf("message not sent\n");

	smtp.disconnect();
}

//------------------------------------------------------------------------------
void test_email()
{
	test_default();
	//test_de();
	//test_html_de();
	//test_bg();
	//test_bg2();
}

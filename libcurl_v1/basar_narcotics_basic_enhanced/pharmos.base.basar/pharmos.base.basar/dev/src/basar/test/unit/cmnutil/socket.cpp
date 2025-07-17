#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include <stdio.h>

//------------------------------------------------------------------------------
extern bool g_RUNSHORT;

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int16;
using basar::Int32;
using basar::BLong;
using basar::cmnutil::TcpSocket;
using basar::cmnutil::TcpSocketException;
using basar::cmnutil::Logging;

//------------------------------------------------------------------------------
SUITE(Socket)
{

//----------------------------------------------------------------------------

static ConstString UNIX_REMOTEHOST      = 
#ifdef _WIN32
                                          "zqas21";
#else
                                          "denu0uazqas21";
#endif                                          
static ConstString UNIX_REMOTESERVICE   = "smtp";
static Int16       UNIX_REMOTEPORT_FAIL = 29999;

static ConstString CICS_REMOTEHOST      = "129.210.15.2";
static Int16       CICS_REMOTEPORT      =  3102;
static Int16       CICS_REMOTEPORT_FAIL =  3103;

static ConstString REMOTEHOST_FAIL      = "xyzhost";
static ConstString REMOTEIP_FAIL        = "1.1.1.1";
static Int16       REMOTEPORT_FAIL      = 4711;

static Int32       TIMEOUT              =  5;	// seconds
static Int32       LOOP_CONNECT         = 20;	// loops for repeated connects

static log4cplus::Logger logSock        = Logging::getInstance("utcmnutil.Socket");

//----------------------------------------------------------------------------
TEST(Close_Ok)
{
	TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
	CHECK(!sock.isOpen());

    sock.close();
    sock.close();
}

//----------------------------------------------------------------------------
TEST(Connect_Ok)
{
	if (g_RUNSHORT)
		return;

	TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
	CHECK(!sock.isOpen());

	TcpSocket::RetCodeEnum ret = sock.open();
	CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
	CHECK(sock.isOpen());

	// exchange data
	{
		char      buf[2048] = { 0 };
		VarString msg;
		BLong     bytes;
	
		bytes = sock.receive(buf, sizeof(buf));
		CHECK_EQUAL("220", VarString(buf).substr(0, 3).c_str());
		CHECK(bytes > 0);

		msg.format("HELO %s\r\n", sock.getLocalHostName().c_str());
		sock.send(msg.c_str(), msg.length());
	
		bytes = sock.receive(buf, sizeof(buf));
		CHECK_EQUAL("250", VarString(buf).substr(0, 3).c_str());
		CHECK(bytes > 0);

		msg.format("QUIT\r\n");
		sock.send(msg.c_str(), msg.length());
		
		bytes = sock.receive(buf, sizeof(buf));
		CHECK_EQUAL("221", VarString(buf).substr(0, 3).c_str());
		CHECK(bytes > 0);
	}

	CHECK(sock.isOpen());
	sock.close();
	CHECK(!sock.isOpen());
}

//------------------------------------------------------------------------------
TEST(isAvail_StateFail)
{
	TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
	CHECK(!sock.isOpen());

	TcpSocket::RetCodeEnum ret = sock.open();
	CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
	CHECK(sock.isOpen());

	// socket already opened
	CHECK_THROW(sock.isAvail(TIMEOUT), TcpSocketException);
}

//------------------------------------------------------------------------------
TEST(isAvail_StateOk)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_StateOk()");
	
	TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
	CHECK(!sock.isOpen());

	bool fthrown = false;

	try
	{
		// socket not yet opened
		sock.isAvail(TIMEOUT);
		CHECK(!sock.isOpen());
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_InvalidHost)
{
	CHECK_THROW(TcpSocket(REMOTEHOST_FAIL, REMOTEPORT_FAIL), TcpSocketException);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_InvalidIp)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnFail_InvalidIp()");

	bool fthrown = false;

	try
	{
		TcpSocket sock(REMOTEIP_FAIL, REMOTEPORT_FAIL);
		CHECK(!sock.isOpen());

		time_t start,
			   stop;
		bool   fAvail;
		
		start = time(0);

		{
			UNITTEST_TIME_CONSTRAINT(5500);	// ms
			
			// host connect() returns CONNINPROGRESS => timeout
			fAvail = sock.isAvail(TIMEOUT);
		}
		
		stop = time(0);
		
		CHECK      (4.5 < difftime(stop, start));  
		CHECK(!sock.isOpen());
		CHECK(!fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_Unix)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnFail_Unix()");

	bool fthrown = false;

	try
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTEPORT_FAIL);
		CHECK(!sock.isOpen());

		bool fAvail;

		{
			UNITTEST_TIME_CONSTRAINT(5500);	// ms

			fAvail = sock.isAvail(TIMEOUT);
		}

		CHECK(!sock.isOpen());
		CHECK(!fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnOk_Unix)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnOk_Unix()");

	if (g_RUNSHORT)
		return;

	bool fthrown = false;

	try
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
		CHECK(!sock.isOpen());

		bool fAvail = sock.isAvail(TIMEOUT);
		CHECK(!sock.isOpen());
		CHECK(fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_Cics)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnFail_Cics()");

	bool fthrown = false;

	try
	{
		TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT_FAIL);
		CHECK(!sock.isOpen());

		bool   fAvail;

		{
			UNITTEST_TIME_CONSTRAINT(5500);	// ms
			
			fAvail = sock.isAvail(TIMEOUT);
		}
		
		CHECK(!sock.isOpen());
		CHECK(!fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnOk_Cics)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnOk_Cics()");

	if (g_RUNSHORT)
		return;

	bool fthrown = false;

	try
	{
		TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT);
		CHECK(!sock.isOpen());

		bool fAvail = sock.isAvail(TIMEOUT);

		CHECK(!sock.isOpen());
		CHECK(fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_FollowingConn_Unix)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnFail_FollowingConn_Unix()");

	if (g_RUNSHORT)
		return;

	bool fthrown = false;

	try
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTEPORT_FAIL);
		CHECK(!sock.isOpen());

		bool fAvail;

		{
			UNITTEST_TIME_CONSTRAINT(5500);	// ms

			fAvail = sock.isAvail(TIMEOUT);
		}

		CHECK(!sock.isOpen());
		CHECK(!fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);

	// test if following connection is corrupt
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
		CHECK(!sock.isOpen());

		TcpSocket::RetCodeEnum ret = sock.open();
		CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
		CHECK(sock.isOpen());

		sock.close();
		CHECK(!sock.isOpen());
	}
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnOk_FollowingConn_Unix)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnOk_FollowingConn_Unix()");
	
	if (g_RUNSHORT)
		return;

	bool fthrown = false;

	try
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
		CHECK(!sock.isOpen());

		bool fAvail = sock.isAvail(TIMEOUT);
		CHECK(!sock.isOpen());
		CHECK(fAvail);
	}
	catch (...)
	{
		fthrown = true;
	}

	CHECK(!fthrown);

	// test if following connection is corrupt
	{
		TcpSocket sock(UNIX_REMOTEHOST, UNIX_REMOTESERVICE);
		CHECK(!sock.isOpen());

		TcpSocket::RetCodeEnum ret = sock.open();
		CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
		CHECK(sock.isOpen());

		sock.close();
		CHECK(!sock.isOpen());
	}
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnFail_FollowingConn_Cics)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnFail_FollowingConn_Cics()");
	
	if (g_RUNSHORT)
		return;

	for (Int32 i = 0; i < LOOP_CONNECT; ++i)
	{
		bool fthrown = false;

		try
		{
			TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT_FAIL);
			CHECK(!sock.isOpen());

			bool   fAvail;

			{
				UNITTEST_TIME_CONSTRAINT(5500);	// ms
				
				fAvail = sock.isAvail(TIMEOUT);
			}
			
			CHECK(!sock.isOpen());
			CHECK(!fAvail);
		}
		catch (...)
		{
			fthrown = true;
		}

		CHECK(!fthrown);

		// test if following connection is corrupt
		{
			TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT);
			CHECK(!sock.isOpen());

			TcpSocket::RetCodeEnum ret = sock.open();
			CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
			CHECK(sock.isOpen());

			sock.close();
			CHECK(!sock.isOpen());
		}
	}
}

//------------------------------------------------------------------------------
TEST(isAvail_ConnOk_FollowingConn_Cics)
{
	BLOG_TRACE_METHOD(logSock, "isAvail_ConnOk_FollowingConn_Cics()");
	
	if (g_RUNSHORT)
		return;

	for (Int32 i = 0; i < LOOP_CONNECT; ++i)
	{
		bool fthrown = false;

		try
		{
			TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT);
			CHECK(!sock.isOpen());

			bool fAvail = sock.isAvail(TIMEOUT);

			CHECK(!sock.isOpen());
			CHECK(fAvail);
		}
		catch (...)
		{
			fthrown = true;
		}

		CHECK(!fthrown);

		// test if following connection is corrupt
		{
			TcpSocket sock(CICS_REMOTEHOST, CICS_REMOTEPORT);
			CHECK(!sock.isOpen());

			TcpSocket::RetCodeEnum ret = sock.open();
			CHECK_EQUAL(TcpSocket::TCP_SUCCESS, ret);
			CHECK(sock.isOpen());

			sock.close();
			CHECK(!sock.isOpen());
		}
	}
}

//------------------------------------------------------------------------------
} // SUITE


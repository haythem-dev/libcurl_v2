//----------------------------------------------------------------------------
/*! \file
 *  \brief  tcp socket class
 *  \author Michael Eichenlaub
 *  \date   31.08.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_socket.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#ifdef __linux__
		#include <rpc/types.h>
	#endif

	#ifdef _AIX
	#include <sys/socket.h>
	#include <sys/param.h>
	#endif
#endif

#include <string.h>
#include <assert.h>

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
Int16					 TcpSocket::s_NET_ADRESSFAMILY = AF_INET;	//!< internetwork: UDP, TCP, etc.

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

typedef Int32			 socklen_t;							//!< socket struct's length
typedef SOCKET			 Socket;							//!< platform specific socket type
typedef char             Option[sizeof(int) + 1];			//!< platform specific option type

static const Int32		 MAXHOSTNAMELEN	 = 256;				//!< maximum host name length
static const Int32		 CONN_TIMEDOUT	 = WSAETIMEDOUT;	//!< socket error code for time out
static const Int32		 CONN_REFUSED	 = WSAECONNREFUSED;	//!< socket error code for connection refused by remote host
static const Int32       CONN_INPROGRESS = WSAEWOULDBLOCK;	//!< socket error code for non-blocking socket 
															//!< whose connection cannot be completed immediately

#else

typedef int				 Socket;							//!< platform specific socket type
typedef int              Option[1];							//!< platform specific option type

static const Int32		 INVALID_SOCKET	 =  -1;				//!< not initialized socket descriptor
static const BLong		 SOCKET_ERROR	 =  -1;				//!< generic socket error code -> check errno
static const Int32		 SD_RECEIVE		 = SHUT_RD;			//!< parameter for ::shutdown(): receive        no longer allowed
static const Int32		 SD_SEND		 = SHUT_WR;			//!< parameter for ::shutdown(): send           no longer allowed
static const Int32		 SD_BOTH		 = SHUT_RDWR;		//!< parameter for ::shutdown(): send & receive no longer allowed
static const Int32		 CONN_TIMEDOUT	 = ETIMEDOUT;		//!< socket error code for time out
static const Int32		 CONN_REFUSED	 = ECONNREFUSED;	//!< socket error code for connection refused by remote host
static const Int32       CONN_INPROGRESS = EINPROGRESS; 	//!< socket error code for non-blocking socket whose connection cannot be completed immediately

#endif

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
TcpSocket::TcpSocket (const VarString & rRemoteHost,
					  Int16			    servicePort)
         : m_State         (STATE_INITIAL     ),
		   m_LocalHostAddr (0                 ),
		   m_ServicePort   (htons(servicePort)),
		   m_Socket        (INVALID_SOCKET    )
{
	try
	{
		init(rRemoteHost);
	}
	catch (const basar::Exception &)
	{
		shutdown();
		throw;
	}
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
TcpSocket::TcpSocket (const VarString & rRemoteHost ,
					  const VarString & rServiceName)
         : m_State         (STATE_INITIAL ),
		   m_LocalHostAddr (0             ),
		   m_ServicePort   (0             ),
		   m_Socket        (INVALID_SOCKET)
{
	try
	{
		init(rRemoteHost, rServiceName);
	}
	catch (const basar::Exception &)
	{
		shutdown();
		throw;
	}
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
TcpSocket::~TcpSocket()
{
	try
	{
		shutdown();
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}

/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32
	// wait to avoid disturbing following connections
	::usleep(50 * 1000);		// 50 ms
#endif
	
}

//------------------------------------------------------------------------------
//! \return non critical reason why connection didn't succeed
//! \throw  no-throw
TcpSocket::RetCodeEnum TcpSocket::open()
{
	create();
	
	RetCodeEnum eRet;

	if (TCP_SUCCESS != (eRet = connect()))
		return eRet;

	enable();

	return eRet;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
void TcpSocket::close(bool halfduplex /* = false */)
{
	disable   (halfduplex);	// connection remains "TIME_WAIT" is ok
	disconnect();
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::send(ConstString buffer,
					 BULong		 buflen) const
{
	BLOG_TRACE_METHOD(LoggerPool::tcpSocket(), "send()");

	assert(0 < buflen);

	const char * pBuf      = buffer;
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	int          bytesSent = 0;
	int          bytesBuf  = static_cast<int>(buflen);
#else
	ssize_t      bytesSent = 0;
	size_t       bytesBuf  = buflen;
#endif

    while ( (0 < bytesBuf)  &&  
		    (0 < (bytesSent = ::send( m_Socket, pBuf, bytesBuf, 0))) )
    {
		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "send(): sent %d bytes", bytesSent);
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}

        pBuf     += bytesSent;
        bytesBuf -= bytesSent;
    }

	if ( (SOCKET_ERROR == bytesSent) ||
		 (0            == bytesSent)    )
	{
		VarString msg;
		msg.format("::send() should transmit %d bytes, returned %d "
				   "and failed with %d", 
					buflen,
					bytesSent,
					getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.send()", 
											msg, 
											__FILE__, __LINE__));
	}
}

//------------------------------------------------------------------------------
//! \return number of received bytes
//! \throw  TcpSocketException
BLong TcpSocket::receive(char * const buffer,
				         BULong	      buflen) const
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	BLong bytesRead = ::recv(m_Socket, buffer, static_cast<int>(buflen), 0);
#else
	BLong bytesRead = ::recv(m_Socket, buffer, buflen, 0);
#endif

	if (SOCKET_ERROR == bytesRead)
	{
		VarString msg;
		msg.format("::recv() failed with %d", getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.receive()", 
					                        msg, 
											__FILE__, __LINE__));
	}

	if (0 == bytesRead)
	{
		VarString msg;
		msg.format("::recv() returns 0: connection closed by remote host");
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.receive()", 
										    msg, 
											__FILE__, __LINE__));
	}

	return bytesRead;
}

//------------------------------------------------------------------------------
//! \return socket is created, connected and enabled
//! \throw  no-throw
bool TcpSocket::isOpen() const
{
	return (STATE_ENABLE == m_State);
}

//------------------------------------------------------------------------------
//! \return could non-blocking connection be completed without timeout or failure?
//! \throw  TcpSocketException
bool TcpSocket::isConnectComplete(Int32 timeout) const
{
	static ConstString func = "basar.cmnutil.TcpSocket.isConnectComplete";

	timeval tv; 
	tv.tv_sec  = timeout;
	tv.tv_usec = 0;

	fd_set writefs;
	FD_ZERO(&writefs);
	FD_SET (static_cast<Socket>(m_Socket), &writefs);

	// test for timeout
	int sel = select(
#ifdef _WIN32
		0, // avoid warning "conversion from SOCKET to int"
#else
		m_Socket + 1,
#endif
		NULL, &writefs, NULL, &tv
	);

	switch (sel)
	{
	case SOCKET_ERROR:
		{
			VarString msg;
			msg.format("select(%d, %d secs) failed with %d", 
					   m_Socket,
					   timeout,
					   errno);
			throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
		}

	case 0:
		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("isConnectComplete(): select(%d, %d secs) returned 0 => timeout", 
					   m_Socket,
					   timeout);
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}

		return false;
	
	default:
		assert(FD_ISSET(m_Socket, &writefs)); 
		break;
	}

	// socket was selected for write => check pending error
	Option    optval;
	socklen_t optlen = sizeof(optval);
	
	if (SOCKET_ERROR == getsockopt(m_Socket, SOL_SOCKET, SO_ERROR, optval, &optlen) )
	{ 
		VarString msg;
		msg.format("getsockopt(%d) failed with %d", 
				   m_Socket,
				   errno);
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	// check the returned value
	int error;

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	error = ::atoi(optval);
#else
	error = optval[0];
#endif

	switch (error)
	{ 
	case 0:
		break;

	case CONN_REFUSED:
		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("isConnectComplete(): getsockopt(%d) returned %d => connection refused", 
					   m_Socket,
					   CONN_REFUSED);
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}

		return false;

	default:
		VarString msg;
		msg.format("delayed, asynchronous connection (%d) failed with %d", 
				   m_Socket,
				   error);
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	return true;
}

//------------------------------------------------------------------------------
//! \return server process is listening at remote host/port and can respond
//! \throw  TcpSocketException
bool TcpSocket::isAvail(Int32 timeout) 
{
	static ConstString func = "basar.cmnutil.TcpSocket.isAvail()";
	
	if (STATE_STARTUP != m_State)
	{
		VarString msg;
		msg.format("m_State: expected %d, but is %d", 
				   STATE_STARTUP,
				   m_State);
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	create       ();				// create socket
	setBlockMode(false);			// non-blocking socket
	RetCodeEnum ret = connect();	// non-blocking connect

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("isAvail(): non-blocking connect() returned %d", ret);
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}
	
	bool fAvail = true;
	
	switch (ret)
	{
	case TCP_SUCCESS:
		break;

	case TCP_CONNREFUSED:
		fAvail = false;
		break;

	case TCP_CONNINPROGRESS:
		fAvail = isConnectComplete(timeout);
		break;

	default:
		{
			VarString msg;
			msg.format("connect(): for non-blocking mode expected %d, "
                       "but failed with %d", 
					   TCP_CONNINPROGRESS,
					   ret);
			throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
		}
	}

	if (fAvail)
	{
		enable();
		disable(false);	// empty buffer
	}

	disconnect();
	
	return fAvail;
}

//------------------------------------------------------------------------------
//! \return local host name
//! \throw  no-throw
const VarString & TcpSocket::getLocalHostName() const
{
	return m_LocalHostName;
}

//------------------------------------------------------------------------------
//! \return socket descriptor
//! \throw  no-throw
Socket TcpSocket::getSocketDescriptor() const
{
	return m_Socket;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
void TcpSocket::init(const VarString & rRemoteHost,
					 const VarString & rServiceName /* = "" */)
{
	startup();

	if (0 == m_ServicePort)
	{
		assert(!rServiceName.empty());

		getServicePort(rServiceName);
	}

	getLocalHost  ();
	getRemoteHost (rRemoteHost);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
void TcpSocket::shutdown()
{
	disable   (false);	// connection remains waiting ?
	disconnect();
	cleanup   ();
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::startup()
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	::WSADATA wsaData;
	Int32     error;

	// init socket library
	if ( 0 != (error = ::WSAStartup( MAKEWORD(1, 1), &wsaData)) )
	{	
		VarString msg;
		msg.format("::WSAStartup(1.1) failed with %d", error);
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.startup()", 
											msg, 
											__FILE__, __LINE__));
	}

#endif

	m_State = STATE_STARTUP;

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("startup(): successful");
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::getLocalHost()
{
	char hostname[MAXHOSTNAMELEN];

	if (SOCKET_ERROR == ::gethostname(hostname, MAXHOSTNAMELEN))
	{
		VarString msg;
		msg.format("::gethostname() failed with %d", getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.getLocalHost()", 
					                        msg, 
											__FILE__, __LINE__));
	}

	::hostent * pEntry = ::gethostbyname(hostname);

	if (0 == pEntry)
	{
		VarString msg;
		msg.format("::gethostbyname(%s) failed with %d", 
			       hostname,
				   getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.getLocalHost()", 
					                        msg, 
											__FILE__, __LINE__));
	}

	m_LocalHostName = pEntry->h_name;
	m_LocalHostAddr = *(reinterpret_cast<ULong32 *>(pEntry->h_addr_list[0]));

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "getLocalHost(): ::gethostbyname(0) = <%s> %d.%d.%d.%d successful", 
			         m_LocalHostName.c_str(),
					 (m_LocalHostAddr >>  0) & 0xff,
					 (m_LocalHostAddr >>  8) & 0xff,
					 (m_LocalHostAddr >> 16) & 0xff,
					 (m_LocalHostAddr >> 24) & 0xff);
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                              msg, 
											  __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::getRemoteHost(const VarString & rName)
{
	// check for host name or host IP address
	ULong32 ip_addr = ::inet_addr(rName.c_str());

	if (0 == ip_addr)	// host must be empty or contains whitespaces
	{
		VarString msg;
		msg.format("::inet_addr(\"%s\") returns 0 -> hostname doesn't contain valid data", 
				   rName.c_str());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.getRemotehost()", 
											msg, 
											__FILE__, __LINE__));
	}


	if (INADDR_NONE == ip_addr)	// host contains name
	{
		::hostent * pEntry = ::gethostbyname(rName.c_str());

		if (0 == pEntry)
		{
			VarString msg;
			msg.format("::gethostbyname(\"%s\") failed with %d", 
				       rName.c_str(),
				       getLastError());
			throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.getRemoteHost()", 
												msg, 
												__FILE__, __LINE__));
		}

		m_RemoteHostAddr = *(reinterpret_cast<ULong32 *>(pEntry->h_addr_list[0]));

		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "getRemoteHost(): ::gethostbyname(%s) = <%s> "
					    "%d.%d.%d.%d successful",
				         rName.c_str(),
						 pEntry->h_name,
						 (m_RemoteHostAddr >>  0) & 0xff,
						 (m_RemoteHostAddr >>  8) & 0xff,
						 (m_RemoteHostAddr >> 16) & 0xff,
						 (m_RemoteHostAddr >> 24) & 0xff);
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}
	}
	else // host contains IP address; don't need to call gethostbyaddr(), 
	{ 	 // because dns request isn't necessary
		m_RemoteHostAddr = ip_addr;

		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "getRemoteHost(): rName = %s (IP address) nothing to do", 
					   rName.c_str());
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
					                              msg, 
												  __FILE__, __LINE__);
		}
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::getServicePort(const VarString & rServiceName)
{
	static ConstString SERVICE_PROTOCOL = "tcp";

	::servent * pEntry = ::getservbyname(rServiceName.c_str(), SERVICE_PROTOCOL);

	if (0 == pEntry)
	{
		VarString msg;
		msg.format("::getservbyname(\"%s\", \"%s\") failed with %d", 
				   rServiceName.c_str(),
				   SERVICE_PROTOCOL,
			       getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.getServicePort()", 
					             msg, 
								 __FILE__, __LINE__));
	}

	m_ServicePort = pEntry->s_port;

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "getServicePort(): ::getservbyname(%s/%s) returns port %d", 
				    rServiceName.c_str(), 
					SERVICE_PROTOCOL,
					ntohs(m_ServicePort));
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				 							  msg, 
											  __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::create()
{
	static const Int32 SOCKETTYPE = SOCK_STREAM;

	if ( INVALID_SOCKET == (m_Socket = static_cast<Int32>(::socket(s_NET_ADRESSFAMILY, 
																   SOCKETTYPE        , 
																   0                 ))) )
	{
		VarString msg;
		msg.format("::socket(%d, %d, 0) failed with %d", 
			       s_NET_ADRESSFAMILY,
				   SOCKETTYPE,
				   getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.create()", 
											msg, 
											__FILE__, __LINE__));
	}

	m_State = STATE_SOCKET;

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("create(): ::socket(%d, %d, 0) = %d successful",
			       s_NET_ADRESSFAMILY,
				   SOCKETTYPE,
				   m_Socket);
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw TcpSocketException
//!
void TcpSocket::setBlockMode (bool block)
{
	static ConstString func = "basar.cmnutil.TcpSocket.setBlockingMode()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	u_long nonblock = block ? 0 : 1;

	if (SOCKET_ERROR == ioctlsocket(m_Socket, FIONBIO, &nonblock))
	{
		VarString msg;
		msg.format("::ioctlsocket(%d, FIONBIO, %d) failed with %d", 
			       m_Socket,
				   nonblock,
				   getLastError());
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

#else

	// read file status flags
	int file = fcntl(m_Socket, F_GETFL, 0);
	
	if ( 0 > file )
	{
		VarString msg;
		msg.format("::fcntl(%d, F_GETFL, 0) failed with %d", 
			       m_Socket,
				   getLastError());
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	if (block)
		file &= ~O_NONBLOCK; 
	else
		file |= O_NONBLOCK;

	// set file status flags
	if( -1 == fcntl(m_Socket, F_SETFL, file) ) 
	{ 
		VarString msg;
		msg.format("::fcntl(%d, F_SETFL, %d) failed with %d", 
			       m_Socket,
				   file,
				   getLastError());
		throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

#endif
}

//------------------------------------------------------------------------------
//! \return non critical reason why connection didn't succeed
//! \throw TcpSocketException
TcpSocket::RetCodeEnum TcpSocket::connect()
{
	static ConstString func  = "basar.cmnutil.TcpSocket.connect()";
	
	RetCodeEnum   eRet       = TCP_SUCCESS;
	::sockaddr_in addrRemote = { 0, 0, 0, { 0 }, {0} };

	addrRemote.sin_family = s_NET_ADRESSFAMILY;
	addrRemote.sin_port   = m_ServicePort;
	memcpy(&addrRemote.sin_addr, &m_RemoteHostAddr, sizeof(m_RemoteHostAddr));

	if ( SOCKET_ERROR == ::connect(m_Socket, 
								   reinterpret_cast<const sockaddr *>(&addrRemote), 
								   static_cast     <int             >(sizeof(addrRemote))) ) 
	{
		switch (getLastError())
		{
		case CONN_TIMEDOUT:
			eRet = TCP_OFFLINE;
			break;

		case CONN_REFUSED:
			eRet = TCP_CONNREFUSED;
			break;

		case CONN_INPROGRESS:
			eRet = TCP_CONNINPROGRESS; 
			break;

		default:
			{
				VarString msg;
				msg.format("::connect socket %d to (%d.%d.%d.%d:%d) failed with %d", 
							m_Socket,
							static_cast<UInt8>(addrRemote.sin_addr.s_addr >>  0),
							static_cast<UInt8>(addrRemote.sin_addr.s_addr >>  8),
							static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 16),
							static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 24),
							ntohs(addrRemote.sin_port),
							getLastError());
				throw TcpSocketException(ExceptInfo(func, msg, __FILE__, __LINE__));
			}
		}
	}
	else
		m_State = STATE_CONNECT;

	log4cplus::LogLevel level = ((eRet == TCP_SUCCESS) || (eRet == TCP_CONNINPROGRESS))
						      ? log4cplus::DEBUG_LOG_LEVEL 
							  : log4cplus::WARN_LOG_LEVEL;

	if (LoggerPool::tcpSocket().isEnabledFor(level))
	{
		::sockaddr_in addrLocal    = { 0, 0, 0, { 0 }, {0} };
		socklen_t     addrLocalLen = sizeof(addrLocal);
        VarString     msg;

		if ( SOCKET_ERROR == ::getsockname(m_Socket                                , 
										   reinterpret_cast<sockaddr *>(&addrLocal), 
										   &addrLocalLen                           ) )
		{
			VarString err;
            err.format ( "::getsockname(socket %d) failed with %d",
						m_Socket,
						getLastError());
            LoggerPool::tcpSocket().forcedLog(level, err, __FILE__, __LINE__);
        }

        msg.format("connect(): ::connect socket %d from (%d.%d.%d.%d:%d) to "
            "(%d.%d.%d.%d:%d) returns %d",
            m_Socket,
            static_cast<UInt8>(addrLocal.sin_addr.s_addr >> 0),
            static_cast<UInt8>(addrLocal.sin_addr.s_addr >> 8),
            static_cast<UInt8>(addrLocal.sin_addr.s_addr >> 16),
            static_cast<UInt8>(addrLocal.sin_addr.s_addr >> 24),
            ntohs(addrLocal.sin_port),
            static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 0),
            static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 8),
            static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 16),
            static_cast<UInt8>(addrRemote.sin_addr.s_addr >> 24),
            ntohs(addrRemote.sin_port),
            eRet);

		LoggerPool::tcpSocket().forcedLog(level, msg, __FILE__, __LINE__);
	}

	return eRet;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
void TcpSocket::enable()
{
	m_State = STATE_ENABLE;

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("enable(): successful");
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::disable(bool halfduplex)
{
	if (m_State >= STATE_ENABLE)
	{
		if (SOCKET_ERROR == ::shutdown(m_Socket, halfduplex ? SD_SEND : SD_BOTH))
		{
			VarString msg;
			msg.format ( "::shutdown(%d) failed with %d", 
				         m_Socket, 
						 getLastError());
			throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.disable()", 
												msg, 
												__FILE__, __LINE__));
		}

		// empty buffers
		const static Int32 BUFLEN = 1024;
		char               buffer[BUFLEN];

		while ( 0 < ::recv(m_Socket, buffer, BUFLEN, 0) )
		{
			// empty queue
			//assert(0);
		}

		m_State = STATE_CONNECT;

		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "disable(): ::shutdown(socket %d, %s) successful", 
				        m_Socket, 
						halfduplex ? "SD_SEND" : "SD_BOTH");
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::disconnect()
{
    if (INVALID_SOCKET == m_Socket)
	{
        BLOG_DEBUG_STREAM(
            LoggerPool::tcpSocket(),
            "disconnect(): socket is invalid -> nothing to do");

        return;
    }

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
    if (SOCKET_ERROR == ::closesocket(m_Socket))
#else
    if (SOCKET_ERROR == ::close(m_Socket))
#endif
	{
		VarString msg;
		msg.format( "closeSocket(%d) failed with %d", 
				        m_Socket, 
						getLastError());
		throw TcpSocketException(
                ExceptInfo(
                    "basar.cmnutil.TcpSocket.disconnect()", 
					msg, 
					__FILE__, __LINE__));
	}

    BLOG_DEBUG_STREAM(
        LoggerPool::tcpSocket(),
        "disconnect(): closeSocket(socket "
        << m_Socket
        << ") successful");

		m_Socket = INVALID_SOCKET;
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::cleanup()
{
	if (m_State >= STATE_STARTUP)
	{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

		if (SOCKET_ERROR == ::WSACleanup())
		{	
			VarString msg;
			msg.format("::WSACleanup() failed with %d", getLastError());
			throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.cleanup()", 
												msg, 
												__FILE__, __LINE__));
		}

#endif

		m_State = STATE_INITIAL;

		if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "cleanup(): successful");
			LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												  msg, 
												  __FILE__, __LINE__);
		}
	}
}

//------------------------------------------------------------------------------
//!	\return platform specific error code after failure
//! \throw  no-throw
Int32 TcpSocket::getLastError() const
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	return ::WSAGetLastError();
#else
	return errno;
#endif
}

//------------------------------------------------------------------------------
#ifdef TCPSOCKET_EXTENDED

//------------------------------------------------------------------------------
//! \throw TcpSocketException
//!
void TcpSocket::config() const
{
	static const BOOL     reuse  = TRUE;
	static const ::LINGER linger = { 1, 0 };

	if ( ( SOCKET_ERROR == ::setsockopt(m_Socket                               , 
										SOL_SOCKET                             , 
										SO_REUSEADDR                           , 
										reinterpret_cast<const char *>(&reuse) , 
										sizeof(reuse)                          ) ) ||
		 ( SOCKET_ERROR == ::setsockopt(m_Socket                               , 
										SOL_SOCKET                             , 
										SO_LINGER                              , 
										reinterpret_cast<const char *>(&linger), 
										sizeof(linger)                         ) )   )
	{
		VarString msg;
		msg.format("::setsockopt(%d, SOL_SOCKET, SO_REUSEADDR | SO_LINGER) failed with %d",
				
			       m_Socket,
				   getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.config()", 
					                        msg, 
											__FILE__, __LINE__));
	}

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("config(): ::setsockopt(%d, SOL_SOCKET, SO_REUSEADDR | SO_LINGER) "
				   "successful",
				   m_Socket);
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											   msg, 
											   __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  TcpSocketException
//!
void TcpSocket::bindLocal()
{
	static const UInt16 LOCALPORT_MIN = 721;
	static const UInt16 LOCALPORT_MAX = 731;

	::sockaddr_in       addrLocal     = { 0 };

	addrLocal.sin_family = s_NET_ADRESSFAMILY;
	memcpy(&addrLocal.sin_addr, &m_LocalHostAddr, sizeof(m_LocalHostAddr));

    for ( UInt16 port = LOCALPORT_MIN ; port <= LOCALPORT_MAX ; ++port) 
	{
		addrLocal.sin_port = htons(port);

		if ( SOCKET_ERROR != ::bind(m_Socket 								      , 
					                reinterpret_cast<const sockaddr *>(&addrLocal), 
									sizeof(addrLocal)                             ) )
			break;		// found local port 
    }

    if ( port > LOCALPORT_MAX ) 
	{
		VarString msg;
		msg.format( "::bind() socket %d to %d.%d.%d.%d:(%d-%d) failed with %d", 
			         m_Socket,
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b1),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b2),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b3),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b4),
					 LOCALPORT_MIN,
					 LOCALPORT_MAX,
					 getLastError());
		throw TcpSocketException(ExceptInfo("basar.cmnutil.TcpSocket.bindLocal()", 
											msg, 
											__FILE__, __LINE__));
    }

	if (LoggerPool::tcpSocket().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "bindLocal(): ::bind() socket %d to %d.%d.%d.%d:%d successful", 
			         m_Socket,
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b1),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b2),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b3),
					 static_cast<UInt8>(addrLocal.sin_addr.S_un.S_un_b.s_b4),
					 ::ntohs(addrLocal.sin_port));
		LoggerPool::tcpSocket().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------

#endif	// TCPSOCKET_EXTENDED

//----------------------------------------------------------------------------
}	//cmnutil
}	//basar

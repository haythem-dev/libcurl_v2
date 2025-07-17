//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for socket
 *  \author Michael Eichenlaub
 *  \date   04.12.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_SOCKET_H
#define GUARD_LIBBASARCMNUTIL_SOCKET_H

//------------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
typedef SOCKET Socket;
#else
typedef basar::Int32 Socket;
#endif

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//############################################################################
//   exception handler
//############################################################################

//----------------------------------------------------------------------------
//! \brief tcp socket exception class; thrown if errors with socket handling occur
//! \n final class
class TcpSocketException : public basar::Exception
{
public:
	//! constructs exception object and initialises exception type		\n no-throw
	LIBBASARCMNUTIL_API		TcpSocketException	(const ExceptInfo & sInfo	//!< exception info struct
												)
												: basar::Exception(sInfo, "TcpSocketException")
												{};
private:
							TcpSocketException	();							//!< forbidden
};

//---------------------------------------------------------------------------
//! class for tcp socket	\n final class
class TcpSocket
{
public:	

	enum RetCodeEnum			//!  socket's return values
	{
		TCP_SUCCESS        = 0,	//!< socket communication OK

		TCP_OFFLINE	       = 1,	//!< socket offline/down
		TCP_CONNREFUSED    = 2,	//!< listener rejected attempts to connect: 
								//!< -> maybe too many client connections, so try again later
		TCP_CONNINPROGRESS = 3,	//!< non-blocking socket's connection cannot completed immediately

		TCP_RETCODE_MAX			//!< maximum return codes
	};


	//! constructor for socket to remote host if remote port is known; initializes use of socket library (Win32)			\n no-throw
	LIBBASARCMNUTIL_API		TcpSocket		(const VarString & rRemoteHost,			//!< remote host name or ip address ("a.b.c.d")
								             Int16			   servicePort			//!< remote service port the daemon listens to
									        );
	//! constructor for socket to remote host if remote service name is known; initializes use of socket library (WIN32)	\n no-throw
	LIBBASARCMNUTIL_API		TcpSocket		(const VarString & rRemoteHost,			//!< remote host name or ip address ("a.b.c.d")
								             const VarString & rServiceName			//!< remote service name the daemon listens to
									        );
	//! destructor: terminate use of socket library (Win32)	\ no-throw
	LIBBASARCMNUTIL_API	   ~TcpSocket		();

	//! create socket, connect to remote host		\n no-throw
	LIBBASARCMNUTIL_API	
	RetCodeEnum				open			();
	//! shutdown and close socket					\n no-throw
	LIBBASARCMNUTIL_API 
	void					close			(bool			   halfduplex = false	//!< shutdown behaviour: disallow only send or send+receive
											);

	//! carefully send data to remote host			\n throw TcpSocketException
	LIBBASARCMNUTIL_API 
	void					send			(ConstString       buffer,				//!< data to send
											 BULong			   buflen				//!< length of data buffer in bytes	
											) const;
	//! receive data from remote host				\n throw TcpSocketException
	LIBBASARCMNUTIL_API 
	BLong					receive			(char * const      buffer,				//!< buffer for received data
											 BULong			   buflen				//!< length of data buffer in bytes	
											) const;

	//! is socket created, connected and enabled?	\n no-throw
	LIBBASARCMNUTIL_API 
	bool					isOpen			() const;

	//! is socket available? is server process listening at remote host/port?	
	//! \n throw  TcpSocketException
	LIBBASARCMNUTIL_API 
	bool					isAvail			(Int32			   timeout				//!< timeout in seconds
											);

	//! get local host name							\n no-throw
	LIBBASARCMNUTIL_API 
	const VarString &		getLocalHostName() const;

	//! get internal socket descriptor							\n no-throw
	LIBBASARCMNUTIL_API 
	Socket					getSocketDescriptor() const;

private:

	//--------------------------------------------------------------------------

	enum StateEnum			//!  socket state  
	{
		STATE_INITIAL,		//!< initial: descriptor not valid
		STATE_STARTUP,		//!< use of socket library (Win32) initialized
		STATE_SOCKET,		//!< valid socket descriptor
		STATE_CONNECT,		//!< connection established
		STATE_ENABLE		//!< connection enabled
	};

	//--------------------------------------------------------------------------

				TcpSocket			();                       	//!< forbidden
				TcpSocket			(const TcpSocket & rCopy);	//!< forbidden
	TcpSocket & operator =			(const TcpSocket & rCopy);	//!< forbidden

	//! initialize use of socket library (Win32), determine service port, local/remote host name/address	\n no-throw
	void		init				(const VarString & rRemoteHost,			//!< remote host name or ip address ("a.b.c.d")
									 const VarString & rServiceName = ""	//!< remote service name: empty if service port is given
									);
	//!  shutdown and close socket, terminate use of socket library (Win32)	\n no-throw 
	void		shutdown			();

	//! initialize use of socket library (Win32)	\n throw TcpSocketException
	void		startup				();
	//! determine local host name/address	\n throw TcpSocketException
	void		getLocalHost		();
	//! determine remote host address if host name is given	\n throw TcpSocketException
	void		getRemoteHost		(const VarString & rName	//!< remote host name or ip address ("a.b.c.d")
									);
	//! determine remote host service port	\n throw TcpSocketException 
	void 		getServicePort		(const VarString & rServiceName	//!< remote service name
		        					);
	//! create stream socket (i.e. get socket descriptor)	\n throw TcpSocketException
	void		create				();
	//! set socket's blocking mode \n throw TcpSocketException
	void		setBlockMode		(bool block	//!< socket's blocking/non-blocking mode
									);
	//! is non-blocking connect completed (was in progress) \n throw TcpSocketException
	bool		isConnectComplete	(Int32			   timeout				//!< timeout in seconds
									) const;
	//! establish connection to remote host	\n throw TcpSocketException
	RetCodeEnum	connect				();
	//! enable connection	\n throw no-throw
	void		enable				();
	//! disable send/receive on socket, empty receive buffer	\n throw TcpSocketExecption
	void		disable				(bool halfduplex	//!< disallow only send or send+receive
									);
	//! close socket \n throw TcpSocketExecption
	void		disconnect			();
	//! terminate use of socket library	(Win32) \n throw TcpSocketExecption
	void		cleanup				();

	//! wrapper for getting platform specific error code	\n no-throw
	Int32		getLastError		() const;

	//--------------------------------------------------------------------------

	static Int16	s_NET_ADRESSFAMILY;	//!< network address family

	StateEnum		m_State;			//!< socket state

	VarString		m_LocalHostName;	//!< local  host name
	ULong32			m_LocalHostAddr;	//!< local  host ip address (network byte order)
	ULong32			m_RemoteHostAddr;	//!< remote host ip address (network byte order)
	Int16			m_ServicePort;		//!< remote service port the remote daemon listens to (network byte order)
	Socket			m_Socket;			//!< socket descriptor

};

//-----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD


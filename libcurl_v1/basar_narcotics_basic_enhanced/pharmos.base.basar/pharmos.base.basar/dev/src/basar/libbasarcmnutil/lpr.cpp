//----------------------------------------------------------------------------
/*! \file
 *  \brief  line printer daemon protocol (LPD) class
 *  \author Michael Eichenlaub
 *  \date   22.08.2006
 */
//----------------------------------------------------------------------------

#include "lpr.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_print.h"

#include <string.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
const Int16 Lpr::s_ServicePort      = 515;						//!< default service port number for service printer/tcp

UInt16		Lpr::s_JobId			= Lpr::initNextJobId();		//!< initialize first job id (by random)

ConstString	Lpr::s_PREFIX_CONTROL	= "cfA";					//!< prefix for LPD's control file
ConstString	Lpr::s_PREFIX_DATA		= "dfA";					//!< prefix for LPD's data file
ConstString	Lpr::s_SPACE			= " ";						//!< definition for LPD's space

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
Lpr::Lpr ( const VarString & rRemoteHost          ,
		   const VarString & rQueue               ,
           Int16             servicePort /* = s_ServicePort */)
         : m_Socket        (rRemoteHost, 
							servicePort  ),
		   m_Queue         (rQueue       ),
	       m_pDataProvider (0            )
{
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
Lpr::Lpr ( const VarString      & rRemoteHost  ,
		   const VarString      & rQueue       ,
		   IPrintDataProvider   & rDataProvider,
           Int16                  servicePort /* = s_ServicePort */)
         : m_Socket        (rRemoteHost   , 
							servicePort   ),
		   m_Queue         (rQueue        ),
	       m_pDataProvider (&rDataProvider)
{
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
Lpr::~Lpr()
{
}

//------------------------------------------------------------------------------
//! \return printer's return code
//! \throw  no-throw
TcpSocket::RetCodeEnum Lpr::execute()
{
	TcpSocket::RetCodeEnum eRet;

	if (TcpSocket::TCP_SUCCESS != (eRet = m_Socket.open()))
		return eRet;

	recvPrintJob   ();
	recvControlFile();
	recvDataFile   ();

	m_Socket.close ();

	setNextJobId   ();

	return TcpSocket::TCP_SUCCESS;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Lpr::recvPrintJob()
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "recvPrintJob()");

	VarString command;
	command.format(  "%c%s\n", 
		             PRINT_RECEIVE_JOB, 
					 m_Queue.c_str());

	sendCommand(command);
	getAck     ();

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "recvPrintJob(%s): successful", m_Queue.c_str());
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
// \throw LPDException
//!
void Lpr::recvControlFile()
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "recvControlFile()");

	static const VarString::size_type MAXLEN_HOSTNAME =  31;							// RFC 1179
	static const size_t               MAXLEN_USERNAME =  31;							// RFC 1179
	static ConstString                INPUTFILE       = "basar::IPrintDataProvider";
	assert(131 >= strlen(INPUTFILE));													// RFC 1179

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	static ConstString ENV_USERNAME    = "USERNAME";						// logged in system user
#else
	static ConstString ENV_USERNAME    = "LOGNAME";							// logged in system user
#endif

	static ConstString USERNAME        = ::getenv(ENV_USERNAME);

	// check environment
	if (0 == USERNAME)
	{
		VarString msg;
		msg.format("::getenv(%s) failed", 
				   ENV_USERNAME);
		throw LPDException(ExceptInfo("basar.cmnutil.Lpr.recvControlFile()", msg, __FILE__, __LINE__));
	}

	VarString hostname = m_Socket.getLocalHostName();
	VarString filename;
	filename.format( "%03d%s", s_JobId, hostname.c_str());
		            

	VarString controlfile;
	controlfile.format ( "H%-.*s\n"			// local host name		(length : <=  31)	RFC 1179
						 "P%-.*s\n"			// user name			(length : <=  31)	RFC 1179
						 "f%s%s\n"			// print formatted file	(length : ?     )	RFC 1179
						 "U%s%s\n"			// unlink file			(length : ?     )	RFC 1179
						 "N%s\n",			// file name			(length : <= 131)	RFC 1179
						 (hostname.length() <= MAXLEN_HOSTNAME) ? hostname.length() : MAXLEN_HOSTNAME, hostname.c_str(),
						 (strlen(USERNAME)  <= MAXLEN_USERNAME) ? strlen(USERNAME)  : MAXLEN_USERNAME, USERNAME,
						 s_PREFIX_DATA,
						 filename.c_str(),
						 s_PREFIX_DATA,
						 filename.c_str(), 
						 INPUTFILE);

	VarString command;
	command.format ( "%c%d%s%s%s\n", 
		             PRINT_RECEIVE_CONTROLFILE,
					 controlfile.length(),
					 s_SPACE,
					 s_PREFIX_CONTROL,
					 filename.c_str());


	sendCommand    (command);		// receive control file
	getAck         ();				// ok
	sendControlData(controlfile);	// receive control file 's data
	sendComplete   ();				// all data sent
	getAck         ();				// ok


	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "recvControlFile(): successful");
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//!        total number of bytes to be printed must be determined 
//!        before data are sent to printer: \n
//!        -> all data must be collected/selected \n
//!        -> take care of memory buffer when printing huge amount of data ( &gt; 10 MB )
//! \throw LPDException
void Lpr::recvDataFile()
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "recvDataFile()");
	assert(0 == m_CollSendData.size());

	while (!m_CollInput.empty())						// data appended: first time by trigger
	{
		// copy callback's data into internal collection (optimized for huge data amount)
		while (!m_CollInput.empty())
		{
			m_CollSendData.push_back(m_CollInput.front());
			m_CollInput.pop();
		}

		if (m_pDataProvider)
			m_pDataProvider->onSendData(m_CollInput);		// give caller chance to append data
	}

	CollSendData::size_type bytes = m_CollSendData.size();

	if (0 >= bytes)
		throw LPDException(ExceptInfo("basar.cmnutil.Lpr.recvDataFile()", "no data to print!", __FILE__, __LINE__));

	VarString command;
	command.format ( "%c%d%s%s%03d%s\n", 
		             PRINT_RECEIVE_DATAFILE,
					 bytes,
					 s_SPACE,
					 s_PREFIX_DATA,
					 s_JobId, 
					 m_Socket.getLocalHostName().c_str());


	sendCommand  (command);			// receive data file
	getAck       ();				// ok

	sendPrintData(m_CollSendData);	// receive data file 's data
	m_CollSendData.clear();

	sendComplete ();				// all data sent
	getAck       ();				// ok

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "recvDataFile(): successful, m_CollSendData.size() = %d", bytes);
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Lpr::sendPrintData(CollSendData & rCollData) const
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "sendPrintData()");

	static const Int32				MAXBUFLEN = 1460;						//	  1500 Byte ethernet MTU 
																			//	-   20 Byte IP header 
																			//	-   20 Byte TCP Header 
																			//	= 1460 byte lpd data -> less packets and overhead
	CollSendData::size_type			size      = rCollData.size();
	CollSendData::iterator			itCur     = rCollData.begin();
	CollSendData::const_iterator	itEnd     = rCollData.end();

	char							buffer[MAXBUFLEN];
	char *							pBuf;
	Int32							buflen;

	do
	{
		// copy data from collection to send buffer
		pBuf   = buffer;
		buflen = 0;

		while ( (itCur != itEnd)    &&		// more data   ?
			    (buflen < MAXBUFLEN)   )	// buffer full ?
		{
			*pBuf = *itCur;

			++itCur ;
			++pBuf  ;
			++buflen;
		}

		m_Socket.send(buffer, buflen);

		if (LoggerPool::lprData().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "sendPrintData(): sent %d bytes:\n"
				        "%-.*s", 
						buflen,
						buflen,
						buffer);
			LoggerPool::lprData().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}


	} while ( itCur != itEnd );

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendPrintData(): sent %d bytes for print data", size);
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Lpr::sendControlData(const VarString & rData) const
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "sendControlData()");

	m_Socket.send(rData.c_str(), static_cast<Int32>(rData.length()));

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendControlData(): sent %d bytes for control data:\n"
			        "%-.*s", 
					rData.length(),
					rData.length() - 1,		// without last letter (linefeed)
					rData.c_str ());
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Lpr::sendCommand(const VarString & rCommand) const
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "sendCommand()");

	m_Socket.send(rCommand.c_str(), static_cast<Int32>(rCommand.length()));

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendCommand(): sent %d bytes for command %02d:\n"
			        "%-.*s", 
			        rCommand.length(),
			        *(rCommand.c_str()),
			        rCommand.length() - 2,	// without first (command) and last letter (linefeed)
			        rCommand.c_str() + 1);
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Lpr::sendComplete() const
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "sendComplete()");

	static const Int32 BUFLEN         = 1;
	static const char  BUFFER[BUFLEN] = { 0 };

	m_Socket.send(BUFFER, BUFLEN);

	if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendComplete(): sent %d byte for \"file is complete\":\n"
			        "0x%02x", 
					BUFLEN,
					*BUFFER);
		LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw LPDException
//!
void Lpr::getAck()
{
	BLOG_TRACE_METHOD(LoggerPool::lpr(), "getAck()");

	const static Int32 BUFLEN         = 1024;
	char               buffer[BUFLEN] = { 0 };
	ssize_t            bytesread      = m_Socket.receive(buffer, BUFLEN);

	switch (bytesread)
	{
	case 1:
		if (buffer[0] != '\0')
		{
			VarString msg;
			msg.format("::remote host sent error %d", buffer[0]);
			throw LPDException(ExceptInfo("basar.cmnutil.Lpr.getAck()", msg, __FILE__, __LINE__));
		}

		if (LoggerPool::lpr().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "getAck(): received acknowledge == 0x00");
			LoggerPool::lpr().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		break;

	default:
		{
			if (BUFLEN <= bytesread)	// buffer too small
			{
				buffer[BUFLEN - 1] = '\0';
				assert(0);
			}
			else
				buffer[bytesread]  = '\0';

			VarString msg;
			msg.format("recveived %d bytes -> %s", 
				       bytesread, 
					   buffer);
			throw LPDException(ExceptInfo("basar.cmnutil.Lpr.getAck()", msg, __FILE__, __LINE__));
		}
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw 
//!	\return job id
UInt16 Lpr::initNextJobId()
{
	static const UInt32 NOW = static_cast<UInt32>(::time(0));

	// seed random-number generator with current time so that numbers will be different every time we run
	srand(NOW);

	setNextJobId();

	return s_JobId;
}

//----------------------------------------------------------------------------
//! job id must be between 0 and 999.
//! It is incremented every call but will be wrapped to 0 when larger than 999
//! \throw no-throw
void Lpr::setNextJobId()
{
	static const UInt16 WRAP = 1000;		// wrap around

	//++s_JobId;

	s_JobId  = static_cast<UInt16>(rand());
	s_JobId %= WRAP;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return reference to lpr's input collection
CollStdIo & Lpr::getInput()
{
	return m_CollInput;
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

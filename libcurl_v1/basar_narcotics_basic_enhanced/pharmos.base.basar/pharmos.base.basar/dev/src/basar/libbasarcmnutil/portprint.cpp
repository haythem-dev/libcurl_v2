//----------------------------------------------------------------------------
/*! \file
 *  \brief  port print (jetdirect) class
 *  \author Michael Eichenlaub
 *  \date   31.08.2006
 */
//----------------------------------------------------------------------------

#include "portprint.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_print.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
const Int16 PortPrint::s_ServicePort = 9100;		//!< default jetdirect service port number

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
PortPrint::PortPrint ( const VarString & rRemoteHost,
					   Int16             servicePort /* = s_ServicePort */)
					 : m_Socket       (rRemoteHost,
					                   servicePort),
					   m_pDataProvider(0           )
{
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
PortPrint::PortPrint ( const VarString    & rRemoteHost  ,
		               IPrintDataProvider & rDataProvider,
					   Int16                servicePort /* = s_ServicePort */)
					 : m_Socket       (rRemoteHost,
					                   servicePort),
					   m_pDataProvider(&rDataProvider)
{
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
PortPrint::~PortPrint()
{
}

//------------------------------------------------------------------------------
//! \return printer's return code
//! \throw  no-throw
TcpSocket::RetCodeEnum PortPrint::execute()
{
	TcpSocket::RetCodeEnum eRet;

	if (TcpSocket::TCP_SUCCESS != (eRet = m_Socket.open()))
		return eRet;

	sendColl      ();

	m_Socket.close(true);	// halfduplex shutdown(SD_SEND)

	return TcpSocket::TCP_SUCCESS;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void PortPrint::sendColl()
{
	BLOG_TRACE_METHOD(LoggerPool::portPrint(), "sendColl()");

	for (;;)
	{
		if (m_pDataProvider)
			m_pDataProvider->onSendData(m_CollInput);		// give caller chance to append data

		if (!m_CollInput.empty())							// data appended
			sendData();	
		else
			break;
	}

	if (LoggerPool::portPrint().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendColl(): successful");
		LoggerPool::portPrint().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void PortPrint::sendData()
{
	BLOG_TRACE_METHOD(LoggerPool::portPrint(), "sendData()");

	static const Int32				MAXBUFLEN = 1460;						//	  1500 Byte ethernet MTU 
																			//	-   20 Byte IP header 
																			//	-   20 Byte TCP Header 
																			//	= 1460 byte lpd data -> less packets and overhead
	CollStdIo::size_type			size      = m_CollInput.size();
	char							buffer[MAXBUFLEN];
	char *							pBuf;
	Int32							buflen;

	while ( !m_CollInput.empty() )
	{
		// copy data from collection to send buffer
		pBuf   = buffer;
		buflen = 0;

		while ( !m_CollInput.empty() &&		// more data   ?
			    (buflen < MAXBUFLEN)    )	// buffer full ?
		{
			*pBuf = m_CollInput.front();

			m_CollInput.pop();
			++pBuf  ;
			++buflen;
		}

		m_Socket.send(buffer, buflen);

		if (LoggerPool::portPrintData().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format( "sendPrintData(): sent %d bytes:\n"
				        "%-.*s", 
						buflen,
						buflen,
						buffer);
			LoggerPool::portPrintData().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
	}

	if (LoggerPool::portPrint().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendPrintData(): sent %d bytes for print data", size);
		LoggerPool::portPrint().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \return reference to PortPrint's input collection
//! \throw  no-throw 
CollStdIo & PortPrint::getInput()
{
	return m_CollInput;
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

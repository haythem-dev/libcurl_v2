//----------------------------------------------------------------------------
/*! \file
 *  \brief  SMTP class
 *  \author Michael Eichenlaub
 *  \date   04.12.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_email.h"

#include "libbasarcmnutil_socket.h"
#include "libbasarcmnutil_logging.h"
#include "loggerpool.h"

#include <string.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
Smtp::Smtp	( const VarString & rRemoteHost,
			  const Int16       servicePort /* = SMTP_PORT_DEFAULT */)
			: m_pSocket(new TcpSocket(rRemoteHost, servicePort))
{
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
Smtp::~Smtp()
{
}

//------------------------------------------------------------------------------
//! \return SMTP's return code
//! \throw  no-throw 
Smtp::RetCodeEnum Smtp::connect()
{
	// --- socket stuff

	TcpSocket::RetCodeEnum eRet;

	// socket connection
	if (TcpSocket::TCP_SUCCESS != (eRet = m_pSocket->open()))
	{
		static const RetCodeEnum retCodeMap[TcpSocket::TCP_RETCODE_MAX] = 
		{
			SMTP_SUCCESS,
			SMTP_OFFLINE,
			SMTP_CONNREFUSED,
			SMTP_CONNINPROGRESS
		};

		assert(TcpSocket::TCP_RETCODE_MAX == sizeof(retCodeMap)/sizeof(retCodeMap[0]));
		
		return retCodeMap[eRet];
	}


	// --- server communication

	assert(m_pSocket->isOpen());

	static ConstString REPLY_WELCOME_SUCCESS = "220";		// SMTP server's expected welcome reply          (first part)
	static ConstString REPLY_GENERIC_SUCCESS = "250";		// SMTP server's expected reply for open command (first part)
	static ConstString CMD_OPEN              = "HELO %s";	// SMTP command for opening channel
	VarString          cmd;
	
	cmd.format(CMD_OPEN, m_pSocket->getLocalHostName().c_str());

	scanResponse(REPLY_WELCOME_SUCCESS);		// scan for server's welcome

	sendCommand (cmd.c_str()          );		// open SMTP channel and send client name
	scanResponse(REPLY_GENERIC_SUCCESS);		// scan for server's reply


	return SMTP_SUCCESS;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void Smtp::disconnect()
{
	assert(m_pSocket->isOpen());

	static ConstString CMD_CLOSE           = "QUIT";	// SMTP close command
	static ConstString REPLY_CLOSE_SUCCESS = "221";		// SMTP server's expected reply for close command (first part)

	// communication with server
	sendCommand (CMD_CLOSE          );					// close SMTP channel
	scanResponse(REPLY_CLOSE_SUCCESS);					// scan for server's reply

	// socket stuff
	m_pSocket->close();
}

//------------------------------------------------------------------------------
//! \return SMTP's return code
//! \throw  no-throw
Smtp::RetCodeEnum Smtp::send(const MailMessage & rMsg)
{
	assert(m_pSocket->isOpen());

	static ConstString CMD_MAIL                 = "MAIL FROM:<%s>";	// SMTP mail command
	static ConstString CMD_RCPT                 = "RCPT TO:<%s>";	// SMTP recipient command
	static ConstString CMD_DATA                 = "DATA";			// SMTP data command
	static ConstString CMD_END_DATA             = ".";				// SMTP end of mail data command
	static ConstString REPLY_GENERIC_SUCCESS    = "250";			// SMTP server's expected reply for generic success (first part of response)
	static ConstString REPLY_ADDRESS_UNROUTABLE = "550";			// SMTP server's possible reply for RCPT command    (first part of response)
	static ConstString REPLY_DATA_SUCCESS       = "354";			// SMTP server's expected reply for data success    (first part of response)
	VarString          buf;

	// is message consistent?
	rMsg.checkSettings();		

    const UInt32 recipientsCount = rMsg.getCountTo() + rMsg.getCountCC() + rMsg.getCountBCC();

    const log4cplus::Logger& logger = LoggerPool::smtp();

	// FROM
	buf.format(CMD_MAIL, rMsg.getFrom().c_str());
	sendCommand (buf.c_str()          );						// send mail FROM
	scanResponse(REPLY_GENERIC_SUCCESS);						// scan for server's reply

	// TO
	for (UInt32 i = 0, lim = rMsg.getCountTo(); i < lim; ++i)
	{
        const basar::VarString& recipient = rMsg.getTo(i);

		buf.format(CMD_RCPT, recipient.c_str());
		sendCommand (buf.c_str());          									// send recipient TO
		if (!scanResponse(REPLY_GENERIC_SUCCESS, REPLY_ADDRESS_UNROUTABLE))		// scan for server's reply
        {
            BLOG_DEBUG_STREAM(logger, "server rejected address <" << recipient << ">, skipping and continuing");
            m_RejectedRecipients.push_back(recipient);
        }
	}

	// CC
	for (UInt32 i = 0, lim = rMsg.getCountCC(); i < lim; ++i)
	{
        const basar::VarString& recipient = rMsg.getCC(i);

		buf.format(CMD_RCPT, recipient.c_str());
		sendCommand (buf.c_str());          									// send recipient CC
		if (!scanResponse(REPLY_GENERIC_SUCCESS, REPLY_ADDRESS_UNROUTABLE))		// scan for server's reply
        {
            BLOG_DEBUG_STREAM(logger, "server rejected address <" << recipient << ">, skipping and continuing");
            m_RejectedRecipients.push_back(recipient);
        }
	}

	// BCC
	for (UInt32 i = 0, lim = rMsg.getCountBCC(); i < lim; ++i)
	{
        const basar::VarString& recipient = rMsg.getBCC(i);

		buf.format(CMD_RCPT, recipient.c_str());
		sendCommand (buf.c_str());          									// send recipient BCC
		if (!scanResponse(REPLY_GENERIC_SUCCESS, REPLY_ADDRESS_UNROUTABLE))		// scan for server's reply
        {
            BLOG_DEBUG_STREAM(logger, "server rejected address <" << recipient << ">, skipping and continuing");
            m_RejectedRecipients.push_back(recipient);
        }
	}

    if (m_RejectedRecipients.size() == recipientsCount)
    {
        //sending of mail failed as server rejected each and every address
        return SMTP_UNROUTEABLE_ADDRESS;
    }

	// DATA
	sendCommand (CMD_DATA);									// send data
	scanResponse(REPLY_DATA_SUCCESS   );					// scan for server's reply

	// HEADER
	buf = rMsg.getHeader();
	sendData(buf.c_str());

	// BODY
	buf = rMsg.getBody();
	sendData(buf.c_str());

	// END OF DATA
	sendCommand (CMD_END_DATA         );					// send end of mail data
	scanResponse(REPLY_GENERIC_SUCCESS);					// scan for server's reply

    if (false == m_RejectedRecipients.empty())
    {
        return SMTP_SUCCESS_WITH_REFUSED_RECIPIENTS;
    }

	return SMTP_SUCCESS;
}

bool Smtp::hasRejectedRecipients() const
{
    return (false == m_RejectedRecipients.empty());
}

const CollBString & Smtp::getRejectedRecipients() const
{
    return m_RejectedRecipients;
}

//------------------------------------------------------------------------------
//! \retval true  correct response
//! \retval false not expected, but uncritical response.
//! \throw  SMTPException (not expected and critical response)
bool Smtp::scanResponse	(ConstString reply,
						 ConstString optional /* = 0 */)
{
	static const Int32 BUFLEN           = 1024;
	char               buffer[BUFLEN]   = { 0 };
	ssize_t            bytesread        = m_pSocket->receive(buffer, BUFLEN);
	bool			   flagExcept       = false;
	bool			   flagReturn       = true;


	if ( (bytesread  < static_cast<Int32>(strlen(reply))    ) ||					// too few chars
		 (0         != strncmp(buffer, reply, strlen(reply)))    )					// not expected reply
	{
		if (0 == optional)			// no optional reply given
			flagExcept = true;		// => exception
		else						// check optional reply
		{
			if ( (bytesread  < static_cast<Int32>(strlen(optional))       ) ||		// too few chars
			     (0         != strncmp(buffer, optional, strlen(optional)))    )	// not expected optional reply
				flagExcept = true;	// => exception
			else
				flagReturn = false;	// => return false
		}
	}

	if (flagExcept)
	{
		VarString msg;
		msg.format("expected reply <%s>, optional reply <%s>; received <%.*s>", 
					reply,
					(0 == optional) ? "NULL" : optional,
					bytesread - strlen("\r\n"), buffer);
		throw SMTPException(ExceptInfo("basar.cmnutil.Smtp.scanResponse()", msg, __FILE__, __LINE__));
	}

	if (LoggerPool::smtp().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "scanResponse() returns %s:\n"
					"<%.*s>", 
					flagReturn ? "true" : "false",
					bytesread - strlen("\r\n"), buffer);
		LoggerPool::smtp().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return flagReturn;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Smtp::sendCommand(ConstString command)
{
	static ConstString CMD_TERM = "\r\n";	// SMTP command termination
	VarString          cmd;
	
	cmd  = command;
	cmd += CMD_TERM;

	m_pSocket->send(cmd.c_str(), static_cast<Int32>(cmd.length()));

	if (LoggerPool::smtp().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendCommand():\n"
			        "<%s>", 
			        command);
		LoggerPool::smtp().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Smtp::sendData(ConstString data)
{
	m_pSocket->send(data, static_cast<Int32>(strlen(data)));

	if (LoggerPool::smtpData().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "sendData():\n"
			        "%.*s", 
					strlen(data) - strlen("\r\n"), data);
		LoggerPool::smtpData().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

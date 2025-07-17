//----------------------------------------------------------------------------
/*! \file
 *  \brief  MailMessage class
 *  \author Michael Eichenlaub
 *  \date   05.12.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_email.h"
#include "libbasarcmnutil_codec.h"

#include "libbasarcmnutil_iattachment.h"

#include "libbasarcmnutil_datetime.h"
#include "loggerpool.h"

#include <string.h>

#include <boost/scoped_array.hpp>

#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#endif

#include "mailhelper.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
ConstString MailMessage::s_CRLF = "\r\n";		//!< constant for carriage return, line feed

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
MailMessage::MailMessage ( const Locale::LanguageEnum eLocale /* = Locale::getLocale() */)
						 :	m_eLanguage(eLocale),
							m_ContentType( TEXT_PLAIN ),
							m_Importance( NORMAL )
{
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
MailMessage::~MailMessage ()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void MailMessage::setSubject(const I18nString & rSubject)
{
	m_Subject = rSubject;

    //sanitize subject - strip newlines
    if ( I18nString::npos != m_Subject.find("\n") )
    {
        m_Subject.remove("\n");
    }

	convert(m_Subject);

	encodeSubject();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void MailMessage::setFrom(const VarString & rSender)
{
	m_From = rSender;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::setTo(const VarString & rRecipient)
{
	assert(!rRecipient.empty());

	if (!rRecipient.empty())
		m_CollTo.push_back(rRecipient);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::setCC(const VarString & rRecipient)
{
	assert(!rRecipient.empty());

	if (!rRecipient.empty())
		m_CollCC.push_back(rRecipient);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::setBCC(const VarString & rRecipient)
{
	assert(!rRecipient.empty());

	if (!rRecipient.empty())
		m_CollBCC.push_back(rRecipient);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::setBody(const I18nString & rText)
{
	m_Body = rText;

	// convert regarding appropriate codepage
	convert(m_Body);

	if( TEXT_PLAIN == m_ContentType )
	{
		// correct line wrapping 
		wrapBody();				
	}

	// encode body
	encodeBody();

	// prepare body for SMTP protocol (e.g. END OF DATA mark)
	cookBody();
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::setContentType(
	const ContentTypeEnum contentType
		)
{
	m_ContentType = contentType;
}

void MailMessage::setImportance( const ImportanceEnum importance )
{
	m_Importance = importance;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::addAttachment(IAttachmentPtr attachment)
{
	m_AttachmentCollection.push_back(attachment);
}


//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return count of entries in email's To-field
UInt32 MailMessage::getCountTo() const
{
	return static_cast<const UInt32>(m_CollTo.size());
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return count of entries in email's Cc-field
UInt32 MailMessage::getCountCC() const
{
	return static_cast<const UInt32>(m_CollCC.size());
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return count of entries in email's Bcc-field
UInt32 MailMessage::getCountBCC() const
{
	return static_cast<const UInt32>(m_CollBCC.size());
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return I18nString containing email's subject
const I18nString & MailMessage::getSubject() const
{
	return m_Subject;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return VarString containing email's From-field
const VarString & MailMessage::getFrom() const
{
	return m_From;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return entry in email's To-collection at position index
const VarString & MailMessage::getTo(UInt32 index) const
{
	assert(index < m_CollTo.size());

	return m_CollTo.at(index);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return entry in email's Cc-collection at position index
const VarString & MailMessage::getCC(UInt32 index) const
{
	assert(index < m_CollCC.size());

	return m_CollCC.at(index);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return entry in email's Bcc-collection at position index
const VarString & MailMessage::getBCC(UInt32 index) const
{
	assert(index < m_CollBCC.size());

	return m_CollBCC.at(index);
}

//------------------------------------------------------------------------------
//! \throw MailMsgException
//!
void MailMessage::checkSettings	() const
{
	if ( m_From.empty()       ||
		(0 == m_CollTo.size())  )
	{
		VarString msg;
		msg.format("one or more header properties not set\n"
			        "    From: <%s>\n"
					"    To  : %d items",
			        m_From.c_str(),
					m_CollTo.size());
		throw MailMsgException(ExceptInfo("basar.cmnutil.MailMessage.checkSettings()", msg, __FILE__, __LINE__));
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return VarString containing email's header
VarString MailMessage::getHeader() const
{
	static ConstString SEPARATOR  = ",";
	static ConstString BRACKET_L  = "<";
	static ConstString BRACKET_R  = ">";
	VarString          line;
	VarString          header;


   	// from - message header
	static ConstString FROM = "From: <%s>";

	line.format(FROM, m_From.c_str());
	header += line;
	header += s_CRLF;


	// to - message header
	static ConstString TO = "To: ";

	for (UInt32 i = 0, lim = static_cast<UInt32>(m_CollTo.size()); i < lim; ++i)
	{
		header += (0 == i) ? TO : SEPARATOR;
		header += BRACKET_L;
		header += m_CollTo.at(i);
		header += BRACKET_R;

		if ((lim - 1) == i)
			header += s_CRLF;
	}


	// cc (but not bcc!) - message header
	static ConstString CC = "Cc: ";

	for (UInt32 i = 0, lim = static_cast<UInt32>(m_CollCC.size()); i < lim; ++i)
	{
		header += (0 == i) ? CC : SEPARATOR;
		header += BRACKET_L;
		header += m_CollCC.at(i);
		header += BRACKET_R;

		if ((lim - 1) == i)
			header += s_CRLF;
	}


	// date - message header
	static ConstString NOW = "Date: %s";
	DateTime           now;

	line.format(NOW, now.toStrInternetMsg().c_str());
	header += line;
	header += s_CRLF;


	// subject - message header
	static ConstString SUBJECT = "Subject: %s";

	line.format(SUBJECT, m_Subject.c_str());
	header += line;
	header += s_CRLF;


	// MIME version - message header
	static ConstString MIME = "MIME-Version: 1.0";

	header += MIME;
	header += s_CRLF;

	// Importance
	static ConstString IMPORTANCE = "Importance: ";
	header += IMPORTANCE;

	switch( m_Importance )
	{
		case HIGH:
		{
			header += "High";
		}
		break;
		case LOW:
		{
			header += "Low";
		}
		break;
		case NORMAL:
		default:
		{
			header += "Normal";
		}
		break;
	}

	header += s_CRLF;

	if (hasAttachments())
	{
		// MIME header for mail with attachments
		static ConstString MESSAGE_CONTENT_TYPE = "Content-Type: multipart/mixed; boundary=\"#BOUNDARY#\"";
		// MIME info string
		static ConstString MIME_MULTIPART_INFO  = "This is a multi-part message in MIME format.";
		header += MESSAGE_CONTENT_TYPE;
		header += s_CRLF;
		header += MIME_MULTIPART_INFO;
		header += s_CRLF;
	}

	if (LoggerPool::mailMsg().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "getHeader():\n"
					"%.*s", 
					header.length() - strlen(s_CRLF), header.c_str());
		LoggerPool::mailMsg().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return header;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return VarString containing header for text parts in multipart emails
VarString MailMessage::getMessageTextHeader() const
{
	// content type - text block header
	static ConstString CONTENT_TEXT_PLAIN = "Content-type: text/plain; charset=\"%s\"";
	static ConstString CONTENT_TEXT_HTML = "Content-type: text/html; charset=\"%s\"";
	VarString          line;
	VarString          header;

	switch( m_ContentType )
	{
		case TEXT_SIMPLEHTML: 
			line.format(CONTENT_TEXT_HTML, MailHelper::getCharset(m_eLanguage)); 
			break;

		case TEXT_PLAIN: 
		default:
			line.format(CONTENT_TEXT_PLAIN, MailHelper::getCharset(m_eLanguage));
			break;
	}
	header += line;
	header += s_CRLF;

	// content-transfer-encoding - text block header
	static ConstString ENCODING = "Content-Transfer-Encoding: quoted-printable";

	header += ENCODING;
	header += s_CRLF;
	
	return header;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return I18nString containing body of email
I18nString MailMessage::getBody() const
{
	I18nString body;
	
	if (hasAttachments())
	{
		body += "--#BOUNDARY#";
		body += s_CRLF;		
	}
	
	body += getMessageTextHeader();
	body += s_CRLF;
	body += getMessageTextBody();
	body += s_CRLF;
	
	std::vector<IAttachmentPtr>::const_iterator itEnd = m_AttachmentCollection.end();
	std::vector<IAttachmentPtr>::const_iterator itAttachment = m_AttachmentCollection.begin();
	
	for ( ; itAttachment != itEnd; ++itAttachment)
	{
		//no need to ask for has attachments...
		body += "--#BOUNDARY#";
		body += s_CRLF;		
		body += (*itAttachment)->getHeader();
		body += (*itAttachment)->getBodyEncoded();
		body += s_CRLF;			
	}
	
	//add final boundary
	if (hasAttachments())
	{
		body += "--#BOUNDARY#--";
		body += s_CRLF;		
	}

	if (LoggerPool::mailMsg().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "getBody():\n"
					"%.*s", 
					body.length() - strlen(s_CRLF), body.c_str());
		LoggerPool::mailMsg().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	return body;
}

const I18nString & MailMessage::getMessageTextBody() const
{
	if (LoggerPool::mailMsg().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format( "getMessageTextBody():\n"
					"%.*s", 
					m_Body.length() - strlen(s_CRLF), m_Body.c_str());
		LoggerPool::mailMsg().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_Body;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::cookBody()
{
	I18nString::size_type len = m_Body.length();

	// exchange leading ".\r\n" if necessary
	static ConstString  FROMLEAD    = ".\r\n";
	static ConstString  TOLEAD      = ".";
    static const size_t LENFROMLEAD = strlen(FROMLEAD);

	if ( (LENFROMLEAD <= len) &&							// body is long enough
		 (FROMLEAD    == m_Body.substr(0, LENFROMLEAD)) )	// first substring contains FROMLEAD
	{
		m_Body.insert(0, TOLEAD);

		len = m_Body.length();
	}


	// exchange "\r\n.\r\n" (meta sign for END OF MAIL DATA)
	static ConstString    FROM        = "\r\n.\r\n";
	static ConstString    TO          = "\r\n..\r\n";
    static const size_t   LENFROM     = strlen(FROM);
	I18nString::size_type pos         = 0;

    while( pos < len )
    {   
		if ( VarString::npos == (pos = m_Body.find(FROM, pos)) )
			break;

		m_Body.erase ( pos, LENFROM );
		m_Body.insert( pos, TO);

		len = m_Body.length();   
    }


	// add trailing CRLF if necessary
	static const size_t LENCRLF = strlen(s_CRLF);

	if ( (LENCRLF >  len) ||									// body to small
		 (s_CRLF  != m_Body.substr(len - LENCRLF, LENCRLF)) )	// last substring doesn't contain CRLF
		m_Body += s_CRLF;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::encodeSubject()
{
	// encode quoted-printable
	Int32                  maxLenEncod = QuotedPrint::getMaxWEncodeLength(m_Subject.c_str()                     , 
		                                                                  static_cast<Int32>(m_Subject.length()),
																		  MailHelper::getCharset(m_eLanguage)   );
	boost::scoped_array < char > pBufEncod(new char[maxLenEncod]);

	Int32                  lenEncod    = QuotedPrint::wencode(m_Subject.c_str()                     , 
															  static_cast<Int32>(m_Subject.length()), 
		                                                      pBufEncod.get()                       , 
															  maxLenEncod                           ,
															  MailHelper::getCharset(m_eLanguage)	);
	m_Subject.assign(pBufEncod.get(), lenEncod);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::encodeBody()
{
	// encode quoted-printable
	Int32                  maxLenEncod = QuotedPrint::getMaxEncodeLength(m_Body.c_str(), static_cast<Int32>(m_Body.length()));
	boost::scoped_array < char > pBufEncod(new char[maxLenEncod]);

	Int32                  lenEncod    = QuotedPrint::encode(m_Body.c_str() , static_cast<Int32>(m_Body.length()), 
		                                                    pBufEncod.get(), maxLenEncod                        );
	m_Body.assign(pBufEncod.get(), lenEncod);
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//!
void MailMessage::wrapBody()
{
	if (0 == m_Body.length())
		return;


	static const char CHAR_LF = '\n';
	static const char CHAR_CR = '\r';


	// --- correct LF to CRLF

	if ( CHAR_LF == m_Body.at(0) )		// body's first character
		m_Body.insert(static_cast<I18nString::size_type>(0), 1, CHAR_CR);	// for Solaris compiler	

	I18nString::size_type pos = 1;
    I18nString::size_type len = m_Body.length();

    while ( pos < len )
    {   
		if ( VarString::npos == (pos = m_Body.find(CHAR_LF, pos)) )
			break;

		if ( CHAR_CR != m_Body.at(pos - 1) )
		{
			m_Body.insert( pos, 1, CHAR_CR);
			len = m_Body.length();
			++pos;
		}

		++pos;
    }


	// --- wrap lines

	static const Int32		POS_WRAP   = 78;
	static const char		CHAR_SPACE = ' ';
	I18nString::size_type	pos_space  = VarString::npos;		// position for ' ' or TAB
	Int32					pos_line   = 0;						// position within line
	VarString				leftPart;
	VarString				rightPart;
	VarString::value_type	ch;

	pos = 0;
    len = m_Body.length();

    while ( pos < len )
    {   
		ch = m_Body.at(pos);

		switch (ch)
		{
		case CHAR_CR:					// ignore CR, look for next LF
			break;

		case CHAR_LF:
			pos_line = 0;				// reset after LF
			break;

		case CHAR_SPACE:				// fall through default case
			 pos_space = pos;			// save last position where wrapping is possible

		default:
			if (pos_line <= POS_WRAP)
				++pos_line;
			else
			{
				if (VarString::npos != pos_space)	// possible position already found
				{
					leftPart  = m_Body.substr(0, pos_space);
					rightPart = m_Body.substr(pos_space, len - pos_space);

					leftPart.trim (VarString::END  );
					rightPart.trim(VarString::FIRST);

					m_Body  = leftPart;
					m_Body += s_CRLF;
					m_Body += rightPart;

					pos_line = 0;

                    len = m_Body.length();
				}
			}

			break;
		}	// end switch


		++pos;
    }
}

//------------------------------------------------------------------------------
//! \throw MailMsgException
//!
void MailMessage::convert(I18nString & rString)
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	// --- convert Windows codepage CP125x to corresponding ISO 8859-x if running with Windows

	// corresponding codepages Windows CP / ISO
	static const UInt32 CODEPAGE[][2] = 
	{
		{ 1252, 28605 },		// ANSI - Latin I          / ISO 8859-15 Latin 9
		{ 1251,	28595 }, 		// ANSI - Cyrillic         / ISO 8859-5  Cyrillic
		{ 1250,	28592 }			// ANSI - Central European / ISO 8859-2  Central Europe
	};

	Int32 cp;

	// get suitable charset
	switch (Locale::getRegularCodepage(m_eLanguage))
	{
	case Locale::CP_EAST:
		cp = 2;
		break;

	case Locale::CP_CYRILLIC:
		cp = 1;
		break;

	case Locale::CP_WEST:
	default:
		cp = 0;
		break;
	}

	const I18nString::size_type		LENGTH  = rString.size() + 1;			// + terminating null
	const I18nString::size_type     ANSILEN = LENGTH * sizeof(char   );
	const I18nString::size_type     WIDELEN = LENGTH * sizeof(wchar_t);

	boost::scoped_array < char    > pAnsiBuf (new char    [LENGTH]);
	boost::scoped_array < wchar_t > pWideBuf (new wchar_t [LENGTH]);		

	memset(pAnsiBuf.get(), 0, ANSILEN);
	memset(pWideBuf.get(), 0, WIDELEN);

	int written;

	// convert from Windows to Unicode
	written = ::MultiByteToWideChar(CODEPAGE[cp][0], 0, 
									rString .c_str(), -1, 
									pWideBuf.get  (), static_cast<int>(WIDELEN));

	if (! written)
	{
		VarString msg;
		msg.format("mapping from encoding %d to Unicode failed with error %d\n"
			       "string to be mapped: <%s>",
			       CODEPAGE[cp][0], 
				   GetLastError(),
				   rString.c_str());
		throw MailMsgException(ExceptInfo("basar.cmnutil.MailMessage.convert()", msg, __FILE__, __LINE__));
	}



	// convert from Unicode to ISO
	written = ::WideCharToMultiByte(CODEPAGE[cp][1], 0, 
									pWideBuf.get(), -1, 
									pAnsiBuf.get(), static_cast<int>(ANSILEN), 
									0, 0);

	if (! written)
	{
		VarString msg;
		msg.format("mapping from Unicode to encoding %d failed with error %d\n"
			       "string to be mapped: <%s>",
			       CODEPAGE[cp][0], 
				   GetLastError(),
				   rString.c_str());
		throw MailMsgException(ExceptInfo("basar.cmnutil.MailMessage.convert()", msg, __FILE__, __LINE__));
	}

	rString = pAnsiBuf.get();
	
#else
	
	rString = rString;
	
#endif
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating if email has attachments
bool MailMessage::hasAttachments() const
{
	return !(m_AttachmentCollection.empty());
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for email
 *  \author Michael Eichenlaub
 *  \date   04.12.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_EMAIL_H
#define GUARD_LIBBASARCMNUTIL_EMAIL_H

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
#include <boost/shared_ptr.hpp>
#include "libbasarcmnutil_locale.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_iattachmentptr.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! \brief MailMessage exception class; thrown if errors with MailMessage occur
//! \n final class
class MailMsgException : public basar::Exception
{
public:
	//! constructs exception object and initialises exception type	\n no-throw
	LIBBASARCMNUTIL_API		MailMsgException	(const ExceptInfo & sInfo	//!< exception info struct
												)
												: basar::Exception(sInfo, "MailMsgException")
												{};
private:
							MailMsgException	();							//!< forbidden
};

//----------------------------------------------------------------------------
//! \brief SMTP protocol exception class; thrown if errors with SMTP protocol occur
//! \n final class
class SMTPException : public basar::Exception
{
public:
	//! constructs exception object and initialises exception type	\n no-throw
	LIBBASARCMNUTIL_API		SMTPException		(const ExceptInfo & sInfo	//!< exception info struct
												)
												: basar::Exception(sInfo, "SMTPException")
												{};
private:
							SMTPException		();							//!< forbidden
};

//----------------------------------------------------------------------------
//! \brief internet message - email
//! \n     final class
class MailMessage
{
	friend class Smtp;		//!< only for basar internal use: calls private access methods

public:
	//! \brief importance of mail message (RFC 4021)
	enum ImportanceEnum
	{
		HIGH,
		NORMAL,
		LOW
	};
	//! \brief content type of mail message
	enum ContentTypeEnum
    { 
        TEXT_PLAIN,			//!< plain text messages
        TEXT_SIMPLEHTML		//!< messages containing simple html
    };  

	//! c'tor	\n no-throw
	LIBBASARCMNUTIL_API						MailMessage		(const Locale::LanguageEnum eLocale = Locale::getLocale()		//!< locale, needed for charset
															);
	//! d'tor	\n no-throw
	LIBBASARCMNUTIL_API					   ~MailMessage		();

	//! set message's subject	\n no-throw
	LIBBASARCMNUTIL_API void				setSubject		(const I18nString &			rSubject			//!< message's subject
															);
	//! set message's sender	\n no-throw
	LIBBASARCMNUTIL_API void				setFrom			(const VarString  &			rSender				//!< message's sender
															);
	//! set message's recipient	(add to To-list) \n no-throw
	LIBBASARCMNUTIL_API void				setTo			(const VarString  &			rRecipient			//!< message's recipient
															);
	//! set message's carbon copy recipient (add to CC-list)	\n no-throw
	LIBBASARCMNUTIL_API void				setCC			(const VarString  &			rRecipient			//!< message's carbon copy recipient
															);
	//! set message's blind carbon copy recipient (add to BCC-list)	\n no-throw
	LIBBASARCMNUTIL_API void				setBCC			(const VarString  &			rRecipient			//!< message's blind carbon copy recipient
															);
	//! set message text: text will be wrapped, encoded and prepared for SMTP protocol	\n no-throw
	LIBBASARCMNUTIL_API void				setBody			(const I18nString  &		rText				//!< message text
															);
	//! set content type: use this method before calling \ref setBody \n \throw no-throw
	LIBBASARCMNUTIL_API void				setContentType  (const ContentTypeEnum		contentType			//!< content type
															);
    //! set importance of mail message \n \throw no-throw
	LIBBASARCMNUTIL_API void				setImportance   (const ImportanceEnum		importance			//!< importance
															);
	//! add an attachment to mail message \n \throw no-throw														
	LIBBASARCMNUTIL_API void				addAttachment   (IAttachmentPtr				attachment          //!< attachment
	                                                        );

private:

	typedef std::vector < VarString >		CollRcpt;		//!< collection type for recipients

	//--------------------------------------------------------------------------

							MailMessage		(const MailMessage & rCopy);	//!< forbidden
	MailMessage &    		operator =		(const MailMessage & rCopy);	//!< forbidden

	UInt32					getCountTo		() const;		//!< get number of recipients		\n no-throw
	UInt32					getCountCC		() const;		//!< get number of cc recipients	\n no-throw
	UInt32					getCountBCC		() const;		//!< get number of bcc recipients	\n no-throw

	//! get message's subject			\n no-throw
	const I18nString &		getSubject		() const;
	//! get message's sender			\n no-throw
	const VarString  &		getFrom			() const;
	//! get message's recipient from list		\n no-throw
	const VarString  &		getTo			(UInt32 index	//!< recipient's list index
											) const;
	//! get message's cc recipient from list	\n no-throw
	const VarString  &		getCC			(UInt32 index	//!< cc recipient's list index
											) const;
	//! get message's bcc recipient from list	\n no-throw
	const VarString  &		getBCC			(UInt32 index	//!< bcc recipient's list index
											) const;
	//! get message's header					\n no-throw
	VarString  		getHeader				() const;
	
	//! get header for body text				\n no-throw
	VarString		getMessageTextHeader	() const;
	
	//! get message body (text + attachments if any) \n no-throw
	I18nString 		getBody					()	const;

	//! get message text (text part of body)	\n no-throw
	const I18nString &	getMessageTextBody	() const;

	//! handle END OF MAILDATA within message text			\n no-throw
	void					cookBody		();
	//! wrap message text lines								\n no-throw
	void					wrapBody		();
	//! encode message text (transform to 7 bit characters)	\n no-throw
	void					encodeBody		();
	//! encode message subject (transform to 7 bit characters)	\n no-throw
	void					encodeSubject	();

	//! check mandatory header fields						\n throw MailMsgException
	void					checkSettings	() const;

	//! convert characters regarding appropriate codepage (Windows CP => ISO)	\n throw MailMsgException
	void					convert			(I18nString  & rString	//!< string to be converted
													);
	//! check is mail has attachments						\n no-throw														
	bool					hasAttachments  () const;



	//--------------------------------------------------------------------------

	static ConstString		s_CRLF;				//!< constant for carriage return, line feed

	Locale::LanguageEnum	m_eLanguage;		//!< locale, needed for selecting correct charset
	ContentTypeEnum			m_ContentType;		//!< content type
	ImportanceEnum			m_Importance;		//!< importance
	CollRcpt				m_CollTo;			//!< collection with recipients
	CollRcpt				m_CollCC;			//!< collection with cc recipients
	CollRcpt				m_CollBCC;			//!< collection with bcc recipients
	I18nString				m_Subject;			//!< message's subject
	VarString				m_From;				//!< message's sender
	I18nString				m_Body;				//!< message text
	
	std::vector<IAttachmentPtr> m_AttachmentCollection;	//!< collection for email's attachments
};

//----------------------------------------------------------------------------
// forward
class TcpSocket;

//----------------------------------------------------------------------------
//! SMTP - Simple Mail Transfer Protocol class	\n final class
class Smtp
{
public:

	enum RetCodeEnum						//!  SMTP's return values
	{
		SMTP_SUCCESS				= 0,	//!< SMTP communication OK

		SMTP_OFFLINE				= 1,	//!< SMTP connect: server is offline/down
		SMTP_CONNREFUSED			= 2,	//!< SMTP connect: server rejected attempts to connect:\n-> maybe too many client connections, so try again later
		SMTP_CONNINPROGRESS         = 3,	//!< SMTP connect: socket is non-blocking, connection cannot completed immediately

        SMTP_SUCCESS_WITH_REFUSED_RECIPIENTS = 4, //!< SMTP send: some addresses where rejected by the server but not all - query rejected addresses with hasRejectedRecipients and/or getRejectedRecipients
		SMTP_UNROUTEABLE_ADDRESS			//!< SMTP send: receiver's address is unroutable (maybe mail server isn't setup to relay mail)
	};

	enum ServicePortEnum						//! SMTP's service port
	{
		SMTP_PORT_DEFAULT = 25					//!< default SMTP service port number
	};


	//! constructor	\n no-throw
	LIBBASARCMNUTIL_API					Smtp			(const VarString & rRemoteHost,						//!< remote host name or ip address ("a.b.c.d")
														 const Int16       servicePort = SMTP_PORT_DEFAULT	//!< printer service port the daemon listens to
														);
	//! destructor	\n no-throw
	LIBBASARCMNUTIL_API				   ~Smtp			();

	//! connect to SMTP server					\n no-throw
	LIBBASARCMNUTIL_API RetCodeEnum		connect			();
	//! disconnect from SMTP server				\n no-throw
	LIBBASARCMNUTIL_API void			disconnect		();
	//! send internet message to SMTP server	\n no-throw
	LIBBASARCMNUTIL_API RetCodeEnum		send			(const MailMessage & rMsg		//!< internet message (header and body)
														);

    //! check whether or not addresses were rejected by the smtp server
    LIBBASARCMNUTIL_API bool            hasRejectedRecipients() const;

    //! get rejected recipient addresses
    //! \return CollBString containing rejected recipient addresses
    LIBBASARCMNUTIL_API const CollBString& getRejectedRecipients() const;
private:

	typedef boost::shared_ptr < TcpSocket >	TcpSocketSharedPtr;		//!< smart pointer to TcpSocket

	//--------------------------------------------------------------------------

										Smtp			();                           	//!< forbidden
										Smtp			(const Smtp & rCopy);			//!< forbidden
	Smtp &    							operator =		(const Smtp & rCopy);			//!< forbidden

	//! scan server's response for expected answer	\n throw SMTPException
	bool								scanResponse	(ConstString reply,				//!< expected reply
														 ConstString optional = 0		//!< optional reply
														);
	//! send SMTP command to SMTP server			\n no-throw
	void								sendCommand		(ConstString command			//!< SMTP command
														);
	//! send SMTP data to SMTP server				\n no-throw
	void								sendData		(ConstString data				//!< SMTP data
														);

	//--------------------------------------------------------------------------

	TcpSocketSharedPtr					m_pSocket;				//!< tcp socket for transmitting data to smtp server

    CollBString                         m_RejectedRecipients;   //!< collection to store rejected recipient addresses
};

//-----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD

//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: class for email attachment
 *  \author Marco Köppendörfer
 *  \date   02.12.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_ATTACHMENT_H
#define GUARD_LIBBASARCMNUTIL_ATTACHMENT_H

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
// include section
//----------------------------------------------------------------------------
#include <iosfwd>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_iattachmentptr.h"
#include "libbasarcmnutil_iattachment.h"
#include "libbasarcmnutil_locale.h"


namespace basar
{
	namespace cmnutil
	{
		//! \brief class representing email attachment
        //! \n     final class
		class Attachment : public IAttachment
		{
			//member functions
			public:
				//! d'tor	\n no-throw
				LIBBASARCMNUTIL_API ~Attachment();
				
				//class member functions
				//! static(!) creator method for an attachment from a file	\n throw BasarFileNotAccessableException
				LIBBASARCMNUTIL_API static IAttachmentPtr createFromFile(const BString& fileName);
				//! static(!) creator method for an attachment from an open stream	\n no-throw
				LIBBASARCMNUTIL_API static IAttachmentPtr createFromMemory(const BString& attachmentFileName, std::istream& attachmentData);
				
				//instance member functions
				
				//! getter for base64 encoded attachment data \n no-throw
				LIBBASARCMNUTIL_API virtual const BString& getBodyEncoded() const;
				
				//! getter for attachment header - header is created on the fly \n no-throw
				LIBBASARCMNUTIL_API virtual BString getHeader() const;
				
			protected:
				//! c'tor  \n no-throw
				Attachment(const Locale::LanguageEnum eLocale = Locale::getLocale()		//!< locale, needed for charset of attachment names
						   );
				
				//! sets m_AttachmentFileSize according to size - use before encoding \n no-throw
				void setSize(size_t size);
				
				//! getter for size - unencoded size is stored \n no-throw
				size_t getSize() const;
				
				//! sets AttachmentName and AttachmentFileName \n no-throw
				void setAttachmentNames(const BString& fileName);
				
				//! encodes passed string according to RFC2047 \n no-throw
				BString createEncodedName(const BString& name) const;
				
				//! creates quoted-printable prefix according to RFC2047 \n no-throw
				BString createQuotedPrintablePrefix() const;
				
				//! splits a quoted printable string to in RCF2047 defined length \n no-throw
				BString splitQuotedPrintableName(const BString& quotedPrintablePrefix, const BString& encodedString, const BString& quotedPrintableSuffix) const;
				
				//! reads passed stream and stores data base64 encoded \n no-throw
				void setData(std::istream& attachmentData);
				
				//! getter for m_AttachmentFileName - filename with suffix, e.g. myText.txt \n no-throw
				const BString& getAttachmentFileName() const;
				
				//! getter for m_AttachmentName - filename w/o suffix, e.g. myText \n no-throw
				const BString& getAttachmentName() const;							
				
			private:
				Attachment(const Attachment&);						//!< forbidden
				Attachment& operator=(const Attachment&);			//!< forbidden
			
			//member variables
			private:
				
				static ConstString		s_CRLF;							//!< constant for carriage return, line feed
				static const UInt32     s_MAX_QUOTEDPRINTABLE_LENGTH;	//!< constant for maximal quoted printable length
				
				Locale::LanguageEnum	m_eLanguage;					//!< locale, needed for selecting correct charset of attachment names
				size_t					m_AttachmentFileSize;			//!< containing file size
				BString					m_Data;							//!< attachment data base64 encoded
				BString					m_AttachmentFileName;			//!< containing file name with suffix, e.g. myFile.txt
				BString					m_AttachmentName;				//!< containing file name w/o suffix, e.g. myFile
				
		};
	}
}



#endif // GUARD_LIBBASARCMNUTIL_ATTACHMENT_H

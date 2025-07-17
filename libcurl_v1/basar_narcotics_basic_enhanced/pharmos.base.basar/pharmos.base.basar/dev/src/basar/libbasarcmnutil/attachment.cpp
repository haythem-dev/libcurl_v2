//----------------------------------------------------------------------------
/*! \file
 *  \brief  class definition for email attachment
 *  \author Marco Köppendörfer
 *  \date   02.12.2013
 */
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// include section
//----------------------------------------------------------------------------
#include <istream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4127 )
#endif // _WIN32

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_array.hpp>

#ifdef _WIN32
#pragma warning( pop )
#endif // _WIN32

#include "libbasarcmnutil_attachment.h"
#include "libbasarcmnutil_codec.h"
#include "libbasarcmnutil_exceptions.h"

#include "attachmentptr.h"
#include "mailhelper.h"

//----------------------------------------------------------------------------
// class definition section
//----------------------------------------------------------------------------
namespace basar
{
	namespace cmnutil
	{
	
		//------------------------------------------------------------------------------
		ConstString Attachment::s_CRLF = "\r\n";		//!< constant for carriage return, line feed
		
		//------------------------------------------------------------------------------
		const UInt32 Attachment::s_MAX_QUOTEDPRINTABLE_LENGTH = 75;
		
		Attachment::~Attachment()
		{
		}
		
		IAttachmentPtr Attachment::createFromFile(const BString& fileName)
		{
			std::ifstream ifs;
			
			//read binary, we don't care about line-endings and stuff
			ifs.open(fileName.c_str(), std::ios::in | std::ios::binary);
			
			if (!ifs.good())
			{
				ifs.close();
				
				std::ostringstream os;
				os << "open of file: >" << fileName << "< failed. Stream's state is not good.";											
				throw BasarFileNotAccessibleException(ExceptInfo("basar.cmnutil.Attachment::createFromFile()", os.str(), __FILE__, __LINE__));
			}
			
			//use create from memory to get our attachment - clever, aren't we ;)
			IAttachmentPtr attachment = Attachment::createFromMemory(fileName, ifs);
			
			ifs.close();			
			
			return attachment;
		}
		
		IAttachmentPtr Attachment::createFromMemory(const BString& attachmentFileName, std::istream& attachmentData)
		{
			AttachmentPtr attachment = AttachmentPtr(new Attachment());
			
			attachment->setAttachmentNames(attachmentFileName);
			
			attachment->setData(attachmentData);
						
			return attachment;
		}
		
		size_t Attachment::getSize() const
		{
			return m_AttachmentFileSize;
		}
		
		const BString& Attachment::getBodyEncoded() const
		{			
			return m_Data;
		}
		
		BString Attachment::getHeader() const
		{
			//Content-Type: application/octet-stream; name=\"name\"
			//Content-Disposition: attachment; filename=\"filename\"; size=<size>
			//Content-Transfer-Encoding: base64
			//
			
			std::ostringstream os;
			
			os << "Content-Type: application/octet-stream; name= \"" << getAttachmentName() << "\"" << s_CRLF;
			os << "Content-Disposition: attachment; size=" << static_cast<UInt32>(getSize()) << "; filename= \"" << getAttachmentFileName() << "\"" << s_CRLF;
			os << "Content-Transfer-Encoding: base64" << s_CRLF;
			os << s_CRLF;
			
			return os.str();
		}
		
		Attachment::Attachment(const Locale::LanguageEnum eLocale /* = Locale::getLocale() */)
						 :	m_eLanguage(eLocale), m_AttachmentFileSize(0), m_Data(""), m_AttachmentFileName(""), m_AttachmentName("")
		{
		}
	
		void Attachment::setSize(size_t size)
		{
			m_AttachmentFileSize = size;
		}
		
		void Attachment::setAttachmentNames(const BString& fileName)
		{
			using boost::filesystem::path;
			
			path filePath(fileName.c_str());
	
			//set filename with filetype suffix - store encoded name
			m_AttachmentFileName = createEncodedName(filePath.filename().string());
			//set filename w/o filetype suffix - store encoded name
			m_AttachmentName = createEncodedName(filePath.filename().stem().string());
		}
		
		BString Attachment::createEncodedName(const BString& name) const
		{		
			// encode quoted-printable - request header encoding style (encodes '_' as '=F5')
			static ConstString QUOTED_PRINTABLE_SOFTBREAK = "?=";
			
			Int32 maxLenEncod = QuotedPrint::getMaxEncodeLength( name.c_str(), static_cast<Int32>(name.length())  );
			
			boost::scoped_array < char > pBufEncod(new char[maxLenEncod]);

			Int32 lenEncod = QuotedPrint::encode(name.c_str()                          , 
												 static_cast<Int32>(name.length())     , 
												 pBufEncod.get()                       , 
												 maxLenEncod                           ,
												 QuotedPrint::ENC_STYLE_HEADER         );

			BString encodedName;
			
			encodedName.assign(pBufEncod.get(), lenEncod);
			//see RFC2047
			//encode as =?<charset>?<format indicator>?<encoded name>?=
			
			//get the quotedprintable prefix =?<charset>?<format indicator>?
			BString quotedPrintablePrefix( createQuotedPrintablePrefix() );
						
			std::ostringstream os;
			
			//check if we would have to split our encoded name
			if (s_MAX_QUOTEDPRINTABLE_LENGTH < encodedName.length() + quotedPrintablePrefix.length() + strlen(QUOTED_PRINTABLE_SOFTBREAK) + strlen(s_CRLF) )
			{
				os << splitQuotedPrintableName(quotedPrintablePrefix, encodedName, QUOTED_PRINTABLE_SOFTBREAK);
			}
			else
			{
				os << quotedPrintablePrefix << encodedName << QUOTED_PRINTABLE_SOFTBREAK;
			}
			
			return os.str();		
		}
		
		BString Attachment::createQuotedPrintablePrefix() const
		{
			static ConstString QUOTED_PRINTABLE_FORMAT_INDICATOR = "Q";
			static ConstString QUOTED_PRINTABLE_FORMAT_START = "=?";
			static ConstString QUOTED_PRINTABLE_FORMAT_DELIMITER = "?";
			
			std::ostringstream os;
			os << QUOTED_PRINTABLE_FORMAT_START << MailHelper::getCharset(m_eLanguage);
			os << QUOTED_PRINTABLE_FORMAT_DELIMITER << QUOTED_PRINTABLE_FORMAT_INDICATOR;
			os << QUOTED_PRINTABLE_FORMAT_DELIMITER;
			
			BString quotedPrintablePrefix(os.str());
			
			return quotedPrintablePrefix;
		}
		
		BString Attachment::splitQuotedPrintableName(const BString& quotedPrintablePrefix, const BString& encodedString, const BString& quotedPrintableSoftbreak) const
		{		
			//lenght in chars for "=?<charset>?Q?" (e.g. "=?ISO-8859-15?Q?" -> 16)
			const size_t QUOTED_PRINTABLE_PREFIX_LENGTH = quotedPrintablePrefix.length();
			
			//length in chars for "?=\r\n" -> 4 
			const size_t QUOTED_PRINTABLE_SUFFIX_LENGTH = quotedPrintableSoftbreak.length() + strlen(s_CRLF);
			
			ConstString SPACE = " ";
			
			BString buf(encodedString);	
			
			std::ostringstream os;
			
			os << quotedPrintablePrefix;
			
			while(s_MAX_QUOTEDPRINTABLE_LENGTH < QUOTED_PRINTABLE_PREFIX_LENGTH + buf.length() + QUOTED_PRINTABLE_SUFFIX_LENGTH )
			{
				//since our string is longer than 75 chars, we will spit at that mark minus mandatory lenghtes
				size_t splitAt = s_MAX_QUOTEDPRINTABLE_LENGTH - QUOTED_PRINTABLE_PREFIX_LENGTH - QUOTED_PRINTABLE_SUFFIX_LENGTH;

				//but we have to take care, that we're not splitting (splitting between '=F5') an encoded char
				if ('=' == buf[splitAt - 1])
				{
					//we hit an encoded char (e.g. =F5 - splitAt points to F), we should split at splitAt-1
					splitAt = splitAt - 1; 
				}
				else if('=' == buf[splitAt - 2])
				{
					//we hit an encoded char (e.g. =F5 - splitAt points to 5), we should split at splitAt-2
					splitAt = splitAt - 2;
				}
				else
				{
					//no else case
				}
				
				//append our found string to output buffer
				os << buf.substr(0, splitAt);
				//add qp softbreak, defined as "?=\r\n " (note the tailing space)
				os << quotedPrintableSoftbreak;
				os << s_CRLF;
				os << SPACE;
				//start a new substring
				os << quotedPrintablePrefix;
				
				//remove already copied bytes from buf
				buf.erase(0, splitAt);
			}	
			
			//append the rest of our buffer (now it's below our max length)
			os << buf;
			os << quotedPrintableSoftbreak;
						
			return os.str();
		}
		
		void Attachment::setData(std::istream& attachmentData)
		{
			
			using boost::archive::iterators::insert_linebreaks;
			using boost::archive::iterators::base64_from_binary;
			using boost::archive::iterators::transform_width;
					
			typedef
				insert_linebreaks<         // insert line breaks every 72 characters
					base64_from_binary<    // convert binary values ot base64 characters
						transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
							const char *,
							6,
							8
						>
					> 
					,72
				> 
				base64_text; // compose all the above operations in to a new iterator

			//subblock to remove contents variable			
			{
				std::ostringstream contents;
				contents << attachmentData.rdbuf();				
				m_Data.assign(contents.str());
				//set size now, before encoding - need unencoded size
				setSize(m_Data.length());
			}
			
			//subblock to remote os varialbe			
			{
				std::ostringstream os;
				//get out content base64 encoded
				std::copy(base64_text(m_Data.c_str()),	base64_text(m_Data.c_str() + m_Data.size()), std::ostream_iterator<char>(os));		
				m_Data.assign(os.str());
			}
			
			//base64 demands 0-2 padding chars depending upon size
			UInt32 writePaddChars = (3-static_cast<UInt32>(getSize())%3)%3;	
			m_Data.append(writePaddChars, '=');
			
			
		}
		
		const BString& Attachment::getAttachmentFileName() const
		{
			return m_AttachmentFileName;
		}
		
		const BString& Attachment::getAttachmentName() const
		{			
			return m_AttachmentName;
		}
		
	}
}

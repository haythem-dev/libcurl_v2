//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for de-/coding
 *  \author Michael Eichenlaub
 *  \date   07.12.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_CODEC_H
#define GUARD_LIBBASARCMNUTIL_CODEC_H

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

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//-----------------------------------------------------------------------------
//! \brief MIME coding
//! \n final class
class MimeChar
{
public:
	//! is character type hexadecimal digit?	\n no-throw
	static bool						isHexDigit		(char ch	//!< character to test
													);
	//! is character type printable?			\n no-throw
	static bool						isPrintable		(char ch	//!< character to test
													);
	//! is character type 7 bit?				\n no-throw
	static bool						isNonAscii		(char ch	//!< character to test
													);
	//! is character type control?				\n no-throw
	static bool 					isControl		(char ch	//!< character to test
													);
	//! is character type space?				\n no-throw
	static bool 					isSpace			(char ch	//!< character to test
													);
	//! is character type special?				\n no-throw
	static bool 					isSpecial		(char ch	//!< character to test
													);
	//! is character type delimiter?			\n no-throw
	static bool 					isDelimiter		(char ch	//!< character to test
													);
	//! is character type token?				\n no-throw
	static bool 					isToken			(char ch	//!< character to test
													);

	static const Int32				s_MIMELINE_MAXLEN;			//!< maximum line length for MIME encoding
	static const Int32				s_ENCODEDWORD_MAXLEN;		//!< maximum length for MIME encoded-word

	static const char				s_CR;						//!< carriage return
	static const char				s_LF;						//!< line feed
	static const char				s_SPACE;					//!< space
	static const char				s_TAB;						//!< (horizontal) tabulator


private:

	enum CharTypeEnum			//! MIME character types
	{ 
		CONTROL  = 0x01,		//!< MIME type control
		SPACE    = 0x02,		//!< MIME type space
		PRINT    = 0x04,		//!< MIME type printable
		DOT      = 0x10,		//!< MIME type dot
		SPECIAL  = 0x20,		//!< MIME type special
		HEXDIGIT = 0x40			//!< MIME type hexadecimal digit
	};

	static const char				s_CHARTBL[128];				//!< character table for MIME coded messages
};

//-----------------------------------------------------------------------------
//! codec Quoted-Printable (e.g. for email) \n final class
class QuotedPrint
{
public:

	enum EncodingStyleEnum			//! Enum for requesting different encoding styles
	{
		ENC_STYLE_BODY_TEXT = 0,    //!< Encode quoted-printable for body text
		ENC_STYLE_HEADER			//!< Encode quoted-printable for header text
	};

	//! encode buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	encode				(ConstString pInput,				//!< input buffer
															 Int32       inSize,				//!< input buffer length
															 char *      pOutput,				//!< output buffer
															 Int32       outSize,				//!< output buffer length
															 EncodingStyleEnum style = QuotedPrint::ENC_STYLE_BODY_TEXT //!< governs whether _ (underscore) will be left alone (ENC_STYLE_BODY_TEXT) of encoded to '=F5' (ENC_STYLE_HEADER)										  	
															);
	//! word-encode buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	wencode				(ConstString pInput,				//!< input buffer
															 Int32       inSize,				//!< input buffer length
															 char *      pOutput,				//!< output buffer
															 Int32       outSize,				//!< output buffer length
															 ConstString charset				//!< charset for word encoding
															);
	//! decode buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	decode				(ConstString pInput,				//!< input buffer
															 Int32       inSize,				//!< input buffer length
															 char *      pOutput,				//!< output buffer
															 Int32       outSize				//!< output buffer length
															);
	//! decode word encoded buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	wdecode				(ConstString pInput,				//!< input buffer
															 Int32       inSize,				//!< input buffer length
															 char *      pOutput,				//!< output buffer
															 Int32       outSize				//!< output buffer length
															);
	//! get maximum length for encoded data ( for passing to encode() )		\n no-throw
	LIBBASARCMNUTIL_API static Int32	getMaxEncodeLength	(ConstString pInput,				//!< input (with decoded data) buffer
															 Int32       inSize					//!< input buffer length
															 );
	//! get maximum length for word encoded data (non-ascii header text (RFC 2047)( for passing to encode() )		\n no-throw
	LIBBASARCMNUTIL_API static Int32	getMaxWEncodeLength	(ConstString pInput,				//!< input (with decoded data) buffer
															 Int32       inSize,				//!< input buffer length
															 ConstString charset				//!< charset for word encoding
															 );
private:
	//! get maximum length for word encoded unit (see getMaxEncodeLength()) \n no-throw
	static Int32						getMaxWUnitLength	(ConstString pInput,				//!< input (with decoded unit) buffer
															 Int32       inSize,				//!< input unit buffer length
															 ConstString charset				//!< charset for word encoding
															 );
	//! word-encode unit buffer		\n no-throw
	static Int32						wUnitEncode			(ConstString pInput,				//!< input (with decoded unit) buffer
															 Int32       inSize,				//!< input unit buffer length
															 char *      pOutput,				//!< output buffer
															 Int32       outSize,				//!< output buffer length
															 ConstString charset				//!< charset for word encoding
															);
	static Int32						findSymbol			(ConstString pData,					//!< buffer to be searched 
															 Int32		 size,					//!< buffer length
															 char  &     delim,					//!< found delimiter
															 Int32 &     nonAsciiChars			//!< number of non ASCII characters
															);

	static ConstString					s_QPTABLE;			//!< encoding table

};

//-----------------------------------------------------------------------------
//! codec base64 (e.g. for email attachments) \n final class
class Base64
{
public:
	//! encode buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	encode				(ConstString pInput,		//!< input buffer
															 Int32       inSize,		//!< input buffer length
															 char *      pOutput,		//!< output buffer
															 Int32       outSize		//!< output buffer length
															);
	//! decode buffer		\n no-throw
	LIBBASARCMNUTIL_API static Int32	decode				(ConstString pInput,		//!< input buffer
															 Int32       inSize,		//!< input buffer length
															 char *      pOutput,		//!< output buffer
															 Int32       outSize		//!< output buffer length
															);
	//! get maximum length for encoded data ( for passing to encode() )		\n no-throw
	LIBBASARCMNUTIL_API static Int32	getMaxEncodeLength	(Int32       inSize			//!< input buffer (with decoded data) length
															);
	//! get maximum length for decoded data ( for passing to decode() )		\n no-throw
	LIBBASARCMNUTIL_API static Int32	getMaxDecodeLength	(Int32       inSize			//!< input buffer (with encoded data) length
															);

private:

	//! decode base64 decoded character										\n no-throw
	static char							decodeChar			(char		 code			//!< character to decode
															);
};

//-----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD

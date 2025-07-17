//----------------------------------------------------------------------------
/*! \file
 *  \brief  quoted-printable codec class
 *  \author Michael Eichenlaub
 *  \date   07.12.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_codec.h"

#include "libbasarcmnutil_i18nstring.h"

#include <assert.h>
#include <string.h>
#include <algorithm>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
ConstString QuotedPrint::s_QPTABLE = "0123456789ABCDEF";		//!< characters for encoding table

//------------------------------------------------------------------------------
//! \return maximum length for encoded data
//! \throw  no-throw 
Int32 QuotedPrint::findSymbol ( ConstString pData, 
								Int32		size, 
								char  &     delim, 
								Int32 &     nonAsciiChars)
{
	delim         = 0;
	nonAsciiChars = 0;

	const char * pCur   = pData;
	const char * pEnd   = pData + size;
	char  ch;

	while (pCur < pEnd)
	{
		ch = *pCur;

		if (MimeChar::isNonAscii(ch))
			++nonAsciiChars;
		else
		{
			if (ch == delim)
			{
				delim = 0;		// stop at any delimeters (space or specials)
				break;
			}

			if (!delim && MimeChar::isDelimiter(ch))
			{
				switch (ch)
				{
				case '"':
					delim = '"';	// quoted-string, delimeter is '"'
					break;

				case '(':
					delim = ')';	// comment, delimeter is ')'
					break;

				case '<':
					delim = '>';	// address, delimeter is '>'
					break;
				}

				break;
			}
		}

		++pCur;
	}

	return static_cast<Int32>(pCur - pData);
}

//------------------------------------------------------------------------------
//! \return maximum length for encoded data
//! \throw  no-throw 
Int32 QuotedPrint::getMaxEncodeLength (ConstString pInput,
									   Int32       inSize)
{
	const char * pData = pInput;
	const char * pEnd  = pInput  + inSize;
	Int32        len   = inSize;

	while (pData < pEnd)
	{
		if ( !MimeChar::isPrintable(*pData++) )
			len += 2;
	}

	len += len / (MimeChar::s_MIMELINE_MAXLEN - 2) * 6;

	return len;
}

//------------------------------------------------------------------------------
//! \return maximum length for word encoded unit
//! \throw  no-throw 
Int32 QuotedPrint::getMaxWUnitLength (ConstString pInput,
									  Int32       inSize,
									  ConstString charset)
{
	if (0 == inSize)
	{
		assert(0);
		return 0;
	}

	Int32 len     = getMaxEncodeLength(pInput, inSize);
	Int32 codelen = static_cast<Int32>(strlen(charset) + 7 + 4);

	assert(codelen < MimeChar::s_ENCODEDWORD_MAXLEN);

	return (len / (MimeChar::s_ENCODEDWORD_MAXLEN - codelen) + 1) * codelen + len;
}

//------------------------------------------------------------------------------
//! \return maximum length for word encoded data
//! \throw  no-throw 
Int32 QuotedPrint::getMaxWEncodeLength (ConstString pInput,
									    Int32       inSize,
									    ConstString charset)
{
	if ( (0 == charset) || ('\0' == *charset) )
	{
		assert(0);
		return 0;
	}


	const char * pData = pInput;
	Int32 len          = 0;
	Int32 inlen        = inSize;
	char  delim;
	Int32 nonAsciiChars;
	Int32 unitSize;

	// divide the field into syntactic units to calculate the output length
	do
	{
		unitSize = findSymbol(pData, inlen, delim, nonAsciiChars);

		if (!nonAsciiChars)
			len += unitSize;
		else
			len += getMaxWUnitLength(pData, unitSize, charset);

		pData += unitSize;
		inlen -= unitSize;

		len++;
		pData++;
		inlen--;
	} while (inlen > 0);

	return len;
}

//------------------------------------------------------------------------------
//! \return actual length for encoded data
//! \throw  no-throw 
Int32 QuotedPrint::encode(ConstString pInput , Int32 inSize,
						  char      * pOutput, Int32 outSize,
						  EncodingStyleEnum style /* = = QuotedPrint::ENC_STYLE_BODY_TEXT */)
{
	static bool         quoteLineBreak    = false;

	const char        * pData             = pInput;
	const char        * pEnd              = pInput  + inSize;
	char              * pOutStart         = pOutput;
	char              * pOutEnd           = pOutput + outSize;
	char              * pSpace            = 0;
	Int32               lenLine           = 0;

	UInt8               ch;
	bool				flagQuote;
	bool				flagCopy;
	Int32				size;

	while (pData < pEnd)
	{
		if (pOutput >= pOutEnd)
			break;

		ch        = *pData;
		flagQuote = false;
		flagCopy  = false;

		// According to RFC 2045, TAB and SPACE MAY be represented as the ASCII characters.
		// But it MUST NOT be so represented at the end of an encoded line.
		if (ch == MimeChar::s_TAB || ch == MimeChar::s_SPACE)
		{
			if ( (pData == (pEnd - 1)) || ((!quoteLineBreak && *(pData + 1) == MimeChar::s_CR)) )
				flagQuote = true;		// quote the SPACE/TAB
			else
				flagCopy = true;		// copy the SPACE/TAB

			if (lenLine > 0)
				pSpace = pOutput;
		}
		else if (!quoteLineBreak && (ch == MimeChar::s_CR || ch == MimeChar::s_LF))
		{
			flagCopy = true;			// keep 'hard' line break
			lenLine  = -1;
			pSpace   = 0;
		}
		else if (!quoteLineBreak && ch == '.')
		{
			if ( (pData - pInput >= 2) &&
				 (*(pData - 2) == MimeChar::s_CR) && (*(pData - 1) == MimeChar::s_LF) &&
				 (*(pData + 1) == MimeChar::s_CR) && (*(pData + 2) == MimeChar::s_LF)    )
				flagQuote = true;		// avoid confusing with SMTP's message end flag
			else
				flagCopy = true;
		}
		else if (ch < 33 || ch > 126 || ch == '=')
			flagQuote = true;			// quote this character
		else if ( (ENC_STYLE_HEADER == style) && (ch == '_') )
			flagQuote = true;
		else
			flagCopy = true;			// copy this character

		if ( (ENC_STYLE_HEADER != style)									&&
			 (lenLine + (flagQuote ? 3 : 1) >= MimeChar::s_MIMELINE_MAXLEN) && 
			 (pOutput + 3                   <= pOutEnd                    )    )
		{
			if ( (pSpace != 0) && (pSpace < pOutput) )
			{
				pSpace++;
				size = static_cast<Int32>(pOutput - pSpace);
				::memmove(pSpace + 3, pSpace, size);
				lenLine = size;
			}
			else
			{
				pSpace  = pOutput;
				lenLine = 0;
			}

			::memcpy(pSpace, "=\r\n", 3);
			pOutput += 3;
			pSpace   = 0;
		}

		if ( flagQuote && (pOutput+3 <= pOutEnd) )
		{
			*pOutput++ = '=';
			*pOutput++ = s_QPTABLE[(ch >> 4) & 0x0f];
			*pOutput++ = s_QPTABLE[ch        & 0x0f];

			lenLine += 3;
		}
		else if (flagCopy)
		{
			*pOutput++ = ch;
			lenLine++;
		}

		pData++;
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//------------------------------------------------------------------------------
//! \return actual length for encoded unit data
//! \throw  no-throw 
Int32 QuotedPrint::wUnitEncode (ConstString pInput , Int32 inSize,
								char *      pOutput, Int32 outSize,
								ConstString charset)
{
	const char * pData      = pInput;
	const char * pEnd       = pInput + inSize;
	char       * pOutStart  = pOutput;
	char       * pOutEnd    = pOutput + outSize;

	Int32        codelen;
	Int32        charsetlen = static_cast<Int32>(strlen(charset));
	Int32        linelen    = 0;
	Int32        maxline    = MimeChar::s_ENCODEDWORD_MAXLEN - charsetlen - 7;
	UInt8        ch;

	while (pData < pEnd)
	{
		ch = *pData++;

		if ( (ch  <  33) || (ch  > 126) || (ch == '=') || (ch == '?') || (ch == '_') )
			codelen = 3;
		else
			codelen = 1;

		if ( (linelen + codelen) > maxline)	// add encoded word tailer
		{
			if ( (pOutput + 3) > pOutEnd)
				break;

			*pOutput++ = '?';
			*pOutput++ = '=';
			*pOutput++ = ' ';

			linelen = 0;
		}

		if (!linelen)						// add encoded word header
		{
			if ( (pOutput + charsetlen + 7) > pOutEnd)
				break;

			*pOutput++ = '=';
			*pOutput++ = '?';
			::memcpy(pOutput, charset, charsetlen);
			pOutput += charsetlen;

			*pOutput++ = '?';
			*pOutput++ = 'Q';
			*pOutput++ = '?';
		}

		linelen += codelen;

		if ( (pOutput + codelen) > pOutEnd)
			break;

		if (codelen > 1)
		{
			*pOutput++ = '=';
			*pOutput++ = s_QPTABLE[(ch >> 4) & 0x0f];
			*pOutput++ = s_QPTABLE[ch & 0x0f];
		}
		else
			*pOutput++ = ch;
	}

	if ( (pOutput + 2) <= pOutEnd)
	{
		*pOutput++ = '?';
		*pOutput++ = '=';
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//------------------------------------------------------------------------------
//! \return actual length for encoded data
//! \throw  no-throw 
Int32 QuotedPrint::wencode(ConstString pInput , Int32 inSize,
						  char       * pOutput, Int32 outSize, 
						  ConstString  charset)
{
	if ( (0 == charset) || ('\0' == *charset) )
	{
		assert(0);
		return 0;
	}


	char       * pOutStart = pOutput;
	char       * pOutEnd   = pOutput + outSize;
	const char * pData     = pInput;
	Int32        inlen     = inSize;
	char         delim;
	Int32        nonAsciiChars;
	I18nString   strUnit;
	Int32        unitsize;
	Int32        encodedlen;
	Int32        copysize;

	// divide the field into syntactic units to encode
	for (;;)
	{
		unitsize = findSymbol(pData, inlen, delim, nonAsciiChars);

		if (!nonAsciiChars)
			strUnit.assign(pData, unitsize);
		else
		{
			strUnit.resize(getMaxWUnitLength(pData, unitsize, charset));
			// I know const_cast is dirty ;-)
			encodedlen = wUnitEncode (pData                              , unitsize, 
				                      const_cast<char *>(strUnit.c_str()), static_cast<Int32>(strUnit.capacity()), 
									  charset);
			strUnit.resize(encodedlen);
		}

		if (unitsize < inlen)
			strUnit += pData[unitsize];		// add the following delimeter (space or special char)

		// copy the encoded string to target buffer
		copysize = std::min<Int32>(static_cast<Int32>(pOutEnd - pOutput), static_cast<Int32>(strUnit.size()));
		::memcpy(pOutput, strUnit.c_str(), copysize);
		pOutput += copysize;

		pData += unitsize + 1;
		inlen -= unitsize + 1;

		if (inlen <= 0)
			break;
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//------------------------------------------------------------------------------
//! \return actual length for decoded data
//! \throw  no-throw 
Int32 QuotedPrint::decode(ConstString pInput , Int32 inSize,
						  char      * pOutput, Int32 outSize)
{
	const char * pData     = pInput;
	const char * pEnd      = pInput + inSize;
	char       * pOutStart = pOutput;
	char       * pOutEnd   = pOutput + outSize;
	char         ch;

	while (pData < pEnd)
	{
		if (pOutput >= pOutEnd)
			break;

		ch = *pData++;

		if ('=' == ch)
		{
			if (pData+2 > pEnd)
				break;				// invalid endcoding

			ch = *pData++;

			if (MimeChar::isHexDigit(ch))
			{
				ch -= (ch > '9') ? 0x37 : '0';
				*pOutput = ch << 4;

				ch = *pData++;
				ch -= (ch > '9') ? 0x37 : '0';
				*pOutput++ |= ch & 0x0f;
			}
			else if ( (ch == MimeChar::s_CR) && (*pData == MimeChar::s_LF) )
				pData++;			// soft line break, eat it
			else					// invalid endcoding, let it go
				*pOutput++ = ch;
		}
		else	// if (ch != MimeChar::s_CR && ch != MimeChar::s_LF)
			*pOutput++ = ch;
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//------------------------------------------------------------------------------
//! \return actual length for decoded data (word-encoded)
//! \throw  no-throw 
Int32 QuotedPrint::wdecode (ConstString pInput , Int32 inSize,
							char      * pOutput, Int32 outSize)
{
	const char * pData      = pInput;
	const char * pEnd       = pData + inSize;
	char       * pOutStart  = pOutput;
	const char * pHeaderEnd;
	const char * pCodeEnd;
	Int32        coding;
	Int32        codelen;

	while (pData < pEnd)
	{
		pHeaderEnd = pData;
		pCodeEnd   = pEnd;

		coding  = 0;
		codelen = static_cast<Int32>(pEnd - pData);

		if ((pData[0] == '=') && 
			(pData[1] == '?')    )	// it might be an encoded-word
		{
			pHeaderEnd = ::strchr(pData + 2, '?');

			if ((pHeaderEnd    !=  0  ) && 
				(pHeaderEnd[2] == '?' ) && 
				(pHeaderEnd+3  <  pEnd)    )
			{
				coding      = tolower(pHeaderEnd[1]);
				pHeaderEnd += 3;
				pCodeEnd    = ::strstr(pHeaderEnd, "?=");	// look for the tailer

				if ( !pCodeEnd || (pCodeEnd >= pEnd) )
					pCodeEnd = pEnd;

				codelen   = static_cast<Int32>(pCodeEnd - pHeaderEnd);
				pCodeEnd += 2;
			}
		}

		Int32        decodedlen;
		const char * pSpace;

		if (coding == 'q')
			decodedlen = decode(pHeaderEnd, codelen, pOutput, outSize);
		else
		{
			assert(coding != 'b');	// base64 not supported

			pCodeEnd = ::strstr(pData + 1, "=?");	// find the next encoded-word

			if ( !pCodeEnd || (pCodeEnd >= pEnd) )
				pCodeEnd = pEnd;
			else 
			{
				if (pData > pInput)
				{
					pSpace = pData;

					while (MimeChar::isSpace(*pSpace))
						pSpace++;

					if (pSpace == pCodeEnd)		// ignore liner-white-spaces between adjacent encoded words
						pData = pCodeEnd;
				}
			}

			decodedlen = std::min<Int32>(static_cast<Int32>(pCodeEnd - pData), outSize);

			::memcpy(pOutput, pData, decodedlen);
		}

		pData    = pCodeEnd;
		pOutput += decodedlen;
		outSize -= decodedlen;

		if (outSize <= 0)
			break;
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

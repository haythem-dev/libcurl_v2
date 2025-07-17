//----------------------------------------------------------------------------
/*! \file
 *  \brief  base64 codec class
 *  \author Michael Eichenlaub
 *  \date   07.12.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_codec.h"

#include <string.h>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
//! \brief flag for adding line break
//! \return flag for adding line break
//! \throw no-throw
static bool flagAddLineBreak()
{
	return true;
}

//------------------------------------------------------------------------------
//! \return base64 decoded character
//! \throw  no-throw 
char Base64::decodeChar(char code)
{
	if (code >= 'A' && code <= 'Z')
		return code - 'A';

	if (code >= 'a' && code <= 'z')
		return code - 'a' + 26;

	if (code >= '0' && code <= '9')
		return code - '0' + 52;

	if (code == '+')
		return 62;

	if (code == '/')
		return 63;

	return 64;
}

//------------------------------------------------------------------------------
//! \return maximum length for to be encoded data
//! \throw  no-throw 
Int32 Base64::getMaxEncodeLength (Int32 inSize)
{
	Int32 len = (inSize + 2) / 3 * 4;

	if (flagAddLineBreak())
		len += (len / MimeChar::s_MIMELINE_MAXLEN + 1) * 2;

	return len;
}

//------------------------------------------------------------------------------
//! \return maximum length for to be decoded data
//! \throw  no-throw 
Int32 Base64::getMaxDecodeLength(Int32 inSize)
{
	return inSize * 3 / 4 + 2;
}

//------------------------------------------------------------------------------
//! \return actual length for encoded data
//! \throw  no-throw 
Int32 Base64::encode(ConstString pInput , Int32 inSize,
					 char      * pOutput, Int32 outSize)
{
	static ConstString BASE64TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		                             "abcdefghijklmnopqrstuvwxyz"
									 "0123456789+/";

	char * pOutStart   = pOutput;
	char * pOutEnd     = pOutput + outSize;
	Int32  lenLine     = 0;
	UInt8  chHigh4bits = 0;		// regard unsigned (for bit shifting)!
	UInt8  ch;					// regard unsigned (for bit shifting)!
	Int32  from;

	for (from = 0; from < inSize; ++from)
	{
		if (pOutput >= pOutEnd)
			break;

		ch = pInput[from];

		switch (from % 3)
		{
		case 0:
			*pOutput++  = BASE64TABLE[ch >> 2];
			chHigh4bits = (ch << 4) & 0x30;
			break;

		case 1:
			*pOutput++  = BASE64TABLE[chHigh4bits | (ch >> 4)];
			chHigh4bits = (ch << 2) & 0x3c;
			break;

		default:
			*pOutput++ = BASE64TABLE[chHigh4bits | (ch >> 6)];

			if (pOutput < pOutEnd)
			{
				*pOutput++ = BASE64TABLE[ch & 0x3f];
				lenLine++;
			}
		}

		++lenLine;

		if ( flagAddLineBreak()                             && 
			 (lenLine       >= MimeChar::s_MIMELINE_MAXLEN) && 
			 ((pOutput + 2) <= pOutEnd                    )    )
		{
			*pOutput++ = MimeChar::s_CR;
			*pOutput++ = MimeChar::s_LF;
			lenLine    = 0;
		}
	}

	if ( ((from % 3) != 0) && (pOutput < pOutEnd))	// 76 = 19 * 4, so the padding wouldn't exceed 76
	{
		*pOutput++ = BASE64TABLE[chHigh4bits];
		Int32 pad  = 4 - (from % 3) - 1;

		if (pOutput + pad <= pOutEnd)
		{
			::memset(pOutput, '=', pad);
			pOutput += pad;
		}
	}

	if ( flagAddLineBreak()         && 
		 (lenLine       != 0      ) && 
		 ((pOutput + 2) <= pOutEnd)    )	// add CRLF
	{
		*pOutput++ = MimeChar::s_CR;
		*pOutput++ = MimeChar::s_LF;
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//------------------------------------------------------------------------------
//! \return actual length for decoded data
//! \throw  no-throw 
Int32 Base64::decode(ConstString pInput , Int32 inSize,
					 char      * pOutput, Int32 outSize)
{
	const char * pData      = pInput;
	const char * pEnd       = pInput + inSize;
	char       * pOutStart  = pOutput;
	char       * pOutEnd    = pOutput + outSize;
	Int32        from       = 0;
	UInt8        chHighBits = 0;	// regard unsigned (for bit shifting)!
	UInt8        ch;				// regard unsigned (for bit shifting)!

	while (pData < pEnd)
	{
		if (pOutput >= pOutEnd)
			break;

		ch = *pData++;

		if (ch == MimeChar::s_CR || ch == MimeChar::s_LF)
			continue;

		ch = decodeChar(ch);

		if (ch >= 64)				// invalid encoding, or trailing pad '='
			break;

		switch ((from++) % 4)
		{
		case 0:
			chHighBits = ch << 2;
			break;

		case 1:
			*pOutput++ = chHighBits | (ch >> 4);
			chHighBits = ch << 4;
			break;

		case 2:
			*pOutput++ = chHighBits | (ch >> 2);
			chHighBits = ch << 6;
			break;

		default:
			*pOutput++ = chHighBits | ch;
		}
	}

	return static_cast<Int32>(pOutput - pOutStart);
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

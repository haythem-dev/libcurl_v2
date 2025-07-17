//----------------------------------------------------------------------------
/*! \file
 *  \brief  MIME charclass
 *  \author Michael Eichenlaub
 *  \date   08.12.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_codec.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
const Int32 MimeChar::s_MIMELINE_MAXLEN		= 76  ;		//!< maximum line length for MIME encoding 
const Int32 MimeChar::s_ENCODEDWORD_MAXLEN	= 75  ;		//!< maximum length for MIME encoded-word

const char  MimeChar::s_CR					= '\r';		//!< carriage return
const char  MimeChar::s_LF					= '\n';		//!< line feed
const char  MimeChar::s_SPACE				= ' ' ;		//!< space
const char  MimeChar::s_TAB					= '\t';		//!< (horizontal) tabulator

//! character table for MIME messages
const char  MimeChar::s_CHARTBL[128]		=
{
	CONTROL,						// 00 (NUL)
	CONTROL,						// 01 (SOH)
	CONTROL,						// 02 (STX)
	CONTROL,						// 03 (ETX)
	CONTROL,						// 04 (EOT)
	CONTROL,						// 05 (ENQ)
	CONTROL,						// 06 (ACK)
	CONTROL,						// 07 (BEL)
	CONTROL,						// 08 (BS)
	SPACE | CONTROL,				// 09 (HT)
	SPACE | CONTROL,				// 0A (LF)
	SPACE | CONTROL,				// 0B (VT)
	SPACE | CONTROL,				// 0C (FF)
	SPACE | CONTROL,				// 0D (CR)
	CONTROL,						// 0E (SI)
	CONTROL,						// 0F (SO)
	CONTROL,						// 10 (DLE)
	CONTROL,						// 11 (DC1)
	CONTROL,						// 12 (DC2)
	CONTROL,						// 13 (DC3)
	CONTROL,						// 14 (DC4)
	CONTROL,						// 15 (NAK)
	CONTROL,						// 16 (SYN)
	CONTROL,						// 17 (ETB)
	CONTROL,						// 18 (CAN)
	CONTROL,						// 19 (EM)
	CONTROL,						// 1A (SUB)
	CONTROL,						// 1B (ESC)
	CONTROL,						// 1C (FS)
	CONTROL,						// 1D (GS)
	CONTROL,						// 1E (RS)
	CONTROL,						// 1F (US)
	SPACE,							// 20 SPACE
	DOT,							// 21 !
	DOT | SPECIAL,					// 22 "
	DOT,							// 23 #
	DOT,							// 24 $
	DOT,							// 25 %
	DOT,							// 26 &
	DOT,							// 27 '
	DOT | SPECIAL,					// 28 (
	DOT | SPECIAL,					// 29 )
	DOT,							// 2A *
	DOT,							// 2B +
	DOT | SPECIAL,					// 2C ,
	DOT,							// 2D -
	DOT | SPECIAL,					// 2E .
	DOT,							// 2F /
	PRINT | HEXDIGIT,				// 30 0
	PRINT | HEXDIGIT,				// 31 1
	PRINT | HEXDIGIT,				// 32 2
	PRINT | HEXDIGIT,				// 33 3
	PRINT | HEXDIGIT,				// 34 4
	PRINT | HEXDIGIT,				// 35 5
	PRINT | HEXDIGIT,				// 36 6
	PRINT | HEXDIGIT,				// 37 7
	PRINT | HEXDIGIT,				// 38 8
	PRINT | HEXDIGIT,				// 39 9
	DOT | SPECIAL,					// 3A :
	DOT | SPECIAL,					// 3B ;
	DOT | SPECIAL,					// 3C <
	DOT | SPECIAL,					// 3D =
	DOT | SPECIAL,					// 3E >
	DOT,							// 3F ?
	DOT | SPECIAL,					// 40 @
	PRINT | HEXDIGIT,				// 41 A
	PRINT | HEXDIGIT,				// 42 B
	PRINT | HEXDIGIT,				// 43 C
	PRINT | HEXDIGIT,				// 44 D
	PRINT | HEXDIGIT,				// 45 E
	PRINT | HEXDIGIT,				// 46 F
	PRINT,							// 47 G
	PRINT,							// 48 H
	PRINT,							// 49 I
	PRINT,							// 4A J
	PRINT,							// 4B K
	PRINT,							// 4C L
	PRINT,							// 4D M
	PRINT,							// 4E N
	PRINT,							// 4F O
	PRINT,							// 50 P
	PRINT,							// 51 Q
	PRINT,							// 52 R
	PRINT,							// 53 S
	PRINT,							// 54 T
	PRINT,							// 55 U
	PRINT,							// 56 V
	PRINT,							// 57 W
	PRINT,							// 58 X
	PRINT,							// 59 Y
	PRINT,							// 5A Z
	DOT | SPECIAL,					// 5B [
	DOT | SPECIAL,					// 5C '\'
	DOT | SPECIAL,					// 5D ]
	DOT,							// 5E ^
	DOT,							// 5F _
	DOT,							// 60 `
	PRINT,							// 61 a
	PRINT,							// 62 b
	PRINT,							// 63 c
	PRINT,							// 64 d
	PRINT,							// 65 e
	PRINT,							// 66 f
	PRINT,							// 67 g
	PRINT,							// 68 h
	PRINT,							// 69 i
	PRINT,							// 6A j
	PRINT,							// 6B k
	PRINT,							// 6C l
	PRINT,							// 6D m
	PRINT,							// 6E n
	PRINT,							// 6F o
	PRINT,							// 70 p
	PRINT,							// 71 q
	PRINT,							// 72 r
	PRINT,							// 73 s
	PRINT,							// 74 t
	PRINT,							// 75 u
	PRINT,							// 76 v
	PRINT,							// 77 w
	PRINT,							// 78 x
	PRINT,							// 79 y
	PRINT,							// 7A z
	DOT,							// 7B {
	DOT,							// 7C |
	DOT,							// 7D }
	DOT,							// 7E ~
	CONTROL							// 7F (DEL)
};

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is HexDigit
bool MimeChar::isHexDigit(char ch)
{
	return !(ch & 0x80) && (s_CHARTBL[static_cast<Int32>(ch)] & HEXDIGIT) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is printable
bool MimeChar::isPrintable(char ch)
{ 
	return !(ch & 0x80) && (s_CHARTBL[static_cast<Int32>(ch)] & PRINT) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is non ascii
bool MimeChar::isNonAscii(char ch) 
{ 
	return (ch & 0x80) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is control character
bool MimeChar::isControl(char ch) 
{ 
	return !(ch & 0x80) && (s_CHARTBL[static_cast<Int32>(ch)] & CONTROL) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is space
bool MimeChar::isSpace(char ch) 
{ 
	return !(ch & 0x80) && (s_CHARTBL[static_cast<Int32>(ch)] & SPACE) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is special
bool MimeChar::isSpecial(char ch) 
{ 
	return !(ch & 0x80) && (s_CHARTBL[static_cast<Int32>(ch)] & SPECIAL) != 0; 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is delimiter
bool MimeChar::isDelimiter(char ch) 
{ 
	return isSpace(ch) || isSpecial(ch); 
}

//------------------------------------------------------------------------------
//! \throw no-throw 
//! \return bool indicating ch is token
bool MimeChar::isToken(char ch) 
{ 
	return isNonAscii(ch) || (ch > s_SPACE && !isSpecial(ch)); 
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

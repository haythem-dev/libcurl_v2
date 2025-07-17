//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of command line tokenizer
 *  \author Thomas Hörath
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_bstring.h"

#include "libbasarcmnutil_exceptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sstream>
#include <algorithm>

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define vsnprintf	_vsnprintf		//!< use MS function
#define snprintf	_snprintf		//!< use MS function
#endif

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
/*!	\throw no-throw 
    standard constructor -> empty allocated BString */
BString::BString()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
BString::BString ( const value_type ch ) 
                 : CharString(sizeof(value_type), ch)
{
} 

//----------------------------------------------------------------------------
/*!	\throw no-throw */
BString::BString ( const value_type * pStr )
                 : CharString(pStr)
{
} 

//----------------------------------------------------------------------------
/*!	\throw no-throw */
BString::BString ( const CharString & rStr) 
                 : CharString(rStr)
{
} 

//----------------------------------------------------------------------------
/*!	\throw no-throw */
BString::BString ( const BString & rStr ) 
                 : CharString(rStr)
{
} 

//----------------------------------------------------------------------------
/*!	\throw no-throw */
BString::BString ( const BString & rStr, 
				   const size_type off, 
				   const size_type count)
                 : CharString(rStr, off, count)
{
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
BString::BString ( const_iterator first, 
				   const_iterator last ) 
				 : CharString(first, last) 
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
    virtual destructor */
BString::~BString()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return right part of string */
const BString BString::right( const value_type ch ) const
{
    size_type p = find(ch);
	return BString(begin() + ((p == npos) ? 0 : (p + sizeof(ch))), 
		           end  ()                                       );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return right part of string */
const BString BString::right( const BString & rStr ) const
{
    size_type p = find(rStr);
	return BString(begin() + ((p == npos) ? 0 : (p + rStr.length())),
		           end  ()                                          );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return left part of string */
const BString BString::left( const value_type ch ) const
{
    size_type p = find(ch);
	return BString(begin(),
		           begin() + ((p == npos) ? length() : p));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return left part of string */
const BString BString::left( const BString & rStr ) const
{
    size_type p = find(rStr);
	return BString(begin(),
		           begin() + ((p == npos) ? length() : p));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return substring */
const BString BString::substr( const size_type off, 
							   const size_type count /* = npos */) const
{
	return CharString::substr( off, count );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if ALL characters are numeric or leading sign
	\retval false otherwise */
bool BString::isDigit() const
{
    if ( empty() )
        return false;

    const_iterator itStr = begin();
	const_iterator itEnd = end  ();

	// skip leading spaces
	while ( (itStr != itEnd) && (::isspace(static_cast<UInt8>(*itStr)) > 0) )		// ascii function
		 ++itStr; 

	if ( itStr == itEnd )	
        return false;		// only spaces

	// skip 1 sign char
    if ( (*itStr == '-') || (*itStr == '+') )			
		++itStr; 
    
	if ( itStr == itEnd )	
        return false;		// no digits

	// skip digits
	while ( (itStr != itEnd) && ::isdigit(static_cast<UInt8>(*itStr)) > 0)			// ascii function
		++itStr;

	// skip trailing spaces
	while ( (itStr != itEnd) && (::isspace(static_cast<UInt8>(*itStr)) > 0) )		// ascii function
		 ++itStr; 

	return (itStr == itEnd);
} 

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return string converted to long */
BLong BString::stol () const
{
	return ::atol(c_str());
}
    
//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return string converted to int */
basar::Int32	BString::stoi () const
{
	return ::atoi(c_str());
}

basar::Int64 BString::stoll() const
{
    basar::Int64 value;

    std::istringstream is(*this);

    is >> value;

    return value;
}
	
//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return string converted to double (locale C) */
basar::Float64 BString::stof () const
{
	return ::atof(c_str());
}
    
//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return int converted to string */
void BString::itos(Int32 number)
{
	std::ostringstream out;

	out << number;
	assign(out.str());
}
    
//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return long converted to string */
void BString::ltos( Long32 number )
{
	std::ostringstream out;

	out << number;
	assign(out.str());
}

void BString::lltos(Int64 number)
{
    std::ostringstream out;
    out << number;

    assign(out.str());
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return double converted to string  (locale C) */
void BString::ftos( Float64 number, Int32 precision /*= 6*/ )
{
	std::ostringstream out;

    out.precision(precision);

	out << std::fixed << number;
	assign(out.str());

    eraseTrailingZeros(*this);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::eraseTrailingZeros(basar::VarString& str)
{
	iterator       itCur   = str.end  () - 1;
	const_iterator itBegin = str.begin()    ;

	while (itBegin <= itCur)
	{
		if ( '0' != *itCur )
			break;

		--itCur;
	}

	str.erase( itCur + 1, str.end() );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::lower()
{
    if ( empty() )
        return;

    iterator       itCur = begin();
	const_iterator itEnd = end  ();

    while( itCur != itEnd )
    {
		*itCur = static_cast<value_type>(::tolower(static_cast<UInt8>(*itCur)));

		++itCur;        
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
const BString BString::toLower() const
{
    BString tmp(*this);

    tmp.lower();

    return tmp;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::upper()
{
    if ( empty() )
        return;

    iterator       itCur = begin();
	const_iterator itEnd = end  ();

    while( itCur != itEnd )
    {
		*itCur = static_cast<value_type>(::toupper(static_cast<UInt8>(*itCur)));

		++itCur;        
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
const BString BString::toUpper() const
{
    BString tmp(*this);

    tmp.upper();

    return tmp;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::trim (TrimPosEnum	eWhere /* = END */)
{
    if ( empty() )
        return; 

	switch (eWhere)
	{
	case ALL:
		eraseAllSpace();
		break;

	case FIRST:
		 eraseFirstSpace();
		 break;

	case END:
		eraseEndSpace();
		break;

	case FIRST_END:
		eraseFirstEndSpace();
		break;

	default:
		assert(0);
		break;
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::cleanchar()
{
    if ( empty() )
        return;

    iterator       itCur = begin();
	const_iterator itEnd = end  ();

	// check for printable character including space (0x20 – 0x7E)
    while( itCur != itEnd )
    {
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
		if ( ! ::isprint(static_cast<UInt8>(*itCur)) || 
			   ::iscntrl(static_cast<UInt8>(*itCur))    )  // Windows strange behaviour for TAB 0x09
#else
		if ( ! ::isprint(static_cast<UInt8>(*itCur)) )
#endif
			*itCur = 0x20;		// set to space

        ++itCur;        
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::reverse( const size_type pos )
{
    if ( empty() )
        return; 

    value_type ch; 
    size_type  len = length() + pos;                          

    for( size_type i = pos; i < len / 2; ++i )          
    {                                  
        ch = at( i );

        replace( i      , 1, 1, at(len-1-i) );            
        replace( len-1-i, 1, 1, ch          );
    }                                  
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::exchange( const value_type ch, ConstString to )
{
    if ( empty() )
        return; 

    size_type pos = 0;

	while ( npos != (pos = find(ch, pos)) )
	{
		replace(pos, sizeof(ch), to);
		pos += strlen(to);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::exchange( const BString & from, const BString & to )
{       
    if ( empty() )
        return; 

    size_type pos     = 0;
    size_type lenFrom = from.length();
    size_type lenTo   = to  .length();

	while ( npos != (pos = find(from, pos)) )
	{
		replace(pos, lenFrom, to);
		pos += lenTo;
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::remove( const BString & str )
{
    if ( empty() || str.empty())
        return; 

    size_type pos = 0;
    size_type len = str.length();

	while ( npos != (pos = find(str, pos)) )
		erase(pos, len);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return number of occurency for given char */
BString::size_type BString::getCount( const value_type ch ) const
{
	size_type count = 0;
    size_type pos   = 0;

	while ( npos != (pos = find(ch, pos)) )
	{
		++count;
		pos += sizeof(ch);
	}

    return count;
}  

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::eraseEndSpace()
{
	if (!empty())
	{
		reverse_iterator       itCur   = rbegin  ();
		const_reverse_iterator itBegin = rend() - 1;

		while (itBegin >= itCur)
	{
		if ( ! ::isspace(static_cast<UInt8>(*itCur)) )
			break;

			++itCur;
	}

		erase( itCur.base(), end() );
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::eraseFirstSpace()
{
	iterator       itCur = begin();
	const_iterator itEnd = end  ();

	while (itEnd != itCur )
	{
		if ( ! ::isspace(static_cast<UInt8>(*itCur)) )
			break;

		++itCur;
	}

	erase( begin(), itCur );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
void BString::eraseFirstEndSpace()
{
	eraseFirstSpace();
	eraseEndSpace  ();
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::eraseAllSpace()
{
	iterator       itCur = begin();
	const_iterator itEnd = end  ();

    while( itEnd != itCur)
    {
		if ( ::isspace(static_cast<UInt8>(*itCur)) )
		{
            itCur = erase(itCur); 
			itEnd = end();
		}
		else
			++itCur;
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::format(const value_type * pFormat, ...)
{	
	va_list argList;

	va_start(argList, pFormat );
	formatV (pFormat , argList);
	va_end  (argList          );
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIndexException */
void BString::formatV(const value_type * pFormat, va_list argList)
{
#ifdef _WIN32
	//enable %n support
	_set_printf_count_output(1);
#endif

	if (!pFormat || ('\0' == *pFormat) )
		return;

	// calculate length of result string
	// Return Value: The number of characters that *would* have been written if n had been sufficiently large, *not* counting the terminating null character.
	const Int32 maxLen = vsnprintf(NULL, 0, pFormat, argList);

	if( maxLen < 0)
	{
		static const Int32 internLen = 32;
		value_type         bufLen    [internLen];

		::snprintf(bufLen    , internLen - 1, "%d", int(maxLen) );
		BString msg;
		
		msg  = "vsnprintf(len = 0, \"";
		msg += pFormat;
		msg += "\") for length calculation returned ";
		msg += bufLen;

		throw OutOfRangeIndexException(ExceptInfo("basar.cmnutil.BString.formatV", 
			                                      msg, 
												  __FILE__, __LINE__));
	}

	resize(maxLen + 1);	// blow up string, include terminating zero

	const Int32 written = vsnprintf( &(*this)[0], size(), pFormat, argList);

	if ( written != maxLen )
	{
		static const Int32 internLen = 32;
		value_type         bufLen    [internLen];
		value_type         bufWritten[internLen];

		::snprintf(bufLen    , internLen - 1, "%d", int(maxLen) );
		::snprintf(bufWritten, internLen - 1, "%d", written);
		BString msg;
		
		msg  = "vsnprintf(len = ";
		msg += bufLen;
		msg += ", \"";
		msg += pFormat;
		msg += "\") returned ";
		msg += bufWritten;

		throw OutOfRangeIndexException(ExceptInfo("basar.cmnutil.BString.formatV", 
			                                      msg, 
												  __FILE__, __LINE__));
	}

	resize(written);
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//! \return new BString starting from off until delimiter is found
BString BString::substrToDelimiter(const size_type off              ,
                                   const BString & delimiter        , 
                                   bool            acceptNoDelimiter) const
{ 
    BString retval;
    size_type internalLength = length();
    if (internalLength > off)
    {
        internalLength -= off;
        if (delimiter.length() > 0)
        {
            size_type pos = find(delimiter, off);
            if (npos != pos)
            {
                if (pos > off)
                {
                  retval = substr(off, pos-off);
                }
            }
            else if (acceptNoDelimiter)
            {
                retval =  substr(off, internalLength);
            }

        }
        else if (acceptNoDelimiter)
        { 
             retval =  substr(off, internalLength);
        }
    }
    return retval;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
*   \return new BString starting from off until delimiter is found, trimmed according to trimpos
*/
BString BString::substrToDelimiterAndTrim(  const size_type off              ,
											const BString & delimiter        ,
											TrimPosEnum     trimpos          , 
											bool            acceptNoDelimiter) const
{
	BString retval = substrToDelimiter(off, delimiter, acceptNoDelimiter);
	retval.trim(trimpos);
	return retval;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*   \return new BString starting from beginning of string until delimiter is found
*/
BString BString::substrToDelimiter(const BString & delimiter        , 
                                   bool            acceptNoDelimiter) const
{ 
	return substrToDelimiter(0, delimiter, acceptNoDelimiter);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
    \return new BString starting from beginning of string until delimiter is found, trimmed according to trimpos
 */
BString BString::substrToDelimiterAndTrim (	const BString & delimiter        ,
											TrimPosEnum     trim             , 
											bool            acceptNoDelimiter) const
{
	return substrToDelimiterAndTrim(0, delimiter, trim, acceptNoDelimiter);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::tokenize(CollBString   & vec               , 
                       const BString & delimiter         , 
                       bool            acceptNoDelimiter ,
                       bool            removeEmptyStrings) const
{
    size_type internalLength = length();
    if (internalLength > 0)
    { 
        size_type delimiterLength = delimiter.length();
        if (delimiterLength > 0)
        {
            size_type off = 0;
            size_type pos = find(delimiter, off);
            while (npos != pos)
            {
                if (pos > off)
                {
                    vec.push_back(substr(off, pos-off));
                }
                else if (pos == off && !removeEmptyStrings)
                {
                    vec.push_back("");
                }
                off = pos + delimiterLength;
                pos = find(delimiter, off);
            }
            if (acceptNoDelimiter && off < internalLength)
            {
                vec.push_back(substr(off, internalLength - off));
            }

        }
        else if (acceptNoDelimiter)
        { 
            vec.push_back(*this);
        }
    }

}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void BString::tokenizeAndTrim(CollBString   & vec               ,
                              const BString & delimiter         , 
                              TrimPosEnum     trimpos           , 
                              bool            acceptNoDelimiter ,
                              bool            removeEmptyStrings) const
{
	CollBString::size_type length = vec.size();
    tokenize(vec, delimiter, acceptNoDelimiter,removeEmptyStrings);

    CollBString::size_type length2 = vec.size();

    if (length < length2)
    {
        for (CollBString::size_type i = length; i < length2; ++i)
        {
            (vec[i]).trim(trimpos);
        }
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*   \return CollBString containing tokens split at delimiter
*/
CollBString BString::tokenize(const BString & delimiter         , 
                              bool            acceptNoDelimiter ,
                              bool            removeEmptyStrings) const
{
    CollBString retval;
    tokenize(retval, delimiter, acceptNoDelimiter,removeEmptyStrings);
    return retval;

}

//----------------------------------------------------------------------------
/*!	\throw no-throw
    \return CollBString containing tokens split at delimiter and trimmed according to trimpos
*/
CollBString BString::tokenizeAndTrim(const BString & delimiter         , 
                                     TrimPosEnum     trimpos           , 
                                     bool            acceptNoDelimiter ,
                                     bool            removeEmptyStrings) const
{
    CollBString retval;
    tokenizeAndTrim(retval, delimiter, trimpos, acceptNoDelimiter,removeEmptyStrings);
    return retval;
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

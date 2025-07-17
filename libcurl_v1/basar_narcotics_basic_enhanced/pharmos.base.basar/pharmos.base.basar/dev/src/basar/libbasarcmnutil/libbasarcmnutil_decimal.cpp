//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of informix Decimal
 *  \author Thomas Hörath
 *  \date   18.09.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_decimal.h"

#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_locale.h"
#include "libbasardebug.h"

#include <string.h>

#include <sqlhdr.h>

#include <ostream>

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
OverflowException::OverflowException (const ExceptInfo& sInfo )
                                     : basar::Exception(sInfo, "OverflowException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
UnderflowException::UnderflowException (const ExceptInfo& sInfo )
                                       : basar::Exception(sInfo, "UnderflowException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
DivisionByZeroException::DivisionByZeroException (const ExceptInfo& sInfo )
                                                 : basar::Exception(sInfo, "DivisionByZeroException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
NonNumericException::NonNumericException (const ExceptInfo& sInfo )
                                         : basar::Exception(sInfo, "NonNumericException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
InvalidExponentException::InvalidExponentException (const ExceptInfo& sInfo )
                                                   : basar::Exception(sInfo, "InvalidExponentException")
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	sInfo struct with output-info */
NullValueException::NullValueException (const ExceptInfo& sInfo )
                                       : basar::Exception(sInfo, "NullValueException")
{
}

//----------------------------------------------------------------------------
void dumpDecimal(size_t addr)
{
	if (0 == addr)
		return;

	const Decimal * const pDec = reinterpret_cast<const Decimal * const>(addr);

	VarString out;

	out.format("#0x%0*lx: %s\n",
		       sizeof(size_t) * 2, pDec,
			   pDec->toNonLocString().c_str());

	debug::printDbgMsg(out.c_str());
}

//----------------------------------------------------------------------------
Int16 Decimal::getAllDecimalPlaces()
{
	return -1;
}

int Decimal::getInfxDescSize()
{
    return sizeof(decimal);
}

//----------------------------------------------------------------------------
Decimal::Decimal() : m_ownsDec(true), m_Dec(new decimal())
{
	if( 0 != deccvdbl( 0.0, m_Dec) )
	{
		m_Dec->dec_exp    =  0;
		m_Dec->dec_pos   = -1;
		m_Dec->dec_ndgts =  0;
		memset(m_Dec->dec_dgts, 0, DECSIZE);
		
		ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal()", "conversion from 0.0 to Decimal failed", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
}

//----------------------------------------------------------------------------
/*! \param rDecVal Decimal with which new Decimal will be allocated */
Decimal::Decimal (const Decimal & rDecVal) : m_ownsDec(true), m_Dec(new decimal())
{
    memcpy(m_Dec, rDecVal.m_Dec, sizeof(decimal));
}
	
//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	Value Int16 with which new Decimal will be allocated */
Decimal::Decimal(const Int16 Value) : m_ownsDec(true), m_Dec(new decimal())
{
	if( 0 != deccvint(Value, m_Dec) )
	{
		m_Dec->dec_exp    =  0;
		m_Dec->dec_pos   = -1;
		m_Dec->dec_ndgts =  0;
		memset(m_Dec->dec_dgts, 0, DECSIZE);
		
		ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal(Int16)", "conversion from Int16 to Decimal failed", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	Value Int32 with which new Decimal will be allocated */
Decimal::Decimal(const Int32 Value) : m_ownsDec(true), m_Dec(new decimal())
{
	if ( 0 != deccvint(Value, m_Dec) )
	{
		m_Dec->dec_exp   =  0;
		m_Dec->dec_pos   = -1;
		m_Dec->dec_ndgts =  0;
		memset(m_Dec->dec_dgts, 0, DECSIZE);
	
		ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal(Int32)", "conversion from Int32 to Decimal failed", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	Value Float32 with which new Decimal will be allocated */
Decimal::Decimal(const Float32 Value) : m_ownsDec(true), m_Dec(new decimal())
{
	if ( 0 != deccvdbl( Value, m_Dec) )
	{
		m_Dec->dec_exp    =  0;
		m_Dec->dec_pos   = -1;
		m_Dec->dec_ndgts =  0;
		memset(m_Dec->dec_dgts, 0, DECSIZE);
		
		ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal(Float32)", "conversion from Float32 to Decimal failed", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	Value Float64 with which new Decimal will be allocated */
Decimal::Decimal(const Float64 Value) : m_ownsDec(true), m_Dec(new decimal())
{
	if (0 != deccvdbl(Value, m_Dec) )
	{
		m_Dec->dec_exp    =  0;
		m_Dec->dec_pos   = -1;
		m_Dec->dec_ndgts =  0;
		memset(m_Dec->dec_dgts, 0, DECSIZE);
	
		ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal(Float64)", "conversion from Float64 to Decimal failed", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rDecStr I18nString to be converted to a Decimal */
Decimal::Decimal(const I18nString & rDecStr) : m_ownsDec(true), m_Dec(new decimal())
{
	fromString(rDecStr);
}

//----------------------------------------------------------------------------
Decimal::Decimal(const decimal * const pDec) : m_ownsDec(false)
{
    m_Dec = const_cast<decimal *>(pDec);
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
Decimal::~Decimal( )
{
    if (m_ownsDec)
    {
        delete m_Dec;
        m_Dec = 0;
    }
}

//----------------------------------------------------------------------------
/*! \throw  UnderflowException, OverflowException
	\param	rDecVal Decimal to add 
	\return result of addition */
const Decimal Decimal::operator + (const Decimal & rDecVal) const
{
	Decimal infDec;
	switch (decadd(const_cast<decimal *>(m_Dec  ),
                   const_cast<decimal *>(rDecVal.m_Dec),
                   infDec.m_Dec )                    )
	{
	case INF_OVERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator+", "operation resulted in overflow", __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	case INF_UNDERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.+", "operation resulted in underflow", __FILE__, __LINE__);
		throw UnderflowException(sInfo);
	}

	default:
		break;
	}

	return infDec;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to add 
	\return result of addition */
const Decimal Decimal::operator + (const Int32 rIntVal) const
{
	return (operator + (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  UnderflowException, OverflowException
	\param	rDecVal Decimal to subtract
	\return result of subtraction */
const Decimal Decimal::operator-(const Decimal& rDecVal) const
{
	Decimal infDec;
	
	switch (decsub(const_cast<decimal *>(m_Dec  ),
			 	   const_cast<decimal *>(rDecVal.m_Dec),
                   infDec.m_Dec)                     )
	{
	case INF_OVERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator-", "Operation  resulted in overflow", __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	case INF_UNDERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator-", "Operation  resulted in underflow", __FILE__, __LINE__);
		throw UnderflowException(sInfo);
	}

	default:
		break;
	}

	return infDec;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to subtract 
	\return result of subtraction */
const Decimal Decimal::operator - (const Int32 rIntVal) const
{
	return (operator - (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  UnderflowException, OverflowException
	\param	rDecVal Decimal to multiply 
	\return result of multiplication */
const Decimal Decimal::operator * (const Decimal& rDecVal) const
{
	Decimal infDec;
	
	switch (decmul(const_cast<decimal *>(m_Dec  ),
                   const_cast<decimal *>(rDecVal.m_Dec),
                   infDec.m_Dec)                     )
	{
	case INF_OVERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator*", "Operation resulted in overflow", __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	case INF_UNDERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator*", "Operation resulted in underflow", __FILE__, __LINE__);
		throw UnderflowException(sInfo);
	}

	default:
		break;
	}

	return infDec;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to multiply 
	\return result of multiplication */
const Decimal Decimal::operator * (const Int32 rIntVal) const
{
	return (operator * (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  UnderflowException, OverflowException, DivisionByZeroException
	\param	rDecVal Decimal to divide by
	\return result of division */
const Decimal Decimal::operator / (const Decimal& rDecVal) const
{
	Decimal infDec;
	
	switch (decdiv(const_cast<decimal *>(m_Dec  ),
                   const_cast<decimal *>(rDecVal.m_Dec),
                   infDec.m_Dec)                     )
	{
	case INF_OVERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator/", "Operation resulted in overflow", __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	case INF_UNDERFLOW:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator/", "Operation  resulted in underflow", __FILE__, __LINE__);
		throw UnderflowException(sInfo);
	}

	case DIVBYZERO:
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator/", "Operation attempted dividing by zero", __FILE__, __LINE__);
		throw DivisionByZeroException(sInfo);
	}

	default:
		break;
	}

	return infDec;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to divide by
	\return result of division */
const Decimal Decimal::operator / (const Int32 rIntVal) const
{
	return (operator / (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	rDecVal Decimal to add 
	\return result of addition */
const Decimal Decimal::operator += (const Decimal& rDecVal)
{
	return (operator= (operator+ (rDecVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to add
	\return result of addition */
const Decimal Decimal::operator += (const Int32 rIntVal)
{
	return (operator += (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	rDecVal Decimal to subtract
	\return subtraction result */
const Decimal Decimal::operator -= (const Decimal& rDecVal)
{
	return (operator= (operator- (rDecVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to subtract
	\return result of subtraction */
const Decimal Decimal::operator -= (const Int32 rIntVal)
{
	return (operator -= (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param rDecVal Decimal to multiply
	\return multiplication result */
const Decimal Decimal::operator *= (const Decimal& rDecVal)
{
	return (operator= (operator* (rDecVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to multiply
	\return result of multiplication */
const Decimal Decimal::operator *= (const Int32 rIntVal)
{
	return (operator *= (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	rDecVal Decimal to divide
	\return division result */
const Decimal Decimal::operator /= (const Decimal& rDecVal)
{
	return (operator= (operator/ (rDecVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rIntVal int to divide
	\return result of division */
const Decimal Decimal::operator /= (const Int32 rIntVal)
{
	return (operator /= (Decimal(rIntVal)));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	rDecVal Decimal to assign
	\return assigned Decimal */
Decimal& Decimal::operator = (const Decimal& rDecVal)
{
	memcpy(m_Dec, rDecVal.m_Dec, sizeof(decimal));

    return *this;
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	rDecVal Decimal to compare 
	\retval true if rDecVal is equal to the Decimal */
bool Decimal::operator == (const Decimal& rDecVal) const
{
	Int32 ret = deccmp(const_cast<decimal *>(m_Dec  ),
                       const_cast<decimal *>(rDecVal.m_Dec));

	if (DECUNKNOWN == ret)
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator==", "one value is NULL", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}

	return (ret == 0);
}

//----------------------------------------------------------------------------
/*! \throw	NullValueException 
	\param	rDecVal Decimal to compare 
	\retval true if rDecVal isn't equal to the Decimal
	\return otherwise returns false */
bool Decimal::operator != (const Decimal& rDecVal) const
{
	return !(operator == (rDecVal));
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	rDecVal Decimal to compare 
	\retval true if rDecVal is smaller than the Decimal
	\return otherwise returns false */
bool Decimal::operator < (const Decimal& rDecVal) const
{
	Int32 ret = deccmp(const_cast<decimal *>(m_Dec  ),
                       const_cast<decimal *>(rDecVal.m_Dec));

	if (DECUNKNOWN == ret)
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator<", "one value is NULL", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}

	return (ret == -1);
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	rDecVal Decimal to compare
	\retval true if rDecVal is smaller than or equal to the Decimal
	\return otherwise returns false */
bool Decimal::operator <= (const Decimal& rDecVal) const
{
	Int32 ret = deccmp(const_cast<decimal *>(m_Dec  ),
                       const_cast<decimal *>(rDecVal.m_Dec));
	if (DECUNKNOWN == ret)
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operatoroperator<=", "one value is NULL", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}

	return (ret != 1);
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	rDecVal Decimal to compare 
	\retval true if rDecVal is greater than the Decimal
	\return otherwise returns false */
bool Decimal::operator > (const Decimal& rDecVal) const
{
	Int32 ret = deccmp(const_cast<decimal *>(m_Dec  ),
                       const_cast<decimal *>(rDecVal.m_Dec));

	if (DECUNKNOWN == ret)
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator>", "one value is NULL", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}
	return (ret == 1);
}

//----------------------------------------------------------------------------
/*! \throw  NullValueException 
	\param	rDecVal Decimal to compare 
	\retval true if rDecVal is greater than or equal to the Decimal
	\return otherwise returns false */
bool Decimal::operator >= (const Decimal& rDecVal) const
{
	Int32 ret = deccmp(const_cast<decimal *>(m_Dec  ),
                       const_cast<decimal *>(rDecVal.m_Dec));
	if (DECUNKNOWN == ret)
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.operator>=", "one value is NULL", __FILE__, __LINE__);
		throw NullValueException(sInfo);
	}

	return (ret != -1);
}

//----------------------------------------------------------------------------
/*! \throw  OverflowException 
	\return the Value of the Decimal as a Float64 */
Float64 Decimal::toFloat64() const
{
	Float64 dbl;

	if (0 != dectodbl(const_cast<decimal *>(m_Dec), &dbl))
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.toFloat64", "conversion failed", __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	return dbl;
}

//----------------------------------------------------------------------------
/*! \throw  OverflowException 
	\return the Value of the Decimal as an Int16 */
Int16 Decimal::toInt16( )	const
{
	Int32 intValue;
	Int32 ret;

	if ( 0 != (ret = dectoint(const_cast<decimal *>(m_Dec), &intValue)) )
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.toInt16", 
			             (INF_OVERFLOW == ret) ? "magnitude of decimal type number is greater than 32.767"
						                       : "conversion failed", 
						 __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	return static_cast<Int16>(intValue);	// dectoint() range: Int16 ! 
}

//----------------------------------------------------------------------------
/*! \throw  OverflowException 
	\return the Value of the Decimal as a Long32 */
Long32 Decimal::toLong32( ) const
{
	int4 longValue;
	Int32  ret;

	if (0 != (ret = dectolong(const_cast<decimal *>(m_Dec), &longValue)) )
	{
		ExceptInfo sInfo("basar.cmnutil.Decimal.toLong32", 
			             (INF_OVERFLOW == ret) ? "magnitude of decimal type number is greater than 2.147.483642"
						                       : "conversion failed", 
						 __FILE__, __LINE__);
		throw OverflowException(sInfo);
	}

	return Long32(longValue);
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return the Value of the Decimal as a Int32 */
Int32 Decimal::toInt32( )		const
{
	return static_cast<Int32>(toLong32());
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	right indicates the number of decimal places to the right of the decimal point
	\return value of decimal as an non localized string */
const VarString Decimal::toNonLocString( const Int16 right /* = s_ALL_DECIMAL_PLACES */ ) const
{
	// maximum length of string to convert decimal to/from
	static const Int16 BUFLEN = 256;

	char  help[BUFLEN] = { 0 };
		
	if ( dectoasc(const_cast<decimal *>(m_Dec),
		          help, BUFLEN - 1, 
		          right) )
		return "";

	help[byleng(help, BUFLEN - 1)] = '\0';

	return help;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	right indicates the number of decimal places to the right of the decimal point
	\return value of Decimal as an localized string (decimal point/comma) */
const I18nString Decimal::toString( const Int16 right /* = s_ALL_DECIMAL_PLACES */ ) const
{
	I18nString decString = toNonLocString(right);
	decString.exchange(Locale::getDecSep(Locale::C), Locale::getDecSep());

    return decString;
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	right indicates the number of decimal places to the right of the decimal point
	\return the Value of the Decimal as an I18nString with thousands separator */
const I18nString  Decimal::toSepString( const Int16 right /* = s_ALL_DECIMAL_PLACES */)	const
{
	I18nString decString = toString(right);
	bool negValue = decString.size() > 0 && decString[0] == '-';

	VarString  thousands = Locale::getDec1000thSep();
	VarString  decsep    = Locale::getDecSep      ();

    //handle right == 0 -> no decimal separator
    size_t pos_decSep = decString.find(decsep);

    if (std::string::npos == pos_decSep)
    {
        pos_decSep = decString.size();
    }

	for (Int32 pos = static_cast<Int32>(pos_decSep) - 3;
		 pos > (negValue ? 1 :0); 
		 pos -= 3)
	{
		decString.insert( pos, thousands );
	}

	return decString;
}

//----------------------------------------------------------------------------
/*! \throw  OverflowException, UnderflowException, NonNumericException, InvalidExponentException
	\param	rDecStr non localized (decimal point) string that is converted to Decimal */
void Decimal::fromNonLocString(const VarString & rDecStr)
{
	
	int ret = deccvasc( const_cast < char * >(rDecStr.c_str ()), 
		               static_cast< Int32  >(rDecStr.length()), 
					   m_Dec          ); 
	
	switch ( ret )
	{
	case INF_OVERFLOW:
		{
			VarString msg;
			msg.format("string <%.*s> overflowed Decimal structure", rDecStr.length(), rDecStr.c_str());
			ExceptInfo sInfo("basar.cmnutil.Decimal.fromNonLocString", msg, __FILE__, __LINE__);
			throw OverflowException(sInfo);
		}

	case INF_UNDERFLOW:
		{
			VarString msg;
			msg.format("string <%.*s> underflowed Decimal structure", rDecStr.length(), rDecStr.c_str());
			ExceptInfo sInfo("basar.cmnutil.Decimal.fromNonLocString", msg, __FILE__, __LINE__);
			throw UnderflowException(sInfo);
		}

	case NONNUMERIC:
		{
			VarString msg;
			msg.format("string <%.*s> has non-numeric characters", rDecStr.length(), rDecStr.c_str());
			ExceptInfo sInfo("basar.cmnutil.Decimal.fromNonLocString", msg, __FILE__, __LINE__);
			throw NonNumericException(sInfo);
		}

	case INVALIDEXPONENT:
		{
			VarString msg;
			msg.format("string <%.*s> has has bad exponent", rDecStr.length(), rDecStr.c_str());
			ExceptInfo sInfo("basar.cmnutil.Decimal.fromNonLocString", msg, __FILE__, __LINE__);
			throw InvalidExponentException(sInfo); 
		}

	default:
		break;
	}
	
	if (isNull())
	{
        if( 0 != deccvdbl( 0.0, m_Dec) )
	    {
		    m_Dec->dec_exp   =  0;
		    m_Dec->dec_pos   = -1;
		    m_Dec->dec_ndgts =  0;
		    memset(m_Dec->dec_dgts, 0, DECSIZE);
    		
		    ExceptInfo sInfo("basar.cmnutil.Decimal.Decimal()", "conversion from 0.0 to Decimal failed", __FILE__, __LINE__);
		    throw NullValueException(sInfo);
	    }	    
	}
	
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param	rDecStr localized (decimal point/comma) string that is converted to Decimal*/
void Decimal::fromString(const I18nString& rDecStr)
{
	VarString strDec = rDecStr;

	strDec.remove(Locale::getDec1000thSep());

	// exchange to technical decimal point format
	strDec.exchange(Locale::getDecSep(), Locale::getDecSep(Locale::C));
	fromNonLocString(strDec);
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	pos the position where the decimal is truncated */
void Decimal::trunc(const Int16 pos)
{
	dectrunc(m_Dec, pos);
}

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param	pos the position where the decimal is rounded */
void Decimal::round(const Int16 pos)
{
	decround(m_Dec, pos);
}


//----------------------------------------------------------------------------
/*	\throw  no-throw 
	\return number of digits */
Int32 Decimal:: getNumberOfDigits() const
{
	Int32 retval  = (m_Dec->dec_ndgts * 2);
	/*if (retval == 0)
	{
		retval = 1;
	}
	else*/ if (retval > 0)
	{
		bool m_wholeNumber = false;
	    bool m_zeroPointNumber = false;
		if (m_Dec->dec_exp >= m_Dec->dec_ndgts)
		{
			m_wholeNumber = true;
			retval = (m_Dec->dec_exp * 2);
		}
		if (1 > m_Dec->dec_exp && retval != 0)
		{
			m_zeroPointNumber = true;
			// the dec_exp is negative at numbers like 0,0001
			retval += -(m_Dec->dec_exp * 2);
		}
		// changes if the last/first value is 0 ?
		if (retval > 0)
		{
			int i1 = 0;
			int i2 =0;
			// check on last 0 only when there 
			// are internal decimal places
			if (!m_wholeNumber)
			{
				i1 = m_Dec->dec_dgts[m_Dec->dec_ndgts-1];
				i2 = i1/10;
				if (i2*10 == i1)
				{
					--retval;
				}
			}
			if (retval > 0 && !m_zeroPointNumber)
			{
				// check on first 0 character:
				i1 = m_Dec->dec_dgts[0];
				i2 = i1/10;
				if (0 == i2)
				{
					--retval;
				}
			}
			if (m_zeroPointNumber)
			{
				++retval;
			}
		}
	}
	return retval;
}
//----------------------------------------------------------------------------
/*	\throw no-throw 
	\return number of decimal places */
Int32  Decimal::getNumberOfDecimalPlaces() const
{
	Int32 retval = 0;
	if (0 == m_Dec->dec_ndgts)
	{
		retval = 0;
	}
	else  
	{
		retval = ((m_Dec->dec_ndgts - m_Dec->dec_exp) * 2);
		// changes if the last value is 0 ?
		if (retval > 0)
		{
			int i1 = m_Dec->dec_dgts[m_Dec->dec_ndgts-1];
			int i2 = i1/10;
			if (i2*10 == i1)
			{
				--retval;
			}
		}
		else
		{
			retval = 0;
		}
	}
	return retval;
}

const decimal * Decimal::getDec() const
{
    return m_Dec;
}

bool Decimal::isNull() const
{
    return (-1 == m_Dec->dec_pos);
}

//---------------------------------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief standard ostream operator
	\param stream - outstream
	\param d - decimal to output
	\return reference to std::ostream
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& stream, const Decimal& d)
{
	return (stream << d.toString());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief operator+ for int + Decimal
	\param op1 - int first summand
	\param op2 - Decimal second summand
	\return Decimal representing result of addition
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal operator+( basar::Int32 op1, const Decimal& op2 )
{
    Decimal d(op1);
    return d.operator+=(op2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief operator* for int * Decimal
	\param op1 - int multiplicand
	\param op2 - Decimal multiplier
	\return Decimal representing result of multiplication
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal operator*( basar::Int32 op1, const Decimal& op2 )
{
    Decimal d(op1);
    return d.operator*=(op2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief operator+ for Float64 + Decimal
	\param op1 - Float64 first summand
	\param op2 - Decimal second summand
	\return Decimal representing result of addition
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal operator+( Float64 op1, const Decimal& op2 )
{
    Decimal d(op1);
    return d.operator+=(op2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief operator* for Float64 * Decimal
	\param op1 - Float64 multiplicand
	\param op2 - Decimal multiplier
	\return Decimal representing result of multiplication
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal operator*( Float64 op1, const Decimal& op2 )
{
    Decimal d(op1);
    return d.operator*=(op2);
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

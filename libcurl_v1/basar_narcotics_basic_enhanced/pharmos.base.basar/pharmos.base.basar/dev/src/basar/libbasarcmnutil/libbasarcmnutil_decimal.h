//----------------------------------------------------------------------------
/*! \file
 *  \brief  decimal class definitions (informix)
 *  \author Thomas Hörath
 *  \date   21.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_DECIMAL_H
#define GUARD_LIBBASARCMNUTIL_DECIMAL_H

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

struct decimal;   //!< forward declaration of informix decimal structure for pointer, defined in informix CSDK's decimal.h

//----------------------------------------------------------------------------
#include "libbasarcmnutil_exceptions.h"


#include <iosfwd>

//----------------------------------------------------------------------------
namespace basar   {
namespace cmnutil {


//----------------------------------------------------------------------------
/*! \brief exception class for decimal overflow
	This is a final class. */
class OverflowException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API OverflowException		(const ExceptInfo& sInfo);
private:
			OverflowException		();							//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception class for decimal underflow

	This is a final class. */
class UnderflowException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API UnderflowException		(const ExceptInfo& sInfo);
private:
			UnderflowException		();							//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception class for decimal division by zero

	This is a final class. */
class DivisionByZeroException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API DivisionByZeroException	(const ExceptInfo& sInfo);
private:
			DivisionByZeroException		();						//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception class for decimal not numeric

	This is a final class. */
class NonNumericException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API NonNumericException		(const ExceptInfo& sInfo);
private:
			NonNumericException		();							//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception class for decimal invalid exponent

	This is a final class. */
class InvalidExponentException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API InvalidExponentException(const ExceptInfo& sInfo);
private:
			InvalidExponentException		();					//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief exception class for decimal null value

	This is a final class. */
class NullValueException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API NullValueException(const ExceptInfo& sInfo);
private:
			NullValueException		();					//!< forbidden
};

//----------------------------------------------------------------------------
//! \brief dump Decimal variable's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasarcmnutil.dll} basar::cmnutil::dumpDecimal( OBJECT_ADDRESS )
//! \endcode
LIBBASARCMNUTIL_API void	dumpDecimal	(size_t addr	//!< object address
										);

//----------------------------------------------------------------------------
/*! \brief class for Decimal near the informix decimal

	This is a final class. */
class Decimal
{
public:

	/*! \brief Standard-Constructor of the Decimal-class; set to 0.0 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API						Decimal		();						
	/*! \brief overwritten Constructor of the Decimal-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API						Decimal		(const Decimal& rDecVal); 
	/*! \brief overwritten Constructor of the Decimal-class 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API explicit			Decimal		(const Int16 value);
	/*! \brief overwritten Constructor of the Decimal-class 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API explicit			Decimal		(const Int32 value);

	/*! \brief overwritten Constructor of the Decimal-class 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API explicit			Decimal		(const Float32 value);
	/*! \brief overwritten Constructor of the Decimal-class 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API explicit			Decimal		(const Float64 value);
	/*! \brief overwritten Constructor of the Decimal-class 
		\n     throw NullValueException */
	LIBBASARCMNUTIL_API explicit			Decimal		(const I18nString & rDecStr);

	/*! \brief overwritten Constructor of the Decimal-class 
		\n     no-throw */
    LIBBASARCMNUTIL_API	explicit			Decimal		(const decimal * pDec);

	/*! \brief Standard-Destructor of the Decimal-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API					   ~Decimal		();

	/*! \brief operator for adding two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator +	(const Decimal & rDecVal) const;
	/*! \brief operator for adding int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator +	(const Int32     rIntVal) const;

	/*! \brief operator for subtracting two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator -	(const Decimal & rDecVal) const;
	/*! \brief operator for subtracting int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator -	(const Int32     rIntVal) const;

	/*! \brief operator for multiplying two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator *	(const Decimal & rDecVal) const;
	/*! \brief operator for multiplying with int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator *	(const Int32     rIntVal) const;

	/*! \brief operator for dividing by Decimal
		\n     throw UnderflowException, OverflowException, DivisionByZeroException */
	LIBBASARCMNUTIL_API const Decimal		operator /	(const Decimal & rDecVal) const;
	/*! \brief operator for dividing by int		\n  no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator /	(const Int32     rIntVal) const;


	/*! \brief operator for adding two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator +=	(const Decimal& rDecVal);
	/*! \brief operator for adding int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator +=	(const Int32     rIntVal);

	/*! \brief operator for subtracting two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator -=	(const Decimal& rDecVal);
	/*! \brief operator for subtracting int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator -=	(const Int32     rIntVal);
	
	/*! \brief operator for multiplying two Decimals
		\n     throw UnderflowException, OverflowException */
	LIBBASARCMNUTIL_API const Decimal		operator *=	(const Decimal& rDecVal);
	/*! \brief operator for multiplying with int	\n no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator *=	(const Int32     rIntVal);
	
	/*! \brief operator for dividing two Decimals
		\n     throw UnderflowException, OverflowException, DivisionByZeroException */
	LIBBASARCMNUTIL_API const Decimal		operator /=	(const Decimal& rDecVal);
	/*! \brief operator for dividing by int		\n  no-throw */
	LIBBASARCMNUTIL_API const Decimal		operator /=	(const Int32     rIntVal);


	/*! \brief operator for assigning a Decimal
		\n     no-throw */
	LIBBASARCMNUTIL_API Decimal&			operator =	(const Decimal& rDecVal);

	/*! \brief operator for comparing the equality of two Decimals
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool    operator ==	(const Decimal& rDecVal) const;
	/*! \brief operator for comparing  the difference of two Decimals 
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool	operator !=	(const Decimal& rDecVal) const;
	/*! \brief check if 1st Decimal is smaller than 2nd
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool	operator <	(const Decimal& rDecVal) const;
	/*! \brief check if 1st Decimal is smaller than or equal to 2nd
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool	operator <=	(const Decimal& rDecVal) const;
	/*! \brief check if 1st Decimal is greater than 2nd
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool	operator >	(const Decimal& rDecVal) const;
	/*! \brief check if 1st Decimal is greater than or equal to 2nd
		\n     NullValueException */
	LIBBASARCMNUTIL_API bool	operator >=	(const Decimal& rDecVal) const;

	/*! \brief as many decimal places are shown as the decimal obj contains	*/
	LIBBASARCMNUTIL_API static Int16	    getAllDecimalPlaces         ();

    //! \brief get size for informix custom datetype description	\n no-throw
    LIBBASARCMNUTIL_API static int          getInfxDescSize             ();
	
	/*! \brief convert a decimal to non localized string	\n no-throw */
	LIBBASARCMNUTIL_API const VarString		toNonLocString	( const Int16 right = getAllDecimalPlaces() ) const;
	/*! \brief convert a decimal to localized string		\n no-throw */
	LIBBASARCMNUTIL_API const I18nString	toString		( const Int16 right = getAllDecimalPlaces() ) const;
	/*! \brief convert a decimal to string with thousands separator		\n no-throw */
	LIBBASARCMNUTIL_API const I18nString	toSepString		( const Int16 right = getAllDecimalPlaces() ) const;
	/*! \brief convert a decimal to Float64			\n OverflowException */
	LIBBASARCMNUTIL_API Float64		        toFloat64		() const;
	/*! \brief convert a decimal to Int16			\n OverflowException */
	LIBBASARCMNUTIL_API Int16			    toInt16			() const;
    /*! \brief convert a decimal to Int32			\n no-throw */
	LIBBASARCMNUTIL_API Int32		        toInt32			() const;
	/*! \brief convert a decimal to Long32			\n throw OverflowException */
	LIBBASARCMNUTIL_API Long32		        toLong32		() const;

	/*! \brief cut a decimal at the position pos	\n no-throw */
	LIBBASARCMNUTIL_API void				trunc			(const Int16 pos);
	/*! \brief round a decimal at the position pos	\n no-throw */
	LIBBASARCMNUTIL_API void				round			(const Int16 pos);

	/*! \brief convert from non localized string to decimal
		\n     throw OverflowException, UnderflowException, NonNumericException, InvalidExponentException */
	LIBBASARCMNUTIL_API void				fromNonLocString	(const VarString & rDecStr);
	/*! \brief convert from localized string to a decimal	\n     no-throw */
	LIBBASARCMNUTIL_API void				fromString			(const I18nString & rDecStr);

   /*! \brief returns the number of decimal places	
	   \n     "29.02" returns "2"
       \n     no-throw */
	LIBBASARCMNUTIL_API Int32               getNumberOfDecimalPlaces	() const;

	 /*! \brief returns the number of digits 
		 \n     "29.02" returns "4"
         \n no-throw */
	LIBBASARCMNUTIL_API Int32               getNumberOfDigits			() const;

    //! \brief get pointer to decimal buffer	\n no-throw
    LIBBASARCMNUTIL_API const decimal *     getDec                      () const;

private:

    bool isNull() const;
    
    
	//! enum representing errorcode of informix-decimal-functions
	enum ErrorEnum 
	{
		INF_OVERFLOW	= -1200,	//!< decimal overflow
		INF_UNDERFLOW	= -1201,	//!< decimal underflow
		DIVBYZERO		= -1202,	//!< dividend = 0
		NONNUMERIC		= -1213,	//!< expected string is not numeric
		INVALIDEXPONENT	= -1216		//!< exponent is invalid
	};

	const bool m_ownsDec;       //!< bool indication if memory behind m_Dec was allocated by decimal instance, thus has to be deleted upon desctruction

	decimal *		m_Dec;		//!< decimal storage member (borrowed from Informix CSDK)

};

//----------------------------------------------------------------------------

//no friend necessary - operator uses public function
LIBBASARCMNUTIL_API std::ostream& operator<< (std::ostream& stream, const Decimal& d);

LIBBASARCMNUTIL_API const Decimal operator+( Int32 op1, const Decimal& op2 );

LIBBASARCMNUTIL_API const Decimal operator*( Int32 op1, const Decimal& op2 );

LIBBASARCMNUTIL_API const Decimal operator+( Float64 op1, const Decimal& op2 );

LIBBASARCMNUTIL_API const Decimal operator*( Float64 op1, const Decimal& op2 );
//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar


//----------------------------------------------------------------------------
#endif	// GUARD

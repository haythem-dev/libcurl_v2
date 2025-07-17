#ifndef GUARD_LIBBASARCMNUTIL_DATE_H
#define GUARD_LIBBASARCMNUTIL_DATE_H

//----------------------------------------------------------------------------
/*! \file
 *  \brief  definition of date class (extraced from datetime.h)
 *  \author Marco Köppendörfer
 *  \date   03.05.2013
 */
//----------------------------------------------------------------------------
#include "libbasarcmnutil_datetypes_common.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_datetimeexception.h"

//------------------------------------------------------------------------------------------------------//
// dll import and export macros
//------------------------------------------------------------------------------------------------------//
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

//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {
//----------------------------------------------------------------------------
//! \brief dump Date variable's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasarcmnutil.dll} basar::cmnutil::dumpDate( OBJECT_ADDRESS )
//! \endcode
LIBBASARCMNUTIL_API void	dumpDate	(size_t addr	//!< object address
										);

//------------------------------------------------------------------------------------------------------//
//! \brief Date: range from 0001-01-01 to 9999-12-31
//!	\n final class
//! \n throws DateTimeException in case of failure
//------------------------------------------------------------------------------------------------------//
class Date
{
public:

	//--------------------------------------------------------------------------------------------------//
	// enum section
	//--------------------------------------------------------------------------------------------------//

	//! predefined null values
	enum NullValueEnum
	{
		NULL_DATE =  0			//!< invalid (= not set) date
	};

	//! day of week 
	enum WeekDayEnum
	{
		SUNDAY     =  0,		//!< Sunday
		MONDAY         ,		//!< Monday
		TUESDAY        ,		//!< Tuesday
		WEDNESDAY      ,		//!< Wednesday
		THURSDAY       ,		//!< Thursday
		FRIDAY         ,		//!< Friday
		SATURDAY				//!< Saturday
	};

	//--------------------------------------------------------------------------------------------------//
	// static section
	//--------------------------------------------------------------------------------------------------//

	//! \brief is given year a leap year?	\n no-throw															
	LIBBASARCMNUTIL_API static bool	isLeapYear		( const Int32 year	//!< year to be checked
													);

	//! \brief calculating days of given month									\n throw DateTimeException
	LIBBASARCMNUTIL_API static Int32	getDaysOfMonth	(const Int32 year,	//!< year to be calculated  [0, 9999]
														 const Int32 month	//!< month to be calculated [1,   12]
														);
	//! \brief calculating date of first Spring moon; used to calculate Easter	\n no-throw
	LIBBASARCMNUTIL_API static Date	getFstSprMoon	(const Int32 year	//!< year to be calculated  [0, 9999]
													);
	//! \brief calculating date of easter sunday; need for holidays calculation	\n no-throw
	LIBBASARCMNUTIL_API	static Date	getEasterSunday	(const Int32 year	//!< year to be calculated, allowed range [1970, 2038]
													);
	//! \brief get current localized system date	\n no-throw
	LIBBASARCMNUTIL_API	static Date	getCurrent		();
    
    //! \brief get size for informix custom datetype description	\n no-throw
    LIBBASARCMNUTIL_API	static int	getInfxDescSize		();

	//--------------------------------------------------------------------------------------------------//
	// c'tor / d'tor
	//--------------------------------------------------------------------------------------------------//

	//! \brief construct current timestamp						\n no-throw
	LIBBASARCMNUTIL_API	Date			();
	//! \brief construct with integer formatted date			\n no-throw */
	LIBBASARCMNUTIL_API	Date			( const Int32 date		//!< integer formatted date (YYYYMMDD)
										);
	//! \brief construct with date components					\n no-throw */
	LIBBASARCMNUTIL_API	Date			( const Int32 year,		//!< year	[0, 9999]
										  const Int32 month,	//!< month  [1,   12]
										  const Int32 day		//!< day	[1,   31]
	                                    );

    //! \brief construct from informix date buffer, note no copying takes place		\n no-throw */
    LIBBASARCMNUTIL_API explicit Date  (const Int32  * infxDate  //!< pointer to informix date buffer
                                       );

    //! \brief copy construct from other Date					\n no-throw */
    LIBBASARCMNUTIL_API Date           (const Date& rDate);

	//! \brief d'tor											\n no-throw
	LIBBASARCMNUTIL_API ~Date			();


	//--------------------------------------------------------------------------------------------------//
	// object methods
	//--------------------------------------------------------------------------------------------------//

	//! \brief get integer date (YYYYMMDD)	\n no-throw
	LIBBASARCMNUTIL_API	Int32			getDate			() const;

	//! \brief get year component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getYear			() const;
	//! \brief get month component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getMonth		() const;
	//! \brief get day component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getDay			() const;

	//! \brief get day of week				\n no-throw
	LIBBASARCMNUTIL_API	WeekDayEnum	getDayOfWeek	() const;
	//! \brief get day of year				\n no-throw
	LIBBASARCMNUTIL_API	Int16			getDayOfYear	() const;
	//! \brief get week of year				\n no-throw 
	LIBBASARCMNUTIL_API	Int16			getWeekOfYear	() const;

    //! \brief get pointer to date buffer				\n no-throw 
    LIBBASARCMNUTIL_API const Int32 *   getDateBuffer   () const;

	//--------------------------------------------------------------------------------------------------//

	//! \brief set integer date					\n no-throw
	LIBBASARCMNUTIL_API	void			setDate			(const Int32 date		//!< integer formatted date (YYYYMMDD)
														);
	//! \brief set integer date					\n no-throw
	LIBBASARCMNUTIL_API	void			setDate			(const Int32 year,		//!< year		   0 - 9999
														 const Int32 month,		//!< month         1 -   12 -> month of year
														 const Int32 day		//!< day           1 -   31 -> day of month
														);

    LIBBASARCMNUTIL_API void            setDate         (const Date& date
                                                        );

	//! \brief set datetime member to invalid state	\n no-throw
	LIBBASARCMNUTIL_API void			setInvalid		();

	//--------------------------------------------------------------------------------------------------//

	//! \brief is object's datetime correctly set?	\n no-throw
	LIBBASARCMNUTIL_API bool            isValid			() const;

	//--------------------------------------------------------------------------------------------------//

    //! \brief adding days							\n no-throw
    LIBBASARCMNUTIL_API void            addDays         (const Int32 days       //!< days to add, can be negative
                                                        );

    //! \brief adding months							\n no-throw
    LIBBASARCMNUTIL_API void            addMonths       (const Int32 months     //!< months to add, can be negative
                                                        );

    //! \brief adding years							\n no-throw
    LIBBASARCMNUTIL_API void            addYears        (const Int32 years      //!< years to add, can be negative
                                                        );

	//--------------------------------------------------------------------------------------------------//

	//! \brief converting to string using format-control string							\n throw DateTimeException
	LIBBASARCMNUTIL_API	const I18nString	toString		(ConstString   format		//!< format control string, see Informix ESQL/C manual
															) const;
	//! \brief converting to date representation for current locale; \n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrDate		() const;

	//--------------------------------------------------------------------------------------------------//

	//! \brief converting from string using format-control string						\n throw DateTimeException
	LIBBASARCMNUTIL_API	void				fromString 		(ConstString        format,	//!< format control string, see Informix ESQL/C manual
															 const I18nString & rDate	//!< date string in representation same as from toStrDate(); depends on locale
															);
	//! \brief converting from date representation for current locale (same as return value from toStrDate())		\n no-throw
	LIBBASARCMNUTIL_API	void				fromStrDate		(const I18nString & rDate	//!< date string in representation same as from toStrDate(); depends on locale
															);


	//--------------------------------------------------------------------------------------------------//
	//	operators
	//--------------------------------------------------------------------------------------------------//

	//! \brief	checking equality	\n no-throw
	LIBBASARCMNUTIL_API bool    operator ==		(const Date      & rDate	//!< passed object to compare with context object
												) const;
    //! \brief checking less than \n no-throw
    LIBBASARCMNUTIL_API bool    operator < ( const Date & rDate //!< passed object to compare with context object
                                           ) const;

    //! \brief checking less than or equal \n no-throw
    LIBBASARCMNUTIL_API bool    operator <= ( const Date & rDate //!< passed object to compare with context object
                                           ) const;

    //! \brief checking greater than \n no-throw
    LIBBASARCMNUTIL_API bool    operator > (const Date& rDate //!< passed object to compare with context object
                                           ) const;

    //! \brief checking greater than or equal \n no-throw
    LIBBASARCMNUTIL_API bool    operator >= (const Date& rDate //!< passed object to compare with context object
                                           ) const;

    //! \brief	assignment opertator	\n no-throw
   	LIBBASARCMNUTIL_API Date&	operator =		(const Date& rDate	//!< passed object to assing to context object
														);

    //! \brief pre-increment to next day    \n no-throw
    LIBBASARCMNUTIL_API Date&   operator++();

    //! \brief pre-decrement to day before    \n no-throw
    LIBBASARCMNUTIL_API Date&   operator--();

    //! \brief post-increment to next day    \n no-throw
    LIBBASARCMNUTIL_API Date    operator++(int);

    //! \brief post-decrement to day before    \n no-throw
    LIBBASARCMNUTIL_API Date    operator--(int);

private:

	//! enum representing errorcode of Informix date functions
	enum ErrorEnum 
	{
		DTERROR_SUCCESS   =     0,	//!< success

		DTERROR_YEAR      = -1204,	//!< input parameter specifies invalid year
		DTERROR_MONTH     = -1205,	//!< input parameter specifies invalid month
		DTERROR_DAY       = -1206,	//!< input parameter specifies invalid day
		DTERROR_DELIMITER = -1209,	//!< because input does not contain delimiters between year, month, and day,
									//!< \n length of input must be exactly six or eight bytes
		DTERROR_CONV      = -1210,	//!< internal date cannot be converted to month-day-year format
		DTERROR_MEMORY    = -1211,	//!< memory-allocation error
		DTERROR_FORMAT    = -1212	//!< format string is NULL, invalid	or does not specify year, month and day
	};

	//--------------------------------------------------------------------------------------------------//
	// methods
	//--------------------------------------------------------------------------------------------------//

	//! \brief is date NULL (9999-12-31)?	\n no-throw
	bool	isNull				() const;

	//! set current localized system timestamp to member		\n no-throw
	void	setCurrent			();

	//--------------------------------------------------------------------------------------------------//
	// member section
	//--------------------------------------------------------------------------------------------------//

	const bool		m_ownsDate;	//!< bool indication if memory behind m_Date was allocated by Date instance and thus has to be deleted upon destruction

	Int32*			m_Date;		//!< holds date in range from 0001-01-01 to 9999-12-31
};

//----------------------------------------------------------------------------

//no friend necessary - operator uses public function
LIBBASARCMNUTIL_API std::ostream& operator<< (std::ostream& stream, const Date& date);

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

#endif //GUARD_LIBBASARCMNUTIL_DATE_H

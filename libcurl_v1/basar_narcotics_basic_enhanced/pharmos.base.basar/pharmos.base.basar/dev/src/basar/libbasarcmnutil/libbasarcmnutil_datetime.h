#ifndef GUARD_LIBBASARCMNUTIL_DATETIME_H
#define GUARD_LIBBASARCMNUTIL_DATETIME_H

//------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  datetime class definitions
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_datetypes_common.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_datetimeexception.h"

#include <iosfwd>

//forward declaration of informix datetime structure for pointer, defined in informix CSDK's datetime.h
struct dtime;

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

// forward declaration
class Timestamp;

//----------------------------------------------------------------------------
//! \brief dump DateTime variable's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasarcmnutil.dll} basar::cmnutil::dumpDateTime( OBJECT_ADDRESS )
//! \endcode
LIBBASARCMNUTIL_API void	dumpDateTime(size_t addr	//!< object address
										);

//------------------------------------------------------------------------------------------------------//
//! \brief Date / Time: range from 0001-01-01 00:00:00.000 to 9999-12-31 23:59:59.999
//!	\n final class
//! \n throws DateTimeException in case of failure
//------------------------------------------------------------------------------------------------------//
class DateTime
{
public:

	//--------------------------------------------------------------------------------------------------//
	// enum section
	//--------------------------------------------------------------------------------------------------//

	//! predefined null values
	enum NullValueEnum
	{
		NULL_DATE =  0,			//!< invalid (= not set) date
		NULL_TIME = -1			//!< invalid (= not set) time
	};

	//! time wrap caused by time calculations
	enum TimeCalcEnum
	{
		DAY_BEFORE = -1,	//!< time wrapped to day before
		DAY_SAME   =  0,	//!< no time wrap
		DAY_AFTER  =  1		//!< time wrapped to next day
	};

	//! datetime units for specifying value range
	enum DateTimeUnitEnum
	{
		DTUNIT_YEAR = 0,	//!< year
		DTUNIT_MONTH,		//!< month
		DTUNIT_DAY,			//!< day
		DTUNIT_HOUR,		//!< hour
		DTUNIT_MIN,			//!< minute
		DTUNIT_SEC,			//!< second
		DTUNIT_DSEC,		//!< 10th second (deci)
		DTUNIT_CSEC,		//!< 100th second (centi)
		DTUNIT_MSEC,		//!< 1000th second (milli)
		DTUNIT_100uSEC,		//!< 10000th second (100 µ)
		DTUNIT_10uSEC		//!< 100000th second (10 µ)
	};

	//--------------------------------------------------------------------------------------------------//
	// static section
	//--------------------------------------------------------------------------------------------------//

	//! \brief get current localized system date	\n no-throw
	LIBBASARCMNUTIL_API	static const DateTime	getCurrent		();

    //! \brief get size for informix custom datetime description	\n no-throw
    LIBBASARCMNUTIL_API	static int	    getInfxDescSize	();


	//--------------------------------------------------------------------------------------------------//
	// c'tor / d'tor
	//--------------------------------------------------------------------------------------------------//

	//! \brief construct current timestamp						\n no-throw
	LIBBASARCMNUTIL_API						DateTime		();
	//! \brief construct with integer formatted date and time	\n no-throw */
	LIBBASARCMNUTIL_API						DateTime		( const Int32 date,				//!< integer formatted date (YYYYMMDD)
															  const Int32 time = NULL_TIME	//!< integer formatted time (hhmmssttt)
															);
	//! \brief construct with date components					\n no-throw */
	LIBBASARCMNUTIL_API						DateTime		( const Int32 year,				//!< year	[0, 9999]
															  const Int32 month,			//!< month  [1,   12]
															  const Int32 day				//!< day	[1,   31]
															);
	//! \brief construct with time components					\n no-throw */
	LIBBASARCMNUTIL_API						DateTime		( const Int32 hour,		//!< hour          0 -  23 -> hour since midnight
															  const Int32 min,		//!< minutes       0 -  59 -> minutes after hour
															  const Int32 sec,		//!< seconds       0 -  59 -> seconds after minute
															  const Int32 msec		//!< milli seconds 0 - 999 -> milli seconds after second
															);
    //! \brief construct from informix datetime buffer, note: no copying done here	\n no-throw */
   	LIBBASARCMNUTIL_API explicit			DateTime		( const dtime * pTime //!< pointer to informix datetime buffer
															);

    //! \brief construct from other datetime object 			\n no-throw */
    LIBBASARCMNUTIL_API						DateTime		( const DateTime& rDt //!< datetime object to be constructed from
															);
	//! \brief conversion contructor, construct from date object 			\n no-throw */
    LIBBASARCMNUTIL_API	explicit			DateTime		( const Date& rDate //!< date object to be constructed from
															);

	//! \brief d'tor											\n no-throw
	LIBBASARCMNUTIL_API					   ~DateTime		();


	//--------------------------------------------------------------------------------------------------//
	// object methods
	//--------------------------------------------------------------------------------------------------//

	//! \brief get integer date (YYYYMMDD)	\n no-throw
	LIBBASARCMNUTIL_API	Int32			getDate			() const;
	//! \brief get integer time (hhmmssttt)	\n no-throw
	LIBBASARCMNUTIL_API	Int32			getTime			() const;

	//! \brief get year component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getYear			() const;
	//! \brief get month component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getMonth		() const;
	//! \brief get day component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getDay			() const;

	//! \brief get hour component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getHour			() const;
	//! \brief get minute component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getMinute		() const;
	//! \brief get second component			\n no-throw
	LIBBASARCMNUTIL_API Int16			getSec			() const;
	//! \brief get millisecond component	\n no-throw
	LIBBASARCMNUTIL_API Int16			getMSec			() const;

    //! \brief get pointer to datetime buffer	\n no-throw
	LIBBASARCMNUTIL_API const dtime * 	getTimestampBuffer	() const;

	//--------------------------------------------------------------------------------------------------//

	//! \brief set integer date					\n no-throw
	LIBBASARCMNUTIL_API	void			setDate			(const Int32 date		//!< integer formatted date (YYYYMMDD)
														);
	//! \brief set integer date					\n no-throw
	LIBBASARCMNUTIL_API	void			setDate			(const Int32 year,		//!< year		   0 - 9999
														 const Int32 month,		//!< month         1 -   12 -> month of year
														 const Int32 day		//!< day           1 -   31 -> day of month
														);
	//! \brief set integer time					\n no-throw
	LIBBASARCMNUTIL_API	void			setTime			(const Int32 time		//!< integer formatted time (hhmmssttt)
														);
	//! \brief set integer time					\n no-throw
	LIBBASARCMNUTIL_API	void			setTime			( const Int32 hour,		//!< hour          0 -  23 -> hour since midnight
														  const Int32 min,		//!< minutes       0 -  59 -> minutes after hour
														  const Int32 sec,		//!< seconds       0 -  59 -> seconds after minute
														  const Int32 msec = 0	//!< milli seconds 0 - 999 -> milli seconds after second
														);

	//! \brief set datetime member to invalid state	\n no-throw
	LIBBASARCMNUTIL_API void			setInvalid		();
	//! \brief set date member to invalid state		\n no-throw
	LIBBASARCMNUTIL_API void			setInvalidDate	();
	//! \brief set time member to invalid state		\n no-throw
	LIBBASARCMNUTIL_API void			setInvalidTime	();

	//! \brief set internal range units (year, ..., msec)		\n no-throw
	LIBBASARCMNUTIL_API void			setRange		(DateTimeUnitEnum from,		//!< biggest datetime unit
														 DateTimeUnitEnum to		//!< smallest datetime unit
														);

	//--------------------------------------------------------------------------------------------------//

	//! \brief is object's datetime correctly set?	\n no-throw
	LIBBASARCMNUTIL_API bool			isValid			() const;
	//! \brief is object's date correctly set?		\n no-throw
	LIBBASARCMNUTIL_API bool			isValidDate		() const;
	//! \brief is object's time correctly set?		\n no-throw
	LIBBASARCMNUTIL_API bool			isValidTime		() const;

    //! \brief returns whether or not DateTime is DST
    LIBBASARCMNUTIL_API bool isDST() const;

	//--------------------------------------------------------------------------------------------------//

	//! \brief adding days							\n no-throw
	LIBBASARCMNUTIL_API	void			addDays			(const Int32 days	//!< days to add, can be negative
														);


    //! \brief adding months							\n no-throw
    LIBBASARCMNUTIL_API void            addMonths       (const Int32 months //!< months to add, can be negative
                                                        );

    //! \brief adding years							\n no-throw
    LIBBASARCMNUTIL_API void            addYears        (const Int32 years //!< years to add, can be negative
                                                        );

	//! \brief adding time showing time wrap		\n no-throw
	LIBBASARCMNUTIL_API	TimeCalcEnum	addTime			(const Int32 time	//!< integer formatted time (hhmmssttt) to add, can be negative
														);

	//--------------------------------------------------------------------------------------------------//

	//! \brief converting to string using format-control string							\n no-throw 
	LIBBASARCMNUTIL_API	const I18nString	toString		(ConstString format	//!< format control string, see Informix SQL-Reference DBTIME
															) const;
	//! \brief converting to Internet Message format (see RFC 2822)						\n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrInternetMsg() const;
	//! \brief converting to date representation for current locale; see strftime("%x")	\n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrDate		() const;
	//! \brief converting to time representation for current locale; see strftime("%X")	\n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrTimeShort	() const;
	//! \brief converting to time representation "hh:mm:ss.ttt"							\n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrTime		() const;

    //!\brief converting to a string with date and time representation for current locale	\n no-throw
	LIBBASARCMNUTIL_API	const I18nString	toStrDateTime	() const;

	//--------------------------------------------------------------------------------------------------//

	//! \brief converting from string using format-control string						\n throw DateTimeException
	LIBBASARCMNUTIL_API	void				fromString 		(ConstString        format,		//!< format control string, see Informix SQL-Reference DBTIME
															 const I18nString & rDateTime	//!< date string in representation same as from toStrDate(); depends on locale
															);
	//! \brief converting from date representation for current locale (same as return value from toStrDate())		\n no-throw
	LIBBASARCMNUTIL_API	void				fromStrDate		(const I18nString & rDate		//!< date string in representation same as from toStrDate(); depends on locale
															);
	//! \brief converting from time representation "hh*mm*ss"							\n no-throw
	LIBBASARCMNUTIL_API	void				fromStrTimeShort(const I18nString & rTime		//!< time string in representation "hh*mm*ss"
															);
	//! \brief converting from time representation "hh*mm*ss*ttt"						\n no-throw
	LIBBASARCMNUTIL_API	void				fromStrTime		(const I18nString & rTime		//!< time string in representation "hh*mm*ss*ttt"
															);

	//! \brief converting from date representation and time representation					\n no-throw
	LIBBASARCMNUTIL_API	void				fromStrDateTime	(const I18nString & rTime		//!<date string + time string
                                                             );



	//--------------------------------------------------------------------------------------------------//
	//	operators
	//--------------------------------------------------------------------------------------------------//

	//! \brief	checking equality logically: e.g. 
	//! \n      DB selected value (2008-01-12 09:32, year to minute) == programmatically created value (2008-01-12 09:32:00.000, year to fraction(3))
	//! \n throw DateTimeException
	LIBBASARCMNUTIL_API bool			operator ==		(const DateTime & rDt	//!< passed object to compare with context object
														) const;
    //! \brief	checking inequality
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API bool            operator!= 		(const DateTime & rDt	//!< passed object to compare with context object
														) const;

    //! \brief assignment operator, needed b/c of const member
   	LIBBASARCMNUTIL_API DateTime&       operator =		(const DateTime & rDt	//!< passed object to assing to context object
	                                                    );

    //! \brief add a timespan resulting in an other datetime
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API DateTime&       operator+=      (const TimeSpan& right );

    //! \brief subtract a timespan, resulting in a datetime
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API DateTime&       operator-=      (const TimeSpan& right );

    //! \brief subtract a datetime, resulting in a timespan
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API const TimeSpan  operator-       (const DateTime& right  //!< passed datetime to subtract from datetime
                                                        ) const;

    //! \brief	checking less than
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API bool            operator<       (const DateTime& rDt    //!< passed object to compare with context object
                                                        ) const;
    //! \brief	checking greater than
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API bool            operator>       (const DateTime& rDt    //!< passed object to compare with context object
                                                        ) const;

     //! \brief	checking less than or equal to
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API bool            operator<=       (const DateTime& rDt    //!< passed object to compare with context object
                                                        ) const;
    //! \brief	checking greater than or equal to
    //! \n throw DateTimeException
    LIBBASARCMNUTIL_API bool            operator>=       (const DateTime& rDt    //!< passed object to compare with context object
                                                        ) const;

	//--------------------------------------------------------------------------------------------------//
	//	friends
	//--------------------------------------------------------------------------------------------------//

    friend class TimeSpan;

private:
    //--------------------------------------------------------------------------------------------------//
	// private enum
	//--------------------------------------------------------------------------------------------------//

    //! \brief enum for mapping CSDK's deccmp values to something readable
    enum ResultOfCompairison
    {
        LESS_THAN = -1,
        EQUAL = 0,
        GREATER_THAN = 1
    };

    //--------------------------------------------------------------------------------------------------//
	// methods
	//--------------------------------------------------------------------------------------------------//


	//! set current localized system timestamp to member		\n throw DateTimeException
	void        setCurrent		();

	//! get string from member using format control string without NULL checking	\n throw DateTimeException
	I18nString  getDateTime		(ConstString format	//! format control string
								) const;
	//! \brief is date part NULL?	\n no-throw
	bool		isNullDate		() const;
	//! \brief is time part NULL?	\n no-throw
	bool		isNullTime		() const;

	//--------------------------------------------------------------------------------------------------//
	// member section
	//--------------------------------------------------------------------------------------------------//

	Timestamp* m_Stamp;				//!< holds date/time in range from 0001-01-01 00:00:00.000 to 9999-12-31 23:59:59.999

    const bool m_ownsTimestamp;		//!< bool indication if memory behind m_Date was allocated by Date instance and thus has to be deleted upon destruction

    bool       m_hasTimeBeenSet;	//!< bool indication if time has been set by user -> interpret 23:59:59.999 as valid time 

     //! \brief returns the standard date format for the actual locale
    ConstBuffer getDateFormat() const;
     //! \brief returns the standard time format for the actual locale
    ConstBuffer getTimeFormat() const;
     //! \brief returns the standard short time format for the actual locale
    ConstBuffer getShortTimeFormat() const;
    
    //! \brief returns the length of a standard date string used in fromDateTimeStr() )
    VarString::size_type getStrDateLength() const;
    
    //! \brief compaires (shrinks larger datetime if necessary) and returns result
    inline ResultOfCompairison internalCompairSizeWithThis(const DateTime& rHs) const;

    //! \brief adds a given amount of time unit to this, e.g. adds 5 years
    inline void internalAddTimeUnitAmount( const basar::Int16 unit, const basar::Int32 amount, ConstString formatString );
};
//----------------------------------------------------------------------------

//no friend necessary - operator uses public function
LIBBASARCMNUTIL_API std::ostream& operator<< (std::ostream& stream, const DateTime& dt);

//! \brief add a timespan resulting in an other datetime
//! \n throw DateTimeException
LIBBASARCMNUTIL_API const DateTime  operator+       (const DateTime& left,  //!< summand
                                                     const TimeSpan& right  //!< summand
                                                        );

//! \brief add a timespan resulting in an other datetime
//! \n throw DateTimeException
LIBBASARCMNUTIL_API const DateTime  operator+       (const TimeSpan& left,  //!< summand
                                                     const DateTime& right  //!< summand
                                                        );

//! \brief subtract a timespan, resulting in a datetime
//! \n throw DateTimeException
LIBBASARCMNUTIL_API const DateTime  operator-       (const DateTime& left,  //!< summand
                                                     const TimeSpan& right  //!< summand
                                                    );

}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

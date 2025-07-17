//---------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  datetime class implementation 
 *  \author Thomas Hörath
 *  \date   02.08.2005
 */
//---------------------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_datetimetools.h"
#include "timestamp.h"
#include "libbasarcmnutil_timespan.h"
#include "libbasarcmnutil_date.h"

#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_locale.h"
#include "libbasardebug.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/timeb.h>
#include <time.h>

#include <ostream>

//---------------------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
void dumpDateTime(size_t addr)
{
	if (0 == addr)
		return;

	const DateTime * const pDtime = reinterpret_cast<const DateTime * const>(addr);
	VarString        out;

	out.format("#0x%0*lx: %s\n",
		       sizeof(size_t) * 2, pDtime,
			   pDtime->toString(FORMAT_SQL_DATETIME_INFORMIX).c_str());

	debug::printDbgMsg(out.c_str());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Returns the dateformat for a locale
    \return FormatString for the actual locale */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer DateTime::getDateFormat()const
{
	static ConstString dateformat[] = 
	{
    	"%02d.%02m.%04Y",
	    "%02d/%02m/%04Y",
    	"%02m/%02d/%04Y"
	};

    switch ( Locale::getDateFormat())
    {
    case Locale::FMT_DATE_AMERICAN:
        return dateformat[2];

    case Locale::FMT_DATE_EUROPEAN_SLASH:
        return dateformat[1];

    case Locale::FMT_DATE_EUROPEAN:
    default:
        return dateformat[0];
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Returns the number of charcters in a standard date string, returned by toStrDate()
    \throw no-throw
    \return  the number of charcters in a standard date string, returned by toStrDate()*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VarString::size_type DateTime::getStrDateLength() const
{
    // for all locales currently the same length
    return 10;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Returns the timeformat for a locale
    \return FormatString for the actual locale */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer DateTime::getTimeFormat() const
{
	static ConstString timeformat[] =
	{	
    	"%02H:%02M:%02S.%03F3",
	    "%02I:%02M:%02S.%03F3 %p",
	};
	
    switch (Locale::getTimeFormat())
    {
	case Locale::FMT_TIME_AMERICAN:
	    return timeformat[1] ;

    case Locale::FMT_TIME_EUROPEAN:
    default:
		return timeformat[0];
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Returns the short time format for a locale
    \return FormatString for the actual locale */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer DateTime::getShortTimeFormat() const
{
	static ConstString shorttimeformat[] =
	{	
	    "%02H:%02M:%02S",
	    "%02I:%02M:%02S %p",
	};
	
    switch (Locale::getTimeFormat())
    {
	case Locale::FMT_TIME_AMERICAN:
        return shorttimeformat[1] ;

    case Locale::FMT_TIME_EUROPEAN:
    default:
		return shorttimeformat[0];
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return current localized system date */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime DateTime::getCurrent()
{
	return DateTime();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return size of internal data structure */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int DateTime::getInfxDescSize()
{
    static int infxDescSize = 0;
    
    if (0 == infxDescSize)
    {
        infxDescSize = sizeof(Timestamp);
    }

    return infxDescSize;
}

//---------------------------------------------------------------------------------------------------------------------//
// object member section (object methods)
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime() : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(false)
{
	try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}
	
	setCurrent();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime (const Int32 date, const Int32 time /* = NULL_TIME */)  : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(false)
{
	try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}

	setDate(date);
	setTime(time);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime (const Int32 year, const Int32 month, const Int32 day) : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(false)
{
	try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}
	
	setDate(year, month, day);
	setInvalidTime();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime (const Int32 hour, const Int32 min, const Int32 sec, const Int32 msec) : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(false)
{
	try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}

	setInvalidDate();
	setTime(hour, min, sec, msec);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime(const dtime* pTime) : m_Stamp(reinterpret_cast<Timestamp*>(const_cast<dtime_t*>(pTime))), m_ownsTimestamp(false), m_hasTimeBeenSet(false)
{
    //check if Time is valid, 2 steps
    //first check if time is valid
    if (!isNullTime())
    {
        m_hasTimeBeenSet = true;
    }
    else
    {
        //now check if date and time are both valid
        //statement is equal to !isNullDate() && !isNullTime()
        m_hasTimeBeenSet = !(isNullDate() || isNullTime());
    }
    
}

DateTime::DateTime(const DateTime& rDt) : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(rDt.m_hasTimeBeenSet)
{
    try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}
	
	memcpy(this->m_Stamp, rDt.m_Stamp, sizeof(Timestamp));
}

DateTime::DateTime( const Date& rDate	) : m_Stamp(0), m_ownsTimestamp(true), m_hasTimeBeenSet(false)
{
	try
	{
		m_Stamp = new Timestamp(DATETIME);
	}
	catch(...)
	{
		delete m_Stamp;
		m_Stamp = 0;
		throw;
	}

	setDate(rDate.getDate());
	setInvalidTime();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::~DateTime	()
{
    if (m_ownsTimestamp)
    {
        delete m_Stamp;
        m_Stamp = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
	\return integer date */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTime::getDate () const
{
	if ( !isValidDate() )
		return NULL_DATE;

	VarString date = getDateTime(dt_format_date());

	return DateTimeTools::getDate(date.substr(0, 4).stoi(),
		                          date.substr(4, 2).stoi(),
								  date.substr(6, 2).stoi());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
	\return integer time */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTime::getTime () const
{
    if ( (!isValidDate() && !isValidTime()) || !m_hasTimeBeenSet )
		return NULL_TIME;

	VarString time = getDateTime(dt_format_time());

	return DateTimeTools::getTime(time.substr(0, 2).stoi(),
		                          time.substr(2, 2).stoi(),
								  time.substr(4, 2).stoi(),
								  time.substr(7, 3).stoi());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return year component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getYear() const
{
	if ( !isValidDate() )
		return NULL_DATE;

	return DateTimeTools::getYear(getDate());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return month component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getMonth() const
{
	if ( !isValidDate() )
		return NULL_DATE;

	return DateTimeTools::getMonth(getDate());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return day component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getDay() const
{
	if ( !isValidDate() )
		return NULL_DATE;

	return DateTimeTools::getDay(getDate());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return hour component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getHour() const
{
	if ( (!isValidDate() && !isValidTime()) || !m_hasTimeBeenSet )
		return NULL_TIME;

	return DateTimeTools::getHour(getTime());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return minute component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getMinute() const
{
	if ( (!isValidDate() && !isValidTime()) || !m_hasTimeBeenSet)
		return NULL_TIME;

	return DateTimeTools::getMinute(getTime());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return second component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTime::getSec() const
{
	if ( (!isValidDate() && !isValidTime()) || !m_hasTimeBeenSet)
		return NULL_TIME;

	return DateTimeTools::getSec(getTime());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return millisecond component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16	DateTime::getMSec() const
{
	if ( (!isValidDate() && !isValidTime()) || !m_hasTimeBeenSet)
		return NULL_TIME;

	return DateTimeTools::getMSec(getTime());
}


const dtime_t* 		DateTime::getTimestampBuffer			() const
{
    return reinterpret_cast<const dtime_t* const>(m_Stamp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setDate (const Int32 date)
{
	setDate(DateTimeTools::getYear (date),
			DateTimeTools::getMonth(date),
			DateTimeTools::getDay  (date));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setDate (const Int32 year, const Int32 month, const Int32 day)
{
	if ( !DateTimeTools::isValidDate(year, month, day) )
	{
		setInvalidDate();
		return;
	}

	VarString dt;

	dt.format(crt_format_date(), year, month, day);		// store date part
	dt += getDateTime(dt_format_time());				// append time part to dt

	VarString fmt = dt_format_date();
	fmt += dt_format_time();

	fromString(fmt.c_str(), dt);						// store total datetime in member
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setTime (const Int32 time)
{
	setTime(DateTimeTools::getHour  (time),
			DateTimeTools::getMinute(time),
			DateTimeTools::getSec   (time),
			DateTimeTools::getMSec  (time));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setTime (const Int32 hour, const Int32 min, const Int32 sec, const Int32 msec /* = 0 */)
{
	if ( !DateTimeTools::isValidTime(hour, min, sec, msec) )
	{
		setInvalidTime();
		return;
	}

	VarString tm;
	VarString dt = getDateTime(dt_format_date());		// save date

	tm.format(crt_format_time(), hour, min, sec, msec);	// calc time
	dt += tm;											// append time to saved date

	VarString fmt = dt_format_date();
	fmt += dt_format_time();

	fromString(fmt.c_str(), dt);						// store total datetime in member

    m_hasTimeBeenSet = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setInvalid()
{
	setInvalidDate();
	setInvalidTime();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setInvalidDate()
{
	VarString dt = nullvalue(0);				// store NULL DATE

	dt += getDateTime(dt_format_time());		// append time to dt
	
	VarString fmt = dt_format_date();
	fmt += dt_format_time();
	fromString(fmt.c_str(), dt);				// store total datetime in member
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setInvalidTime()
{
	VarString dt = getDateTime(dt_format_date());	// save date

	dt += nullvalue(1);								// append NULL TIME to saved date
	
	VarString fmt = dt_format_date();
	fmt += dt_format_time();

	fromString(fmt.c_str(), dt);                    // store total datetime in member

    m_hasTimeBeenSet = false;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setRange(DateTimeUnitEnum from,
						DateTimeUnitEnum to  )
{
	dtime_t dtTmp = *reinterpret_cast<dtime_t *>(m_Stamp);						// copy current value to temporary variable

	static const Int16 DATETIME_UNITS[] = 
	{
		TU_YEAR,			//!< year
		TU_MONTH,			//!< month
		TU_DAY,				//!< day
		TU_HOUR,			//!< hour
		TU_MINUTE,			//!< minute
		TU_SECOND,			//!< second
		TU_F1,				//!< 10th second
		TU_F2,				//!< 100th second
		TU_F3,				//!< 1000th second
		TU_F4,				//!< 10000th second
		TU_F5				//!< 100000th second
	};

	m_Stamp->m_Qual = TU_DTENCODE(DATETIME_UNITS[from], DATETIME_UNITS[to]);		// set new range
	::dtextend(&dtTmp, reinterpret_cast<dtime_t *>(m_Stamp));					// copy saved value to extended member

    //check DateTimeUnitEnum to if time is still valid (based on set range)
    if ( DateTime::DTUNIT_YEAR  == to ||
         DateTime::DTUNIT_MONTH == to ||
         DateTime::DTUNIT_DAY   == to
        )
    {
        m_hasTimeBeenSet = false;
    }
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true  if date and time is correctly set/valid 
	\retval false if date and time is not set/invalid     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::isValid() const
{
    return (isValidDate() && (isValidTime() || m_hasTimeBeenSet));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true  if date is correctly set/valid 
	\retval false if date is not set/invalid     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::isValidDate() const
{
	return !isNullDate();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true  if time is correctly set/valid 
	\retval false if time is not set/invalid     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::isValidTime() const
{
	return !isNullTime();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	no operation if date is not appropriate
    \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::addDays( const Int32 days )
{
	if ( !isValidDate() )
    {
		return;
    }

    internalAddTimeUnitAmount( TU_DAY, days, "%d" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	no operation if date is not appropriate
    \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::addMonths (const Int32 months )
{
	if ( !isValidDate() )
    {
		return;
    }

    internalAddTimeUnitAmount( TU_MONTH, months, "%m" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	no operation if date is not appropriate
    \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::addYears( const Int32 years )
{
	if ( !isValidDate() )
    {
		return;
    }

    internalAddTimeUnitAmount( TU_YEAR, years, "%Y" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	time will wrap if it passes midnight.
	\throw  no-throw 
	\return flag for time wrapping if added time passes midnight */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime::TimeCalcEnum DateTime::addTime (const Int32 time)
{
	Int32 absTime = ::abs(time);

	if ( ! isValidTime() || ! DateTimeTools::isValidTime(absTime) )
    {
		return DAY_SAME;
    }

	// --- calc interval

	static const Int16 INTV_FIELD0_LEN = 2;
	char  tm[16];

	::sprintf(tm,	"%0*d:%02d:%02d.%03d",				// calculate time
					INTV_FIELD0_LEN,
					DateTimeTools::getHour  (absTime),	
					DateTimeTools::getMinute(absTime),
					DateTimeTools::getSec   (absTime),
					DateTimeTools::getMSec  (absTime));

	intrvl_t inv;
	inv.in_qual = TU_IENCODE(INTV_FIELD0_LEN, TU_HOUR, TU_F3);							// length of hour

	static ConstString INTV_FORMAT = "%02.2H:%02.2M:%02.2S%03.3F3";

	if (0 > ::incvfmtasc(tm, const_cast<char *>(INTV_FORMAT), &inv))		// convert to interval
		return DAY_SAME;


	// --- calc datetime

	DateTime  output;
	DateTime  input (getDate(), getTime());		// duplicate to variable with full range year to fraction: 
												// suitable range needed for dtadd/subinv() (see Informix manual)
	bool flagValidDate = isValidDate();			// save original value for further processing

	if ( ! flagValidDate )
		input.setDate(20000101);				// 2000-01-01: set to any valid date for Informix function

	Int32 dateBefore = input.getDate();			// save day for comparing with output

	dtime_t * pDtOut = reinterpret_cast<dtime_t *>(output.m_Stamp);
	dtime_t * pDtIn  = reinterpret_cast<dtime_t *>(input .m_Stamp);

	// datetime value must include all the fields present in interval value
	if ( 0 > ( (time >= 0) ? ::dtaddinv(pDtIn, &inv, pDtOut) 
						   : ::dtsubinv(pDtIn, &inv, pDtOut) ) )
		return DAY_SAME;

	::dtextend(pDtOut, reinterpret_cast<dtime_t *>(m_Stamp));	// copy output to *this regarding datetime qualifier

	// correct original invalid date
	if ( ! flagValidDate )
		setInvalidDate();

	Int32 dateAfter = output.getDate();			// save day for comparing with input

	return (dateAfter > dateBefore) ? DAY_AFTER : ((dateAfter == dateBefore) ? DAY_SAME : DAY_BEFORE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Format control (see also strftime()):
	\n		\%a  is replaced by abbreviated weekday name 
	\n		\%A  is replaced by full weekday name 
	\n      \%b  is replaced by abbreviated month name
	\n      \%B  is replaced by full month name
	\n		\%c  is replaced by date and time representation (YYYY-MM-DD hh:mm:ss)
	\n		\%C  is replaced by century
	\n      \%d  is replaced by day of month as decimal number [01,31]
	\n      \%D  is replaced by date representation (MM/DD/YY)
	\n      \%Fn is replaced by fraction of second (including point up until informix 11.70xC7) 
    \n          with scale that integer n	specifies.
	\n          default value of n is 2; range of n is 0 = n = 5
	\n      \%H  is replaced by hour (24-hour clock)
	\n      \%h  same as %b
	\n      \%I  is replaced by hour (12-hour clock)
	\n      \%M  is replaced by minute as a decimal number [00,59]
	\n      \%m  is replaced by month as a decimal number [01,12]
	\n      \%p  is replaced by A.M. or P.M. (or equivalent in the locale file)
	\n      \%S  is replaced by second as a decimal number [00,59]
	\n		\%u	is replaced by day of week (0 = Sunday
	\n		\%w	same as %u
	\n		\%x  is replaced by date representation (MM/DD/YYYY)
	\n		\%X	is replaced by time representation (hh:mm:ss)
	\n      \%y  is replaced by year as a two-digit decimal number
	\n      \%Y  is replaced by year as a four-digit decimal number. 
	\n      \%\%  is replaced by \% (to allow \% in format string)
	\throw  no-throw
	\return converted string using format-control string 
	\retval empty string if date/time is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString	DateTime::toString (ConstString format) const
{
	assert(format);

//	allow NULL value for saving unset DB datetime, don't perform any checks

	return getDateTime(format);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\return internet message format string, e.g. "30 Oct 2006 13:16:12 +0100" 
	\retval empty string if date/time is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString	DateTime::toStrInternetMsg() const
{
	if ( !isValid() )
		return "";

	static ConstString MONTHNAME[] = 
	{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static const Long32 DIVISOR    = -36;

	VarString date;

#if _MSC_VER < 1900 || !defined(_MSC_VER)
	long timeZone = timezone;
#else
	long timeZone = 0;
	_get_timezone(&timeZone);
#endif

    Long32 dstOffset = timeZone/DIVISOR;

    if ( true == this->isDST() )
    {
        dstOffset += 100;
    }

	date.format("%d %s %d %02d:%02d:%02d %+0.4ld",
		        getDay   (),
			    MONTHNAME[getMonth() - 1],
		        getYear  (),
		        getHour  (),
				getMinute(),
				getSec   (),
			    (dstOffset));

	return date;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return date representation for current locale
	\retval empty string if date is invalid 
	\note   provides only last 2 digits of year in some locals */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString DateTime::toStrDate() const
{
   return toString(getDateFormat());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\return time representation for current locale 
	\retval empty string if time is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString DateTime::toStrTimeShort() const
{
    return toString(getShortTimeFormat());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return time representation "hh:mm:ss.ttt" 
	\retval empty string if time is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString DateTime::toStrTime() const
{
	return toString(getTimeFormat());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Format control (see also strftime()):
	\n		%a  is replaced by abbreviated weekday name 
	\n		%A  is replaced by full weekday name 
	\n      %b  is replaced by abbreviated month name
	\n      %B  is replaced by full month name
	\n		%c  is replaced by date and time representation (YYYY-MM-DD hh:mm:ss)
	\n		%C  is replaced by century
	\n      %d  is replaced by day of month as decimal number [01,31]
	\n      %D  is replaced by date representation (MM/DD/YY)
    \n      %Fn is replaced by fraction of second (including point up until informix 11.70xC7)
    \n          with scale that integer n	specifies.
    \n          default value of n is 2; range of n is 0 = n = 5
	\n      %H  is replaced by hour (24-hour clock)
	\n      %h  same as %b
	\n      %I  is replaced by hour (12-hour clock)
	\n      %M  is replaced by minute as a decimal number [00,59]
	\n      %m  is replaced by month as a decimal number [01,12]
	\n      %p  is replaced by A.M. or P.M. (or equivalent in the locale file)
	\n      %S  is replaced by second as a decimal number [00,59]
	\n		%u	is replaced by day of week (0 = Sunday
	\n		%w	same as %u
	\n		%x  is replaced by date representation (MM/DD/YYYY)
	\n		%X	is replaced by time representation (hh:mm:ss)
	\n      %y  is replaced by year as a two-digit decimal number
	\n      %Y  is replaced by year as a four-digit decimal number. 
	\n      %%  is replaced by % (to allow % in format string)
	\throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::fromString(ConstString format, const I18nString & rDateTime)
{
	assert(format);

	const char * fmt;
	const char * pBuf;
	char         buf   [32];
	char         fmtbuf[32];

	if (rDateTime.empty())
	{
		::sprintf(buf   , "%s%s", nullvalue(0), nullvalue(1) );
		::sprintf(fmtbuf, "%s%s", dt_format_date(), dt_format_time());
		fmt  = fmtbuf;
		pBuf = buf;
	}
	else
	{
		fmt  = format;
		pBuf = rDateTime.c_str();
	}

	if ( 0 > ::dtcvfmtasc(const_cast      <char    *>(pBuf    ),
		                  const_cast      <char    *>(fmt     ),
						  reinterpret_cast<dtime_t *>(m_Stamp)) )
	{
		VarString msg;
		msg.format("::dtcvfmtasc(format=\"%s\", <%s>) failed", format, rDateTime.c_str());
		ExceptInfo sInfo("basar.cmnutil.DateTime.fromString()", msg, __FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::fromStrDate(const I18nString & rDate)
{
	Int32 save = getTime();
	fromString(getDateFormat(), rDate);
	setTime(save);
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::fromStrTimeShort (const I18nString & rTime)
{
	Int32 save = getDate();
    fromString(getShortTimeFormat(),  rTime);
	setDate(save);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::fromStrTime (const I18nString & rTime)
{
	Int32 save = getDate();
	fromString(getTimeFormat(),  rTime);
	setDate(save);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw
	\retval	true if context object is congruent with passed object
	\retval	false otherwise	 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::operator == (const DateTime& rDt) const
{
    return EQUAL == internalCompairSizeWithThis(rDt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw
	\retval	false if context object is congruent with passed object
	\retval	true otherwise	 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool DateTime::operator!= (const DateTime & rDt) const
{
    return !(operator==(rDt));
}

//! \return reference to DateTime object which was assigned
DateTime& DateTime::operator = (const DateTime& rDt)
{
    //since assignment can only happen on non-const objects
    //we can savely(?) assume that we own our timestamp
    
    assert(m_ownsTimestamp == true);

    if (this != &rDt)
    {
        m_hasTimeBeenSet = rDt.m_hasTimeBeenSet;
        memcpy(this->m_Stamp, rDt.m_Stamp, sizeof(Timestamp));
    }

    return *this;
}

LIBBASARCMNUTIL_API DateTime & DateTime::operator+=(const TimeSpan & right)
{
    Int32 ret = ::dtaddinv(reinterpret_cast< dtime_t* >(m_Stamp), const_cast<intrvl_t*>(right.getTimestampBuffer()), reinterpret_cast< dtime_t* >(m_Stamp));

    if (0 > ret)
	{
		VarString msg;
		msg.format("::dtaddinv failed with %d", 
					ret);
		ExceptInfo sInfo("basar.cmnutil.DateTime.operator+()", msg, __FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}

    return *this;
}

DateTime & DateTime::operator-=(const TimeSpan & right)
{
    Int32 ret = ::dtsubinv(reinterpret_cast< dtime_t* >(m_Stamp), reinterpret_cast<intrvl_t*>(right.m_Stamp), reinterpret_cast< dtime_t* >(m_Stamp));

    if (0 > ret)
	{
		VarString msg;
		msg.format("::dtsubinv failed with %d", 
					ret);
		ExceptInfo sInfo("basar.cmnutil.DateTime.operator-()", msg, __FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}
    return *this;
}

//! \return new TimeSpan instance containing result of substraction
const TimeSpan DateTime::operator-(const DateTime& right) const
{
    TimeSpan res;
    
    Int32 ret = ::dtsub(reinterpret_cast< dtime_t* >(m_Stamp), reinterpret_cast< dtime_t* >(right.m_Stamp), reinterpret_cast<intrvl_t*>(res.m_Stamp));

    if (0 > ret)
	{
		VarString msg;
		msg.format("::dtsub failed with %d", 
					ret);
		ExceptInfo sInfo("basar.cmnutil.DateTime.operator-()", msg, __FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}

    return res;
}

//! \return bool indication if instance is less than passed rDt
bool DateTime::operator<(const DateTime& rDt) const
{
    return LESS_THAN == internalCompairSizeWithThis(rDt);
}

//! \return bool indication if instance is greater than passed rDt
bool DateTime::operator>(const DateTime& rDt) const
{
    return GREATER_THAN == internalCompairSizeWithThis(rDt);
}

//! \return bool indication if instance is less than or equal to passed rDt
bool DateTime::operator<=(const DateTime& rDt) const
{
    return (EQUAL == internalCompairSizeWithThis(rDt) || LESS_THAN == internalCompairSizeWithThis(rDt) );
}

//! \return bool indication if instance is greater or equal to than passed rDt
bool DateTime::operator>=(const DateTime& rDt) const
{
    return (EQUAL == internalCompairSizeWithThis(rDt) || GREATER_THAN == internalCompairSizeWithThis(rDt));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::setCurrent()
{
	// Informix CSDK time resolution is only 10 ms => use CRT 1 ms
	// ::dtcurrent(reinterpret_cast<dtime_t *>(&m_Stamp));  

	::tm  * pBdTime;	// CRT broken-down time
	Int32   msec;

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	timeb   calTime;	// struct with CRT calendar time and milli seconds

	ftime(&calTime);

	if ( 0 == (pBdTime = ::localtime(&(calTime.time))) )
	{
		ExceptInfo sInfo("basar.cmnutil.DateTime.setCurrent()", 
						 "::localtime() returns 0", 
						__FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}

	msec = calTime.millitm;

#else

	timeval calTime;	// struct with CRT calendar time and micro seconds

	::gettimeofday(&calTime, 0);

	if ( 0 == (pBdTime = ::localtime(&(calTime.tv_sec))) )
	{
		ExceptInfo sInfo("basar.cmnutil.DateTime.setCurrent()", 
						 "::localtime() returns 0", 
						__FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}

	msec = static_cast<Int32>(calTime.tv_usec / 1000);	// usec => msec

#endif

	setDate(pBdTime->tm_year + 1900, 
		    pBdTime->tm_mon  +    1, 
			pBdTime->tm_mday       );

	setTime(pBdTime->tm_hour,
		    pBdTime->tm_min ,
			pBdTime->tm_sec , 
			msec            );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Format control (see also strftime()):
	\n		%a  is replaced by abbreviated weekday name 
	\n		%A  is replaced by full weekday name 
	\n      %b  is replaced by abbreviated month name
	\n      %B  is replaced by full month name
	\n		%c  is replaced by date and time representation (YYYY-MM-DD hh:mm:ss)
	\n		%C  is replaced by century
	\n      %d  is replaced by day of month as decimal number [01,31]
	\n      %D  is replaced by date representation (MM/DD/YY)
    \n      %Fn is replaced by fraction of second (including point up until informix 11.70xC7)
    \n          with scale that integer n	specifies.
    \n          default value of n is 2; range of n is 0 = n = 5
	\n      %H  is replaced by hour (24-hour clock)
	\n      %h  same as %b
	\n      %I  is replaced by hour (12-hour clock)
	\n      %M  is replaced by minute as a decimal number [00,59]
	\n      %m  is replaced by month as a decimal number [01,12]
	\n      %p  is replaced by A.M. or P.M. (or equivalent in the locale file)
	\n      %S  is replaced by second as a decimal number [00,59]
	\n		%u	is replaced by day of week (0 = Sunday
	\n		%w	same as %u
	\n		%x  is replaced by date representation (MM/DD/YYYY)
	\n		%X	is replaced by time representation (hh:mm:ss)
	\n      %y  is replaced by year as a two-digit decimal number
	\n      %Y  is replaced by year as a four-digit decimal number. 
	\n      %%  is replaced by % (to allow % in format string)
	\throw  DateTimeException
	\return converted string using format-control string */ 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString DateTime::getDateTime (ConstString format) const
{
	assert(format);

	VarString buf;

	{
		const Int32 bufsize = 32;
		
		// calc date part
		char        bufdt[bufsize] = { 0 };

		if ( TU_DAY  < TU_START(m_Stamp->m_Qual) )	// not in range year to day  => invalid date
			::strcpy(bufdt, nullvalue(0));
		else
			::dttofmtasc(reinterpret_cast < dtime_t * >(m_Stamp) ,
						 bufdt, bufsize,
						 const_cast < char * > (dt_format_date()));

		// calc time part
		char buftm[bufsize] = { 0 };

		if ( TU_HOUR > TU_END  (m_Stamp->m_Qual) )	// not in range hour to msec => invalid time
			::strcpy(buftm, nullvalue(1));
		else
			::dttofmtasc(reinterpret_cast < dtime_t * >(m_Stamp) ,
						buftm, bufsize,
						const_cast < char * > (dt_format_time()));

		// compose buffer with datetime
		buf.format("%s %s", bufdt, buftm);
	}


	// copy to datetime clone
	dtime_t clone;

	{
		clone.dt_qual = TU_DTENCODE(TU_YEAR, TU_F3);

		VarString fmt;
		fmt.format("%s %s", dt_format_date(), dt_format_time()); 

		Int32 ret = ::dtcvfmtasc(const_cast < char * > (buf.c_str()), const_cast < char * > (fmt.c_str()), &clone);

		if (0 > ret)
		{
			VarString msg;
			msg.format("::dtcvfmtasc(%s, \"%s\") failed with %d", 
				        buf.c_str(),
						fmt.c_str(), 
						ret);
			ExceptInfo sInfo("basar.cmnutil.DateTime.getDateTime()", msg, __FILE__, __LINE__);
			throw DateTimeException(sInfo);
		}
	}


	// copy clone to return value with callers format string
	const Int32 dtsize     = 1024;
	char        dt[dtsize] = { 0 };

	if ( 0 > ::dttofmtasc(&clone, dt, dtsize, const_cast<char *>(format)) )
	{
		VarString msg;
		msg.format("::dttofmtasc((%02d%02d-%02d-%02d %02d:%02d:%02d.%02d%02d), format=\"%s\", buffer length = %d) failed", 
			        clone.dt_dec.dec_dgts[0],
			        clone.dt_dec.dec_dgts[1],
			        clone.dt_dec.dec_dgts[2],
			        clone.dt_dec.dec_dgts[3],
			        clone.dt_dec.dec_dgts[4],
			        clone.dt_dec.dec_dgts[5],
			        clone.dt_dec.dec_dgts[6],
			        clone.dt_dec.dec_dgts[7],
			        clone.dt_dec.dec_dgts[8],
					format, 
					sizeof(dt));
		ExceptInfo sInfo("basar.cmnutil.DateTime.getDateTime()", msg, __FILE__, __LINE__);
		throw DateTimeException(sInfo);
	}

	return dt;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is NULL_DATE or failure
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::isNullDate () const
{
	if ( TU_DAY < TU_START(m_Stamp->m_Qual) )	// not in range year to day
		return true;

	VarString date = getDateTime(dt_format_date());

	return ( 0 == ::strncmp(date.c_str(), nullvalue(0), ::strlen(nullvalue(0))) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is NULL_TIME or failure
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTime::isNullTime () const
{
	if ( TU_HOUR > TU_END(m_Stamp->m_Qual) )	// not in range hour to msec
		return true;

	VarString time = getDateTime(dt_format_time());

	return ( 0 == ::strncmp(time.c_str(), nullvalue(1), ::strlen(nullvalue(1))) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	
    date and time representation for current locale, uses  toStrDate() and toStrTime
    \throw no-throw 
	\return date and time representation for current locale
	\retval empty string if date  is invalid 
	\note   provides only last 2 digits of year in some locals */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString DateTime::toStrDateTime		() const 
{
    VarString value = toStrDate();
    value.append(" ");
    value.append(toStrTime());
    return value;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	
     reads date and time representation for current locale, uses  toStrDate() and toStrTime()
     expects one character between date and time.
    \throw no-throw 
	\return date and time representation for current locale
	\retval empty string if date  is invalid 
	\note   provides only last 2 digits of year in some locales */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTime::fromStrDateTime	(const I18nString & value)
{
    VarString::size_type dateLength = getStrDateLength();
    VarString::size_type length     = value.length();

    if (dateLength < length)
    {
        VarString datevalue = value.substr(0,dateLength);
        fromStrDate(datevalue);
        fromStrTime(value.substr(dateLength+1,length));
    }
    else  
    {
        // if length < dateLength, 
        // there will be a better exception with more 
        // information in fromStrDate
        fromStrDate(value);
        setTime(0);
    }
}

inline DateTime::ResultOfCompairison DateTime::internalCompairSizeWithThis(const DateTime& rHs) const
{
    //shortcut
    if (this == &rHs)
    {
        return EQUAL;
    }

	//todo: check whether or not smaller m_Qual is (at least partly) contained in larger one
	//1) find larger one (-> lager m_Qual)
	//2) compaire tu_start & tu_end of smaller one with the ones of larger one
	//3) if not contained, throw

    //initialize pointers in case both m_Qual are the same, otherwise we'd compare null pointers
    const DateTime* cmpSmaller = &rHs;

    const DateTime* cmpLarger = this;

	int start_rhs = 0;
	int end_rhs = 0;
	int start_this = 0;
	int end_this = 0;


	if( m_Stamp->m_Qual != rHs.m_Stamp->m_Qual )
    {
		start_rhs = TU_START(rHs.m_Stamp->m_Qual);
		end_rhs = TU_END(rHs.m_Stamp->m_Qual);
		start_this = TU_START(m_Stamp->m_Qual);
		end_this = TU_END(m_Stamp->m_Qual);

        if (m_Stamp->m_Qual > rHs.m_Stamp->m_Qual)
		{
			if (! (
					(start_rhs >= start_this) && (end_rhs <= end_this)
				)
				)
			{
				ExceptInfo sInfo("basar.cmnutil.DateTime.internalCompairSizeWithThis", "", __FILE__, __LINE__);
				throw IncompatibleRangeException(sInfo);
			}
		}
		else
		{
			if (! (
					(start_this >= start_rhs) && (end_this <= end_rhs)
				)
				)
			{
				ExceptInfo sInfo("basar.cmnutil.DateTime.internalCompairSizeWithThis", "", __FILE__, __LINE__);
				throw IncompatibleRangeException(sInfo);
			}
            cmpSmaller = this;

            cmpLarger = &rHs;

		}
    }

	//expand (=shrink) larger one and exchange pointers if necessary
	DateTime* ptmp = 0;

	const DateTime* otherTmp = 0;

	DateTime tmp(*cmpLarger);

	ptmp = &tmp;

	if (m_Stamp->m_Qual != rHs.m_Stamp->m_Qual)
	{
		ptmp->m_Stamp->m_Qual = cmpSmaller->m_Stamp->m_Qual;

		::dtextend(reinterpret_cast<dtime_t*>(cmpLarger->m_Stamp), reinterpret_cast<dtime_t*>(ptmp->m_Stamp));     

		if (m_Stamp->m_Qual < rHs.m_Stamp->m_Qual)
		{
			otherTmp = ptmp;

			ptmp = const_cast<DateTime*>(cmpSmaller);

			cmpSmaller = otherTmp;
		}
	}
	else
	{
		ptmp = const_cast<DateTime*>(cmpLarger);
	}
    
    return static_cast<ResultOfCompairison>(::deccmp(reinterpret_cast<dec_t*>(&ptmp->m_Stamp->m_Dec), reinterpret_cast<dec_t*>(&cmpSmaller->m_Stamp->m_Dec)));
}

bool DateTime::isDST() const
{
    if ( !isValid() )
    {
        return false;
    }

    time_t rawtime;

    rawtime = ::time(NULL);

    struct tm timeinfo = *localtime( &rawtime );

    timeinfo.tm_year =  this->getYear() - 1900;
    timeinfo.tm_mon = this->getMonth() - 1;
    timeinfo.tm_mday = this->getDay();

    timeinfo.tm_hour = this->getHour();
    timeinfo.tm_min = this->getMinute();
    timeinfo.tm_sec = this->getSec();

    timeinfo.tm_isdst = -1;

    mktime ( &timeinfo );

    return (timeinfo.tm_isdst == 1);

}

inline void DateTime::internalAddTimeUnitAmount( const basar::Int16 unit, const basar::Int32 amount, ConstString formatString )
{
	// --- calc interval
	Int32 absAmount = ::abs(amount);
    char  tm[16] = {0};
	Int16 len     = static_cast<Int16>(::sprintf(tm, "%d", absAmount));

	intrvl_t inv;
    inv.in_qual = TU_IENCODE(len, unit, unit);

    int ret = ::incvfmtasc(tm, const_cast<char *>(formatString), &inv);

	if (0 > ret)			// convert to interval
		return;

	// --- calc datetime

	dtime_t * pDt = reinterpret_cast<dtime_t *>(m_Stamp);
	dtime_t   result;

	// datetime value must include all the fields present in interval value
	if ( 0 > ( (amount >= 0) ? ::dtaddinv(pDt, &inv, &result) 
		                   : ::dtsubinv(pDt, &inv, &result) ) )
		return;
	
	::dtextend(&result, pDt);	// set result to member regarding datetime qualifier
}

//---------------------------------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\brief standard ostream operator
	\param stream - outstream
	\param dt - datetime to output
	\return reference to std::ostream
	\throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& stream, const DateTime& dt)
{
	return (stream << dt.toStrDateTime());
}

//! \return new DateTime instance containing result of addition
const DateTime operator+(const DateTime& left, const TimeSpan& right)
{
    DateTime res(left);

    res += right;

    return res;
}

//! \return new DateTime instance containing result of addition
const DateTime operator+(const TimeSpan & left, const DateTime & right)
{
    DateTime res(right);

    res += left;

    return res;
}

//! \return new DateTime instance containing result of substraction
const DateTime operator-(const DateTime& left, const TimeSpan& right)
{
    DateTime res(left);

    res -= right;

    return res;
}

//---------------------------------------------------------------------------------------------------------------------//

}//cmnutil
}//basar

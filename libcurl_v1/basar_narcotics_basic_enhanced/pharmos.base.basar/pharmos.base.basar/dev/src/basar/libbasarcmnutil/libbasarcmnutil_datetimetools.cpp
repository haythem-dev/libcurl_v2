//---------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  datetime tools implementation 
 *  \author Michael Eichenlaub
 *  \date   29.03.2007
 */
//---------------------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_datetimetools.h"
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_timespan.h"

#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

//---------------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return date in integer format (YYYYMMDD) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getDate (const Int32 year, const Int32 month, const Int32 day)
{
	return ( year  * 10000  +
			 month *   100  + 
			 day              );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
	\return time in integer format (HHMMSSTTT) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getTime (const Int32 hour, const Int32 min, const Int32 sec, const Int32 msec /* = 0 */)
{
	return ( hour * 10000000  +
			 min  *   100000  +
			 sec  *     1000  +
			 msec               );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return year component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getYear( const Int32 date )
{
	return static_cast<Int16>(date / 10000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return month component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getMonth( const Int32 date )
{
	return static_cast<Int16>((date / 100) % 100);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return day component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getDay( const Int32 date )
{
	return static_cast<Int16>(date % 100);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return hour component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getHour( const Int32 time	)
{
	return static_cast<Int16>(time / 10000000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return minute component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getMinute( const Int32 time )
{
	return static_cast<Int16>((time / 100000) % 100);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return second component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getSec( const Int32 time )
{
	return static_cast<Int16>((time / 1000) % 100);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return millisecond component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int16 DateTimeTools::getMSec( const Int32 time )
{
	return static_cast<Int16>(time % 1000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return total number of milliseconds after midnight */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getTotalMSec(const Int32 time)
{
	return (getHour  (time) * 60 * 60 * 1000 +
		    getMinute(time)      * 60 * 1000 +
			getSec   (time)           * 1000 +
			getMSec  (time)                    );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return integer formatted time */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getTimeByTotalMSec(const Int32 msec)
{
	return getTime(  msec / (60 * 60 * 1000)        ,		// hour
		            (msec / (     60 * 1000)) %   60,		// minute
					(msec /            1000 ) %   60,		// secs
					 msec	                  % 1000);		// milliseconds
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const basar::DateTime DateTimeTools::getDateTimeFromUnixTimestamp( basar::Int32 unixTimestamp )
{
    const basar::DateTime& localEpoch = getLocalTimeFromUTC( getEpoch() );

    basar::TimeSpan secondsSinceEpoch;
    secondsSinceEpoch.fromNumeric( unixTimestamp, TimeSpan::TSUNIT_SEC );

    return (localEpoch + secondsSinceEpoch);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
basar::Int32 DateTimeTools::getUnixTimestampFromDateTime( const basar::DateTime& d )
{
    const basar::DateTime& epoch = getEpoch();

    basar::DateTime utc = getUTCFromLocalTime( d );

    basar::TimeSpan secondsSinceEpoch = utc - epoch;

    TimeSpan::Numeric n = secondsSinceEpoch.toNumeric();

    basar::Int32 ret = n.seconds;

    ret += TimeSpan::SECONDS_PER_MINUTE * n.minutes;

    ret += TimeSpan::SECONDS_PER_HOUR * n.hours;

    ret += TimeSpan::SECONDS_PER_DAY * n.days;

    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is valid 
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTimeTools::isValidDate (const Int32 year, const Int32 month, const Int32 day)
{
	if ((year <= 0)	|| (year > 9999))
		return false;

	if ((month > 12) || (month < 1))
		return false;

	if ((day < 1) || (day > 31))		
		return false;

	if ((day > 30) &&					
		(month == 4 || month == 6 || month == 9 || month == 11))	
		return false;

	if (month == 2)   // February 
	{
		if (day > 29)
			return false;

		if (day == 29)
		{
			if (! Date::isLeapYear(year) )
				return false;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is valid 
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTimeTools::isValidDate (const Int32 date)
{
	return isValidDate(getYear(date), getMonth(date), getDay(date));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is valid 
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTimeTools::isValidTime (const Int32 hour,	const Int32 min, const Int32 sec, const Int32 msec /* = 0 */)
{
	return (static_cast<UInt32>(hour) <   24) && 
		   (static_cast<UInt32>(min ) <   60) && 
		   (static_cast<UInt32>(sec ) <   60) && 
		   (static_cast<UInt32>(msec) < 1000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\retval true if date is valid 
	\retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DateTimeTools::isValidTime (const Int32 time)
{
	return isValidTime(getHour(time), getMinute(time), getSec(time), getMSec(time));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\return old style integer formatted time (hhmmss) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getOldTime(const Int32 newtime)
{
	return newtime / 1000;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw 
	\return new style integer formatted time (hhmmssttt) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 DateTimeTools::getNewTime(const Int32 oldtime)
{
	return oldtime * 1000;
}

const basar::DateTime& DateTimeTools::getEpoch()
{
    static const basar::DateTime epoch(19700101, 0);

    return epoch;
}

inline DateTime DateTimeTools::getUTCFromLocalTime(const basar::DateTime& d)
{
    using namespace boost::posix_time;

    // boost::date_time::c_local_adjustor uses the C-API to adjust a
    // moment given in utc to the same moment in the local time zone.
    typedef boost::date_time::c_local_adjustor<ptime> local_adj;

    const ptime utc_now(boost::gregorian::date(d.getYear(), d.getMonth(), d.getDay()), boost::posix_time::time_duration(d.getHour(), d.getMinute(), d.getSec()));
    const ptime local_now = local_adj::utc_to_local(utc_now);

    const time_duration diff = utc_now - local_now;

    return d - TimeSpan(Int32(diff.total_seconds()), TimeSpan::TSUNIT_SEC);
}

inline DateTime DateTimeTools::getLocalTimeFromUTC(const basar::DateTime& d)
{
    using namespace boost::posix_time;

    // boost::date_time::c_local_adjustor uses the C-API to adjust a
    // moment given in utc to the same moment in the local time zone.
    typedef boost::date_time::c_local_adjustor<ptime> local_adj;

    const ptime utc_now(boost::gregorian::date(d.getYear(), d.getMonth(), d.getDay()), boost::posix_time::time_duration(d.getHour(), d.getMinute(), d.getSec()));
    const ptime local_now = local_adj::utc_to_local(utc_now);

    const time_duration diff = local_now - utc_now;

    return d + TimeSpan(Int32(diff.total_seconds()), TimeSpan::TSUNIT_SEC);
}

//---------------------------------------------------------------------------------------------------------------------//
}//cmnutil
}//basar

#ifndef GUARD_LIBBASARCMNUTIL_TIMESTAMP_H
#define GUARD_LIBBASARCMNUTIL_TIMESTAMP_H

#include "libbasarcmnutil_datetypes_common.h"
#include "libbasarcmnutil_datetimetools.h"

#include <sqlhdr.h>
#include <cstdio>

//---------------------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil{

//---------------------------------------------------------------------------------------------------------------------//
/*! \brief  get date format string
            (functions avoids static init order fiasco)
    \return internally used Infx CSDK format control string for date */
static inline ConstBuffer dt_format_date()
{
	return "%04Y%02m%02d";	
}


/*! \brief  get timespan format string
            (functions avoids static init order fiasco)
    \return internally used Infx CSDK format control string for date */
static inline ConstBuffer ts_format_short()
{
    return "%d %02H:%02M:%02S%03F3";	
}

/*! \brief  get time format string
            (functions avoids static init order fiasco)
    \return internally used Infx CSDK format control string for time */
static inline ConstBuffer dt_format_time()
{
	return "%02H%02M%02S.%03F3";
}

/*! \brief  get date format string (C runtime)
            (functions avoids static init order fiasco)
    \return CRT format control string for date */
static inline ConstBuffer crt_format_date()
{
	return "%04d%02d%02d";
}

/*! \brief  get time format string (C runtime)
            (functions avoids static init order fiasco)
    \return CRT format control string for time */
static ConstBuffer crt_format_time()
{
	return "%02d%02d%02d.%03d";
}

//! internal NULL values for date and time
typedef ConstString NullValues[2];


/*! \brief calculate internal NULL date and time (Informix)
	\return internal NULL date and time value */
static const NullValues & calcNullDateTime()
{
	static char       date[16];
	static char       time[16];
	static NullValues ret = { date, time };

	sprintf(date, "%d", NULLVAL_SQL_DATE); 
	sprintf(time, crt_format_time(), 
	        DateTimeTools::getHour  (NULLVAL_SQL_TIME),
			DateTimeTools::getMinute(NULLVAL_SQL_TIME),
			DateTimeTools::getSec   (NULLVAL_SQL_TIME),
			DateTimeTools::getMSec  (NULLVAL_SQL_TIME)); 

	return ret;
}

/*! \brief  store NULL date and time (calculated once)
	\return internal NULL date and time value */
static inline ConstBuffer nullvalue(Int32 idx	//!< 0: date, 1: time
							)
{
	static const NullValues & NULLVALUE = calcNullDateTime();	
	
	return NULLVALUE[idx];
}

/*! \brief  enum for indicating how timestamps's internal buffer should be initialized */
enum populateInitialValuesFor
{
    DATETIME,
    TIMESPAN_DAYS
};


//! length of character array digits
#define DEC_DIGITS		16	

//! date/time storage. order of fields is necessary for collaboration with Informix CSDK type dtime_t (must be in sync)
class Timestamp
{
    friend class DateTime;
    friend class TimeSpan;
private:
	//! c'tor	\n DateTimeException
	Timestamp		(populateInitialValuesFor what //<! requested initialization
        );

    //! c'tor, no implementation
    Timestamp();

	Int16						m_Qual;			//!< qualifier
    ::decimal					m_Dec;			//!< digits of fields
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

#endif //GUARD
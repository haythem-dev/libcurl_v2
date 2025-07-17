#ifndef GUARD_LIBBASARCMNUTIL_DATETIMETOOLS_H
#define GUARD_LIBBASARCMNUTIL_DATETIMETOOLS_H



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

//----------------------------------------------------------------------------
// include section
//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {

//--------------------------------------------------------------------------------------------------//
//! \brief date/time tools: useful static methods for handling integer formatted date/time etc.
//! \n final class
//--------------------------------------------------------------------------------------------------//
class LIBBASARCMNUTIL_API DateTimeTools
{
public:

	//! \brief get integer formatted date (YYYYMMDD) from given parameters	\n no-throw
	static Int32	getDate				( const Int32 year,		//!< year		   0 - 9999
										  const Int32 month,	//!< month         1 -   12 -> month of year
										  const Int32 day		//!< day           1 -   31 -> day of month
										);
	//! \brief get integer formatted time (hhmmssttt) from given parameters	\n no-throw
	static Int32	getTime				( const Int32 hour,		//!< hour          0 -  23 -> hour since midnight
										  const Int32 min,		//!< minutes       0 -  59 -> minutes after hour
										  const Int32 sec,		//!< seconds       0 -  59 -> seconds after minute
										  const Int32 msec = 0	//!< milli seconds 0 - 999 -> milli seconds after second
										);

	//! \brief get year component from integer formatted date	\n no-throw
	static Int16	getYear				( const Int32 date		//!< integer formatted date (YYYYMMDD)
										);
	//! \brief get month component from integer formatted date	\n no-throw
	static Int16	getMonth			( const Int32 date		//!< integer formatted date (YYYYMMDD)
										);
	//! \brief get day component from integer formatted date	\n no-throw
	static Int16	getDay				( const Int32 date		//!< integer formatted date (YYYYMMDD)
										);
	//! \brief get hour component from integer formatted time	\n no-throw
	static Int16	getHour				( const Int32 time		//!< integer formatted time (hhmmssttt)
										);
	//! \brief get minute component from integer formatted time	\n no-throw
	static Int16	getMinute			( const Int32 time		//!< integer formatted time (hhmmssttt)
										);
	//! \brief get second component from integer formatted time	\n no-throw
	static Int16	getSec				( const Int32 time		//!< integer formatted time (hhmmssttt)
										);
	//! \brief get millisecond component from integer formatted time	\n no-throw
	static Int16	getMSec				( const Int32 time		//!< integer formatted time (hhmmssttt)
										);

	//! \brief get total number of milliseconds after midnight from given integer formatted time		\n no-throw
	static Int32	getTotalMSec		( const Int32 time		//!< integer formatted time (hhmmssttt)
										);
	//! \brief get integer formatted time (hhmmssttt) from total number of milliseconds after midnight	\n no-throw
	static Int32	getTimeByTotalMSec	( const Int32 msec		//!< total number of milliseconds after midnight
										);

    //! \brief create DateTime from Unix Timestamp
    //! \param unixTimestamp - basar::Int32 representing a Unix Timestamp
    //! \return basar::DateTime set with the date/time indicated by the Timestamp
    static const basar::DateTime getDateTimeFromUnixTimestamp( basar::Int32 unixTimestamp );

    //! \brief create Unix Timestamp from DateTime
    //! \param d - basar::DateTime representing a datetime to convert
    //! \return basar::Int32 indicating the seconds since Epoch
    static basar::Int32 getUnixTimestampFromDateTime( const basar::DateTime& d );

	//--------------------------------------------------------------------------------------------------//

	//! \brief is given date valid?			\n no-throw
	static bool	isValidDate			( const Int32 year,		//!< year		   0 - 9999
									  const Int32 month,	//!< month         1 -   12 -> month of year
									  const Int32 day		//!< day           1 -   31 -> day of month
									);
	//! \brief is given date valid?			\n no-throw
	static bool	isValidDate			( const Int32 date		//!< integer formatted date (YYYYMMDD)
									);
	//! \brief is given time valid?			\n no-throw
	static bool	isValidTime			( const Int32 hour,		//!< hour          0 -  23 -> hour since midnight
									  const Int32 min,		//!< minutes       0 -  59 -> minutes after hour
									  const Int32 sec,		//!< seconds       0 -  59 -> seconds after minute
									  const Int32 msec = 0	//!< milli seconds 0 - 999 -> milli seconds after second
											);
	//! \brief is given time valid?			\n no-throw
	static bool	isValidTime			( const Int32 time		//!< integer formatted time (hhmmssttt)
									);

	//--------------------------------------------------------------------------------------------------//
	// methods for old style integer formatted time
	//--------------------------------------------------------------------------------------------------//

	//! \brief get old style integer formatted time  (hhmmss)		\n no-throw
	static Int32	getOldTime			(const Int32 newtime	//!< integer formatted time (hhmmssttt)
										);

	//! \brief get new style integer formatted time  (hhmmssttt)	\n no-throw
	static Int32	getNewTime			(const Int32 oldtime	//!< integer formatted time (hhmmss)
										);

private:

    static const basar::DateTime& getEpoch();

    //! get a DateTime in UTC from this
    static inline DateTime    getUTCFromLocalTime( const basar::DateTime& d);

    //! get a DateTime in LocalDateTime from this
    static inline DateTime    getLocalTimeFromUTC( const basar::DateTime& d);

};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

#endif //DATETIMETOOLS_H

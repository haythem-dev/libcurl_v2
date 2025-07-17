//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar data type for informix interval
 *  \author Marco Köppendörfer
 *  \date   03.05.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBBASARCMNUTIL_TIMESPAN_H
#define GUARD_LIBBASARCMNUTIL_TIMESPAN_H


//------------------------------------------------------------------------------------------------------//
// dll import and export macros
//------------------------------------------------------------------------------------------------------//

#ifndef LIBBASARCMNUTIL_API
#ifdef _WIN32
#ifdef LIBBASARCMNUTIL_EXPORTS
#define LIBBASARCMNUTIL_API __declspec(dllexport)   //!< dll exported
#else
#define LIBBASARCMNUTIL_API __declspec(dllimport)   //!< dll imported
#endif
#else
#define LIBBASARCMNUTIL_API
#endif
#endif

//forward declaration of informix datetime structure for pointer, defined in informix CSDK's datetime.h
struct intrvl;

//------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_decimal.h"

#include <iosfwd>


//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace cmnutil
    {

        //forward declaration
        class Timestamp;


        //------------------------------------------------------------------------------------------------------//
        //! \brief TimeSpan : representing a time interval, based on Informix's INTERVAL (ranging form Days to MSecs, max: 999999999 23:59:59.998 - about 2.7 milion years)
        //! \n final class
        //! \n throws DateTimeException in case of failure
        //------------------------------------------------------------------------------------------------------//
        class TimeSpan
        {
            public:

                //--------------------------------------------------------------------------------------------------//
                //  enums
                //--------------------------------------------------------------------------------------------------//
                //! units for specifying fractions of seconds
                enum TimeSpanFractionEnum
                {
                    TSFRACTION_DSEC = 6,        //!< 10th second (deci)
                    TSFRACTION_CSEC,        //!< 100th second (centi)
                    TSFRACTION_MSEC,        //!< 1000th second (milli)
                    TSFRACTION_100uSEC,     //!< 10000th second (100 µ)
                    TSFRACTION_10uSEC       //!< 100000th second (10 µ)
                };

                //! datetime units for specifying value range
                enum TimeSpanUnitEnum
                {
                    TSUNIT_DAY = 2,     //!< day
                    TSUNIT_HOUR,        //!< hour
                    TSUNIT_MIN,         //!< minute
                    TSUNIT_SEC,         //!< second
                    TSUNIT_DSEC = TSFRACTION_DSEC,        //!< 10th second (deci)
                    TSUNIT_CSEC = TSFRACTION_CSEC,        //!< 100th second (centi)
                    TSUNIT_MSEC = TSFRACTION_MSEC,        //!< 1000th second (milli)
                    TSUNIT_100uSEC = TSFRACTION_100uSEC,     //!< 10000th second (100 µ)
                    TSUNIT_10uSEC = TSFRACTION_10uSEC       //!< 100000th second (10 µ)
                };

                //--------------------------------------------------------------------------------------------------//
                //  definitions
                //--------------------------------------------------------------------------------------------------//
                //! datatype for numerical representation of a TimeSpan object
                struct Numeric
                {
                    Numeric() : days(0), hours(0), minutes(0), seconds(0), fraction(0), isNegative(false)
                    {}

                    //! returns the total seconds (days & hours & minutes converted to seconds) excluding the fractional seconds.
                    basar::Int64 getTotalSeconds() const
                    {
                        return (isNegative ? -1 : 1) * (
                              seconds
                            + minutes * SECONDS_PER_MINUTE
                            + hours * SECONDS_PER_HOUR
                            + days * SECONDS_PER_DAY
                        );
                    }

                    basar::Int32 days;
                    basar::Int32 hours;
                    basar::Int32 minutes;
                    basar::Int32 seconds;
                    basar::Int32 fraction;
                    bool isNegative;
                };

                //--------------------------------------------------------------------------------------------------//
                //  friends
                //--------------------------------------------------------------------------------------------------//

                friend class DateTime;

                //--------------------------------------------------------------------------------------------------//
                // static section
                //--------------------------------------------------------------------------------------------------//

                //! \brief get size for informix custom timespan description    \n no-throw
                LIBBASARCMNUTIL_API static int          getInfxDescSize ();

                LIBBASARCMNUTIL_API static const basar::Int32               SECONDS_PER_MINUTE  = 60;       //!< number of seconds per minute
                LIBBASARCMNUTIL_API static const basar::Int32               MINUTES_PER_HOUR    = 60;       //!< number of mnutes per hour
                LIBBASARCMNUTIL_API static const basar::Int32               HOURS_PER_DAY       = 24;       //!< number of hours per day

                //derived values
                LIBBASARCMNUTIL_API static const basar::Int32               MINUTES_PER_DAY     = MINUTES_PER_HOUR * HOURS_PER_DAY;         //!< number of minutes per day
                LIBBASARCMNUTIL_API static const basar::Int32               SECONDS_PER_HOUR    = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;    //!< number of seconds per hour
                LIBBASARCMNUTIL_API static const basar::Int32               SECONDS_PER_DAY     = HOURS_PER_DAY * SECONDS_PER_HOUR;         //!< number of seconds per day

                //--------------------------------------------------------------------------------------------------//
                // c'tor / d'tor
                //--------------------------------------------------------------------------------------------------//

                //! \brief construct an empty timespan                      \n no-throw
                LIBBASARCMNUTIL_API                     TimeSpan        ();

                //! \brief construct from normalized string ("%d %H:%M:%S%F3")                  \n no-throw
                LIBBASARCMNUTIL_API                     TimeSpan        ( const I18nString& normalizedString );

                //! \brief copy-c'tor                                       \n no-throw
                LIBBASARCMNUTIL_API                     TimeSpan        ( const TimeSpan& source );

                //! \brief d'tor                        \n no-throw
                LIBBASARCMNUTIL_API                     ~TimeSpan       ();

                //! \brief construct from informix interval buffer, note: no copying done here  \n no-throw */
                LIBBASARCMNUTIL_API explicit            TimeSpan        ( const intrvl* pTime //!< pointer to informix interval buffer
                                                                        );

                //! \brief construct TimeSpan from an interval given by value of time units
                LIBBASARCMNUTIL_API                     TimeSpan        ( basar::Int32 value, TimeSpanUnitEnum unit );

                //--------------------------------------------------------------------------------------------------//
                // operators
                //--------------------------------------------------------------------------------------------------//

                //! \brief assignment operator, needed b/c of const member
                //! \param rHs - passed object to assign from
                //! \return timespan representing the result of the assignment
                LIBBASARCMNUTIL_API TimeSpan&           operator=( const TimeSpan& rHs );

                //! \brief compound add and assign TimeSpan, works results of 1824635 days (about 4999 years)
                //! \return timespan representing the result of the addition
                //! \param rHs - TimeSpan which is added
                LIBBASARCMNUTIL_API TimeSpan& operator+=( const TimeSpan& rHs );

                //! \brief compound subtract and assign TimeSpan, works results of 1824635 days (about 4999 years)
                //! \return timespan representing the result of the subtraction
                //! \param rHs - TimeSpan which is subtracted
                LIBBASARCMNUTIL_API TimeSpan& operator-=( const TimeSpan& rHs );

                //! \brief divide by numeric value, result in TimeSpan
                //! \return timespan representing the result of the division
                //! \param divisor - numeric value to divide by
                LIBBASARCMNUTIL_API TimeSpan&           operator/=( const Float64 divisor );

                //! \brief divide by decimal (Float64 internally), result in TimeSpan
                //! \return timepsan representing the result of the division
                //! \param divisor - decimal value to divide by
                LIBBASARCMNUTIL_API TimeSpan&           operator/=( const Decimal& divisor );

                //! \brief divide by TimeSpan, resulting in fraction
                //! \return Float64 expressing the ratio of the two timespans
                //! \param divisor - timespan to divide by
                LIBBASARCMNUTIL_API Float64             operator/( const TimeSpan& divisor ) const;

                //! \brief multiplicate by numeric value, result in TimeSpan
                //! \return timepsan representing the result of the multiplication
                //! \param mult - numeric value to multiplicate by
                LIBBASARCMNUTIL_API TimeSpan&           operator*=( const Float64 mult );

                //! \brief multiplicate by decimal (Float64 internally), result in TimeSpan
                //! \return timepsan representing the result of the multiplication
                //! \param mult - decimal value to multiplicate by
                LIBBASARCMNUTIL_API TimeSpan&           operator*=( const Decimal& mult );

                //! \brief check equality
                //! \param rHs - TimeSpan to check equality with
                //! \return bool indicating equality
                LIBBASARCMNUTIL_API bool                operator==( const TimeSpan& rHs ) const;

                //! \brief check inequality
                //! \param rHs - TimeSpan to check inequality with
                //! \return bool indicating inequality
                LIBBASARCMNUTIL_API bool                operator!=( const TimeSpan& rHs ) const;

                //! \brief compare less than
                //! \param rHs - TimeSpan to check less than
                //! \return bool indicating less than
                LIBBASARCMNUTIL_API bool                operator<( const TimeSpan& rHs ) const;

                //! \brief compare greater than
                //! \param rHs - TimeSpan to check greater than
                //! \return bool indicating greater than
                LIBBASARCMNUTIL_API bool                operator>( const TimeSpan& rHs ) const;

                //--------------------------------------------------------------------------------------------------//
                // object methods
                //--------------------------------------------------------------------------------------------------//
                //! \brief get pointer to interval buffer   \n no-throw
                LIBBASARCMNUTIL_API const intrvl*       getTimestampBuffer  () const;

                //--------------------------------------------------------------------------------------------------//

                //!\brief converting to a string representation for current locale, nomalized to "days hours:minutes:seconds.fraction"  \n no-throw
                LIBBASARCMNUTIL_API const I18nString    toNormalizedString  () const;

                //!\brief converting to a string representation for current locale, displayed as internally represented by range, eg. 72 hours are displayed as 72 hours not as 3 days  \n no-throw
                LIBBASARCMNUTIL_API const I18nString    toString() const;

                //!\brief converting to a string representation for current locale, displayed as given by format, see Informix SQL-Reference DBTIME\n no-throw
                LIBBASARCMNUTIL_API const I18nString    toFormattedString( ConstString format ) const;

                //! \brief converting from string using format-control string                       \n throw DateTimeException
                LIBBASARCMNUTIL_API void                fromString      ( ConstString        format,    //!< format control string, see Informix SQL-Reference DBTIME
                        const I18nString& rTimeSpan    //!< date string in representation same as from toStrDate(); depends on locale
                                                                        );

                //! \brief converting from string using format-control string %d %02H:%02M:%02S%03F3                    \n throw DateTimeException
                LIBBASARCMNUTIL_API void                fromNormalizedString        ( const I18nString& rTimeSpan   //!< date string in representation same as from toStrDate(); depends on locale
                                                                                    );

                //! \brief create a TimeSpan from an amount of a certain unit of time units ( e.g. days, hours, seconds) \n throw DateTimeException, NotImplementedException
                LIBBASARCMNUTIL_API void                fromNumeric( basar::Int32 value, TimeSpanUnitEnum unit );

                //! \brief create a TimeSpan from a TimeSpan::Numeric struct            \n throw DateTimeException
                LIBBASARCMNUTIL_API void                fromNumeric( const TimeSpan::Numeric& numeric, TimeSpanFractionEnum unit = TSFRACTION_MSEC );

                //! \brief return a normalized TimeSpan::Numeric struct representing the TimeSpan           \n no-throw
                LIBBASARCMNUTIL_API TimeSpan::Numeric   toNumeric() const;
                //--------------------------------------------------------------------------------------------------//

                //! \brief set internal range units (year, ..., msec)       \n no-throw
                LIBBASARCMNUTIL_API void                setRange ( TimeSpanUnitEnum from,       //!< biggest datetime unit
                        TimeSpanUnitEnum to       //!< smallest datetime unit
                                                                 );

                //! \brief set to invalid value     \n DateTimeException
                LIBBASARCMNUTIL_API void                setInvalid ();

                //--------------------------------------------------------------------------------------------------//

                //! \brief check if TimeSpan is valid (= not NULL TimeSpan)     \n DateTimeException
                LIBBASARCMNUTIL_API bool                isValid ();


                //! \brief check if TimeSpan is null (== NULL TimeSpan)         \n DateTimeException
                LIBBASARCMNUTIL_API bool                isNull ();


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

                //! enum representing errorcode of Informix interval functions
                enum ErrorEnum
                {
                    INERROR_SUCCESS             =     0,    //!< success

                    INERROR_INCOMPATIBLEVALUE   = -1266,    //!< value is incompatible with operation
                    INERROR_INVALIDQUALIFIER    = -1268     //!< interval contains invalid qualifier

                };

                //--------------------------------------------------------------------------------------------------//
                // private constants
                //--------------------------------------------------------------------------------------------------//
                static const Int32 dummy_date = 49990101; //!< base date used for interval additions / subtractions

                //--------------------------------------------------------------------------------------------------//
                // private functions
                //--------------------------------------------------------------------------------------------------//

                //! \brief checks if Timestamp is owned by instance, throws if otherwise \n throw DateTimeException
                inline void checkIfTimestampIsOwned() const;

                //! \brief divide by numberic data type     \n throws DateTimeException
                inline  void internalDivideByNumeric( Float64 divisor, Timestamp* result );

                //! \brief multiplicate by numberic data type       \n throws DateTimeException
                inline  void internalMultiplicateByNumeric( Float64 mult, Timestamp* result );

                //! \brief compare with other timespan      \n throws DateTimeException
                inline  ResultOfCompairison internalCompairSizeWithThis( const TimeSpan& rHs ) const;

                //! \brief get format string for time span range        \n no-throw
                inline  ConstBuffer getRangedFormat( int tu_start ) const;

                //! \brief create TimeSpan from an amount of seconds        \n throw DateTimeException
                inline void internalFromNumericSeconds( basar::Int32 seconds );

                //! \brief create TimeSpan from an amount of days           \n throw DateTimeException
                inline void internalFromNumericDays( basar::Int32 days );

                //! \brief create TimeSpan from an amount of minutes        \n throw DateTimeException
                inline void internalFromNumericMinutes( basar::Int32 minutes );

                //! \brief create TimeSpan from an amount of hours          \n throw DateTimeException
                inline void internalFromNumericHours( basar::Int32 hours );

                inline TimeSpan::Numeric fixNumeric( const TimeSpan::Numeric& n, TimeSpanFractionEnum unit ) const;

                inline basar::Int32 getFractionPerSecond( TimeSpanFractionEnum unit ) const;

                inline basar::Int32 getFractionLength( TimeSpanFractionEnum unit ) const;

                inline bool isNegative() const;


                //--------------------------------------------------------------------------------------------------//
                // members
                //--------------------------------------------------------------------------------------------------//

                const bool m_ownsTimestamp; //!< indicating instance is owner of it's data buffer

                Timestamp* m_Stamp; //!< holds pointer to data buffer

        };

//no friend necessary - operators use public functions
        LIBBASARCMNUTIL_API std::ostream& operator<< ( std::ostream& stream, const TimeSpan& ts );

        LIBBASARCMNUTIL_API bool operator<=( const TimeSpan& lhs, const TimeSpan& rhs );

        LIBBASARCMNUTIL_API bool operator>=( const TimeSpan& lhs, const TimeSpan& rhs );

        LIBBASARCMNUTIL_API const TimeSpan operator+( TimeSpan lhs, const TimeSpan& rhs );

        LIBBASARCMNUTIL_API const TimeSpan operator-( TimeSpan lhs, const TimeSpan& rhs );

        LIBBASARCMNUTIL_API const TimeSpan operator* ( basar::Float64 mult, TimeSpan ts );

        LIBBASARCMNUTIL_API const TimeSpan operator* ( basar::Int32 mult, TimeSpan ts );

        LIBBASARCMNUTIL_API const TimeSpan operator* ( TimeSpan ts, basar::Float64 mult );

        LIBBASARCMNUTIL_API const TimeSpan operator* ( TimeSpan ts, basar::Int32 mult );

        LIBBASARCMNUTIL_API const TimeSpan operator/ ( TimeSpan ts, basar::Float64 div );

        LIBBASARCMNUTIL_API const TimeSpan operator/ ( TimeSpan ts, basar::Int32 div );

        LIBBASARCMNUTIL_API const TimeSpan operator/ ( TimeSpan ts, const basar::Decimal& div );

//----------------------------------------------------------------------------
    }   // namespace cmnutil
}   // namespace basar



#endif //GUARD_LIBBASARCMNUTIL_TIMEINTERVAL_H
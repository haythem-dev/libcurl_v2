//---------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  date implementation
 *  \author Michael Eichenlaub
 *  \date   29.03.2007
 */
//---------------------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_datetimetools.h"
#include "libbasarcmnutil_datetime.h"

#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_locale.h"
#include "libbasardebug.h"

#include <assert.h>
#include <string.h>

#include <sqlhdr.h>

#include <ostream>

//---------------------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace cmnutil
    {

//----------------------------------------------------------------------------
        void dumpDate( size_t addr )
        {
            if ( 0 == addr )
            {
                return;
            }

            const Date* const pDate = reinterpret_cast<const Date* const>( addr );
            VarString          out;

            out.format( "#0x%0*lx: %s\n",
                        sizeof( size_t ) * 2, pDate,
                        pDate->toString( "yyyy-mm-dd" ).c_str() );

            debug::printDbgMsg( out.c_str() );
        }

//---------------------------------------------------------------------------------------------------------------------//
        /*! \brief calculate internal NULL date (Informix)
            \return internal NULL date value */
        static Long32 calcNullDate()
        {
            int4 null;

            Int16  mdy[3] =
            {
                DateTimeTools::getMonth( NULLVAL_SQL_DATE ),
                DateTimeTools::getDay  ( NULLVAL_SQL_DATE ),
                DateTimeTools::getYear ( NULLVAL_SQL_DATE )
            };

            ::rmdyjul( mdy, &null );

            return Long32(null);
        }

        /*! \brief  store NULL date (calculated once)
            \return internal NULL date value */
        static Long32 nullvalue()
        {
            static const Long32 NULLVALUE = calcNullDate();

            return NULLVALUE;
        }

        /*! \brief  hold days per month values (function avoids static init order fiasco)
            \return days for given month */
        static char days_per_month( Int32 idx   //!< month 0 .. 11
                                  )
        {
            static const char DAYS_PER_MONTH[] =
            {
                31, 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31
            };

            return DAYS_PER_MONTH[idx];
        }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \retval true if leap year
            \retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::isLeapYear( const Int32 year )
        {
            if ( year % 400 == 0 )
            {
                return true;
            }

            if ( year % 100 == 0 )
            {
                return false;
            }

            if ( year %   4 == 0 )
            {
                return true;
            }

            return false;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  DateTimeException
            \return number of days of given month */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int32   Date::getDaysOfMonth ( const Int32 year, const Int32 month )
        {
            if ( ! DateTimeTools::isValidDate( year, month, 1 ) )
            {
                VarString msg;
                msg.format( "year %d, month %d isn't a valid date", year, month );

                ExceptInfo sInfo( "basar.cmnutil.Date.getDaysOfMonth()", msg, __FILE__, __LINE__ );

                throw DateTimeException( sInfo );
            }

            if ( ( 2 == month ) && isLeapYear( year ) )
            {
                return 29;
            }
            else
            {
                return days_per_month( month - 1 );
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \return date of first Spring moon */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date Date::getFstSprMoon( const Int32 year )
        {
            static const Int16 easterDays[] =
            {
                14,  3, 23, 11, 31,
                18,  8, 28, 16,  5,
                25, 13,  2, 22, 10,
                30, 17,  7, 27
            };

            Int16 day   = easterDays[ year % 19 ];
            Int16 month = ( day < 19 ) ? 4 : 3;

            return Date ( year, month, day );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \return date of Easter Sunday */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date Date::getEasterSunday ( const Int32 year )
        {
            Date moon ( getFstSprMoon( year ) );

            // 1st Sunday after spring moon
            moon.addDays( 7 - moon.getDayOfWeek() );

            return moon;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return current localized system date */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date Date::getCurrent()
        {
            return Date();
        }

        int Date::getInfxDescSize()
        {
            static int infxDescSize = 0;

            if ( 0 == infxDescSize )
            {
                infxDescSize = sizeof( Long32 );
            }

            return infxDescSize;
        }

//---------------------------------------------------------------------------------------------------------------------//
// object member section (object methods)
//---------------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date::Date() : m_ownsDate( true ), m_Date( 0 )
        {
            m_Date = new Int32;

            setCurrent();
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date::Date ( const Int32 date ) : m_ownsDate( true ), m_Date( 0 )
        {
            m_Date = new Int32;

            setDate( date );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date::Date ( const Int32 year, const Int32 month, const Int32 day ) : m_ownsDate( true ), m_Date( 0 )
        {
            m_Date = new Int32;

            setDate( year, month, day );
        }

        Date::Date ( const Int32* infxDate ) :
            m_ownsDate( false ),
            m_Date    ( const_cast<Int32*>( infxDate ) )
        {
        }

        Date::Date ( const Date& rDate ) : m_ownsDate( true ), m_Date( 0 )
        {
            m_Date = new Int32;

            memcpy( m_Date, rDate.m_Date, sizeof( Long32 ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date::~Date ()
        {
            if ( m_ownsDate )
            {
                delete m_Date;
                m_Date = 0;
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return integer date */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int32 Date::getDate () const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            Int16 mdy[3];

            if ( 0 > ::rjulmdy( *m_Date, mdy ) )
            {
                return NULL_DATE;
            }

            return DateTimeTools::getDate( mdy[2], mdy[0], mdy[1] );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return year component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int16   Date::getYear() const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            return DateTimeTools::getYear( getDate() );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return month component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int16   Date::getMonth() const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            return DateTimeTools::getMonth( getDate() );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return day component */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int16 Date::getDay() const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            return DateTimeTools::getDay( getDate() );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \return day of week\
            \retval 0 = SUNDAY if date is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Date::WeekDayEnum Date::getDayOfWeek () const
        {
            if ( !isValid() )
            {
                return SUNDAY;
            }

            return static_cast<Date::WeekDayEnum>( ::rdayofweek( *m_Date ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \return day of year: 0 – 365 (January 01 = 0)
            \retval 0 if date is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int16 Date::getDayOfYear () const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            Int16      doy  = -1;
            Int16      year = getYear ();
            Int16      mon  = getMonth() - 1;
            Int16      day  = getDay  ();

            for ( Int32 i = 0; i < mon; ++i )
            {
                doy = doy + days_per_month( i );

                if ( ( 1 == i ) && isLeapYear( year ) ) // February and leap year
                {
                    ++doy;
                }
            }

            return doy + day;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \note   algorithm DIN 1355 / ISO 8601 from http://www.a-m-i.de/tips/datetime/datetime.php;
                    also see: http://de.wikipedia.org/wiki/Kalenderwoche#Kalenderwoche
            \throw  no-throw
            \return week of year: Monday is first day of week (1 – 53)
            \retval 0 if date is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int16 Date::getWeekOfYear () const
        {
            if ( !isValid() )
            {
                return NULL_DATE;
            }

            Int16 doy     = getDayOfYear() + 1;                                           // [1, 366]
            Int16 dowJan1 = static_cast<Int16>( Date( getYear(), 1, 1 ).getDayOfWeek() ); // 1st January: [0, 6]

            // exception: Friday and Saturday
            if ( dowJan1 >= FRIDAY )
            {
                dowJan1 -= 7;
            }

            // exception: begin of year with week no. from year before
            if ( ( doy + dowJan1 ) <= 1 )
            {
                return Date( getYear() - 1, 12, 31 ).getWeekOfYear();
            }

            Int16 week = ( ( doy - 1 + dowJan1 - 1 ) / 7 ) + 1;

            assert( week >= 1 );
            assert( week <= 53 );

            // 53 weeks is only possible for years beginning with Thursday;
            // in leap years 53 weeks are possible with a beginning Wednesday (e.g 1992);
            // otherwise this week is week no. 1 for the year after
            if ( 53 == week )
            {
                if (    ( dowJan1 != THURSDAY )   &&  ( ( dowJan1 != WEDNESDAY ) || !isLeapYear( getYear() ) )   )
                {
                    week = 1;    // adjust value
                }
            }

            return week;
        }

        const Int32* Date::getDateBuffer   () const
        {
            return m_Date;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::setDate ( const Int32 date )
        {
            setDate( DateTimeTools::getYear ( date ),
                     DateTimeTools::getMonth( date ),
                     DateTimeTools::getDay  ( date ) );
        }

        void Date::setDate (const Date& date )
        {
            this->operator=( date );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::setDate ( const Int32 year, const Int32 month, const Int32 day )
        {
            if ( ! DateTimeTools::isValidDate( year, month, day ) )
            {
                setInvalid();
                return;
            }

            Int16 mdy[3] =
            {
                static_cast<Int16>( month ),
                static_cast<Int16>( day  ),
                static_cast<Int16>( year )
            };

            if ( 0 > ::rmdyjul( mdy, reinterpret_cast<int4*>( m_Date ) ) )
            {
                setInvalid();
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::setInvalid()
        {
            *m_Date = nullvalue();
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \retval true  if date and time is correctly set/valid
            \retval false if date and time is not set/invalid     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::isValid() const
        {
            return !isNull();
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! no operation if date is not appropriate
            \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::addDays ( const Int32 days )
        {
            if ( !isValid() )
            {
                return;
            }

            *m_Date += days;

            assert( isValid() );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! no operation if date is not appropriate
            \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::addMonths( const Int32 months )
        {
            if ( !isValid() )
            {
                return;
            }

            DateTime dt( *this );

            dt.addMonths( months );

            if ( dt.isValidDate() )
            {
                setDate( dt.getDate() );
            }

        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! no operation if date is not appropriate
            \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::addYears( const Int32 years )
        {
            if ( !isValid() )
            {
                return;
            }

            DateTime dt( *this );

            dt.addYears( years );

            if ( dt.isValidDate() )
            {
                setDate( dt.getDate() );
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! no operation if date is not appropriate
            \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! Format control:
            \n      dd      day of month as a two-digit number (01 through 31)
            \n      ddd     day of week as a three-letter abbreviation (Sun through Sat)
            \n      mm      month as a two-digit number (01 through 12)
            \n      mmm     month as a three-letter abbreviation (Jan through Dec)
            \n      yy      year as a two-digit number (00 through 99)
            \n      yyyy    year as a four-digit number (0001 through 9999)
            \n      ww      day of week as a two-digit number (00 for Sunday, 01 for Monday, 02 for Tuesday ... 06 for Saturday)
            \n      Further controls see Informix ESQL/C Manual: Working with Numeric Strings - Formatting Numeric Strings
            \throw  DateTimeException
            \return converted string using format-control string
            \retval empty string if date/time is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const I18nString Date::toString ( ConstString format ) const
        {
            assert( format );

            // if ( !isValid() )    return "";          MEI: allow NULL value for saving unset DB date

            char         str[1024] = {0};
            Int32        ret;

            switch ( ret = ::rfmtdate( *m_Date, const_cast<char*>( format ), str ) )
            {
                case 0:
                    return str;

                case DTERROR_CONV:
                {
                    VarString msg;
                    msg.format( "::rfmtdate(format=\"%s\") returns %d => conversion failure", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.toString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_MEMORY:
                {
                    VarString msg;
                    msg.format( "::rfmtdate(format=\"%s\") returns %d => memory allocation error", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.toString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_FORMAT:
                {
                    VarString msg;
                    msg.format( "::rfmtdate(format=\"%s\") returns %d => invalid format string", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.toString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                default:
                {
                    VarString msg;
                    msg.format( "::rfmtdate(format=\"%s\") returns %d", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.toString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \return date representation for current locale
            \retval empty string if date is invalid */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const I18nString Date::toStrDate() const
        {
            static ConstString format[] =
            {
                "dd.mm.yyyy",
                "dd/mm/yyyy",
                "mm/dd/yyyy"
            };

            switch ( Locale::getDateFormat() )
            {
                case Locale::FMT_DATE_AMERICAN:
                    return toString( format[2] );

                case Locale::FMT_DATE_EUROPEAN_SLASH:
                    return toString( format[1] );

                case Locale::FMT_DATE_EUROPEAN:
                default:
                    return toString( format[0] );
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! Format control:
            \n      dd      day of month as a two-digit number (01 through 31)
            \n      ddd     day of week as a three-letter abbreviation (Sun through Sat)
            \n      mm      month as a two-digit number (01 through 12)
            \n      mmm     month as a three-letter abbreviation (Jan through Dec)
            \n      yy      year as a two-digit number (00 through 99)
            \n      yyyy    year as a four-digit number (0001 through 9999)
            \n      ww      day of week as a two-digit number (00 for Sunday, 01 for Monday, 02 for Tuesday ... 06 for Saturday)
            \n      Further controls see Informix ESQL/C Manual: Working with Numeric Strings - Formatting Numeric Strings
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::fromString( ConstString format, const I18nString& rDate )
        {
            assert( format );

            if ( rDate.empty() )
            {
                setInvalid();
                return;
            }

            Int32 ret;

            switch ( ret = ::rdefmtdate( reinterpret_cast<int4*>( m_Date ),
                                         const_cast<char*>( format ),
                                         const_cast<char*>( rDate.c_str() ) ) )
            {
                case 0:
                    break;

                case DTERROR_YEAR:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\", <%s>) returns %d => invalid year", format, rDate.c_str(), ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_MONTH:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\", <%s>) returns %d => invalid month", format, rDate.c_str(), ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_DAY:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\", <%s>) returns %d => invalid day", format, rDate.c_str(), ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_DELIMITER:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\") returns %d => no delimiters or invalid string length", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                case DTERROR_FORMAT:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\", <%s>) returns %d => invalid format string", format, rDate.c_str(), ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }

                default:
                {
                    VarString msg;
                    msg.format( "::rdefmtdate(format=\"%s\") returns %d", format, ret );
                    ExceptInfo sInfo( "basar.cmnutil.Date.fromString()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );
                }
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::fromStrDate( const I18nString& rDate )
        {
            static ConstString format[] =
            {
                "dd.mm.yyyy",
                "dd/mm/yyyy",
                "mm/dd/yyyy"
            };

            switch ( Locale::getDateFormat() )
            {
                case Locale::FMT_DATE_AMERICAN:
                    fromString( format[2], rDate );
                    break;

                case Locale::FMT_DATE_EUROPEAN_SLASH:
                    fromString( format[1], rDate );
                    break;

                case Locale::FMT_DATE_EUROPEAN:
                default:
                    fromString( format[0], rDate );
                    break;
            }
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if context object is congruent with passed object
            \retval false otherwise  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::operator == ( const Date& rDate ) const
        {
            return *( m_Date ) == *( rDate.m_Date );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if this is less than passed object (this is earlier)
            \retval false otherwise  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::operator< ( const Date& rDate ) const
        {
            return *( m_Date ) < *( rDate.m_Date );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if this is less than or equal passed object (this is earlier)
            \retval false otherwise  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::operator<= ( const Date& rDate ) const
        {
            return ( this->operator<( rDate ) || this->operator==( rDate ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if this is greater than passed object (this is earlier)
            \retval false otherwise  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::operator> ( const Date& rDate ) const
        {
            return ( false == this->operator<=( rDate ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if this is greater than or equal passed object (this is earlier)
            \retval false otherwise  */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::operator>= ( const Date& rDate ) const
        {
            return ( false == this->operator<( rDate ) );
        }

//! \return reference to Date object which was assigned
        Date& Date::operator =  ( const Date& rDate )
        {
            assert( m_ownsDate == true );

            if ( this != &rDate )
            {
                memcpy( m_Date, rDate.m_Date, sizeof( Long32 ) );
            }

            return *this;
        }

        Date& Date::operator++()
        {
            assert( m_ownsDate == true );

            ++(*m_Date);

            return *this;
        }

        Date& Date::operator--()
        {
            assert( m_ownsDate == true );

            --(*m_Date);

            return *this;
        }

        Date Date::operator++(int)
        {
            assert( m_ownsDate == true );

            Date tmp( *this );

            ++(*m_Date);

            return tmp;
        }

        Date Date::operator--(int)
        {
            assert( m_ownsDate == true );

            Date tmp( *this );

            --(*m_Date);

            return tmp;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw  no-throw
            \retval true if date is NULL value (i.e SQL default date)
            \retval false otherwise */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool Date::isNull () const
        {
            return ( nullvalue() == *m_Date );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Date::setCurrent()
        {
            ::rtoday( reinterpret_cast<int4*>( m_Date ) );
        }

//---------------------------------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief standard ostream operator
            \param stream - outstream
            \param date - date to output
            \return reference to std::ostream
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::ostream& operator<< ( std::ostream& stream, const Date& date )
        {
            return ( stream << date.toStrDate() );
        }

//---------------------------------------------------------------------------------------------------------------------//
    }//cmnutil
}//basar

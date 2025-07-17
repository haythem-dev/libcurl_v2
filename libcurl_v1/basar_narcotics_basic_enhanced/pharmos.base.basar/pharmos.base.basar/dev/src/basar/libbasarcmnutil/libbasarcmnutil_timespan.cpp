//---------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  time span class implementation
 *  \author Marco Köppendörfer
 *  \date   06.05.2013
 */
//---------------------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_timespan.h"
#include "timestamp.h"
#include "libbasarcmnutil_i18nstring.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <assert.h>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <cstring>

//---------------------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace cmnutil
    {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \throw no-throw
            \return size of internal data structure */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int TimeSpan::getInfxDescSize()
        {
            static int infxDescSize = 0;

            if ( 0 == infxDescSize )
            {
                infxDescSize = sizeof( Timestamp );
            }

            return infxDescSize;
        }


//--------------------------------------------------------------------------------------------------//
// c'tor / d'tor
//--------------------------------------------------------------------------------------------------//

        TimeSpan::TimeSpan() : m_ownsTimestamp( true ), m_Stamp( 0 )
        {
            try
            {
                m_Stamp = new Timestamp( TIMESPAN_DAYS );
            }
            catch ( ... )
            {
                delete m_Stamp;
                m_Stamp = 0;
                throw;
            }
        }

        TimeSpan::TimeSpan( const I18nString& normalizedString ) : m_ownsTimestamp( true ), m_Stamp( 0 )
        {
            //dontdo: use smart_ptr - note: smart_ptr not feasable - TimeSpan uses mixed ownership of internal buffer

            try
            {
                m_Stamp = new Timestamp( TIMESPAN_DAYS );
            }
            catch ( ... )
            {
                delete m_Stamp;
                m_Stamp = 0;
                throw;
            }

            //avoid resource leak - fromNormalizedString throws
            try
            {
                fromNormalizedString( normalizedString );
            }
            catch ( ... )
            {
                delete m_Stamp;
                m_Stamp = 0;
                throw;
            }
        }

        TimeSpan::TimeSpan( const TimeSpan& source ) : m_ownsTimestamp( true ), m_Stamp( 0 )
        {
            try
            {
                m_Stamp = new Timestamp( TIMESPAN_DAYS );
            }
            catch ( ... )
            {
                delete m_Stamp;
                m_Stamp = 0;
                throw;
            }

            memcpy( m_Stamp, source.m_Stamp, sizeof( Timestamp ) );
        }

        TimeSpan::~TimeSpan()
        {
            if ( m_ownsTimestamp )
            {
                delete m_Stamp;
                m_Stamp = 0;
            }
        }

        TimeSpan::TimeSpan( const intrvl* pTime ) : m_ownsTimestamp( false ), m_Stamp( reinterpret_cast<Timestamp*>( const_cast<intrvl_t*>( pTime ) ) )
        {
        }

        TimeSpan::TimeSpan(basar::Int32 value, TimeSpanUnitEnum unit) : m_ownsTimestamp(true), m_Stamp(0)
        {
            try
            {
                m_Stamp = new Timestamp( TIMESPAN_DAYS );
            }
            catch ( ... )
            {
                delete m_Stamp;
                m_Stamp = 0;
                throw;
            }

            fromNumeric(value, unit);
        }


//--------------------------------------------------------------------------------------------------//
// operators
//--------------------------------------------------------------------------------------------------//
        TimeSpan& TimeSpan::operator=( const TimeSpan& rHs )
        {
            checkIfTimestampIsOwned();

            if ( this != &rHs )
            {
                memcpy( this->m_Stamp, rHs.m_Stamp, sizeof( Timestamp ) );
            }

            return *this;
        }

        TimeSpan& TimeSpan::operator+=( const TimeSpan& rHs )
        {
            //logic for interval addition as follows (there is no invaddinv function in informix sdk):
            //1) create arbituary datetime
            //2) store initial arbituary datetime value
            //3) add right hand interval to arbituary datetime
            //4) add left hand interval to result of 3)
            //5) subtract initial arbituary datetime from result of 4) to get result of interval addition

            checkIfTimestampIsOwned();

            DateTime dummy( dummy_date, 0 );

            static const DateTime dummy2( dummy );

            //done:  interval ranges are expanded automatically

            Int32 ret = ::dtaddinv( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<intrvl*>( rHs.m_Stamp ), reinterpret_cast<dtime*>( dummy.m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtaddinv adding right hand interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator+=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            if ( false == dummy.isValid() )
            {
                VarString msg( "::dtaddinv adding right hand interval resulted in invalid DateTime - used Timespan might be too large" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator+=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            ret = ::dtaddinv( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<intrvl*>( m_Stamp ), reinterpret_cast<dtime*>( dummy.m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtaddinv adding left hand interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator+=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            if ( false == dummy.isValid() )
            {
                VarString msg( "::dtaddinv adding left hand interval resulted in invalid DateTime - used Timespan might be too large" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator+=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            ret = ::dtsub( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<dtime*>( dummy2.m_Stamp ), reinterpret_cast<intrvl*>( m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtsub creating result interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator+=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            return *this;
        }

        TimeSpan& TimeSpan::operator-=( const TimeSpan& rHs )
        {
            //logic for interval subtraction as follows (there is no invsubinv function in informix sdk):
            //1) create arbituary datetime
            //2) store initial arbituary datetime value
            //3) subtract right hand interval to arbituary datetime
            //4) add left hand interval to result of 3)
            //5) subtract initial arbituary datetime from result of 4) to get result of interval addition

            checkIfTimestampIsOwned();

            DateTime dummy( dummy_date, 0 );

            static const DateTime dummy2( dummy );

            //done: interval ranges are expanded automatically

            Int32 ret = ::dtsubinv( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<intrvl*>( rHs.m_Stamp ), reinterpret_cast<dtime*>( dummy.m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtsubinv subtracting right hand interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator-=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            if ( false == dummy.isValid() )
            {
                VarString msg( "::dtaddinv subtracting right hand interval resulted in invalid DateTime - used Timespan might be too large" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator-=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            ret = ::dtaddinv( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<intrvl*>( m_Stamp ), reinterpret_cast<dtime*>( dummy.m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtaddinv adding left hand interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator-=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            if ( false == dummy.isValid() )
            {
                VarString msg( "::dtaddinv subtracting left hand interval resulted in invalid DateTime - used Timespan might be too large" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator-=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            ret = ::dtsub( reinterpret_cast<dtime*>( dummy.m_Stamp ), reinterpret_cast<dtime*>( dummy2.m_Stamp ), reinterpret_cast<intrvl*>( m_Stamp ) );

            if ( 0 > ret )
            {
                VarString msg( "::dtsub creating result interval" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator-=()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            return *this;
        }

        TimeSpan& TimeSpan::operator/=( const Float64 divisor )
        {
            checkIfTimestampIsOwned();

            internalDivideByNumeric( divisor, m_Stamp );

            return *this;
        }

        TimeSpan& TimeSpan::operator/=( const Decimal& divisor )
        {
            checkIfTimestampIsOwned();

            internalDivideByNumeric( divisor.toFloat64(), m_Stamp );

            return *this;
        }

        Float64 TimeSpan::operator/( const TimeSpan& divisor ) const
        {
            Float64 result = 0.0;

            Int32 ret = ::invdivinv( reinterpret_cast<intrvl_t*>( m_Stamp ), reinterpret_cast<intrvl_t*>( divisor.m_Stamp ), &result );

            if ( 0 > ret )
            {
                VarString msg( "::invdivinv dividing two intervals" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.operator/()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            return result;

        }

        TimeSpan& TimeSpan::operator*=( const Float64 mult )
        {
            checkIfTimestampIsOwned();

            internalMultiplicateByNumeric( mult, m_Stamp );

            return *this;
        }

        TimeSpan& TimeSpan::operator*=( const Decimal& mult )
        {
            checkIfTimestampIsOwned();

            internalMultiplicateByNumeric( mult.toFloat64(), m_Stamp );

            return *this;
        }

        bool TimeSpan::operator==( const TimeSpan& rHs ) const
        {
            return EQUAL == internalCompairSizeWithThis( rHs );
        }

        bool TimeSpan::operator!=( const TimeSpan& rHs ) const
        {
            return !( operator==( rHs ) );
        }

        bool TimeSpan::operator<( const TimeSpan& rHs ) const
        {
            return LESS_THAN == internalCompairSizeWithThis( rHs );
        }

        bool TimeSpan::operator>( const TimeSpan& rHs ) const
        {
            return GREATER_THAN == internalCompairSizeWithThis( rHs );
        }

//--------------------------------------------------------------------------------------------------//
// object methods
//--------------------------------------------------------------------------------------------------//
        const intrvl* TimeSpan::getTimestampBuffer() const
        {
            return reinterpret_cast<const intrvl*>( m_Stamp );
        }

        const I18nString TimeSpan::toNormalizedString() const
        {
            return toFormattedString( ts_format_short() );
        }

//returns string according to internal range
//DONE: find better name - renamed old toString to toNormalizedString
        const I18nString TimeSpan::toString() const
        {
            return toFormattedString( getRangedFormat( TU_START( m_Stamp->m_Qual ) ) );
        }

        const I18nString TimeSpan::toFormattedString( ConstString format ) const
        {
            checkIfTimestampIsOwned();

            const bool isNegative = this->isNegative();

            const Int32 bufsize = 64;

            char        bufspan[bufsize] = { 0 };

            Timestamp copy( *m_Stamp );

            if ( true == isNegative )
            {
                copy.m_Dec.dec_pos = 1;
            }

            int ret = ::intofmtasc( reinterpret_cast<intrvl_t*>( &copy ), bufspan, bufsize, const_cast<char*>( format ) );

            if ( 0 > ret )
            {
                VarString msg;
                msg.format( "::intofmtasc failed: format string was %s ", format );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.toFormattedString()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            std::ostringstream os;

            if ( true == isNegative )
            {
                os << "-";
            }

            os << bufspan;

            return os.str();
        }


        void TimeSpan::fromString ( ConstString format, const I18nString& rTimeSpan )
        {
            checkIfTimestampIsOwned();

            assert( format );

            const char* fmt;
            const char* pBuf;
            char         buf   [32];
            char         fmtbuf[32];

            if ( rTimeSpan.empty() )
            {
                ::sprintf( buf   , "%s", NULLVAL_SQL_INTERVAL_SHORT );
                ::sprintf( fmtbuf, "%s", ts_format_short() );
                fmt  = fmtbuf;
                pBuf = buf;
            }
            else
            {
                fmt  = format;
                pBuf = rTimeSpan.c_str();
            }

            if ( 0 > ::incvfmtasc( const_cast      <char*>( pBuf    ),
                                   const_cast      <char*>( fmt     ),
                                   reinterpret_cast<intrvl_t*>( m_Stamp ) ) )
            {
                VarString msg;
                msg.format( "::incvfmtasc(format=\"%s\", <%s>) failed", format, rTimeSpan.c_str() );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.fromString()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

            //TODO: check if TU_START is TU_DAY or greater in order to forbid creation of YYYY-MM intervals
        }

        void TimeSpan::fromNormalizedString ( const I18nString& rTimeSpan )
        {
            checkIfTimestampIsOwned();

            const char* fmt;
            const char* pBuf;
            char         buf   [32];
            char         fmtbuf[32];

            ::sprintf( buf   , "%s", NULLVAL_SQL_INTERVAL_SHORT );
            ::sprintf( fmtbuf, "%s", ts_format_short() );
            fmt  = fmtbuf;

            if ( rTimeSpan.empty() )
            {
                pBuf = buf;
            }
            else
            {
                pBuf = rTimeSpan.c_str();
            }

            if ( 0 > ::incvfmtasc( const_cast      <char*>( pBuf    ),
                                   const_cast      <char*>( fmt     ),
                                   reinterpret_cast<intrvl_t*>( m_Stamp ) ) )
            {
                VarString msg;
                msg.format( "::incvfmtasc(format=\"%s\", <%s>) failed", fmt, rTimeSpan.c_str() );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.fromString()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }
        }

        void TimeSpan::fromNumeric( basar::Int32 value, TimeSpanUnitEnum unit )
        {
            checkIfTimestampIsOwned();

            value = ::abs( value );

            switch ( unit )
            {
                case TimeSpan::TSUNIT_SEC:
                    internalFromNumericSeconds( value );
                    break;

                case TimeSpan::TSUNIT_MIN:
                    internalFromNumericMinutes( value );
                    break;

                case TimeSpan::TSUNIT_HOUR:
                    internalFromNumericHours( value );
                    break;

                case TimeSpan::TSUNIT_DAY:
                    internalFromNumericDays( value );
                    break;

                default:
                {
                    std::ostringstream os;
                    os << "passed unit type is not implemented, passed unit type was <" << unit << ">";

                    ExceptInfo sInfo( "basar.cmnutil.TimeSpan.fromNumeric()", os.str(), __FILE__, __LINE__ );
                    throw BasarNotImplementedException( sInfo );
                }
                break;
            }
        }

        void TimeSpan::fromNumeric( const TimeSpan::Numeric& numeric, TimeSpanFractionEnum unit /*= TSFRACTION_MSEC*/ )
        {
            std::ostringstream os;

            TimeSpan::Numeric n = fixNumeric( numeric, unit );

            if ( true == numeric.isNegative )
            {
                os << '-';
            }

            os << n.days << ' ' << std::setw( 2 ) << std::setfill( '0' ) << n.hours << std::setw( 1 ) << ':' << std::setw( 2 ) << n.minutes << std::setw( 1 ) << ':' << std::setw( 2 ) << n.seconds << std::setw( 1 ) << '.';

            const basar::Int32 fractionLength = getFractionLength( unit );

            os << std::setw( fractionLength ) << n.fraction;

            fromNormalizedString( os.str() );
        }

        TimeSpan::Numeric TimeSpan::toNumeric() const
        {
            TimeSpan::Numeric n;

            basar::I18nString tsString = toNormalizedString();

            boost::regex re( "\\d+" );

            boost::sregex_token_iterator it ( tsString.begin(), tsString.end(), re );

            n.days = boost::lexical_cast<basar::Int32>( it->str() );
            ++it;
            n.hours = boost::lexical_cast<basar::Int32>( it->str() );
            ++it;
            n.minutes = boost::lexical_cast<basar::Int32>( it->str() );
            ++it;
            n.seconds = boost::lexical_cast<basar::Int32>( it->str() );
            ++it;
            n.fraction = boost::lexical_cast<basar::Int32>( it->str() );

            if ( true == isNegative() )
            {
                n.isNegative = true;
            }

            return n;
        }

        inline void TimeSpan::checkIfTimestampIsOwned() const
        {
            if ( false == m_ownsTimestamp )
            {
                VarString msg;
                msg.format( "Timestamp is not owned by instance" );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.fromString()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }
        }

        void TimeSpan::internalFromNumericSeconds( basar::Int32 seconds )
        {
            basar::Int32 days = seconds / TimeSpan::SECONDS_PER_DAY;

            seconds %= TimeSpan::SECONDS_PER_DAY;

            basar::Int32 hours = seconds / TimeSpan::SECONDS_PER_HOUR;

            seconds %= TimeSpan::SECONDS_PER_HOUR;

            basar::Int32 minutes = seconds / TimeSpan::SECONDS_PER_MINUTE;

            seconds %= TimeSpan::SECONDS_PER_MINUTE;

            std::ostringstream os;

            os <<  days << ' ' << std::setw( 2 ) << std::setfill( '0' ) << hours << std::setw( 1 ) << ':' << std::setw( 2 ) << minutes << std::setw( 1 ) << ':' << std::setw( 2 ) << seconds << ".000"; //<trailing ms

            fromNormalizedString( os.str() );
        }

        void TimeSpan::internalFromNumericDays( basar::Int32 days )
        {
            std::ostringstream os;

            os << days << " 00:00:00.000";

            fromNormalizedString( os.str() );
        }

        void TimeSpan::internalFromNumericMinutes( basar::Int32 minutes )
        {
            basar::Int32 days = minutes / TimeSpan::MINUTES_PER_DAY;

            minutes %= TimeSpan::MINUTES_PER_DAY;

            basar::Int32 hours = minutes / TimeSpan::MINUTES_PER_HOUR;

            minutes %= TimeSpan::MINUTES_PER_HOUR;

            std::ostringstream os;

            os << days << ' ' << std::setw( 2 ) << std::setfill( '0' ) << hours << std::setw( 1 ) << ':' << std::setw( 2 ) << minutes << ":00.000";

            fromNormalizedString( os.str() );
        }

        void TimeSpan::internalFromNumericHours( basar::Int32 hours )
        {
            basar::Int32 days = hours / TimeSpan::HOURS_PER_DAY;

            hours %= TimeSpan::HOURS_PER_DAY;

            std::ostringstream os;

            os << days << ' ' << std::setw( 2 ) << std::setfill( '0' ) << hours << ":00:00.000";

            fromNormalizedString( os.str() );
        }

        inline TimeSpan::Numeric TimeSpan::fixNumeric( const TimeSpan::Numeric& n, TimeSpanFractionEnum unit ) const
        {
            TimeSpan::Numeric fix( n );

            //make sure all members of fix are positive
            fix.days = ::abs( fix.days );
            fix.hours = ::abs( fix.hours );
            fix.minutes = ::abs( fix.minutes );
            fix.seconds = ::abs( fix.seconds );
            fix.fraction = ::abs( fix.fraction );

            const basar::Int32 Fractions_per_Second = getFractionPerSecond( unit );

            //first: add all remainders to next bigger member (sec -> min, min -> hour, etc..)
            fix.seconds += fix.fraction / Fractions_per_Second;
            fix.minutes += fix.seconds / SECONDS_PER_MINUTE;
            fix.hours += fix.minutes / MINUTES_PER_HOUR;
            fix.days += fix.hours / HOURS_PER_DAY;

            //second: fix remaining member
            fix.fraction %= Fractions_per_Second;
            fix.seconds %= SECONDS_PER_MINUTE;
            fix.minutes %= MINUTES_PER_HOUR;
            fix.hours %= HOURS_PER_DAY;

            return fix;
        }

        inline basar::Int32 TimeSpan::getFractionPerSecond( TimeSpanFractionEnum unit ) const
        {
            switch ( unit )
            {
                case TSUNIT_DSEC:
                    return 10;
                    break;

                case TSUNIT_CSEC:
                    return 100;
                    break;

                case TSUNIT_MSEC:
                    return 1000;
                    break;

                case TSUNIT_100uSEC:
                    return 10000;
                    break;

                case TSUNIT_10uSEC:
                    return 100000;
                    break;

                default:
                {
                    std::ostringstream os;
                    os << "passed unit type is not a fraction of a second, passed unit type was <" << unit << ">";

                    ExceptInfo sInfo( "TimeSpan::getFractionPerSecond()", os.str(), __FILE__, __LINE__ );
                    throw BasarInvalidParameterException( sInfo );
                }
                break;
            };
        }

        inline basar::Int32 TimeSpan::getFractionLength( TimeSpanFractionEnum unit ) const
        {
            switch ( unit )
            {
                case TSUNIT_DSEC:
                    return 1;
                    break;

                case TSUNIT_CSEC:
                    return 2;
                    break;

                case TSUNIT_MSEC:
                    return 3;
                    break;

                case TSUNIT_100uSEC:
                    return 4;
                    break;

                case TSUNIT_10uSEC:
                    return 5;
                    break;

                default:
                {
                    std::ostringstream os;
                    os << "passed unit type is not a fraction of a second, passed unit type was <" << unit << ">";

                    ExceptInfo sInfo( "TimeSpan::getFractionPerSecond()", os.str(), __FILE__, __LINE__ );
                    throw BasarInvalidParameterException( sInfo );
                }
                break;
            };
        }

        inline bool TimeSpan::isNegative() const
        {
            return ( 0 == m_Stamp->m_Dec.dec_pos );
        }

        void TimeSpan::setRange ( TimeSpanUnitEnum from, TimeSpanUnitEnum to )
        {
            intrvl_t ivTmp = *reinterpret_cast<intrvl_t*>( m_Stamp );                       // copy current value to temporary variable

            //make sure same interval class
            assert( TU_DAY >= TU_START( m_Stamp->m_Qual ) );

            static const Int16 TIMESPAN_UNITS[] =
            {
                TU_YEAR,            //!< year
                TU_MONTH,           //!< month
                TU_DAY,             //!< day
                TU_HOUR,            //!< hour
                TU_MINUTE,          //!< minute
                TU_SECOND,          //!< second
                TU_F1,              //!< 10th second
                TU_F2,              //!< 100th second
                TU_F3,              //!< 1000th second
                TU_F4,              //!< 10000th second
                TU_F5               //!< 100000th second
            };

            //TU_IENCODE(9, TU_DAY, TU_F3);
            m_Stamp->m_Qual = TU_IENCODE( 9, TIMESPAN_UNITS[from], TIMESPAN_UNITS[to] ); // set new range
            ::invextend( &ivTmp, reinterpret_cast<intrvl_t*>( m_Stamp ) );              // copy saved value to extended member
        }

        void TimeSpan::setInvalid()
        {
            const char* fmt;
            const char* pBuf;
            char         buf   [32];
            char         fmtbuf[32];

            ::sprintf( buf   , "%s", NULLVAL_SQL_INTERVAL_SHORT );
            ::sprintf( fmtbuf, "%s", ts_format_short() );

            fmt  = fmtbuf;
            pBuf = buf;

            if ( 0 > ::incvfmtasc( const_cast      <char*>( pBuf    ),
                                   const_cast      <char*>( fmt     ),
                                   reinterpret_cast<intrvl_t*>( m_Stamp ) ) )
            {
                VarString msg;
                msg.format( "::incvfmtasc(format=\"%s\", <%s>) failed", fmt, NULLVAL_SQL_INTERVAL_SHORT );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.fromString()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }

        }

        bool TimeSpan::isValid()
        {
            return !isNull();
        }

        bool TimeSpan::isNull()
        {
            //initalized with invalid timespan - TODO: use some kind of static value
            TimeSpan tmp;

            return EQUAL == internalCompairSizeWithThis( tmp );
        }

//--------------------------------------------------------------------------------------------------//
// private functions
//--------------------------------------------------------------------------------------------------//
        void TimeSpan::internalDivideByNumeric( Float64 divisor, Timestamp* result )
        {
            basar::Int32 ret = ::invdivdbl( reinterpret_cast<intrvl_t*>( m_Stamp ), divisor, reinterpret_cast<intrvl_t*>( result ) );

            if ( 0 > ret )
            {
                VarString msg;
                msg.format( "::invdivdbl failed with %d", ret );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.internalDivideByNumeric()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }
        }

        void TimeSpan::internalMultiplicateByNumeric( Float64 mult, Timestamp* result )
        {
            basar::Int32 ret = ::invmuldbl( reinterpret_cast<intrvl_t*>( m_Stamp ), mult, reinterpret_cast<intrvl_t*>( result ) );

            if ( 0 > ret )
            {
                VarString msg;
                msg.format( "::invmuldbl failed with %d", ret );
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.internalDivideByNumeric()", msg, __FILE__, __LINE__ );
                throw DateTimeException( sInfo );
            }
        }

        TimeSpan::ResultOfCompairison TimeSpan::internalCompairSizeWithThis( const TimeSpan& rHs ) const
        {
            //shortcut
            if ( this == &rHs )
            {
                return EQUAL;
            }

            if ( ! ( ( TU_DAY <= TU_START( m_Stamp->m_Qual ) ) && ( TU_DAY <= TU_START( rHs.m_Stamp->m_Qual ) ) )  )
            {
                ExceptInfo sInfo( "basar.cmnutil.TimeSpan.internalCompairSizeWithThis()", "compaired types are not compatible", __FILE__, __LINE__ );
                throw TypeMismatchException( sInfo );
            }

            //initialize pointers in case both m_Qual are the same, otherwise we'd compare null pointers
            const TimeSpan* cmpCopy = this;

            const TimeSpan* cmpOther = &rHs;

            //also initialize tmp
            TimeSpan tmp( *this );

            //tricky part: find out which TimeSpan has the lesser TU_START
            if ( m_Stamp->m_Qual != rHs.m_Stamp->m_Qual )
            {
                if ( TU_START( m_Stamp->m_Qual ) > TU_START( rHs.m_Stamp->m_Qual ) )
                {
                    //make copy of rHs, expand it's interval, compare
                    cmpCopy = &rHs;

                    cmpOther = this;
                }

                //no else part, see initialization of cmpCopy and cmpOther

                //expand tmp

                tmp.m_Stamp->m_Qual = cmpOther->m_Stamp->m_Qual;

                basar::Int32 retVal = ::invextend( reinterpret_cast<intrvl_t*>( cmpCopy->m_Stamp ), reinterpret_cast<intrvl_t*>( tmp.m_Stamp ) );

                //todo: check return value
                if ( 0 > retVal )
                {
                    VarString msg( "::invextend failed" );
                    ExceptInfo sInfo( "basar.cmnutil.TimeSpan.internalCompairSizeWithThis()", msg, __FILE__, __LINE__ );
                    throw DateTimeException( sInfo );

                }
            }

            return static_cast<ResultOfCompairison>( ::deccmp( reinterpret_cast<dec_t*>( &tmp.m_Stamp->m_Dec ), reinterpret_cast<dec_t*>( &cmpOther->m_Stamp->m_Dec ) ) );
        }

        ConstBuffer TimeSpan::getRangedFormat( int tu_start ) const
        {
            //TODO: use nice functions akin to ts_format_short()
            switch ( tu_start )
            {
                case TU_DAY:
                {
                    return ts_format_short();
                    break;
                }

                case TU_HOUR:
                {
                    return "%H:%M:%S%F3";
                    break;
                }

                case TU_MINUTE:
                {
                    return "%M:%S%F3";
                    break;
                }

                case TU_SECOND:
                {
                    return "%S%F3";
                    break;
                }

                case TU_F1:
                case TU_F2:
                case TU_F3:
                {
                    return "%F3";
                    break;
                }

                default:
                {
                    return ts_format_short();
                    break;
                }
            }
        }

//---------------------------------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief standard ostream operator
            \param stream - outstream
            \param ts - timespan to output
            \return reference to std::ostream
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::ostream& operator<< ( std::ostream& stream, const TimeSpan& ts )
        {
            return ( stream << ts.toNormalizedString() );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan less-than or equal
            \param lhs - left hand of comparison
            \param rhs - right hand of comparison
            \return bool representing result of comparison
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator<=( const TimeSpan& lhs, const TimeSpan& rhs )
        {
            return ( lhs.operator < ( rhs ) || lhs.operator == ( rhs ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan greater-than or equal
            \param lhs - left hand of comparison
            \param rhs - right hand of comparison
            \return bool representing result of comparison
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator>=( const TimeSpan& lhs, const TimeSpan& rhs )
        {
            return ( lhs.operator > ( rhs ) || lhs.operator == ( rhs ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan addition
            \param lhs - left hand of addition
            \param rhs - right hand of addition
            \return TimeSpan representing result of addition
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator+( TimeSpan lhs, const TimeSpan& rhs )
        {
            lhs += rhs;
            return lhs;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan subtraction
            \param lhs - left hand of subtraction
            \param rhs - right hand of subtraction
            \return TimeSpan representing result of subtraction
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator-( TimeSpan lhs, const TimeSpan& rhs )
        {
            lhs -= rhs;
            return lhs;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan multiplication
            \param mult - numeric value to multiplcate by
            \param ts - timespan that is multiplicated
            \return TimeSpan representing result of multiplication
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator* ( basar::Float64 mult, TimeSpan ts )
        {
            return ts *= mult;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan multiplication
            \param mult - numeric value to multiplcate by
            \param ts - timespan that is multiplicated
            \return TimeSpan representing result of multiplication
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator* ( basar::Int32 mult, TimeSpan ts )
        {
            return ts.operator *= ( static_cast<basar::Float64>( mult ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan multiplication
            \param mult - numeric value to multiplcate by
            \param ts - timespan that is multiplicated
            \return TimeSpan representing result of multiplication
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator* ( TimeSpan ts, basar::Float64 mult )
        {
            return ts *= mult;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan multiplication
            \param mult - numeric value to multiplcate by
            \param ts - timespan that is multiplicated
            \return TimeSpan representing result of multiplication
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator* ( TimeSpan ts, basar::Int32 mult )
        {
            return ts.operator *= ( static_cast<basar::Float64>( mult ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan division
            \param div - numeric value to divide by
            \param ts - timespan that is divided
            \return TimeSpan representing result of division
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator/ ( TimeSpan ts, basar::Float64 div )
        {
            return ts /= div;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan division
            \param div - numeric value to divide by
            \param ts - timespan that is divided
            \return TimeSpan representing result of division
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator/ ( TimeSpan ts, basar::Int32 div )
        {
            return ts.operator /= ( static_cast<basar::Float64>( div ) );
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*! \brief timespan division
            \param div - decimal value to divide by
            \param ts - timespan that is divided
            \return TimeSpan representing result of division
            \throw DateTimeException */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const TimeSpan operator/ ( TimeSpan ts, const basar::Decimal& div )
        {
            return ts /= div;
        }

//---------------------------------------------------------------------------------------------------------------------//

    }//cmnutil
}//basar
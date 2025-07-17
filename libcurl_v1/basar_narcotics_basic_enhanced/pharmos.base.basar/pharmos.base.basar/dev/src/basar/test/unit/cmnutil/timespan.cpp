#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include <iostream>
#include <sstream>


using basar::TimeSpan;
using basar::Float64;
using basar::Float32;

//------------------------------------------------------------------------------
SUITE( TestTimeSpan )
{
    TEST( TimeSpanToString )
    {
        basar::TimeSpan ts;

        CHECK_EQUAL( basar::cmnutil::NULLVAL_SQL_INTERVAL_SHORT, ts.toString() );
    }

    TEST( TimeSpanMaxValue )
    {
        basar::TimeSpan ts;

        ts.fromNormalizedString( "999999999 23:59:59.998" );

        CHECK_EQUAL( "999999999 23:59:59.998", ts.toString() );
    }

    TEST( TimeSpanFromString )
    {
        basar::TimeSpan ts;
        ts.fromString( "%d %H:%M:%S%F3", "12 00:00:00.123" );

        CHECK_EQUAL( ts.toString(), "12 00:00:00.123" );
    }

    TEST( TimeSpanFromEmptyString )
    {
        basar::TimeSpan ts;

        ts.fromString( "%d %H:%M:%S%F3", "" );

        CHECK( ts.isNull() );

        ts.fromNormalizedString( "" );

        CHECK( ts.isNull() );

    }

    TEST( TimeSpanSetRange )
    {
        TimeSpan ts1;

        ts1.fromString( "%d %H:%M:%S%F3", "3 00:00:00.000" );

        CHECK_EQUAL( "03 00:00:00.000", ts1.toString() );

        ts1.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

        CHECK_EQUAL( "72:00:00.000", ts1.toString() );
    }

    TEST( TimeSpanFromStringOrthogonality )
    {
        TimeSpan ts1;

        ts1.fromString( "%H:%M:%S%F3", "72:00:00.000" );

        //TimeSpan defaults to range days-fraction
        CHECK_EQUAL( "03 00:00:00.000", ts1.toString() );

        ts1.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

        CHECK_EQUAL( "72:00:00.000", ts1.toString() );
    }

    TEST( TimeSpanSetRangeOrthogonality )
    {
        TimeSpan ts1;

        ts1.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

        ts1.fromString( "%H:%M:%S%F3", "72:00:00.000" );

        CHECK_EQUAL( "72:00:00.000", ts1.toString() );
    }

    TEST( TimeSpanAddTimeSpan )
    {
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromString( "%d %H:%M:%S%F3", "25 23:03:04.000" );

            ts1 += ts2;

            CHECK_EQUAL( "38 01:06:08.000", ts1.toString() );
        }
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromString( "%d %H:%M:%S%F3", "25 23:03:04.000" );

            TimeSpan tsRes = ts1 + ts2;

            CHECK_EQUAL( "38 01:06:08.000", tsRes.toString() );
        }
    }

    TEST( TimeSpanAddLargeTimeSpan )
    {
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "9999999 00:00:00.000" );

            CHECK_THROW( ts2 += ts1, basar::cmnutil::DateTimeException );
        }

        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "99999 00:00:00.000" );

            TimeSpan tsRes = ts1 + ts2;

            CHECK_EQUAL( "100011 02:03:04.000", tsRes.toString() );
        }

        {
            TimeSpan ts1;
            ts1.fromNormalizedString( "1460000 00:00:00.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "0 23:00:00.000" );

            ts1 += ts2;

            CHECK_EQUAL( "1460000 23:00:00.000", ts1.toString() );
        }

        {
            TimeSpan ts1;
            ts1.fromNormalizedString( "1824635 00:00:00.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "0 23:00:00.000" );

            ts1 += ts2;

            CHECK_EQUAL( "1824635 23:00:00.000", ts1.toString() );
        }

    }

    TEST( TimeSpanAddRangedTimeSpan )
    {
        TimeSpan ts1;
        ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

        TimeSpan ts2;

        ts2.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

        ts2.fromString( "%H:%M:%S%F3", "72:00:00.000" );

        CHECK_EQUAL( "72:00:00.000", ts2.toString() );

        ts1 += ts2;

        CHECK_EQUAL( "15 02:03:04.000", ts1.toString() );
    }

    TEST( TimeSpanSubTimeSpan )
    {
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromString( "%d %H:%M:%S%F3", "25 23:03:04.000" );

            ts2 -= ts1;

            CHECK_EQUAL( "13 21:00:00.000", ts2.toString() );
        }
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromString( "%d %H:%M:%S%F3", "25 23:03:04.000" );

            TimeSpan tsRes = ts2 - ts1;

            CHECK_EQUAL( "13 21:00:00.000", tsRes.toString() );
        }

    }
    TEST ( TimeSpanSubLargeTimeSpan )
    {
        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "9999999 00:00:00.000" );

            CHECK_THROW( ts1 -= ts2, basar::cmnutil::DateTimeException );
        }

        {
            TimeSpan ts1;
            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "99999 00:00:00.000" );

            TimeSpan tsRes = ts2 - ts1;

            CHECK_EQUAL( "99986 21:56:56.000", tsRes.toString() );
        }

        {
            TimeSpan ts1;
            ts1.fromNormalizedString( "1460000 00:00:00.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "0 23:00:00.000" );

            ts1 -= ts2;

            CHECK_EQUAL( "1459999 01:00:00.000", ts1.toString() );
        }

        {
            TimeSpan ts1;
            ts1.fromNormalizedString( "1824635 00:00:00.000" );

            TimeSpan ts2;
            ts2.fromNormalizedString( "0 23:00:00.000" );

            ts1 -= ts2;

            CHECK_EQUAL( "1824634 01:00:00.000", ts1.toString() );
        }
    }

    TEST( TimeSpanSubRangedTimeSpan )
    {
        TimeSpan ts1;
        ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

        TimeSpan ts2;

        ts2.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

        ts2.fromString( "%H:%M:%S%F3", "72:00:00.000" );

        CHECK_EQUAL( "72:00:00.000", ts2.toString() );

        ts1 -= ts2;

        CHECK_EQUAL( "09 02:03:04.000", ts1.toString() );
    }

    TEST( MultTimeSpan )
    {
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            ts1 *= 2.0;

            CHECK_EQUAL( "24 04:06:08.000", ts1.toString() );
        }

        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "12 02:03:04.000" );

            ts1 *= -1.0;

            CHECK_EQUAL( "-12 02:03:04.000", ts1.toString() );
        }
    }

    TEST( AddTimeSpanToDateTime )
    {
        {
            basar::DateTime dt1( 20100101 );

            basar::TimeSpan ts;

            ts.fromString( "%d %H:%M:%S%F3", "12 00:00:00.000" );

            basar::DateTime dt2 = dt1 + ts;

            CHECK_EQUAL( 20100113, dt2.getDate() );
        }
    }

    TEST( TimeSpanEqualityOperator )
    {
        {
            TimeSpan ts1;

            CHECK( ts1 == ts1 );
        }
        {
            TimeSpan ts1, ts2;
            CHECK( ts1 == ts1 );
        }
        {
            TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "3 00:00:00.000" );

            ts2.setRange( TimeSpan::TSUNIT_HOUR, TimeSpan::TSUNIT_MSEC );

            ts2.fromString( "%H:%M:%S%F3", "72:00:00.000" );

            CHECK_EQUAL( "72:00:00.000", ts2.toString() );

            CHECK( ts1 == ts2 );

            //DONE: check if ts1 and ts2 are still the same

            CHECK_EQUAL( "03 00:00:00.000", ts1.toString() );

            CHECK_EQUAL( "72:00:00.000", ts2.toString() );

        }
        {
            TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "3 00:00:00.000" );

            ts2.fromString( "%H:%M:%S%F3", "72:00:00.000" );

            //notice: no setRange, ts2 uses default day-fraction

            CHECK_EQUAL( "03 00:00:00.000", ts2.toString() );

            CHECK( ts1 == ts2 );

            CHECK_EQUAL( "03 00:00:00.000", ts1.toString() );

            CHECK_EQUAL( "03 00:00:00.000", ts2.toString() );

        }
    }

    TEST( TimeSpanLessThanOperator )
    {
        basar::TimeSpan ts1, ts2;

        ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

        ts2.fromString( "%d %H:%M:%S%F3", "12 13:00:00.000" );

        CHECK( ts2 < ts1 );
    }

    TEST( TimeSpanLessOrEqualOperator )
    {
        {
            basar::TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "12 13:00:00.000" );

            CHECK( ts2 <= ts1 );
        }
        {
            basar::TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            CHECK( ts2 <= ts1 );
        }
    }

    TEST( TimeSpanGreaterThanOperator )
    {
        basar::TimeSpan ts1, ts2;

        ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

        ts2.fromString( "%d %H:%M:%S%F3", "12 13:00:00.000" );

        CHECK( ts1 > ts2 );
    }

    TEST( TimeSpanGreaterOrEqualOperator )
    {
        {
            basar::TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "12 13:00:00.000" );

            CHECK( ts1 >= ts2 );
        }
        {
            basar::TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            CHECK( ts2 >= ts1 );
        }
    }

    TEST( DivideTimeSpanByNumeric )
    {
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            Float64 d = 2.0;

            TimeSpan rests = ts1 / d;

            CHECK_EQUAL( "12 18:00:00.000" , rests.toString() );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            Float32 d = 2.0;

            TimeSpan rests = ts1 / d;

            CHECK_EQUAL( "12 18:00:00.000" , rests.toString() );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            basar::Int32 d = 2;

            TimeSpan rests = ts1 / d;

            CHECK_EQUAL( "12 18:00:00.000" , rests.toString() );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            basar::Int16 d = 2;

            TimeSpan rests = ts1 / d;

            CHECK_EQUAL( "12 18:00:00.000" , rests.toString() );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            basar::Decimal d( 2.0 );

            TimeSpan rests = ts1 / d;

            CHECK_EQUAL( "12 18:00:00.000" , rests.toString() );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            Float64 d = 0.0;

            CHECK_THROW( ( ts1 / d ), basar::cmnutil::DateTimeException );


        }
    }

    TEST( DivideTimeSpanByTimeSpan )
    {
        {
            TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "12 18:00:00.000" );

            Float64 d = ts1 / ts2;

            CHECK_EQUAL( 2.0, d );
        }
        {
            TimeSpan ts1, ts2;

            ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

            ts2.fromString( "%d %H:%M:%S%F3", "00 00:00:00.000" );

            CHECK_THROW( ( ts1 / ts2 ), basar::cmnutil::DateTimeException );

        }

    }

    TEST( TimeSpanToNormalizedString )
    {
        TimeSpan ts1;

        ts1.fromString( "%H:%M:%S%F3", "72:00:00.000" );

        CHECK_EQUAL( "03 00:00:00.000", ts1.toNormalizedString() );
    }

    TEST( TimeSpanToFormattedString )
    {
        {
            TimeSpan ts1;

            ts1.fromString( "%H:%M:%S%F3", "72:00:00.000" );

            CHECK_EQUAL( "03 00:00:00.000", ts1.toFormattedString( "%d %H:%M:%S%F3" ) );
        }
        {
            TimeSpan ts1;

            ts1.fromString( "%d %H:%M:%S%F3", "03 00:00:00.000" );

            CHECK_EQUAL( "72:00:00.000", ts1.toFormattedString( "%H:%M:%S%F3" ) );
        }

    }


    TEST( TimeSpanValidity )
    {
        {
            TimeSpan ts1;

            CHECK( ts1.isNull() );

            ts1.fromNormalizedString( "12 00:00:00.123" );

            CHECK( ts1.isValid() );
        }
        {
            TimeSpan ts;

            CHECK( ts.isNull() );
            CHECK( ! ts.isValid() );

            ts.fromNormalizedString( basar::cmnutil::NULLVAL_SQL_INTERVAL_SHORT );

            CHECK( ! ts.isValid() );
        }
        {
            TimeSpan ts;

            ts.fromNormalizedString( "12 00:00:00.123" );

            CHECK( ts.isValid() );

            ts.setInvalid();

            CHECK( ! ts.isValid() );
        }
    }

    TEST( TimeSpanStreamOpOut )
    {
        TimeSpan ts1;

        std::ostringstream os;

        ts1.fromString( "%d %H:%M:%S%F3", "25 12:00:00.000" );

        os << ts1;

        CHECK_EQUAL( "25 12:00:00.000", os.str().c_str() );

    }

    //check for memleaks in ctor
    TEST( TimeSpanStringCtorBad )
    {
        try
        {
            //will throw DateTimeException
            TimeSpan ts1( "25 12:00:00000" );
        }
        catch ( basar::cmnutil::DateTimeException& )
        {
            bool expThrown = true;

            CHECK( expThrown );
        }

    }

    TEST( TimeSpanStringCtorGood )
    {
        try
        {
            TimeSpan ts1( "25 12:00:00.000" );

            CHECK_EQUAL( "25 12:00:00.000", ts1.toNormalizedString() );
        }
        catch ( basar::cmnutil::DateTimeException& )
        {
            bool expThrown = true;

            CHECK( expThrown );
        }

    }

    TEST( TimeSpanFromNumeric )
    {

        {
            TimeSpan ts;
            CHECK_THROW( ts.fromNumeric( 0, TimeSpan::TSUNIT_CSEC ), basar::cmnutil::BasarNotImplementedException );
        }

        {
            TimeSpan ts;

            ts.fromNumeric( 0, TimeSpan::TSUNIT_SEC );
            CHECK_EQUAL( "00 00:00:00.000", ts.toString() );
        }

        try
        {
            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::SECONDS_PER_MINUTE, TimeSpan::TSUNIT_SEC );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_MIN );

                CHECK_EQUAL( ts1, ts2 );
            }

            //hours
            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::MINUTES_PER_HOUR, TimeSpan::TSUNIT_MIN );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_HOUR );

                CHECK_EQUAL( ts1, ts2 );
            }

            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::SECONDS_PER_HOUR, TimeSpan::TSUNIT_SEC );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_HOUR );

                CHECK_EQUAL( ts1, ts2 );
            }


            //days
            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::HOURS_PER_DAY, TimeSpan::TSUNIT_HOUR );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_DAY );

                CHECK_EQUAL( ts1, ts2 );
            }

            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::MINUTES_PER_DAY, TimeSpan::TSUNIT_MIN );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_DAY );

                CHECK_EQUAL( ts1, ts2 );
            }

            {
                TimeSpan ts1;
                ts1.fromNumeric( TimeSpan::SECONDS_PER_DAY, TimeSpan::TSUNIT_SEC );

                TimeSpan ts2;
                ts2.fromNumeric( 1, TimeSpan::TSUNIT_DAY );

                CHECK_EQUAL( ts1, ts2 );
            }

        }
        catch ( basar::cmnutil::DateTimeException& )
        {
            bool expThrown = true;

            CHECK( expThrown );
        }
        catch ( basar::cmnutil::BasarNotImplementedException& )
        {
            bool expThrown = true;

            CHECK( expThrown );
        }
    }

    TEST( TimeSpanToNumeric )
    {
        TimeSpan ts1;

        ts1.fromNormalizedString( "123456 23:45:32.123" );

        TimeSpan::Numeric numTS = ts1.toNumeric();

        CHECK_EQUAL( 123456,    numTS.days );
        CHECK_EQUAL( 23,        numTS.hours );
        CHECK_EQUAL( 45,        numTS.minutes );
        CHECK_EQUAL( 32,        numTS.seconds );
        CHECK_EQUAL( 123,       numTS.fraction );
    }

    TEST( TimeSpanToNumericMaxValue )
    {
        basar::TimeSpan ts;

        ts.fromNormalizedString( "999999999 23:59:59.998" );

        TimeSpan::Numeric numTS = ts.toNumeric();

        CHECK_EQUAL( 999999999,    numTS.days );
        CHECK_EQUAL( 23,        numTS.hours );
        CHECK_EQUAL( 59,        numTS.minutes );
        CHECK_EQUAL( 59,        numTS.seconds );
        CHECK_EQUAL( 998,       numTS.fraction );
    }

    TEST( TimeSpanToNumericZeroValue )
    {
        basar::TimeSpan ts;

        ts.fromNormalizedString( "0 00:00:00.000" );

        TimeSpan::Numeric numTS = ts.toNumeric();

        CHECK_EQUAL( 0, numTS.days );
        CHECK_EQUAL( 0, numTS.hours );
        CHECK_EQUAL( 0, numTS.minutes );
        CHECK_EQUAL( 0, numTS.seconds );
        CHECK_EQUAL( 0, numTS.fraction );

    }

    TEST( TimeSpanFromNumericStruct )
    {
        TimeSpan::Numeric num;

        num.days = 123455;
        num.hours = 47;
        num.minutes = 45;
        num.seconds = 32;
        num.fraction = 123;

        TimeSpan ts1;

        ts1.fromNumeric( num );

        CHECK_EQUAL( "123456 23:45:32.123", ts1.toNormalizedString() );
    }

    TEST( TimeSpanFromNumericMaxTSValue )
    {
        TimeSpan::Numeric num;

        num.days = 999999999;
        num.hours = 23;
        num.minutes = 59;
        num.seconds = 59;
        num.fraction = 998;

        TimeSpan ts1;

        ts1.fromNumeric( num );

        CHECK_EQUAL( "999999999 23:59:59.998", ts1.toNormalizedString() );
    }

    TEST( NegativeTimeSpanToNumeric )
    {
        TimeSpan ts;

        ts.fromNormalizedString( "-23 21:43:34.167" );

        TimeSpan::Numeric numTS = ts.toNumeric();

        CHECK_EQUAL( 23, numTS.days );
        CHECK_EQUAL( 21, numTS.hours );
        CHECK_EQUAL( 43, numTS.minutes );
        CHECK_EQUAL( 34, numTS.seconds );
        CHECK_EQUAL( 167, numTS.fraction );
        CHECK_EQUAL( true, numTS.isNegative );

        TimeSpan ts2;

        ts2.fromNumeric( numTS );

        CHECK_EQUAL( ts, ts2 );

    }



}// SUITE
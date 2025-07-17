#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"



using basar::RuntimeException;
using basar::cmnutil::DateTimeException;
using basar::Int16;
using basar::Int32;
using basar::ConstString;
using basar::cmnutil::Date;
using basar::cmnutil::Locale;

//------------------------------------------------------------------------------
SUITE( TestDate )
{

    struct DateFixture
    {
        static ::tm* getNow()
        {
            time_t calTime;

            ::time( &calTime );
            return ::localtime( &calTime );
        };
    };

    TEST_FIXTURE( DateFixture, statics )
    {
        CHECK( Date::isLeapYear( 2008 ) );
        CHECK( !Date::isLeapYear( 2007 ) );
        CHECK( !Date::isLeapYear( 2100 ) );
        CHECK( Date::isLeapYear( 2000 ) );
        CHECK( Date::isLeapYear( 1600 ) );
        CHECK( !Date::isLeapYear( 1700 ) );
        CHECK( Date::isLeapYear( 1604 ) );
        CHECK( !Date::isLeapYear( 1603 ) );


        CHECK_EQUAL( 20080322, Date::getFstSprMoon( 2008 ).getDate() );
        CHECK_EQUAL( 20070402, Date::getFstSprMoon( 2007 ).getDate() );
        CHECK_EQUAL( 20060413, Date::getFstSprMoon( 2006 ).getDate() );
        CHECK_EQUAL( 20050325, Date::getFstSprMoon( 2005 ).getDate() );
        CHECK_EQUAL( 20040405, Date::getFstSprMoon( 2004 ).getDate() );
        CHECK_EQUAL( 20030416, Date::getFstSprMoon( 2003 ).getDate() );
        CHECK_EQUAL( 20080323, Date::getEasterSunday( 2008 ).getDate() );
        CHECK_EQUAL( 20070408, Date::getEasterSunday( 2007 ).getDate() );
        CHECK_EQUAL( 20060416, Date::getEasterSunday( 2006 ).getDate() );
        CHECK_EQUAL( 20050327, Date::getEasterSunday( 2005 ).getDate() );
        CHECK_EQUAL( 20040411, Date::getEasterSunday( 2004 ).getDate() );
        CHECK_EQUAL( 20030420, Date::getEasterSunday( 2003 ).getDate() );

        CHECK_EQUAL( DateFixture::getNow()->tm_year + 1900, Date::getCurrent().getYear () );
        CHECK_EQUAL( DateFixture::getNow()->tm_mon  +    1, Date::getCurrent().getMonth() );
        CHECK_EQUAL( DateFixture::getNow()->tm_mday       , Date::getCurrent().getDay  () );


        CHECK_THROW( Date::getDaysOfMonth( 2007,  0 ), DateTimeException );
        CHECK_THROW( Date::getDaysOfMonth( 2007, 13 ), DateTimeException );

        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 1 ) );
        CHECK_EQUAL( 28, Date::getDaysOfMonth( 2007, 2 ) );
        CHECK_EQUAL( 29, Date::getDaysOfMonth( 2008, 2 ) );
        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 3 ) );

        CHECK_EQUAL( 30, Date::getDaysOfMonth( 2007, 4 ) );
        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 5 ) );
        CHECK_EQUAL( 30, Date::getDaysOfMonth( 2007, 6 ) );

        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 7 ) );
        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 8 ) );
        CHECK_EQUAL( 30, Date::getDaysOfMonth( 2007, 9 ) );

        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 10 ) );
        CHECK_EQUAL( 30, Date::getDaysOfMonth( 2007, 11 ) );
        CHECK_EQUAL( 31, Date::getDaysOfMonth( 2007, 12 ) );
    }


    TEST_FIXTURE( DateFixture, Ctor )
    {
        CHECK_EQUAL( DateFixture::getNow()->tm_year + 1900, Date().getYear () );
        CHECK_EQUAL( DateFixture::getNow()->tm_mon  +    1, Date().getMonth() );
        CHECK_EQUAL( DateFixture::getNow()->tm_mday       , Date().getDay  () );

        CHECK_EQUAL( 19991028       , Date( 19991028    ).getDate() );
        CHECK_EQUAL( 19230908       , Date( 19230908    ).getDate() );
        CHECK_EQUAL( Date::NULL_DATE, Date( 0           ).getDate() );
        CHECK_EQUAL( 19451230       , Date( 1945, 12, 30 ).getDate() );
    }

    TEST( get )
    {
        CHECK_EQUAL( DateFixture::getNow()->tm_year + 1900, Date().getYear () );
        CHECK_EQUAL( DateFixture::getNow()->tm_mon  +    1, Date().getMonth() );
        CHECK_EQUAL( DateFixture::getNow()->tm_mday       , Date().getDay  () );

        CHECK_EQUAL( Date( DateFixture::getNow()->tm_year + 1900,
                           DateFixture::getNow()->tm_mon  +    1,
                           DateFixture::getNow()->tm_mday       ).getDate(), Date().getDate() );
    }

    TEST_FIXTURE( DateFixture, calculate )
    {
        Date d1,
             d2( 20070328 ),
             d3( 20070101 ),
             d4( 20071231 ),
             d5( 20071224 ),
             d6( 20080328 );

        CHECK_EQUAL( DateFixture::getNow()->tm_wday, d1.getDayOfWeek() );

        CHECK_EQUAL( 3, d2.getDayOfWeek() );
        CHECK_EQUAL( 1, d3.getDayOfWeek() );
        CHECK_EQUAL( 1, d4.getDayOfWeek() );
        CHECK_EQUAL( 1, d5.getDayOfWeek() );

        CHECK_EQUAL( Date::MONDAY   , Date( 2007, 12, 31 ).getDayOfWeek() );

        CHECK_EQUAL( Date::TUESDAY  , Date( 2008,  1,  1 ).getDayOfWeek() );
        CHECK_EQUAL( Date::THURSDAY , Date( 2008,  2, 28 ).getDayOfWeek() );
        CHECK_EQUAL( Date::FRIDAY   , Date( 2008,  2, 29 ).getDayOfWeek() );
        CHECK_EQUAL( Date::SATURDAY , Date( 2008,  3,  1 ).getDayOfWeek() );

        CHECK_EQUAL( Date::WEDNESDAY, Date( 2008, 12, 31 ).getDayOfWeek() );
        CHECK_EQUAL( Date::THURSDAY , Date( 2009,  1,  1 ).getDayOfWeek() );
    }

    TEST_FIXTURE( DateFixture, getDayOfYear )
    {
        CHECK_EQUAL( DateFixture::getNow()->tm_yday, Date().getDayOfYear() );

        CHECK_EQUAL( 86, Date( 20070328 ).getDayOfYear() );
        CHECK_EQUAL(  0, Date( 20070101 ).getDayOfYear() );
        CHECK_EQUAL( 364, Date( 20071231 ).getDayOfYear() );
        CHECK_EQUAL( 357, Date( 20071224 ).getDayOfYear() );
        CHECK_EQUAL( 87, Date( 20080328 ).getDayOfYear() );

        CHECK_EQUAL( 0  , Date( 20000101 ).getDayOfYear() );
        CHECK_EQUAL( 365, Date( 20001231 ).getDayOfYear() );
        CHECK_EQUAL( 0  , Date( 20010101 ).getDayOfYear() );
        CHECK_EQUAL( 364, Date( 20011231 ).getDayOfYear() );
        CHECK_EQUAL( 0  , Date( 20020101 ).getDayOfYear() );
        CHECK_EQUAL( 364, Date( 20021231 ).getDayOfYear() );
        CHECK_EQUAL( 0  , Date( 20040101 ).getDayOfYear() );
        CHECK_EQUAL( 365, Date( 20041231 ).getDayOfYear() );
        CHECK_EQUAL( 0  , Date( 20080101 ).getDayOfYear() );
        CHECK_EQUAL( 365, Date( 20081231 ).getDayOfYear() );
        CHECK_EQUAL( 0  , Date( 20100101 ).getDayOfYear() );
        CHECK_EQUAL( 364, Date( 20101231 ).getDayOfYear() );
        CHECK_EQUAL( 364, Date( 21001231 ).getDayOfYear() );
    }

    TEST( getWeekOfYear )
    {
        // see http://www.phys.uu.nl/~vgent/calendar/isocalendar.htm: check the 15 possible ISO calendars

        Int16 chkyear;

        // 1.)
        chkyear = 2006;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 2.)
        chkyear = 2011;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 3.)
        chkyear = 2005;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 4.)
        chkyear = 2010;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 5.)
        chkyear = 2009;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 6.)
        chkyear = 2003;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 7.)
        chkyear = 2002;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 8.)
        chkyear = 2007;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 9.)
        chkyear = 2012;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 10.)
        chkyear = 2000;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 11.)
        chkyear = 2016;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 51, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 12.)
        chkyear = 2004;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 13.)
        chkyear = 2020;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 53, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 14.)
        chkyear = 2008;

        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );

        // 15.)
        chkyear = 2024;

        CHECK_EQUAL( 51, Date( chkyear - 1, 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear - 1, 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    ,  1,  7 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  8 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1,  9 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 10 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear    ,  1, 11 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 24 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 25 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 26 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 27 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 28 ).getWeekOfYear() );
        CHECK_EQUAL( 52, Date( chkyear    , 12, 29 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 30 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear    , 12, 31 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  1 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  2 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  3 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  4 ).getWeekOfYear() );
        CHECK_EQUAL( 1, Date( chkyear + 1,  1,  5 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  6 ).getWeekOfYear() );
        CHECK_EQUAL( 2, Date( chkyear + 1,  1,  7 ).getWeekOfYear() );


        CHECK_EQUAL( 13, Date( 20070328 ).getWeekOfYear() );
    }

    TEST( set )
    {
        Date d1;

        d1.setDate( 20000101 );
        CHECK_EQUAL( 20000101, d1.getDate() );

        d1.setDate( 1978, 1, 23 );
        CHECK_EQUAL( 19780123, d1.getDate() );
    }

    TEST( setInvalid )
    {
        Date d1;
        Date d2( 20051231 );

        d1.setInvalid();

        CHECK( !d1.isValid() );
        CHECK_EQUAL( Date::NULL_DATE, d1.getDate() );

        d2.setInvalid();

        CHECK( !d2.isValid() );
        CHECK_EQUAL( Date::NULL_DATE, d2.getDate() );
    }

    TEST_FIXTURE( DateFixture, isValid )
    {
        Date d1;
        Date d2( 0 );
        Date d3( -5 );
        Date d4( 20051231 );


        CHECK( d1.isValid() );
        CHECK_EQUAL( Date( DateFixture::getNow()->tm_year + 1900,
                           DateFixture::getNow()->tm_mon  +    1,
                           DateFixture::getNow()->tm_mday       ).getDate(), d1.getDate() );

        CHECK( !d2.isValid() );
        CHECK_EQUAL( Date::NULL_DATE, d2.getDate() );

        CHECK( !d3.isValid() );
        CHECK_EQUAL( Date::NULL_DATE, d3.getDate() );

        CHECK( d4.isValid() );
        CHECK_EQUAL( 20051231, d4.getDate() );
    }

    TEST( addDays )
    {
        Date  d1( 20071231 );
        Int32 add[] =
        {
            1,
            -1,
            366,
            -366,
            -366
        };

        d1.addDays( add[0] );
        CHECK_EQUAL( 20080101, d1.getDate() );

        d1.addDays( add[1] );
        CHECK_EQUAL( 20071231, d1.getDate() );

        d1.addDays( add[2] );
        CHECK_EQUAL( 20081231, d1.getDate() );

        d1.addDays( add[3] );
        CHECK_EQUAL( 20071231, d1.getDate() );

        d1.addDays( add[4] );
        CHECK_EQUAL( 20061230, d1.getDate() );
    }

//------------------------------------------------------------------------------
TEST(addMonths)
{
    Date d1(20071231);

    d1.addMonths( 1 );

    CHECK_EQUAL( 20080131, d1.getDate() );

    d1.addMonths( 1 );
    CHECK_EQUAL( 20080229, d1.getDate() );

    d1.addMonths( 4 );
    CHECK_EQUAL( 20080629, d1.getDate() );

    Date d2(20071231);
    d2.addMonths( 6 );
    CHECK_EQUAL( 20080630, d2.getDate() );
}
//------------------------------------------------------------------------------
TEST(addYears)
{
    {
        Date d1(20071231);

        d1.addYears( 1 );

        CHECK_EQUAL( 20081231, d1.getDate() );
    }

    {
        Date d1( 20081231);

        d1.addYears( -1 );

        CHECK_EQUAL( 20071231, d1.getDate() );
    }

    {
        Date d1(20160229);

        d1.addYears( 1 );

        CHECK_EQUAL( 20170228, d1.getDate() );
    }

    {
        Date d1(20170228);

        d1.addYears( -1 );

        CHECK_EQUAL( 20160228, d1.getDate() );
    }
}

//------------------------------------------------------------------------------
TEST( incrementsDecrements )
{
    {
        Date d1(20160531);

        ++d1;
        ++d1;

        CHECK_EQUAL( 20160602, d1.getDate() );
    }

    {
        Date d1(20151231);

        Date d_inc = d1++;

        CHECK_EQUAL( 20151231, d_inc.getDate() );

        CHECK_EQUAL( 20160101, d1.getDate() );
    }

    {
        Date d1(20160302);

        --d1;
        --d1;

        CHECK_EQUAL( 20160229, d1.getDate() );
    }

    {
        Date d1(20160101);

        Date d_dec = d1--;

        CHECK_EQUAL( 20160101, d_dec.getDate() );

        CHECK_EQUAL( 20151231, d1.getDate() );
    }
}

//------------------------------------------------------------------------------

    TEST( toString )
    {
        static ConstString fmt[] =
        {
            "dd ddd",
            "mm mmm",
            "yy yyyy",
            "ww",
            basar::cmnutil::FORMAT_SQL_DATE_INFORMIX
        };

        Date d1( 20070402 ),
             d2( 19901031 ),
             nul;
        nul.setInvalid();

        CHECK_EQUAL( "02 Mon", d1 .toString( fmt[0] ).c_str() );
        CHECK_EQUAL( "31 Wed", d2 .toString( fmt[0] ).c_str() );
        CHECK_EQUAL( "31 Fri", nul.toString( fmt[0] ).c_str() );

        CHECK_EQUAL( "04 Apr", d1 .toString( fmt[1] ).c_str() );
        CHECK_EQUAL( "10 Oct", d2 .toString( fmt[1] ).c_str() );
        CHECK_EQUAL( "12 Dec", nul.toString( fmt[1] ).c_str() );

        CHECK_EQUAL( "07 2007", d1 .toString( fmt[2] ).c_str() );
        CHECK_EQUAL( "90 1990", d2 .toString( fmt[2] ).c_str() );
        CHECK_EQUAL( "99 9999", nul.toString( fmt[2] ).c_str() );

        CHECK_EQUAL( "Mon", d1 .toString( fmt[3] ).c_str() );
        CHECK_EQUAL( "Wed", d2 .toString( fmt[3] ).c_str() );
        CHECK_EQUAL( "Fri", nul.toString( fmt[3] ).c_str() );

        CHECK_EQUAL( "04/02/2007", d1 .toString( fmt[4] ).c_str() );
        CHECK_EQUAL( "10/31/1990", d2 .toString( fmt[4] ).c_str() );
        CHECK_EQUAL( "12/31/9999", nul.toString( fmt[4] ).c_str() );

        for ( Locale::LanguageEnum j = Locale::LOCALE_FIRST;
                j < Locale::LOCALE_NUMBER;
                j = static_cast<Locale::LanguageEnum>( j + 1 ) )
        {
            try
            {
                Locale::setLocale( j );
            }
            catch ( const basar::RuntimeException& )
            {
                continue;
            }

            switch ( j )
            {
                case Locale::C:
                case Locale::ENGLISH_US:
                    CHECK_EQUAL( "10/31/1990"               , d2.toStrDate(                       ).c_str() );
                    CHECK_EQUAL( "Wed, 31 Oct 1990 00:00:00", d2.toString ( "%a, %d %b %Y %H:%M:%S" ).c_str() );
                    break;

                case Locale::ENGLISH_GB:
                case Locale::FRENCH_FR:
                    CHECK_EQUAL( "31/10/1990"               , d2.toStrDate(                       ).c_str() );
                    CHECK_EQUAL( "Wed, 31 Oct 1990 00:00:00", d2.toString ( "%a, %d %b %Y %H:%M:%S" ).c_str() );
                    break;

                default:
                    CHECK_EQUAL( "31.10.1990"               , d2.toStrDate(                       ).c_str() );
                    CHECK_EQUAL( "Wed, 31 Oct 1990 00:00:00", d2.toString ( "%a, %d %b %Y %H:%M:%S" ).c_str() );
                    break;
            }
        }

        try
        {
            Locale::setLocale( Locale::C );

            CHECK_EQUAL( "12/31/9999"               , nul.toStrDate(                       ).c_str() );
            CHECK_EQUAL( "Fri, 31 Dec 9999 00:00:00", nul.toString ( "%a, %d %b %Y %H:%M:%S" ).c_str() );

            Date now;
            CHECK_EQUAL( "10/31/1990"               , d2 .toString( basar::cmnutil::FORMAT_SQL_DATE_INFORMIX ).c_str() );
        }
        catch ( const RuntimeException& )
        {
        }
    }

    TEST( StreamOutput )
    {
        std::ostringstream os;

        Date d2( 19901031 );

        for ( Locale::LanguageEnum j = Locale::LOCALE_FIRST;
                j < Locale::LOCALE_NUMBER;
                j = static_cast<Locale::LanguageEnum>( j + 1 ) )
        {
            try
            {
                Locale::setLocale( j );
            }
            catch ( const basar::RuntimeException& )
            {
                continue;
            }

            switch ( j )
            {
                case Locale::C:
                case Locale::ENGLISH_US:
                    os << d2;
                    CHECK_EQUAL( "10/31/1990", os.str() );
                    os.str( "" );
                    break;

                case Locale::ENGLISH_GB:
                case Locale::FRENCH_FR:
                    os << d2;
                    CHECK_EQUAL( "31/10/1990", os.str() );
                    os.str( "" );
                    break;

                default:
                    os << d2;
                    CHECK_EQUAL( "31.10.1990", os.str() );
                    os.str( "" );
                    break;
            }
        }
    }

    TEST( fromString )
    {
        Date d;

        for ( Locale::LanguageEnum j = Locale::LOCALE_FIRST;
                j < Locale::LOCALE_NUMBER;
                j = static_cast<Locale::LanguageEnum>( j + 1 ) )
        {
            try
            {
                Locale::setLocale( j );
            }
            catch ( const basar::RuntimeException& )
            {
                continue;
            }

            d.fromString( "yyyy-mm-dd", "2007-12-24" );
            CHECK_EQUAL( 20071224, d.getDate() );


            switch ( j )
            {
                case Locale::C:
                case Locale::ENGLISH_US:
                    d.fromStrDate( "03/13/2007" );
                    CHECK_EQUAL( 20070313, d.getDate() );
                    break;

                case Locale::ENGLISH_GB:
                case Locale::FRENCH_FR:
                    d.fromStrDate( "13/03/2007" );
                    CHECK_EQUAL( 20070313, d.getDate() );
                    break;

                default:
                    d.fromStrDate( "13.03.2007" );
                    CHECK_EQUAL( 20070313, d.getDate() );
                    break;
            }
        }
    }

    TEST( fromEmptyString )
    {
        Date d;

        for ( Locale::LanguageEnum j = Locale::LOCALE_FIRST;
                j < Locale::LOCALE_NUMBER;
                j = static_cast<Locale::LanguageEnum>( j + 1 ) )
        {
            try
            {
                Locale::setLocale( j );
            }
            catch ( const basar::RuntimeException& )
            {
                continue;
            }


            d.fromString( "yyyy-mm-dd", "" );

            CHECK( !( d.isValid() ) );
        }
    }

    TEST( compare )
    {
        Date d1;
        Date d2( 1289, 12, 23 );
        Date d3( d2 );
        Date d4( 1289, 12, 23 );

        CHECK( !( d1 == d2 ) );
        CHECK( d2 == d3 );
        CHECK( !( d1 == d3 ) );
        CHECK( d4 == d2 );
    }

    TEST( lessThan )
    {
        Date d1;
        Date d2( 1289, 12, 23 );
        Date d3( d2 );
        Date d4( 1289, 12, 23 );
        Date d5( 1289, 12, 24 );
        Date d6( 1289, 12, 22 );

        CHECK( !( d1 < d2 ) );
        CHECK( !( d2 < d3 ) );
        CHECK( !( d1 < d3 ) );
        CHECK( !( d4 < d2 ) );

        CHECK( d2 < d5 );
        CHECK( !( d2 < d6 ) );
    }

    TEST( lessThanOrEqual )
    {
        Date d1;
        Date d2( 1289, 12, 23 );
        Date d3( d2 );
        Date d4( 1289, 12, 23 );
        Date d5( 1289, 12, 24 );
        Date d6( 1289, 12, 22 );

        CHECK( !( d1 <= d2 ) );
        CHECK( d2 <= d3 );
        CHECK( !( d1 <= d3 ) );
        CHECK(  d4 <= d2 );

        CHECK( d2 <= d5 );
        CHECK( !( d2 <= d6 ) );
    }

    TEST( greaterThan )
    {
        Date d1;
        Date d2( 1289, 12, 23 );
        Date d3( d2 );
        Date d4( 1289, 12, 23 );
        Date d5( 1289, 12, 24 );
        Date d6( 1289, 12, 22 );

        CHECK(  d1 > d2 );
        CHECK( !( d2 > d3 ) );
        CHECK(  d1 > d3 );
        CHECK( !( d4 > d2 ) );

        CHECK( !( d2 > d5 ) );
        CHECK(  d2 > d6 );
    }

    TEST( greaterThanOrEqual )
    {
        Date d1;
        Date d2( 1289, 12, 23 );
        Date d3( d2 );
        Date d4( 1289, 12, 23 );
        Date d5( 1289, 12, 24 );
        Date d6( 1289, 12, 22 );

        CHECK(  d1 >= d2 );
        CHECK(  d2 >= d3 );
        CHECK(  d1 >= d3 );
        CHECK( d4 >= d2 );

        CHECK( !( d2 >= d5 ) );
        CHECK(  d2 >= d6 );
    }
//------------------------------------------------------------------------------
}   // end TESTSUITE

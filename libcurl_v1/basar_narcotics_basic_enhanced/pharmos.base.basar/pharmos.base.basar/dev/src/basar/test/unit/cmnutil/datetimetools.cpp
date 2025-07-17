#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

using basar::Int16;
using basar::Int32;
using basar::cmnutil::DateTimeTools;

//------------------------------------------------------------------------------
SUITE(TestDateTimeTools)
{

struct Fixture
{
	Fixture	() : date(20061231), time(235933591), 
				 y   (2006)    , m   (12)       , d   (31),
				 h   (23)      , mnt (59)       , sec (33),  ms  (591)
			{};

	Int32 date;
	Int32 time;

	Int16 y, m, d;
	Int16 h, mnt, sec, ms;
};

TEST_FIXTURE(Fixture, GetComponent)
{
	CHECK_EQUAL(y, DateTimeTools::getYear  (date));
	CHECK_EQUAL(m, DateTimeTools::getMonth (date));
	CHECK_EQUAL(d, DateTimeTools::getDay   (date));

	CHECK_EQUAL(h  , DateTimeTools::getHour  (time));
	CHECK_EQUAL(mnt, DateTimeTools::getMinute(time));
	CHECK_EQUAL(sec, DateTimeTools::getSec   (time));
	CHECK_EQUAL(ms , DateTimeTools::getMSec  (time));
}

TEST_FIXTURE(Fixture, GetInteger)
{
	CHECK_EQUAL(date, DateTimeTools::getDate(y, m, d));
	CHECK_EQUAL(time, DateTimeTools::getTime(h, mnt, sec, ms));
}

TEST_FIXTURE(Fixture, ValidComponent)
{
	CHECK(DateTimeTools::isValidDate(y, m, d));
	CHECK(DateTimeTools::isValidDate(9999, m, d));
	CHECK(DateTimeTools::isValidDate(2004, 2, 29));

	CHECK(!DateTimeTools::isValidDate(0, m, d));
	CHECK(!DateTimeTools::isValidDate(-5, m, d));
	CHECK(!DateTimeTools::isValidDate(10000, m, d));
	CHECK(!DateTimeTools::isValidDate(y, 0, d));
	CHECK(!DateTimeTools::isValidDate(y, m, 0));
	CHECK(!DateTimeTools::isValidDate(y, 2, 29));

	CHECK(DateTimeTools::isValidTime(h, mnt, sec, ms));

	CHECK(!DateTimeTools::isValidTime(40, mnt, sec, ms));
	CHECK(!DateTimeTools::isValidTime(h, 60, sec, ms));
	CHECK(!DateTimeTools::isValidTime(h, mnt, -1, 7999));
}

TEST_FIXTURE(Fixture, ValidInteger)
{
	CHECK(DateTimeTools::isValidDate(date));
	CHECK(DateTimeTools::isValidTime(time));

	CHECK(DateTimeTools::isValidTime(0));
	CHECK(DateTimeTools::isValidTime(1));

	CHECK(!DateTimeTools::isValidDate(date + 2000));
	CHECK(!DateTimeTools::isValidDate(basar::DateTime::NULL_DATE));
	CHECK(!DateTimeTools::isValidDate(-1));
	CHECK(!DateTimeTools::isValidDate(1));

	CHECK(!DateTimeTools::isValidTime(basar::DateTime::NULL_TIME));
}

TEST(GetOldNewStyles)
{
	static const Int32 _new = 190123345;
	static const Int32 _old = 131745;

	CHECK_EQUAL(131745000, DateTimeTools::getNewTime(_old));
	CHECK_EQUAL(190123   , DateTimeTools::getOldTime(_new));
}

TEST(GetTotal)
{
	CHECK_EQUAL(       0, DateTimeTools::getTotalMSec(        0));
	CHECK_EQUAL(       1, DateTimeTools::getTotalMSec(1        ));
	CHECK_EQUAL(86399999, DateTimeTools::getTotalMSec(235959999));
	CHECK_EQUAL(86400000, DateTimeTools::getTotalMSec(240000000));
	CHECK_EQUAL(96400000, DateTimeTools::getTotalMSec(264640000));
	CHECK_EQUAL( 3600000, DateTimeTools::getTotalMSec( 10000000));
	CHECK_EQUAL(   60000, DateTimeTools::getTotalMSec(   100000));

	CHECK_EQUAL(        0, DateTimeTools::getTimeByTotalMSec(       0));
	CHECK_EQUAL(        1, DateTimeTools::getTimeByTotalMSec(       1));
	CHECK_EQUAL(235959999, DateTimeTools::getTimeByTotalMSec(86399999));
	CHECK_EQUAL(240000000, DateTimeTools::getTimeByTotalMSec(86400000));
	CHECK_EQUAL(264640000, DateTimeTools::getTimeByTotalMSec(96400000));
	CHECK_EQUAL( 10000000, DateTimeTools::getTimeByTotalMSec( 3600000));
	CHECK_EQUAL(   100000, DateTimeTools::getTimeByTotalMSec(   60000));
}

TEST(DateTimeFromUnixTimestamp)
{
    {
        basar::cmnutil::DateTime t = DateTimeTools::getDateTimeFromUnixTimestamp(1421228823);

        CHECK_EQUAL(20150114, t.getDate());
        CHECK_EQUAL(104703000, t.getTime());
    }

    {
        basar::cmnutil::DateTime t = DateTimeTools::getDateTimeFromUnixTimestamp(0);
        CHECK_EQUAL(19700101, t.getDate());
        CHECK_EQUAL(10000000, t.getTime());
    }
}

TEST(DateTimeToUnixTimestamp)
{
    {
        basar::DateTime epoch( 19700101, 0 );

        basar::Int32 zero = DateTimeTools::getUnixTimestampFromDateTime( epoch );

        CHECK_EQUAL( 3600, zero );
    }

    {
        basar::DateTime d( 20150114, 104703000 );

        basar::Int32 unixTimeStamp = DateTimeTools::getUnixTimestampFromDateTime( d );

        CHECK_EQUAL( 1421228823, unixTimeStamp );
    }

    {
        basar::DateTime d( 20170425, 144856000);

        basar::Int32 unixTimeStamp = DateTimeTools::getUnixTimestampFromDateTime( d );

        CHECK_EQUAL(1493124536, unixTimeStamp);
    }
}

//------------------------------------------------------------------------------
}	// end TESTSUITE

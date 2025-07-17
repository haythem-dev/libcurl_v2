#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"


using basar::TimeSpan;
using basar::Float64;
using basar::Float32;
using basar::DateTime;
using basar::cmnutil::Locale;


SUITE(TestDateTimeTimeSpan)
{

    TEST(AddTimeSpanToDateTime)
    {
        DateTime dt(20130513, 120423000);

        TimeSpan ts;

        ts.fromNormalizedString("2 06:12:45.000");

        DateTime res = dt + ts;

        //request german locale for string equality
        Locale::setLocale(Locale::GERMAN_DE);

        CHECK_EQUAL("15.05.2013 18:17:08.000", res.toStrDateTime());
    }

    TEST(AddTimeSpanToDateTime_DSTChange)
    {
        //request german locale for string equality
        Locale::setLocale(Locale::GERMAN_DE);

        DateTime dt(20170325, 120423000);

        CHECK_EQUAL(false, dt.isDST());

        TimeSpan ts;
        ts.fromNumeric(23, TimeSpan::TSUNIT_HOUR); //don't use 24 hours as 24 hours = 1 day

        DateTime res = ts + dt;

        CHECK_EQUAL(true, res.isDST());

        CHECK_EQUAL("26.03.2017 11:04:23.000", res.toStrDateTime());
    }

    TEST(SubTwoDateTimes)
    {
        DateTime dt1(20130526, 124334123);
        DateTime dt2(20130530, 124334123);

        TimeSpan ts = dt2 - dt1;

        CHECK_EQUAL("04 00:00:00.000", ts.toNormalizedString());
    }

    TEST(SubTimeSpanFromDateTime)
    {
        TimeSpan ts;

        ts.fromNormalizedString("2 06:12:45.000");

        DateTime dt(20130515, 181708000);

        DateTime res = dt - ts;

        //request german locale for string equality
        Locale::setLocale(Locale::GERMAN_DE);

        CHECK_EQUAL("13.05.2013 12:04:23.000", res.toStrDateTime());

    }

    TEST(DateTimeTimeSpanOperators)
    {
        //request german locale for string equality
        Locale::setLocale(Locale::GERMAN_DE);

        const DateTime dt1(20130526, 124334123);
        const DateTime dt2(20130530, 124334123);

        const TimeSpan ts = dt2 - dt1;

        CHECK_EQUAL("04 00:00:00.000", ts.toNormalizedString());

        {
            DateTime res(dt1);

            res += ts;

            CHECK_EQUAL(dt2, res);
        }

        {
            DateTime res1 = dt1 + ts;
            DateTime res2 = ts + dt1;

            CHECK_EQUAL(res1, res2);
        }

        {
            DateTime res(dt2);

            res -= ts;

            CHECK_EQUAL(dt1, res);
        }

        {
            DateTime res;

            res = dt2 - ts;

            CHECK_EQUAL(dt1, res);
        }

    }

}
#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#endif

#include <stdlib.h>
#include <sys/timeb.h>

using basar::RuntimeException;
using basar::Int16;
using basar::Int32;
using basar::UInt32;
using basar::ConstString;
using basar::DateTime;
using basar::Date;
using basar::Time;
using basar::cmnutil::Locale;

//------------------------------------------------------------------------------
SUITE(TestDateTime)
{

static void mysleep(Int32 milliSec)
{
#ifdef _WIN32
	::Sleep(milliSec);
#else
	struct timeval waitTime;

	waitTime.tv_sec  = milliSec / 1000;
	waitTime.tv_usec = (milliSec % 1000) * 1000;

	select(0, 0, 0, 0, &waitTime);	 
#endif
}

//------------------------------------------------------------------------------
struct Fixture
{
	Fixture() : pTmNow(getNow())	{};

	static ::tm * getNow()
	{
		time_t calTime;

		::time(&calTime);
		return ::localtime(&calTime);
	};

	tm * pTmNow;
};

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, statics)
{
	DateTime now = DateTime::getCurrent();

	CHECK_EQUAL(pTmNow->tm_year + 1900, now.getYear  ());
	CHECK_EQUAL(pTmNow->tm_mon  +    1, now.getMonth ());
	CHECK_EQUAL(pTmNow->tm_mday       , now.getDay   ());
	CHECK_EQUAL(pTmNow->tm_hour       , now.getHour  ());
	CHECK_EQUAL(pTmNow->tm_min        , now.getMinute());
	CHECK_EQUAL(pTmNow->tm_sec        , now.getSec   ());
	CHECK(0   <= now.getMSec());
	CHECK(1000 > now.getMSec());

	timeb       tb;
	DateTime    d1, d2;
	Int32       t1, t2, diff;
	const Int32 TOLERANCE = 100;	// msec

	for (Int32 i = 0; i < 6; ++i)
	{
		now = DateTime::getCurrent();
		ftime(&tb);

		CHECK(0   <= now.getMSec());
		CHECK(1000 > now.getMSec());

		diff = tb.millitm - now.getMSec();
		CHECK( (::abs(diff) <= TOLERANCE) || (::abs(diff) >= (1000 - TOLERANCE)));	// e.g (1, 3) or (999, 1) msec

		// ---

		d1 = DateTime::getCurrent();
		mysleep(35);
		d2 = DateTime::getCurrent();

		t1 = d1.getTime();
		t2 = d2.getTime();
		CHECK(t2 > t1);

		d1.setTime(t2);
		d1.addTime(1);	
		t1 = d1.getTime();
		CHECK(t1 >= t2);
	}
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, Ctor)
{
	DateTime now;

	CHECK_EQUAL(pTmNow->tm_year + 1900, now.getYear  ());
	CHECK_EQUAL(pTmNow->tm_mon  +    1, now.getMonth ());
	CHECK_EQUAL(pTmNow->tm_mday       , now.getDay   ());
	CHECK_EQUAL(pTmNow->tm_hour       , now.getHour  ());
	CHECK_EQUAL(pTmNow->tm_min        , now.getMinute());
	CHECK_EQUAL(pTmNow->tm_sec        , now.getSec   ());
	CHECK(0   <= now.getMSec());
	CHECK(1000 > now.getMSec());

	CHECK_EQUAL(19991028           , DateTime(19991028           ).getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, DateTime(19991028           ).getTime());
	CHECK_EQUAL(19230908           , DateTime(19230908, 134523432).getDate());
	CHECK_EQUAL(134523432          , DateTime(19230908, 134523432).getTime());
	CHECK_EQUAL(134523000          , DateTime(19230908, 134523000).getTime());
	CHECK_EQUAL(20080206           , DateTime(20080206, 170843000).getDate());
	CHECK_EQUAL(170843000          , DateTime(20080206, 170843000).getTime());
	CHECK_EQUAL(DateTime::NULL_DATE, DateTime(0       ,  92345678).getDate());
	CHECK_EQUAL(92345678           , DateTime(0       ,  92345678).getTime());
	CHECK_EQUAL(19451230           , DateTime(1945, 12, 30       ).getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, DateTime(1945, 12, 30       ).getTime());
	CHECK_EQUAL(DateTime::NULL_DATE, DateTime(23, 45, 15, 654    ).getDate());
	CHECK_EQUAL(234515654          , DateTime(23, 45, 15, 654    ).getTime());
}

//------------------------------------------------------------------------------

TEST(copy_ctor)
{
    {
        DateTime dt1(20250415);

        DateTime dt2(dt1);

        CHECK_EQUAL(20250415, dt2.getDate());
    }
    {
        DateTime dt1(20250415);

        DateTime* p_dt2 = 0;

        CHECK(0 == p_dt2);

        p_dt2 = new DateTime(dt1);
        
        CHECK_EQUAL(p_dt2->getDate(), 20250415);

        delete p_dt2;
    }
    {
        DateTime dt1(20130723, 235959999);

        CHECK(dt1.isValid());

        DateTime dt2(dt1);

        CHECK(dt2.isValid());
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, get)
{
	DateTime now;

	CHECK_EQUAL(pTmNow->tm_year + 1900, now.getYear  ());
	CHECK_EQUAL(pTmNow->tm_mon  +    1, now.getMonth ());
	CHECK_EQUAL(pTmNow->tm_mday       , now.getDay   ());
	CHECK_EQUAL(pTmNow->tm_hour       , now.getHour  ());
	CHECK_EQUAL(pTmNow->tm_min        , now.getMinute());
	CHECK_EQUAL(pTmNow->tm_sec        , now.getSec   ());
	CHECK(0   <= now.getMSec());
	CHECK(1000 > now.getMSec());

	CHECK_EQUAL(DateTime(Fixture::pTmNow->tm_year + 1900,
		                 Fixture::pTmNow->tm_mon  +    1,
			             Fixture::pTmNow->tm_mday       ).getDate(), now.getDate());
	CHECK_CLOSE(DateTime(Fixture::pTmNow->tm_hour,
			             Fixture::pTmNow->tm_min,
		                 Fixture::pTmNow->tm_sec ,
						 0                       ).getTime(), now.getTime(), 1100);


	DateTime d(20070321, 134517123);

	CHECK_EQUAL(2007, d.getYear  ());
	CHECK_EQUAL(   3, d.getMonth ());
	CHECK_EQUAL(  21, d.getDay   ());
	CHECK_EQUAL(  13, d.getHour  ());
	CHECK_EQUAL(  45, d.getMinute());
	CHECK_EQUAL(  17, d.getSec   ());
	CHECK_EQUAL( 123, d.getMSec  ());

	CHECK_EQUAL(DateTime(20070321, 134517123).getDate(), d.getDate());
	CHECK_EQUAL(DateTime(20070321, 134517123).getTime(), d.getTime());
}

//------------------------------------------------------------------------------
TEST(set)
{
	DateTime d1;

	d1.setDate(20001231);
	d1.setTime(30201999);

	CHECK_EQUAL(20001231, d1.getDate());
	CHECK_EQUAL(30201999, d1.getTime());

	d1.setDate(1978, 1, 23);
	d1.setTime(8, 12, 45, 567);
	CHECK_EQUAL(19780123, d1.getDate());
	CHECK_EQUAL(81245567, d1.getTime());

}

//------------------------------------------------------------------------------
TEST(setInvalid)
{
	DateTime d1, d2, d3, d4;

	d1.setInvalid    ();
	d2.setInvalidDate();
	d3.setInvalidTime();

	CHECK(!d1.isValid    ());
	CHECK(!d1.isValidDate());
	CHECK(!d1.isValidTime());
	CHECK_EQUAL(DateTime::NULL_DATE, d1.getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, d1.getTime());

	CHECK(!d2.isValid    ());
	CHECK(!d2.isValidDate());
	CHECK( d2.isValidTime());
	CHECK_EQUAL(DateTime::NULL_DATE, d2.getDate());
	CHECK      (DateTime::NULL_TIME != d2.getTime());

	CHECK(!d3.isValid    ());
	CHECK( d3.isValidDate());
	CHECK(!d3.isValidTime());
	CHECK      (DateTime::NULL_DATE != d3.getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, d3.getTime());

	CHECK(d4.isValid    ());
	CHECK(d4.isValidDate());
	CHECK(d4.isValidTime());
	CHECK(DateTime::NULL_DATE != d4.getDate());
	CHECK(DateTime::NULL_TIME != d4.getTime());
}

//------------------------------------------------------------------------------
TEST(setRange)
{
	DateTime d0 (23451231, 234554123);
	DateTime d1 (d0);
	DateTime d2 (d1);
	DateTime d3 (d2);
	DateTime d4 (d3);

	CHECK_EQUAL(23451231,  d0.getDate());
	CHECK_EQUAL(234554123, d0.getTime());

	d1.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_SEC);
	CHECK_EQUAL(23451231,  d1.getDate());
	CHECK_EQUAL(234554000, d1.getTime());

	d2.setRange(DateTime::DTUNIT_HOUR, DateTime::DTUNIT_SEC);
	CHECK_EQUAL(DateTime::NULL_DATE, d2.getDate());
	CHECK_EQUAL(234554000          , d2.getTime());

	d3.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_DAY);
	CHECK_EQUAL(23451231           , d3.getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, d3.getTime());

	d4.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_MSEC);
	CHECK_EQUAL(23451231 , d4.getDate());
	CHECK_EQUAL(234554123, d4.getTime());
	d4.setInvalid();
	CHECK_EQUAL(DateTime::NULL_DATE, d4.getDate());
	CHECK_EQUAL(DateTime::NULL_TIME, d4.getTime());
}

//------------------------------------------------------------------------------
TEST(isValid)
{
	DateTime d1;
	DateTime d2(0);
	DateTime d3(0, 0);
	DateTime d4(-5);
	DateTime d5(20051231, -6);
	DateTime d6(20051231, 123456789);

	CHECK(d1.isValid    ());
	CHECK(d1.isValidDate());
	CHECK(d1.isValidTime());
	CHECK(DateTime::NULL_DATE != d1.getDate());
	CHECK(DateTime::NULL_TIME != d1.getTime());

	CHECK(!d2.isValid    ());
	CHECK(!d2.isValidDate());
	CHECK(!d2.isValidTime());
	CHECK(DateTime::NULL_DATE == d2.getDate());
	CHECK(DateTime::NULL_TIME == d2.getTime());

	CHECK(!d3.isValid    ());
	CHECK(!d3.isValidDate());
	CHECK( d3.isValidTime());
	CHECK(DateTime::NULL_DATE == d3.getDate());
	CHECK(DateTime::NULL_TIME != d3.getTime());

	CHECK(!d4.isValid    ());
	CHECK(!d4.isValidDate());
	CHECK(!d4.isValidTime());
	CHECK(DateTime::NULL_DATE == d4.getDate());
	CHECK(DateTime::NULL_TIME == d4.getTime());

	CHECK(!d5.isValid    ());
	CHECK(d5.isValidDate());
	CHECK(!d5.isValidTime());
	CHECK(DateTime::NULL_DATE != d5.getDate());
	CHECK(DateTime::NULL_TIME == d5.getTime());

	CHECK(d6.isValid    ());
	CHECK(d6.isValidDate());
	CHECK(d6.isValidTime());
	CHECK(DateTime::NULL_DATE != d6.getDate());
	CHECK(DateTime::NULL_TIME != d6.getTime());

	d1.setInvalidTime();
	CHECK(!d1.isValidTime());

	d5.setInvalidDate();
	CHECK(!d5.isValidDate());

	d6.setInvalid();
	CHECK(!d6.isValid());

	CHECK( ! DateTime(2008, 2, 26).isValid() );
	CHECK( ! DateTime(20080226   ).isValid() );

	{
		DateTime dt(99991231, 235959999);

		CHECK( !dt.isValid    () );
		CHECK( !dt.isValidDate() );
		CHECK( !dt.isValidTime() );		// not the best solution => invalid time should be improved!
	}
}

//------------------------------------------------------------------------------
TEST(addDays)
{
	DateTime  d1(20071231);
	Int32 add[] =  
	{	
		   1,
		  -1,
		 366,
		-366,
		-366
	};

	d1.addDays(add[0]);
	CHECK_EQUAL( 20080101, d1.getDate());

	d1.addDays(add[1]);
	CHECK_EQUAL( 20071231, d1.getDate());

	d1.addDays(add[2]);
	CHECK_EQUAL( 20081231, d1.getDate());

	d1.addDays(add[3]);
	CHECK_EQUAL( 20071231, d1.getDate());

	d1.addDays(add[4]);
	CHECK_EQUAL( 20061230, d1.getDate());
}

//------------------------------------------------------------------------------
TEST(addMonths)
{
    DateTime d1(20071231);

    d1.addMonths( 1 );

    CHECK_EQUAL( 20080131, d1.getDate() );

    d1.addMonths( 1 );
    CHECK_EQUAL( 20080229, d1.getDate() );

    d1.addMonths( 4 );
    CHECK_EQUAL( 20080629, d1.getDate() );

    DateTime d2(20071231);
    d2.addMonths( 6 );
    CHECK_EQUAL( 20080630, d2.getDate() );
}
//------------------------------------------------------------------------------
TEST(addYears)
{
    {
        DateTime d1(20071231);

        d1.addYears( 1 );

        CHECK_EQUAL( 20081231, d1.getDate() );
    }

    {
        DateTime d1( 20081231);

        d1.addYears( -1 );

        CHECK_EQUAL( 20071231, d1.getDate() );
    }

    {
        DateTime d1(20160229);

        d1.addYears( 1 );

        CHECK_EQUAL( 20170228, d1.getDate() );
    }

    {
        DateTime d1(20170228);

        d1.addYears( -1 );

        CHECK_EQUAL( 20160228, d1.getDate() );
    }
}

//------------------------------------------------------------------------------
TEST(addTime)
{
	DateTime d1(DateTime::NULL_DATE, 140000000),
		     d2(20070330           , 140000000);

	DateTime::TimeCalcEnum eFlag;

	eFlag = d1.addTime(  23456789);
	CHECK_EQUAL(DateTime::NULL_DATE, d1.getDate());
	CHECK_EQUAL(163456789          , d1.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );

	eFlag = d1.addTime(-170000000);
	CHECK_EQUAL(DateTime::NULL_DATE , d1.getDate());
	CHECK_EQUAL(233456789           , d1.getTime());
	CHECK_EQUAL(DateTime::DAY_BEFORE, eFlag       );

	eFlag = d1.addTime( 110000000);
	CHECK_EQUAL(DateTime::NULL_DATE, d1.getDate());
	CHECK_EQUAL(103456789          , d1.getTime());
	CHECK_EQUAL(DateTime::DAY_AFTER, eFlag       );

	eFlag = d1.addTime(1333333333);
	CHECK_EQUAL(DateTime::NULL_DATE, d1.getDate());
	CHECK_EQUAL(103456789          , d1.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );

	eFlag = d1.addTime(-500000);
	CHECK_EQUAL(DateTime::NULL_DATE, d1.getDate());
	CHECK_EQUAL(102956789          , d1.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );


	eFlag = d2.addTime(  23456789);
	CHECK_EQUAL(20070330           , d2.getDate());
	CHECK_EQUAL(163456789          , d2.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );

	eFlag = d2.addTime(-170000000);
	CHECK_EQUAL(20070329            , d2.getDate());
	CHECK_EQUAL(233456789           , d2.getTime());
	CHECK_EQUAL(DateTime::DAY_BEFORE, eFlag       );

	eFlag = d2.addTime( 110000000);
	CHECK_EQUAL(20070330           , d2.getDate());
	CHECK_EQUAL(103456789          , d2.getTime());
	CHECK_EQUAL(DateTime::DAY_AFTER, eFlag       );

	eFlag = d2.addTime(1333333333);
	CHECK_EQUAL(20070330           , d2.getDate());
	CHECK_EQUAL(103456789          , d2.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );

	eFlag = d2.addTime(-500000);
	CHECK_EQUAL(20070330           , d2.getDate());
	CHECK_EQUAL(102956789          , d2.getTime());
	CHECK_EQUAL(DateTime::DAY_SAME , eFlag       );
}

//------------------------------------------------------------------------------
TEST(toString)
{
	DateTime d1(19901031, 233456234),
	         d2(20071223,  83412345), 
			 d3(20070511, 155733687),
             d5(20160429,  93612345),
             nul;
	DateTime d4(1812, 10, 23);
	Time     t1(15, 19, 31, 000);
	Time     t2(17, 21, 45, 987);

	nul.setInvalid();
	t1 .setRange(Time::DTUNIT_HOUR, Time::DTUNIT_SEC );
	t2 .setRange(Time::DTUNIT_HOUR, Time::DTUNIT_MSEC);

	
	static ConstString fmt[] = 
	{
		"%a %A",
		"%b %B",
		"%c",
		"%C",
		"%d %D",
		//"%f",
		"%F",
		//"%f3",
		"%F3",
		"%H %h",
		"%I",
		//"%i",
		"%j",	// ???
		//"%J",	
		"%M %m",
		"%p",
		//"%P",
		"%S",
		//"%s",
		"%u",
		//"%U",
		"%w",
		//"%W",
		"%x %X",
		"%y %Y",
		//"%z",
		//"%Z"
        basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX
	};

	static ConstString res[][4] = 
	{
		{ "Fri Friday"             , "Sun Sunday"             , "Wed Wednesday"          , "Fri Friday"              },
		{ "May May"                , "Dec December"           , "Oct October"            , "Dec December"            },
		{ "2007-05-11 15:57:33"    , "2007-12-23 08:34:12"    , "1990-10-31 23:34:56"    , "9999-12-31 23:59:59"     },
		{ "20"                     , "20"                     , "19"                     , "99"                      },
		{ "11 05/11/07"            , "23 12/23/07"            , "31 10/31/90"            , "31 12/31/99"             },
		{ "68"                     , "34"                     , "23"                     , "99"                      },
		{ "687"                    , "345"                    , "234"                    , "999"                     },
		{ "15 May"                 , "08 Dec"                 , "23 Oct"                 , "23 Dec"                  },
		{ "03"		               , "08"		              , "11"		             , "11"                      },
		{ "001"		               , "001"		              , "001"		             , "001"                     },
		{ "57 05"		           , "34 12"		          , "34 10"		             , "59 12"                   },
		{ "PM"		               , "AM"		              , "PM"		             , "PM"                      },
		{ "33"		               , "12"		              , "56"		             , "59"                      },
		{ "05"		               , "07"		              , "03"		             , "05"                      },
		{ "05"   	               , "00"		              , "03"		             , "05"                      },
		{ "05/11/2007 15:57:33"    , "12/23/2007 08:34:12"    , "10/31/1990 23:34:56"	 , "12/31/9999 23:59:59"     },
		{ "07 2007"		           , "07 2007"		          , "90 1990"		         , "99 9999"                 },
		{ "2007-05-11 15:57:33.687", "2007-12-23 08:34:12.345", "1990-10-31 23:34:56.234", "9999-12-31 23:59:59.999" }
	};

	for (UInt32 i = 0; i < sizeof(fmt)/sizeof(fmt[0]); ++i)	
	{
		CHECK_EQUAL(res[i][0], d3 .toString(fmt[i]).c_str());
		CHECK_EQUAL(res[i][1], d2 .toString(fmt[i]).c_str());
		CHECK_EQUAL(res[i][2], d1 .toString(fmt[i]).c_str());
		CHECK_EQUAL(res[i][3], nul.toString(fmt[i]).c_str());
	}

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		try
		{
			Locale::setLocale(j);
		}
		catch (const basar::RuntimeException &)
		{
			continue;
		}

        CHECK_EQUAL("29 Apr 2016 09:36:12 +0200", d5.toStrInternetMsg().c_str() );

		CHECK_EQUAL("23 Dec 2007 08:34:12 +0100", d2.toStrInternetMsg()                .c_str());
		CHECK_EQUAL("Sun, 23 Dec 2007 08:34:12" , d2.toString ("%a, %d %b %Y %H:%M:%S").c_str());

        CHECK_EQUAL("9999-12-31 15:19:31.000"   , t1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
		CHECK_EQUAL("9999-12-31 17:21:45.987"   , t2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());

		CHECK_EQUAL("2007-05-11 15:57:33.687"   , d3.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
		CHECK_EQUAL("1812-10-23 23:59:59.999"   , d4.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());

		CHECK_EQUAL("23.05.2010"                , DateTime(20100523).toString("%02d.%02m.%Y").c_str());
		CHECK_EQUAL("23.05.10"                  , DateTime(20100523).toString("%02d.%02m.%y").c_str());

		switch (j)
		{
		case Locale::C:
		case Locale::ENGLISH_US:
			CHECK_EQUAL("12/23/2007"                , d2.toStrDate     ().c_str());
			CHECK_EQUAL("08:34:12 AM"               , d2.toStrTimeShort().c_str());
			CHECK_EQUAL("08:34:12.345 AM"           , d2.toStrTime     ().c_str());
			CHECK_EQUAL("12/23/2007 08:34:12.345 AM", d2.toStrDateTime ().c_str());
			break;
		case Locale::ENGLISH_GB:
		case Locale::FRENCH_FR:
			CHECK_EQUAL("23/12/2007"             , d2.toStrDate     ().c_str());
			CHECK_EQUAL("08:34:12"               , d2.toStrTimeShort().c_str());
			CHECK_EQUAL("08:34:12.345"           , d2.toStrTime     ().c_str());
			CHECK_EQUAL("23/12/2007 08:34:12.345", d2.toStrDateTime ().c_str());

			break;
		default:
			CHECK_EQUAL("23.12.2007"             , d2.toStrDate     ().c_str());
			CHECK_EQUAL("08:34:12"               , d2.toStrTimeShort().c_str());
			CHECK_EQUAL("08:34:12.345"           , d2.toStrTime     ().c_str());
			CHECK_EQUAL("23.12.2007 08:34:12.345", d2.toStrDateTime ().c_str());
			break;
		}
	}
}

TEST(StreamOutput)
{
	DateTime d2(20071223,  83412345);

	std::ostringstream os;
	
	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		j < Locale::LOCALE_NUMBER; 
		j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		try
		{
			Locale::setLocale(j);
		}
		catch (const basar::RuntimeException &)
		{
			continue;
		}

		switch (j)
		{
		case Locale::C:
		case Locale::ENGLISH_US:
			os << d2;
			CHECK_EQUAL("12/23/2007 08:34:12.345 AM", os.str());
			os.str("");
			break;
		case Locale::ENGLISH_GB:
		case Locale::FRENCH_FR:
			os << d2;
			CHECK_EQUAL("23/12/2007 08:34:12.345", os.str());
			os.str("");

			break;
		default:
			os << d2;
			CHECK_EQUAL("23.12.2007 08:34:12.345", os.str());
			os.str("");
			break;
		}
	}
}


//------------------------------------------------------------------------------
TEST(toString_valid_mintime)
{
	Locale::setLocale(Locale::C);

	DateTime valid(20120217, 0);

	CHECK_EQUAL("02/17/2012 12:00:00.000 AM", valid.toStrDateTime().c_str());

	CHECK_EQUAL(20120217 , valid.getDate ());
	CHECK_EQUAL(2012     , valid.getYear ());
	CHECK_EQUAL(2        , valid.getMonth());
	CHECK_EQUAL(17       , valid.getDay  ());

	CHECK_EQUAL(0        , valid.getTime  ());
	CHECK_EQUAL(0        , valid.getHour  ());
	CHECK_EQUAL(0        , valid.getMinute());
	CHECK_EQUAL(0        , valid.getSec   ());
	CHECK_EQUAL(0        , valid.getMSec  ());
}

//------------------------------------------------------------------------------
TEST(toString_valid_maxtime)
{
	Locale::setLocale(Locale::C);

 	DateTime valid(20120217, 235959999);

	CHECK_EQUAL("02/17/2012 11:59:59.999 PM", valid.toStrDateTime().c_str());

	CHECK_EQUAL(20120217 , valid.getDate ());
	CHECK_EQUAL(2012     , valid.getYear ());
	CHECK_EQUAL(2        , valid.getMonth());
	CHECK_EQUAL(17       , valid.getDay  ());

	// hour to min (length 1128): e.g. tables giexclusionperiod, gigeneralparam
	CHECK_EQUAL(235959999, valid.getTime  ());
	CHECK_EQUAL(23       , valid.getHour  ());
	CHECK_EQUAL(59       , valid.getMinute());
	CHECK_EQUAL(59       , valid.getSec   ());
	CHECK_EQUAL(999      , valid.getMSec  ());
}

//------------------------------------------------------------------------------
TEST(toString_valid_maxtime_decr)
{
	Locale::setLocale(Locale::C);

	DateTime valid(20120217, 235959998);

	CHECK_EQUAL("02/17/2012 11:59:59.998 PM", valid.toStrDateTime().c_str());

	CHECK_EQUAL(20120217 , valid.getDate ());
	CHECK_EQUAL(2012     , valid.getYear ());
	CHECK_EQUAL(2        , valid.getMonth());
	CHECK_EQUAL(17       , valid.getDay  ());

	CHECK_EQUAL(235959998, valid.getTime  ());
	CHECK_EQUAL(23       , valid.getHour  ());
	CHECK_EQUAL(59       , valid.getMinute());
	CHECK_EQUAL(59       , valid.getSec   ());
	CHECK_EQUAL(998      , valid.getMSec  ());
}

//------------------------------------------------------------------------------
TEST(toString_invalid)
{
	Locale::setLocale(Locale::C);

	DateTime nul;
	nul.setInvalid();

	CHECK_EQUAL("12/31/9999"               , nul.toStrDate(                       ).c_str());
	CHECK_EQUAL("Fri, 31 Dec 9999 23:59:59", nul.toString ("%a, %d %b %Y %H:%M:%S").c_str());
	CHECK_EQUAL(""                         , nul.toStrInternetMsg().c_str());

	nul.setDate(DateTime::NULL_DATE + 1);
	CHECK_EQUAL("11:59:59.999 PM", nul.toStrTime     ().c_str());
	CHECK_EQUAL("11:59:59 PM"    , nul.toStrTimeShort().c_str());

	nul.setTime(DateTime::NULL_TIME + 1);
	nul.setInvalidDate();
	CHECK_EQUAL("12/31/9999", nul.toStrDate().c_str());

	CHECK_EQUAL("Fri, 31 Dec 9999 00:00:00", nul.toString("%a, %d %b %Y %H:%M:%S").c_str());
}

//------------------------------------------------------------------------------
TEST(toString_invalid_Date)
{
	Locale::setLocale(Locale::C);

	DateTime dt(DateTime::NULL_DATE, 152731987);
	dt.setRange(DateTime::DTUNIT_HOUR, DateTime::DTUNIT_MSEC);

	CHECK_EQUAL("9999-12-31"             , dt.toString("%04Y-%02m-%02d"));
	CHECK_EQUAL("15:27:31.987"           , dt.toString(basar::cmnutil::FORMAT_SQL_TIME_INFORMIX));
	CHECK_EQUAL("9999-12-31 15:27:31.987", dt.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX));
}

//------------------------------------------------------------------------------
TEST(toString_invalid_Time)
{
	Locale::setLocale(Locale::C);

	DateTime dt(20190831, DateTime::NULL_TIME);
	dt.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_DAY);

	CHECK_EQUAL("2019-08-31"             , dt.toString("%04Y-%02m-%02d"));
	CHECK_EQUAL("23:59:59.999"           , dt.toString(basar::cmnutil::FORMAT_SQL_TIME_INFORMIX));
	CHECK_EQUAL("2019-08-31 23:59:59.999", dt.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX));
}
 
//------------------------------------------------------------------------------
TEST(fromString)
{
    try
    {
	    DateTime d;
	    d.fromString("%c", "");
	    CHECK_EQUAL(DateTime::NULL_DATE, d.getDate());
	    CHECK_EQUAL(DateTime::NULL_TIME, d.getTime());

	    for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		    j < Locale::LOCALE_NUMBER; 
		    j = static_cast<Locale::LanguageEnum>(j + 1))
	    {
		    try
		    {
			    Locale::setLocale(j);
		    }
		    catch (const basar::RuntimeException &)
		    {
			    continue;
		    }

			d.fromString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX, "9999-12-31 15:19:31.000");
			CHECK_EQUAL(DateTime::NULL_DATE, d.getDate());
			CHECK_EQUAL(151931000          , d.getTime());    

            d.fromString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX, "9999-12-31 23:59:59.999");
			CHECK_EQUAL(DateTime::NULL_DATE, d.getDate());
			CHECK_EQUAL(DateTime::NULL_TIME, d.getTime());  

			d.fromString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX, "9999-12-31 17:21:45.987");
			CHECK_EQUAL(DateTime::NULL_DATE, d.getDate());
			CHECK_EQUAL(172145987          , d.getTime());          

			d.fromString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX, "1812-10-23 23:59:59.999");
			CHECK_EQUAL(18121023           , d.getDate());
            CHECK_EQUAL(235959999, d.getTime());          

			d.fromString("%c", "2007-12-24 23:45:21");
		    CHECK_EQUAL(20071224 , d.getDate());
		    CHECK_EQUAL(234521000, d.getTime());

		    d.fromString("%c.%F3", "2007-12-24 23:45:21.345");
		    CHECK_EQUAL(20071224 , d.getDate());
		    CHECK_EQUAL(234521345, d.getTime());


		    switch (j)
		    {
		    case Locale::C:
		    case Locale::ENGLISH_US:
			    d.fromStrDate("03/13/2007");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(234521345, d.getTime());

			    d.fromStrTime("05:56:12.789 PM");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());

			    d.fromStrTimeShort("01:34:56 PM"); 
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(133456000, d.getTime());

                d.fromStrDateTime("07/14/2008");
                CHECK_EQUAL(20080714 , d.getDate());
			    CHECK_EQUAL(0, d.getTime());

                d.fromStrDateTime("07/15/2008 05:56:12.789 PM");
                CHECK_EQUAL(20080715 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());
			    break;

		    case Locale::ENGLISH_GB:
		    case Locale::FRENCH_FR:
			    d.fromStrDate("13/03/2007");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(234521345, d.getTime());
    			
			    d.fromStrTime("17:56:12.789");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());

			    d.fromStrTimeShort("13:34:56");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(133456000, d.getTime());

                d.fromStrDateTime("14/07/2008");
                CHECK_EQUAL(20080714 , d.getDate());
			    CHECK_EQUAL(0, d.getTime());

                d.fromStrDateTime("15/07/2008 17:56:12.789");
                CHECK_EQUAL(20080715 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());
			    break;

		    default:
			    d.fromStrDate("13.03.2007");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(234521345, d.getTime());
    			
			    d.fromStrTime("17:56:12.789");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());
    			
			    d.fromStrTimeShort("13:34:56");
			    CHECK_EQUAL(20070313 , d.getDate());
			    CHECK_EQUAL(133456000, d.getTime());

                d.fromStrDateTime("14.07.2008");
                CHECK_EQUAL(20080714 , d.getDate());
			    CHECK_EQUAL(0, d.getTime());

                d.fromStrDateTime("15.07.2008 17:56:12.789");
                CHECK_EQUAL(20080715 , d.getDate());
			    CHECK_EQUAL(175612789, d.getTime());
			    break;
		    }
	    }
    }
    catch (basar::Exception & ex)
    {
        std::cout << "BasarException occurred " << ex.what() << std::endl;
        throw;
    }
}

//------------------------------------------------------------------------------
TEST(compare)
{
    {
        DateTime d1;
	    mysleep(1000);
	    DateTime d2;
	    DateTime d3(d2);
	    DateTime d4(20071223, 134759678);
	    DateTime d5(20071223, 134759678);

		CHECK( (d1 == d1));

	    CHECK(!(d1 == d2));
	    CHECK( (d2 == d3));
	    CHECK(!(d1 == d3));

	    CHECK(d5 == d4);

	    CHECK(! (DateTime(20000112, 230000001) == DateTime(20000112, 230000000)));
    }
    {
        //example from datetime header
        DateTime d1(20080112, 93200001);
        DateTime d2(d1);
        d2.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_MIN);

		CHECK(d2 == d1);

        CHECK(d1 == d2);
    }
    {
        //example from datetime header
        DateTime d1(20080112, 93200001);
        DateTime d2(20080112, 93200000);
        d2.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_MIN);

		CHECK(d2 == d1);

        //d2 is now interally 2008-01-12 09:32
        CHECK(d1 == d2);
    }
}

TEST(lessThan)
{
    DateTime d1;
    mysleep(1000);
    DateTime d2;

    CHECK(d1 < d2);
}

TEST(greaterThan)
{
    DateTime d1;
    mysleep(1000);
    DateTime d2;

    CHECK(d2 > d1);
}

TEST(inequality)
{
    DateTime d1;
    mysleep(1000);
    DateTime d2;

    CHECK(d2 != d1);
}

TEST(lessThanOrEqual)
{
    DateTime d1;
    mysleep(1000);
    DateTime d2;

    CHECK(d1 <= d2);

    DateTime d3(d1);

    CHECK(d1 <= d3);
	CHECK(d3 <= d1);
}

TEST(greaterThanOrEqual)
{
    DateTime d1;
    mysleep(1000);
    DateTime d2;

    CHECK(d2 >= d1);

    DateTime d3(d1);

    CHECK(d3 >= d1);
	CHECK(d1 >= d3);
}


TEST(lessThanInteresting)
{
    DateTime d1(20130508, 160123432);

    DateTime d2;

    //set only time, set range accordingly
    d2.setTime(120000123);

    //basicly emulating a Time instance
    d2.setRange(DateTime::DTUNIT_HOUR, DateTime::DTUNIT_MSEC);

    //expection: d2 < d1
    CHECK(d2 < d1);

	CHECK(d1 > d2);
}

TEST(IncompatibleRangeException)
{
    //basically a Date
	DateTime d1(20130508);

	d1.setRange(DateTime::DTUNIT_YEAR, DateTime::DTUNIT_DAY);

    DateTime d2;

    //set only time, set range accordingly
    d2.setTime(120000123);

    //basicly emulating a Time instance
    d2.setRange(DateTime::DTUNIT_HOUR, DateTime::DTUNIT_MSEC);
	
	CHECK_THROW((d1 == d2), basar::IncompatibleRangeException);

}

TEST(DateToDateTimeConversionConstructor)
{
	Date d(20130522);

	DateTime dt(d);

	CHECK_EQUAL(20130522, dt.getDate());

	CHECK(!dt.isValidTime());
}

//------------------------------------------------------------------------------
}	// end TESTSUITE

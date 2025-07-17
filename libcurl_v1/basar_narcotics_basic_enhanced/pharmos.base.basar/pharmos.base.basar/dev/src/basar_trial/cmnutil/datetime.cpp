
#include <libbasarcmnutil.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <sys/timeb.h>

#include "testcmnutil.h"

//------------------------------------------------------------------------------
using std::cout;
using std::endl;

using basar::cmnutil::DateTime;
using basar::cmnutil::DateTimeTools;
using basar::cmnutil::Date;
using basar::Time;
using basar::cmnutil::Locale;
using basar::Int16;
using basar::Int32;

//------------------------------------------------------------------------------
static void mysleep(Int32 milliSec)
{
#ifdef WIN32
	::Sleep(milliSec);
#else
	struct timeval waitTime;

	waitTime.tv_sec  = milliSec / 1000;
	waitTime.tv_usec = (milliSec % 1000) * 1000;

	select(0, 0, 0, 0, &waitTime);	 
#endif
}

//------------------------------------------------------------------------------
static void dt_test_tool()
{
	printf("\n");
	printf("--- dt_test_tool -------------------------\n");
	printf("\n");

	printf("\nTools get ...\n");

	{
		Int32 date = 20061231;
		Int32 time = 235933591;

		printf(	"(%d,%d) => %02d-%02d-%02d %02d:%02d:%02d.%03d\n", 
			    date, time,
				DateTimeTools::getYear  (date),
				DateTimeTools::getMonth (date),
				DateTimeTools::getDay   (date),
				DateTimeTools::getHour  (time),
				DateTimeTools::getMinute(time),
				DateTimeTools::getSec   (time),
				DateTimeTools::getMSec  (time));

		Int16 y = 2003,
			  m = 10,
			  d = 28;
		Int16 h   = 13,
			  min = 23,
			  sec = 34,
			  ms  = 599;

		printf("date(%d,%d,%d) = %d, time(%d,%d,%d,%d) = %d\n", 
			    y, m, d, DateTimeTools::getDate(y, m, d),
				h, min, sec, ms, DateTimeTools::getTime(h, min, sec, ms));
	}

	printf("\nTools valid ...\n");

	{
		Int16 y = 2003,
			  m = 10,
			  d = 28;
		Int16 h   = 13,
			  min = 23,
			  sec = 34,
			  ms  = 599;

		printf("valid date(%d,%d,%d)    = %d\n", y, m, d         , DateTimeTools::isValidDate(y, m, d));
		printf("valid time(%d,%d,%d,%d) = %d\n", h, min, sec, ms, DateTimeTools::isValidTime(h, min, sec, ms));

		m = 2;
		d = 29;
		ms = 7999;

		printf("valid date(%d,%d,%d)    = %d\n", y, m, d         , DateTimeTools::isValidDate(y, m, d));
		printf("valid date(%d,%d,%d)    = %d\n", 2004, m, d      , DateTimeTools::isValidDate(2004, m, d));
		printf("valid time(%d,%d,%d,%d) = %d\n", h, min, sec, ms, DateTimeTools::isValidTime(h, min, sec, ms));

		Int32 date = 20031028;
		Int32 time = 132334599;

		printf("valid date %d = %d\n", date               , DateTimeTools::isValidDate(date));
		printf("valid time %d = %d\n", time               , DateTimeTools::isValidTime(time));
		printf("valid date %d = %d\n", date + 2000        , DateTimeTools::isValidDate(date + 2000));
		printf("valid time %d = %d\n", DateTime::NULL_TIME, DateTimeTools::isValidTime(DateTime::NULL_TIME));
		printf("valid time %d = %d\n", 0                  , DateTimeTools::isValidTime(0));
		printf("valid time %d = %d\n", 1                  , DateTimeTools::isValidTime(1));
		printf("valid date %d = %d\n", DateTime::NULL_DATE, DateTimeTools::isValidDate(DateTime::NULL_DATE));
		printf("valid date %d = %d\n", -1                 , DateTimeTools::isValidDate(-1));
		printf("valid date %d = %d\n", 1                  , DateTimeTools::isValidDate(1));
	}

	printf("\nTools total ...\n");

	{
		Int32 s[] =		// milli sec
		{
			0,
			1,
			86400000,
			3600000,
			60000,
			96400000,
		};

		Int32 t[] =		// time
		{
			0, 
			1,
			235959999,
			10000000,
			100000,
			-1
		};

		for (UInt32 i = 0; i < sizeof(s)/sizeof(s[0]); ++i)
		{
			printf("total msecs (%10d) = %10d; ", t[i], DateTimeTools::getTotalMSec(t[i]));
			printf("time        (%10d) = %10d\n", s[i], DateTimeTools::getTimeByTotalMSec(s[i]));
		}
	}

	printf("\nTools old/new ...\n");

	{
		static const Int32 _new = 190123345;
		static const Int32 _old = 131745;

		printf("old %10d => new %10d\n", _old, DateTimeTools::getNewTime(_old));
		printf("new %10d => old %10d\n", _new, DateTimeTools::getOldTime(_new));
	}

}

static void dt_test_date()
{
	printf("\n");
	printf("--- dt_test_date -------------------------\n");
	printf("\n");

	{
		printf("\nstatics ...\n");

		Int32 year[] = 
		{
			2008,
			2007,
			2006,
			2005,
			2004,
			2003
		};

		for (UInt32 i = 0; i < sizeof(year)/sizeof(year[0]); ++i)
		{
			printf("year %d\n  isleapyear() = %d\n", year[i], Date::isLeapYear(year[i]));

			Date d1 = Date::getFstSprMoon  (year[i]);
			Date d2 = Date::getEasterSunday(year[i]);

			printf("  1st spring moon: %d\n  Easter Sunday %d\n", d1.getDate(), d2.getDate());
		}

		printf("getCurrent date %d\n", Date::getCurrent().getDate());
	}

	{
		printf("\nc'tor ...\n");

		static ConstString inp[] = 
		{
			"",
			"19991028",
			"19230908",
			"0",
			"1945, 12, 30"
		};

		static Date d[] = 
		{
			Date(),
			Date(19991028),
			Date(19230908),
			Date(0),
			Date(1945, 12, 30)
		};

		for (UInt32 i = 0; i < sizeof(d)/sizeof(d[0]); ++i)
		{
			printf("%-20.20s date %10d\n", inp[i], d[i].getDate());
		}
	}

	{
		printf("\nget ...\n");

		static const Int32 i = 20070321;
		Date d1;
		Date d2(i);

		printf(	"current               => %02d-%02d-%02d (%d)\n", 
				d1.getYear  (), d1.getMonth (), d1.getDay   (), d1.getDate());
		printf(	"(%d)       => %02d-%02d-%02d (%d)\n", 
			    i,
				d2.getYear  (), d2.getMonth (), d2.getDay   (), d2.getDate());
	}


	{
		printf("\ncalculate ...\n");

		static Date d[] = 
		{
			Date(),
			Date(20070328),
			Date(20070101),
			Date(20071231),
			Date(20071224)
		};

		for (UInt32 i = 0; i < sizeof(d)/sizeof(d[0]); ++i)
			printf("day of week %d, day of year %d, week of year %d\n", d[i].getDayOfWeek(), d[i].getDayOfYear(), d[i].getWeekOfYear());
	}

	{
		printf("\nset ...\n");

		static const Int32 i = 20000101;
		Date d1;

		d1.setDate(i);

		printf(	"(%d)          => %02d-%02d-%02d(%d)\n", 
			    i,
				d1.getYear  (), d1.getMonth (), d1.getDay   (), d1.getDate());

		static const Int32 y    = 1978,
						   m    = 1,
						   d    = 23;
		d1.setDate(y, m, d);

		printf(	"(%d,%d,%d) => %02d-%02d-%02d (%d)\n", 
			    y, m, d,
				d1.getYear  (), d1.getMonth (), d1.getDay   (),	d1.getDate());
	}

	{
		printf("\nset invalid ...\n");

		Date d1, d2, d3;

		d1.setInvalid    ();

		printf("setInvalid    () (%10d) => isValid     %d\n", d1.getDate(), d1.isValid    ());
	}

	{
		printf("\nvalid ...\n");

		Date d1;
		Date d2(0);
		Date d4(-5);
		Date d5(20051231);

		printf("%8d isValid %d\n"
			   "%8d isValid %d\n"
			   "%8d isValid %d\n"
			   "%8d isValid %d\n",
			   d1.getDate(), d1.isValid(),
			   d2.getDate(), d2.isValid(),
			   d4.getDate(), d4.isValid(),
			   d5.getDate(), d5.isValid()); 

		d5.setInvalid();
		printf("setInvalid    () => isValid     %d\n", d5.isValid());
	}

	{
		printf("\nadd Date ..\n");

		Date               d1(20071231);
		Int32                  add[] =  
		{	
			   1,
			  -1,
			 366,
			-366,
			-366
		};

		for (unsigned int i = 0; i < sizeof(add)/sizeof(add[0]); ++i)
		{
			printf("%11d + %11d = ", d1.getDate(), add[i]);
			d1.addDate(add[i]);
			printf("added date = %11d\n", d1.getDate()); 
		}
	}

	{
		printf("\ntoString ..\n");

		static ConstString fmt[] = 
		{
			"dd ddd",
			"mm mmm",
			"yy yyyy",
			"ww",
			FORMAT_SQL_DATE_INFORMIX
		};


		Date d(20070402), d1(19901031), nul;
		nul.setInvalid();


		for (UInt32 i = 0; i < sizeof(fmt)/sizeof(fmt[0]); ++i)	
		{
			printf("fmt = <%s> => <%s>\n", fmt[i], d  .toString(fmt[i]).c_str());
			printf("fmt = <%s> => <%s>\n", fmt[i], d1 .toString(fmt[i]).c_str());
			printf("fmt = <%s> => <%s>\n", fmt[i], nul.toString(fmt[i]).c_str());
		}


		for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; j < Locale::LOCALE_NUMBER; j = (Locale::LanguageEnum)(j + 1))
		{
			try
			{
				Locale::setLocale(j);
			}
			catch (const basar::RuntimeException &)
			{
				continue;
			}

			printf("locale = %d\n", j);

			printf("  toStrDate()                          => <%s>\n"  , d1.toStrDate     ().c_str());
			printf("  toString(\"%%a, %%d %%b %%Y %%H:%%M:%%S\") => <%s>\n", d1.toString("%a, %d %b %Y %H:%M:%S").c_str());
		}



		d.setInvalid();
		printf("toStrDate()                           => <%s>\n", d.toStrDate     ().c_str());

		printf("toString(\"%%a, %%d %%b %%Y %%H:%%M:%%S\") => <%s>\n", d.toString("%a, %d %b %Y %H:%M:%S").c_str());

		Date now;
		printf("now.toString(FORMAT_SQL_DATE_INFORMIX): <%s>\n", now.toString(FORMAT_SQL_DATE_INFORMIX).c_str());
}


	{
		printf("\nfromString ..\n");

		static ConstString str[] =
		{
			"2007-12-24",
			"03/13/2007",

			"2007-12-24",
			"12.03.2007",
		};

		Date d;

		Locale::setLocale(Locale::C); printf("locale = %d\n", Locale::C);

		d.fromString      ("yyyy-mm-dd", str[0]);	printf("  fromString      (\"%-25s\") => (%10d)\n", str[0], d.getDate());
		d.fromStrDate     (str[1]);			        printf("  fromStrDate     (\"%-25s\") => (%10d)\n", str[1], d.getDate());


		Locale::setLocale(Locale::GERMAN_DE);  printf("locale = %d\n", Locale::GERMAN_DE);

		d.fromString      ("yyyy-mm-dd", str[2]);	printf("  fromString      (\"%-25s\") => (%10d)\n", str[2], d.getDate());
		d.fromStrDate     (str[3]);			        printf("  fromStrDate     (\"%-25s\") => (%10d)\n", str[3], d.getDate());
	}

	{
		printf("\ncompare ...\n");

		Date d1;
		Date d2(1289, 12, 23);
		Date d3(d2);

		printf("d1(%d)\nd2(%d)\nd3(%d)\n", 
			    d1.getDate(),
			    d2.getDate(),
			    d3.getDate());
		printf("d1 == d2 = %d\n", d1 == d2);
		printf("d3 == d2 = %d\n", d3 == d2);
	}
}

//------------------------------------------------------------------------------
void dt_test_datetime()
{
	printf("\n");
	printf("--- dt_test_datetime -------------------------\n");
	printf("\n");

	{
		printf("\nstatics ...\n");

		printf("getCurrent date %d\n", DateTime::getCurrent().getDate());
		for (int i = 0; i < 20; ++i)
		{
			printf("getCurrent time %d\n", DateTime::getCurrent().getTime());

#ifndef TEST_TIME_RESOLUTION
			timeb tb;
			ftime(&tb);
			printf("ftime() = (%d, %d sec, %d msec, %d min) => %s\n", tb.dstflag, tb.time, tb.millitm, tb.timezone, ctime(&tb.time));
#endif
			mysleep(35);
		}
	}

	{
		printf("\nc'tor ...\n");

		static ConstString inp[] = 
		{
			"",
			"19991028",
			"19230908, 134523432",
			"0, 92345678",
			"1945, 12, 30",
			"23, 45, 15, 654"
		};

		static DateTime d[] = 
		{
			DateTime(),
			DateTime(19991028),
			DateTime(19230908, 134523432),
			DateTime(0, 92345678),
			DateTime(1945, 12, 30),
			DateTime(23, 45, 15, 654)
		};

		for (UInt32 i = 0; i < sizeof(d)/sizeof(d[0]); ++i)
		{
			printf("%-20.20s date %10d, time %10d\n", inp[i], d[i].getDate(), d[i].getTime());
		}
	}

	{
		printf("\nget ...\n");

		static const Int32 i = 20070321,
			               j = 134517123;
		DateTime d1;
		DateTime d2(i, j);

		printf(	"current               => %02d-%02d-%02d %02d:%02d:%02d.%03d (%d, %d)\n", 
				d1.getYear  (), d1.getMonth (), d1.getDay   (),
				d1.getHour  (), d1.getMinute(), d1.getSec   (), d1.getMSec  (),
				d1.getDate(), d1.getTime());
		printf(	"(%d, %d) => %02d-%02d-%02d %02d:%02d:%02d.%03d (%d, %d)\n", 
			    i, j,
				d2.getYear  (), d2.getMonth (), d2.getDay   (),
				d2.getHour  (), d2.getMinute(), d2.getSec   (), d2.getMSec  (),
				d2.getDate(), d2.getTime());
	}

	{
		printf("\nset ...\n");

		static const Int32 i = 20000101,
			               j = 30201999;
		DateTime d1;

		d1.setDate(i);
		d1.setTime(j);

		printf(	"(%d, %d)          => %02d-%02d-%02d %02d:%02d:%02d.%03d (%d, %d)\n", 
			    i, j,
				d1.getYear  (), d1.getMonth (), d1.getDay   (),
				d1.getHour  (), d1.getMinute(), d1.getSec   (), d1.getMSec  (),
				d1.getDate(), d1.getTime());

		static const Int32 y    = 1978,
						   m    = 1,
						   d    = 23,
						   h    = 8,
						   min  = 12, 
					       sec  = 45,
						   msec = 567;
		d1.setTime(h, min, sec, msec);
		d1.setDate(y, m, d);

		printf(	"((%d,%d,%d), (%d,%d,%d,%d))) => %02d-%02d-%02d %02d:%02d:%02d.%03d (%d, %d)\n", 
			    y, m, d, h, min, sec, msec,
				d1.getYear  (), d1.getMonth (), d1.getDay   (),
				d1.getHour  (), d1.getMinute(), d1.getSec   (), d1.getMSec  (),
				d1.getDate(), d1.getTime());
	}

	{
		printf("\nset invalid ...\n");

		DateTime d1, d2, d3;

		d1.setInvalid    ();
		d2.setInvalidDate();
		d3.setInvalidTime();

		printf("setInvalid    () (%10d,%10d) => isValid     %d\n", d1.getDate(), d1.getTime(), d1.isValid    ());
		printf("setInvalidDate() (%10d,%10d) => isValidDate %d\n", d2.getDate(), d2.getTime(), d2.isValidDate());
		printf("setInvalidTime() (%10d,%10d) => isValidTime %d\n", d3.getDate(), d3.getTime(), d3.isValidTime());
	}

	{
		printf("\nisvalid ...\n");

		DateTime d1;
		DateTime d2(0);
		DateTime d3(0, 0);
		DateTime d4(-5);
		DateTime d5(20051231, -6);
		DateTime d6(20051231, 123456789);

		printf("%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n"
			   "%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n"
			   "%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n"
			   "%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n"
			   "%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n"
			   "%8d|%9d isValid %d,  isValidDate %d, isValidTime %d\n",
			   d1.getDate(), d1.getTime(), d1.isValid(),  d1.isValidDate(),  d1.isValidTime(), 
			   d2.getDate(), d2.getTime(), d2.isValid(),  d2.isValidDate(),  d2.isValidTime(), 
			   d3.getDate(), d3.getTime(), d3.isValid(),  d3.isValidDate(),  d3.isValidTime(), 
			   d4.getDate(), d4.getTime(), d4.isValid(),  d4.isValidDate(),  d4.isValidTime(), 
			   d5.getDate(), d5.getTime(), d5.isValid(),  d5.isValidDate(),  d5.isValidTime(), 
			   d6.getDate(), d6.getTime(), d6.isValid(),  d6.isValidDate(),  d6.isValidTime());

		d1.setInvalidTime();
		printf("setInvalidTime() => isValidTime %d\n", d1.isValidTime());
		d5.setInvalidDate();
		printf("setInvalidDate() => isValidDate %d\n", d5.isValidDate());
		d6.setInvalid();
		printf("setInvalid    () => isValid     %d\n", d6.isValid());
	}
#ifndef NO_DT_ADD
	{
		printf("\nadd Date ..\n");

		DateTime               d1(20071231);
		Int32                  add[] =  
		{	
			   1,
			  -1,
			 366,
			-366,
			-366
		};

		for (unsigned int i = 0; i < sizeof(add)/sizeof(add[0]); ++i)
		{
			printf("%11d + %11d = ", d1.getDate(), add[i]);
			d1.addDate(add[i]);
			printf("added date = %11d\n", d1.getDate()); 
		}
	}

	{
		printf("\nadd Time ..\n");

		DateTime               d[] = 
		{
			DateTime(0       , 140000000),
			DateTime(20070330, 140000000)
		};

		Int32                  add[] =  
		{	
			  23456789,
			-170000000,
		     110000000,
		    1333333333
		};

		DateTime::TimeCalcEnum eFlag;

		for (unsigned int i = 0; i < sizeof(add)/sizeof(add[0]); ++i)
		{
			for (unsigned int j = 0; j < sizeof(d)/sizeof(d[0]); ++j)
			{
				printf("(%11d, %11d) + %11d = ", d[j].getDate(), d[j].getTime(), add[i]);
				eFlag = d[j].addTime(add[i]);
				printf("add return %2d, (%11d, %11d)\n", eFlag, d[j].getDate(), d[j].getTime()); 
			}
		}
	}

#endif	

	{
		printf("\ntoString ..\n");

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
			FORMAT_SQL_DATETIME_INFORMIX
		};

		DateTime d(20071223, 83412345), d1(19901031, 233456234);
		DateTime nul;
		nul.setInvalid();

		for (UInt32 i = 0; i < sizeof(fmt)/sizeof(fmt[0]); ++i)	
		{
			printf("fmt = <%s> => <%s>\n", fmt[i], d  .toString(fmt[i]).c_str());
			printf("fmt = <%s> => <%s>\n", fmt[i], d1 .toString(fmt[i]).c_str());
			printf("fmt = <%s> => <%s>\n", fmt[i], nul.toString(fmt[i]).c_str());
		}

		static const Locale::LanguageEnum lgg[] =
		{
			Locale::GERMAN_DE,
#ifdef WIN32			
			Locale::ENGLISH_GB,
			Locale::FRENCH_FR,
#endif
			Locale::C
		};

		for (UInt32 j = 0; j < sizeof(lgg)/sizeof(lgg[0]); ++j)
		{
			Locale::setLocale(lgg[j]);
			printf("locale = %d\n", lgg[j]);
			
			printf("  internet message format              => <%s>\n", d.toStrInternetMsg().c_str());

			printf("  toStrDate()                          => <%s>\n"  , d.toStrDate     ().c_str());
			printf("  toStrTimeShort()                     => <%s>\n"  , d.toStrTimeShort().c_str());
			printf("  toStrTime                            => <%s>\n", d.toStrTime       ().c_str());
			printf("  toString(\"%%a, %%d %%b %%Y %%H:%%M:%%S\")    => <%s>\n", d.toString("%a, %d %b %Y %H:%M:%S").c_str());
		}

		d.setInvalidTime();
		printf("internet message format                => <%s>\n", d.toStrInternetMsg().c_str());

		d.setDate(DateTime::NULL_DATE + 1);
		printf("toStrTime                              => <%s>\n", d.toStrTime       ().c_str());
		printf("toStrTimeShort()                       => <%s>\n", d.toStrTimeShort().c_str());

		d.setTime(DateTime::NULL_TIME + 1);
		d.setInvalidDate();
		printf("toStrDate()                            => <%s>\n", d.toStrDate     ().c_str());

		printf("toString(\"%%a, %%d %%b %%Y %%H:%%M:%%S\") => <%s>\n", d.toString("%a, %d %b %Y %H:%M:%S").c_str());

		DateTime now;

		printf("now.toString(FORMAT_SQL_DATETIME_INFORMIX): <%s>\n", now.toString(FORMAT_SQL_DATETIME_INFORMIX).c_str());
	}


	{
		printf("\nfromString ..\n");

		static ConstString str[] =
		{
			"2007-12-24 23:45:21",
			"2007-12-24 23:45:21.345",
			"03/13/2007",
			"05:56:12.789 AM",
			"01:34:56 PM",

			"2007-12-24 23:45:21",
			"2007-12-24 23:45:21.345",
			"12.03.2007",
			"17:56:12.789",
			"13:34:56",
		};

		DateTime d;

		d.fromString      ("%c", "");	printf("  fromString      (\"%-25s\") => (%10d,%10d)\n", "", d.getDate(), d.getTime());
		
		Locale::setLocale(Locale::C); printf("locale = %d\n", Locale::C);

		d.fromString      ("%c", str[0]);	printf("  fromString      (\"%-25s\") => (%10d,%10d)\n", str[0], d.getDate(), d.getTime());
		d.fromString      ("%c%F3", str[1]);printf("  fromString      (\"%-25s\") => (%10d,%10d)\n", str[1], d.getDate(), d.getTime());
		d.fromStrDate     (str[2]);			printf("  fromStrDate     (\"%-25s\") => (%10d,%10d)\n", str[2], d.getDate(), d.getTime());
		d.fromStrTime     (str[3]);			printf("  fromStrTime     (\"%-25s\") => (%10d,%10d)\n", str[3] , d.getDate(), d.getTime());
		d.fromStrTimeShort(str[4]);			printf("  fromStrTimeShort(\"%-25s\") => (%10d,%10d)\n", str[4], d.getDate(), d.getTime());


		Locale::setLocale(Locale::GERMAN_DE);  printf("locale = %d\n", Locale::GERMAN_DE);

		d.fromString      ("%c", str[5]);	printf("  fromString      (\"%-25s\") => (%10d,%10d)\n", str[5], d.getDate(), d.getTime());
		d.fromString      ("%c%F3", str[6]);printf("  fromString      (\"%-25s\") => (%10d,%10d)\n", str[6], d.getDate(), d.getTime());
		d.fromStrDate     (str[7]);			printf("  fromStrDate     (\"%-25s\") => (%10d,%10d)\n", str[7], d.getDate(), d.getTime());
		d.fromStrTime     (str[8]);			printf("  fromStrTime     (\"%-25s\") => (%10d,%10d)\n", str[8], d.getDate(), d.getTime());
		d.fromStrTimeShort(str[9]);			printf("  fromStrTimeShort(\"%-25s\") => (%10d,%10d)\n", str[9], d.getDate(), d.getTime());
	}

	{
		printf("\ncompare ...\n");

		DateTime d1;
		mysleep(300);
		DateTime d2;
		DateTime d3(d2);

		printf("d1(%d,%d)\nd2(%d,%d)\nd3(%d,%d)\n", 
			    d1.getDate(), d1.getTime(),
			    d2.getDate(), d2.getTime(),
			    d3.getDate(), d3.getTime());
		printf("d1 == d2 = %d\n", d1 == d2);
		printf("d3 == d2 = %d\n", d3 == d2);
	}
}

//------------------------------------------------------------------------------
void test_datetime()
{
	dt_test_tool();
	dt_test_date();
	dt_test_datetime();
}


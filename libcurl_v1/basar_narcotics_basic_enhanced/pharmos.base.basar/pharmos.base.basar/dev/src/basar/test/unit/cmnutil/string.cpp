#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include <stdio.h>
#include <iomanip>

//------------------------------------------------------------------------------
using basar::cmnutil::Locale;
using basar::cmnutil::CharString;
using basar::cmnutil::BString;
using basar::cmnutil::I18nString;
using basar::cmnutil::CollBString;

using basar::ConstString;
using basar::ConstBuffer;
using basar::Int32;
using basar::Float64;
using basar::Long64;
using basar::ULong64;
using basar::Int64;
using basar::Decimal;

//------------------------------------------------------------------------------
// some tests from:
//   https://github.com/mirrors/wine/blob/master/dlls/msvcrt/tests/printf.c
//
SUITE(String)
{

//----------------------------------------------------------------------------
// test static init
static bool testStaticInit()
{
    // implicit: Locale GERMAN_DE
    I18nString str;
    str.ftos(1.2f);

    if ("1,2" != str)
    {
        printf("%s(%d) : testStaticInit failed: "
               "expected \"1,2\", actual \"%s\"\n", 
               __FILE__, __LINE__,
               str.c_str());
        exit(EXIT_FAILURE);
    }

    return true;
}

static bool dummyTestStaticInit = testStaticInit();


//----------------------------------------------------------------------------
// check some g++ compiler warings: -Wformat
//
void never_executed()
{
    dummyTestStaticInit = 0; // avoid error: 'SuiteString::dummyTestStaticInit' defined but not used [-Werror=unused-variable]

    /*! \ingroup DIFF_WIN_UNIX */
    // AIX gcc seems to have a bug: error: format '%lld' expects argument of type 'long long int', but argument 2 has type 'basar::ULong64 {aka long unsigned int}' [-Werror=format]
    // even though sizeof(long long int) == sizeof(long int) == 8 on 64-Bit ...
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wformat"
#pragma message("Following warnings about format '%ll' are a GCC bug on AIX...")
#endif // __GNUC__

    printf("test%%");
    printf("%%0");

    printf("%d", 123);
    printf("% d", 1);
    printf("%+d", 1);
    printf("%d", -1);
    printf("%10d" , 1977);
    printf("%010d", 1977);
    printf("%10d" , -1977);
    printf("%010d", -1977);
    printf("%04d, %05d", -3, 4);
    printf("%04d", 42);
    printf("%0*d", 4, 42);
    printf("%3d", 1234);
    printf("% .80d", 1);
    printf("%hd", 74565);
    printf("%hd", 291);

    printf("%-i", -1);
    printf("%-i", 1);
    printf("%+i", 1);
    printf("%hi", 74565);
    printf("%hi", 291);

    printf("%2.4e", 8.6);
    printf("% 2.4e", 8.6);
    printf("% 014.4e", 8.6);
    printf("% 2.4e", -8.6);
    printf("%+2.4e", 8.6);
    printf("%+#23.15e", 789456123.0);
    printf("%-#23.15e", 789456123.0);
    printf("%#23.15e", 789456123.0);
    printf("%#1.1g", 789456123.0);
    printf("%2.4g", 8.6);
    printf("%f", 50000.1234);
    printf("%.4f", 50000.1234);
    printf("%2.4f", 8.6);
    printf("%0f", 0.6);
    printf("%.0f", 0.6);
    printf("%e and %f", 50000.1234, 1.23);
    printf("%4.2f %+.0e %E", 3.1416, 3.1416, 3.1416);
    printf("%4.2f %+.0e %E", -3.1416, -3.1416, -3.1416);
    printf("%Le", (long double)-1.1);
    printf("%Lf", (long double)-1.1);
    printf("%Lg", (long double)-1.1);
    printf("%f", 123.456);
    printf("%05f", 123.456);
    printf("%0*f", 5, 123.456);
    printf("%.5f", 123.456);
    printf("%.*f", 5, 123.456);
    printf("<%e>"  , 123.456);
    printf("<%.3f>", 123.456);
    printf("<%g>" , 123.456);
    printf("<%e>"  , 123.456);
    printf("<%.3f>", 123.456);
    printf("<%g>" , 123.456);
    printf("%lld", ((ULong64)0xffffffff)*0xffffffff);
    printf("%+8lld", (Long64)100);
    printf("%+.8lld", (Long64)100);
    printf("%+10.8lld", (Long64)100);
    printf("%-1.5lld", (Long64)-100);
    printf("%5lld", (Long64)100);
    printf("%5lld", (Long64)-100);
    printf("%-5lld", (Long64)100);
    printf("%-5lld", (Long64)-100);
    printf("%-.5lld", (Long64)100);
    printf("%-.5lld", (Long64)-100);
    printf("%-8.5lld", (Long64)100);
    printf("%-8.5lld", (Long64)-100);
    printf("%05lld", (Long64)100);
    printf("%05lld", (Long64)-100);
    printf("% lld", (Long64)100);
    printf("% lld", (Long64)-100);
    printf("% 5lld", (Long64)100);
    printf("% 5lld", (Long64)-100);
    printf("% .5lld", (Long64)100);
    printf("% .5lld", (Long64)-100);
    printf("% 8.5lld", (Long64)100);
    printf("% 8.5lld", (Long64)-100);
    printf("%.0lld", (Long64)0);
    printf("%+25.22lld", (Long64)-1);
    printf("%-+8.5lld", (Long64)100);
    printf("%-+8.5lld", (Long64)100);
    printf("%.80lld", (Long64)1);
    printf("% .80lld", (Long64)1);
    printf("%lli", (Long64)100);
    printf("%-1d", 2);
    printf("%u", -1);
    printf("%lu", (unsigned long)-1);
    printf("%25.22llu", (Long64)-1);
    printf("%30.25llu", (Long64)-1);
    printf("%hu", 74565);
    printf("%hu", 291);
    printf("%4c", '1');
    printf("%-4c", '1');
    printf("%c", '\xDC');
    printf("%c%c%c", 'a', 'b', 'c');
    printf("%3c", 'a');
    printf("x%cx", 0x100+'X');
    printf("%#012x", 1);
    printf("%#4.8x", 1);
    printf("%#-8.2x", 1);
    printf("%#18.21llX", (Long64)-100);
    printf("%#21.18llx", (Long64)-100);
    printf("%x",  33065982);
    printf("%#x", 33065982);
    printf("%#X", 33065982);
    printf("%hx", 0x12345);
    printf("%hx", 0x123);
    printf("%hX", 0x1234a);
    printf("%hX", 0xa23);
    printf("%o", 10);
    printf("%#08o", 1);
    printf("%#-8.5llo", (Long64)100);
    printf("%#20.24llo", (Long64)-100);
    printf("%#24.20llo", (Long64)-100);
    printf("%#.25llo", (Long64)-100);
    printf("%ho", 0221505);
    printf("%ho", 0443);
    printf("%p", (void *)57);
    printf("%20p", (void *)57);
    printf("%-20p", (void *)57);
    printf("%p", (char*)0);
    printf("%s", "123");
    printf("%5s", "123");
    printf("%*s", 5, "123");
    printf("%s", "%%%%");
    printf("Hallo %s!", "Welt");
    printf("%1.3s", "Test");
    printf("%1.10s", "Test");
    printf("%12.3s", "Test");
    printf("%-12.3s", "Test");
    printf("%12s", "Test");
    printf("%-12s", "Test");
    printf("%4s", "foo");
    printf("%.1s", "foo");
    printf("%.*s", 1, "foo");
    printf("%*s", 5, "foo");
    int i1, i2;
    printf("asdf%nXYZ%n", &i1, &i2);
    short s1, s2;
    printf("asdf%hnXYZ%hn", &s1, &s2);
    long l1, l2;
    printf("asdf%lnXYZ%ln", &l1, &l2);
    printf("%c %hd %c%s%c", 'a', 3, 'b', "GGGG", 'c');
    printf("%c %hd %c%-40s%c", 'a', 3, 'b', "GGGG", 'c');
    printf("%1.1s%8s", "test", "12345678");
    printf("%g %s", 50000.1234, "Hallo");
    printf("Die %s auf alles: %d!", "Antwort", 42);
    printf("%04d%12s", 42, "Test");
    printf("%d %x %o %#x %#o", 100, 100, 100, 100, 100);
    printf("%d %x %o %#x %#o", -100, -100, -100, -100, -100);
    printf("%c\xDC %.2s", '\xDC', "ss");

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__
}

TEST(VarString_format)
{
    // the 64bit compiler will create 32-Bit values for "2" and "3" values instead of "64-Bit" causing a bad_alloc exception
    // we're now using vsnprintf for calculating the the length of the string instead, so this should not happen anymore.
    basar::VarString indexRowComment;

    indexRowComment.format(
        "%-*.*s"
        "%-*.*s"
        "%-*.*s"
        ,
        9, 9, "INDEXROW",
        2, 2, "DE",
        3, 3, "D"
    );

    CHECK_EQUAL("INDEXROW DED  ", indexRowComment.c_str());
}

//----------------------------------------------------------------------------
class StringFix
{
public:
    BString    bStr;
    I18nString iStr;

    void cheq(ConstString exp)
    {
        CHECK_EQUAL(exp, bStr.c_str());
        CHECK_EQUAL(exp, iStr.c_str());
    }
    void cheq(ConstString        exp, 
              const BString    & bval, 
              const I18nString & ival)
    {
        CHECK_EQUAL(exp, bval.c_str());
        CHECK_EQUAL(exp, ival.c_str());
    }

    void cheq(BString::size_type exp, 
              BString::size_type bval, 
              BString::size_type ival)
    {
        CHECK_EQUAL((int)exp, (int)bval);
        CHECK_EQUAL((int)exp, (int)ival);
    }

    void cheq(basar::Int64 exp,
              basar::Int64 bval,
              basar::Int64 ival)
    {
        CHECK_EQUAL(exp, bval);
        CHECK_EQUAL(exp, ival);
    }

    void cheq(int exp, 
              int bval, 
              int ival)
    {
        CHECK_EQUAL(exp, bval);
        CHECK_EQUAL(exp, ival);
    }

#ifdef BASAR_ENV_64BIT
    void cheq(int exp, 
              basar::BLong bval, 
              basar::BLong ival)
    {
        CHECK_EQUAL(exp, bval);
        CHECK_EQUAL(exp, ival);
    }
#endif

    void cheq(Float64 exp, 
              Float64 bval, 
              Float64 ival)
    {
        CHECK_CLOSE(exp, bval, 0.00001);
        CHECK_CLOSE(exp, ival, 0.00001);
    }

    void chtrue(bool bcond,
                bool icond)
    {
        CHECK(bcond);
        CHECK(icond);
    }

    void chfalse(bool bcond,
                 bool icond)
    {
        CHECK(!bcond);
        CHECK(!icond);
    }
};

//------------------------------------------------------------------------------
#define ASSFIX(val)          { bStr = (val); \
                               iStr = (val); }
#define CALLFIX(call)        { bStr.call; \
                               iStr.call; }
#define ASSCALLFIX(val, call){ ASSFIX(val); \
                               CALLFIX(call); }

#define CHTRUEFIX(val, call) { ASSFIX(val); \
                               chtrue(bStr.call, iStr.call); }
#define CHFALSEFIX(val, call){ ASSFIX(val); \
                               chfalse(bStr.call, iStr.call); }
#define CHEQFIX(exp, call)   { cheq(exp, bStr.call, iStr.call); }


//------------------------------------------------------------------------------
TEST_FIXTURE(StringFix, Ctor_cmn)
{
    {
        BString    bstr;
        I18nString istr;
        cheq("", bstr, istr);
    }

    {
        BString    bstr('b');
        I18nString istr('b');
        cheq("b", bstr, istr);
    }

    {
	    BString::value_type arg = 0xDCu;	// 0xDC = 'Ü'
        BString             bstr(arg);
        I18nString          istr(arg);
        cheq("\xDC", bstr, istr);
    }

    {
        BString    bstr("test");
        I18nString istr("test");
        cheq("test", bstr, istr);
    }

    {
	    CharString arg = "copyCharString";
        BString    bstr(arg);
        I18nString istr(arg);
        cheq("copyCharString", bstr, istr);
    }

    BString arg = "copyBString";

    {
        BString    bstr(arg);
        I18nString istr(arg);
        cheq("copyBString", bstr, istr);
    }

    {
        BString    bstr(arg, 0, 1);
        I18nString istr(arg, 0, 1);
        cheq("c", bstr, istr);
    }

    {
        BString    bstr(arg, 1, 1);
        I18nString istr(arg, 1, 1);
        cheq("o", bstr, istr);
    }

    {
        BString    bstr(arg, 1, 40);
        I18nString istr(arg, 1, 40);
        cheq("opyBString", bstr, istr);
    }

    {
        BString    bstr(arg.begin() + 1, arg.end() - 1);
        I18nString istr(arg.begin() + 1, arg.end() - 1);
        cheq("opyBStrin", bstr, istr);
    }

	CHECK_THROW(BString   (arg, 40, 1), std::out_of_range);
	CHECK_THROW(I18nString(arg, 40, 1), std::out_of_range);
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, Ctor_ext)
{
    {
	    I18nString::value_type arg = 0xDCu;	// 0xDC = 'Ü'
        I18nString             istr(arg);
        CHECK_EQUAL("\xDC", istr.c_str());
    }

    I18nString arg = "copyI18nString";

    {
        I18nString istr(arg);
        CHECK_EQUAL("copyI18nString", istr.c_str());
    }

    {
        I18nString istr(arg, 0, 1);
        CHECK_EQUAL("c", istr.c_str());
    }

    {
        I18nString istr(arg, 1, 1);
        CHECK_EQUAL("o", istr.c_str());
    }

    {
        I18nString istr(arg, 1, 40);
        CHECK_EQUAL("opyI18nString", istr.c_str());
    }

    {
        I18nString istr(arg.begin() + 1, arg.end() - 1);
        CHECK_EQUAL("opyI18nStrin", istr.c_str());
    }

    CHECK_THROW(I18nString(arg, 40, 1), std::out_of_range);
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, leftright)
{
	ASSFIX("0123456789");

	CHEQFIX("012"       , left ('3'));
	CHEQFIX("456789"    , right('3'));
	CHEQFIX("0123456789", left ('a'));

    ASSFIX("0123456789A");
    CHEQFIX("0123456789A", right('a'));

	CHEQFIX("012"        , left ("345"));
	CHEQFIX("6789A"      , right("345"));
	CHEQFIX("0123456789A", left ("35" ));
    
    ASSFIX("0123456789AB");
	CHEQFIX("0123456789AB", right("35" ));

    ASSFIX("\xDC");
	CHEQFIX(""    , right(-36));	
	CHEQFIX("\xDC", right("35"));
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, substring)
{
	ASSFIX("0123456789");

	CHEQFIX("0123456789", substr(0, 100));
	CHEQFIX("01234567"  , substr(0,   8));
	CHEQFIX("456789"    , substr(4, 100));
	CHEQFIX("45"        , substr(4,   2));

	CHECK_THROW(bStr.substr(100, 3), std::out_of_range);
	CHECK_THROW(iStr.substr(100, 3), std::out_of_range);

    ASSFIX("\xDC");
	CHEQFIX("\xDC", substr(0, 1)); 
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, digit)
{
	CHTRUEFIX("123"  , isDigit());
	CHTRUEFIX(" 123" , isDigit());
	CHTRUEFIX(" 123 ", isDigit());
	CHTRUEFIX(" -123", isDigit());

    CHFALSEFIX(""     , isDigit());
    CHFALSEFIX("     ", isDigit());
    CHFALSEFIX("-"    , isDigit());
	CHFALSEFIX(" 123-", isDigit());
	CHFALSEFIX(" 123f", isDigit());
	CHFALSEFIX("abcde", isDigit());
	CHFALSEFIX("Z"    , isDigit());
	CHFALSEFIX("\xDF" , isDigit());
	CHFALSEFIX("\xF6" , isDigit());
	CHFALSEFIX("\xDC" , isDigit());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, fromStringToFloatDec)
{
	const Float64 f = 1234.567;

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
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
            ASSFIX("1234.567");
			break;

		default:
            bStr = "1234.567";
            iStr = "1234,567";
			break;
        }
        
        CHEQFIX(f, stof());
	}
}

TEST_FIXTURE(StringFix, ftosPrecision)
{
    Locale::setLocale(Locale::C);
    {
        const Float64 f = 1234.567;
        CALLFIX(ftos(f));
        cheq("1234.567");

    }
    {
        const Float64 f(1234.5678);
        CALLFIX(ftos(f));
        cheq("1234.5678");
    }
    {
        const Float64 f(1234.56789);
        CALLFIX(ftos(f));
        cheq("1234.56789");
    }
    {
        const Float64 f(1234.567891);
        CALLFIX(ftos(f));
        cheq("1234.567891");
    }
    {
        const Float64 f(1234.5678912);
        CALLFIX(ftos(f,7));
        cheq("1234.5678912");
    }
    {
        const Float64 f(1234.56789123);
        CALLFIX(ftos(f,8));
        cheq("1234.56789123");
    }
    {
        const Float64 f(1234.56789163);
        CALLFIX(ftos(f,8));
        cheq("1234.56789163");
    }
    {
        const Float64 f(1234.567891634);
        CALLFIX(ftos(f,9));
        cheq("1234.567891634");
    }
    {
        const Float64 f(1234.5678916345);
        CALLFIX(ftos(f,10));
        cheq("1234.5678916345");
    }
    {
        const Float64 f(1234.56789163456);
        CALLFIX(ftos(f,11));
        cheq("1234.56789163456");
    }
    {
        const Float64 f(1234.567891634567);
        CALLFIX(ftos(f,12));
        cheq("1234.567891634567");
    }
    {
        const Float64 f(1234.5678916345678);
        CALLFIX(ftos(f,13));
        cheq("1234.5678916345678");
    }
    {
        const Float64 f(0.56789163456789);
        CALLFIX(ftos(f,14));
        cheq("0.56789163456789");
    }
    {
        const Float64 f(0.567891634567891);
        CALLFIX(ftos(f,15));
        cheq("0.567891634567891");
    }
    {
        const Float64 f(0.5678916345678912);
        CALLFIX(ftos(f,16));
        cheq("0.5678916345678912");
    }
    {
        //
        const Float64 f(0.56789163456789127);
        CALLFIX(ftos(f,17));
        cheq("0.56789163456789127");
    }
    //{
    //    //overshooting precision
    //    const Float64 f(0.56789163);
    //    CALLFIX(ftos(f,256));
    //    cheq("1234.56789163");
    //}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, fromFloatToString)
{
	const Float64 f = 1234.567;

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

        CALLFIX(ftos(f));

		switch (j)
		{
		case Locale::C:
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
		    cheq("1234.567");
			break;

		default:
			CHECK_EQUAL("1234.567", bStr.c_str());
			CHECK_EQUAL("1234,567", iStr.c_str());
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, fromStringTo)
{
    ASSFIX("45678901");
	CHEQFIX(45678901, stoi());

    ASSFIX("995678901");
    CHEQFIX(995678901, stol());

    ASSFIX("-9223372036854775806");
    CHEQFIX(-9223372036854775806, stoll());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, toString)
{
	CALLFIX(itos(45678901));
	cheq("45678901");

	CALLFIX(ltos(22678901));
	cheq("22678901");

    CALLFIX(lltos(-9223372036854775806));
    cheq("-9223372036854775806");
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, upper)
{
	ConstString ascii     = "ABCDEFabcdef012345;:-_# '"; 
	ConstString ascii_up  = "ABCDEFABCDEF012345;:-_# '";

#ifdef _WIN32
	ConstString extend    = "\x80\xB5\xC4\xE4\xDC\xFC\xDF\xCF\x9F\xB8\x8A";
	ConstString west_up   = "\x80\xB5\xC4\xC4\xDC\xDC\xDF\xCF\x9F\xB8\x8A";
	ConstString east_up   = "\x80\xB5\xC4\xC4\xDC\xDC\xDF\xCF\x8F\xB8\x8A";
	ConstString cyr_up    = "\x80\xB5\xC4\xC4\xDC\xDC\xDF\xCF\x8F\xA8\x8A";
#else
	ConstString extend    = "\xA4\xB5\xC4\xE4\xDC\xFC\xDF\xCF\xBE\xAF";
	ConstString west_up   = "\xA4\xB5\xC4\xC4\xDC\xDC\xDF\xCF\xBE\xAF";
	ConstString east_up   = "\xA4\xA5\xC4\xC4\xDC\xDC\xDF\xCF\xAE\xAF";
	ConstString cyr_up    = "\xA4\xB5\xC4\xC4\xBC\xAC\xBF\xCF\xBE\xAF";
#endif

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

        // ASCII
        ASSCALLFIX(ascii, upper());
        cheq(ascii_up);

        // codepage
        ASSCALLFIX(extend, upper());
        CHECK_EQUAL(extend, bStr.c_str());

		switch (j)
		{
		case Locale::BULGARIAN_BG:
			CHECK_EQUAL(cyr_up, iStr.c_str());
			break;

		case Locale::CROATIAN_HR:
		case Locale::CZECH_CZ:
		case Locale::BOSNIAN_BA:
		case Locale::SERBIAN_RS:
			CHECK_EQUAL(east_up, iStr.c_str());
			break;

		case Locale::C:
			CHECK_EQUAL(extend, iStr.c_str());
			break;

		default:
			CHECK_EQUAL(west_up, iStr.c_str());
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, lower)
{
	ConstString ascii     = "ABCDEFabcdef012345;:-_# '"; 
	ConstString ascii_low = "abcdefabcdef012345;:-_# '";

#ifdef _WIN32
	ConstString extend    = "\x80\xB5\xC4\xE4\xDC\xFC\xDF\xCF\x9F\xB8\x8A";
    ConstString west_low  = "\x80\xB5\xE4\xE4\xFC\xFC\xDF\xEF\xFF\xB8\x9A";
	ConstString east_low  = "\x80\xB5\xE4\xE4\xFC\xFC\xDF\xEF\x9F\xB8\x9A";
	ConstString cyr_low   = "\x90\xB5\xE4\xE4\xFC\xFC\xFF\xEF\x9F\xB8\x9A";
#else
	ConstString extend    = "\xA4\xB5\xC4\xE4\xDC\xFC\xDF\xCF\xBE\xAF";
	ConstString west_low  = "\xA4\xB5\xE4\xE4\xFC\xFC\xDF\xEF\xBE\xAF";
	ConstString east_low  = "\xA4\xB5\xE4\xE4\xFC\xFC\xDF\xEF\xBE\xBF";
	ConstString cyr_low   = "\xF4\xD5\xE4\xE4\xDC\xFC\xDF\xEF\xDE\xFF";
#endif

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

        // ASCII
        ASSCALLFIX(ascii, lower());
        cheq(ascii_low);

        // codepage
        ASSCALLFIX(extend, lower());
        CHECK_EQUAL(extend, bStr.c_str());

		switch (j)
		{
		case Locale::BULGARIAN_BG:
			CHECK_EQUAL(cyr_low, iStr.c_str());
			break;

		case Locale::CROATIAN_HR:
		case Locale::CZECH_CZ:
		case Locale::BOSNIAN_BA:
		case Locale::SERBIAN_RS:
			CHECK_EQUAL(east_low, iStr.c_str());
			break;

		case Locale::C:
			CHECK_EQUAL(extend, iStr.c_str());
			break;

		default:
			CHECK_EQUAL(west_low, iStr.c_str());
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, clean)
{
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

        ASSCALLFIX("abc\tde\r\nfg\nhi" "\x7F\xDC\xEC" "jkl", 
                   cleanchar());
        CHECK_EQUAL("abc de  fg hi   jkl", bStr.c_str());
        CHECK_EQUAL((Locale::C == j) ? "abc de  fg hi   jkl"
                                     : "abc de  fg hi \xDC\xECjkl", 
                    iStr.c_str());

        ASSCALLFIX("a b" "\x10" "c" "\xB5" "d" "\xC4\x0", 
                   cleanchar());
        CHECK_EQUAL("a b c d ", bStr.c_str());
        CHECK_EQUAL((Locale::C == j) ? "a b c d "
                                     : "a b c\xB5" "d\xC4", 
                    iStr.c_str());

        ASSCALLFIX("\xDC", 
                   cleanchar());
        CHECK_EQUAL(" ", bStr.c_str());
        CHECK_EQUAL((Locale::C == j) ? " "
                                     : "\xDC", 
                    iStr.c_str());
    }
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, reverse)
{
	ASSCALLFIX("abcde", reverse());
	cheq("edcba");

	ASSCALLFIX("abcde", reverse(1));
	cheq("aedcb");

	ASSCALLFIX("abcde", reverse(7));
	cheq("abcde");

	ASSCALLFIX("\xDC\xD0", reverse());
	cheq("\xD0\xDC");	
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, exchange)
{
	ConstString c = "abcXdeXfg";
	ConstString d = "XdeX";
	ConstString e = "abcDEfghDEEijkDEEDlmn";
    ConstString f = "\xDC";
	
	ASSCALLFIX(c, exchange('X', "Y"));
    cheq("abcYdeYfg");

	ASSCALLFIX(c, exchange('X', "MNO"));
	cheq("abcMNOdeMNOfg");

	ASSCALLFIX(c, exchange("X", "Y"));
	cheq("abcYdeYfg");

	ASSCALLFIX(c, exchange("X", "MNO"));
	cheq("abcMNOdeMNOfg");

	ASSCALLFIX(d, exchange('X', "Y"));
	cheq("YdeY");

	ASSCALLFIX(d, exchange('X', "MNO"));
	cheq("MNOdeMNO");

	ASSCALLFIX(d, exchange('X', ""));
	cheq("de");

	ASSCALLFIX(d, exchange("X", "Y"));
	cheq("YdeY");

	ASSCALLFIX(d, exchange("X", "MNO"));
	cheq("MNOdeMNO");

	ASSCALLFIX(d, exchange("X", ""));
	cheq("de");

	ASSCALLFIX(e, exchange("DE", "XYZABC"));
	cheq("abcXYZABCfghXYZABCEijkXYZABCEDlmn");

	ASSCALLFIX(e, exchange("DE", "DEDE"));
	cheq("abcDEDEfghDEDEEijkDEDEEDlmn");

	ASSCALLFIX(e, exchange("DE", "XYZABCD"));
	cheq("abcXYZABCDfghXYZABCDEijkXYZABCDEDlmn");

	ASSCALLFIX(f, exchange(0xDCu, "XYZ"));
	cheq("XYZ");
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, remove)
{
	ConstString a = "";
	ConstString b = " ";
	ConstString c = " bc n ";
	ConstString d = " bc bbcc ";
    ConstString e = "\xDC";
	
	ASSCALLFIX(a, remove(""));
	cheq("");

	ASSCALLFIX(a, remove(" "));
	cheq("");
	
	ASSCALLFIX(b, remove(""));
	cheq(" ");

	ASSCALLFIX(b, remove(" "));
	cheq("");

	ASSCALLFIX(c, remove(" "));
	cheq("bcn");

	ASSCALLFIX(c, remove("  "));
	cheq(" bc n ");

	ASSCALLFIX(d, remove("bc"));
	cheq("  bc ");

	ASSCALLFIX(e, remove(BString(0xDCu)));
	cheq("");
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, count)
{
	ConstString a = "";
	ConstString b = " ";
	ConstString c = "  ";
	ConstString d = " b  ";
    ConstString e = "\xDC";

	ASSFIX(a);
	CHEQFIX(0u, getCount(' '));

	ASSFIX(b);
	CHEQFIX(1u, getCount(' '));

	ASSFIX(c);
	CHEQFIX(2u, getCount(' '));

	ASSFIX(d);
	CHEQFIX(3u, getCount(' '));

	ASSFIX(d);
	CHEQFIX(0u, getCount(-1));

	ASSFIX(e);
	CHEQFIX(1u, getCount(0xDCu));
}


//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, signedChar)
{
	BString   ::value_type bvt = 0xDCu;
	I18nString::value_type ivt = 0xDCu;

#ifdef _WIN32
	CHECK(bvt  < 0);	// => signed char
	CHECK(ivt  < 0);	// => signed char
#else
#ifdef _AIX
	CHECK(bvt  > 0);	// => unsigned char
	CHECK(ivt  > 0);	// => unsigned char
#else
	CHECK((bvt  < 0);	// => signed char
	CHECK((ivt  < 0);	// => signed char
#endif
#endif
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, trim)
{
	static ConstString INPUT[] = 
	{
		""                ,	" "                ,
		"  "              , "a"                ,
		"ab"              , "a b"              ,
		" a  b "          , "                 ",
        "                ", "               "  ,
        "              "  , "             "    ,
		"\xDC"
	};

	static ConstString RESULT0[] = 
	{
		""  , ""  , ""  , "a",
		"ab", "ab", "ab", "" ,
        ""  , ""  , ""  , "" ,
		"\xDC"
	};

	static ConstString RESULT1[] = 
	{
		""  , ""   , ""     , "a",
		"ab", "a b", "a  b ", "",
        ""  , ""   , ""     , "",
		"\xDC"
	};
	static ConstString RESULT2[] = 
	{
		""  , ""   , ""     , "a",
		"ab", "a b", " a  b", "",
        ""  , ""   , ""     , "",
		"\xDC"
	};
	static ConstString RESULT3[] = 
	{
		""  , ""   , ""    , "a",
		"ab", "a b", "a  b", "",
        ""  , ""   , ""    , "",
		"\xDC"
	};


	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		Locale::setLocale(j);

		for (unsigned int i = 0; i < sizeof(INPUT)/sizeof(INPUT[0]); ++i)
		{
			ASSCALLFIX(INPUT[i], trim(BString::ALL));
			cheq(RESULT0[i]);

			ASSCALLFIX(INPUT[i], trim(BString::FIRST));
			cheq(RESULT1[i]);

			ASSCALLFIX(INPUT[i], trim(BString::END));
			cheq(RESULT2[i]);

			ASSCALLFIX(INPUT[i], trim(BString::FIRST_END));
			cheq(RESULT3[i]);
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_noparam)
{
	static ConstString FORMAT[] = 
	{
		""                , " ",
		"A"               , "ABC",
		"12345678901234"  , "123456789012345",
		"1234567890123456", "12345678901234567"
	};

	static const BString::size_type SIZE[] = 
	{
	    0 ,  1,  1,  3, 
        14, 15, 16, 17
	};

	CHECK(sizeof(FORMAT)/sizeof(*FORMAT) == sizeof(SIZE  )/sizeof(*SIZE  ));

	for (unsigned int i = 0; i < sizeof(FORMAT)/sizeof(*FORMAT); ++i)
	{
		CALLFIX(format(FORMAT[i]));
        cheq(FORMAT[i]);
		CHEQFIX(SIZE[i], size());
	}

	ASSCALLFIX("", format(0));
	cheq("");
	CHEQFIX(0u, size());

    CALLFIX(format("test%%"));
    cheq("test%");
	CHEQFIX(5u, size());

    CALLFIX(format("%%0"));
    cheq("%0");
	CHEQFIX(2u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_int)
{
	CALLFIX(format("%d", 123));
	cheq("123");
	CHEQFIX(3u, size());

    CALLFIX(format("% d", 1));
    cheq( " 1");
	CHEQFIX(2u, size());

    CALLFIX(format("%+d", 1));
    cheq( "+1");
	CHEQFIX(2u, size());

    CALLFIX(format("%d", -1));
    cheq("-1");
	CHEQFIX(2u, size());

    CALLFIX(format("%10d" , 1977));
    cheq("      1977");
	CHEQFIX(10u, size());

    CALLFIX(format("%010d", 1977));
    cheq("0000001977");
	CHEQFIX(10u, size());

    CALLFIX(format("%10d" , -1977));
    cheq("     -1977");
	CHEQFIX(10u, size());

    CALLFIX(format("%010d", -1977));
    cheq("-000001977");
	CHEQFIX(10u, size());

    CALLFIX(format("%04d, %05d", -3, 4));
    cheq("-003, 00004");
	CHEQFIX(11u, size());

    CALLFIX(format("%04d", 42));
    cheq("0042");
	CHEQFIX(4u, size());

    CALLFIX(format("%012d", 42));
	cheq("000000000042");
	CHEQFIX(12u, size());

    CALLFIX(format("%0*d", 4, 42));
	cheq("0042");
	CHEQFIX(4u, size());

	CALLFIX(format("%0*d", 12, 42));
	cheq("000000000042");
	CHEQFIX(12u, size());

    CALLFIX(format("%3d", 1234));
    cheq("1234");
	CHEQFIX(4u, size());

    CALLFIX(format("% .80d", 1));
	CHEQFIX(81u, size());

    CALLFIX(format("%hd", 74565));
    cheq("9029");
	CHEQFIX(4u, size());

    CALLFIX(format("%hd", 291));
    cheq("291");
	CHEQFIX(3u, size());

    // ---
    CALLFIX(format("%-i", -1));
    cheq("-1");
	CHEQFIX(2u, size());

    CALLFIX(format("%-i", 1));
    cheq("1");
	CHEQFIX(1u, size());

    CALLFIX(format("%+i", 1));
    cheq("+1");
	CHEQFIX(2u, size());

    CALLFIX(format("%hi", 74565));
    cheq("9029");
	CHEQFIX(4u, size());

    CALLFIX(format("%hi", 291));
    cheq("291");
	CHEQFIX(3u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_double)
{
    CALLFIX(format("%2.4e", 8.6));
    CHECK_EQUAL("8.6000e+00", bStr.c_str());
    CHECK_EQUAL("8,6000e+00", iStr.c_str());
	CHEQFIX(10u, size());

    CALLFIX(format("% 2.4e", 8.6));
    CHECK_EQUAL(" 8.6000e+00", bStr.c_str());
    CHECK_EQUAL(" 8,6000e+00", iStr.c_str());
	CHEQFIX(11u, size());

    CALLFIX(format("% 014.4e", 8.6));
    CHECK_EQUAL(" 0008.6000e+00", bStr.c_str());
    CHECK_EQUAL(" 0008,6000e+00", iStr.c_str());
	CHEQFIX(14u, size());

    CALLFIX(format("% 2.4e", -8.6));
    CHECK_EQUAL("-8.6000e+00", bStr.c_str());
    CHECK_EQUAL("-8,6000e+00", iStr.c_str());
	CHEQFIX(11u, size());

    CALLFIX(format("%+2.4e", 8.6));
    CHECK_EQUAL("+8.6000e+00", bStr.c_str());
    CHECK_EQUAL("+8,6000e+00", iStr.c_str());
	CHEQFIX(11u, size());

    CALLFIX(format("%+#23.15e", 789456123.0));
    CHECK_EQUAL(" +7.894561230000000e+08", bStr.c_str());
    CHECK_EQUAL(" +7,894561230000000e+08", iStr.c_str());
	CHEQFIX(23u, size());

    CALLFIX(format("%-#23.15e", 789456123.0));
    CHECK_EQUAL("7.894561230000000e+08  ", bStr.c_str());
    CHECK_EQUAL("7,894561230000000e+08  ", iStr.c_str());
	CHEQFIX(23u, size());

    CALLFIX(format("%#23.15e", 789456123.0));
    CHECK_EQUAL("  7.894561230000000e+08", bStr.c_str());
    CHECK_EQUAL("  7,894561230000000e+08", iStr.c_str());
	CHEQFIX(23u, size());
    // ---
    CALLFIX(format("%#1.1g", 789456123.0));
    CHECK_EQUAL("8.e+08", bStr.c_str());
    CHECK_EQUAL("8,e+08", iStr.c_str());
	CHEQFIX(6u, size());

    CALLFIX(format("%4.2f %+.0e %E", 3.1416, 3.1416, 3.1416));
    CHECK_EQUAL("3.14 +3e+00 3.141600E+00", bStr.c_str());
    CHECK_EQUAL("3,14 +3e+00 3,141600E+00", iStr.c_str());
	CHEQFIX(24u, size());

    CALLFIX(format("%4.2f %+.0e %E", -3.1416, -3.1416, -3.1416));
    CHECK_EQUAL("-3.14 -3e+00 -3.141600E+00", bStr.c_str());
    CHECK_EQUAL("-3,14 -3e+00 -3,141600E+00", iStr.c_str());
	CHEQFIX(26u, size());

    CALLFIX(format("%Le", (long double)-1.1));
    CHECK_EQUAL("-1.100000e+00", bStr.c_str());
    CHECK_EQUAL("-1,100000e+00", iStr.c_str());
	CHEQFIX(13u, size());

    CALLFIX(format("%e and %f", 50000.1234, 1.23));
	CHECK_EQUAL("5.000012e+04 and 1.230000" , bStr.c_str());
	CHECK_EQUAL("5,000012e+04 and 1,230000" , iStr.c_str());

    // --- %f
    CALLFIX(format("%f", 50000.1234));
    CHECK_EQUAL("50000.123400", bStr.c_str());
    CHECK_EQUAL("50000,123400", iStr.c_str());

    CALLFIX(format("%.4f", 50000.1234));
    CHECK_EQUAL("50000.1234", bStr.c_str());
    CHECK_EQUAL("50000,1234", iStr.c_str());

    CALLFIX(format("%2.4f", 8.6));
    CHECK_EQUAL("8.6000", bStr.c_str());
    CHECK_EQUAL("8,6000", iStr.c_str());
	CHEQFIX(6u, size());

    CALLFIX(format("%0f", 0.6));
    CHECK_EQUAL("0.600000", bStr.c_str());
    CHECK_EQUAL("0,600000", iStr.c_str());
	CHEQFIX(8u, size());

    CALLFIX(format("%.0f", 0.6));
    cheq("1");
	CHEQFIX(1u, size());

    CALLFIX(format("%Lf", (long double)-1.1));
    CHECK_EQUAL("-1.100000", bStr.c_str());
    CHECK_EQUAL("-1,100000", iStr.c_str());
	CHEQFIX(9u, size());

    // --- %g
    CALLFIX(format("%2.4g", 8.6));
    CHECK_EQUAL("8.6", bStr.c_str());
    CHECK_EQUAL("8,6", iStr.c_str());
	CHEQFIX(3u, size());

    CALLFIX(format("%Lg", (long double)-1.1));
    CHECK_EQUAL("-1.1", bStr.c_str());
    CHECK_EQUAL("-1,1", iStr.c_str());
	CHEQFIX(4u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_double_length)
{
	CALLFIX(format("%f", 123.456));
	CHECK_EQUAL("123.456000", bStr.c_str());
	CHECK_EQUAL("123,456000", iStr.c_str());
	CHEQFIX(10u, size());

	CALLFIX(format("%05f", 123.456));
	CHECK_EQUAL("123.456000", bStr.c_str());
	CHECK_EQUAL("123,456000", iStr.c_str());
	CHEQFIX(10u, size());

	CALLFIX(format("%0*f", 5, 123.456));
	CHECK_EQUAL("123.456000", bStr.c_str());
	CHECK_EQUAL("123,456000", iStr.c_str());
	CHEQFIX(10u, size());

	CALLFIX(format("%.5f", 123.456));
	CHECK_EQUAL("123.45600", bStr.c_str());
	CHECK_EQUAL("123,45600", iStr.c_str());
	CHEQFIX(9u, size());

	CALLFIX(format("%.*f", 5, 123.456));
	CHECK_EQUAL("123.45600", bStr.c_str());
	CHECK_EQUAL("123,45600", iStr.c_str());
	CHEQFIX(9u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_double_locale)
{
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
		case Locale::GERMAN_DE:
		case Locale::GERMAN_AT:
		case Locale::CROATIAN_HR:
		case Locale::FRENCH_FR:
		case Locale::BULGARIAN_BG:
        case Locale::BOSNIAN_BA:
        case Locale::SERBIAN_RS:
        case Locale::CZECH_CZ:
            CALLFIX(format("<%e>"  , 123.456));
			CHECK_EQUAL("<1.234560e+02>" , bStr.c_str());
			CHECK_EQUAL("<1,234560e+02>" , iStr.c_str());

            CALLFIX(format("<%.3f>", 123.456));
            CHECK_EQUAL("<123.456>", bStr.c_str());
            CHECK_EQUAL("<123,456>", iStr.c_str());

            CALLFIX(format("<%g>" , 123.456));
		    CHECK_EQUAL("<123.456>", bStr.c_str());
		    CHECK_EQUAL("<123,456>", iStr.c_str());

            break;

        default:
            CALLFIX(format("<%e>"  , 123.456));
            cheq("<1.234560e+02>");

            CALLFIX(format("<%.3f>", 123.456));
            cheq("<123.456>");

            CALLFIX(format("<%g>" , 123.456));
		    cheq("<123.456>");

            break;
        }
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_long)
{
    CALLFIX(format("%ld", ((ULong64)0xffffffff)*0xffffffff));
/*! \ingroup DIFF_WIN_UNIX */
// it seems that the AIX handles %ld as %lld, since with %ld a "1" should be the result, with %lld the "-8589934591" ...
// VS2015 on x64 handles this correctly!
#ifndef _WIN64
	if (sizeof(size_t) == 8)
    {
        cheq("-8589934591");
	    CHEQFIX(11u, size());
    }
    else
#endif // !_WIN64
    {
        cheq("1");
	    CHEQFIX(1u, size());
    }

    CALLFIX(format("%+8ld", (Long64)100));
    cheq("    +100");
	CHEQFIX(8u, size());

    CALLFIX(format("%+.8ld", (Long64)100));
    cheq("+00000100");
	CHEQFIX(9u, size());

    CALLFIX(format("%+10.8ld", (Long64)100));
    cheq(" +00000100");
	CHEQFIX(10u, size());

    CALLFIX(format("%-1.5ld", (Long64)-100));
    cheq("-00100");
	CHEQFIX(6u, size());

    CALLFIX(format("%5ld", (Long64)100));
    cheq("  100");
	CHEQFIX(5u, size());

    CALLFIX(format("%5ld", (Long64)-100));
    cheq(" -100");
	CHEQFIX(5u, size());

    CALLFIX(format("%-5ld", (Long64)100));
    cheq("100  ");
	CHEQFIX(5u, size());

    CALLFIX(format("%-5ld", (Long64)-100));
    cheq("-100 ");
	CHEQFIX(5u, size());

    CALLFIX(format("%-.5ld", (Long64)100));
    cheq("00100");
	CHEQFIX(5u, size());

    CALLFIX(format("%-.5ld", (Long64)-100));
    cheq("-00100");
	CHEQFIX(6u, size());

    CALLFIX(format("%-8.5ld", (Long64)100));
    cheq("00100   ");
	CHEQFIX(8u, size());

    CALLFIX(format("%-8.5ld", (Long64)-100));
    cheq("-00100  ");
	CHEQFIX(8u, size());

    CALLFIX(format("%05ld", (Long64)100));
    cheq("00100");
	CHEQFIX(5u, size());

    CALLFIX(format("%05ld", (Long64)-100));
    cheq("-0100");
	CHEQFIX(5u, size());

    CALLFIX(format("% ld", (Long64)100));
    cheq(" 100");
	CHEQFIX(4u, size());

    CALLFIX(format("% ld", (Long64)-100));
    cheq("-100");
	CHEQFIX(4u, size());

    CALLFIX(format("% 5ld", (Long64)100));
    cheq("  100");
	CHEQFIX(5u, size());

    CALLFIX(format("% 5ld", (Long64)-100));
    cheq(" -100");
	CHEQFIX(5u, size());

    CALLFIX(format("% .5ld", (Long64)100));
    cheq(" 00100");
	CHEQFIX(6u, size());

    CALLFIX(format("% .5ld", (Long64)-100));
    cheq("-00100");
	CHEQFIX(6u, size());

    CALLFIX(format("% 8.5ld", (Long64)100));
    cheq("   00100");
	CHEQFIX(8u, size());

    CALLFIX(format("% 8.5ld", (Long64)-100));
    cheq("  -00100");
	CHEQFIX(8u, size());

    CALLFIX(format("%.0ld", (Long64)0));
    cheq("");
	CHEQFIX(0u, size());

    CALLFIX(format("%+25.22ld", (Long64)-1));
    cheq("  -0000000000000000000001");
	CHEQFIX(25u, size());

    CALLFIX(format("%-+8.5ld", (Long64)100));
    cheq("+00100  ");
	CHEQFIX(8u, size());

    CALLFIX(format("%-+8.5ld", (Long64)100));
    cheq("+00100  ");
	CHEQFIX(8u, size());

    CALLFIX(format("%.80ld", (Long64)1));
	CHEQFIX(80u, size());

    CALLFIX(format("% .80ld", (Long64)1));
	CHEQFIX(81u, size());

    CALLFIX(format("%-1d", 2));
    cheq("2");
	CHEQFIX(1u, size());

    CALLFIX(format("% li", (Long64)100));
    cheq(" 100");
	CHEQFIX(4u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_int64)
{
    CALLFIX(format("%lld", -9223372036854775806));
    cheq("-9223372036854775806");

    CALLFIX(format("%lld", ((ULong64)0xffffffff) * 0xffffffff));
    cheq("-8589934591");
    CHEQFIX(11u, size());

    CALLFIX(format("%+8lld", (Int64)100));
    cheq("    +100");
    CHEQFIX(8u, size());

    CALLFIX(format("%+.8lld", (Int64)100));
    cheq("+00000100");
    CHEQFIX(9u, size());

    CALLFIX(format("%+10.8lld", (Int64)100));
    cheq(" +00000100");
    CHEQFIX(10u, size());

    CALLFIX(format("%-1.5lld", (Int64)-100));
    cheq("-00100");
    CHEQFIX(6u, size());

    CALLFIX(format("%5lld", (Int64)100));
    cheq("  100");
    CHEQFIX(5u, size());

    CALLFIX(format("%5lld", (Int64)-100));
    cheq(" -100");
    CHEQFIX(5u, size());

    CALLFIX(format("%-5lld", (Int64)100));
    cheq("100  ");
    CHEQFIX(5u, size());

    CALLFIX(format("%-5lld", (Int64)-100));
    cheq("-100 ");
    CHEQFIX(5u, size());

    CALLFIX(format("%-.5lld", (Int64)100));
    cheq("00100");
    CHEQFIX(5u, size());

    CALLFIX(format("%-.5lld", (Int64)-100));
    cheq("-00100");
    CHEQFIX(6u, size());

    CALLFIX(format("%-8.5lld", (Int64)100));
    cheq("00100   ");
    CHEQFIX(8u, size());

    CALLFIX(format("%-8.5lld", (Int64)-100));
    cheq("-00100  ");
    CHEQFIX(8u, size());

    CALLFIX(format("%05lld", (Int64)100));
    cheq("00100");
    CHEQFIX(5u, size());

    CALLFIX(format("%05lld", (Int64)-100));
    cheq("-0100");
    CHEQFIX(5u, size());

    CALLFIX(format("% lld", (Int64)100));
    cheq(" 100");
    CHEQFIX(4u, size());

    CALLFIX(format("% lld", (Int64)-100));
    cheq("-100");
    CHEQFIX(4u, size());

    CALLFIX(format("% 5lld", (Int64)100));
    cheq("  100");
    CHEQFIX(5u, size());

    CALLFIX(format("% 5lld", (Int64)-100));
    cheq(" -100");
    CHEQFIX(5u, size());

    CALLFIX(format("% .5lld", (Int64)100));
    cheq(" 00100");
    CHEQFIX(6u, size());

    CALLFIX(format("% .5lld", (Int64)-100));
    cheq("-00100");
    CHEQFIX(6u, size());

    CALLFIX(format("% 8.5lld", (Int64)100));
    cheq("   00100");
    CHEQFIX(8u, size());

    CALLFIX(format("% 8.5lld", (Int64)-100));
    cheq("  -00100");
    CHEQFIX(8u, size());

    CALLFIX(format("%.0lld", (Int64)0));
    cheq("");
    CHEQFIX(0u, size());

    CALLFIX(format("%+25.22lld", (Int64)-1));
    cheq("  -0000000000000000000001");
    CHEQFIX(25u, size());

    CALLFIX(format("%-+8.5lld", (Int64)100));
    cheq("+00100  ");
    CHEQFIX(8u, size());

    CALLFIX(format("%-+8.5lld", (Int64)100));
    cheq("+00100  ");
    CHEQFIX(8u, size());

    CALLFIX(format("%.80lld", (Int64)1));
    CHEQFIX(80u, size());

    CALLFIX(format("% .80lld", (Int64)1));
    CHEQFIX(81u, size());

    CALLFIX(format("%-1d", 2));
    cheq("2");
    CHEQFIX(1u, size());

    CALLFIX(format("% lli", (Int64)100));
    cheq(" 100");
    CHEQFIX(4u, size());

}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_unsigned)
{
	/*! \ingroup DIFF_WIN_UNIX */
	// it seems that the AIX handles %lu as %llu, since with %lu a "4294967295" should be the result, with %llu the "18446744073709551615" ...
	// VS2015 on x64 handles this correctly!
#ifndef _WIN64
	if (sizeof(size_t) == 8)
    {
        CALLFIX(format("%u", -1));
        cheq("4294967295");
        CHEQFIX(10u, size());

        CALLFIX(format("%lu", (unsigned long)-1));
        cheq("18446744073709551615");
        CHEQFIX(20u, size());

        CALLFIX(format("%25.22lu", (Long64)-1));
        cheq("   0018446744073709551615");
        CHEQFIX(25u, size());

        CALLFIX(format("%30.25lu", (Long64)-1));
        cheq("     0000018446744073709551615");
        CHEQFIX(30u, size());
    }
    else
#endif // !_WIN64
    {
        CALLFIX(format("%u", -1));
        cheq("4294967295");
        CHEQFIX(10u, size());

        CALLFIX(format("%lu", (unsigned long)-1));
        cheq("4294967295");
        CHEQFIX(10u, size());

        CALLFIX(format("%25.22lu", (Long64)-1));
        cheq("   0000000000004294967295");
        CHEQFIX(25u, size());

        CALLFIX(format("%30.25lu", (Long64)-1));
        cheq("     0000000000000004294967295");
        CHEQFIX(30u, size());
    }

    CALLFIX(format("%hu", 74565));
    cheq("9029");
	CHEQFIX(4u, size());

    CALLFIX(format("%hu", 291));
    cheq("291");
	CHEQFIX(3u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_char)
{
    CALLFIX(format("%4c", '1'));
    cheq("   1");
	CHEQFIX(4u, size());

    CALLFIX(format("%-4c", '1'));
    cheq("1   ");
	CHEQFIX(4u, size());

    CALLFIX(format("%c", '\xDC'));
	cheq("\xDC");
	CHEQFIX(1u, size());

	CALLFIX(format("%c%c%c", 'a', 'b', 'c'));
	cheq("abc");
	CHEQFIX(3u, size());

    CALLFIX(format("%3c", 'a'));
    cheq("  a");
	CHEQFIX(3u, size());

    CALLFIX(format("x%cx", 0x100+'X'));
    cheq("xXx");
	CHEQFIX(3u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_hex)
{
    CALLFIX(format("%#012x", 1));
    cheq("0x0000000001");
	CHEQFIX(12u, size());

    CALLFIX(format("%#4.8x", 1));
    cheq("0x00000001");
	CHEQFIX(10u, size());

    CALLFIX(format("%#-8.2x", 1));
    cheq("0x01    ");
	CHEQFIX(8u, size());

	/*! \ingroup DIFF_WIN_UNIX */
	// it seems that the AIX handles %lX as %llX, since with %lX a "0X0000000000000FFFFFF9C" should be the result, with %llX the "0X00000FFFFFFFFFFFFFF9C" ...
	// VS2015 on x64 handles this correctly!
#ifndef _WIN64
    if (sizeof(size_t) == 8)
    {
        CALLFIX(format("%#18.21lX", (Long64)-100));
        cheq("0X00000FFFFFFFFFFFFFF9C");
        CHEQFIX(23u, size());

        CALLFIX(format("%#21.18lx", (Long64)-100));
        cheq(" 0x00ffffffffffffff9c");
        CHEQFIX(21u, size());
    }
    else
#endif // !_WIN64
    {
        CALLFIX(format("%#18.21lX", (Long64)-100));
        cheq("0X0000000000000FFFFFF9C");
        CHEQFIX(23u, size());

        CALLFIX(format("%#21.18lx", (Long64)-100));
        cheq(" 0x0000000000ffffff9c");
        CHEQFIX(21u, size());
	}

	CALLFIX(format("%#18.21llX", (Long64)-100));
	cheq("0X00000FFFFFFFFFFFFFF9C");
	CHEQFIX(23u, size());

	CALLFIX(format("%#21.18llx", (Long64)-100));
	cheq(" 0x00ffffffffffffff9c");
	CHEQFIX(21u, size());
	
	CALLFIX(format("%x", 3065982));
    cheq("2ec87e");
	CHEQFIX(6u, size());

    CALLFIX(format("%#x", 3065982));
    cheq("0x2ec87e");
	CHEQFIX(8u, size());

    CALLFIX(format("%#X", 3065982));
    cheq("0X2EC87E");
	CHEQFIX(8u, size());

    CALLFIX(format("%hx", 0x12345));
    cheq("2345");
	CHEQFIX(4u, size());

    CALLFIX(format("%hx", 0x123));
    cheq("123");
	CHEQFIX(3u, size());

    CALLFIX(format("%hX", 0x1234a));
    cheq("234A");
	CHEQFIX(4u, size());

    CALLFIX(format("%hX", 0xb23));
    cheq("B23");
	CHEQFIX(3u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_oct)
{
    CALLFIX(format("%o", 10));
    cheq("12");
	CHEQFIX(2u, size());

    CALLFIX(format("%#08o", 1));
    cheq("00000001");
	CHEQFIX(8u, size());

    CALLFIX(format("%#-8.5lo", (Long64)100));
    cheq("00144   ");
	CHEQFIX(8u, size());

	/*! \ingroup DIFF_WIN_UNIX */
	// it seems that the AIX handles %lo as %llo, since with %lo a "000000000000037777777634" should be the result, with %llo the "001777777777777777777634" ...
	// VS2015 on x64 handles this correctly!
#ifndef _WIN64
	if (sizeof(size_t) == 8)
    {
        CALLFIX(format("%#20.24lo", (Long64)-100));
        cheq("001777777777777777777634");
        CHEQFIX(24u, size());

        CALLFIX(format("%#24.20lo", (Long64)-100));
        cheq(" 01777777777777777777634");
        CHEQFIX(24u, size());

        CALLFIX(format("%#.25lo", (Long64)-100));
        cheq("0001777777777777777777634");
        CHEQFIX(25u, size());
    }
    else
#endif // !_WIN64
    {
        CALLFIX(format("%#20.24lo", (Long64)-100));
        cheq("000000000000037777777634");
        CHEQFIX(24u, size());

        CALLFIX(format("%#24.20lo", (Long64)-100));
        cheq("    00000000037777777634");
        CHEQFIX(24u, size());

        CALLFIX(format("%#.25lo", (Long64)-100));
        cheq("0000000000000037777777634");
        CHEQFIX(25u, size());
    }

	CALLFIX(format("%#20.24llo", (Long64)-100));
	cheq("001777777777777777777634");
	CHEQFIX(24u, size());

	CALLFIX(format("%#24.20llo", (Long64)-100));
	cheq(" 01777777777777777777634");
	CHEQFIX(24u, size());

	CALLFIX(format("%#.25llo", (Long64)-100));
	cheq("0001777777777777777777634");
	CHEQFIX(25u, size());
	
	CALLFIX(format("%ho", 0221505));
    cheq("21505");
	CHEQFIX(5u, size());

    CALLFIX(format("%ho", 0443));
    cheq("443");
	CHEQFIX(3u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_pointer)
{
	/*! \ingroup DIFF_WIN_UNIX */
	// AIX does not output pointers with padding zeros. (The C Standard does not define an output format, so this one is not a bug)
#ifdef _AIX
	CALLFIX(format("%p", (void *)57));
	cheq("39");
	CHEQFIX(2u, size());

	CALLFIX(format("%20p", (void *)57));
	cheq("                  39");
	CHEQFIX(20u, size());

	CALLFIX(format("%-20p", (void *)57));
	cheq("39                  ");
	CHEQFIX(20u, size());

	char * p = 0;
	CALLFIX(format("%p", p));
	cheq("0");
	CHEQFIX(1u, size());

	CALLFIX(format("%p", 0));
	cheq("0");
	CHEQFIX(1u, size());
#elif defined(_WIN64)
	// Windows does output it with padding zeros
	CALLFIX(format("%p", (void *)57));
	cheq("0000000000000039");
	CHEQFIX(16u, size());

	CALLFIX(format("%20p", (void *)57));
	cheq("    0000000000000039");
	CHEQFIX(20u, size());

	CALLFIX(format("%-20p", (void *)57));
	cheq("0000000000000039    ");
	CHEQFIX(20u, size());

	char * p = 0;
	CALLFIX(format("%p", p));
	cheq("0000000000000000");
	CHEQFIX(16u, size());

	CALLFIX(format("%p", 0));
	cheq("0000000000000000");
	CHEQFIX(16u, size());
#elif defined(_WIN32)
	// Windows does output it with padding zeros
	CALLFIX(format("%p", (void *)57));
	cheq("00000039");
	CHEQFIX(8u, size());

	CALLFIX(format("%12p", (void *)57));
	cheq("    00000039");
	CHEQFIX(12u, size());

	CALLFIX(format("%-12p", (void *)57));
	cheq("00000039    ");
	CHEQFIX(12u, size());

	char * p = 0;
	CALLFIX(format("%p", p));
	cheq("00000000");
	CHEQFIX(8u, size());

	CALLFIX(format("%p", 0));
	cheq("00000000");
	CHEQFIX(8u, size());
#endif // _WIN32
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_string_length)
{
	CALLFIX(format("%s", "123"));
	cheq("123");
	CHEQFIX(3u, size());

	CALLFIX(format("%5s", "123"));
	cheq("  123");
	CHEQFIX(5u, size());

	CALLFIX(format("%*s", 5, "123"));
	cheq("  123");
	CHEQFIX(5u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_string)
{
    CALLFIX(format("%s", "%%%%"));
    cheq("%%%%");
	CHEQFIX(4u, size());

    CALLFIX(format("Hallo %s!", "Welt"));
    cheq("Hallo Welt!");
	CHEQFIX(11u, size());

    CALLFIX(format("%1.3s", "Test"));
    cheq("Tes");
	CHEQFIX(3u, size());

    CALLFIX(format("%1.10s", "Test"));
    cheq("Test");
	CHEQFIX(4u, size());

    CALLFIX(format("%12.3s", "Test"));
    cheq("         Tes");
	CHEQFIX(12u, size());

    CALLFIX(format("%-12.3s", "Test"));
    cheq("Tes         ");
	CHEQFIX(12u, size());

    CALLFIX(format("%12s", "Test"));
    cheq("        Test");
	CHEQFIX(12u, size());

    CALLFIX(format("%-12s", "Test"));
    cheq("Test        ");
	CHEQFIX(12u, size());

    CALLFIX(format("%4s", "foo"));
    cheq(" foo");
	CHEQFIX(4u, size());

    CALLFIX(format("%.1s", "foo"));
    cheq("f");
	CHEQFIX(1u, size());

    CALLFIX(format("%.*s", 1, "foo"));
    cheq("f");
	CHEQFIX(1u, size());

    CALLFIX(format("%*s", 5, "foo"));
    cheq("  foo");
	CHEQFIX(5u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_nothing)
{
    {
        int b = 0;
        int i = 0;

        bStr.format("asdf%n", &b);
        iStr.format("asdf%n", &i);

        CHECK_EQUAL(4, b);
        CHECK_EQUAL(4, i);
        cheq("asdf");
	    CHEQFIX(4u, size());
    }

    {
        int b = 0;
        int i = 0;

        bStr.format("asdf%nXYZ", &b);
        iStr.format("asdf%nXYZ", &i);

        CHECK_EQUAL(4, b);
        CHECK_EQUAL(4, i);
        cheq("asdfXYZ");
	    CHEQFIX(7u, size());
    }

    {
        int b = 0;
        int i = 0;

        bStr.format("a%%sdf%nXYZ", &b);
        iStr.format("a%%sdf%nXYZ", &i);

        CHECK_EQUAL(5, b);
        CHECK_EQUAL(5, i);
        cheq("a%sdfXYZ");
	    CHEQFIX(8u, size());
    }

    {
        int b = 0;
        int i = 0;

        bStr.format("a%%s%%df%nXYZ", &b);
        iStr.format("a%%s%%df%nXYZ", &i);

        CHECK_EQUAL(6, b);
        CHECK_EQUAL(6, i);
        cheq("a%s%dfXYZ");
	    CHEQFIX(9u, size());
    }

    {
        int b = 0;
        int i = 0;

        bStr.format("a%%s%%df%nX%%YZ", &b);
        iStr.format("a%%s%%df%nX%%YZ", &i);

        CHECK_EQUAL(6, b);
        CHECK_EQUAL(6, i);
        cheq("a%s%dfX%YZ");
	    CHEQFIX(10u, size());
    }

    {
        int b1 = 0,
            b2 = 0;
        int i1 = 0,
            i2 = 0;

        bStr.format("asdf%nXYZ%n", &b1, &b2);
        iStr.format("asdf%nXYZ%n", &i1, &i2);

        CHECK_EQUAL(4, b1);
        CHECK_EQUAL(7, b2);
        CHECK_EQUAL(4, i1);
        CHECK_EQUAL(7, i2);
        cheq("asdfXYZ");
	    CHEQFIX(7u, size());
    }

    {
        short bs1 = 0,
              bs2 = 0;
        short is1 = 0,
              is2 = 0;

        bStr.format("asdf%hnXYZ%hn", &bs1, &bs2);
        iStr.format("asdf%hnXYZ%hn", &is1, &is2);

        CHECK_EQUAL(4, bs1);
        CHECK_EQUAL(7, bs2);
        CHECK_EQUAL(4, is1);
        CHECK_EQUAL(7, is2);
        cheq("asdfXYZ");
	    CHEQFIX(7u, size());
    }

    {
        long bl1 = 0,
             bl2 = 0;
        long il1 = 0,
             il2 = 0;

        bStr.format("asdf%lnXYZ%ln", &bl1, &bl2);
        iStr.format("asdf%lnXYZ%ln", &il1, &il2);

        CHECK_EQUAL(4, bl1);
        CHECK_EQUAL(7, bl2);
        CHECK_EQUAL(4, il1);
        CHECK_EQUAL(7, il2);
        cheq("asdfXYZ");
	    CHEQFIX(7u, size());
    }
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, format_misc)
{
    CALLFIX(format("%c %hd %c%s%c", 'a', 3, 'b', "GGGG", 'c'));
	cheq("a 3 bGGGGc");
	CHEQFIX(10u, size());

	CALLFIX(format("%c %hd %c%-40s%c", 'a', 3, 'b', "GGGG", 'c'));
	cheq("a 3 bGGGG                                    c");
	CHEQFIX(46u, size());

	CALLFIX(format("%1.1s%8s", "test", "12345678"));
	cheq("t12345678");
	CHEQFIX(9u, size());
    
    CALLFIX(format("%g %s", 50000.1234, "Hallo"));
    CHECK_EQUAL("50000.1 Hallo", bStr.c_str());
    CHECK_EQUAL("50000,1 Hallo", iStr.c_str());

    CALLFIX(format("Die %s auf alles: %d!", "Antwort", 42));
    cheq("Die Antwort auf alles: 42!");
	CHEQFIX(26u, size());

    CALLFIX(format("%04d%12s", 42, "Test"));
    cheq("0042        Test");
	CHEQFIX(16u, size());

    CALLFIX(format("%d %x %o %#x %#o", 100, 100, 100, 100, 100));
    cheq("100 64 144 0x64 0144");
	CHEQFIX(20u, size());

    CALLFIX(format("%d %x %o %#x %#o", -100, -100, -100, -100, -100));
    cheq("-100 ffffff9c 37777777634 0xffffff9c 037777777634");
	CHEQFIX(49u, size());

	CALLFIX(format("%c\xDC %.2s", '\xDC', "ss"));
	cheq("\xDC\xDC ss");
	CHEQFIX(5u, size());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, stdStreamOpInterOp)
{
    {
        std::stringstream out;

        BString str1 = "00000000";
        BString str2 = "00test";
        BString str3 = "a";

        out << str1;
        out << std::setfill('0') 
            << std::setw(8) 
            << str2;
        CHECK_EQUAL(16, (int)out.str().length());
        CHECK_EQUAL("000000000000test", out.str());
        
        out << std::setfill(' ') 
            << std::setw(20) 
            << str3;
        CHECK_EQUAL(36, (int)out.str().length());
        CHECK_EQUAL("000000000000test                   a", out.str());
    }

    {
        std::stringstream out;

        I18nString str1 = "00000000";
        I18nString str2 = "00test";
        I18nString str3 = "a";

        out << str1;
        out << std::setfill('0') 
            << std::setw(8) 
            << str2;
        CHECK_EQUAL(16, (int)out.str().length());
        CHECK_EQUAL("000000000000test", out.str());
        
        out << std::setfill(' ') 
            << std::setw(20) 
            << str3;
        CHECK_EQUAL(36, (int)out.str().length());
        CHECK_EQUAL("000000000000test                   a", out.str());
    }
}

//----------------------------------------------------------------------------
TEST(substrToDelimiter)
{
    BString baseString = "aaa=bbb;ccc= ddd ; ggg = hhh ;;; iii ";
    BString str;

    int startposA = 0;
    int startposC = startposA +  8;
    int startposG = startposC + 10;
    int startposI = startposG + 14;
    int startposN = startposA + startposI-2;

    // with aceptNoDelimiter
    str = baseString.substrToDelimiter(             ";");
    CHECK_EQUAL("aaa=bbb", str.c_str());

    str = baseString.substrToDelimiter(startposC-1, ";");
    CHECK_EQUAL("", str.c_str());

    str = baseString.substrToDelimiter(startposC  , ";");
    CHECK_EQUAL("ccc= ddd ", str.c_str());
    
    str = baseString.substrToDelimiter(startposG  , ";");
    CHECK_EQUAL(" ggg = hhh ", str.c_str()); 
    
    str = baseString.substrToDelimiter(startposI  , ";");
    CHECK_EQUAL(" iii ", str.c_str()); 
    
    str = baseString.substrToDelimiter(startposN  , ";");
    CHECK_EQUAL("", str.c_str()); 

    // without aceptNoDelimiter
    str = baseString.substrToDelimiter(             ";", false);
    CHECK_EQUAL("aaa=bbb", str.c_str());
    
    str = baseString.substrToDelimiter(startposC-1, ";", false);
    CHECK_EQUAL("", str.c_str());
    
    str = baseString.substrToDelimiter(startposC  , ";", false);
    CHECK_EQUAL("ccc= ddd ", str.c_str());
    
    str = baseString.substrToDelimiter(startposG  , ";", false);
    CHECK_EQUAL(" ggg = hhh ", str.c_str()); 
    
    str = baseString.substrToDelimiter(startposI  , ";", false);
    CHECK_EQUAL("", str.c_str()); 
    
    str = baseString.substrToDelimiter(startposN  , ";", false);
    CHECK_EQUAL("", str.c_str()); 
}

//----------------------------------------------------------------------------
TEST(substrToDelimiterAndTrim)
{
    BString baseString = "aaa=bbb;ccc= ddd ; ggg = hhh ; iii ";
    BString str;

    int startposA = 0;
    int startposC = startposA +  8;
    int startposG = startposC + 10;
    int startposI = startposG + 12;

    // --- without aceptNoDelimiter
    str = baseString.substrToDelimiterAndTrim  (";", BString::FIRST_END);
    CHECK_EQUAL("aaa=bbb", str.c_str());
    
    str =  baseString.substrToDelimiterAndTrim (";", BString::END);
    CHECK_EQUAL("aaa=bbb", str.c_str());
    
    str =  baseString.substrToDelimiterAndTrim (";", BString::FIRST);
    CHECK_EQUAL("aaa=bbb", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(";", BString::ALL);
    CHECK_EQUAL("aaa=bbb", str.c_str());

    str =  baseString.substrToDelimiterAndTrim (startposG,";", BString::FIRST_END);
    CHECK_EQUAL("ggg = hhh", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::END);
    CHECK_EQUAL(" ggg = hhh", str.c_str());
    
    str =  baseString.substrToDelimiterAndTrim (startposG,";", BString::FIRST);
    CHECK_EQUAL("ggg = hhh ", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::ALL);
    CHECK_EQUAL("ggg=hhh", str.c_str());

    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::FIRST_END);
    CHECK_EQUAL("iii", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::END);
    CHECK_EQUAL(" iii", str.c_str());
    
    str =  baseString.substrToDelimiterAndTrim (startposI,";", BString::FIRST);
    CHECK_EQUAL("iii ", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::ALL);
    CHECK_EQUAL("iii", str.c_str());

    // --- without aceptNoDelimiter
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::FIRST_END, false);
    CHECK_EQUAL("ggg = hhh", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::END      , false);
    CHECK_EQUAL(" ggg = hhh", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::FIRST    , false);
    CHECK_EQUAL("ggg = hhh ", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposG,";", BString::ALL      , false);
    CHECK_EQUAL("ggg=hhh", str.c_str());

    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::FIRST_END, false);
    CHECK_EQUAL("", str.c_str());
    
    str =  baseString.substrToDelimiterAndTrim (startposI,";", BString::END      , false);
    CHECK_EQUAL("", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::FIRST    , false);
    CHECK_EQUAL("", str.c_str());
    
    str =   baseString.substrToDelimiterAndTrim(startposI,";", BString::ALL      , false);
    CHECK_EQUAL("", str.c_str());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, tokenize)
{
    BString baseString       = "aaa=bbb;ccc= ddd ; ggg = hhh ;; iii ;jjj";
    int     numberSemicol    = 5;
    int     numberSemicolAll = numberSemicol + 1;

    {
        CollBString vec1 = baseString.tokenize(";");
        CollBString vec2 = baseString.tokenize(";", false);

        CHECK_EQUAL( numberSemicolAll, static_cast<int>(vec1.size()));
        CHECK_EQUAL( numberSemicol   , static_cast<int>(vec2.size()));

        CHECK_EQUAL("aaa=bbb"    , vec1[0].c_str());
        CHECK_EQUAL("aaa=bbb"    , vec2[0].c_str());
        CHECK_EQUAL("ccc= ddd "  , vec1[1].c_str());
        CHECK_EQUAL("ccc= ddd "  , vec2[1].c_str());
        CHECK_EQUAL(" ggg = hhh ", vec1[2].c_str());
        CHECK_EQUAL(" ggg = hhh ", vec2[2].c_str());
        CHECK_EQUAL(""           , vec1[3].c_str());
        CHECK_EQUAL(""           , vec2[3].c_str());
        CHECK_EQUAL(" iii "      , vec1[4].c_str());
        CHECK_EQUAL(" iii "      , vec2[4].c_str());
        CHECK_EQUAL("jjj"        , vec1[5].c_str());
        baseString.tokenize(vec1, ";");

        int number = numberSemicolAll*2;
        CHECK_EQUAL( number,  static_cast<int>(vec1.size()));

        baseString.tokenize(vec1, ";", false);
        number+= numberSemicol;
        CHECK_EQUAL( number,  static_cast<int>(vec1.size()));
    } 

    {
        CollBString vecTT = baseString.tokenize(";", true , true);
        CollBString vecFT = baseString.tokenize(";", false, true); 
        CollBString vecTF = baseString.tokenize(";", true , false);
        CollBString vecFF = baseString.tokenize(";", false, false); 

        CHECK_EQUAL( numberSemicol  , static_cast<int>(vecTT.size()));
        CHECK_EQUAL( numberSemicol+1, static_cast<int>(vecTF.size()));
        CHECK_EQUAL( numberSemicol-1, static_cast<int>(vecFT.size()));
        CHECK_EQUAL( numberSemicol  , static_cast<int>(vecFF.size()));

        CHECK_EQUAL("aaa=bbb",vecTT[0].c_str());
        CHECK_EQUAL("aaa=bbb",vecTF[0].c_str());
        CHECK_EQUAL("aaa=bbb",vecFT[0].c_str());
        CHECK_EQUAL("aaa=bbb",vecFF[0].c_str());

        CHECK_EQUAL(" iii ", vecTT[3].c_str());
        CHECK_EQUAL(""     , vecTF[3].c_str());
        CHECK_EQUAL(" iii ", vecFT[3].c_str());
        CHECK_EQUAL(""     , vecFF[3].c_str());
    }

    {
        BString baseString2 = "jjj=kkk;";
        CollBString vec21   = baseString2.tokenize(";");
        CollBString vec22   = baseString2.tokenize(";", false);

        CHECK_EQUAL( 1, static_cast<int>(vec21.size()));
        CHECK_EQUAL( 1, static_cast<int>(vec22.size()));
    }
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, tokenizeAndTrim)
{
    BString baseString       = "aaa=bbb;ccc= ddd ; ggg = hhh ; iii ";
    int     numberSemicol    = 3;
    int     numberSemicolAll = numberSemicol+1;

    CollBString vec1FE = baseString.tokenizeAndTrim(";", BString::FIRST_END );
    CollBString vec1EN = baseString.tokenizeAndTrim(";", BString::END);
    CollBString vec1FI = baseString.tokenizeAndTrim(";", BString::FIRST);
    CollBString vec1AL = baseString.tokenizeAndTrim(";", BString::ALL);
    CollBString vec2FE = baseString.tokenizeAndTrim(";", BString::FIRST_END, false);
    CollBString vec2EN = baseString.tokenizeAndTrim(";", BString::END      , false);
    CollBString vec2FI = baseString.tokenizeAndTrim(";", BString::FIRST    , false);
    CollBString vec2AL = baseString.tokenizeAndTrim(";", BString::ALL      , false);

    // first call:
    {
        CHECK_EQUAL( numberSemicolAll, static_cast<int>(vec1FE.size()));
        CHECK_EQUAL( numberSemicolAll, static_cast<int>(vec1EN.size()));
        CHECK_EQUAL( numberSemicolAll, static_cast<int>(vec1FI.size()));
        CHECK_EQUAL( numberSemicolAll, static_cast<int>(vec1AL.size()));

        CHECK_EQUAL( numberSemicol, static_cast<int>(vec2FE.size()));
        CHECK_EQUAL( numberSemicol, static_cast<int>(vec2EN.size()));
        CHECK_EQUAL( numberSemicol, static_cast<int>(vec2FI.size()));
        CHECK_EQUAL( numberSemicol, static_cast<int>(vec2AL.size()));

        CHECK_EQUAL("aaa=bbb", vec1FE[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec1EN[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec1FI[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec1AL[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec2FE[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec2EN[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec2FI[0].c_str());
        CHECK_EQUAL("aaa=bbb", vec2AL[0].c_str());

        CHECK_EQUAL("ccc= ddd",  vec1FE[1].c_str());
        CHECK_EQUAL("ccc= ddd",  vec1EN[1].c_str());
        CHECK_EQUAL("ccc= ddd ", vec1FI[1].c_str());
        CHECK_EQUAL("ccc=ddd",   vec1AL[1].c_str());
        CHECK_EQUAL("ccc= ddd",  vec2FE[1].c_str());
        CHECK_EQUAL("ccc= ddd",  vec2EN[1].c_str());
        CHECK_EQUAL("ccc= ddd ", vec2FI[1].c_str());
        CHECK_EQUAL("ccc=ddd",   vec2AL[1].c_str());

        CHECK_EQUAL("ggg = hhh",  vec1FE[2].c_str());
        CHECK_EQUAL(" ggg = hhh", vec1EN[2].c_str());
        CHECK_EQUAL("ggg = hhh ", vec1FI[2].c_str());
        CHECK_EQUAL("ggg=hhh",    vec1AL[2].c_str());
        CHECK_EQUAL("ggg = hhh",  vec2FE[2].c_str());
        CHECK_EQUAL(" ggg = hhh", vec2EN[2].c_str());
        CHECK_EQUAL("ggg = hhh ", vec2FI[2].c_str());
        CHECK_EQUAL("ggg=hhh",    vec2AL[2].c_str());

        CHECK_EQUAL("iii",  vec1FE[3].c_str());
        CHECK_EQUAL(" iii", vec1EN[3].c_str());
        CHECK_EQUAL("iii ", vec1FI[3].c_str());
        CHECK_EQUAL("iii",  vec1AL[3].c_str());
    }

    // second call:
    CollBString vecFI_EN_ALL = vec1FI;
    {
        int number1 = numberSemicolAll;
        CHECK_EQUAL(number1,  static_cast<int>(vecFI_EN_ALL.size()));

        int number2 = number1 + numberSemicolAll;
        baseString.tokenizeAndTrim(vecFI_EN_ALL, ";", BString::END );
        CHECK_EQUAL(number2,  static_cast<int>(vecFI_EN_ALL.size()));

        int number3 = number2 + numberSemicol;
        baseString.tokenizeAndTrim(vecFI_EN_ALL, ";", BString::ALL , false);
        CHECK_EQUAL(number3,  static_cast<int>(vecFI_EN_ALL.size()));

        // check all values:
        {
            int i = 0;
            int k = 0;

            for (i = 0; i < number1; ++i)
            {
                CHECK_EQUAL(vec1FI[i],vecFI_EN_ALL[i]);
            }

            for ( i = number1, k = 0; i < number2; ++i, ++k)
            {
                CHECK_EQUAL(vec1EN[k],vecFI_EN_ALL[i]);
            }

            for ( i = number2, k = 0; i < number3; ++i, ++k)
            {
                CHECK_EQUAL(vec1AL[k],vecFI_EN_ALL[i]);
            }
        }
    }
}

//----------------------------------------------------------------------------
TEST_FIXTURE(StringFix, cast_ConstString)
{
	static ConstString CMP[] = 
	{
        // short: static  internal string buffer
		"ABC",
        
        // long:  dynamic internal string buffer
		"A123456789" "B123456789" 
        "C123456789" "D123456789" 
        "E123456789" "F123456789"
	};

	const BString * pColl[] = 
	{
		new BString(CMP[0]),
		new BString(CMP[1])
	};

	// printf
	{
		BString::value_type buf[2048];

		for (size_t i = 0; i < sizeof(pColl)/sizeof(pColl[0]); ++i)
		{
			::sprintf(buf, "%s", pColl[i]->c_str());
			CHECK_EQUAL(CMP[i % 2], buf);

// Solaris raises compiler warnings
/*#if defined _WIN32 && !defined NDEBUG                        C4477 : 'sprintf' : format string '%s' requires an argument of type 'char *', but variadic argument 1 has type 'const basar::cmnutil::BString'
			bool ExceptionAccessViolation = false;	

			try
			{
				::sprintf(buf, "%s", *pColl[i]);	// w/o c_str()
				CHECK(0 != strcmp(CMP[i % 2], buf));
			}
			catch (...)
			{
				ExceptionAccessViolation = true;		// probably access violation
			}
			CHECK( ! ExceptionAccessViolation );
#endif*/ 
		}
	}

	// BString format
	{
		BString buf;

		for (size_t i = 0; i < sizeof(pColl)/sizeof(pColl[0]); ++i)
		{
			buf.format("%s", pColl[i]->c_str());
			CHECK_EQUAL(CMP[i % 2], buf.c_str());

// Solaris raises compiler warnings
/*#if defined _WIN32 && !defined NDEBUG

			bool ExceptionAccessViolation = false;	

			try
			{
				buf.format("%s", *pColl[i]);	// w/o c_str()
				CHECK(0 != strcmp(CMP[i % 2], buf.c_str()));
			}
			catch (...)
			{
				ExceptionAccessViolation = true;		// probably access violation
			}

			CHECK( ! ExceptionAccessViolation );
#endif*/
		}
	}

	// I18nString format
	{
		I18nString buf;

		for (size_t i = 0; i < sizeof(pColl)/sizeof(pColl[0]); ++i)
		{
			buf.format("%s", pColl[i]->c_str());
			CHECK_EQUAL(CMP[i % 2], buf.c_str());

// Solaris raises compiler warnings
/* #if defined _WIN32 && !defined NDEBUG

			bool ExceptionAccessViolation = false;	

			try
			{
				buf.format("%s", *pColl[i]);	// w/o c_str()
				CHECK(0 != strcmp(CMP[i % 2], buf.c_str()));
			}
			catch (...)
			{
				ExceptionAccessViolation = true;		// probably access violation
			}

			CHECK( ! ExceptionAccessViolation );
#endif*/
		}
	}

    // stream
	{
		std::stringstream ss;

		for (size_t i = 0; i < sizeof(pColl)/sizeof(pColl[0]); ++i)
		{
			ss.str("");
			ss << pColl[i]->c_str();
			CHECK_EQUAL(ss.str().c_str(), CMP[i % 2]);

			ss.str("");
			ss << *pColl[i];
			CHECK_EQUAL(ss.str().c_str(), CMP[i % 2]);
		}
	}

	for (size_t i = 0; i < sizeof(pColl)/sizeof(pColl[0]); ++i)
	{
		delete pColl[i];
	}
}

//------------------------------------------------------------------------------
}	// SUITE

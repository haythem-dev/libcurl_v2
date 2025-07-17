#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

using basar::I18nString;
using basar::cmnutil::Locale;
using basar::cmnutil::Decimal;
using basar::cmnutil::DivisionByZeroException;
using basar::cmnutil::OverflowException;
using basar::Int16;
using basar::Int32;
using basar::UInt16;
using basar::UInt32;
using basar::Long32;
using basar::ULong32;
using basar::Float32;
using basar::Float64;

static Decimal & getDecConst()
{
	static Decimal xxx(12345.1234f);
	return xxx;
}

static Decimal yyy("3.4");

//------------------------------------------------------------------------------
SUITE(TestDecimal)
{

//----------------------------------------------------------------------------
struct Fixture
{
	Fixture() : d0(12347.01),
		        d1(12345.99),
				d2(98765.00 ),
                d3(0.5) {};

	Decimal d0, d1, d2, d3;
};

//----------------------------------------------------------------------------
TEST(check_static_init)
{
	Decimal d = getDecConst();
}

//----------------------------------------------------------------------------
TEST(init_from_buitin_type)
{
    const Float64 TOL = 0.00001;

    Decimal d1 = static_cast<Decimal>(0);
    Decimal d2 = static_cast<Decimal>(0.0);

    CHECK_EQUAL(0, d1.toInt32());
    CHECK_CLOSE(0.0, d2.toFloat64(), TOL );
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, Ctor)
{
	const Float64 TOL = 0.00001;

	CHECK_CLOSE(12347.01 , d0.toFloat64(), TOL);
	CHECK_CLOSE(12345.99,  d1.toFloat64(), TOL);
	CHECK_CLOSE(98765.0  , d2.toFloat64(), TOL);

	Int16   s = 12;
	Int32   t = 1234567;
	Long32  l = 98765432;
	Float32 f = static_cast<Float32>(3.4);
	Float64 d = 1237889.4561;

	CHECK_CLOSE(0.0         , Decimal(  ).toFloat64(), TOL);
	CHECK_CLOSE(12345.99    , Decimal(d1).toFloat64(), TOL);
	CHECK_CLOSE(12.0        , Decimal(s) .toFloat64(), TOL);
	CHECK_CLOSE(1234567.0   , Decimal(t) .toFloat64(), TOL);
	CHECK_CLOSE(98765432.0  , Decimal(l) .toFloat64(), TOL);
	CHECK_CLOSE(3.4         , Decimal(f) .toFloat64(), TOL);
	CHECK_CLOSE(1237889.4561, Decimal(d) .toFloat64(), TOL);

	Locale::setLocale(Locale::C);

	CHECK_CLOSE(3.0         , Decimal("3.0")         .toFloat64(), TOL);
	CHECK_CLOSE(15.01       , Decimal("15.01")       .toFloat64(), TOL);
	CHECK_CLOSE(16.01       , Decimal("16.01")       .toFloat64(), TOL);
	CHECK_CLOSE(2616.0111   , Decimal("2616.0111")   .toFloat64(), TOL);
	CHECK_CLOSE(12347.01    , Decimal("12347.01")    .toFloat64(), TOL);
	CHECK_CLOSE(12345.99    , Decimal("12345.99")    .toFloat64(), TOL);
	CHECK_CLOSE(98765.0     , Decimal("98765.0")     .toFloat64(), TOL);
	CHECK_CLOSE(1237889.4561, Decimal("1237889.4561").toFloat64(), TOL);
	CHECK_CLOSE(56.011      , Decimal("56.011")      .toFloat64(), TOL);

	CHECK_CLOSE(-3.0         , Decimal("-3.0")         .toFloat64(), TOL);
	CHECK_CLOSE(-15.01       , Decimal("-15.01")       .toFloat64(), TOL);
	CHECK_CLOSE(-16.01       , Decimal("-16.01")       .toFloat64(), TOL);
	CHECK_CLOSE(-2616.0111   , Decimal("-2616.0111")   .toFloat64(), TOL);
	CHECK_CLOSE(-12347.01    , Decimal("-12347.01")    .toFloat64(), TOL);
	CHECK_CLOSE(-12345.99    , Decimal("-12345.99")    .toFloat64(), TOL);
	CHECK_CLOSE(-98765.0     , Decimal("-98765.0")     .toFloat64(), TOL);
	CHECK_CLOSE(-1237889.4561, Decimal("-1237889.4561").toFloat64(), TOL);
	CHECK_CLOSE(-56.011      , Decimal("-56.011")      .toFloat64(), TOL);
}

//----------------------------------------------------------------------------
TEST(Digits)
{
	Locale::setLocale(Locale::C);

	CHECK_EQUAL(2, Decimal("12345.99")    .getNumberOfDecimalPlaces());
	CHECK_EQUAL(1, Decimal("13.9")        .getNumberOfDecimalPlaces());
	CHECK_EQUAL(0, Decimal("125.0")       .getNumberOfDecimalPlaces());
	CHECK_EQUAL(7, Decimal("125.0123456") .getNumberOfDecimalPlaces());
	CHECK_EQUAL(8, Decimal("125.01234569").getNumberOfDecimalPlaces());

    CHECK_EQUAL( 7, Decimal("12345.99")    .getNumberOfDigits());
	CHECK_EQUAL( 3, Decimal("13.9")        .getNumberOfDigits());
	CHECK_EQUAL( 3, Decimal("125.0")       .getNumberOfDigits());
	CHECK_EQUAL(10, Decimal("125.0123456") .getNumberOfDigits());

	// an equal number in front of the separator, seems to
	// make the programm assume, the whole number of values are
	// 17 (infx ??), but not when the value is > 16
	// or sume such, this strange behaviour should
	// be unter test.
	CHECK_EQUAL(4, Decimal("15.01    ").getNumberOfDigits());
	CHECK_EQUAL(4, Decimal("16.01    ").getNumberOfDigits());
	CHECK_EQUAL(7, Decimal("1515.011 ").getNumberOfDigits());
	CHECK_EQUAL(7, Decimal("1616.011 ").getNumberOfDigits());
	CHECK_EQUAL(8, Decimal("2616.0111").getNumberOfDigits());
	CHECK_EQUAL(5, Decimal("56.011   ").getNumberOfDigits());

	CHECK_EQUAL(2, Decimal("15.01    ").getNumberOfDecimalPlaces());
	CHECK_EQUAL(2, Decimal("16.01    ").getNumberOfDecimalPlaces());
	CHECK_EQUAL(3, Decimal("1515.011 ").getNumberOfDecimalPlaces());
	CHECK_EQUAL(3, Decimal("1616.011 ").getNumberOfDecimalPlaces());
	CHECK_EQUAL(4, Decimal("2616.0111").getNumberOfDecimalPlaces());
	CHECK_EQUAL(3, Decimal("56.011   ").getNumberOfDecimalPlaces());

	CHECK_EQUAL(2, Decimal("20").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("20").getNumberOfDecimalPlaces()); 

	CHECK_EQUAL(0, Decimal("0").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("0").getNumberOfDecimalPlaces());

	CHECK_EQUAL(1, Decimal("2").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("2").getNumberOfDecimalPlaces());

	CHECK_EQUAL(3, Decimal("200").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("200").getNumberOfDecimalPlaces());

	CHECK_EQUAL(2, Decimal("0.1").getNumberOfDigits());
	CHECK_EQUAL(1, Decimal("0.1").getNumberOfDecimalPlaces());

	CHECK_EQUAL(4, Decimal("0.001").getNumberOfDigits());
	CHECK_EQUAL(3, Decimal("0.001").getNumberOfDecimalPlaces());

	CHECK_EQUAL(5, Decimal("0.0001").getNumberOfDigits());
	CHECK_EQUAL(4, Decimal("0.0001").getNumberOfDecimalPlaces());

	CHECK_EQUAL(5, Decimal("0.1001").getNumberOfDigits());
	CHECK_EQUAL(4, Decimal("0.1001").getNumberOfDecimalPlaces());

	CHECK_EQUAL(1, Decimal("-2").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("-2").getNumberOfDecimalPlaces());

	CHECK_EQUAL(3, Decimal("-200").getNumberOfDigits());
	CHECK_EQUAL(0, Decimal("-200").getNumberOfDecimalPlaces());

	CHECK_EQUAL(2, Decimal("-0.1").getNumberOfDigits());
	CHECK_EQUAL(1, Decimal("-0.1").getNumberOfDecimalPlaces());

	CHECK_EQUAL(4, Decimal("-0.001").getNumberOfDigits());
	CHECK_EQUAL(3, Decimal("-0.001").getNumberOfDecimalPlaces());

	CHECK_EQUAL(5, Decimal("-0.0001").getNumberOfDigits());
	CHECK_EQUAL(4, Decimal("-0.0001").getNumberOfDecimalPlaces());

	CHECK_EQUAL(5, Decimal("-0.1001").getNumberOfDigits());
	CHECK_EQUAL(4, Decimal("-0.1001").getNumberOfDecimalPlaces());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, toString)
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
		case Locale::C:
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
			CHECK_EQUAL("12347.01" , d0.toString().c_str());
			CHECK_EQUAL("12345.99" , d1.toString().c_str());
			CHECK_EQUAL("12345.9"  , Decimal("12345.93").toString(1).c_str());
		    CHECK_EQUAL("12345.987", Decimal("12345.987").toString( ).c_str());
			CHECK_EQUAL("98765.0"  , d2.toString().c_str());
			break;

		default:
			CHECK_EQUAL("12347,01" , d0.toString().c_str());
			CHECK_EQUAL("12345,99" , d1.toString().c_str());
			CHECK_EQUAL("12345,9"  , Decimal("12345,93") .toString(1).c_str());
		    CHECK_EQUAL("12345,987", Decimal("12345,987").toString( ).c_str());
			CHECK_EQUAL("98765,0"  , d2.toString().c_str());
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, StreamOutput)
{
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
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
			{
				os << d0;
				CHECK_EQUAL("12347.01" , os.str());
				os.str("");
				os << d1;
				CHECK_EQUAL("12345.99" , os.str());
				os.str("");
				os << Decimal("12345.987");
				CHECK_EQUAL("12345.987", os.str());
				os.str("");
				os << d2;
				CHECK_EQUAL("98765.0"  , os.str());
				os.str("");
				break;
			}

		default:
			{
				os << d0;
				CHECK_EQUAL("12347,01" , os.str());
				os.str("");
				os << d1;
				CHECK_EQUAL("12345,99" , os.str());
				os.str("");
				os << Decimal("12345,987");
				CHECK_EQUAL("12345,987", os.str());
				os.str("");
				os << d2;
				CHECK_EQUAL("98765,0"  , os.str());
				os.str("");
			}
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, toNonLocString)
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

		CHECK_EQUAL("12347.01" , d0.toNonLocString() .c_str());
		CHECK_EQUAL("12345.99" , d1.toNonLocString() .c_str());
        CHECK_EQUAL("0.5"      , d3.toNonLocString() .c_str());

		switch (j)
		{
		case Locale::C:
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
	    case Locale::GERMAN_CH:
	    case Locale::FRENCH_CH:
			CHECK_EQUAL("12345.9"  , Decimal("12345.93" ).toNonLocString(1).c_str());
			CHECK_EQUAL("12345.987", Decimal("12345.987").toNonLocString( ).c_str());
			break;

		default:
			CHECK_EQUAL("12345.9"  , Decimal("12345,93" ).toNonLocString(1).c_str());
			CHECK_EQUAL("12345.987", Decimal("12345,987").toNonLocString( ).c_str());
			break;	
		}

		CHECK_EQUAL("98765.0"  , d2.toNonLocString() .c_str());
	}
}


//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, toSepString)
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
		case Locale::C:
		case Locale::ENGLISH_GB:
		case Locale::ENGLISH_US:
			CHECK_EQUAL("12,347.01", d0.toSepString() .c_str());
			CHECK_EQUAL("12,345.99", d1.toSepString() .c_str());
			CHECK_EQUAL("12,345.9" , Decimal("12345.93").toSepString(1).c_str());
			CHECK_EQUAL("98,765.0" , d2.toSepString() .c_str());
            CHECK_EQUAL("12,347",    d0.toSepString(0) .c_str());
			break;

		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
			CHECK_EQUAL("12'347.01", d0.toSepString().c_str());
			CHECK_EQUAL("12'345.99", d1.toSepString().c_str());
			CHECK_EQUAL("12'345.9" , Decimal("12345.93").toSepString(1).c_str());
			CHECK_EQUAL("98'765.0" , d2.toSepString().c_str());
            CHECK_EQUAL("12'347",    d0.toSepString(0).c_str());
			break;

		case Locale::FRENCH_FR:
		case Locale::BULGARIAN_BG:
		case Locale::CZECH_CZ:
            // not space, it's 160 - NBSP
			CHECK_EQUAL("12 347,01", d0.toSepString().c_str());
			CHECK_EQUAL("12 345,99", d1.toSepString().c_str());
			CHECK_EQUAL("12 345,9" , Decimal("12345,93").toSepString(1).c_str());
			CHECK_EQUAL("98 765,0" , d2.toSepString().c_str());
            CHECK_EQUAL("12 347",    d0.toSepString(0).c_str());
			break;

		default:
			CHECK_EQUAL("12.347,01", d0.toSepString().c_str());
			CHECK_EQUAL("12.345,99", d1.toSepString().c_str());
			CHECK_EQUAL("12.345,9" , Decimal("12345,93").toSepString(1).c_str());

			CHECK_EQUAL("-1.345.678,95", Decimal("-1345678,95").toSepString(2).c_str());
			CHECK_EQUAL("-12.345,9", Decimal("-12345,93").toSepString(1).c_str());
			CHECK_EQUAL("-123,9", Decimal("-123,93").toSepString(1).c_str());
			CHECK_EQUAL("-12,8", Decimal("-12,8").toSepString(1).c_str());


			CHECK_EQUAL("98.765,0" , d2.toSepString().c_str());
            CHECK_EQUAL("12.347",    d0.toSepString(0).c_str());
			break;
		}
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, fromString)
{
	Decimal e0,
		    e1, 
			e2;

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
			e0.fromString("12,347.01");
			e1.fromString("12,345.99");
			e2.fromString("98,765.0");
			break;

		case Locale::GERMAN_CH:
		case Locale::FRENCH_CH:
			e0.fromString("12'347.01");
			e1.fromString("12'345.99");
			e2.fromString("98'765.0" );
			break;

		case Locale::FRENCH_FR:
		case Locale::BULGARIAN_BG:
		case Locale::CZECH_CZ:
            // not space, it's 160 - NBSP
			e0.fromString("12 347,01");
			e1.fromString("12 345,99");
			e2.fromString("98 765,0" );
			break;

		default:
			e0.fromString("12.347,01");
			e1.fromString("12.345,99");
			e2.fromString("98.765,0" );
			break;
		}

		CHECK(e0 == d0);
		CHECK(e1 == d1);
		CHECK(e2 == d2);
	}
}

//----------------------------------------------------------------------------
TEST(fromEmptyString)
{
    Decimal d;
    Decimal d1(0.5);
    d.fromNonLocString("");
    
    CHECK_EQUAL("0.0", d.toNonLocString());
    
    Decimal res = d1 + d;
    
    CHECK_EQUAL("0.5", res.toNonLocString());
    
    
    d.fromString("");
    CHECK_EQUAL("0.0", d.toNonLocString());
}


//----------------------------------------------------------------------------

TEST_FIXTURE(Fixture, fromNonLocString)
{
	Decimal e0,
		    e1, 
			e2;

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

		e0.fromNonLocString("12347.01");
		e1.fromNonLocString("12345.99");
		e2.fromNonLocString("98765.0 ");

		CHECK(e0 == d0);
		CHECK(e1 == d1);
		CHECK(e2 == d2);
	}
}

//----------------------------------------------------------------------------
TEST(toXYZ)
{
	Locale::setLocale(Locale::C);

	CHECK_EQUAL(123455563.334, Decimal("123455563.334").toFloat64());
	CHECK_EQUAL(12346        , Decimal("12346.334")    .toInt16  ());
	CHECK_THROW(Decimal("123455563.334").toInt16(), OverflowException);
	CHECK_EQUAL(123455563    , Decimal("123455563.334").toLong32 ());
	CHECK_EQUAL(123455563    , Decimal("123455563.334").toInt32 ());
    CHECK_EQUAL(123455562    , Decimal("123455562.987").toInt32());

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		Locale::setLocale(j);

		const Float64 f = 1.23;
		Decimal       d(f);

		CHECK_CLOSE(f, d.toFloat64(), 0.000001);
	}

	Decimal dc(0);
	dc = Decimal(3);
	Float64 f = dc.toFloat64();
	CHECK_CLOSE(3.0, f, 0.0);
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, calc)
{
	CHECK_EQUAL(24693.00     , (d0 + d1).toFloat64());
	CHECK_EQUAL(    1.02     , (d0 - d1).toFloat64());
	CHECK_EQUAL(290154.735   , (d0 * Decimal(23.5)).toFloat64());
	CHECK_CLOSE(7.9991       , (d2 / d0).toFloat64(), 0.0001);

	CHECK_THROW((d0/Decimal()).toFloat64(), DivisionByZeroException);

	Locale::setLocale(Locale::C);

	Decimal e("12346.23456789");
	Decimal f("12346.23456789");

	e.round(3);
	f.trunc(4);

	CHECK_EQUAL(12346.235 , e.toFloat64());
	CHECK_EQUAL(12346.2345, f.toFloat64());

	Int32   i0 = -16;
	Int16   i1 = -17;
	Long32  l0 = -20;
	UInt32  u0 =  18;
	UInt16  u1 =  19;
	ULong32 u3 =  20;

	CHECK_CLOSE(12331.01, (d0 + i0).toFloat64(), 0.0001);
	CHECK_CLOSE(12330.01, (d0 + i1).toFloat64(), 0.0001);
	CHECK_CLOSE(12327.01, (d0 + l0).toFloat64(), 0.0001);
	CHECK_CLOSE(12365.01, (d0 + u0).toFloat64(), 0.0001);
	CHECK_CLOSE(12366.01, (d0 + u1).toFloat64(), 0.0001);
	CHECK_CLOSE(12367.01, (d0 + u3).toFloat64(), 0.0001);

	CHECK_CLOSE(12363.01, (d0 - i0).toFloat64(), 0.0001);
	CHECK_CLOSE(12364.01, (d0 - i1).toFloat64(), 0.0001);
	CHECK_CLOSE(12367.01, (d0 - l0).toFloat64(), 0.0001);
	CHECK_CLOSE(12329.01, (d0 - u0).toFloat64(), 0.0001);
	CHECK_CLOSE(12328.01, (d0 - u1).toFloat64(), 0.0001);
	CHECK_CLOSE(12327.01, (d0 - u3).toFloat64(), 0.0001);

	CHECK_CLOSE(-197552.16, (d0 * i0).toFloat64(), 0.0001);
	CHECK_CLOSE(-209899.17, (d0 * i1).toFloat64(), 0.0001);
	CHECK_CLOSE(-246940.20, (d0 * l0).toFloat64(), 0.0001);
	CHECK_CLOSE( 222246.18, (d0 * u0).toFloat64(), 0.0001);
	CHECK_CLOSE( 234593.19, (d0 * u1).toFloat64(), 0.0001);
	CHECK_CLOSE( 246940.20, (d0 * u3).toFloat64(), 0.0001);

	CHECK_CLOSE(-771.6881, (d0 / i0).toFloat64(), 0.0001);
	CHECK_CLOSE(-726.2947, (d0 / i1).toFloat64(), 0.0001);
	CHECK_CLOSE(-617.3505, (d0 / l0).toFloat64(), 0.0001);
	CHECK_CLOSE( 685.9450, (d0 / u0).toFloat64(), 0.0001);
	CHECK_CLOSE( 649.8426, (d0 / u1).toFloat64(), 0.0001);
	CHECK_CLOSE( 617.3505, (d0 / u3).toFloat64(), 0.0001);

	{
		Decimal s0( 2803.34 );
		Decimal s1( 99.99   );
		Decimal s2(100.00   );
		Decimal s3 = s0 * ( s1 / s2);

		CHECK_CLOSE (2803.06, s3.toFloat64(), 0.001);
	}

    {
        const Decimal s0( "42.5" );
        const Int32 i2( 12 );

        const Decimal expected("54.5");

        Decimal actual = i2 + s0;

        CHECK_EQUAL(expected, actual);
    }

    {
        const Decimal s0( "42.5" );
        const Int32 i2( 12 );

        const Decimal expected("510");

        Decimal actual = i2 * s0;

        CHECK_EQUAL(expected, actual);
    }

    {
        const Decimal s0( "42.5" );
        const Float64 f0( 12.0 );

        const Decimal expected("54.5");

        Decimal actual = f0 + s0;

        CHECK_EQUAL(expected, actual);
    }

    {
        const Decimal s0( "42.5" );
        const Float64 f0( 12.0 );

        const Decimal expected("510");

        Decimal actual = f0 * s0;

        CHECK_EQUAL(expected, actual);
    }

    {
        const Decimal s0( "42.5" );
        const Float32 f0( 12.0 );

        const Decimal expected("54.5");

        Decimal actual = f0 + s0;

        CHECK_EQUAL(expected, actual);
    }

    {
        const Decimal s0( "42.5" );
        const Float32 f0( 12.0 );

        const Decimal expected("510");

        Decimal actual = f0 * s0;

        CHECK_EQUAL(expected, actual);
    }


}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, assign)
{
	Decimal d;

	d = d0;

	CHECK(d == d0);
	CHECK(d.toFloat64() == d0.toFloat64());
	CHECK(d != d1);
	CHECK(d.toFloat64() != d1.toFloat64());

	d = d0;
	d += d1;
	CHECK_EQUAL(24693.00, d.toFloat64());

	d = d0;
	d -= d1;
	CHECK_EQUAL(    1.02, d.toFloat64());

	d = d0;
	d *= Decimal(23.5);
	CHECK_EQUAL(290154.735, d.toFloat64());

	d = d2;
	d /= d0;
	CHECK_CLOSE(7.9991, d.toFloat64(), 0.0001);

	CHECK_THROW(d0/=Decimal(), DivisionByZeroException);

	Int32   i0 = -16;
	Int16   i1 = -17;
	Long32  l0 = -20;
	UInt32  u0 =  18;
	UInt16  u1 =  19;
	ULong32 u3 =  20;

	d0 = Decimal(12347.01);

	CHECK_CLOSE(12331.01                         , (d0 += i0).toFloat64(), 0.0001);
	CHECK_CLOSE(12330.01 + i0                    , (d0 += i1).toFloat64(), 0.0001);
	CHECK_CLOSE(12327.01 + i0 + i1               , (d0 += l0).toFloat64(), 0.0001);
	CHECK_CLOSE(12365.01 + i0 + i1 + l0          , (d0 += u0).toFloat64(), 0.0001);
	CHECK_CLOSE(12366.01 + i0 + i1 + l0 + u0     , (d0 += u1).toFloat64(), 0.0001);
	CHECK_CLOSE(12367.01 + i0 + i1 + l0 + u0 + u1, (d0 += u3).toFloat64(), 0.0001);

	d0 = Decimal(12347.01);

	CHECK_CLOSE(12363.01                         , (d0 -= i0).toFloat64(), 0.0001);
	CHECK_CLOSE(12364.01 - i0                    , (d0 -= i1).toFloat64(), 0.0001);
	CHECK_CLOSE(12367.01 - i0 - i1               , (d0 -= l0).toFloat64(), 0.0001);
	CHECK_CLOSE(12329.01 - i0 - i1 - l0          , (d0 -= u0).toFloat64(), 0.0001);
	CHECK_CLOSE(12328.01 - i0 - i1 - l0 - u0     , (d0 -= u1).toFloat64(), 0.0001);
	CHECK_CLOSE(12327.01 - i0 - i1 - l0 - u0 - u1, (d0 -= u3).toFloat64(), 0.0001);

	d0 = Decimal(12347.01);

	CHECK_CLOSE(-197552.16                         , (d0 *= i0).toFloat64(), 0.0001);
	CHECK_CLOSE(-209899.17 * i0                    , (d0 *= i1).toFloat64(), 0.0001);
	CHECK_CLOSE(-246940.20 * i0 * i1               , (d0 *= l0).toFloat64(), 0.0001);
	CHECK_CLOSE( 222246.18 * i0 * i1 * l0          , (d0 *= u0).toFloat64(), 0.0001);
	CHECK_CLOSE( 234593.19 * i0 * i1 * l0 * u0     , (d0 *= u1).toFloat64(), 0.0001);
	CHECK_CLOSE( 246940.20 * i0 * i1 * l0 * u0 * u1, (d0 *= u3).toFloat64(), 0.0001);

	d0 = Decimal(12347.01);

	CHECK_CLOSE(-771.6881                         , (d0 /= i0).toFloat64(), 0.0001);
	CHECK_CLOSE(-726.2947 / i0                    , (d0 /= i1).toFloat64(), 0.0001);
	CHECK_CLOSE(-617.3505 / i0 / i1               , (d0 /= l0).toFloat64(), 0.0001);
	CHECK_CLOSE( 685.9450 / i0 / i1 / l0          , (d0 /= u0).toFloat64(), 0.0001);
	CHECK_CLOSE( 649.8426 / i0 / i1 / l0 / u0     , (d0 /= u1).toFloat64(), 0.0001);
	CHECK_CLOSE( 617.3505 / i0 / i1 / l0 / u0 / u1, (d0 /= u3).toFloat64(), 0.0001);

}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, compare)
{
	{
		Decimal d5(0.0),
				d6(d5);

		CHECK(! (d0 == d1));
		CHECK(  (d0 != d1));
		CHECK(  (d0 > d1));
		CHECK(  (d0 >= d1));
		CHECK(! (d0 < d1));
		CHECK(! (d0 <= d1));

		CHECK(   d5 == d6  );
		CHECK(! (d5 != d6) );

		CHECK(   d1 != d5  );
		CHECK(! (d1 == d5) );
	}


	{
		Locale::setLocale(Locale::C);

		Decimal t1("1.11");
		Decimal t2(1.11);
		Decimal t3(1.11);

		CHECK( ! (t1 == t2) );
		CHECK(   (t3 == t2) );

		t2.trunc(2);
		t3.round(2);

		CHECK(  (t1 == t2) );
		CHECK(  (t1 == t3) );
	}

	{
		Locale::setLocale(Locale::C);

		Decimal t1("0.99");
		Decimal t2(0.99);
		Decimal t3(0.99);

		CHECK( ! (t1 == t2) );
		CHECK(   (t3 == t2) );

		t2.trunc(2);
		t3.round(2);

		CHECK( !(t1 == t2) );
		CHECK(  (t1 == t3) );
	}
}

//------------------------------------------------------------------------------
}	// end TESTSUITE

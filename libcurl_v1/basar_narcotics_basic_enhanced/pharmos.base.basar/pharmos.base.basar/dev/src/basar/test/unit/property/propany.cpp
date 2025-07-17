#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include <limits>
#include <property_any.hpp>


using basar::VarString;
using basar::Float64;
using basar::property::any;
using basar::property::any_cast;

//------------------------------------------------------------------------------
SUITE(property_Test_PropertyDescription)
{

//------------------------------------------------------------------------------
TEST(any_memleaks)
{
	VarString s1("test1");

	any a1;
	any a2(123456);
	any a3((Float64)1.243);
	any a4(s1);

	any a5(a1);
	any a6(a2);
	any a7(a3);
	any a8(a4);

    CHECK_EQUAL( false, a2.empty() );

	CHECK      (a5.empty());
	CHECK_EQUAL(123456, a6.getInt32());
	CHECK_CLOSE(1.243 , a7.getFloat64(), 0.00001);
	CHECK_EQUAL(s1    , *any_cast<VarString>(&a8));

	{
		any b1;
		b1 = 123456;

		any b2;
		b2 = (Float64)1.243;

		any b3;
		b3 = s1;

		any b4;
		b4 = a1;

		any b5;
		b5 = a2;

		any b6;
		b6 = a3;

		any b7;
		b7 = a4;

		CHECK_EQUAL(123456, b1.getInt32());
		CHECK_CLOSE(1.243 , b2.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b3));
		CHECK      (b4.empty());
		CHECK_EQUAL(123456, b5.getInt32());
		CHECK_CLOSE(1.243 , b6.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b7));
	}

	{
		any b1(123456);
		b1 = 123456;

		any b2(123456);
		b2 = (Float64)1.243;

		any b3(123456);
		b3 = s1;

		any b4(123456);
		b4 = a1;

		any b5(123456);
		b5 = a2;

		any b6(123456);
		b6 = a3;

		any b7(123456);
		b7 = a4;

		CHECK_EQUAL(123456, b1.getInt32());
		CHECK_CLOSE(1.243 , b2.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b3));
		CHECK      (b4.empty());
		CHECK_EQUAL(123456, b5.getInt32());
		CHECK_CLOSE(1.243 , b6.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b7));
	}

	{
		any b1(s1);
		b1 = 123456;

		any b2(s1);
		b2 = (Float64)1.243;

		any b3(s1);
		b3 = s1;

		any b4(s1);
		b4 = a1;

		any b5(s1);
		b5 = a2;

		any b6(s1);
		b6 = a3;

		any b7(s1);
		b7 = a4;

		CHECK_EQUAL(123456, b1.getInt32());
		CHECK_CLOSE(1.243 , b2.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b3));
		CHECK      (b4.empty());
		CHECK_EQUAL(123456, b5.getInt32());
		CHECK_CLOSE(1.243 , b6.getFloat64(), 0.00001);
		CHECK_EQUAL(s1    , *any_cast<VarString>(&b7));
	}

    {
        any b1(std::numeric_limits<basar::Int64>::max());

        CHECK_EQUAL(std::numeric_limits<basar::Int64>::max(), b1.getInt64());

        b1 = 0;

        CHECK_EQUAL(0, b1.getInt64());
        CHECK(b1.empty());

        b1 = std::numeric_limits<basar::Int64>::min();

        CHECK_EQUAL(std::numeric_limits<basar::Int64>::min(), b1.getInt64());
    }
}

//------------------------------------------------------------------------------
}	// end SUITE


#include <libbasarcmnutil.h>

#include "testcmnutil.h"

//------------------------------------------------------------------------------
using basar::cmnutil::I18nString;
using basar::cmnutil::Locale;
using basar::cmnutil::Decimal;

//------------------------------------------------------------------------------
void test_decimal()
{
	printf("\n");
	printf("--- test_decimal ---------------------------\n");
	printf("\n");

	for (basar::Int8 j = 0; j < 2; ++j)
	{
		if (0 == j)
		{
			Locale::setLocale(Locale::GERMAN_DE);
			printf("Decimal::setLocale(basar::cmnutil::GERMAN)\n");
		}
		else
		{
			Locale::setLocale(Locale::ENGLISH_GB);
			printf("Decimal::setLocale(basar::cmnutil::ENGLISH)\n");
		}

		{
			Decimal d1(7.77);
			Decimal d2(2.555);
			Decimal d4(5);
			basar::Float64 flo; 

			flo = d1.toFloat64();
			printf("flo %11.4f; Float64 %11.4f; toString <%.8s>\n", flo, d1.toFloat64(), d1.toString().c_str());
			flo = d2.toFloat64();
			printf("flo %11.4f; Float64 %11.4f; toString <%.8s>\n", flo, d2.toFloat64(), d2.toString().c_str());
			flo = d4.toFloat64();
			printf("flo %11.4f; Float64 %11.4f; toString <%.8s>\n", flo, d4.toFloat64(), d4.toString().c_str());

			Decimal d3(d1);
			flo = d3.toFloat64();
			printf("flo %11.4f; Float64 %11.4f; toString <%.8s>\n", flo, d3.toFloat64(), d3.toString().c_str());

			d1 = d2 + d3;
			flo = d1.toFloat64();
			printf("flo %11.4f: <%.8s> = <%.8s> + <%.8s>\n", flo, d1.toString().c_str(), d2.toString().c_str(), d3.toString().c_str());
			I18nString tmp = d1.toString();
			d1 /= d2;
			flo = d1.toFloat64();
			printf("flo %11.4f: <%.8s> = <%.8s> / <%.8s>\n", flo, d1.toString().c_str(), tmp.c_str(), d2.toString().c_str());

			if ( d1 == d3 )
				assert (0);

			Decimal d5(0.0);
			Decimal d6(0.0);

			printf("d1 == d2 = %d\n", d1 == d2);
			printf("d1 == d1 = %d\n", d1 == d1);
			printf("d5 == d2 = %d\n", d5 == d2);
			printf("d5 == d6 = %d\n", d5 == d6);
		}

		printf("\n");

		{
			Decimal        d1(7.77),
						d2,
						d4(5);
			basar::Float64 flo;

			I18nString str = "2.005,77";
			d2.fromString(str);
			flo = d2.toFloat64();
			printf("flo            %11.4f\n"
				"string         <%20.20s>; Float64 %11.4f\n"
				"toString       <%20.20s>\n", 
					flo,
					str.c_str(),
					d2.toFloat64(),
					d2.toString().c_str());


			str = d4.toString(4);
			printf("tostring       <%20.20s>; Float64 %11.4f\n", 
					str.c_str(),
					d4.toFloat64());

			str = d1.toString(Decimal::s_ALL_DECIMAL_PLACES);
			printf("tostring       <%20.20s>; Float64 %11.4f\n", 
					str.c_str(),
					d1.toFloat64());

			str = "223534534005,77";
			d2.fromString(str);
			printf("decimal string <%20.20s>; Float64 %11.4f\n"
				"toString       <%20.20s>\n"
				"toSepString    <%20.20s>\n", 
					str.c_str     (),
					d2.toFloat64  (),
					d2.toString   ().c_str(),
					d2.toSepString().c_str());
		}

		printf("\n");

		{
			double f = 12346.23456789;
			Decimal dd( f );
			printf("double %20.9f converted to:\n", f);
			printf("  toString    <%s>\n"
				"  toSepString <%s>\n"
				"  toFloat64   %f\n"
				"  toLong32    %ld\n",
				dd.toString   ().c_str(),
				dd.toSepString().c_str(),
				dd.toFloat64  (),
				dd.toLong32   ());
		}

		printf("\n");

		{
			const basar::Int8 tr = 3;
			const basar::Int8 rd = 4;
			const double      f2 = 12346.23456789;
			printf("double %20.9f to:\n", f2);
			Decimal dd2( f2 );
			dd2.trunc( 3 );
			printf("  truncated(%d): %f %s\n", tr, dd2.toFloat64(), dd2.toString().c_str());
			Decimal dd3( f2 );
			dd3.round( 4 );
			printf("  rounded(%d)  : %f %s\n", rd, dd3.toFloat64(), dd3.toString().c_str());
		}

		printf("\n");
	}

	{
		const char * const DEC[] = 
		{
			"2.005,77",
			"2,005.77",
			"2005,77",
			"2005.77",
			"200577"
		};
		Decimal    dStr;
		I18nString str;

		Locale::setLocale(Locale::GERMAN_DE);
		printf("Decimal::setLocale(basar::cmnutil::GERMAN)\n");

		for (basar::UInt8 i = 0; i < (sizeof(DEC)/sizeof(*DEC)); ++i)
		{
			str = DEC[i];
			dStr.fromString(str);

			printf("de: decimal string <%8.8s>; Float64 %11.4f; toString <%8.8s>\n", 
				   DEC[i],
				   dStr.toFloat64(),
				   dStr.toString().c_str());
		}

		Locale::setLocale(Locale::ENGLISH_GB);
		printf("Decimal::setLocale(basar::cmnutil::ENGLISH)\n");

		printf("\n");

		for (basar::UInt8 i = 0; i < (sizeof(DEC)/sizeof(*DEC)); ++i)
		{
			str = DEC[i];
			dStr.fromString(str);

			printf("en: decimal string <%8.8s>; Float64 %11.4f; toString <%8.8s>\n", 
				   DEC[i],
				   dStr.toFloat64(),
				   dStr.toString().c_str());
		}
	}

	printf("\n\n");
}


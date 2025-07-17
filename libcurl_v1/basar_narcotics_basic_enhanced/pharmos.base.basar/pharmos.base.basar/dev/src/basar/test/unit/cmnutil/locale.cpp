#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

//------------------------------------------------------------------------------
using basar::cmnutil::Locale;
using basar::I18nString;
using basar::ConstString;
using basar::UInt32;

//------------------------------------------------------------------------------
SUITE(TestLocale)
{

//------------------------------------------------------------------------------
TEST(LocaleByString)
{
	ConstString strUPPER[] = 
	{
		"C"                        ,
		"GERMAN_DE"                ,
		"GERMAN_AT"                ,
		"GERMAN_CH"                ,
		"ENGLISH_GB"               ,
		"ENGLISH_US"               ,
		"FRENCH_FR"                ,
		"FRENCH_CH"                ,
		"CROATIAN_HR"              ,
		"BULGARIAN_BG"             ,
		"BOSNIAN_BA"               ,
		"SERBIAN_RS"               ,
		"CZECH_CZ"                 
	};

	ConstString strLOWER[] = 
	{
		"c"                        ,
		"german_de"                ,
		"german_at"                ,
		"german_ch"                ,
		"english_gb"               ,
		"english_us"               ,
		"french_fr"                ,
		"french_ch"                ,
		"croatian_hr"              ,
		"bulgarian_bg"             ,
		"bosnian_ba"               ,
		"serbian_rs"               ,
		"czech_cz"                 
	};

	Locale::LanguageEnum enumerated[] = 
	{
		Locale::C           ,
		Locale::GERMAN_DE	,
		Locale::GERMAN_AT	,
		Locale::GERMAN_CH	,
		Locale::ENGLISH_GB	,
		Locale::ENGLISH_US	,
		Locale::FRENCH_FR	,
		Locale::FRENCH_CH	,
		Locale::CROATIAN_HR	,
		Locale::BULGARIAN_BG,
		Locale::BOSNIAN_BA	,
		Locale::SERBIAN_RS	,
		Locale::CZECH_CZ
	};

	for (size_t i = 0; i < sizeof(strUPPER)/sizeof(strUPPER[0]); ++i)
	{
		CHECK_EQUAL(enumerated[i], Locale::getLocaleByString(strUPPER[i]));
	}

	for (size_t i = 0; i < sizeof(strLOWER)/sizeof(strLOWER[0]); ++i)
	{
		CHECK_EQUAL(enumerated[i], Locale::getLocaleByString(strLOWER[i]));
	}

	CHECK_EQUAL(Locale::LOCALE_FIRST,      Locale::getLocaleByString(strUPPER[0]));
	CHECK_EQUAL(Locale::LOCALE_NUMBER - 1, Locale::getLocaleByString(strUPPER[sizeof(strLOWER)/sizeof(strLOWER[0]) - 1]));

	CHECK_EQUAL(Locale::LOCALE_INVALID   , Locale::getLocaleByString("nonsense"));
}

//------------------------------------------------------------------------------
TEST(CodepageByString)
{
	ConstString strUPPER[] = 
	{
		"WEST",
		"EAST",
		"CYRILLIC"
	};

	ConstString strLOWER[] = 
	{
		"west",
		"east",
		"cyrillic"
	};

	Locale::CodepageEnum enumerated[] = 
	{
		Locale::CP_WEST,
		Locale::CP_EAST,
		Locale::CP_CYRILLIC
	};

	for (size_t i = 0; i < sizeof(strUPPER)/sizeof(strUPPER[0]); ++i)
	{
		CHECK_EQUAL(enumerated[i], Locale::getCodepageByString(strUPPER[i]));
	}

	for (size_t i = 0; i < sizeof(strLOWER)/sizeof(strLOWER[0]); ++i)
	{
		CHECK_EQUAL(enumerated[i], Locale::getCodepageByString(strLOWER[i]));
	}

	CHECK_EQUAL(Locale::CP_FIRST,	   Locale::getCodepageByString(strUPPER[0]));
	CHECK_EQUAL(Locale::CP_NUMBER - 1, Locale::getCodepageByString(strUPPER[sizeof(strLOWER)/sizeof(strLOWER[0]) - 1]));

	CHECK_EQUAL(Locale::CP_INVALID   , Locale::getCodepageByString("nonsense"));
}

//------------------------------------------------------------------------------
TEST(DateTimeSeparator)
{
	ConstString FMTDATESEP[] = 
	{
		"/",
		".",
		".",
		".",
		"/",
		"/",
		"/",
		".",
		".",
		".",
		".",
		".",
		"."
	};

	ConstString FMTTIMESEP[] = 
	{
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":",
		":"
	};

	ConstString FMTTIMEFRACSEP[] = 
	{
		".",
		",",
		",",
		".",
		".",
		".",
		",",
		".",
		",",
		",",
		",",
		",",
		","
	};

	CHECK_EQUAL(sizeof(FMTDATESEP)    /sizeof(FMTDATESEP[    0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(FMTTIMESEP)    /sizeof(FMTTIMESEP    [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(FMTTIMEFRACSEP)/sizeof(FMTTIMEFRACSEP[0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		Locale::setLocale(j);

		CHECK_EQUAL(FMTDATESEP    [j], Locale::getDateSep    ());
		CHECK_EQUAL(FMTTIMESEP    [j], Locale::getTimeSep    ());
		CHECK_EQUAL(FMTTIMEFRACSEP[j], Locale::getTimeFracSep());
	}
}

//------------------------------------------------------------------------------
TEST(DecimalSeparator)
{
	ConstString FMTDECSEP[] = 
	{
		".",
		",",
		",",
		".",
		".",
		".",
		",",
		".",
		",",
		",",
		",",
		",",
		","
	};

	ConstString FMTDEC1000THSEP[] = 
	{
		",",
		".",
		".",
		"'",
		",",
		",",
		" ", // not space, it's 160 - NBSP
		"'",
		".",
		" ", // not space, it's 160 - NBSP
		".",
		".",
		" "  // not space, it's 160 - NBSP
	};

	CHECK_EQUAL(sizeof(FMTDECSEP)      /sizeof(FMTDECSEP      [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(FMTDEC1000THSEP)/sizeof(FMTDEC1000THSEP[0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		Locale::setLocale(j);

		CHECK_EQUAL(FMTDECSEP      [j], Locale::getDecSep      (j));
		CHECK_EQUAL(FMTDEC1000THSEP[j], Locale::getDec1000thSep(j));
	}
}

//------------------------------------------------------------------------------
}	// end TESTSUITE

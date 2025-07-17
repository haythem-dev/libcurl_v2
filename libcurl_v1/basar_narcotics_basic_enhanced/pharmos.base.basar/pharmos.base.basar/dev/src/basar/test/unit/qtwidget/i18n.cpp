//----------------------------------------------------------------------------
/*! \file
 *  \brief  i18n functions regarding Qt
 *  \author Michael Eichenlaub
 *  \date   24.06.2009
 */
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"

#include <QtCore/QTextCodec>


//------------------------------------------------------------------------------
using basar::ConstString;
using basar::UInt32;
using basar::cmnutil::Locale;
using basar::gui::widget::I18n;

//------------------------------------------------------------------------------
SUITE(TestI18n)
{

//------------------------------------------------------------------------------
// after dll init
TEST(initialsettings)
{
	CHECK_EQUAL(Locale::LOCALE_DEFAULT, Locale::getLocale());
	CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_DEFAULT), QTextCodec::codecForLocale()->name().constData());
	CHECK      (I18n::getQtLocale(Locale::LOCALE_DEFAULT) == QLocale());

	CHECK_EQUAL("dd.MM.yyyy hh:mm:ss", I18n::getPhDateTimeFormat());
	CHECK_EQUAL("dd.MM.yyyy"         , I18n::getPhDateFormat    ());
	CHECK_EQUAL("hh:mm:ss"           , I18n::getPhTimeFormat    ());
}

//------------------------------------------------------------------------------
TEST(localeset)
{
	const Locale::LanguageEnum LANGS[] = 
	{
		Locale::C            ,
		Locale::GERMAN_DE    ,
		Locale::GERMAN_AT    ,
		Locale::GERMAN_CH    ,
		Locale::ENGLISH_GB   ,
		Locale::ENGLISH_US   ,
		Locale::FRENCH_FR    ,
		Locale::FRENCH_CH    ,
		Locale::CROATIAN_HR  ,
		Locale::BULGARIAN_BG ,
		Locale::BOSNIAN_BA   ,
		Locale::SERBIAN_RS   ,
		Locale::CZECH_CZ
	};

	ConstString CODECS[] = 
	{
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1252",
		"windows-1251",
		"windows-1252",
		"windows-1252",
		"windows-1252"
	};

	const QLocale LOCALES[] = 
	{
		QLocale(QLocale::C        , QLocale::AnyCountry          ),
		QLocale(QLocale::German   , QLocale::Germany             ),
		QLocale(QLocale::German   , QLocale::Austria             ),
		QLocale(QLocale::German   , QLocale::Switzerland         ),
		QLocale(QLocale::English  , QLocale::UnitedKingdom       ),
		QLocale(QLocale::English  , QLocale::UnitedStates        ),
		QLocale(QLocale::French   , QLocale::France              ),
		QLocale(QLocale::French   , QLocale::Switzerland         ),
		QLocale(QLocale::Croatian , QLocale::Croatia             ),
		QLocale(QLocale::Bulgarian, QLocale::Bulgaria            ),
		QLocale(QLocale::Bosnian  , QLocale::BosniaAndHerzegowina),
		QLocale(QLocale::Serbian  , QLocale::Serbia				 ),
		QLocale(QLocale::Czech    , QLocale::CzechRepublic       )
	};

	CHECK_EQUAL(sizeof(LANGS)  /sizeof(LANGS  [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(CODECS) /sizeof(CODECS [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(LOCALES)/sizeof(LOCALES[0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		I18n::setI18n(j);

		CHECK_EQUAL(LANGS  [j], Locale::getLocale());
		CHECK_EQUAL(CODECS [j], QTextCodec::codecForLocale()->name().constData());
		CHECK      (LOCALES[j] == QLocale());
	}
}

//------------------------------------------------------------------------------
TEST(LocaleSetWithDifferingCodepage)
{
	const Locale::LanguageEnum LANGS[] = 
	{
		Locale::C            ,
		Locale::GERMAN_DE    ,
		Locale::GERMAN_AT    ,
		Locale::GERMAN_CH    ,
		Locale::ENGLISH_GB   ,
		Locale::ENGLISH_US   ,
		Locale::FRENCH_FR    ,
		Locale::FRENCH_CH    ,
		Locale::CROATIAN_HR  ,
		Locale::BULGARIAN_BG ,
		Locale::BOSNIAN_BA   ,
		Locale::SERBIAN_RS   ,
		Locale::CZECH_CZ
	};

	ConstString CODECS[] = 
	{
		"windows-1252",
		"windows-1250",
		"windows-1251",
		"windows-1252",
		"windows-1250",
		"windows-1251",
		"windows-1252",
		"windows-1250",
		"windows-1251",
		"windows-1252",
		"windows-1250",
		"windows-1251",
		"windows-1252"
	};

	const QLocale LOCALES[] = 
	{
		QLocale(QLocale::C        , QLocale::AnyCountry          ),
		QLocale(QLocale::German   , QLocale::Germany             ),
		QLocale(QLocale::German   , QLocale::Austria             ),
		QLocale(QLocale::German   , QLocale::Switzerland         ),
		QLocale(QLocale::English  , QLocale::UnitedKingdom       ),
		QLocale(QLocale::English  , QLocale::UnitedStates        ),
		QLocale(QLocale::French   , QLocale::France              ),
		QLocale(QLocale::French   , QLocale::Switzerland         ),
		QLocale(QLocale::Croatian , QLocale::Croatia             ),
		QLocale(QLocale::Bulgarian, QLocale::Bulgaria            ),
		QLocale(QLocale::Bosnian  , QLocale::BosniaAndHerzegowina),
		QLocale(QLocale::Serbian  , QLocale::Serbia				 ),
		QLocale(QLocale::Czech    , QLocale::CzechRepublic       )
	};

	CHECK_EQUAL(sizeof(LANGS)  /sizeof(LANGS  [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(CODECS) /sizeof(CODECS [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(LOCALES)/sizeof(LOCALES[0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		I18n::setI18n(j, static_cast< Locale::CodepageEnum >(j % Locale::CP_NUMBER));

		CHECK_EQUAL(LANGS  [j], Locale::getLocale());
		CHECK_EQUAL(CODECS [j], QTextCodec::codecForLocale()->name().constData());
		CHECK      (LOCALES[j] == QLocale());
	}
}

//------------------------------------------------------------------------------
TEST(PhoenixDateTimeFormat)
{
	ConstString FMTDATETIME[] = 
	{
		"MM/dd/yyyy hh:mm:ss AP",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd/MM/yyyy hh:mm:ss",
		"MM/dd/yyyy hh:mm:ss AP",
		"dd/MM/yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss",
		"dd.MM.yyyy hh:mm:ss"
	};

	ConstString FMTDATE[] = 
	{
		"MM/dd/yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd/MM/yyyy",
		"MM/dd/yyyy",
		"dd/MM/yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy",
		"dd.MM.yyyy"
	};

	ConstString FMTTIME[] = 
	{
		"hh:mm:ss AP",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss AP",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss",
		"hh:mm:ss"
	};

	CHECK_EQUAL(sizeof(FMTDATETIME)/sizeof(FMTDATETIME[0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(FMTDATE)    /sizeof(FMTDATE    [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));
	CHECK_EQUAL(sizeof(FMTTIME)    /sizeof(FMTTIME    [0]), static_cast<UInt32>(Locale::LOCALE_NUMBER));

	for (Locale::LanguageEnum j = Locale::LOCALE_FIRST; 
		 j < Locale::LOCALE_NUMBER; 
		 j = static_cast<Locale::LanguageEnum>(j + 1))
	{
		I18n::setI18n(j);
		CHECK_EQUAL(FMTDATETIME[j], I18n::getPhDateTimeFormat());
		CHECK_EQUAL(FMTDATE    [j], I18n::getPhDateFormat    ());
		CHECK_EQUAL(FMTTIME    [j], I18n::getPhTimeFormat    ());
	}
}

//------------------------------------------------------------------------------
}	// SUITE

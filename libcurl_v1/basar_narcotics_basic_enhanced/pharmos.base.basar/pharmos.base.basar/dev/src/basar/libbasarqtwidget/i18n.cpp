//----------------------------------------------------------------------------
/*! \file
 *  \brief  i18n functions regarding Qt
 *  \author Michael Eichenlaub
 *  \date   24.06.2009
 */
//----------------------------------------------------------------------------

#include "libbasarqtwidget_i18n.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#pragma warning(disable: 4251) //C4251: 'QTextCodec::ConverterState::flags' : class 'QFlags<Enum>' needs to have dll-interface to be used by clients of struct 'QTextCodec::ConverterState'
#pragma warning(disable: 4512) //C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include <QtCore/QLocale>
#include <QtCore/QTextCodec>
#ifdef _WIN32
#pragma warning (pop)
#endif



//----------------------------------------------------------------------------
namespace basar	 {
namespace gui	 {
namespace widget {
	
//----------------------------------------------------------------------------
using basar::ConstString;
using basar::cmnutil::Locale;

//----------------------------------------------------------------------------
//! initialize Qt regarding i18n and check success
static bool initQtSettings()
{
	QLocale      qtlocale  = I18n::getQtLocale   (Locale::getLocale  ());
	ConstString  codecname = I18n::getQtCodecName(Locale::getCodepage());

	QLocale   ::setDefault         (qtlocale);								// necessary for displaying decimal points/commata
	QTextCodec::setCodecForLocale   (QTextCodec::codecForName(codecname));	// necessary for QString.[from/to]Local8Bit() <=> I18nString ( char * ) 

	return true;
}

//----------------------------------------------------------------------------
static bool initI18n = initQtSettings();	//!< initialize Qt regarding i18n

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void I18n::setI18n(basar::cmnutil::Locale::LanguageEnum eLocale)
{
	setI18n(eLocale, Locale::getPhoenixCodepage(eLocale));
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void I18n::setI18n(basar::cmnutil::Locale::LanguageEnum eLocale,
				   basar::cmnutil::Locale::CodepageEnum eCodepage)
{
	QLocale      qtlocale  = getQtLocale   (eLocale  );
	ConstString  codecname = getQtCodecName(eCodepage);

	Locale    ::setLocale          (eLocale);								// necessary for basar decimals, date, time
	Locale    ::setCodepage        (eCodepage);								// necessary for basar string

	QLocale   ::setDefault         (qtlocale);								// necessary for displaying decimal points/commata at Qt widgets
	QTextCodec::setCodecForLocale  (QTextCodec::codecForName(codecname));	// necessary for QString.[from/to]Local8Bit() <=> I18nString ( char * ) 
}

//----------------------------------------------------------------------------
//! \throw	no-throw
//! \return Qt codec name for given basar codepage/charset
//!
ConstString I18n::getQtCodecName(basar::cmnutil::Locale::CodepageEnum eCodepage)
{
	static ConstString CODECS[] = 
	{
		"windows-1252",
		"windows-1251",
		"windows-1250"
	};

	switch (eCodepage)
	{
	case Locale::CP_EAST:
		return CODECS[2];

	case Locale::CP_CYRILLIC:
		return CODECS[1];

	case Locale::CP_WEST:
	default:
		return CODECS[0];
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return Qt locale for given basar locale
//!
QLocale I18n::getQtLocale(basar::cmnutil::Locale::LanguageEnum eLocale)
{
	switch (eLocale)
	{
	case Locale::C:				return QLocale(QLocale::C        , QLocale::AnyCountry          );
	case Locale::ENGLISH_US:	return QLocale(QLocale::English  , QLocale::UnitedStates        );
	case Locale::GERMAN_AT:		return QLocale(QLocale::German   , QLocale::Austria             );
	case Locale::GERMAN_CH:		return QLocale(QLocale::German   , QLocale::Switzerland         );
	case Locale::ENGLISH_GB:	return QLocale(QLocale::English  , QLocale::UnitedKingdom       );
	case Locale::FRENCH_FR:		return QLocale(QLocale::French   , QLocale::France              );
	case Locale::FRENCH_CH:		return QLocale(QLocale::French   , QLocale::Switzerland         );
	case Locale::CROATIAN_HR:	return QLocale(QLocale::Croatian , QLocale::Croatia             );
	case Locale::BULGARIAN_BG:	return QLocale(QLocale::Bulgarian, QLocale::Bulgaria            );
	case Locale::BOSNIAN_BA:	return QLocale(QLocale::Bosnian  , QLocale::BosniaAndHerzegowina);
	case Locale::SERBIAN_RS:	return QLocale(QLocale::Serbian  , QLocale::Serbia              );
	case Locale::CZECH_CZ:		return QLocale(QLocale::Czech    , QLocale::CzechRepublic       );
	case Locale::GERMAN_DE:
	default:					return QLocale(QLocale::German   , QLocale::Germany             );
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return Phoenix display format string regarding QDateTimeEdit (or derived class)
//!
ConstString	I18n::getPhDateTimeFormat(basar::cmnutil::Locale::LanguageEnum eLocale /* = getLocale() */)
{
	switch ( Locale::getDateFormat(eLocale) )
	{
	case Locale::FMT_DATE_AMERICAN:
        return "MM/dd/yyyy hh:mm:ss AP";

	case Locale::FMT_DATE_EUROPEAN_SLASH:
        return "dd/MM/yyyy hh:mm:ss";

	case Locale::FMT_DATE_EUROPEAN:
    default:
        return "dd.MM.yyyy hh:mm:ss";
    }
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return Phoenix display format string regarding QDateEdit (or derived class)
//!
ConstString	I18n::getPhDateFormat(basar::cmnutil::Locale::LanguageEnum eLocale /* = getLocale() */)
{
	switch ( Locale::getDateFormat(eLocale))
	{
	case Locale::FMT_DATE_AMERICAN:
		return "MM/dd/yyyy";

	case Locale::FMT_DATE_EUROPEAN_SLASH:
		return "dd/MM/yyyy";

	case Locale::FMT_DATE_EUROPEAN:
    default:
        return "dd.MM.yyyy";
    }
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return Phoenix display format string regarding QTimeEdit (or derived class)
//!
ConstString	I18n::getPhTimeFormat (basar::cmnutil::Locale::LanguageEnum eLocale /* = getLocale() */)
{
    switch (Locale::getTimeFormat(eLocale))
    {
	case Locale::FMT_TIME_AMERICAN:
        return "hh:mm:ss AP";

    case Locale::FMT_TIME_EUROPEAN:
    default:
        return "hh:mm:ss";
    }
}

//----------------------------------------------------------------------------
}	// namespace widget
}	// namespace gui
}	// namespace basar

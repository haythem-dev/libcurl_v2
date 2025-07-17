//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for language dependencies
 *  \author M. Eichenlaub
 *  \date   27.02.2007
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_locale.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasardebug.h"

#include "loggerpool.h"

#include <string.h>

#include <sqlhdr.h>

#include <clocale>

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define putenv	_putenv		//!< use MS function
#endif

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! \brief  set Informix environment variable
//! \return success
//! \param  env environment variable to set: NAME=value
static bool ifxPutEnv (const char * const env)
{
    char * str = const_cast < char * > (env);
    
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	return (0 == ifx_putenv(str));
#else
	return (0 == putenv(str));
#endif
}

//----------------------------------------------------------------------------
//! \brief  query Informix environment variable
//! \return environment variable value or NULL
//! \param  env environment variable
static ConstBuffer ifxGetEnv (const char * const env)
{
    char * str = const_cast < char * > (env);

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	return ifx_getenv(str);
#else
	return getenv(str);
#endif
}

//----------------------------------------------------------------------------
bool Locale::initSystemSettings()
{
	bool        ret      = true;
	VarString   txt;

	// operating system environment variable, needed by C runtime
	static char ENV_OS_LC_ALL[] = "LC_ALL=C";
	
	// environment
	if ( ::putenv(ENV_OS_LC_ALL) )
	{
		ret = false;

		VarString msg;
		msg.format("\nLocale::initSystemSettings(): ::putenv(%s) failed", ENV_OS_LC_ALL);
		txt += msg;
	}

	// CRT locale
	char * pLocale = std::setlocale(LC_ALL, 0);
		
	if ( !pLocale  || 
		(0 != ::strncmp(pLocale, "C", 1)) )
	{
		ret = false;

		VarString msg;
		msg.format("\nLocale::initSystemSettings(): ::setlocale(LC_ALL, 0) returned %s",
			       pLocale ? pLocale : "NULL");
		txt += msg;
	}


	// controls decimal display
	const char * const pEnv = ifxGetEnv("DBMONEY"); 

	if ( pEnv && 
		 (0 == ::strchr(pEnv, '.')) )
	{
		ret = false;

		VarString msg;
        msg.format("\nLocale::initSystemSettings(): environment DBMONEY = %s",
			       pEnv);
		txt += msg;
	}

	// Informix environment variable
	static const char * const ENVINFX[] = 
	{
		"GL_DATE=%m/%d/%iY",								// formats date
		"GL_DATETIME=%04iY-%02m-%02d %02H:%02M:%02S%03F3",	// formats datetime
		"CLIENT_LOCALE=en_us.8859-1",                       // default client locale
		"DB_LOCALE=en_us.8859-1"                            // default db locale 
	};

	// --- Informix settings
	for (size_t i = 0; i < sizeof(ENVINFX)/sizeof(ENVINFX[0]); ++i)
	{
		if ( ! ifxPutEnv(ENVINFX[i]) )
		{
			ret = false;

			VarString msg;
			msg.format("\nLocale::initSystemSettings(): Informix putenv(%s) failed", ENVINFX[i]);
			txt += msg;
		}
	}
    
	
	// --- error handling
	if ( !ret )
	{
		basar::debug::printError(txt.c_str(), "error!");
		exit(EXIT_FAILURE);
	}

	return true;
}

//----------------------------------------------------------------------------
/*! get current basar locale reference, initialized by default (function avoids
    static init order fiasco)
    \return Language enum */
Locale::LanguageEnum & Locale::locale()
{
	static LanguageEnum loc = Locale::LOCALE_DEFAULT;
	return loc;
}

//----------------------------------------------------------------------------
//! \throw RuntimeException
//!
void Locale::setLocale(const LanguageEnum loc)
{
	if ( LOCALE_NUMBER <= loc )
	{
		VarString msg;
		msg.format("locale %d not valid", loc);
		
		ExceptInfo sInfo("basar.cmnutil.Locale.setLocale", msg, __FILE__, __LINE__);
		throw RuntimeException(sInfo);	
	}

	locale() = loc;

	if (LoggerPool::locale().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("setLocale(%d) set", loc);
		LoggerPool::locale().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//!	\return global locale
Locale::LanguageEnum Locale::getLocale()
{
	return locale();
}

//----------------------------------------------------------------------------
//! \return locale corresponding to string; LOCALE_INVALID if not found
Locale::LanguageEnum Locale::getLocaleByString (const VarString & loc)
{
    VarString key = loc;
    key.upper();

    LanguageEnum lang;

    for (size_t i = 0; i < LOCALE_NUMBER; ++i)
    {
        lang = static_cast<LanguageEnum>(i);

        if (0 == ::strcmp(getLocaleString(lang), key.c_str()))
            return lang;
    }

    return LOCALE_INVALID;
}

//----------------------------------------------------------------------------
//! \return locale enum as string (basar specific): e.g: "CROATIAN_HR"
ConstBuffer Locale::getLocaleString(const LanguageEnum loc /* = getLocale() */)
{
    // enum as strings
	static ConstString LOCALE[] =
	{
		"C"           ,	// locale: C
		"GERMAN_DE"   ,	// locale: German Germany
		"GERMAN_AT"   ,	// locale: German Austria
		"GERMAN_CH"   ,	// locale: German Switzerland
		"ENGLISH_GB"  ,	// locale: English United Kingdom
		"ENGLISH_US"  ,	// locale: English United Stated
		"FRENCH_FR"   ,	// locale: French France
		"FRENCH_CH"   ,	// locale: French Switzerland
		"CROATIAN_HR" ,	// locale: Croatian Croatia
		"BULGARIAN_BG",	// locale: Bulgarian Bulgaria
		"BOSNIAN_BA"  ,	// locale: Bosnian Bosnia and Herzegovina - Latin
		"SERBIAN_RS"  ,	// locale: Serbian Serbia - Latin
		"CZECH_CZ"	    // locale: Czech Czech Republic
	};

	return LOCALE[loc];
}

//----------------------------------------------------------------------------
/*! get current basar codepage, initialized by default (functions avoids 
    static init order fiasco)
    \return Codepage enum */
Locale::CodepageEnum & Locale::codepage()
{
	static Locale::CodepageEnum cp = Locale::CP_DEFAULT;	
	return cp;
}

//----------------------------------------------------------------------------
void Locale::setCodepage(const CodepageEnum cp)
{
	if ( CP_NUMBER <= cp )
	{
		VarString msg;
		msg.format("codepage %d not valid", cp);
		
		ExceptInfo sInfo("basar.cmnutil.Locale.setCodepage", msg, __FILE__, __LINE__);
		throw RuntimeException(sInfo);	
	}

	codepage() = cp;

	if (LoggerPool::locale().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("setCodepage(%d) set", cp);
		LoggerPool::locale().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//!	\return global codepage
Locale::CodepageEnum Locale::getCodepage()
{
	return codepage();
}

//----------------------------------------------------------------------------
//! \return codepage/charset enum; CP_INVALID if not found
Locale::CodepageEnum Locale::getCodepageByString(const VarString & cp)
{
    VarString key = cp;
    key.upper();

    CodepageEnum codepage;

    for (size_t i = 0; i < CP_NUMBER; ++i)
    {
        codepage = static_cast<CodepageEnum>(i);

        if (0 == ::strcmp(getCodepageString(codepage), key.c_str()))
            return codepage;
    }

    return CP_INVALID;
}

//----------------------------------------------------------------------------
//! \return codepage enum as string (basar specific): e.g: "WEST"
ConstBuffer Locale::getCodepageString(const CodepageEnum cp /* = getCodepage() */)
{
    // enum as strings
	static ConstString CODEPAGE[] =
	{
		"WEST",	   // e.g. de, en, fr
		"EAST",	   // also Central European; e.g. hr
		"CYRILLIC" // e.g. ru, bg
	};
	
	return CODEPAGE[cp];
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return codepage/charset enum
Locale::CodepageEnum Locale::getRegularCodepage(const LanguageEnum loc /* = getLocale() */)
{
	switch (loc)
	{
	case BULGARIAN_BG:
		return CP_CYRILLIC;

	case CROATIAN_HR:
	case BOSNIAN_BA:
	case SERBIAN_RS:
	case CZECH_CZ:
		return CP_EAST;

	case C:
	case ENGLISH_GB:
	case ENGLISH_US:
	case FRENCH_CH:
	case FRENCH_FR:
	case GERMAN_DE:
	case GERMAN_AT:
	case GERMAN_CH:
	default:
		return CP_WEST;
	}
}

//----------------------------------------------------------------------------
//! \return codepage/charset enum regarding Phoenix Informix DB locales
Locale::CodepageEnum Locale::getPhoenixCodepage(const LanguageEnum loc /* = getLocale() */)
{
	switch (loc)
	{
	case BULGARIAN_BG:
		return CP_CYRILLIC;

	case CROATIAN_HR:
	case BOSNIAN_BA:
	case SERBIAN_RS:
	case CZECH_CZ:
		// fall through: Informix DBs for countries above have DB locale en_us.8859-1 (WEST)!
	case C:
	case ENGLISH_GB:
	case ENGLISH_US:
	case FRENCH_CH:
	case FRENCH_FR:
	case GERMAN_DE:
	case GERMAN_AT:
	case GERMAN_CH:
	default:
		return CP_WEST;
	}
}

//----------------------------------------------------------------------------
//!	\return time format enum
Locale::TimeFormatEnum Locale::getTimeFormat(const LanguageEnum loc /* = getLocale() */)
{
	switch (loc)
	{
	case C:
	case ENGLISH_US:
		return FMT_TIME_AMERICAN;

	default:
		return FMT_TIME_EUROPEAN;
	}
}

//----------------------------------------------------------------------------
//! \return time format separator
ConstBuffer	Locale::getTimeSep (const LanguageEnum /* loc = getLocale() */)
{
/*
	switch (getTimeFormat(loc))
    {
	case FMT_TIME_AMERICAN:
    case FMT_TIME_EUROPEAN:
    default:
        return ":";
    }
*/
	return ":";
}

//----------------------------------------------------------------------------
//! \return fractional time format separator 
ConstBuffer	Locale::getTimeFracSep (const LanguageEnum loc /* = getLocale() */)
{
	return getDecSep(loc);
}

//----------------------------------------------------------------------------
//!	\return date format enum
Locale::DateFormatEnum Locale::getDateFormat(const LanguageEnum loc /* = getLocale() */)
{
	switch (loc)
	{
	case C:
	case ENGLISH_US:
		return FMT_DATE_AMERICAN;

	case ENGLISH_GB:
	case FRENCH_FR:
		return FMT_DATE_EUROPEAN_SLASH;

	default:
		return FMT_DATE_EUROPEAN;
	}
}

//----------------------------------------------------------------------------
//! \return date format separator 
ConstBuffer	Locale::getDateSep (const LanguageEnum loc /* = getLocale() */)
{
	switch ( getDateFormat(loc))
	{
	case FMT_DATE_AMERICAN:
	case FMT_DATE_EUROPEAN_SLASH:
		return "/";

	case FMT_DATE_EUROPEAN:
    default:
        return ".";
    }
}

//----------------------------------------------------------------------------
//! \return decimal format separator 
ConstBuffer Locale::getDecSep(const LanguageEnum loc /* = getLocale() */)
{
	switch (loc)
	{
		case C:
		case ENGLISH_GB:
		case ENGLISH_US:
		case GERMAN_CH:
		case FRENCH_CH:
			return ".";
						
		default:
			return ",";
	}    
}

//----------------------------------------------------------------------------
//! \return decimal format thousands separator 
//!
ConstBuffer	Locale::getDec1000thSep (const LanguageEnum loc /* = getLocale() */)
{
    switch (loc)
	{
	case C:
	case ENGLISH_GB:
	case ENGLISH_US:
		return ",";

	case GERMAN_CH:
	case FRENCH_CH:
		return "'";

	case FRENCH_FR:
	case BULGARIAN_BG:
	case CZECH_CZ:
		return " "; // not space, it's 160 - NBSP

	case CROATIAN_HR:
	case GERMAN_DE:
	case GERMAN_AT:
	case BOSNIAN_BA:
	case SERBIAN_RS:
	default:
		return ".";
	}
}

//----------------------------------------------------------------------------
}	//cmnutil
}	//basar

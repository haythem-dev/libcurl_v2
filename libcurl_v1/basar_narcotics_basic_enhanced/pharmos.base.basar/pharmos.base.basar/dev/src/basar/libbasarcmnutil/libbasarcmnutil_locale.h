//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for language dependencies
 *  \author M. Eichenlaub
 *  \date   27.02.2007
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_LOCALE_H
#define GUARD_LIBBASARCMNUTIL_LOCALE_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

namespace std
{
    class locale;
}

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! \brief language dependendant stuff
class Locale
{
public:

	//! language dependant locale for representing date/decimal/string
	enum LanguageEnum
	{
		LOCALE_FIRST =            0		,	//!< first valid locale

		C            = LOCALE_FIRST		,	//!< locale: C
		GERMAN_DE						,	//!< locale: German Germany
		GERMAN_AT						,	//!< locale: German Austria
		GERMAN_CH						,	//!< locale: German Switzerland
		ENGLISH_GB						,	//!< locale: English United Kingdom
		ENGLISH_US						,	//!< locale: English United Stated
		FRENCH_FR						,	//!< locale: French France
		FRENCH_CH						,	//!< locale: French Switzerland
		CROATIAN_HR						,	//!< locale: Croatian Croatia
		BULGARIAN_BG					,	//!< locale: Bulgarian Bulgaria
		BOSNIAN_BA						,	//!< locale: Bosnian Bosnia and Herzegovina - Latin
		SERBIAN_RS						,	//!< locale: Serbian Serbia - Latin
		CZECH_CZ						,	//!< locale: Czech Czech Republic

		LOCALE_NUMBER					,	//!< after last valid locale => number of locales
		LOCALE_INVALID = LOCALE_NUMBER	,	//!< invalid locale => for finding
		LOCALE_DEFAULT = GERMAN_DE			//!< default locale
	};

	//! time display format
	enum TimeFormatEnum
	{
		FMT_TIME_AMERICAN,	//!< with AM/PM
		FMT_TIME_EUROPEAN	//!< 24 hour
	};

	//! date display format
	enum DateFormatEnum
	{
		FMT_DATE_AMERICAN,			//!< MM/DD/YYYY
		FMT_DATE_EUROPEAN_SLASH,	//!< DD/MM/YYYY
		FMT_DATE_EUROPEAN			//!< DD.MM.YYYY
	};

	//! charset/codepage
	enum CodepageEnum
	{
		CP_FIRST = 0,			//!< first valid codepage

		CP_WEST = CP_FIRST,		//!< e.g. de, en, fr
		CP_EAST,				//!< also Central European; e.g. hr
		CP_CYRILLIC,			//!< e.g. ru, bg

		CP_NUMBER,				//!< number of codepages
		CP_INVALID = CP_NUMBER,	//!< invalid locale => for finding

		CP_DEFAULT = CP_WEST	//!< default codepage
	};

    //! initialize system settings (Informix, C locale) and check success
    //! \n error forces exit()
    //! \return bool indicating success
    static bool initSystemSettings ();

    // --- locale ----------

	//! set global basar locale. Default: German	\n RuntimeException
	LIBBASARCMNUTIL_API static void			setLocale		  (const LanguageEnum loc //!< language to set
															  );
	//! get global basar locale	
	LIBBASARCMNUTIL_API	static LanguageEnum getLocale		  ();
	//! get basar locale corresponding to case insensitive string
	LIBBASARCMNUTIL_API	static LanguageEnum	getLocaleByString (const VarString  & loc //!< basar specific case insensitive locale string: e.g "CRoatian_HR"
															  );
	//! get basar locale as string (basar specific): e.g "CROATIAN_HR"
	LIBBASARCMNUTIL_API	static ConstBuffer	getLocaleString   (const LanguageEnum loc = getLocale() //!< language to ask for
		                                                      );        
    // --- codepage --------

	//! set global codepage. Default: West	\n RuntimeException
	LIBBASARCMNUTIL_API static void			setCodepage			(const CodepageEnum cp //!< codepage to set
																);
	//! get global codepage/charset enum	
	LIBBASARCMNUTIL_API	static CodepageEnum	getCodepage			();
	//! get codepage/charset enum for given case insensitive codepage string
	LIBBASARCMNUTIL_API	static CodepageEnum	getCodepageByString (const VarString & cp //!< language to ask for
		                                                        );
	//! get codepage/charset enum as string: e.g. "WEST"
	LIBBASARCMNUTIL_API	static ConstBuffer  getCodepageString	(const CodepageEnum cp = getCodepage() //!< basar specific caseinsensitive codepage string: e.g. "West"  
		                                                        );
	//! get appropriate codepage/charset enum for given language			
	LIBBASARCMNUTIL_API	static CodepageEnum	getRegularCodepage	(const LanguageEnum loc = getLocale() //!< language to ask for
		                                                        );
	//! get codepage/charset enum for given language (Phoenix specific: Informix DB locale settings)
	LIBBASARCMNUTIL_API	static CodepageEnum getPhoenixCodepage	(const LanguageEnum loc = getLocale() //!< language to ask for
		                                                        );
    // --- time ------------

	//! get time format	enum
	LIBBASARCMNUTIL_API	static TimeFormatEnum getTimeFormat	 (const LanguageEnum loc = getLocale() //!< language to ask for
		                                                     );
	//! get time format separator for given basar locale	
	LIBBASARCMNUTIL_API static ConstBuffer	  getTimeSep	 (const LanguageEnum loc = getLocale() //!< language to ask for
															 );
	//! get fractional time format separator for given basar locale	
	LIBBASARCMNUTIL_API static ConstBuffer	  getTimeFracSep (const LanguageEnum loc = getLocale() //!< language to ask for
															 );
    // --- date ------------

    //! get date format	enum
	LIBBASARCMNUTIL_API	static DateFormatEnum getDateFormat	(const LanguageEnum loc = getLocale() //!< language to ask for
		                                                    );
	//! get date format separator for given basar locale	
	LIBBASARCMNUTIL_API static ConstBuffer	  getDateSep	(const LanguageEnum loc = getLocale() //!< language to ask for
															);
    // --- decimal ---------

	//! get decimal format separator for given basar locale
	LIBBASARCMNUTIL_API static ConstBuffer getDecSep	   (const LanguageEnum loc = getLocale() //!< language to ask for
														   );
	//! get decimal format thousands separator for given basar locale
	LIBBASARCMNUTIL_API static ConstBuffer getDec1000thSep (const LanguageEnum loc = getLocale() //!< language to ask for
														   );

private:
			 Locale		();                     //!< forbidden
			 Locale		(const Locale & rCopy);	//!< forbidden
	Locale & operator =	(const Locale & rCopy);	//!< forbidden

    //! get current basar locale reference
    static LanguageEnum & locale();
    //! get current basar codepage reference
    static CodepageEnum & codepage();
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

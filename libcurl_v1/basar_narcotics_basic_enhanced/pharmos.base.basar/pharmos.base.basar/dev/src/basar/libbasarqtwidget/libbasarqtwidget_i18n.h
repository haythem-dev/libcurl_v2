//----------------------------------------------------------------------------
/*! \file
 *  \brief  i18n functions regarding Qt
 *  \author Michael Eichenlaub
 *  \date   24.06.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_I18N_H
#define GUARD_LIBBASARQTWIDGET_I18N_H

//------------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARQTWIDGET_API
	#ifdef _WIN32
		#ifdef LIBBASARQTWIDGET_EXPORTS
			#define LIBBASARQTWIDGET_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARQTWIDGET_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARQTWIDGET_API
	#endif
#endif

//------------------------------------------------------------------------------
#include "libbasarcmnutil_locale.h"

//------------------------------------------------------------------------------
class QLocale;

//------------------------------------------------------------------------------
namespace basar {
namespace gui   {
namespace widget{

//----------------------------------------------------------------------------
//! \brief settings for Qt codec, Qt locale, basar locale
//! \n final class
class I18n
{
public:
	//! set Qt codec, Qt locale, basar locale					\n no-throw
	LIBBASARQTWIDGET_API static void		setI18n				(basar::cmnutil::Locale::LanguageEnum eLocale	//!< language dependant locale
																);

	//! set Qt codec, Qt locale, basar locale					\n no-throw
	LIBBASARQTWIDGET_API static void		setI18n				(basar::cmnutil::Locale::LanguageEnum eLocale,	//!< language dependant locale
																 basar::cmnutil::Locale::CodepageEnum eCodepage	//!< differing codepage regarding locale 
																);

	//! get Qt codec name for given basar codepage/charset		\n no-throw
	LIBBASARQTWIDGET_API static ConstString getQtCodecName		(basar::cmnutil::Locale::CodepageEnum eCodepage	//!< language dependant codepage
																);

	//! get Qt locale for given basar locale					\n no-throw
	LIBBASARQTWIDGET_API static QLocale		getQtLocale			(basar::cmnutil::Locale::LanguageEnum eLocale	//!< language dependant locale
																);

	//! get Phoenix display format string regarding QDateTimeEdit (or derived class) for given basar locale		\n no-throw
	LIBBASARQTWIDGET_API static ConstString	getPhDateTimeFormat	(basar::cmnutil::Locale::LanguageEnum eLocale =
																	basar::cmnutil::Locale::getLocale()		//!< language dependant locale
																);
	//! get Phoenix display format string regarding QDateEdit (or derived class) for given basar locale		\n no-throw
	LIBBASARQTWIDGET_API static ConstString	getPhDateFormat		(basar::cmnutil::Locale::LanguageEnum eLocale =
																	basar::cmnutil::Locale::getLocale()		//!< language dependant locale
																);
	//! get Phoenix display format string regarding QTimeEdit (or derived class) for given basar locale		\n no-throw
	LIBBASARQTWIDGET_API static ConstString	getPhTimeFormat		(basar::cmnutil::Locale::LanguageEnum eLocale = 
																	basar::cmnutil::Locale::getLocale()		//!< language dependant locale
																);
};

//------------------------------------------------------------------------------
}	// namespace widget
}	// namespace gui
}	// namespace basar

//------------------------------------------------------------------------------
#endif //GUARD

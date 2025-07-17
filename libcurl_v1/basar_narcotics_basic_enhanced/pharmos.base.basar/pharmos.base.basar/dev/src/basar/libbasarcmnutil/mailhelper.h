//----------------------------------------------------------------------------
/*! \file
 *  \brief  helper class for mail
 *  \author Marco Köppendörfer
 *  \date   05.12.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_MAILHELPER_H
#define GUARD_LIBBASARCMNUTIL_MAILHELPER_H


//----------------------------------------------------------------------------
// include section
//----------------------------------------------------------------------------
#include "libbasar_definitions.h" //for ConstBuffer
#include "libbasarcmnutil_locale.h"

//----------------------------------------------------------------------------
// class declaration section
//----------------------------------------------------------------------------
namespace basar
{
	namespace cmnutil
	{
		class MailHelper
		{
			public:
				// get charset depending on locale						\n no-throw
				static ConstBuffer				getCharset		(Locale::LanguageEnum eLanguage);
			private:
				MailHelper();										//!< forbidden - this class is intended to be used statically
				MailHelper(const MailHelper&);						//!< forbidden
				MailHelper& operator=(const MailHelper&);			//!< forbidden
				
		};
	}
}

#endif // GUARD_LIBBASARCMNUTIL_MAILHELPER_H
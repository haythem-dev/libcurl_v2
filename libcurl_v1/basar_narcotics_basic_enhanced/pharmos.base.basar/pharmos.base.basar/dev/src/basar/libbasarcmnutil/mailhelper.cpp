//----------------------------------------------------------------------------
/*! \file
 *  \brief  class definition for mail helper class
 *  \author Marco Köppendörfer
 *  \date   05.12.2013
 */
//----------------------------------------------------------------------------

#include "mailhelper.h"

namespace basar
{
	namespace cmnutil
	{
		//------------------------------------------------------------------------------
		//! \throw no-throw 
		//! \param eLanguage value of lanugage in use
		//! \return ConstBuffer containing ISO character set name
		ConstBuffer MailHelper::getCharset(Locale::LanguageEnum	eLanguage)
		{
			static ConstString ISOCODE_2  = "ISO-8859-2";
			static ConstString ISOCODE_5  = "ISO-8859-5";
			static ConstString ISOCODE_15 = "ISO-8859-15";

			switch (eLanguage)
			{
			case Locale::CROATIAN_HR:
				return ISOCODE_2;
				
			case Locale::BULGARIAN_BG:
				return ISOCODE_5;
				
			default:
				return ISOCODE_15;
			}
		}		
	}
}
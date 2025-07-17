//----------------------------------------------------------------------------
/*! \file
 *  \brief  Informix locale environment     
 *  \author Michael Eichenlaub
 *  \date   23.03.2009
 */
//----------------------------------------------------------------------------

#include "localeenvinfx.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasardbsql_functions.h"

//------------------------------------------------------------------------------
namespace basar	  {

//------------------------------------------------------------------------------
namespace cmnutil {

//----------------------------------------------------------------------------
using db::sql::LocaleEnvInfxImpl;

/*!	specialised method */
//! \return shared_ptr to LocaleEnvInfxImpl instance
template <>
typename Singleton<LocaleEnvInfxImpl>::InstancePtr & Singleton<LocaleEnvInfxImpl>::inst()
{
    // avoid init order fiasco
    db::sql::init();

    static InstancePtr s_Inst; 
    return s_Inst;
}

//----------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! Informix locale constants for environment:  sysmaster.sysdbslocale.dbs_collate, CLIENT_LOCALE, DB_LOCALE
static ConstString LOCALE_ENV_INFX[][3] =
{
	//   Use ISO rather than Windows CP encoding
	//   Reason is the DB server's capability for storing only ISO encoded characters.
	//   There is no overhead for converting from ISO to Windows CP and vice versa.
	//   Windows CP encoding with no corresponding ISO character leads to 
	//   Informix error "-202 An illegal character has been found in the statement".

	//   use CLIENT_LOCALE "ISO 8859-1" instead of "CP 1252" on Windows side:
	//   reason is the DB server's capability for storing only ISO encoded characters.
	//   There is no overhead for converting from ISO to Windows CP and vice versa.
	//   Windows CP encoding with no corresponding ISO character leads to 
	//   Informix error "-202 An illegal character has been found in the statement".
	//   e.g.: EUR sign 0x80 (CP 1252) can not be converted to ISO and leads to 
	//   Informix error -202. With "ISO 8859-1" EUR sign 0x80 (CP 1252) is stored 
	//   as 0x80 in DB (without conversion); but it can't displayed by Unix application.

	{ "de_DE.819", "de_de.8859-1", "de_de.8859-1" },
	{ "de_AT.819", "de_at.8859-1", "de_ch.8859-1" },
	{ "de_CH.819", "de_ch.8859-1", "de_ch.8859-1" },
	{ "en_GB.819", "en_gb.8859-1", "en_gb.8859-1" },
	{ "en_US.819", "en_us.8859-1", "en_us.8859-1" },
	{ "fr_FR.819", "fr_fr.8859-1", "fr_fr.8859-1" },
	{ "fr_CH.819", "fr_ch.8859-1", "fr_ch.8859-1" },

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	{ "bg_BG.915", "bg_bg.1251"  , "bg_bg.8859-5" },
	{ "hr_HR.912", "hr_hr.1250"  , "hr_hr.8859-2" },
	{ "cs_cz.912", "cs_cz.1250"  , "cs_cz.8859-2" }
#else
	{ "bg_BG.915", "bg_bg.8859-5", "bg_bg.8859-5" },
	{ "hr_HR.912", "hr_hr.8859-2", "hr_hr.8859-2" },
	{ "cs_cz.912", "cs_cz.8859-2", "cs_cz.8859-2" }
#endif
};

//------------------------------------------------------------------------------
//! \throw no-throw
//!
LocaleEnvInfxImpl::LocaleEnvInfxImpl()
{
	for (size_t i = 0; i < sizeof(LOCALE_ENV_INFX) / sizeof(LOCALE_ENV_INFX[0]); ++i)
	{
		EnvInfxLoc env( LOCALE_ENV_INFX[i][1],						// CLIENT_LOCALE
				        LOCALE_ENV_INFX[i][2] );					// DB_LOCALE));

		CollLocEnvInfx::value_type val (LOCALE_ENV_INFX[i][0],	// sysmaster.sysdbslocale.dbs_collate
			                            env);

		m_CollLocEnvInfx.insert(val);
	}
}

//------------------------------------------------------------------------------
//! \throw OutOfRangeIndexException
//! \return value pairs for CLIENT_LOCALE, DB_LOCALE for given sysmaster.sysdbslocale.dbs_collate
const EnvInfxLoc & LocaleEnvInfxImpl::getLocaleEnvs(const VarString & dbsCollate) const
{
	CollLocEnvInfx::const_iterator it = m_CollLocEnvInfx.find(dbsCollate);

	if (m_CollLocEnvInfx.end() == it)	
	{
		VarString msg;
		msg.format("sysmaster.sysdbslocale.dbs_collate <%s> not found in LOCALE_ENV_INFX/m_CollLocEnvInfx", 
			       dbsCollate.c_str());
		throw OutOfRangeIndexException ( ExceptInfo ( "basar.db.sql.LocaleEnvInfxImpl.getLocaleEnv", 
			                                          msg, 
													  __FILE__, __LINE__));
	}

	return it->second;
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

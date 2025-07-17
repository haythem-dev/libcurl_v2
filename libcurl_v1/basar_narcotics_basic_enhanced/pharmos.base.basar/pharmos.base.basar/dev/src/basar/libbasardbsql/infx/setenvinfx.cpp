//----------------------------------------------------------------------------
/*! \file
 *  \brief  Informix environment setting and restoring
 *  \author Michael Eichenlaub
 *  \date   25.03.2009
 */
//----------------------------------------------------------------------------

#include "setenvinfx.h"

#include "libbasarcmnutil_logging.h"
#include "libbasardbsql_databaseinfo.h"
#include "libbasardbsql_exceptions.h"
#include "loggerpool.h"
#include "dblocaleinfx.h"
#include "localeenvinfx.h"

#include <esqlc.h>
#include <string.h>

//------------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
// avoid static init order fiasco using functions
static ConstBuffer CLIENT_LOCALE      () { return "CLIENT_LOCALE";	 } //!< Informix environment variable 
static ConstBuffer DB_LOCALE          () { return "DB_LOCALE";		 } //!< Informix environment variable 
static ConstBuffer CONNECTION_TIMEOUT () { return "INFORMIXCONTIME"; } //!< Informix environment variable 
static ConstBuffer CONNECTION_RETRIES () { return "INFORMIXCONRETRY";} //!< Informix environment variable 

#define ENV_BUFSIZE     128 //!< buffer size for environment variable assignment

//------------------------------------------------------------------------------
//! \throw no-throw
//!
SetEnvInfx::SetEnvInfx()
{
	backup();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
SetEnvInfx::~SetEnvInfx()
{
	restore();

    BLOG_DEBUG_STREAM(LoggerPool::setEnvInfx(), 
        "~SetEnvInfx(): restored previously set environment values");
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SetEnvInfx::adjustEnv(const DatabaseInfo & dbinfo)
{
    // search in cached data for sysmaster.sysdbslocale.dbs_collate
	VarString dbLocale = DbLocaleInfx::getInstance().getLocale(dbinfo);

    BLOG_DEBUG_STREAM(LoggerPool::setEnvInfx(),
        "adjustLocale(): found locale <" << dbLocale << "> "
        << "for DB " << dbinfo.database << "@" << dbinfo.dbServer);

	// get appropriate values for given DB locale
	EnvInfxLoc loc = LocaleEnvInfx::getInstance().getLocaleEnvs(dbLocale);

	setEnvLocales(loc.localeClient, loc.localeDB);

    setEnvTimeout(dbinfo.conntime.timeout.c_str(), 
                  dbinfo.conntime.retry  .c_str());
}

//------------------------------------------------------------------------------
void SetEnvInfx::setEnvLocales(ConstString client,
                               ConstString db    )
{
    // "static" needed for Unix putenv() !
	static char varClient[ENV_BUFSIZE];
	static char varDb    [ENV_BUFSIZE];

    setEnviron(varClient, CLIENT_LOCALE(), client);
    setEnviron(varDb    , DB_LOCALE    (), db    );
}

//------------------------------------------------------------------------------
void SetEnvInfx::setEnvTimeout(ConstString timeout,
                               ConstString retries)
{
    // "static" needed for Unix putenv() !
	static char varTimeout[ENV_BUFSIZE];
	static char varRetries[ENV_BUFSIZE];

    setEnviron(varTimeout, CONNECTION_TIMEOUT(), timeout);
    setEnviron(varRetries, CONNECTION_RETRIES(), retries);
}

//------------------------------------------------------------------------------
//! \throw DbExceptionInfx
//!
void SetEnvInfx::setEnviron(char * const assign,
                            ConstString  var,
                            ConstString  val)
{
    assert(0 != val);
    assert(0 != infxGetEnv(var));

    if (0 == strcmp(val, infxGetEnv(var)))
    {
        //nothing to do
        return;
    }

	::sprintf(assign, "%s=%s", var, val);
    assert(strlen(assign) < ENV_BUFSIZE);

    if (RETCODE_OK != infxPutEnv(assign))
	{
		VarString msg;
		msg.format("putenv(%s) for retries failed", assign);

		throw DbExceptionInfx(ExceptInfo(
            "basar.db.sql.SetEnvInfx.setEnviron", 
            msg, 
            __FILE__, __LINE__));
	}

    BLOG_DEBUG_STREAM(LoggerPool::setEnvInfx(),
        "setEnviron(): set environment value "
        << "(" << assign << ")");
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SetEnvInfx::clear()
{
	setEnvLocales("", "");
	setEnvTimeout("", "");
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SetEnvInfx::backup()
{
    m_prevClient  = infxGetEnv(CLIENT_LOCALE     ());
    m_prevDb      = infxGetEnv(DB_LOCALE         ());

    m_prevTimeout = infxGetEnv(CONNECTION_TIMEOUT());
    m_prevRetry   = infxGetEnv(CONNECTION_RETRIES());
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SetEnvInfx::restore()
{
    setEnvLocales(m_prevClient .c_str(), m_prevDb   .c_str());
    setEnvTimeout(m_prevTimeout.c_str(), m_prevRetry.c_str());
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

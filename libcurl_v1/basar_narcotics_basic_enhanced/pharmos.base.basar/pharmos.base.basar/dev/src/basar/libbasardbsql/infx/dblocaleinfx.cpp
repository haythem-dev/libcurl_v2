//----------------------------------------------------------------------------
/*! \file
 *  \brief  container with collation values for particular Informix DBs     
 *  \author Michael Eichenlaub
 *  \date   23.03.2009
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_resultsetref.h"
#include "dblocaleinfx.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasardbsql_exceptions.h"
#include "loggerpool.h"
#include "libbasardbsql_functions.h"
#include "libbasardbsql_connectionref.h"
#include "libbasardbsql_manager.h"
#include "libbasardbsql_statementref.h"
#include "libbasardbsql_databaseinfo.h"

//------------------------------------------------------------------------------
namespace basar	{

//------------------------------------------------------------------------------
namespace cmnutil {

//----------------------------------------------------------------------------
using db::sql::DbLocaleInfxImpl;

/*!	specialised method */
//! \return shared_ptr to DbLocaleInfxImpl instance
template <>
typename Singleton<DbLocaleInfxImpl>::InstancePtr & Singleton<DbLocaleInfxImpl>::inst()
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
static ConstString DB_SYSMASTER     = "sysmaster";	//!< Informix meta database name for retrieving db locales
static ConstString LOCALE_SYSMASTER = "en_US.819";	//!< locale for Informix meta database sysmaster

//------------------------------------------------------------------------------
//! \throw no-throw
//!
DbLocaleInfxImpl::DbLocaleInfxImpl()
{
}

//------------------------------------------------------------------------------
//! \throw DbExceptionInfx
//! \return locale (e.g. "en_us.819") for given db\@server
//!
//! to avoid endless recursion by calling ConnectionInfx instance to retrieve sysmaster 
//! database (if a SQL select must be executed), a hardcoded locale "en_us.819" is returned 
//! when a connection to database "sysmaster" is tried.
VarString DbLocaleInfxImpl::getLocale (const DatabaseInfo & dbinfo)
{
	// --- to avoid endless recursion
	{
		VarString db = dbinfo.database;
		db.lower();

		if (DB_SYSMASTER == db)	
		{
			if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("getLocale(): chosen DB is %s@%s => returning %s", 
					       db.c_str(), 
						   dbinfo.dbServer.c_str(),
						   LOCALE_SYSMASTER);
				LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}

			return LOCALE_SYSMASTER;
		}
	}


	// --- search in container
	DBKeyInfx searchkey(dbinfo.dbServer, dbinfo.database);

	CollLocaleDBInfx::const_iterator it = m_CollLocaleDBInfx.find(searchkey);

	// cache hit
	if (it != m_CollLocaleDBInfx.end())
	{
		if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("getLocale(): found locale %s for chosen DB %s@%s in cache", 
				        it->second     .c_str(),
						dbinfo.database.c_str(), 
						dbinfo.dbServer.c_str());
			LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		return it->second;
	}


	// cache miss -> (re)load sysmaster.sysdbslocale for given DB server
	if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("getLocale(): 1st search for DB %s@%s yields cache miss => loadLocale(%s)", 
					dbinfo.database.c_str(), 
					dbinfo.dbServer.c_str(),
					dbinfo.dbServer.c_str());
		LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	loadLocale(dbinfo);	


	// 2nd try for searching
	if ( (it = m_CollLocaleDBInfx.find(searchkey)) ==  m_CollLocaleDBInfx.end() )
	{
		VarString msg;
		msg.format("getLocale(): 2nd search for DB %s@%s yields cache miss again", 
					dbinfo.database.c_str(),
					dbinfo.dbServer.c_str()); 
		throw DbExceptionInfx(ExceptInfo("basar.db.sql.DbLocaleInfxImpl.getLocale", msg, __FILE__, __LINE__));
	}

	if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("getLocale(): found locale %s for chosen DB %s@%s in reloaded cache", 
				    it->second     .c_str(),
					dbinfo.database.c_str(), 
					dbinfo.dbServer.c_str());
		LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return it->second;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void DbLocaleInfxImpl::loadLocale (const DatabaseInfo & dbinfo)
{
	DatabaseInfo dbmeta = dbinfo;

	dbmeta.database           = DB_SYSMASTER;
	dbmeta.concurrTransaction = false;

	assert(dbmeta.dbServer     == dbinfo.dbServer);
	assert(dbmeta.eConnectType == INFORMIX       );


	ConstString sql = "SELECT dbs_dbsname, dbs_collate "
		              "FROM   sysdbslocale "
					  "WHERE  LOWER(dbs_dbsname) NOT LIKE 'sys%';";

	ConnectionRef conn   = Manager::getInstance().createConnect(dbmeta);
	conn.setIsolationLevel(basar::INFX_DIRTYREAD);

	StatementRef  stmt   = conn.createStatement();
	ResultsetRef  result = stmt.executeQuery(sql);

	std::pair < CollLocaleDBInfx::iterator, bool > inserted;		// return value for map::insert()

	while (result.next())
	{
		DBKeyInfx                    key(dbinfo.dbServer, result.getString(0));		// dbs_dbsname
		CollLocaleDBInfx::value_type val(key            , result.getString(1));		// dbs_collate

		inserted = m_CollLocaleDBInfx.insert(val);

		if ( inserted.second )		// correctly inserted
		{
			if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("loadLocale(): inserted successfully key(%s,%s), value <%s>", 
							dbinfo.dbServer.c_str(),
							result.getString(0),
							result.getString(1));
				LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
		}
		else						// duplicate key => database@dbserver possibly updated for locale => update cache
		{
			if (LoggerPool::dbLocaleInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("loadLocale(): insert found already located  key(%s,%s): old value <%s> is updated to <%s>", 
							dbinfo.dbServer.c_str(),
							result.getString(0),
							inserted.first->second.c_str(),
							result.getString(1));
				LoggerPool::dbLocaleInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}

			inserted.first->second = result.getString(1);
		}
	}
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

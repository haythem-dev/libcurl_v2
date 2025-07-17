//----------------------------------------------------------------------------
/*! \file
 *  \brief  container with locale values for particular Informix DBs     
 *  \author Michael Eichenlaub
 *  \date   23.03.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_DBLOCALEINFX_H
#define GUARD_DBLOCALEINFX_H

//----------------------------------------------------------------------------
#include <map>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_pattern.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
class DatabaseInfo;

//----------------------------------------------------------------------------
//! \brief comparable key for Informix database\@server; keys are always lower case!
class DBKeyInfx
{
public:
	//! c'tor	\n no-throw
					DBKeyInfx		(const VarString & dbserver,	//!< database server: e.g. "zdev21_tcp"
									 const VarString & database		//!< database: e.g. "ode21"
									)
									: m_DbServer(dbserver),
									  m_Database(database)
					{
						m_DbServer.lower();
						m_Database.lower();
					};

	//! less operator; needed by map containing key and value	\n no-throw
	//! \return true if this is less than compared key, false otherwise
	bool			operator <		(const DBKeyInfx & rhs			//!< compared key
									) const
					{
						if (m_DbServer < rhs.m_DbServer)
							return true;
						else
						{
							if (m_DbServer == rhs.m_DbServer)
							{
								if (m_Database < rhs.m_Database)
									return true;
							}
						}

						return false;
					};

private:
					DBKeyInfx		();						// forbidden
	DBKeyInfx	  & operator =		(const DBKeyInfx &);	// forbidden

	VarString		m_DbServer;				//!< database server: e.g. "zdev21_tcp"
	VarString		m_Database;				//!< database: e.g. "ode21"
};


//----------------------------------------------------------------------------
//! \brief contains Informix locale data from sysmaster.sysdbslocale.dbs_collate 
//!        for particular databases in a map. If data for a given DB are not found, 
//!        a new SQL select for sysmaster.sysdbslocale\@\<given DB server\> is executed.
class DbLocaleInfxImpl
{
public:
	//! default c'tor	\n no-throw
						DbLocaleInfxImpl	();					
						
	//! get locale (e.g. "en_us.819") for given db\@server	\n throw DbExceptionInfx
	VarString			getLocale			(const DatabaseInfo & dbinfo	//!< database specific connection data
											);
										
private:										
						DbLocaleInfxImpl	(const DbLocaleInfxImpl &);	//!< forbidden
	DbLocaleInfxImpl &	operator =			(const DbLocaleInfxImpl &);	//!< forbidden

	//! load given DB server's locales (DB specific) by executing SQL select from sysmaster.sysdbslocale	\n no-throw
	void				loadLocale			(const DatabaseInfo & dbinfo	//!< database specific connection data
											);
	
	//! definition for mapping Informix db\@server to db's locale
	typedef std::map < DBKeyInfx, VarString > CollLocaleDBInfx;

	CollLocaleDBInfx	m_CollLocaleDBInfx;		//!< collection for mapping Informix db\@server to db's locale
};

//------------------------------------------------------------------------------
//! \brief singleton wrapper for DbLocaleInfxImpl \n final class
class DbLocaleInfx : public cmnutil::Singleton<DbLocaleInfxImpl>
{
private:
						DbLocaleInfx	();						//!< forbidden
						DbLocaleInfx	(const DbLocaleInfx &);	//!< forbidden
	DbLocaleInfx &		operator =		(const DbLocaleInfx &);	//!< forbidden
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

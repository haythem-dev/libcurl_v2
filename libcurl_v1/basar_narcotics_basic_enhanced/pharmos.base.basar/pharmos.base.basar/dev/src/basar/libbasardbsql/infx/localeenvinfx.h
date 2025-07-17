//----------------------------------------------------------------------------
/*! \file
 *  \brief  Informix locale environment     
 *  \author Michael Eichenlaub
 *  \date   23.03.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOCALEENVINFX_H
#define GUARD_LOCALEENVINFX_H

//----------------------------------------------------------------------------
#include <map>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_pattern.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief holds Informix environment variables CLIENT_LOCALE, DB_LOCALE
class EnvInfxLoc
{
public:
	//! c'tor
				EnvInfxLoc ( ConstString client, //!< CLIENT_LOCALE
						     ConstString db      //!< DB_LOCALE
						   ) 
						: localeClient(client),
						  localeDB    (db    )
				{};

	ConstString localeClient;					//!< CLIENT_LOCALE
	ConstString localeDB;						//!< DB_LOCALE

private:
	//!< forbidden assigment operator
	EnvInfxLoc & operator = (const EnvInfxLoc &);		
};

//------------------------------------------------------------------------------
//! \brief holds data for mapping between sysmaster.sysdbslocale.dbs_collate 
//!        and corresponding Informix locale environment variables; 
//!        provides lookup for given dbs_collate to setup appropriate connection environment
class LocaleEnvInfxImpl
{
public:
	//! \brief default c'tor
						LocaleEnvInfxImpl	();

	//! get Informix locale environment value pairs \n throw OutOfRangeIndexException
	const EnvInfxLoc &	getLocaleEnvs		(const VarString & dbsCollate	//!< sysmaster.sysdbslocale.dbs_collate
											) const;

private:
						LocaleEnvInfxImpl	(const LocaleEnvInfxImpl &);	// forbidden
	LocaleEnvInfxImpl & operator =			(const LocaleEnvInfxImpl &);	// forbidden

	//! definition for mapping DB environment information: sysmaster.sysdbslocale.dbs_collate <=> CLIENT_LOCALE, DB_LOCALE
	typedef std::map < VarString, EnvInfxLoc > CollLocEnvInfx;

	CollLocEnvInfx		m_CollLocEnvInfx;		//!< collection for mapping DB environment information
};

//------------------------------------------------------------------------------
//! \brief singleton wrapper for LocaleEnvInfxImpl \n final class
class LocaleEnvInfx : public cmnutil::Singleton<LocaleEnvInfxImpl>
{
private:
						LocaleEnvInfx   ();							//!< forbidden
						LocaleEnvInfx   (const LocaleEnvInfx &);	//!< forbidden
	LocaleEnvInfx &		operator =		(const LocaleEnvInfx &);	//!< forbidden
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

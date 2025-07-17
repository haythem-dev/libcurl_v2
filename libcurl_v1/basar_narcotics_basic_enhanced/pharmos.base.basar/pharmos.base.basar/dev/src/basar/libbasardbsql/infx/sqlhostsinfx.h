//----------------------------------------------------------------------------
/*! \file
 *  \brief  retrieving Informix sqlhosts parameter
 *  \author Michael Eichenlaub
 *  \date   05.08.2010
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLHOSTSINFX_H
#define GUARD_SQLHOSTSINFX_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! Hold Informix sqlhosts parameters. Dependent on platform, registry (Windows)
//! or file $INFORMIXDIR/etc/sqlhosts (can also be specified by $INFORMIXSQLHOSTS)
//! is queried.
class SqlHostsInfx
{
public:
	//! c'tor: evaluate members by given database server \n no-throw
					SqlHostsInfx	(const VarString &  dbserver	//!< database server (e.g.: zdev21_tcp)
									);

	//! get host running database server \n no-throw
	VarString		getHost			() const;

	//! get protocol network parameter \n no-throw
	VarString		getProtocol		() const;
	
	//! get service network parameter: could be service name or port number \n no-throw
	VarString		getService		() const;

private:
					SqlHostsInfx	();						// forbidden
					SqlHostsInfx	(const SqlHostsInfx &);	// forbidden
	SqlHostsInfx &	operator =		(const SqlHostsInfx &);	// forbidden

	//! \brief evaluate members by given database server dependent on platform 
	//! \n     throw RuntimeException, InvalidStateException
	void			queryParams		();

	VarString	m_DbServer;		//!< database server (e.g.: zdev21_tcp)
	VarString   m_Host;			//!< host running database server
	VarString	m_Protocol;		//!< protocol network parameter
	VarString	m_Service;		//!< service network parameter: service name or port number
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

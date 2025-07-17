//----------------------------------------------------------------------------
/*! \file
 *  \brief  database parameter 
 *  \author Michael Eichenlaub
 *  \date   04.08.2010
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_databaseinfo.h"

#include "libbasarcmnutil_socket.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw no-throw
//!
DatabaseInfo::DatabaseInfo() : eConnectType      (INFORMIX ),
							   connectionId      (-2),		// no slot (tcapi.h)
							   concurrTransaction(false)
{
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void DatabaseInfo::setInfx(const VarString & dbs               ,
						   const VarString & db                ,	
						   const VarString & usr     /* = "" */,
						   const VarString & pwd     /* = "" */)
{
	eConnectType       = INFORMIX;
	concurrTransaction = false;
	dbServer           = dbs;
	database           = db;
	user               = usr;
	passwd             = pwd;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void DatabaseInfo::setInfxConcurrent(const VarString & dbs               ,
									 const VarString & db                ,		
									 const VarString & usr     /* = "" */,
						             const VarString & pwd     /* = "" */)
{
	eConnectType       = INFORMIX;
	concurrTransaction = true;
	dbServer           = dbs;
	database           = db;
	user               = usr;
	passwd             = pwd;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void DatabaseInfo::setInfxTimeout(const VarString & timeout,
                                  const VarString & retry)
{
	conntime.timeout = timeout;
    conntime.retry   = retry;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void DatabaseInfo::setODBC(const VarString & dataSourceName,
						   const VarString & usr /* = "" */,
						   const VarString & pwd /* = "" */)
{
	eConnectType   = ODBC;
	database       = dataSourceName;
	user           = usr;
	passwd         = pwd;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void DatabaseInfo::setCICS(const VarString & configName    ,
						   const VarString & ipAddr        ,
						   const VarString & port          ,		
						   const VarString & startTransact ,
						   const VarString & usr /* = "" */,
						   const VarString & pwd /* = "" */)
{
	eConnectType             = CICS;
	dbServer                 = configName;
	cicsCustom.ipAddr        = ipAddr;
	cicsCustom.port          = port;
	cicsCustom.startTransact = startTransact;
	user                     = usr;
	passwd                   = pwd;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing CICS name for opened session after successful connect
VarString DatabaseInfo::getCICSName() const
{
	return database;
}

//----------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return bool indicating if Cics is available
bool DatabaseInfo::isCicsAvail(Int32 timeout)
{
	if (CICS != eConnectType)
	{
		VarString msg;
		msg.format("query for CICS availability with wrong connection type %d",
			       eConnectType);
		throw InvalidParameterException(ExceptInfo("basar.db.sql.DatabaseInfo.isCicsAvail",
											       msg,
											       __FILE__, __LINE__));
	}

	cmnutil::TcpSocket sock(cicsCustom.ipAddr, static_cast<Int16>(cicsCustom.port.stoi()));

	return sock.isAvail(timeout);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar


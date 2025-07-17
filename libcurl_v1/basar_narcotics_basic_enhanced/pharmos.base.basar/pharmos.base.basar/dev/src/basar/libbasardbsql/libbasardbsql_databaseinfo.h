//------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface: database parameters
 *  \author Michael Eichenlaub
 *  \date   04.08.2010
 */
//------------------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBASARDBSQL_DATABASEINFO_H
#define GUARD_LIBBASARDBSQL_DATABASEINFO_H

//------------------------------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasardbsql_api.h"

//------------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace db	{
namespace sql
{

//------------------------------------------------------------------------------------------------------------//
//! \brief database specific connection data \n
//!        for typical settings use appropriate set method
//------------------------------------------------------------------------------------------------------------//
class DatabaseInfo
{
public:
	//! default c'tor
	LIBBASARDBSQL_API
				DatabaseInfo	 ();	

	//! set typical Informix connection parameters
	LIBBASARDBSQL_API
	void		setInfx			 (const VarString & dbs,    		//!< database server (e.g. "zdev21_tcp")
								  const VarString & db,				//!< database (e.g. "ode21")
								  const VarString & usr     = "",	//!< database user 
								  const VarString & pwd     = ""	//!< database user's password
								 );

	//! set Informix connection parameters for concurrent transaction
	LIBBASARDBSQL_API
	void		setInfxConcurrent(const VarString & dbs,     		//!< database server (e.g. "zdev21_tcp")
								  const VarString & db,				//!< database (e.g. "ode21")
								  const VarString & usr     = "",	//!< database user 
								  const VarString & pwd     = ""	//!< database user's password
								 );

	//! set Informix connection timeout. Only for connections in main/primary thread!
	LIBBASARDBSQL_API
	void		setInfxTimeout	 (const VarString & timeout,    //!< timeout in seconds
								  const VarString & retry	    //!< additional retries
								 );

    //! set ODBC connection parameters
	LIBBASARDBSQL_API
	void		setODBC			 (const VarString & dataSourceName,	//!< data source name (e.g. "zdev21_tcp")
								  const VarString & usr = "",		//!< database user 
								  const VarString & pwd = ""		//!< database user's password 
								 );

	//! set recommended CICS connection parameters
	LIBBASARDBSQL_API
	void		setCICS			 (const VarString & configName,		//!< configuration (e.g. "PhoenixGeneric_west")
								  const VarString & ipAddr,			//!< CICS host ip address
								  const VarString & port,			//!< CICS host port
								  const VarString & startTransact,	//!< CICS start transaction
								  const VarString & usr = "",		//!< CICS user 
								  const VarString & pwd = ""		//!< CICS user's password 
								 );

	//! get CICS name for opened session after successful connect (e.g. "A02CICS2")
	LIBBASARDBSQL_API
	VarString	getCICSName		 () const;

	//! is CICS available? \n throw InvalidParameterException
	LIBBASARDBSQL_API
	bool		isCicsAvail		 (Int32 timeout		//!< timeout in seconds
								 );

	//! custom settings for generic CICS configuration
	struct CicsCustom
	{
		VarString ipAddr;			//!< CICS host ip address
		VarString port;				//!< CICS host port
		VarString startTransact;    //!< CICS host start transaction (e.g. "TCAS")
	};

    //! Informix connection timeout settings
    struct InfxConnTime
    {
        VarString timeout;  //!< seconds the CONNECT statement continues before returning an error
        VarString retry;    //!< maximum number of additional connection attempts
    };

	VarString          dbServer;			//!< \brief
											//!< Informix: database server (e.g. "zdev21_tcp") \n
	                                        //!< ODBC: not used \n
	                                        //!< CICS: configuration (e.g. "VSEGO_70"); precedence over slot ID    
	VarString          database;			//!< \brief
											//!< Informix: database (e.g. "ode21") \n
											//!< ODBC: data source name (e.g. "zdev21_ode21") \n
											//!< CICS: hold CICS name for opened session after successful connect (e.g. "A02CICS2")
	VarString          user;				//!< database user            (optional)
	VarString          passwd;				//!< database user's password (optional)	
	CicsCustom         cicsCustom;			//!< custom settings for CICS connection			
	ConnectionTypeEnum eConnectType;		//!< database type (INFORMIX/ODBC/CICS)
	Int32			   connectionId;        //!< \brief
											//!< Informix: not used \n
											//!< ODBC: not used \n
											//!< CICS: slot ID, dbserver must not be set
	bool               concurrTransaction;	//!< \brief
											//!< Informix: \n
											//!< &nbsp;&nbsp;&nbsp;&nbsp; true: can switch to different connection while a transaction 
											//!<                                is active in current connection.\n
											//!< &nbsp;&nbsp;&nbsp;&nbsp; false: if within transaction, SET CONNECTION statement fails, 
											//!<                                 returning an error, and transaction in current connection 
											//!<                                 continues to be active \n
											//!< ODBC: not used \n
											//!< CICS: not used
    InfxConnTime       conntime;            //!< Informix connection timeout settings
};

//------------------------------------------------------------------------------------------------------------//
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------------------------------------//
#endif	// GUARD

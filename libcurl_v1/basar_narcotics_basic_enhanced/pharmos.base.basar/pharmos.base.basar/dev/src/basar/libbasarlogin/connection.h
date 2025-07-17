//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class for login
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CONNECTION_LIBBASARLOGIN_H
#define GUARD_CONNECTION_LIBBASARLOGIN_H

//----------------------------------------------------------------------------
#include "libbasardbsql_databaseinfo.h"
#include "libbasardbaspect_connectionref.h"
#include "configurationptr.h"
#include "configuration.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//---------------------------------------------------------------------------
/*! \brief class for use case connection; handling standard and backup connect 
	\n throws BasarException
	\n final class */
class Connection
{

public:	

	typedef boost::shared_ptr< Connection >   Connectionptr;

	/*! \brief std-destructor
		\n no-throw */
	virtual ~Connection();
	
	/*! \brief std-constructor
		\n no-throw */
	Connection( Configurationptr config );

	/*! \brief start main connection
		\n no-throw */
	void connect();
	
	/*! \brief start backup connection
		\n BasarException */
	void connectBackup();
	
	/*! \brief disconnect open connections
		\n BasarException */
	void disconnect();

	/*! \brief is backup connected
		\n no-throw */
	bool isBackup() const;

	/*! \brief return ref to active connection
		\n no-throw */
	const basar::db::aspect::ConnectionRef getActiveConn() const;

	/*! \brief return name of host (main)
		\n no-throw */
	const basar::VarString	getHostname			() const;
	
	/*! \brief return name of database (main)
		\n no-throw */
	const basar::VarString	getDatabase			() const;
	
	/*! \brief return name of host (backup)
		\n no-throw */
	const basar::VarString	getHostnameBackup   () const;
	
	/*! \brief return name of database (backup)
		\n no-throw */
	const basar::VarString	getDatabaseBackup	() const;

	/*! \brief set name of host (main)
		\n no-throw */
	void					setHostname			(const basar::VarString	& rHostMain	//!< main host name
												);
	
	/*! \brief set name of host (backup)
		\n no-throw */
	void					setHostnameBackup	(const basar::VarString	& rHostBackup //!< backup host name
												);
	
	/*! \brief set name of database (main)
		\n no-throw */
	void					setDatabase			(const basar::VarString	& rDbMain //!< main database name
												);
	
	/*! \brief set name of database (backup)
		\n no-throw */
	void					setDatabaseBackup	(const basar::VarString	& rDbBackup //!< backup database name
												);

private:
	bool m_Backup;	//!< backup connection is active

	basar::login::Configurationptr	m_Configuration;

	basar::db::aspect::ConnectionRef::DatabaseInfo  m_DbInfo;		//!< info to active connection
	basar::db::aspect::ConnectionRef				m_ActivConn;	//!< reference to active connection
	basar::db::aspect::ConnectionRef				m_ConnToRelease;/*!< \brief connection to release at appl end 
																		(only used in cases, where there was 
																		 needed a backup connection 
																		 --> then main is set to this member 
																		 to close at the end!)*/

	/*! \brief set main connection infos to m_DbInfo
		\n no-throw */
	void setMainInfo();
	
	/*! \brief set backup connection infos to m_DbInfo
		\n no-throw */
	void setBackupInfo();

	/*! \brief check if flag file exists!? If so, then connect login to test environment!
	    \n search order: executable's directory, working directory
		\n no-throw */
	bool isLogin4Test();

	//! \brief evaluate config parameters for connection 
	//! \n     no-throw
	void evalConfig();

	//! \brief evaluate config parameters for connection timeout
	//! \n     no-throw
	void evalConfigTimeout();

	VarString m_HostMain;		//!< main host name
    VarString m_HostBackup;		//!< backup host name
	VarString m_DbMain;			//!< main database name
    VarString m_DbBackup;		//!< backup database name
};

//----------------------------------------------------------------------------
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD

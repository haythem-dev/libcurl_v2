//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: manager implementation for database connections
 *  \author Michael Eichenlaub
 *  \date   23.08.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_MANAGERIMPL_H
#define GUARD_LIBBASARDBSQL_MANAGERIMPL_H

//------------------------------------------------------------------------------
#include "libbasarcmnutil_pattern.h"
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------------------------------------//
// forward declaration
//------------------------------------------------------------------------------------------------------------//
class DatabaseInfo;
class ConnectionRef;
class ConnectionList_Iterator;

//------------------------------------------------------------------------------
//! \brief manager for database connections \n final class
class ManagerImpl
{
	// --- friend section ----------------------------------------------------

	friend class cmnutil::Singleton < ManagerImpl > ;	//!< creator class
	friend class Connection;							//!< created class that's able to unregister 
	
public:

	// --- method section ----------------------------------------------------

	//! shut down all connections									\n no-throw
												   ~ManagerImpl		();

	//! \brief establish a connection to given database info		\n InvalidParameterException
	LIBBASARDBSQL_API	ConnectionRef				createConnect	(const DatabaseInfo & rDbInfo,
																	 bool                 openConn = true
																	);
	//! close all open connections and destroy their objects		\n InvalidStateException
	LIBBASARDBSQL_API  void							shutdown		(); 



	/*! \brief return the database info to given iterator and set connId to connections unique id 
 		\n     OutOfRangeIteratorException */
	LIBBASARDBSQL_API const DatabaseInfo & 			getConnInfo		(const ConnectionList_Iterator it, 
																	 VarString & connId 
																	) const;
	/*! \brief	check if connection to given iterator is open.
 				\n OutOfRangeIteratorException */
	LIBBASARDBSQL_API bool				 			isOpen			(const ConnectionList_Iterator it
																	) const;

	/*! \brief return an iterator to the first element of the connection list. 
 		\n     no-throw */
	LIBBASARDBSQL_API ConnectionList_Iterator		begin			() const;	
	/*! \brief return an iterator to the last element of the connection list. 
 		\n     no-throw */
	LIBBASARDBSQL_API ConnectionList_Iterator		end				() const;
	

	/*! \brief return the size of the connection list. 
 		\n     no-throw */
	LIBBASARDBSQL_API ConnectionListSize			size			() const;
	/*! \brief return the number of all open connections in CollConnection. 
 		\n     no-throw */
	LIBBASARDBSQL_API ConnectionListSize			sizeOpen		() const;	


private:

	// --- method section ----------------------------------------------------

													ManagerImpl		();								//!< constructed by singleton \n no-throw
													ManagerImpl		(const ManagerImpl & rCopy);	//!< forbidden
	ManagerImpl &									operator =		(const ManagerImpl & rCopy);	//!< forbidden

	//! unregister connection from collection
	void											unregister		(Connection * pConn				//!< connection reference to unregister
																	);	
    //! \brief compose database connection string
    VarString       getDbName       (const DatabaseInfo & dbinfo    //! connection parameters
                                    );

	// --- member section ----------------------------------------------------
	CollConnectionSharedPtr		m_CollConnectionSharedPtr;	//!< shared pointer to collection with connection objects
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for database connection
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_CONNECTIONREF_H
#define GUARD_LIBBASARDBSQL_CONNECTIONREF_H

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
// forward declarations
class DatabaseInfo;
class StatementRef;
class PreparedStatementRef;

//------------------------------------------------------------------------------
//! reference class representing connection (session) with database \n final class 
class ConnectionRef
{
	friend class ManagerImpl;	//!< creator class

public:
	    
	//! construct reference object and initialises shared pointer to implementation object to null					\n no-throw
	LIBBASARDBSQL_API						ConnectionRef			();

	//! destruct reference object																					\n no-throw
	LIBBASARDBSQL_API					   ~ConnectionRef			();

	LIBBASARDBSQL_API void					reset					();			//!< set pointer to implementation object to null	 \n no-throw
	LIBBASARDBSQL_API bool					isNull					() const;	//!< check if shared pointer to implementation class is null \n no-throw


	LIBBASARDBSQL_API void					open					();			//!< open connection to database     \n no-throw
	LIBBASARDBSQL_API void					close					();			//!< close connection to database    \n no-throw
	LIBBASARDBSQL_API bool					isOpen					() const;	//!< check if connection to database is logically open (by client status)	\n no-throw
	LIBBASARDBSQL_API bool					isValid					();      	//!< check if connection to database is technically open (by SQL query)		\n no-throw

	//! create statement object				\n no-throw
	LIBBASARDBSQL_API StatementRef			createStatement			();
	//! create prepared statement object	\n no-throw
	LIBBASARDBSQL_API PreparedStatementRef	createPreparedStatement	(const VarString & rSql,						//!< statement to prepare
 																	 CursorTypeEnum	   eCursorType = CURS_DEFAULT	//!< cursor type
																	);
	//! \brief create prepared cursor statement: "update/delete ... where current of ..."
	//!        "where current of" clause is automtically added and derived from stmtsel 
	//!        ("select ... for update", Informix specific)
	//! \n     no-throw
	LIBBASARDBSQL_API PreparedStatementRef	createPrepRowStmt		(const VarString    & rSql,		//!< statement to prepare
		                                                             PreparedStatementRef stmtSel	//!< select ... for update statement whose cursor should be used
																	);
	
	LIBBASARDBSQL_API void					begin					();			//!< begin transaction				\n no-throw
	LIBBASARDBSQL_API void					commit					();			//!< commit transaction				\n no-throw
	LIBBASARDBSQL_API void					rollback				();			//!< rollback transaction			\n no-throw

	LIBBASARDBSQL_API bool					isInTransaction			() const;	//!< check if transaction is in progress	\n no-throw


	//! set transaction isolation level																				\n no-throw
	LIBBASARDBSQL_API void					setIsolationLevel		(const basar::IsolationLevelEnum eLevel,				//!< transaction isolation level
																	 const bool						 retainLocks = false	//!< only for Informix:\n true : RETAIN UPDATE LOCKS for SELECT ... FOR UPDATE \n false: no retain
																	);
	//! set mode for accessing locked rows or tables														\n no-throw
	LIBBASARDBSQL_API void					setLockModeWait			(const basar::LockModeEnum eMode   ,	//!< mode for accessing locked rows or tables
																	 const UInt32              secs = 0		//!< maximum number of seconds that process waits for lock to be released before issuing an error
																	);

	//! set common flag for fetching: let fetched data untouched (i.e. with trailing blanks) or format (trim) them \n no-throw
	LIBBASARDBSQL_API void					setRawFetching			(bool raw	//!< true: untouched data \n false: formatted data
																	);
	//! set fetch buffer size			\n no-throw
	LIBBASARDBSQL_API void					setFetBufSize			(Int32 size	//!< new fetch buffer size in bytes
																	);
	//! get fetch buffer size			\n no-throw
	LIBBASARDBSQL_API Int32					getFetBufSize			();


	
	LIBBASARDBSQL_API const DatabaseInfo &	getDbInfo				() const;	//!< get database info \n no-throw


private:
	//! construct connection reference class by shared pointer to implementation object \n no-throw
											ConnectionRef			(ConnectionSharedPtr pImpl);
	
	//! check and give shared pointer to implementation object \n throw NullReferenceException
	const ConnectionSharedPtr &				getImpl					() const;

	ConnectionSharedPtr m_pImpl;	//!< shared pointer to connection implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD_LIBBASARDBSQL_CONNECTIONREF_H

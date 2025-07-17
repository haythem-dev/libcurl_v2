//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CONNECTION_H
#define GUARD_CONNECTION_H

//----------------------------------------------------------------------------
#include "database.h"
#include "sqloperative.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
class StatementRef;			// forward
class PreparedStatementRef; // forward

//----------------------------------------------------------------------------
//! connection implementation class \n base class
class Connection : public SqlOperative
{
	friend class Statement;			//!< needs connection methods
	friend class PreparedStatement;	//!< needs connection methods
	friend class Resultset;			//!< needs connection methods
	friend class Insertset;			//!< needs connection methods
	friend class ConnectionOdbc;	//!< for setting weak poiner
	friend class ConnectionInfx;	//!< for setting weak poiner
	friend class ConnectionCics;	//!< for setting weak poiner

public:
	
	// --- method section ----------------------------------------------------

	//! \brief get this pointer as shared pointer via weak pointer (-> only one group of shared pointer exists)
	ConnectionSharedPtr				getThis					();

	//! \brief close connection and unregister connection by manager \n no-throw
	virtual						   ~Connection				();

	//! \brief open connection to database		\n throw InvalidStateException
	virtual void					open					();
	//! \brief close connection to database		\n no-throw	
	virtual void					close					();
	//! \brief check if connection is logically open	\n no-throw
	bool							isOpen					() const;	
	//! \brief check if connection is technically open	\n no-throw
	virtual bool					isValid					() = 0;	
	//! \brief begin transaction				\n throw DbException
	virtual void					begin					() = 0;	
	//! \brief commit transaction				\n throw DbException
	virtual void					commit					() = 0;		
	//! \brief rollback transaction				\n throw DbException
	virtual void					rollback				() = 0;		
	//! \brief check if transaction is in progress	\n no-throw
	bool							isInTransaction			() const;	


	//! \brief set transaction isolation level				\n throw InvalidStateException
	virtual void					setIsolationLevel		(basar::IsolationLevelEnum eLevel,		//!< mode for accessing locked rows or tables
															 bool					   retainLocks  //!< only for Informix:\n true : RETAIN UPDATE LOCKS for SELECT ... FOR UPDATE \n false: no retain
															);

	//! \brief set transaction lock mode					\n throw InvalidStateException
	virtual void					setLockModeWait			(basar::LockModeEnum eMode,				//!< mode for accessing locked rows or tables
															 UInt32				 secs				//!< maximum number of seconds that process waits for lock to be released before issuing an error
															);

	//! \brief set common flag for fetching: let fetched data untouched (i.e. with trailing blanks) or format (trim) them \n no-throw
	virtual void					setRawFetching			(bool raw			//!< true: untouched data \n false: formatted data
															);
	//! \brief set fetch buffer size						\n throw InvalidStateException
	virtual void					setFetBufSize			(Int32 size			//!< new fetch buffer size in bytes
															);
	//! \brief get fetch buffer size						\n throw InvalidStateException
	virtual Int32					getFetBufSize			();


	//! \brief create statement reference object			\n no-throw	
	//! \return created StatementRef
	virtual StatementRef			createStatement			() = 0;
			
	//! \brief create prepared statement reference object	\n no-throw
	//! \return created PreparedStatementRef
	virtual PreparedStatementRef	createPreparedStatement	(const VarString & rSql,		//!< statement to prepare
															 CursorTypeEnum	   eCursorType	//!< cursor type
															) = 0;
	//! \brief create prepared statement for update/delete cursor 
	//!        (Informix: ... where current of CURSOR)
	//! \n throw InvalidStateException
	virtual PreparedStatementRef	createPrepRowStmt		(const VarString    & rSql,		//!< statement to prepare
															 PreparedStatementRef stmtSel	//!< select ... for update statement whose cursor should be used
															);
	//! \brief get database info object			\n no-throw
	const DatabaseInfo & 			getDbInfo				() const;	
	

protected:

	// --- method section ----------------------------------------------------

	//! \brief construct implementation object by database specific connection data	\n no-throw
									Connection				(const DatabaseInfo & rDbInfo 		//!< database specific connection data NULL
															);

									Connection				();									//!< forbidden
									Connection				(const Connection & rCopy);			//!< forbidden
	Connection &    				operator =				(const Connection & rCopy);			//!< forbidden

	//! \brief check connection state                        	\n throw InvalidStateException
	virtual void					checkConnState			(const VarString & rWhat,	//!< text description 
															 const VarString & rFile,	//!< file where exception is thrown
															 UInt32			   line		//!< line of file where exception is thrown 
													        ) const;

	//! \brief check if raw flag for fetching is set			\n no-throw
	virtual bool					isFetchModeRaw			() const;

	//! \brief build connect string							\n no-throw
	VarString						getConnectStr			() const;

	//! \brief set member weak pointer
	void							setWeakPtr				(ConnectionWeakPtr weakPtr	//!< weak pointer to set to m_WeakPtr
															);		

	// --- member section ----------------------------------------------------

	Database						m_Database;			//!< database implementation object
	bool							m_Open;				//!< connection state
	bool							m_InTransaction;	//!< transaction state
	bool							m_flagRawFetching;	//!< true: fetching untouched data \n false: fetching formatted data
	VarString						m_ConnId;			//!< application's unique connection ID (i.e. object address)

	ConnectionWeakPtr				m_WeakPtr;			//!< weak pointer to this, needed for common reference group 
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

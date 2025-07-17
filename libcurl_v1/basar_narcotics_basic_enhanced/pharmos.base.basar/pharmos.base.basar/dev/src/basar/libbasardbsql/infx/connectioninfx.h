//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class for informix
 *  \author Thomas Hörath
 *  \date   06.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CONNECTIONINFX_H
#define GUARD_CONNECTIONINFX_H

//----------------------------------------------------------------------------
#include "connection.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! connection implementation class \n final class
class ConnectionInfx : public Connection
{
	friend class StatementInfx;				//!< for accessing setCurrent()
	friend class PreparedStatementInfx;		//!< for accessing setCurrent()
	friend class InsertsetInfx;				//!< for accessing setCurrent()
	friend class ResultsetInfx;				//!< for accessing setCurrent()

public:

	// --- method section ----------------------------------------------------

	//! create ConnectionInfx implementation object by database specific connection data	\n no-throw
	static ConnectionSharedPtr		create					(const DatabaseInfo & rDbInfo ,	//!< database specific connection data
															 bool                 openConn	//!< open connection to database
															);
//! \brief close connection		\n no-throw
	virtual						   ~ConnectionInfx			();		

	//! \brief open connection to database		\n throw InvalidStateException
	virtual void					open					();
	//! \brief close connection to database		\n no-throw
	virtual void					close					();

	//! \brief check if connection is technically open	\n no-throw
	virtual bool					isValid					();	

	//! \brief begin transaction				\n no-throw
	virtual void					begin					();	
	//! \brief commit transaction				\n no-throw
	virtual void					commit					();	
	//! \brief rollback transaction				\n no-throw
	virtual void					rollback				();		

	//! \brief set transaction isolation level \n no-throw
	virtual void					setIsolationLevel		(basar::IsolationLevelEnum eLevel,
															 bool					   retainLocks
															);
	//! set transaction lock mode       \n no-throw
	virtual void					setLockModeWait			(basar::LockModeEnum eMode,
															 UInt32              secs 
															);

	//! set fetch buffer size			\n no-throw 
	virtual void					setFetBufSize			(Int32 size		//!< new fetch buffer size in bytes
															);

	//! get fetch buffer size			\n no-throw
	virtual Int32					getFetBufSize			();


	//! create statement reference object			\n no-throw
	virtual StatementRef			createStatement			();	
	//! create prepared statement reference object	\n no-throw
	virtual PreparedStatementRef	createPreparedStatement	(const VarString & rSql,		//!< SQL statement to prepare
															 CursorTypeEnum	   eCursorType	//!< cursor type
															);
	//! \brief create prepared statement for update/delete cursor 
	//!        (Informix: ... where current of CURSOR)
	//! \n     no-throw
	virtual PreparedStatementRef	createPrepRowStmt	    (const VarString    & rSql,		//!< statement to prepare
															 PreparedStatementRef stmtSel	//!< select ... for update statement whose cursor should be used
															);

private:

	// --- method section ----------------------------------------------------

	//! construct implementation object by database specific connection data	\n no-throw
									ConnectionInfx			(const DatabaseInfo & rDbInfo 		//!< database specific connection data NULL
															);

									ConnectionInfx			();										//!< forbidden
									ConnectionInfx			(const ConnectionInfx & rCopy);			//!< forbidden
	ConnectionInfx &    			operator =				(const ConnectionInfx & rCopy);			//!< forbidden

	//! \brief set connection to current	 - can be set to critical section in future	\n no-throw
	void							setCurrent				();

	//! \brief get current connection - can be set to critical section in future		\n no-throw
	const ConnectionInfx *			getCurrent				();

	//! \brief reset current connection to 0 - can be set to critical section in future	\n no-throw
	void							resetCurrent			();

	//! \brief set current connection to static member									\n no-throw
	void							saveCurrent				();

	static const ConnectionInfx * 	s_pCurrent;			//!< current connection (i.e. object address)

};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

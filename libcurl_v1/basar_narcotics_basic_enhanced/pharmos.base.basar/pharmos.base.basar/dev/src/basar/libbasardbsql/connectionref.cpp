//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection reference class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_connectionref.h"

#include "libbasarcmnutil_exceptions.h"
#include "connection.h"
#include "libbasardbsql_statementref.h"
#include "libbasardbsql_preparedstatementref.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
ConnectionRef::ConnectionRef ()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \param  pImpl shared pointer to implementation object
ConnectionRef::ConnectionRef ( ConnectionSharedPtr pImpl )
                             : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
ConnectionRef::~ConnectionRef ()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ConnectionRef::reset()
{
	m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool ConnectionRef::isNull() const
{
	return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
const ConnectionSharedPtr & ConnectionRef::getImpl() const
{
	if (0 == m_pImpl.get())
		throw NullReferenceException(ExceptInfo ("basar.db.sql.ConnectionRef.getImpl", 
												 "m_pImpl.get() == 0", 
												 __FILE__, __LINE__));

	return m_pImpl;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionRef::open()
{
	getImpl()->open();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionRef::close()
{
	getImpl()->close();
}

//----------------------------------------------------------------------------
//!	\retval true  if connection is logically opened
//! \retval false if connection is logically close/not opened
//! \throw  no-throw
bool ConnectionRef::isOpen() const
{
	return getImpl()->isOpen();
}

//----------------------------------------------------------------------------
//!	\retval true  if connection is technically opened
//! \retval false if connection is technically close/not opened
//! \throw  no-throw
bool ConnectionRef::isValid()
{
	return getImpl()->isValid();
}

//----------------------------------------------------------------------------
//!	\return statement reference object
//! \throw no-throw
StatementRef ConnectionRef::createStatement()
{
	return getImpl()->createStatement();
}

//----------------------------------------------------------------------------
//!	\return prepared statement reference object
//! \throw no-throw
PreparedStatementRef ConnectionRef::createPreparedStatement(const VarString & rSql,
															CursorTypeEnum	  eCursorType /* = CURS_DEFAULT */ )
{
	return getImpl()->createPreparedStatement(rSql, eCursorType);
}

//----------------------------------------------------------------------------
//!	\return prepared statement reference object
//! \throw no-throw
PreparedStatementRef ConnectionRef::createPrepRowStmt(const VarString    & rSql,
													  PreparedStatementRef stmtSel)
{
	return getImpl()->createPrepRowStmt(rSql, stmtSel);
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionRef::begin()
{
	getImpl()->begin();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionRef::commit()
{
	getImpl()->commit();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionRef::rollback()
{
	getImpl()->rollback();
}

//----------------------------------------------------------------------------
//!	\return transaction in progress
//! \throw  no-throw
bool ConnectionRef::isInTransaction() const
{
	return getImpl()->isInTransaction();
}

//----------------------------------------------------------------------------
//!	\return database info
//! \throw  no-throw
const DatabaseInfo & ConnectionRef::getDbInfo() const
{
	return getImpl()->getDbInfo();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ConnectionRef::setIsolationLevel(const basar::IsolationLevelEnum eLevel,
									  const bool					  retainLocks /* = false */)
{
	getImpl()->setIsolationLevel( eLevel, retainLocks);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
void ConnectionRef::setLockModeWait(const basar::LockModeEnum eMode,
								    const UInt32       secs /* = 0 */)
{
	getImpl()->setLockModeWait( eMode, secs );
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionRef::setRawFetching (bool raw)
{
	getImpl()->setRawFetching(raw);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionRef::setFetBufSize (Int32 size)
{
	getImpl()->setFetBufSize(size);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return size of fetch buffer
Int32 ConnectionRef::getFetBufSize()
{
	return getImpl()->getFetBufSize();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

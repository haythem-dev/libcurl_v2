//------------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//------------------------------------------------------------------------------

#include "connection.h"

#include "libbasardbsql_manager.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasardbsql_exceptions.h"
#include "libbasardbsql_preparedstatementref.h"

//------------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \throw no-throw
Connection::Connection ( const DatabaseInfo & rDbInfo )
                       : m_Database       (rDbInfo),
					     m_Open           (false  ),
						 m_InTransaction  (false  ),
					     m_flagRawFetching(false  )
{
	// each byte of address is represented by 2-digit hexadecimal number
	m_ConnId.format("connect_0x%0*lx", sizeof(size_t) * 2, this);
}

//------------------------------------------------------------------------------
//! \throw no-throw
Connection::~Connection()
{
	Manager::getInstance().unregister(this);
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException 
void Connection::open()
{
	ConstString func = "basar.db.sql.Connection.open";

	checkSqlState (func, __FILE__, __LINE__);

	m_Open = true;
}

//------------------------------------------------------------------------------
//! \throw no-throw
void Connection::close()
{	
	ConstString func = "basar.db.sql.Connection.close";

	checkSqlState   (func, __FILE__, __LINE__);

	m_Open = false;
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException
//! \return nothing (always throws)
PreparedStatementRef Connection::createPrepRowStmt (const VarString    & ,
												    PreparedStatementRef )
{
	bool f = true;


	if (f)
	{
		// if this virtual method is called then 
		// there doesn't exist any RDBMS specific implementation
		ExceptInfo exinfo("basar.db.sql.Connection.createPrepRowStmt",
						  "method not implemented in RDBMS specific context",
						   __FILE__, __LINE__);
		throw InvalidStateException(exinfo);
	}

	return PreparedStatementRef();
}

//----------------------------------------------------------------------------
//!	\return transaction in progress
//! \throw  no-throw
bool Connection::isInTransaction() const
{
	return m_InTransaction;
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! 
void Connection::setIsolationLevel(basar::IsolationLevelEnum,
								   bool                     )
{
	// if this virtual method is called then there doesn't exist 
    // any RDBMS specific implementation
	throw InvalidStateException(
        ExceptInfo(
            "basar.db.sql.Connection.setIsolationLevel",
										   "method not implemented in RDBMS specific context",
										   __FILE__, __LINE__));
}
	
//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! 
void Connection::setLockModeWait(basar::LockModeEnum,
			  					 UInt32             )
{
	// if this virtual method is called then there doesn't exist 
    // any RDBMS specific implementation
	throw InvalidStateException(
        ExceptInfo(
            "basar.db.sql.Connection.setLockModeWait",
										   "method not implemented in RDBMS specific context",
										   __FILE__, __LINE__));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \retval true  fetching untouched data 
//! \retval false fetching formatted data
bool Connection::isFetchModeRaw() const
{
	return m_flagRawFetching;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! 
void Connection::setRawFetching(bool raw)
{
	m_flagRawFetching = raw;
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! 
void Connection::setFetBufSize(Int32)
{
	// if this virtual method is called then there doesn't exist 
    // any RDBMS specific implementation
	throw InvalidStateException(
            ExceptInfo(
                "basar.db.sql.Connection.setFetBufSize",
										   "method not implemented in RDBMS specific context",
										   __FILE__, __LINE__));
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! \return nothing (always throws)
Int32 Connection::getFetBufSize()
{
	// if this virtual method is called then there doesn't exist 
    // any RDBMS specific implementation
	throw InvalidStateException(
            ExceptInfo(
                "basar.db.sql.Connection.getFetBufSize",
											   "method not implemented in RDBMS specific context",
											   __FILE__, __LINE__));
	}

//------------------------------------------------------------------------------
//!	\retval true  if connection is opened
//! \retval false if connection is closed/not opened
//! \throw no-throw
bool Connection::isOpen() const
{
	return m_Open;
}

//------------------------------------------------------------------------------
//!	\return database info
//! \throw no-throw
const DatabaseInfo & Connection::getDbInfo() const
{
	return m_Database.getDbInfo();
}

//------------------------------------------------------------------------------
//! \return connect string <database\@dbserver>
//! \throw no-throw
VarString Connection::getConnectStr() const
{
    // delimiter for connect string (e.g. "db@dbserver")
	static ConstString DBDELIM = "@";		

    VarString          conn(m_Database.getDbInfo().database);
	
	conn += DBDELIM;
	conn += m_Database.getDbInfo().dbServer;

	return conn;
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException
void Connection::checkConnState (const VarString & rWhat,
								 const VarString & rFile,
								 UInt32			   line ) const
{
	if (!isOpen())
		throw InvalidStateException(
            ExceptInfo(rWhat, "!isOpen()", rFile, line));
}

//------------------------------------------------------------------------------
//! \throw DbException
void Connection::begin()
{
	if (isInTransaction())
		throw DbException(
                ExceptInfo(
                    "basar.db.sql.Connection.begin", 
		                              "already in transaction", 
									  __FILE__, __LINE__));

	m_InTransaction = true;
}

//------------------------------------------------------------------------------
//! \throw DbException
void Connection::commit()
{
	if (!isInTransaction())
		throw DbException(
                ExceptInfo(
                    "basar.db.sql.Connection.commit", 
		                              "not in transaction", 
						  	          __FILE__, __LINE__));
	m_InTransaction = false;
}

//------------------------------------------------------------------------------
//! \throw DbException
void Connection::rollback()
{
	if (!isInTransaction())
		throw DbException(
                ExceptInfo(
                    "basar.db.sql.Connection.rollback", 
		                              "not in transaction", 
									  __FILE__, __LINE__));
	m_InTransaction = false;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return (only one group of) connection shared pointer 
ConnectionSharedPtr	Connection::getThis()
{
	return ConnectionSharedPtr(m_WeakPtr);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
void Connection::setWeakPtr(ConnectionWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar


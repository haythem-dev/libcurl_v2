//------------------------------------------------------------------------------//
/*! \file
 *  \brief  connection implementation class
 *  \author Björn Bischof
 *  \date   21.10.2005
 */
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//

#include "connection.h"

#include "libbasardbsql_statementref.h"
#include "libbasardbsql_preparedstatementref.h"
#include "libbasardbaspect_exceptions.h"
#include "accessor.h"

//------------------------------------------------------------------------------//
// namespace
//------------------------------------------------------------------------------//
namespace basar	 {
namespace db	 {
namespace aspect {

//-------------------------------------------------------------------------------//
// static section
//-------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
//! \param connRef reference to a db::sql connection
//! \return shared pointer to created connection
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<Connection> Connection::create( const basar::db::sql::ConnectionRef connRef )
{
	boost::shared_ptr<Connection> p( new Connection(connRef) );
    p->setThisPtr( p );
    return p;
}
//-------------------------------------------------------------------------------//
// object section
//-------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
//! \param pWeak weak pointer to set to member
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////
void Connection::setThisPtr( const boost::weak_ptr<Connection> pWeak )
{
	m_this_as_weak_ptr = pWeak;
}

///////////////////////////////////////////////////////////////////////////////////
//! \param connRef reference to a db::sql connection
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////
Connection::Connection ( const basar::db::sql::ConnectionRef connRef )
					   : m_SqlConnectionRef ( connRef ), 
						 m_InTransaction    ( false )
{
}

///////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
}

//-------------------------------------------------------------------------------//
// transaction methods
//-------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
/*!	\throw	AlreadyInTransactionException */
///////////////////////////////////////////////////////////////////////////////////
void Connection::beginTransaction()
{
	ConstString fun = "Connection::transactionBegin()";
	
	if( m_InTransaction )
		throw AlreadyInTransactionException( ExceptInfo( fun, "Already in Transaction!", __FILE__, __LINE__) );

	m_SqlConnectionRef.begin();
	m_InTransaction = true;
}

///////////////////////////////////////////////////////////////////////////////////
/*!	\throw	NotInTransactionException */
///////////////////////////////////////////////////////////////////////////////////
void Connection::commitTransaction()
{
	ConstString fun = "Connection::transactionCommit()";

	if( !m_InTransaction )
		throw NotInTransactionException(ExceptInfo( fun, "Not in Transaction!", __FILE__, __LINE__) );

	m_SqlConnectionRef.commit();

	clearRegisteredAccessors();
	m_InTransaction = false;
}

///////////////////////////////////////////////////////////////////////////////////
//!	\throw	NotInTransactionException
///////////////////////////////////////////////////////////////////////////////////
void Connection::rollbackTransaction()
{
	const char* fun = "Connection::transactionRollback()";

	if( ! m_InTransaction )
		throw NotInTransactionException(ExceptInfo( fun, "Not in Transaction!", __FILE__, __LINE__) );

	m_SqlConnectionRef.rollback(); // rollback of the database

	// rollback of the propertytable( reset to the snapshot created in the execute()-method )
	resetAccessorsToSnapshots();
	clearRegisteredAccessors();
	m_InTransaction = false;
}

///////////////////////////////////////////////////////////////////////////////////
//!	\throw	InvalidValueException
///////////////////////////////////////////////////////////////////////////////////
void Connection::clearRegisteredAccessors()
{
	AccessorList::const_iterator itEnd = m_RegisteredAccessors.end();
	AccessorSharedPtr            pAcc;

	for (AccessorList::iterator it = m_RegisteredAccessors.begin(); itEnd != it; ++it)
	{
		// pAcc is thread safe: see boost doc
		if ( !(pAcc = it->second.lock()) )		// wpAcc expired
			throw InvalidValueException(ExceptInfo( "Connection::clearRegisteredAccessors()",
				                                    "registered accessor instance has been expired",
													__FILE__, __LINE__));

		pAcc->clearInternalSnapshot();
	}

	m_RegisteredAccessors.clear();
}
///////////////////////////////////////////////////////////////////////////////////
//!	\throw	InvalidValueException
///////////////////////////////////////////////////////////////////////////////////
void Connection::resetAccessorsToSnapshots()
{
	AccessorList::const_iterator itEnd = m_RegisteredAccessors.end();
	AccessorSharedPtr            pAcc;

	for (AccessorList::iterator it = m_RegisteredAccessors.begin(); itEnd != it; ++it )
	{
		// pAcc is thread safe: see boost doc
		if ( !(pAcc = it->second.lock()) )		// wpAcc expired
			throw InvalidValueException(ExceptInfo( "Connection::resetAccessorsToSnapshots()",
				                                    "registered accessor instance has been expired",
													__FILE__, __LINE__));

		pAcc->resetInternalSnapshot();
	}
}

///////////////////////////////////////////////////////////////////////////////////
//!	\retval true if a transaction is activ
//! \return otherwise returns false
//!	\throw	no-throw
///////////////////////////////////////////////////////////////////////////////////
bool Connection::isInTransaction() const
{
	return m_InTransaction;
}
///////////////////////////////////////////////////////////////////////////////////
//!	\retval true if given accessor is registered
//! \return otherwise returns false
//! \param  wpAcc weak pointer to accessor
//!	\throw	InvalidValueException
///////////////////////////////////////////////////////////////////////////////////
bool Connection::isRegistered( const AccessorWeakPtr wpAcc ) const
{
	AccessorSharedPtr pAcc = wpAcc.lock();

	// pAcc is thread safe: see boost doc
	if ( !pAcc )		// wpAcc expired
		throw InvalidValueException(ExceptInfo( "Connection::isRegistered()",
				                                "registered accessor instance has been expired",
												__FILE__, __LINE__));

	const AccessorList::key_type key = pAcc.get();

	return (m_RegisteredAccessors.find(key) != m_RegisteredAccessors.end());
}

///////////////////////////////////////////////////////////////////////////////////
//!	\retval true if given accessor is registered
//! \return otherwise returns false
//! \param  acc reference to accessor
///////////////////////////////////////////////////////////////////////////////////
bool Connection::isRegistered( Accessor& acc) const
{
    return (m_RegisteredAccessors.find(&acc) != m_RegisteredAccessors.end());
}

///////////////////////////////////////////////////////////////////////////////////
//!	\throw	InvalidValueException
//! \param  wpAcc weak pointer to accessor
///////////////////////////////////////////////////////////////////////////////////
void Connection::doRegister( const AccessorWeakPtr wpAcc )
{
	AccessorSharedPtr pAcc = wpAcc.lock();

	// pAcc is thread safe: see boost doc
	if ( !pAcc )		// wpAcc expired
		throw InvalidValueException(ExceptInfo( "Connection::doRegister()",
				                                "registered accessor instance has been expired",
												__FILE__, __LINE__));

	// duplicate value is not stored (could be checked by pair<>.second)
	m_RegisteredAccessors.insert(AccessorList::value_type(pAcc.get(), wpAcc));	
}

///////////////////////////////////////////////////////////////////////////////////
//!	\throw	NotInTransactionException, InvalidParameterException 
//! \param  acc accessor to unregister
///////////////////////////////////////////////////////////////////////////////////
void Connection::unRegister( Accessor & acc )
{
	ConstString fun = "Connection::unRegister()";

	if ( !m_InTransaction )
	{
		if (0 != m_RegisteredAccessors.size())
		{
			VarString msg;
			msg.format("registered accessors list not empty() after transaction: %d item(s)", m_RegisteredAccessors.size());
			throw NotInTransactionException(ExceptInfo( fun, msg, __FILE__, __LINE__) );
		}

		return;		// nothing to do: list was emptied by rollback transaction
	}
    
    //logic hole here: what's with unregistered accessors?
	
	if (0 == m_RegisteredAccessors.erase(&acc))
	{
		VarString msg;
		msg.format("registered accessor instance <%s> couldn't be erased", acc.getAccessorName().c_str());
		throw InvalidParameterException(ExceptInfo( fun, msg, __FILE__, __LINE__));
	}
}

///////////////////////////////////////////////////////////////////////////////////
/*!	\return a reference object of the class StatementRef belonging to the 
			concerned connection.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
Connection::StatementRef Connection::createStatement()
{
	return m_SqlConnectionRef.createStatement();
}
///////////////////////////////////////////////////////////////////////////////////
/*!	\return a reference object of the class StatementRef belonging to the 
			concerned connection.
	\param  rSql sql statement to prepare
	\param  eCursorType cursor type to use in prepare (defaults to sql::CURS_DEFAULT)
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
Connection::PreparedStatementRef Connection::createPreparedStatement(const VarString& rSql, sql::CursorTypeEnum eCursorType /* = sql::CURS_DEFAULT*/)
{
    return m_SqlConnectionRef.createPreparedStatement(rSql, eCursorType);
}
///////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
///////////////////////////////////////////////////////////////////////////////////
void Connection::open()
{
	m_SqlConnectionRef.open();
}
///////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
///////////////////////////////////////////////////////////////////////////////////
void Connection::close()
{
	m_SqlConnectionRef.close();
}
///////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if the connection (this) has already been opened.
	\return otherwise returns false
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
bool Connection::isOpen() const
{
	return m_SqlConnectionRef.isOpen();
}

///////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if the connection technically open.
	\return otherwise returns false
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
bool Connection::isValid()
{
    return m_SqlConnectionRef.isValid();
}

///////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
//! \param  eLevel isolation level
///////////////////////////////////////////////////////////////////////////////////
void Connection::setIsolationLevel( const basar::IsolationLevelEnum eLevel )
{
	m_SqlConnectionRef.setIsolationLevel( eLevel );
}
///////////////////////////////////////////////////////////////////////////////////
//! \param  eMode lock mode
//! \param  secs seconds to wait in locked state
//!	\throw	no-throw
///////////////////////////////////////////////////////////////////////////////////
void Connection::setLockModeWait( const basar::LockModeEnum eMode, const UInt32 secs )
{
	m_SqlConnectionRef.setLockModeWait( eMode, secs );
}
///////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////
void Connection::setFetBufSize(int size)
{
	m_SqlConnectionRef.setFetBufSize(size);
}
///////////////////////////////////////////////////////////////////////////////////
/*!	\return	the size of the fetch buffer
	\throw	 no-throw */
///////////////////////////////////////////////////////////////////////////////////
int Connection::getFetBufSize()
{
	return m_SqlConnectionRef.getFetBufSize();
}
///////////////////////////////////////////////////////////////////////////////////
/*!	\return	current, connection specific information about db, dbserver, user, password
			and the type of connection (odbc, Informix)
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
const Connection::DatabaseInfo&	Connection::getDbInfo() const
{
	return m_SqlConnectionRef.getDbInfo();
}

///////////////////////////////////////////////////////////////////////////////////
/*!	\return	connectionref of db::sql
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////
const basar::db::sql::ConnectionRef	Connection::getSqlConnection() const
{
	return m_SqlConnectionRef;
}

//------------------------------------------------------------------------------
}	// namespace aspect
}	// namespace db
}	// namespace basar

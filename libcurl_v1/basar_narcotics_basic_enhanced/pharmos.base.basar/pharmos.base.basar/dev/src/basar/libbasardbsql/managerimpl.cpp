//----------------------------------------------------------------------------
/*! \file
 *  \brief  manager implementation class
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_managerimpl.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"

#include "libbasardbsql_functions.h"
#include "libbasardbsql_connectionlist_iterator.h"
#include "libbasardbsql_connectionref.h"
#include "connectioninfx.h"
#include "connectioncics.h"
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#include "connectionodbc.h"
#endif

//----------------------------------------------------------------------------
namespace basar	  {

//----------------------------------------------------------------------------
namespace cmnutil {

using db::sql::ManagerImpl;

/*!	specialised method */
//! \return shared_ptr to ManagerImpl instance
template <>
typename Singleton<ManagerImpl>::InstancePtr & Singleton<ManagerImpl>::inst()
{
    // avoid init order fiasco
    db::sql::init();

    static InstancePtr s_Inst;
    return s_Inst;
}

//----------------------------------------------------------------------------
}

//----------------------------------------------------------------------------
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw no-throw
//!
ManagerImpl::ManagerImpl()
: m_CollConnectionSharedPtr( new CollConnection )
{
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
ManagerImpl::~ManagerImpl()
{
	try
	{
		shutdown();
	}
	catch (const basar::Exception& ex)
	{

        LoggerPool::manager().forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
        
        assert(0);
	}
	catch (...)
	{
        LoggerPool::manager().forcedLog(log4cplus::ERROR_LOG_LEVEL, "unknown exception", __FILE__, __LINE__);
		assert(0);
	}
}

//----------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \param  rDbInfo  database specific connection data
//! \param  openConn open connection to database, default true
//! \return connection reference object
//----------------------------------------------------------------------------
ConnectionRef ManagerImpl::createConnect(const DatabaseInfo & rDbInfo,
										 bool                 openConn /* = true */)
{

	ConnectionRef connRef ;


	if (basar::INFORMIX == rDbInfo.eConnectType )
	{
		connRef = ConnectionInfx::create(rDbInfo, openConn);
	}
	else if (basar::ODBC == rDbInfo.eConnectType)
	{
		/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
		connRef =  ConnectionOdbc::create(rDbInfo, openConn) ;
#else
		throw InvalidParameterException(ExceptInfo("basar.db.sql.Connection.create", 
			"no odbc connection for Unix implemented!", 
			__FILE__, __LINE__));
#endif
	}
	else if (basar::CICS == rDbInfo.eConnectType )
	{
		connRef =  ConnectionCics::create(rDbInfo, openConn) ;
	}
	else
	{
		throw InvalidParameterException(
			ExceptInfo("basar.db.sql.Connection.create", 
			"connectiontype unknown!", 
			__FILE__, __LINE__));
	}


	CollConnection::value_type collVal(connRef.getImpl().get(),			// connection impl address as key
									   connRef.getImpl()      );		// connection reference as weak pointer
	m_CollConnectionSharedPtr->insert(collVal);

	if (LoggerPool::manager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("register connect_0x%0*x (%s/%s/%s)", 
			       sizeof(ULong32 *) * 2, connRef.getImpl().get(),
				   rDbInfo.dbServer.c_str(),
				   getDbName(rDbInfo).c_str(),
				   rDbInfo.user    .c_str());
		LoggerPool::manager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return connRef;
}

//----------------------------------------------------------------------------
//! \throw InvalidStateException
//! 
void ManagerImpl::shutdown()
{
	CollConnection::iterator		itConn;
	CollConnection::const_iterator	citEnd = m_CollConnectionSharedPtr->end();
	
	for( itConn = m_CollConnectionSharedPtr->begin(); citEnd != itConn; ++itConn)
	{
		if ( itConn->second.expired() )
			throw InvalidStateException(ExceptInfo ("basar.db.sql.ManagerImpl.shutdown", 
													"itConn->second.expired() == true", 
													__FILE__, __LINE__));

		if ( itConn->second.lock()->isOpen() )
			itConn->second.lock()->close();				// close connection

		if ( itConn->second.expired() )
		{
			if (LoggerPool::manager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("shutdown: unregister connect_0x%0*x (%s/%s/%s)", 
						sizeof(ULong32 *) * 2, itConn->first,
						itConn->first->getDbInfo().dbServer.c_str(),
						getDbName(itConn->first->getDbInfo()).c_str(),
						itConn->first->getDbInfo().user    .c_str());
				LoggerPool::manager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}

			m_CollConnectionSharedPtr->erase(itConn);			// erase connection from collection: attention 
		}
		else 
		{	
			// unerasable connection
			if (LoggerPool::manager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("shutdown: couldn't unregister connect_0x%0*x (%s/%s/%s): is still referenced %d time(s)", 
						sizeof(ULong32 *) * 2, itConn->first,
						itConn->first->getDbInfo().dbServer.c_str(),
						getDbName(itConn->first->getDbInfo()).c_str(),
						itConn->first->getDbInfo().user    .c_str(),
						itConn->second.use_count());
				LoggerPool::manager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
		}
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! 
void ManagerImpl::unregister(Connection * pConn)
{
	if( 0 == m_CollConnectionSharedPtr->erase(pConn))
	{
		// ok if Manager has been cleared before
		//assert(0);	// no exception because called by ~ConnectionRef
	}

	if (LoggerPool::manager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("unregister connect_0x%0*x (%s/%s/%s)", 
			       sizeof(ULong32 *) * 2, pConn,
				   pConn->getDbInfo().dbServer.c_str(),
				   getDbName(pConn->getDbInfo()).c_str(),
				   pConn->getDbInfo().user    .c_str());
		LoggerPool::manager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing composed database connection string
VarString ManagerImpl::getDbName(const DatabaseInfo & dbinfo)
{
    VarString db;

    if (basar::CICS == dbinfo.eConnectType)
        db.format(
        "%s:%s", 
        dbinfo.cicsCustom.ipAddr.c_str(),
        dbinfo.cicsCustom.port  .c_str());
    else
        db = dbinfo.database;

    return db;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return iterator to first element of ConnectionList
ConnectionList_Iterator ManagerImpl::begin() const
{
	return ConnectionList_Iterator( m_CollConnectionSharedPtr, m_CollConnectionSharedPtr->begin() );
}
	
//----------------------------------------------------------------------------
//! \throw no-throw
//! \return iterator to position beyond the last element of ConnectionList
ConnectionList_Iterator ManagerImpl::end () const 
{
	return ConnectionList_Iterator( m_CollConnectionSharedPtr, m_CollConnectionSharedPtr->end() );
}

//----------------------------------------------------------------------------
//! \throw OutOfRangeIteratorException
//! \param it iterator of CollConnection 
//! \param connId reference to VarString, set to application's unique connection id
//! \return database information
const DatabaseInfo & ManagerImpl::getConnInfo( const ConnectionList_Iterator it, VarString & connId ) const
{
	if( end() == it )
	{		
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.db.sql.ManagerImpl.getConnInfo()", 
							"Iterator of managerimpl CollConnection is at the end.", 
							__FILE__, __LINE__)
		      );
	}
	
	connId.format( "connect_0x%0*lx", sizeof(size_t) * 2, it->first );

	return it->second.lock()->getDbInfo();	
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \param it iterator of CollConnection 
//! \retval true if given connection is open
//! \retval false if given connection is not open
bool ManagerImpl::isOpen( const ConnectionList_Iterator it ) const
{
	if( end() == it )
	{	
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.db.sql.ManagerImpl.isOpen()", 
				            "Iterator of managerimpl CollConnection is at the end.", 
							__FILE__, __LINE__)
			  );
	}

	return it->second.lock()->isOpen();	
}
//----------------------------------------------------------------------------
//! \throw no-throw
//! \return number of Connections in ConnectionList
ConnectionListSize ManagerImpl::size()	const
{
	return m_CollConnectionSharedPtr->size();
}
	
//----------------------------------------------------------------------------
//! \throw no-throw
//! \return number of open Connections in ConnectionList
ConnectionListSize ManagerImpl::sizeOpen() const
{
	ConnectionListSize sizeOp = 0;
	
	CollConnection::const_iterator	citEnd = m_CollConnectionSharedPtr->end();
	CollConnection::const_iterator	citConn;
	
	for ( citConn = m_CollConnectionSharedPtr->begin(); citEnd != citConn; ++citConn )
	{
		if (true == citConn->second.lock()->isOpen())
			++sizeOp;
	}
	
	return sizeOp;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

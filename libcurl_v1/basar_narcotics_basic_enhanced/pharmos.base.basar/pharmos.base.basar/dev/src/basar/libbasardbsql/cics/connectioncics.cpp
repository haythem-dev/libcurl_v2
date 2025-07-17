/*
 * @file ConnectionCics.cpp
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */
//------------------------------------------------------------------------------

#include "sqlexcepthandlercics.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasardbsql_statementref.h"
#include "libbasardbsql_resultsetref.h"
#include "libbasardbsql_exceptions.h"
#include "libbasardbsql_preparedstatementref.h"
#include "statementcics.h"
#include "connectioncics.h"

//----------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \return shared_ptr to ConnectionCics instance
ConnectionSharedPtr ConnectionCics::create(const DatabaseInfo & rDbInfo ,
										   bool                 openConn)
{
	ConnectionSharedPtr pShared( new ConnectionCics(rDbInfo, openConn));
	pShared->setWeakPtr(pShared);

	return pShared;
}

//----------------------------------------------------------------------------
//! no transactions enabled at the moment.
ConnectionCics::ConnectionCics ( const DatabaseInfo & rDbInfo ,
							     bool                 openConn) 
							   : Connection       (rDbInfo),
							     m_connInfo       (0),
								 m_hasTransactions(false)
{
	initConnInfo();

	m_SqlExceptHnd = SqlExceptHandlerCics::create();

	if (openConn)
		open();
}

//----------------------------------------------------------------------------
ConnectionCics::~ConnectionCics()
{
	try
	{
		close();
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::connCics(), "unknown exception");
		assert(0);
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void  ConnectionCics::initConnInfo()
{
	DatabaseInfo dbinfo = m_Database.getDbInfo();

	m_connInfo.m_slot  = dbinfo.connectionId;

	if (0 < dbinfo.dbServer.length())
	{
		m_connInfo.m_config   = dbinfo.dbServer;
		m_connInfo.m_custconn = dbinfo.cicsCustom;
		m_connInfo.m_user     = dbinfo.user;
		m_connInfo.m_passwd   = dbinfo.passwd;
	}
}

// ---------------------------------------------------------------------------
//! \throw InvalidStateException if opened twice or session for slot 
//!        already exists. Assume that process manages its own slots 
//!        (tcaccess.ini: inProcess=1). So a slot must not be opened 
//!        several times  
void ConnectionCics::open()
{
	ConstString func = "basar.db.sql.ConnectionCics.open";

	if ( isOpen())
		throw InvalidStateException(
                ExceptInfo(func, "isOpen()",__FILE__, __LINE__));

	CicsDefinitions::openSession(m_connInfo);

	m_Database.m_DbInfo.database = 
        CicsDefinitions::getSessionCicsName(m_connInfo);

    // save found slot
	m_Database.m_DbInfo.connectionId = 
        static_cast<Int32>(m_connInfo.m_slot);	

	Connection::open();

    BLOG_DEBUG_STREAM(
        LoggerPool::connCics(),
        "connection "
        << m_ConnId
        << " to "
        << m_Database.getDbInfo().dbServer
        << "@"
        << m_connInfo.m_slot
        << " opened");

}

// ---------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionCics::close()
{ 
	if (!isOpen())
        return;

	if (m_InTransaction)
	    rollback();

	CicsDefinitions::closeSession(m_connInfo);

	Connection::close();

    BLOG_DEBUG_STREAM(
        LoggerPool::connCics(),
        "connection "
        << m_ConnId
        << " to "
        << m_Database.getDbInfo().dbServer
        << "@"
        << m_connInfo.m_slot
        << " closed");
}

//----------------------------------------------------------------------------
//!	\retval true  if connection is technically opened
//! \retval false if connection is technically close/not opened
//! \throw  no-throw
//!
bool ConnectionCics::isValid()
{
	if (!isOpen())
		return false;

	ConstString  SQL  = "SELECT 1 FROM system.systab WHERE 1 = 0;";
	StatementRef stmt = createStatement();

	try
	{
		ResultsetRef result = stmt.executeSingleQuery(SQL);	
	}
	catch (const DbException &)
	{
        BLOG_ERROR_STREAM(
            LoggerPool::connCics(),
            "connection "
            << m_ConnId
            << " to "
            << m_Database.getDbInfo().dbServer
            << "@"
            << m_connInfo.m_slot
            << " not longer valid: checked with <"
            << SQL
            << ">");

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw InvalidStateException
//!
void ConnectionCics::begin()
{
	if (!m_hasTransactions)
    {
		throw InvalidStateException(
                ExceptInfo(
                    "basar.db.sql.ConnectionCics::begin()", 
					"Transaction not possible, uses autocommit" ,
					__FILE__, __LINE__));
    }

	CicsStatementInfo statementInfo(m_connInfo,  "nocommit");
	CicsDefinitions::startAndExec(statementInfo);

	Connection::begin();
}

//----------------------------------------------------------------------------
//! \throw InvalidStateException
//!
void ConnectionCics::commit()
{
	if (!m_hasTransactions)
    {
		throw InvalidStateException(
                ExceptInfo(
                    "basar.db.sql.ConnectionCics::commit()", 
                    "Transaction not possible, uses autocommit" ,
                    __FILE__, __LINE__));
    }

	CicsStatementInfo statementInfo(m_connInfo,  "commit");
	CicsDefinitions::startAndExec(statementInfo);

	Connection::commit();
}

//----------------------------------------------------------------------------
//! \throw InvalidStateException
//!
void ConnectionCics::rollback()
{
	if (!m_hasTransactions)
    {
		throw InvalidStateException(
                ExceptInfo( 
                    "basar.db.sql.ConnectionCics::rollback()", 
					"Transaction not possible, uses autocommit",
					__FILE__, __LINE__));
    }

	CicsStatementInfo statementInfo(m_connInfo ,  "rollback");
	CicsDefinitions::startAndExec(statementInfo);

	Connection::rollback();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return statement reference object
StatementRef ConnectionCics::createStatement()
{
	if ( !isOpen() )
    {
		throw InvalidStateException(
                ExceptInfo(	
                    "basar.db.sql.ConnectionCics.createStatement",
                    "!isOpen()",
					__FILE__, __LINE__));
    }

	return StatementRef(StatementCics::create(getThis(), m_connInfo));
}

//----------------------------------------------------------------------------
//! \throw  InvalidStateException
//! \return nothing (always throws)
PreparedStatementRef ConnectionCics::createPreparedStatement(
    const VarString &, 
    CursorTypeEnum   )
{
	// if this virtual method is called 
    // then there doesn't exist any RDBMS specific implementation
	throw InvalidStateException(
            ExceptInfo(
                "basar.db.sql.ConnectionCics.createPreparedStatement",
				"method not implemented in RDBMS specific context",
				__FILE__, __LINE__));
}

//----------------------------------------------------------------------------
// namespaces
}
}
}

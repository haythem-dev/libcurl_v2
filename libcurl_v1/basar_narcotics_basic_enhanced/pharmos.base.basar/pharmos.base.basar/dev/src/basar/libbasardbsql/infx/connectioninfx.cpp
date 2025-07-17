//------------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class for informix
 *  \author Michael Eichenlaub
 *  \date   06.10.2005
 */
//------------------------------------------------------------------------------

#include "connectioninfx.h"

#include "sqlexcepthandlerinfx.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasardbsql_exceptions.h"
#include "libbasardbsql_preparedstatementref.h"
#include "libbasardbsql_statementref.h"
#include "statementinfx.h"
#include "preparedstatementinfx.h"
#include "setenvinfx.h"

//------------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! init static variable for current connection
const ConnectionInfx * ConnectionInfx::s_pCurrent = 0;

//------------------------------------------------------------------------------
//! \return	shared pointer to Database implementation object
//! \throw no-throw
ConnectionSharedPtr	ConnectionInfx::create(const DatabaseInfo & rDbInfo,
									       bool                 openConn)
{
	ConnectionSharedPtr pShared( new ConnectionInfx(rDbInfo));	// create implementation object from heap
	pShared->setWeakPtr(pShared);								// save in weak pointer

	//check here if open was requested -> open throws if it failed
	if (openConn)
	{
		pShared->open();
	}

	return pShared;
}

//------------------------------------------------------------------------------
//! \throw no-throw
ConnectionInfx::ConnectionInfx ( const DatabaseInfo & rDbInfo)	
							   : Connection       (rDbInfo)
{
	m_SqlExceptHnd = SqlExceptHandlerInfx::create();
}

//------------------------------------------------------------------------------
//! \throw no-throw
ConnectionInfx::~ConnectionInfx()
{
	try
	{
		close();
	}
	catch (const basar::Exception& ex)
	{
		LoggerPool::connInfx().forcedLog(
            log4cplus::ERROR_LOG_LEVEL, 
            ex.what(), 
            __FILE__, __LINE__);

        assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::connInfx(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException
//!
void ConnectionInfx::open()
{
	if (true == isOpen())
		throw InvalidStateException(ExceptInfo ("basar.db.sql.ConnectionInfx.open", 
		                            "isOpen() == true", 
									__FILE__, __LINE__));

	DatabaseInfo dbInfo = m_Database.getDbInfo();

	// narrow scope for instance settings => short lifetime and quick restore of old environment values
	{
		SetEnvInfx settings;

        // client, db locale for chosen db
        SetEnvInfx::adjustEnv(m_Database.getDbInfo());

		switch (m_Database.getConnType())
		{
		case Database::CONNTYPE_USER_DEFAULT:
			infxConnectDb(
                getConnectStr().c_str(),
                m_ConnId       .c_str(),
                dbInfo.concurrTransaction);
			break;

		case Database::CONNTYPE_USER_SET:
		default:
			infxConnectDbUser(
                getConnectStr().c_str(),
                m_ConnId       .c_str(),
                dbInfo.user    .c_str(),
                dbInfo.passwd  .c_str(),
                dbInfo.concurrTransaction);
			break;
		}
	}

	Connection::open();
	
	saveCurrent();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s opened: %s connected to %s",
			       m_ConnId.c_str(),
				   m_Database.getDbInfo().user.c_str(),
				   getConnectStr()            .c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//!	\retval true  if connection is technically opened
//! \retval false if connection is technically close/not opened
//! \throw  no-throw
bool ConnectionInfx::isValid()
{
	if (!isOpen())
		return false;

	ConstString SQL = "SELECT 1 FROM syscolumns WHERE 0 = 1;";

	StatementRef  stmt   = createStatement();

	try
	{
		ResultsetRef result = stmt.executeSingleQuery(SQL);	
	}
	catch (const DbException &)
	{
        if (LoggerPool::connInfx().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("connection %s to %s not longer valid: checked with <%s>", 
						m_ConnId       .c_str(),
						getConnectStr().c_str(),
						SQL);
            LoggerPool::connInfx().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionInfx::close()
{	
	if (!isOpen())	// nothing to do
		return;

	setCurrent		();

	if (m_InTransaction)
		rollback(); //!< if there is still an open transaction at this moment, then roll back !

	infxDisconnectDb(m_ConnId.c_str());

	Connection::close();

	if ( getCurrent() == this )
		resetCurrent();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s closed: %s disconnected from %s",
			       m_ConnId.c_str(),
				   m_Database.getDbInfo().user.c_str(),
				   getConnectStr()            .c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//!	\return created statement reference object
//! \throw no-throw
StatementRef ConnectionInfx::createStatement()
{
	return StatementRef(StatementInfx::create(getThis())); 
}

//------------------------------------------------------------------------------
//!	\return created prepared statement reference object
//! \throw no-throw
PreparedStatementRef ConnectionInfx::createPreparedStatement(const VarString & rSql       ,
															 CursorTypeEnum	   eCursorType)
{
	return PreparedStatementRef(PreparedStatementInfx::create(getThis()  , 
															  rSql       , 
															  eCursorType)); 
}

//------------------------------------------------------------------------------
//!	\return created prepared statement for update/delete cursor 
//! \throw no-throw
PreparedStatementRef ConnectionInfx::createPrepRowStmt (const VarString    & rSql   ,
													    PreparedStatementRef stmtSel)
{
	const PreparedStatementInfx & rStmtInfx = 
		static_cast < const PreparedStatementInfx & > (* (stmtSel.getImpl()) );

	return PreparedStatementRef(PreparedStatementInfx::createRowStmt (getThis(), 
																	  rSql     , 
																	  rStmtInfx)); 
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionInfx::begin()
{
	ConstString func = "basar.db.sql.ConnectionInfx.begin";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxBeginwork ();
	checkSqlState (func, __FILE__, __LINE__);

	Connection::begin();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s begin work",
			       m_ConnId.c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionInfx::commit()
{
	ConstString func = "basar.db.sql.ConnectionInfx.commit";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxCommitwork();
	checkSqlState (func, __FILE__, __LINE__);

	Connection::commit();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s commit work",
			       m_ConnId.c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionInfx::rollback()
{
	ConstString func = "basar.db.sql.ConnectionInfx.rollback";

	checkConnState  (func, __FILE__, __LINE__);
	setCurrent	    ();
	infxRollbackwork();
	checkSqlState   (func, __FILE__, __LINE__);

	Connection::rollback();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s rollback work",
			       m_ConnId.c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \param eLevel	   transaction isolation level
//! \param retainLocks true : RETAIN UPDATE LOCKS for SELECT ... FOR UPDATE \n false: no retain
//! \throw no-throw
void ConnectionInfx::setIsolationLevel(basar::IsolationLevelEnum eLevel     ,
									   bool					     retainLocks)
{
	ConstString func = "basar.db.sql.ConnectionInfx.setIsolationLevel";

	checkConnState(func, __FILE__, __LINE__);

	InfxIsoLevelEnum eInfxLevel;

	// level mapping
	switch (eLevel)
	{
	case INFX_COMITTEDREAD_LASTCOMMITTED:
		eInfxLevel = COMITTEDREAD_LASTCOMMITTED;
		break;

	case ANSI_READCOMMITTED:	// fall through
	case INFX_COMITTEDREAD:
		eInfxLevel = COMITTEDREAD;
		break;

	case INFX_CURSORSTABILITY:
		eInfxLevel = CURSORSTABILITY;
		break;

	case ANSI_REPEATABLEREAD:	// fall through
	case ANSI_SERIALIZABLE:
	case INFX_REPEATABLEREAD:
		eInfxLevel = REPEATABLEREAD;
		break;

	case INFX_DIRTYREAD_WITHWARNING:
		eInfxLevel = DIRTYREAD_WITHWARNING;
		break;

	case ANSI_READUNCOMMITTED:	// fall through
	case INFX_DIRTYREAD:
	default:
		eInfxLevel = DIRTYREAD;
		break;
	}

	setCurrent	   ();
	infxSetIsoLevel(eInfxLevel, retainLocks);
	checkSqlState  (func, __FILE__, __LINE__);

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s setIsolationLevel(%d)",
			       m_ConnId.c_str(),
				   eLevel      );
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \param  eMode mode for accessing locked rows or tables
//! \param  secs  maximum number of seconds that process waits for lock to be 
//!         released before issuing an error
//! \throw  no-throw
void ConnectionInfx::setLockModeWait(basar::LockModeEnum eMode,
			  				         UInt32              secs )
{
	ConstString func = "basar.db.sql.ConnectionInfx.setLockModeWait";

	checkConnState(func, __FILE__, __LINE__);

	InfxLockModeEnum eInfxMode;

	switch (eMode)
	{
	case WAIT:
		eInfxMode = ::WAIT;
		break;

	case WAITSECS:
		eInfxMode = ::WAITSECS;
		break;

	case NOTWAIT:	// fall through
	default:
		eInfxMode = ::NOTWAIT;
		break;
	}

	setCurrent	   ();
	infxSetLockMode(eInfxMode, secs);
	checkSqlState  (func, __FILE__, __LINE__);

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s infxSetLockMode(%d,%d)",
			       m_ConnId.c_str(),
				   eInfxMode,
				   secs);
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ConnectionInfx::setCurrent()
{	
	if (getCurrent() == this)
		return;		// am I current? -> nothing to do

	infxSetCurrentDb(m_ConnId.c_str());
	checkSqlState   ("basar.db.sql.ConnectionInfx.setCurrent", __FILE__, __LINE__);

	saveCurrent();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s infxSetCurrentDb",
			       m_ConnId.c_str());
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return pointer to current connection
const ConnectionInfx * ConnectionInfx::getCurrent()
{
	return s_pCurrent;
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ConnectionInfx::saveCurrent()
{
	s_pCurrent = this;
}

//------------------------------------------------------------------------------
//! \throw no-throw 
void ConnectionInfx::resetCurrent()
{
	s_pCurrent = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! 
void ConnectionInfx::setFetBufSize (Int32 size)
{
	infxSetFetBufSize(size);

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s setFetBufSize(%ld)",
			       m_ConnId.c_str(),
				   size);
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return Int32 representing fetch buffer size
Int32 ConnectionInfx::getFetBufSize()
{
	Int32 size = infxGetFetBufSize();

	if (LoggerPool::connInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s getFetBufSize = %ld",
			       m_ConnId.c_str(),
				   size);
		LoggerPool::connInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return size;
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset multiple rows Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   21.08.2005
 */
//----------------------------------------------------------------------------

#include "resultsetinfxmulti.h"

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasardbsql_exceptions.h"
#include "infxstring.h"

#include "esqlc.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to resultset base implementation object
//! \throw  no-throw
ResultsetSharedPtr ResultsetInfxMulti::create (	ConnectionSharedPtr pConn            ,
												const VarString &   rStmtId          ,
												const VarString &   rCursorId	     ,
												CursorTypeEnum	    eCursorType      ,
												::sqlda *	        pDescIn /* = 0 */)
{
	return ResultsetSharedPtr(new ResultsetInfxMulti (pConn      , 
													  rStmtId    ,
													  rCursorId  , 
													  eCursorType,
													  pDescIn    ));
}

//------------------------------------------------------------------------------
//! \note  open cursor is time-critical
//! \throw no-throw
ResultsetInfxMulti::ResultsetInfxMulti	( ConnectionSharedPtr pConn      ,
								          const VarString &   rStmtId    ,
										  const VarString &   rCursorId	 ,
								          CursorTypeEnum	  eCursorType,
										  ::sqlda *	          pDescIn    )
								        : ResultsetInfx(pConn, rStmtId),
										  m_CursorId   (rCursorId),
										  m_eCursorType(eCursorType),
										  m_pDescIn    (pDescIn)
{
	BLOG_TRACE_METHOD(LoggerPool::resSetInfxMlt(), 
					  "basar.db.sql.ResultsetInfxMulti.ResultsetInfxMulti()");

	declareCurs ();	
	openCurs	();	

	m_eState = STATE_OPENED;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetInfxMulti::~ResultsetInfxMulti()
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
		BLOG_ERROR(LoggerPool::resSetInfxMlt(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::close()
{
	if ( ! isOpen() )
		return;

	closeCurs();				// close cursor
	freeCurs ();				// free cursor

	ResultsetInfx::close();		// free descriptor output

	m_eState = STATE_INVALID;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::reopen()
{
	if ( ! isOpen() )	// cursor was closed and freed
		declareCurs();

	// closing Informix cursor not necessary: see Informix syntax manual:
	// cursor is automatically closed
	openCurs();

	m_eState = STATE_OPENED;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::openCurs()
{
	ConstString func = "basar.db.sql.ResultsetInfxMulti.openCurs";

	checkConnState (func, __FILE__, __LINE__);
	setCurrent	   ();
	privateOpenCurs();
	checkSqlState  (func, __FILE__, __LINE__);

	if (LoggerPool::resSetInfxMlt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: open cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::resSetInfxMlt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ePos        = POS_BEFORE_FIRST;
	m_FetchedRows = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::closeCurs()
{
	ConstString func = "basar.db.sql.ResultsetInfxMulti.closeCurs";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxCloseCurs (m_CursorId.c_str());
	checkSqlState (func, __FILE__, __LINE__);
	
	if (LoggerPool::resSetInfxMlt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s - %d rows fetched: close cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str(),
					m_FetchedRows);
		LoggerPool::resSetInfxMlt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ePos        = POS_INVALID;
	m_FetchedRows = 0;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//!
void ResultsetInfxMulti::declareCurs()
{
	ConstString func = "basar.db.sql.ResultsetInfxMulti.declareCurs";

	infxDeclCurs  (m_CursorId.c_str(), 
		           m_StmtId  .c_str(),
				   (m_eCursorType == CURS_HOLD_OVER_COMMIT) ? CURSOR_WITH_HOLD 
														  : CURSOR_WITHOUT_HOLD);
	checkSqlState (func, __FILE__, __LINE__);

	if (LoggerPool::resSetInfxMlt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: declare cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::resSetInfxMlt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::freeCurs()
{
	ConstString func = "basar.db.sql.ResultsetInfxMulti.freeCurs";

	infxFreeCurs  (m_CursorId.c_str());		// free cursor
	checkSqlState (func, __FILE__, __LINE__);

	if (LoggerPool::resSetInfxMlt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: free cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::resSetInfxMlt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return number of rows affected by statement (update/delete)
Int32 ResultsetInfxMulti::execCurrentRow(const VarString & rSql)
{
	VarString func = LoggerPool::resSetInfxMltExecCurrentRow().getName();

	VarString statement;
	statement.format("%s WHERE CURRENT OF %s", rSql.c_str(), m_CursorId.c_str());

	const InfxString infxSql (statement);

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxExecImm	  (infxSql.out());
	checkSqlState (func, __FILE__, __LINE__);

	Int32 rows = infxGetCountDiagRows();

	if (LoggerPool::resSetInfxMltExecCurrentRow().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: %d rows affected by executing <%s>",
				   m_StmtId  .c_str(),
				   m_CursorId.c_str(),
				   rows,
				   infxSql.out());
		LoggerPool::resSetInfxMltExecCurrentRow().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return rows;
}

//----------------------------------------------------------------------------
//! \note  time-critical; avoid unnecessary statement, instantiations, etc.!
//! \retval true  if new current row is valid
//! \retval false if there are no more rows
//! \throw no-throw
//!
bool ResultsetInfxMulti::next()
{
	switch (m_ePos)
	{
	case POS_AFTER_LAST:	// new fetch not necessary
		if (LoggerPool::resSetInfxMltNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s, %s - %d rows fetched: m_ePos == POS_AFTER_LAST(%d) -> m_ePos = POS_AFTER_LAST(%d), return false",
						m_StmtId  .c_str(),
						m_CursorId.c_str(),
						m_FetchedRows,
						m_ePos,
						POS_AFTER_LAST);
			LoggerPool::resSetInfxMltNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		m_ePos  = POS_AFTER_LAST;
		return false;

	default:
		break;
	}

	ConstString func = "basar.db.sql.ResultsetInfxMulti.next";

	checkConnState    (func, __FILE__, __LINE__);
	setCurrent	      ();
	infxFetchCursInto (m_CursorId.c_str(), m_pDescOut);

    if ( false == infxCheckLocBufferAllocSucceeded( m_pDescOut ))
    {
        const basar::VarString msg = "alloc of large object buffer did not succeed.";
        BLOG_ERROR_STREAM(LoggerPool::resSetInfxMlt(), msg);

        throw DbExceptionInfx( ExceptInfo(func, msg, __FILE__, __LINE__));
    }

	if (true == isSqlStateNotFound(func, __FILE__, __LINE__))
	{
		if (LoggerPool::resSetInfxMltNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s, %s - %d rows fetched: isSqlStateNotFound == true",
					    m_StmtId  .c_str(),
						m_CursorId.c_str(),
						m_FetchedRows);
			LoggerPool::resSetInfxMltNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		m_ePos = POS_AFTER_LAST;
		return false;
	}

	++m_FetchedRows;
	m_ePos = POS_VALID;

    //request BLOB buffer reallocation
    infxSetDescAlloc( m_pDescOut );

	if (LoggerPool::resSetInfxMltNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s - %d rows fetched: isSqlStateNotFound == false",
					m_StmtId  .c_str(),
					m_CursorId.c_str(),
					m_FetchedRows);
		LoggerPool::resSetInfxMltNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxMulti::privateOpenCurs()
{
	if (0 == m_pDescIn)
		infxOpenCurs(m_CursorId.c_str());
	else
		infxOpenCursUsing(m_CursorId.c_str(), m_pDescIn);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

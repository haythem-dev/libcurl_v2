//----------------------------------------------------------------------------
/*! \file
 *  \brief  insertset rows informix implementation class
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//----------------------------------------------------------------------------

#include "insertsetinfx.h"

#include "libbasarcmnutil_exceptions.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "connectioninfx.h"

#include <esqlc.h>

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to insertset base implementation object
//! \throw  no-throw
InsertsetSharedPtr InsertsetInfx::create(ConnectionSharedPtr pConn             ,
									     const VarString &   rStmtId           ,
										 const VarString &   rCursorId	       ,
										 CursorTypeEnum	     eCursorType       ,
										 ::sqlda *	         pDescIn /* = 0 */ )
{
	return InsertsetSharedPtr(new InsertsetInfx(pConn, 
												rStmtId,
												rCursorId,
												eCursorType, 
												pDescIn));
}

//------------------------------------------------------------------------------
//! \note  open cursor is time-critical
//! \throw no-throw
InsertsetInfx::InsertsetInfx	( ConnectionSharedPtr pConn      ,
								  const VarString &   rStmtId    ,
								  const VarString &   rCursorId	 ,
								  CursorTypeEnum	  eCursorType,
								  ::sqlda *	          pDescIn)
								: Insertset			(pConn    ),
		  					      m_StmtId			(rStmtId  ),
								  m_CursorId        (rCursorId),
								  m_pDescIn         (pDescIn  )

{
	declareCurs	     (eCursorType);	// declare cursor
	openCurs	     ();			// open cursor

	m_eState = STATE_OPENED;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
InsertsetInfx::~InsertsetInfx()
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
		BLOG_ERROR(LoggerPool::insSetInfx(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::close()
{
	if ( ! isOpen() )
		return;

	closeCurs();	// close cursor
	freeCurs ();	// free cursor

	m_eState = STATE_INVALID;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::openCurs()
{
	ConstString func = "basar.db.sql.InsertsetInfx.openCurs";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxOpenCurs  (m_CursorId.c_str());
	checkSqlState (func, __FILE__, __LINE__);

	if (LoggerPool::insSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: open cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::insSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_InsertedRows = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::closeCurs()
{
	ConstString func = "basar.db.sql.InsertsetInfx.close";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxCloseCurs (m_CursorId.c_str());
	checkSqlState (func, __FILE__, __LINE__);
	
	if (LoggerPool::insSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s - %d rows fetched: close cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str(),
					m_InsertedRows);
		LoggerPool::insSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_InsertedRows = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::declareCurs(CursorTypeEnum eCursorType)
{
	ConstString func = "basar.db.sql.InsertsetInfx.declareCurs";

	infxDeclCurs  (m_CursorId.c_str(), 
		           m_StmtId  .c_str(), 
				   (eCursorType == CURS_HOLD_OVER_COMMIT) ? CURSOR_WITH_HOLD 
				                                          : CURSOR_WITHOUT_HOLD);
	checkSqlState (func, __FILE__, __LINE__);

	if (LoggerPool::insSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: declare cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::insSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::freeCurs()
{
	ConstString func = "basar.db.sql.InsertsetInfx.freeCurs";

	infxFreeCurs  (m_CursorId.c_str());		// free cursor
	checkSqlState (func, __FILE__, __LINE__);

	if (LoggerPool::insSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s: free cursor",
					m_StmtId  .c_str(),
					m_CursorId.c_str());
		LoggerPool::insSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return number of inserted rows in database
UInt32 InsertsetInfx::flushRows()
{
	ConstString func = "basar.db.sql.InsertsetInfx.flushRows";

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	infxFlushCurs (m_CursorId.c_str());
	checkSqlState (func, __FILE__, __LINE__);

	Int32 rows = infxGetCountDiagRows();

	if (LoggerPool::insSetInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s - flushRows(): flushed rows %d (totally %d)",
					m_StmtId  .c_str(),
					m_CursorId.c_str(),
					rows,
					m_InsertedRows);
		LoggerPool::insSetInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return rows;
}

//----------------------------------------------------------------------------
//! \throw no-throw
void InsertsetInfx::insertRow()
{
	VarString func = LoggerPool::insSetInfxInsertRow().getName();

	checkConnState  (func, __FILE__, __LINE__);
	setCurrent	    ();
	privateInsertRow();
	checkSqlState   (func, __FILE__, __LINE__);

	++m_InsertedRows;

	if (LoggerPool::insSetInfxInsertRow().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s, %s - insertRow(): inserted rows %d",
					m_StmtId  .c_str(),
					m_CursorId.c_str(),
					m_InsertedRows);
		LoggerPool::insSetInfxInsertRow().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::privateInsertRow()
{
	if (0 == m_pDescIn)
		infxPutCurs(m_CursorId.c_str());
	else
		infxPutCursUsing(m_CursorId.c_str(), m_pDescIn);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetInfx::setCurrent()
{	
	static_cast<ConnectionInfx *>(m_pConn.get())->setCurrent();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

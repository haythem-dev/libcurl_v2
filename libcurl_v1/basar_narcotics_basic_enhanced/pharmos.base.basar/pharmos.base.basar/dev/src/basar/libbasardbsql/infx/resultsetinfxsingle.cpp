//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset single row Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   20.08.2005
 */
//----------------------------------------------------------------------------

#include "resultsetinfxsingle.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"
#include "sqldiagnosticsinfx.h"

#include "esqlc.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to resultset base implementation object
//! \throw  no-throw
ResultsetSharedPtr ResultsetInfxSingle::create (ConnectionSharedPtr pConn  ,
									            const VarString &   rStmtId,
												::sqlda *	        pDescIn /* = 0 */ )
{
	return ResultsetSharedPtr(new ResultsetInfxSingle(pConn, rStmtId, pDescIn));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetInfxSingle::ResultsetInfxSingle ( ConnectionSharedPtr pConn  ,
										   const VarString &   rStmtId,
										   ::sqlda *	       pDescIn)
								         : ResultsetInfx(pConn, rStmtId),
										   m_pDescIn    (pDescIn)
{
	BLOG_TRACE_METHOD(LoggerPool::resSetInfxSgl(), "basar.db.sql.ResultsetInfxSingle.ResultsetInfxSingle()");

	m_eState      = STATE_OPENED;
	m_ePos        = POS_BEFORE_FIRST;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetInfxSingle::~ResultsetInfxSingle()
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
		BLOG_ERROR(LoggerPool::resSetInfxSgl(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxSingle::close()
{
	if ( ! isOpen() )
		return;

	ResultsetInfx::close();		// free descriptor output

	m_eState      = STATE_INVALID;
	m_ePos        = POS_INVALID;
	m_FetchedRows = 0;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxSingle::reopen()
{
	if ( ! isOpen() )	// cursor must be opened before
		m_eState  = STATE_OPENED;

	m_ePos        = POS_BEFORE_FIRST;
	m_FetchedRows = 0;
}

//----------------------------------------------------------------------------
//! \retval true  if new current row is valid
//! \retval false if there are no more rows
//! \throw no-throw
bool ResultsetInfxSingle::next()
{
	ConstString func = "basar.db.sql.ResultsetInfxSingle.next()";

	BLOG_TRACE_METHOD(LoggerPool::resSetInfxSglNext(), func);

	switch (m_ePos)
	{
	case POS_AFTER_LAST:	// new fetch not necessary
	case POS_VALID:			// fetch only once!
		if (LoggerPool::resSetInfxSglNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s - %d rows fetched: m_ePos == %d -> m_ePos = POS_AFTER_LAST(%d), return false",
						m_StmtId.c_str(),
						m_FetchedRows,
						m_ePos,
						POS_AFTER_LAST);
			LoggerPool::resSetInfxSglNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		m_ePos  = POS_AFTER_LAST;

		return false;

	default:
		if (LoggerPool::resSetInfxSglNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s - %d rows fetched: m_ePos == %d",
						m_StmtId.c_str(),
						m_FetchedRows,
						m_ePos);
			LoggerPool::resSetInfxSglNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		break;
	}

	checkConnState(func, __FILE__, __LINE__);
	setCurrent	  ();
	privateExec	  ();

	if (LoggerPool::resSetInfxSglNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("called privateExec()\n"
				    "    dump m_pDescOut\n"
					"%s",
					SqlDiagnosticsInfx::getDescDump(m_pDescOut).c_str());

		LoggerPool::resSetInfxSglNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if (true == isSqlStateNotFound(func, __FILE__, __LINE__))
	{
		if (LoggerPool::resSetInfxSglNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("%s - %d rows fetched: isSqlStateNotFound == true",
					    m_StmtId.c_str(),
						m_FetchedRows);
			LoggerPool::resSetInfxSglNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		m_ePos = POS_AFTER_LAST;

		return false;
	}
	
	++m_FetchedRows;
	m_ePos = POS_VALID;

	if (LoggerPool::resSetInfxSglNext().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s - %d rows fetched: isSqlStateNotFound == false",
					m_StmtId.c_str(),
					m_FetchedRows);
		LoggerPool::resSetInfxSglNext().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetInfxSingle::privateExec()
{
	if (0 == m_pDescIn)
		infxExecInto(m_StmtId.c_str(), m_pDescOut);
	else
		infxExecIntoUsing(m_StmtId.c_str(), m_pDescOut, m_pDescIn);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
*  \brief  implementation class for informix statements
*  \author Michael Eichenlaub
*  \date   11.08.2005
*/
//----------------------------------------------------------------------------

#include "statementinfx.h"

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_decimal.h"
#include "connection.h"
#include "connectioninfx.h"
#include "resultsetinfxmulti.h"
#include "resultsetinfxsingle.h"
#include "insertsetinfx.h"
#include "infxstring.h"

#include "esqlc.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to statement implementation object
//! \throw  no-throw
StatementSharedPtr StatementInfx::create(ConnectionSharedPtr pConn)
{
    return StatementSharedPtr(new StatementInfx(pConn));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
StatementInfx::StatementInfx ( ConnectionSharedPtr pConn )
    : Statement(pConn)
{
    // each byte of address is represented by 2-digit hexadecimal number
    m_StmtId.format("stmt_0x%0*lx", sizeof(size_t) * 2, this);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
StatementInfx::~StatementInfx ()
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
        BLOG_ERROR(LoggerPool::stmtInfx(), "unknown exception");
        assert(0);
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void StatementInfx::close()
{
    if ( ! isOpen() )
        return;

    Statement::close();

    ConstString func = "basar.db.sql.StatementInfx.close";

    checkConnState(func, __FILE__, __LINE__);
    setCurrent	  ();
    infxFreeStmt  (m_StmtId.c_str());
    checkSqlState (func, __FILE__, __LINE__);

    m_Open = false;

    if (LoggerPool::stmtInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s for %s@%s closed",
            m_StmtId.c_str(),
            m_pConn.get()->getDbInfo().database.c_str(),
            m_pConn.get()->getDbInfo().dbServer.c_str());
        LoggerPool::stmtInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return reference to created resultset
ResultsetRef StatementInfx::executeQuery(const VarString & rSql, 
										 CursorTypeEnum	   eCursorType)
{
    VarString func = LoggerPool::stmtInfxExecQuery().getName();

    close();

	const InfxString infxSql (rSql);

    try
    {
        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxPrepStmt  (m_StmtId.c_str(), infxSql.out());
        checkSqlState (func, __FILE__, __LINE__);

        m_ResultsetRef = ResultsetRef(ResultsetInfxMulti::create(m_pConn, 
																	m_StmtId,
																	getCursorId(CURSORID_RESULTSET),
																	eCursorType));
    }
    catch (const Exception & )
    {
        if (LoggerPool::stmtInfxExecQuery().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: executed query <%s>", m_StmtId.c_str(), infxSql.out());
            LoggerPool::stmtInfxExecQuery().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        throw;
    }

    m_Open = true;

    if (LoggerPool::stmtInfxExecQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed query <%s>",
            m_StmtId.c_str(),
            infxSql.out());
        LoggerPool::stmtInfxExecQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_ResultsetRef;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return reference to created resultset
ResultsetRef StatementInfx::executeSingleQuery(const VarString & rSql)
{
    VarString func = LoggerPool::stmtInfxExecSingleQuery().getName();

    close();

	const InfxString infxSql (rSql);

    try
    {
        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxPrepStmt  (m_StmtId.c_str(), infxSql.out());
        checkSqlState (func, __FILE__, __LINE__);

        m_ResultsetRef = ResultsetRef(ResultsetInfxSingle::create(m_pConn, m_StmtId));
    }
    catch (const Exception & )
    {
        if (LoggerPool::stmtInfxExecSingleQuery().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: executed single query <%s>", m_StmtId.c_str(), infxSql.out());
            LoggerPool::stmtInfxExecSingleQuery().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        throw;
    }

    m_Open = true;

    if (LoggerPool::stmtInfxExecSingleQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed single query <%s>",
            m_StmtId.c_str(),
            infxSql.out());
        LoggerPool::stmtInfxExecSingleQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_ResultsetRef;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return reference to created insertset
InsertsetRef StatementInfx::executeInsert(const VarString & rSql, CursorTypeEnum eCursorType)
{
    VarString func = LoggerPool::stmtInfxExecInsert().getName();

    close();

	const InfxString infxSql (rSql);

    try
    {
        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxPrepStmt  (m_StmtId.c_str(), infxSql.out());
        checkSqlState (func, __FILE__, __LINE__);

        m_InsertsetRef = InsertsetRef(InsertsetInfx::create(m_pConn, 
															m_StmtId, 
															getCursorId(CURSORID_INSERTSET),
															eCursorType));
    }
    catch (const Exception & )
    {
        if (LoggerPool::stmtInfxExecInsert().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: executed insert <%s>",
                m_StmtId.c_str(),
                infxSql.out());
            LoggerPool::stmtInfxExecInsert().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        throw;
    }

    m_Open = true;

    if (LoggerPool::stmtInfxExecInsert().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed insert <%s>",
            m_StmtId.c_str(),
            infxSql.out());
        LoggerPool::stmtInfxExecInsert().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_InsertsetRef;
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return ExecuteReturnInfo containing number of rows affected by statement (update, delete, insert)
ExecuteReturnInfo StatementInfx::execute(const VarString & rSql, bool flagExcept)
{
    VarString func = LoggerPool::stmtInfxExec().getName();

    close         ();

    ExecuteReturnInfo retval;
	const InfxString  infxSql (rSql);

    try
    {
        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxExecImm   (infxSql.out());

        if ( checkSqlState (func, __FILE__, __LINE__, flagExcept) )
        {
            retval.m_affectedRows  = infxGetCountDiagRows();


            if (checkStmtStart(infxSql.out(), "insert"))
            {
                const Int64 lastSerial = infxGetSerialValue();
                if (0 != lastSerial)
                {
                    retval.m_lastSerial.push_back(lastSerial);
                }
            }

            if (LoggerPool::stmtInfxExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;
                msg.format("%s: %d rows affected by executing <%s>",
                    m_StmtId.c_str(),
                    retval.m_affectedRows,
                    infxSql.out());
                LoggerPool::stmtInfxExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
            }
        }
        else
        {
            retval.m_error = getLastError();
            retval.m_DBError = getRawDBError();
            if (LoggerPool::stmtInfxExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
            {
                VarString msg;
                msg.format("%s: error no %d returned by executing <%s>",
                    m_StmtId.c_str(),
                    retval.m_error,
                    infxSql.out());
                LoggerPool::stmtInfxExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
            }
        }
    }
    catch (const Exception & )
    {
        if (LoggerPool::stmtInfxExec().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: executed <%s>",
                m_StmtId.c_str(),
                infxSql.out());
            LoggerPool::stmtInfxExec().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

		throw;
    }

    return retval; 
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException, InvalidParameterException
//! \return result of aggregate select
basar::Decimal StatementInfx::executeAggregate	(const VarString & rSql)
{
    VarString func = LoggerPool::stmtInfxExecAggregate().getName();

    close();

	const InfxString infxSql (rSql);

    try
    {
        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxPrepStmt  (m_StmtId.c_str(), infxSql.out());
        checkSqlState (func, __FILE__, __LINE__);

        m_ResultsetRef = ResultsetRef(ResultsetInfxSingle::create(m_pConn, m_StmtId));
    }
    catch (const Exception & )
    {
        if (LoggerPool::stmtInfxExecAggregate().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: executed aggregate select <%s>", m_StmtId.c_str(), infxSql.out());
            LoggerPool::stmtInfxExecAggregate().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        throw;
    }

    m_Open = true;

    if (LoggerPool::stmtInfxExecAggregate().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed aggregate select <%s>",
            m_StmtId.c_str(),
            infxSql.out());
        LoggerPool::stmtInfxExecAggregate().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    if ( m_ResultsetRef.getSelectedCols() != 1 )
    {
        basar::VarString msg;
        msg.format("not exactly one column (=> %d) selected in aggregate select <%s>.", 
					m_ResultsetRef.getSelectedCols(),
					infxSql.out());
        throw InvalidStateException(ExceptInfo("basar.db.sql.StatementInfx.executeAggregate", msg, __FILE__, __LINE__));
    }

    m_ResultsetRef.next();

    InfxSqlTypeEnum type = ( static_cast<ResultsetInfx*>(m_ResultsetRef.m_pImpl.get()) )->getType(0);

	switch (type)
	{
	case TYPE_DECIMAL:
        return m_ResultsetRef.getDecimal(0);
		
	case TYPE_DOUBLE:
	    {
    	    Decimal d(m_ResultsetRef.getFloat64(0));
        	return d;
        }
	
	case TYPE_FLOAT:
        {
            Decimal d(m_ResultsetRef.getFloat32(0));
            return d;
        }
	
	case TYPE_INT:
        {
            Decimal d(m_ResultsetRef.getInt32(0));
            return d;
        }
	
	case TYPE_SHORT:
        {
            Decimal d(m_ResultsetRef.getInt16(0));
            return d;
        }

	default:
        {
			if (true)
			{
                basar::VarString msg;
                msg.format("aggregate select <%s> resulted in an invalid aggregate type <%d>. "
							"only numeric results allowed.", 
                        	infxSql.out(), type);
                throw InvalidParameterException(ExceptInfo(	"basar.db.sql.StatementInfx.executeAggregate", 
															msg, 
															__FILE__, __LINE__));
			}

			return Decimal();
		}
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void StatementInfx::setCurrent()
{	
	static_cast<ConnectionInfx *>(m_pConn.get())->setCurrent();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing cursor id for created resultset/insertset
VarString StatementInfx::getCursorId (CursorIdEnum eCursorId)
{
	VarString ret;

	// each byte of address is represented by 2-digit hexadecimal number
	ret.format ("%s_0x%0*lx", 
				(CURSORID_RESULTSET == eCursorId) ? "rescursor" : "inscursor",
				sizeof(size_t) * 2, 
				this);

	return ret;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

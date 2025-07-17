//----------------------------------------------------------------------------
/*! \file
*  \brief  prepared statement informix implementation class
*  \author Roland Kiefert
*  \date   03.11.2005
*/
//----------------------------------------------------------------------------

#include "preparedstatementinfx.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_timespan.h"
#include "libbasarcmnutil_date.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "connection.h"
#include "connectioninfx.h"
#include "resultsetinfxmulti.h"
#include "resultsetinfxsingle.h"
#include "sqldiagnosticsinfx.h"
#include "insertsetinfx.h"
#include "libbasardbsql_preparedstatementref.h"

#include "infxstring.h"

#include "esqlc.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return recommended method name
//! \throw  no-throw
ConstBuffer PreparedStatementInfx::getMethodName(Int32 type)
{
    static ConstString method[] =	// regard enum InfxSqlTypeEnum order in esqlc.h !
    {
        s_METHOD_INVALID,
        s_METHOD_SETSTRING,
        s_METHOD_SETFLOAT64,
        s_METHOD_SETDECIMAL,
        s_METHOD_SETFLOAT32,
        s_METHOD_SETINT32,	
        s_METHOD_SETINT16,	
        s_METHOD_SETDATE,	
        s_METHOD_SETDATETIME,
        s_METHOD_SETTIMESPAN,
        s_METHOD_SETSTRING,
        s_METHOD_SETBINARY,
        s_METHOD_SETINT64,
        s_METHOD_UNKNOWN
    };

    if ((Int32)(sizeof(method)/sizeof(*method)) <= type)
    {
        assert(0);
        return s_METHOD_INVALID;
    }

    return method[type];
}

//------------------------------------------------------------------------------
//! \return	shared pointer to statement implementation object
//! \throw  no-throw
PreparedStatementSharedPtr PreparedStatementInfx::create(
	ConnectionSharedPtr pConn      , 
    const VarString   & rSql       ,
    CursorTypeEnum	    eCursorType)
{
    return PreparedStatementSharedPtr(new PreparedStatementInfx(pConn, 
																rSql, 
																eCursorType));
}

//------------------------------------------------------------------------------
//! \return	shared pointer to statement implementation object
//! \throw  no-throw
PreparedStatementSharedPtr PreparedStatementInfx::createRowStmt(
	ConnectionSharedPtr           pConn  , 
    const VarString             & rSql   ,
    const PreparedStatementInfx & stmtSel)
{
    return PreparedStatementSharedPtr(new PreparedStatementInfx(pConn, 
																rSql, 
																stmtSel));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatementInfx::PreparedStatementInfx ( ConnectionSharedPtr pConn      , 
											   const VarString   & rSql       ,
											   CursorTypeEnum	   eCursorType)
											 : PreparedStatement(pConn, rSql),
											   m_pDescIn        (0          ),
											   m_eCursorType    (eCursorType),
											   m_statement      (rSql)
{
	construct();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatementInfx::PreparedStatementInfx ( ConnectionSharedPtr           pConn  , 
											   const VarString             & rSql   ,
											   const PreparedStatementInfx & stmtSel)
											 : PreparedStatement(pConn, rSql),
											   m_pDescIn        (0          ),
											   m_eCursorType    (CURS_DEFAULT)
{
	m_statement.format("%s WHERE CURRENT OF %s",
		               rSql.c_str(),
					   stmtSel.getCursorId(CURSORID_RESULTSET).c_str());

	construct();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatementInfx::~PreparedStatementInfx ()
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
        BLOG_ERROR(LoggerPool::preparedStmtInfx(), "unknown exception");
        assert(0);
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::close()
{
    if (false == isOpen())
        return;

    PreparedStatement::close();

    infxFreeImplicitDesc(m_pDescIn, DONT_FREE_LOC_BUFFER);

    ConstString func = "basar.db.sql.PreparedStatementInfx.close";

    checkConnState(func, __FILE__, __LINE__);
    setCurrent	  ();
    infxFreeStmt  (m_StmtId.c_str());
    checkSqlState (func, __FILE__, __LINE__);

    m_Open = false;

    if (LoggerPool::preparedStmtInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s for %s@%s closed",
            m_StmtId.c_str(),
            m_pConn.get()->getDbInfo().database.c_str(),
            m_pConn.get()->getDbInfo().dbServer.c_str());
        LoggerPool::preparedStmtInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return reference to created resultset
ResultsetRef PreparedStatementInfx::executeQuery()
{
    VarString func = LoggerPool::preparedStmtInfxExecQuery().getName();

    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in prepared statement %s", 
            m_StmtId.c_str());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "prepared statement <" << m_StmtId << "> is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    if (m_ResultsetRef.isNull())	// not yet instantiated
        m_ResultsetRef = ResultsetRef(ResultsetInfxMulti::create(m_pConn, 
																	m_StmtId, 
																	getCursorId(CURSORID_RESULTSET),
																	m_eCursorType, 
																	m_pDescIn));
    else 
        m_ResultsetRef.reopen();	// reuse cursor select

    if (LoggerPool::preparedStmtInfxExecQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed query",
            m_StmtId.c_str());
        LoggerPool::preparedStmtInfxExecQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_ResultsetRef;
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return reference to created resultset
ResultsetRef PreparedStatementInfx::executeSingleQuery()
{
    VarString func = LoggerPool::preparedStmtInfxExecSingleQuery().getName();

    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in prepared statement %s", m_StmtId.c_str());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "prepared statement <" << m_StmtId << "> is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    if (m_ResultsetRef.isNull())	// not yet instantiated
        m_ResultsetRef = ResultsetRef(ResultsetInfxSingle::create(m_pConn, m_StmtId, m_pDescIn));
    else 
        m_ResultsetRef.reopen();	// reuse singleton select

    if (LoggerPool::preparedStmtInfxExecSingleQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed single query\n"
            "    dump m_pDescIn\n"
            "%s",
            m_StmtId.c_str(),
            SqlDiagnosticsInfx::getDescDump(m_pDescIn).c_str());

        LoggerPool::preparedStmtInfxExecSingleQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_ResultsetRef;
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return reference to created insertset
InsertsetRef PreparedStatementInfx::executeInsert()
{
    VarString func = LoggerPool::preparedStmtInfxExecInsert().getName();

    /* is handled by Informix itself
    if (false == isParamsSet())
    {
		VarString msg;
		msg.format("not all parameters are set in prepared statement %s", 
		m_StmtId.c_str());
		throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }*/


	if (false == isOpen())
	{
		std::ostringstream os;
		os << "prepared statement <" << m_StmtId << "> is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    if (m_InsertsetRef.isNull())	// not yet instantiated
        m_InsertsetRef = InsertsetRef(InsertsetInfx::create(m_pConn, 
															m_StmtId, 
															getCursorId(CURSORID_INSERTSET),
															m_eCursorType, 
															m_pDescIn));
    else 
        m_InsertsetRef.flushRows();

    if (LoggerPool::preparedStmtInfxExecInsert().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: executed insert", 
            m_StmtId.c_str());
        LoggerPool::preparedStmtInfxExecInsert().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_InsertsetRef;
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return number of rows affected by statement (update, delete, insert)
ExecuteReturnInfo PreparedStatementInfx::execute(bool flagExcept)
{
    ExecuteReturnInfo retval;
    VarString func = LoggerPool::preparedStmtInfxExec().getName();

    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in prepared statement %s", 
            m_StmtId.c_str());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "statement " << m_StmtId << " is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    checkConnState(func, __FILE__, __LINE__);
    setCurrent	  ();

    infxExecUsing (m_StmtId.c_str(), m_pDescIn);

    if ( checkSqlState (func, __FILE__, __LINE__, flagExcept) )
    {
        retval.m_affectedRows  = infxGetCountDiagRows();
        if (checkStmtStart(m_statement, "insert"))
        {
            Int64 lastSerial = infxGetSerialValue();
            if (lastSerial != 0)
            {
                retval.m_lastSerial.push_back(infxGetSerialValue());
            }
        }
    }
    else
    {
        retval.m_error = getLastError();
    }

    if (LoggerPool::preparedStmtInfxExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: %d rows affected",
            m_StmtId.c_str(),
            retval.m_affectedRows);
        LoggerPool::preparedStmtInfxExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return retval;
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException, InvalidParameterException
//! \return result of aggregate select
basar::Decimal PreparedStatementInfx::executeAggregate	()
{
    VarString func = LoggerPool::stmtInfxExecAggregate().getName();

    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in prepared statement %s", 
            m_StmtId.c_str());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "statement " << m_StmtId << " is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    checkConnState(func, __FILE__, __LINE__);
    setCurrent	  ();

    if (m_ResultsetRef.isNull())	// not yet instantiated
    {
        m_ResultsetRef = ResultsetRef(ResultsetInfxSingle::create(m_pConn, m_StmtId, m_pDescIn));
    }
    else 
    {
        m_ResultsetRef.reopen();	// reuse singleton select
    }

    if (LoggerPool::stmtInfxExecAggregate().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        std::ostringstream os;
        
        os << m_StmtId << ": executed aggregate select";
        
        LoggerPool::stmtInfxExecAggregate().forcedLog(log4cplus::DEBUG_LOG_LEVEL, os.str(), __FILE__, __LINE__);
    }

    if ( m_ResultsetRef.getSelectedCols() != 1 )
    {
        std::ostringstream os;
        
        os << m_StmtId << ": not exactly one column (=> " << m_ResultsetRef.getSelectedCols() << ") selected";

        throw InvalidStateException(ExceptInfo("basar.db.sql.StatementInfx.executeAggregate", os.str(), __FILE__, __LINE__));
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
                
                std::ostringstream os;
                
                os << m_StmtId << ": aggregate select resulted in an invalid aggregate type <" << type << ">. only numeric results allowed.";

                throw InvalidParameterException(ExceptInfo(	"basar.db.sql.StatementInfx.executeAggregate", 
															os.str(), 
															__FILE__, __LINE__));
			}

			return Decimal();
		}
    }
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//!
void PreparedStatementInfx::checkType ( UInt32          index,
    InfxSqlTypeEnum cmpType,
    ConstString     func,
    ConstString     file,
    UInt32		    line) const
{
    InfxSqlTypeEnum eType = infxGetDescSimpleType(m_pDescIn, index);

    if ( (cmpType == eType) || (TYPE_UNKNOWN == eType) )
        return;

    VarString val;
    val.format("index %ld, name \"%s\", type \"%s\" : use method %s!", 
        index,
        infxGetDescColName (m_pDescIn, index),
        infxGetDescTypeName(m_pDescIn, index),
        getMethodName      (eType         ));

    throw InvalidParameterException(ExceptInfo (func, val, file, line));
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setInt64(UInt32 pos, const Int64 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setInt64";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType(pos, TYPE_INT8, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;
    infxSetDescInt64(m_pDescIn, pos, &rValue);

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setInt64[%d]: <%lld>",
            m_StmtId.c_str(),
            pos,
            rValue);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setInt32(UInt32 pos, const Int32 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setInt32";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_INT, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;
    infxSetDescInt( m_pDescIn, pos, &rValue );

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setInt32[%d]: <%d>",
            m_StmtId.c_str(),
            pos,
            rValue);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setInt16(UInt32 pos, const Int16 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setInt16";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_SHORT, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;
    infxSetDescShort(m_pDescIn, pos, &rValue);	

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setInt16[%d]: <%d>",
            m_StmtId.c_str(),
            pos,
            rValue);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setString(UInt32 pos, ConstString value)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setString";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_STRING, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    infxSetDescString( m_pDescIn, pos, value);	

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setString[%d]: <%s>",
            m_StmtId.c_str(),
            pos,
            value);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setBinary(UInt32 pos, const Binary & value)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setBinary";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType(pos, TYPE_BYTE, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    if (0 == value.size())
    {
        infxSetDescByte(m_pDescIn, pos, NULL, static_cast<unsigned int>(value.size()));
    }
    else
    {
        infxSetDescByte(m_pDescIn, pos, &value[0], static_cast<unsigned int>(value.size()));
    }

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setBinary[%d]",
            m_StmtId.c_str(),
            pos);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setFloat64(UInt32 pos, const Float64 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setFloat64";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_DOUBLE, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;
    infxSetDescDouble(m_pDescIn, pos, &rValue);	

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setFloat64[%d]: <%f>",
            m_StmtId.c_str(),
            pos,
            rValue);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw OutOfRangeIndexException
//!
void PreparedStatementInfx::setFloat32(UInt32 pos, const Float32 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setFloat32";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_FLOAT, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;
    infxSetDescFloat(m_pDescIn, pos, &rValue);

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setFloat32[%d]: <%f>",
            m_StmtId.c_str(),
            pos,
            rValue);
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw OutOfRangeIndexException
//!
void PreparedStatementInfx::setDecimal(UInt32 pos, const Decimal & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setDecimal";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_DECIMAL, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    const decimal * const dp = rValue.getDec();

    if (!m_CollHasBuffer[pos])
    {
        infxSetBufferToItemDesc(m_pDescIn, m_pRow.get(), m_pIndicator.get(), pos);
        m_CollHasBuffer[pos] = true;
    }

    infxSetDescDecimal(m_pDescIn, pos, dp);

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDecimal[%d]: <%s>",
            m_StmtId.c_str(),
            pos,
            rValue.toString().c_str());
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//!        Informix SQL DateTime is used for Time
//! \throw no-throw
void PreparedStatementInfx::setTime(UInt32 pos, const Time & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setTime";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_DATETIME, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    const dtime * const dp = rValue.getTimestampBuffer();

    if (!m_CollHasBuffer[pos])
    {
        infxSetBufferToItemDesc(m_pDescIn, m_pRow.get(), m_pIndicator.get(), pos);
        m_CollHasBuffer[pos] = true;
    }

    infxSetDescDateTime(m_pDescIn, pos, dp );

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setTime[%d]: <%s>",
            m_StmtId.c_str(),
            pos,
            rValue.toStrTime().c_str());
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setDate(UInt32 pos, const Date & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setDate";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_DATE, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    const Int32 * const pdate = rValue.getDateBuffer();

    if (!m_CollHasBuffer[pos])
    {
        infxSetBufferToItemDesc(m_pDescIn, m_pRow.get(), m_pIndicator.get(), pos);
        m_CollHasBuffer[pos] = true;
    }

    infxSetDescDate( m_pDescIn, pos, pdate);

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDate[%d]: <%s>",
            m_StmtId.c_str(),
            pos,
            rValue.toStrDate().c_str());
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setDateTime(UInt32 pos, const DateTime & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setDateTime";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_DATETIME, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    const dtime * const dp = rValue.getTimestampBuffer();

    if (!m_CollHasBuffer[pos])
    {
        infxSetBufferToItemDesc(m_pDescIn, m_pRow.get(), m_pIndicator.get(), pos);
        m_CollHasBuffer[pos] = true;
    }

    infxSetDescDateTime(m_pDescIn, pos, dp );	

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDateTime[%d]: <%s, %s>",
            m_StmtId.c_str(),
            pos,
            rValue.toStrDate().c_str(), rValue.toStrTime().c_str());
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setTimeSpan(UInt32 pos, const TimeSpan & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementInfx.setTimeSpan";

    checkRange(pos, func, __FILE__, __LINE__);
    checkType (pos, TYPE_INTERVAL, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    const intrvl * const dp = rValue.getTimestampBuffer();

    if (!m_CollHasBuffer[pos])
    {
        infxSetBufferToItemDesc(m_pDescIn, m_pRow.get(), m_pIndicator.get(), pos);
        m_CollHasBuffer[pos] = true;
    }

    infxSetDescTimeInterval(m_pDescIn, pos, dp );	

    if (LoggerPool::preparedStmtInfxSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDateTime[%d]: <%s, %s>",
            m_StmtId.c_str(),
            pos,
            rValue.toNormalizedString().c_str());
        LoggerPool::preparedStmtInfxSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementInfx::setCurrent()
{	
	static_cast<ConnectionInfx *>(m_pConn.get())->setCurrent();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing cursor id for created resultset/insertset
VarString PreparedStatementInfx::getCursorId (CursorIdEnum eCursorId) const
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
//! \throw no-throw
//!
void PreparedStatementInfx::construct()
{
    // each byte of address is represented by 2-digit hexadecimal number
    m_StmtId.format("stmt_0x%0*lx", sizeof(size_t) * 2, this);

    VarString        func = LoggerPool::preparedStmtInfx().getName();
	const InfxString infxSql (m_statement);

    try
    {

        for (UInt32 i = 0; i < m_CountParameter; i++)
        {
            m_CollHasBuffer.push_back(false);
        }

        checkConnState(func, __FILE__, __LINE__);
        setCurrent	  ();
        infxPrepStmt  (m_StmtId.c_str(), infxSql.out());
        checkSqlState (func, __FILE__, __LINE__);

		// only if parameters in SQL statement are given
		if (0 < m_CountParameter)
		{
			m_pDescIn = infxDescribeStmtIn(m_StmtId.c_str());

			// get number of descriptors
			m_CountParameter = infxGetDescCount(m_pDescIn);

			infxPrepDescIn(m_pDescIn, isFetchModeRaw());

       		// get row size and prepare descriptors (length, offsets)
		    Int32 size = infxPrepDescOut(m_pDescIn, isFetchModeRaw());

            // allocate and init row buffer
            m_pRow.reset(new char[size]);
            memset(m_pRow.get(), 0, sizeof(char) * size);

            // allocate and init indicator variable
            m_pIndicator.reset(new Int16[m_CountParameter]);
            memset(m_pIndicator.get(), 0, sizeof(Int16) * m_CountParameter);
		}
    }
    catch (const Exception & )
    {
		infxFreeImplicitDesc(m_pDescIn, DONT_FREE_LOC_BUFFER);

        if (LoggerPool::preparedStmtInfx().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("%s: constructing PreparedStatementInfx <%s>", 
				        m_StmtId.c_str(), 
						infxSql.out());
            LoggerPool::preparedStmtInfx().forcedLog(log4cplus::ERROR_LOG_LEVEL, 
				                                           msg, 
														   __FILE__, __LINE__);
        }

        throw;
    }


    m_Open = true;

    if (LoggerPool::preparedStmtInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s: constructing PreparedStatementInfx <%s>", 
			        m_StmtId.c_str(), 
					infxSql.out());
        LoggerPool::preparedStmtInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
			                                           msg, 
													   __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

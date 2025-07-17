//----------------------------------------------------------------------------
/*! \file
*  \brief  prepared statement odbc implementation class
*  \author Thomas Hörath
*  \date   07.11.2005
*/
//----------------------------------------------------------------------------

#include "preparedstatementodbc.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_timespan.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "connection.h"
#include "resultsetodbc.h"

#include <QtCore/QVariant>
#include <QtCore/QDateTime.h>



//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to statement implementation object
//! \throw  no-throw
PreparedStatementSharedPtr PreparedStatementOdbc::create(ConnectionSharedPtr pConn, 
														 const VarString   & rSql, 
														 QSqlDatabase      & rQSqlDb)
{
    return PreparedStatementSharedPtr(new PreparedStatementOdbc(pConn, rSql, rQSqlDb));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatementOdbc::PreparedStatementOdbc ( ConnectionSharedPtr pConn, const VarString & rSql, QSqlDatabase& rQSqlDb)
    : PreparedStatement  (pConn, rSql), 
    m_QSqlQuery        (rQSqlDb    ),
    m_rSqlExceptHndOdbc(*(static_cast<SqlExceptHandlerOdbc*>(m_SqlExceptHnd.get())))

{
    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);

    ConstString func = "basar.db.sql.PreparedStatementOdbc.PreparedStatementOdbc";

    try 
    {
        checkConnState(func, __FILE__, __LINE__);

		m_QSqlQuery.setForwardOnly(true);

		m_QSqlQuery.prepare(rSql.c_str());

        checkSqlState (func, __FILE__, __LINE__);

        m_Open = true;
    }
    catch (const Exception &)
    {
        if (LoggerPool::preparedStmtOdbc().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("constructing PreparedStatementOdbc <%s>", rSql.c_str());
            LoggerPool::preparedStmtOdbc().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

        m_rSqlExceptHndOdbc.resetQSqlQuery();

        throw;
    }
    catch (...)
    {
        m_rSqlExceptHndOdbc.resetQSqlQuery();
        BLOG_ERROR(LoggerPool::preparedStmtOdbc(), "unknown exception");
        assert(0);
        throw;
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatementOdbc::~PreparedStatementOdbc ()
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
        BLOG_ERROR(LoggerPool::preparedStmtOdbc(), "unknown exception");
        assert(0);
    }

    m_rSqlExceptHndOdbc.resetQSqlQuery();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementOdbc::close()
{
    if (false == isOpen())
        return;

    PreparedStatement::close();

    // after m_QSqlQuery.clear() stmt cannot be retrieved --> save it now
    VarString stmt;
    if (LoggerPool::preparedStmtOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
        stmt.format("<%s>", m_QSqlQuery.lastQuery().toLatin1().constData());

    ConstString func = "basar.db.sql.PreparedStatementOdbc.close";

    checkConnState(func, __FILE__, __LINE__);

    m_QSqlQuery.clear();

    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);
    checkSqlState (func, __FILE__, __LINE__);

    m_Open = false;

    if (LoggerPool::preparedStmtOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s for %s@%s closed",
            stmt.c_str(),
            m_pConn.get()->getDbInfo().database.c_str(),
            m_pConn.get()->getDbInfo().dbServer.c_str());
        LoggerPool::preparedStmtOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

void PreparedStatementOdbc::setInt64(UInt32 /*pos*/, const Int64 & /*rValue*/)
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.PreparedStatementOdbc::setInt64", "Not implemented (yet?)", __FILE__, __LINE__));
}

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//! \return reference to created resultset
ResultsetRef PreparedStatementOdbc::executeQuery()
{
    return execQuery("basar.db.sql.PreparedStatementOdbc.executeQuery");
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return reference to created resultset
ResultsetRef PreparedStatementOdbc::executeSingleQuery()
{
    return execQuery("basar.db.sql.PreparedStatementOdbc.executeSingleQuery");
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return reference to created resultset
basar::Decimal PreparedStatementOdbc::executeAggregate()
{
    throw NotImplementedException (ExceptInfo("basar.db.sql.PreparedStatementOdbc::executeAggregate","Not implemented, functionality not supported by ODBC",__FILE__, __LINE__));
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return number of rows affected by statement (update, delete, insert)
ExecuteReturnInfo PreparedStatementOdbc::execute(bool flagExcept)
{
    ExecuteReturnInfo retval;
    VarString func = LoggerPool::preparedStmtOdbcExec().getName();

    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in <%s>", 
            m_QSqlQuery.lastQuery().toLatin1().constData());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "prepared statement <" << m_QSqlQuery.lastQuery().toLatin1().constData() << "> is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    checkConnState(func, __FILE__, __LINE__);

    m_QSqlQuery.exec();

    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);

    if ( checkSqlState (func, __FILE__, __LINE__, flagExcept) )
    {
        retval.m_affectedRows   =  m_QSqlQuery.numRowsAffected();
        if (checkStmtStart(m_QSqlQuery.executedQuery().toLatin1().constData(), "insert"))
        {
            QVariant var =  m_QSqlQuery.lastInsertId ();
            if (!var.isNull() )
            {
                bool ok = false;
                Int32 lastSerial = m_QSqlQuery.lastInsertId ().toInt(&ok);
                if (ok && lastSerial != 0)
                {
                    retval.m_lastSerial.push_back(lastSerial);
                }
            }
        }
    }
    else
    {
        retval.m_error = getLastError();
    }

    if (LoggerPool::preparedStmtOdbcExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%d rows affected by executing <%s>",
            retval.m_affectedRows,
            m_QSqlQuery.executedQuery().toLatin1().constData());
        LoggerPool::preparedStmtOdbcExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return retval;
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//! \return reference to created resultset
ResultsetRef PreparedStatementOdbc::execQuery(const VarString & func)
{
    if (false == isParamsSet())
    {
        VarString msg;
        msg.format("not all parameters are set in <%s>", m_QSqlQuery.lastQuery().toLatin1().constData());
        throw InvalidParameterException(ExceptInfo(func, msg , __FILE__, __LINE__));
    }

	if (false == isOpen())
	{
		std::ostringstream os;
		os << "prepared statement <" << m_QSqlQuery.lastQuery().toLatin1().constData() << "> is closed";
		throw InvalidStateException(ExceptInfo(func, os.str(), __FILE__, __LINE__));
	}

    checkConnState(func, __FILE__, __LINE__);

    m_QSqlQuery.exec();

    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);

    checkSqlState (func, __FILE__, __LINE__);

    m_ResultsetRef = ResultsetRef(ResultsetOdbc::create(m_pConn, m_QSqlQuery));


    if (LoggerPool::preparedStmtOdbcExecQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("executed query <%s>",
            m_QSqlQuery.executedQuery().toLatin1().constData());
        LoggerPool::preparedStmtOdbcExecQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }

    return m_ResultsetRef;
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setInt32(UInt32 pos, const Int32 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setInt32";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, rValue);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
		msg.format("%s - setInt32[%d]: <%d> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue,
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setInt16(UInt32 pos, const Int16 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setInt16";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, rValue);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setInt16[%d]: <%d> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue,
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setString(UInt32 pos, ConstString value)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setString";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, value);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setString[%d]: <%s> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            value,
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

void PreparedStatementOdbc::setBinary(UInt32 /*pos*/, const Binary & /*value*/)
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.PreparedStatementOdbc::setBinary", "Not implemented (yet?)", __FILE__, __LINE__));
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setFloat64(UInt32 pos, const Float64 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setFloat64";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, rValue);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setFloat64[%d]: <%f> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue,
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setFloat32(UInt32 pos, const Float32 & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setFloat32";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, rValue);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setFloat32[%d]: <%f> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue,
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setDecimal(UInt32 pos, const Decimal & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setDecimal";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    m_QSqlQuery.bindValue(pos, rValue.toFloat64());

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDecimal[%d]: <%s> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue.toString().c_str(),
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setTime(UInt32 pos, const Time & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setTime";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    QTime tm = rValue.isValidTime() ? QTime(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec())
        : QTime(23, 59, 59, 999);
    m_QSqlQuery.bindValue(pos, tm);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setTime[%d]: <%s> (type %d: %s)",
            m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue.toStrTime().c_str(),
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setDate(UInt32 pos, const Date & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setDate";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    QDate dt = rValue.isValid() ? QDate(rValue.getYear(), rValue.getMonth(), rValue.getDay())
        : QDate(9999, 12, 31);

    m_QSqlQuery.bindValue(pos, dt);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDate[%d]: <%s> (type %d: %s)",
			m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue.toStrDate().c_str(),
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setDateTime(UInt32 pos, const DateTime & rValue)
{
    ConstString func = "basar.db.sql.PreparedStatementOdbc.setDateTime";

    checkRange(pos, func, __FILE__, __LINE__);

    m_CollParamFlags[pos] = true;

    QDate qd = rValue.isValidDate() ? QDate(rValue.getYear(), rValue.getMonth (), rValue.getDay())
        : QDate(9999, 12, 31);
    QTime qt = rValue.isValidTime() ? QTime(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec())
        : QTime(23, 59, 59, 999);

    QDateTime dt(qd, qt);

    m_QSqlQuery.bindValue(pos, dt);

    if (LoggerPool::preparedStmtOdbcSetVal().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("%s - setDateTime[%d]: <%s, %s> (type %d: %s)",
			m_QSqlQuery.lastQuery().toLatin1().constData(),
            pos,
            rValue.toStrDate().c_str(), rValue.toStrTime().c_str(),
			m_QSqlQuery.boundValue(pos).type(), m_QSqlQuery.boundValue(pos).typeName());
        LoggerPool::preparedStmtOdbcSetVal().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
    }
}

//----------------------------------------------------------------------------
//! \throw no-throw
void PreparedStatementOdbc::setTimeSpan(UInt32, const TimeSpan &)
{
    throw NotImplementedException (ExceptInfo("basar.db.sql.PreparedStatementOdbc::setDateTime","Not implemented, DataType not supported by Qt",__FILE__, __LINE__));
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

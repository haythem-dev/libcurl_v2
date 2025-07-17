//----------------------------------------------------------------------------
/*! \file
*  \brief  implementation class for odbc statements
*  \author Michael Eichenlaub
*  \date   11.08.2005
*/
//----------------------------------------------------------------------------

#include "statementodbc.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_logging.h"
#include "loggerpool.h"
#include "sqlexcepthandlerodbc.h"
#include "connection.h"
#include "resultsetodbc.h"

//----------------------------------------------------------------------------
namespace basar	{
    namespace db	{
        namespace sql	{

            //------------------------------------------------------------------------------
            //! \return	shared pointer to statement implementation object
            //! \throw  no-throw
            StatementSharedPtr StatementOdbc::create(ConnectionSharedPtr pConn, QSqlDatabase& rQSqlDb)
            {
                return StatementSharedPtr(new StatementOdbc(pConn, rQSqlDb));
            }

            //------------------------------------------------------------------------------
            //! \throw no-throw
            //!
            StatementOdbc::StatementOdbc ( ConnectionSharedPtr pConn, QSqlDatabase& rQSqlDb )
                : Statement          (pConn  ),
                m_QSqlQuery        (rQSqlDb),
                m_rSqlExceptHndOdbc(*(static_cast<SqlExceptHandlerOdbc*>(m_SqlExceptHnd.get())))
            {
                //ODBC_FIX
                m_QSqlQuery.setForwardOnly(true);
                m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);
            }

            //------------------------------------------------------------------------------
            //! \throw no-throw
            //!
            StatementOdbc::~StatementOdbc ()
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
                    BLOG_ERROR(LoggerPool::stmtOdbc(), "unknown exception");
                    assert(0);
                }

                m_rSqlExceptHndOdbc.resetQSqlQuery();
            }

            //------------------------------------------------------------------------------
            //! \throw no-throw
            //!
            void StatementOdbc::close()
            {
                if ( ! isOpen() )
                    return;

                Statement::close();

                // after m_QSqlQuery.clear() stmt cannot be retrieved --> save it now
                VarString stmt;
                if (LoggerPool::stmtOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                    stmt.format("<%s>", m_QSqlQuery.lastQuery().toLatin1().constData());

                ConstString func = "basar.db.sql.StatementOdbc.close";

                checkConnState(func, __FILE__, __LINE__);

                m_QSqlQuery.clear();

                m_QSqlQuery.setForwardOnly(true);

                m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);
                checkSqlState (func, __FILE__, __LINE__);

                m_Open = false;

                if (LoggerPool::stmtOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {
                    VarString msg;
                    msg.format("statement for %s@%s closed: %s",
                        m_pConn.get()->getDbInfo().database.c_str(),
                        m_pConn.get()->getDbInfo().dbServer.c_str(),
                        stmt.c_str());
                    LoggerPool::stmtOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                }
            }

            //------------------------------------------------------------------------------
            //! \throw InvalidParameterException
            //! \return reference to created resultset
            ResultsetRef StatementOdbc::executeQuery(const VarString & rSql, CursorTypeEnum    /* eCursorType */)
            {
                return execQuery(rSql, "basar.db.sql.StatementOdbc.executeQuery");
            }

            //------------------------------------------------------------------------------
            //! \throw  InvalidParameterException
            //! \return reference to created resultset
            ResultsetRef StatementOdbc::executeSingleQuery(const VarString & rSql)
            {
                return execQuery(rSql, "basar.db.sql.StatementOdbc.executeSingleQuery");
            }

            //------------------------------------------------------------------------------
            //! \throw  no-throw
            //! \return reference to created resultset
            ResultsetRef StatementOdbc::execQuery(const VarString & rSql, ConstString func)
            {
                close();

                assert(m_QSqlQuery.isForwardOnly());

                try
                {
                    checkConnState(func, __FILE__, __LINE__);

                    m_QSqlQuery.exec( QString::fromLocal8Bit(rSql.c_str()) );

                    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);
                    checkSqlState (func, __FILE__, __LINE__);

                    m_ResultsetRef = ResultsetRef(ResultsetOdbc::create(m_pConn, m_QSqlQuery));
                }
                catch (const Exception & )
                {
                    if (LoggerPool::stmtOdbcExecQuery().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
                    {
                        VarString msg;
                        msg.format("executed query <%s>", rSql.c_str());
                        LoggerPool::stmtOdbcExecQuery().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                    }

                    throw;
                }

                m_Open = true;

                if (LoggerPool::stmtOdbcExecQuery().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {
                    VarString msg;
                    msg.format("executed query <%s>", rSql.c_str());
                    LoggerPool::stmtOdbcExecQuery().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                }

                return m_ResultsetRef;
            }

            //------------------------------------------------------------------------------
            //! \throw  no-throw
            //! \return number of rows affected by statement (update, delete, insert)
            ExecuteReturnInfo StatementOdbc::execute(const VarString & rSql, bool flagExcept)
            {
                VarString func = LoggerPool::stmtOdbcExec().getName();

                close();

                assert(m_QSqlQuery.isForwardOnly());

                ExecuteReturnInfo retval;

                try
                {
                    checkConnState(func, __FILE__, __LINE__);

                    m_QSqlQuery.exec( QString::fromLocal8Bit(rSql.c_str()) );

                    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);

                    if ( checkSqlState (func, __FILE__, __LINE__, flagExcept) )
                    {
                        retval.m_affectedRows   =  m_QSqlQuery.numRowsAffected();
                        if (checkStmtStart(rSql, "insert"))
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
                        if (LoggerPool::stmtOdbcExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                        {
                            VarString msg;
                            msg.format("%d rows affected by executing <%s>",
                                retval.m_affectedRows,
                                rSql.c_str());
                            LoggerPool::stmtOdbcExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        retval.m_error = getLastError();
                        retval.m_DBError = getRawDBError();
                        if (LoggerPool::stmtOdbcExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                        {
                            VarString msg;
                            msg.format("error nr %d returned by executing <%s>",
                                retval.m_error,
                                rSql.c_str());
                            LoggerPool::stmtOdbcExec().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                        }
                    }

                }
                catch (const Exception & )
                {
                    if (LoggerPool::stmtOdbcExec().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
                    {
                        VarString msg;
                        msg.format("executed <%s>", rSql.c_str());
                        LoggerPool::stmtOdbcExec().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                    }

                    throw;
                }

                m_Open = true;

                return retval;
            }

            //------------------------------------------------------------------------------
            //! \throw  InvalidStateException, InvalidParameterException
            //! \return result of aggregate select
            basar::Decimal StatementOdbc::executeAggregate	(const VarString & rSql)
            {
                VarString func = LoggerPool::stmtOdbcExecAggregate().getName();

                close();

                assert(m_QSqlQuery.isForwardOnly());

                try
                {
                    checkConnState(func, __FILE__, __LINE__);

                    m_QSqlQuery.exec( QString::fromLocal8Bit(rSql.c_str()) );

                    m_rSqlExceptHndOdbc.setQSqlQuery(&m_QSqlQuery);
                    checkSqlState (func, __FILE__, __LINE__);

                    m_ResultsetRef = ResultsetRef(ResultsetOdbc::create(m_pConn, m_QSqlQuery));
                }
                catch (const Exception & )
                {
                    if (LoggerPool::stmtOdbcExecAggregate().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
                    {
                        VarString msg;
                        msg.format("executed aggregate select <%s>", rSql.c_str());
                        LoggerPool::stmtOdbcExecAggregate().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
                    }

                    throw;
                }

                m_Open = true;

                if (LoggerPool::stmtOdbcExecAggregate().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {
                    VarString msg;
                    msg.format("executed query <%s>", rSql.c_str());
                    LoggerPool::stmtOdbcExecAggregate().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
                }

                if ( m_ResultsetRef.getSelectedCols() != 1 )
                {
                    basar::VarString msg;
                    msg.format("not exactly one column (=> %d) selected in aggregate select <%s>.", 
                               m_ResultsetRef.getSelectedCols(),
                               rSql.c_str());
                    throw InvalidStateException(ExceptInfo("basar.db.sql.StatementOdbc.executeAggregate", msg, __FILE__, __LINE__));
                }

                m_ResultsetRef.next();

                QVariant::Type type = (static_cast<ResultsetOdbc*>(m_ResultsetRef.m_pImpl.get()) )->getType(0);

                if ( type == QVariant::String ) //decimal
                {
                    return m_ResultsetRef.getDecimal(0);
                }
                else if ( type == QVariant::Double ) //float32 + float64
                {
                    basar::Decimal d(m_ResultsetRef.getFloat64(0));
                    return d;
                }
                else if ( type == QVariant::Int )	//int32 + int16
                {
                    basar::Decimal d(m_ResultsetRef.getInt32(0));
                    return d;
                }
                else
                {
                    basar::VarString msg;
                    msg.format("aggregate select <%s> resulted in an invalid aggregate type <%d>. only numeric results allowed.", 
                        rSql.c_str(), type);
                    throw InvalidParameterException(ExceptInfo("basar.db.sql.StatementOdbc.executeAggregate", msg, __FILE__, __LINE__));
                }
            }

            //----------------------------------------------------------------------------
        }	// namespace sql
    }	// namespace db
}	// namespace basar

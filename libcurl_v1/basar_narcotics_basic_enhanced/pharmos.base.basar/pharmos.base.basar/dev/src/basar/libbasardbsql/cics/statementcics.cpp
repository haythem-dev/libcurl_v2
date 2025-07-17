/*
* @file statementcics.cpp

* @brief ...
* @author Anke Klink

* @date 2007
*/
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "resultsetcics.h"
#include "connection.h"
#include "libbasardbsql_exceptions.h"
#include "statementcics.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{
// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return shared_ptr to StatementCics instance
StatementSharedPtr StatementCics::create(
    ConnectionSharedPtr        pConn, 
    const CicsConnectionInfo & info)
{
    return StatementSharedPtr(new StatementCics(pConn, info));
}

// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
StatementCics::StatementCics(
    ConnectionSharedPtr        pConn,
	const CicsConnectionInfo & info )
    : Statement      (pConn),
	  m_statementInfo(info)
{
}

// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
StatementCics::~StatementCics()
{
    try
    {
        close();
    }
    catch (const basar::Exception & ex)
    {
        BLOG_ERROR(LoggerPool::stmtCics(), ex.what());
        assert(0);
    }
    catch (...)
    {
        BLOG_ERROR(LoggerPool::stmtCics(), "unknown exception");
        assert(0);
    }
}

// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return ResultsetRef referencing executed statement's results
ResultsetRef StatementCics::executeQuery(
    const VarString & rSql, 
    CursorTypeEnum        )
{
    VarString func = "basar.db.sql.StatementCics.executeQuery";

    close();

	//check length of statement, 32k max - todo: magic number
	if (rSql.length() > TCACCESS_STATEMENT_MAX_LENGTH)
	{
		throw DbExceptionStatementLengthExceeded(ExceptInfo("basar.db.sql.StatementCics::executeQuery", "Cics Statement size exceeds 32k bytes", __FILE__, __LINE__));
	}

    try
    {
        checkConnState(func, __FILE__, __LINE__);

        m_statementInfo.m_statement = rSql;
        CicsDefinitions::start(m_statementInfo);

        checkSqlState (func, __FILE__, __LINE__);

        m_ResultsetRef = 
        ResultsetRef(ResultsetCics::create(m_pConn, m_statementInfo));
    }
    catch (const Exception & )
    {
        BLOG_ERROR_STREAM(LoggerPool::stmtCicsExecQuery(),
        "executed query <"
        << rSql
        << ">");

        throw;
    }

    m_Open = true;

    BLOG_DEBUG_STREAM(LoggerPool::stmtCicsExecQuery(),
        "executed query <"
        << rSql
        << ">");

    return m_ResultsetRef;
}

// --------------------------------------------------------------------------------------------------
//! execute a SQL single select statement, return a one row resultset \n InvalidParameterException
//! \return ResultsetRef referencing a one row resultset
ResultsetRef StatementCics::executeSingleQuery(const VarString & rSql)
{
    return executeQuery(rSql, CURS_DEFAULT);
}

// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return ExecuteReturnInfo indicating success or failure of executed statement
ExecuteReturnInfo  StatementCics::execute(
    const VarString & rSql, 
    bool              flagExcept)
{
    VarString func = "basar.db.sql.StatementCics.execute";

    close();

    ExecuteReturnInfo retval;

	//check length of statement, 32k max - todo: magic number, flagExcept
	if (rSql.length() > TCACCESS_STATEMENT_MAX_LENGTH)
	{
		BLOG_ERROR_STREAM(LoggerPool::stmtCicsExecQuery(), "statement >" << rSql << "< exceeds 32k bytes");
		//honor flagExcept
		if (flagExcept)
		{
			throw DbExceptionStatementLengthExceeded(ExceptInfo("basar.db.sql.StatementCics::execute", "Cics Statement size exceeds 32k bytes", __FILE__, __LINE__));
		}
		else
		{
			retval.m_error = DBErrorCode::LENGHT_EXCEEDED;
			return retval;
		}
	}

    try
    {
        checkConnState(func, __FILE__, __LINE__);

        if ( checkSqlState (func, __FILE__, __LINE__, flagExcept) )
        {
            m_statementInfo.m_statement = rSql;
            CicsDefinitions::start(m_statementInfo);

			retval = CicsDefinitions::exec(m_statementInfo, flagExcept);
        }
    }
    catch (const Exception & )
    {
        BLOG_ERROR_STREAM(LoggerPool::stmtCicsExecQuery(),
            "executed <"
            << rSql
            << ">");

        throw;
    }

    m_Open = true;

    if (LoggerPool::stmtCicsExec().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;

		if (DBErrorCode::SUCCESS == retval.m_error)
            msg.format(
                "%d rows affected by executing <%s>", 
                retval.m_affectedRows, 
                rSql.c_str());
        else
            msg.format(
                "error no %d returned by executing <%s>", 
                retval.m_error, 
                rSql.c_str());

        LoggerPool::stmtCicsExec().forcedLog(
            log4cplus::DEBUG_LOG_LEVEL, 
            msg, 
            __FILE__, __LINE__);
    }

    if (DBErrorCode::SUCCESS != retval.m_error)
    {
        retval.m_DBError = getRawDBError();
    }

    return retval; 
}

// --------------------------------------------------------------------------------------------------
//! \throw InvalidParameterException
//!
//! execute a SQL single select statement containing count(*) or 
//! sum, avg, min, max for numeric fields
//! \return result of single select statement containing sql aggregate (count, sum, avg, min, max)
basar::Decimal StatementCics::executeAggregate (const VarString & rSql)
{
    VarString func = "basar.db.sql.StatementCics.executeAggregate";

	m_ResultsetRef = executeSingleQuery(rSql);

    if ( !m_ResultsetRef.next() )
        throw InvalidParameterException(
                ExceptInfo(
                    func, 
                    "fetch failed - no data found", 
                    __FILE__, __LINE__));

	ResultsetCics * helper = 
        static_cast<ResultsetCics *> (m_ResultsetRef.m_pImpl.get());

	const Int32     index = 0;

    switch (helper->getType(index))
	{
	case DECIMAL:
		return helper->getDecimal(index);

	case LINT:
		return Decimal(helper->getInt32(index));

	case SINT:
		return Decimal(helper->getInt16(index));

	case FLOAT:
		return Decimal(helper->getFloat64(index));

	default:
		{
			VarString msg;
			msg.format(	
                "data type %d not compatible with "
                "StatementCics::executeAggregate()", 
										helper->getType(index));
			throw InvalidParameterException(
                    ExceptInfo(
                        func, 
                        msg, 
                        __FILE__, __LINE__));
		}
    }
}

// --------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
void StatementCics::close()
{
    // in any case close members: m_ResultsetRef etc.
    Statement::close();

    // close opened statement handle
    CicsDefinitions::stop(m_statementInfo);

    ConstString func = "basar.db.sql.StatementCics.close";

    checkSqlState (func, __FILE__, __LINE__);

    BLOG_DEBUG_STREAM(
        LoggerPool::stmtCics(),
        ""
        << (isOpen() ? "" : "not")
        << " open statement for "
        << m_pConn.get()->getDbInfo().dbServer
        << " closed: <"
        << ( m_statementInfo.m_statement.empty() 
             ? "empty" 
             : m_statementInfo.m_statement)
        << ">");

    m_Open = false;
}

// --------------------------------------------------------------------------------------------------
}
}
}

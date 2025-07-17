//----------------------------------------------------------------------------
/*! \file
 *  \brief  handler for SQL exceptions (odbc)
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#include "sqlexcepthandlerodbc.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasardbsql_exceptions.h"
#include "sqldiagnosticsodbc.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // qstring.h - C4127: conditional expression is constant
#endif
#include <QtSql/QSqlQuery>
#ifdef _WIN32
#pragma warning (pop)
#endif

#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \return	shared pointer to sql diagnostics implementation object
//! \throw  no-throw
boost::shared_ptr < SqlExceptHandler > SqlExceptHandlerOdbc::create (QSqlDatabase& rQSqlDb)
{
	return boost::shared_ptr < SqlExceptHandler >(new SqlExceptHandlerOdbc(rQSqlDb));
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandlerOdbc::SqlExceptHandlerOdbc(QSqlDatabase& rQSqlDb)
	: m_rQSqlDb (rQSqlDb), 
	  m_pQQuery (0)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandlerOdbc::~SqlExceptHandlerOdbc()
{
	m_pQQuery = 0;
}

//----------------------------------------------------------------------------
//! \throw  DbExceptionOdbc
//! \retval true if no data found
//! \retval false if data found
bool SqlExceptHandlerOdbc::isSqlStateNotFound(const VarString & rWhat,
											  const VarString & rFile,
											  UInt32		    line)
{
	if ( m_rQSqlDb .lastError().type() == QSqlError::NoError && 
		 m_pQQuery->lastError().type() == QSqlError::NoError    )
	{
		return true;
	}
	
	clearWarnings();
	throw DbExceptionOdbc(ExceptInfo(rWhat, "SQLSTATE == QSqlError", rFile, line), m_rQSqlDb, m_pQQuery);
}

//----------------------------------------------------------------------------
//! \throw DbExceptionOdbc
//! \return true in case of no error, false (or DbExceptionInfx depending on flagExcept) otherwise
bool SqlExceptHandlerOdbc::checkSqlState(const VarString & rWhat     ,
										 const VarString & rFile     ,
										 UInt32			   line	     ,
										 bool              flagExcept)
{
    bool retval = false;
	if ( 0 == m_pQQuery )
	{
		if ( m_rQSqlDb.lastError().type() == QSqlError::NoError )
		{
			clearWarnings();
			retval =  true;
		}
	}
	else if ( (m_rQSqlDb.lastError ().type() == QSqlError::NoError) && 
			  (m_pQQuery->lastError().type() == QSqlError::NoError)    )
	{
		clearWarnings();
		retval =  true;
	}
    if (!retval)
    {
		if ( flagExcept )
		{
			throw DbExceptionOdbc(ExceptInfo(rWhat, "SQLSTATE == QSqlError", rFile, line), m_rQSqlDb, m_pQQuery);
		}
    }
	return retval;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return returns the last Error as DBErrorCode::Code
DBErrorCode::Code SqlExceptHandlerOdbc::getLastError()
{
    DBErrorCode::Code  retval = DBErrorCode::SUCCESS;

	if ( m_rQSqlDb.lastError().type() != QSqlError::NoError ||
        m_pQQuery->lastError().type() != QSqlError::NoError)
    {
        retval = DBErrorCode::UNKNOWN;
    }

	return retval;
}

DBError SqlExceptHandlerOdbc::getRawDBError()
{
    DBError retVal;

    retVal.m_rawMainError = m_rQSqlDb.lastError().nativeErrorCode().toInt();
    retVal.m_rawSubError = retVal.m_rawMainError;

    createWarnings();

    m_SqlDiagRef.getAllExcepts(retVal.m_errorMsg);

    return retVal;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandlerOdbc::createWarnings()
{
	m_SqlDiagRef.m_pImpl = SqlDiagnosticsOdbc::create(m_rQSqlDb, m_pQQuery);
}


//----------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandlerOdbc::setQSqlQuery(QSqlQuery* pQQuery)
{
	m_pQQuery = pQQuery;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandlerOdbc::resetQSqlQuery()
{
	m_pQQuery = 0;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

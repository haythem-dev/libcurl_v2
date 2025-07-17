//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

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
SqlDiagSharedPtr SqlDiagnosticsOdbc::create ( QSqlDatabase& rQSqlDb, QSqlQuery * pQQuery)
{
	return SqlDiagSharedPtr(new SqlDiagnosticsOdbc(rQSqlDb, pQQuery));
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsOdbc::SqlDiagnosticsOdbc(QSqlDatabase& rQSqlDb, QSqlQuery * pQQuery)
:m_rQSqlDb (rQSqlDb), m_pQQuery (pQQuery)
{
	init();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsOdbc::~SqlDiagnosticsOdbc()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void SqlDiagnosticsOdbc::init()
{
	m_ItDiagInfo = m_CollDiagInfo.begin();

	if ( 0 == m_pQQuery )
	{
		if ( m_rQSqlDb.lastError().type() == QSqlError::NoError )
		{
			assert (0);
			return;
		}
	}
	else
	{
		if ( m_rQSqlDb.lastError().type() == QSqlError::NoError && m_pQQuery->lastError().type() == QSqlError::NoError)
		{
			assert (0);
			return;
		}
	}
	
	if ( m_pQQuery != 0 && 
		m_rQSqlDb.lastError().type() != QSqlError::NoError && 
		m_pQQuery->lastError().type() != QSqlError::NoError )
	{
		m_CountExcepts = 2;
	}
	else
	{
		m_CountExcepts = 1;
	}

	
	DiagnosticInfo	diagInfo;
	m_DbServer	= m_rQSqlDb.hostName().toLatin1().constData();
	m_ConnId.format("ODBC-Connect to %s", m_rQSqlDb.databaseName().toLatin1().constData());

	if ( m_pQQuery != 0 && m_pQQuery->lastError().type() != QSqlError::NoError )
	{
		diagInfo.msgTxt		= m_pQQuery->lastError().text().toLatin1().constData();
		diagInfo.sqlState.format("odbc-error: %d", m_pQQuery->lastError().type());
		diagInfo.sqlCode	= m_pQQuery->lastError().nativeErrorCode().toInt() ;
	}
	else
	{
		diagInfo.msgTxt		= m_rQSqlDb.lastError().text().toLatin1().constData();
		diagInfo.sqlState.format("odbc-error: %d", m_rQSqlDb.lastError().type());
		diagInfo.sqlCode	= m_rQSqlDb.lastError().nativeErrorCode().toInt() ;
	}
	
	m_CollDiagInfo.push_back(diagInfo);

	if ( m_CountExcepts > 1 )
	{
		diagInfo.msgTxt		= m_rQSqlDb.lastError().text().toLatin1().constData();
		diagInfo.sqlState.format("odbc-error:%d", m_rQSqlDb.lastError().type());
		diagInfo.sqlCode	= m_rQSqlDb.lastError().nativeErrorCode().toInt() ;

		m_CollDiagInfo.push_back(diagInfo);
	}
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

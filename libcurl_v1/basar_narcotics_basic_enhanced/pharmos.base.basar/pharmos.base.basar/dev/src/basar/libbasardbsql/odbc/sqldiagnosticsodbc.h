//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class (odbc)
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLDIAGNOSTICSODBC_H
#define GUARD_SQLDIAGNOSTICSODBC_H

//----------------------------------------------------------------------------
#include "sqldiagnostics.h"
#include "libbasardbsql_definitions.h"

//----------------------------------------------------------------------------
class QSqlDatabase;
class QSqlQuery;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! SQL diagnostics area's info implementation class for informix \n final class
class SqlDiagnosticsOdbc : public SqlDiagnostics
{
public:
	static SqlDiagSharedPtr	create			(
											 QSqlDatabase& rQSqlDb,	//!< reference to Qt-Connection, so that exception knows to which connection it belongs;
											 QSqlQuery	 * pQQuery	//!< pointer to Qt-Query, so that exception knows to which statement it belongs; might be null;
											);						//!< create sql diagnostics implementation object	\n no-throw

	//! destructs implementation object			\n no-throw
	virtual					~SqlDiagnosticsOdbc	();	

protected:
							SqlDiagnosticsOdbc	(
												 QSqlDatabase& rQSqlDb,		//!< reference to Qt-Connection, so that exception knows to which connection it belongs;
												 QSqlQuery	 * pQQuery		//!< pointer to Qt-Query, so that exception knows to which statement it belongs; might be null;
												 );							//!< initialize \n no-throw	

							SqlDiagnosticsOdbc	(const SqlDiagnosticsOdbc & rCopy);	//!< forbidden
	SqlDiagnosticsOdbc &		operator =		(const SqlDiagnosticsOdbc & rCopy);	//!< forbidden

	//! requests total exceptions number and all Odbc SQL exceptions \n no-throw
	virtual void			init			();

	QSqlDatabase &			m_rQSqlDb;		//!< QSqlDatabase from QT: connection where all errors occur
	QSqlQuery	 *			m_pQQuery;		//!< QSqlQuery from QT: statement where errors occur


};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

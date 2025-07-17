//----------------------------------------------------------------------------
/*! \file
 *  \brief  exception class for physical database errors
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_exceptions.h"
#include "sqldiagnosticsodbc.h"
#include "libbasardbsql_sqldiagnosticsref.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbExceptionOdbc::DbExceptionOdbc ( const ExceptInfo & sInfo  , 
								   QSqlDatabase     & rQSqlDb, 
								   QSqlQuery        * pQQuery)
								 : DbException (sInfo, 
								                "DbExceptionOdbc", 
												getAddReason(rQSqlDb, pQQuery))
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbExceptionOdbc::~DbExceptionOdbc ( )
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return exception reasons
VarString DbExceptionOdbc::getAddReason( QSqlDatabase & rQSqlDb, QSqlQuery * pQQuery)
{
	SqlDiagnosticsRef sqlDiagRef = SqlDiagnosticsOdbc::create(rQSqlDb, pQQuery);

	VarString allExcepts;
	sqlDiagRef.getAllExcepts(allExcepts);	

	return allExcepts;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

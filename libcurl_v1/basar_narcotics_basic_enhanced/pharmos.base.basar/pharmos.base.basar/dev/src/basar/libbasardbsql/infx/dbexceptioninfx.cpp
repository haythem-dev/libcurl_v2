//----------------------------------------------------------------------------
/*! \file
 *  \brief  exception class for physical database errors on informix
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_exceptions.h"
#include "sqldiagnosticsinfx.h"
#include "libbasardbsql_sqldiagnosticsref.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbExceptionInfx::DbExceptionInfx ( const ExceptInfo & sInfo )
								 : DbException (sInfo, 
								                "DbExceptionInfx", 
												getAddReason())
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbExceptionInfx::~DbExceptionInfx ( )
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return exception reasons
VarString DbExceptionInfx::getAddReason()
{
	SqlDiagnosticsRef sqlDiagRef = SqlDiagnosticsInfx::create();

	VarString allExcepts;
	sqlDiagRef.getAllExcepts(allExcepts);	

	return allExcepts;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

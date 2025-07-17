/** $Id$
 *
 * @file sqldiagnosticscics.h
 * @author Anke Klink
 * @date 2007
 */


#ifndef  __GUARD_BASAR_LIBBASARDBSQL_CICS_SQLDIAGNOSTICSCICS_H__ 
#define  __GUARD_BASAR_LIBBASARDBSQL_CICS_SQLDIAGNOSTICSCICS_H__ 

//----------------------------------------------------------------------------
#include "sqldiagnostics.h"
#include "libbasardbsql_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace db    {
namespace sql   {

//------------------------------------------------------------------------------
//! Empty Class, not used to check on the suitabilty of a statement for CICS
class SqlDiagnosticsCics : public SqlDiagnostics
{
public:
	//! internal create Method
	static SqlDiagSharedPtr	create				();

	//! destructs implementation object			\n no-throw
	virtual					~SqlDiagnosticsCics	();


	SqlDiagnosticsCics	( );							//!< initialize \n no-throw
protected:
							SqlDiagnosticsCics	(const SqlDiagnosticsCics & rCopy);	//!< forbidden
	SqlDiagnosticsCics &	operator =			(const SqlDiagnosticsCics & rCopy);	//!< forbidden

	//! requests total exceptions number and all Cics SQL exceptions \n no-throw
	virtual void			init				();
};

//------------------------------------------------------------------------------
}
}
}

//------------------------------------------------------------------------------
#endif 

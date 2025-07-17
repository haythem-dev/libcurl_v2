//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class (infx)
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLDIAGNOSTICSINFX_H
#define GUARD_SQLDIAGNOSTICSINFX_H

//----------------------------------------------------------------------------
#include "libbasardbsql_definitions.h"
#include "sqldiagnostics.h"

//----------------------------------------------------------------------------
struct sqlda;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! SQL diagnostics area's info implementation class for informix \n final class
class SqlDiagnosticsInfx : public SqlDiagnostics
{
public:
	//! get dump of statement descriptors				\n no-throw
	static VarString		getDescDump			(::sqlda * m_pDesc					//!< pointer to sql descriptor area
												);	

	//! create sql diagnostics implementation object	\n no-throw
	static SqlDiagSharedPtr	create				();			

	//! destructs implementation object					\n no-throw
	virtual					~SqlDiagnosticsInfx	();	


protected:
							SqlDiagnosticsInfx	();									//!< initialize \n no-throw	
							SqlDiagnosticsInfx	(const SqlDiagnosticsInfx & rCopy);	//!< forbidden
	SqlDiagnosticsInfx &	operator =			(const SqlDiagnosticsInfx & rCopy);	//!< forbidden

	//! requests total exceptions number and all Informix SQL exceptions \n no-throw
	virtual void			init				();
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

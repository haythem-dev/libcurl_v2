//----------------------------------------------------------------------------
/*! \file
 *  \brief  handler for SQL exceptions
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLEXCEPTHANDLER_H
#define GUARD_SQLEXCEPTHANDLER_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbsql_definitions.h"
#include "libbasardbsql_sqldiagnosticsref.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! handler for SQL exceptions \n base class
class SqlExceptHandler
{
public:
	//! constructs implementation object		\n no-throw
						SqlExceptHandler	();	
	
	//! destructs implementation object			\n no-throw
	virtual				~SqlExceptHandler	();	

	//! check and handle Informix SQL STATE	\n throw  DbException
	//! \return true in case of no error, false (or DbException depending on flagExcept) otherwise
	virtual bool		checkSqlState		(const VarString & rWhat,		//!< text description 
											 const VarString & rFile,		//!< file where exception is thrown
											 UInt32			   line,		//!< line of file where exception is thrown 
											 bool              flagExcept	//!< if error: DB exception / return code
											) = 0;	
    //! returns the last DBErrorCode::Code 
    virtual   DBErrorCode::Code        getLastError() = 0;

    virtual DBError                    getRawDBError() = 0;

	//! check Informix SQL state for "NO DATA FOUND"	\n no-throw
	//! \return true if no data found otherwise false
	virtual bool		isSqlStateNotFound	(const VarString & rWhat,	//!< text description 
											 const VarString & rFile,	//!< file where exception is thrown
											 UInt32			   line		//!< line of file where exception is thrown 
											) = 0;
	
	//! clears warning object (i.e. reset SqlDiagnosticsRef)	 \n no-throw 
	void				clearWarnings		();

	//! get SQL diagnostics reference object					\n no-throw
	SqlDiagnosticsRef	getWarnings			() const;

protected:
						SqlExceptHandler	(const SqlExceptHandler & rCopy);	//!< forbidden
	SqlExceptHandler &	operator =			(const SqlExceptHandler & rCopy);	//!< forbidden

	//! clear existing warning object and create new one		\n no-throw
	virtual void		createWarnings		() = 0;


    SqlDiagnosticsRef	m_SqlDiagRef;		//!< SQL diagnostics reference object
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

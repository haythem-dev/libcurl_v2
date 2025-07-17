//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for SQL diagnostics area
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_SQLDIAGNOSTICSREF_H
#define GUARD_LIBBASARDBSQL_SQLDIAGNOSTICSREF_H

//------------------------------------------------------------------------------
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! reference class representing SQL diagnostics area's \n final class 
class SqlDiagnosticsRef
{
	friend class SqlExceptHandler;		//!< creator class
	friend class SqlExceptHandlerInfx;	//!< creator class
	friend class SqlExceptHandlerOdbc;	//!< creator class
	friend class DbExceptionInfx ;		//!< creator class
	friend class DbExceptionOdbc  ;		//!< creator class

public:
	//! construct reference object and initialise shared pointer to implementation object to null \n no-throw
	LIBBASARDBSQL_API						SqlDiagnosticsRef	();

	//! destruct reference object																	\n no-throw
	LIBBASARDBSQL_API					   ~SqlDiagnosticsRef	();

	LIBBASARDBSQL_API void					reset				() ;		//!< set pointer to implementation object to null		\n no-throw
	LIBBASARDBSQL_API bool					isNull				() const;	//!< check if shared pointer to implementation class is null	\n no-throw

	//! write all exceptions in caller string			\n no-throw
	LIBBASARDBSQL_API void					getAllExcepts		(VarString & outString	//!< string where exceptions are written to
																) const;	

	//! move to specific exception						\n no-throw
	LIBBASARDBSQL_API void					absolute			(Int32 exceptIndex	//!< exception index where to move to
																);

	//! get number of current SQL exceptions			\n no-throw
	LIBBASARDBSQL_API Int32					getCountExcepts		() const;	

	//! get name of database server					\n no-throw
	LIBBASARDBSQL_API const VarString &		getDbServer			() const;
	//! get connection ID							\n no-throw
	LIBBASARDBSQL_API const VarString &		getConnId			() const;	

	//! get SQLSTATE value								\n no-throw
	LIBBASARDBSQL_API const VarString &		getSqlState			() const;	
	//! get Informix-specific exception code SQLCODE	\n no-throw
	LIBBASARDBSQL_API BLong					getSqlCode			() const;	
	//! get message text to describe exception			\n no-throw
	LIBBASARDBSQL_API const VarString &		getMsgTxt  			() const;	

private:
	//! construct SQL diagnostics reference object by shared pointer to implementation object \n no-throw
											SqlDiagnosticsRef	(SqlDiagSharedPtr pImpl);
	
	//! check and return shared pointer to implementation object \n throw NullReferenceException
	const SqlDiagSharedPtr &				getImpl			() const;

	SqlDiagSharedPtr m_pImpl;	//!< shared pointer to implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

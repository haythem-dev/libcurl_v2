//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for insertset
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_INSERTSETREF_H
#define GUARD_LIBBASARDBSQL_INSERTSETREF_H

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \brief reference class representing database insert set 
//! \n     final class 
//!
//! When Insertset implementation object is constructed, an internal open() is called.\n
//! i.e.: in insertset with multiple rows cursor will immediately be opened;
//! (no need for checking opened cursor).
class InsertsetRef
{
	friend class StatementInfx;				//!< creator class
	friend class PreparedStatementInfx;		//!< creator class

public:

	// --- method section - reference handling -------------------------------

	//! construct reference object and initialise shared pointer to implementation object to null \n no-throw
	LIBBASARDBSQL_API						InsertsetRef		();

	//! destruct reference object																	\n no-throw
	LIBBASARDBSQL_API					   ~InsertsetRef		();

	LIBBASARDBSQL_API void					reset				() ;		//!< set pointer to implementation object to null		\n no-throw
	LIBBASARDBSQL_API bool					isNull				() const;	//!< check if shared pointer to implementation class is null	\n no-throw


	// --- method section - impl object state handling -----------------------

	//! check if insertset is open (i.e. cursor opened)									\n no-throw
	LIBBASARDBSQL_API bool					isOpen				() const;
	//! release insertset resources								\n no-throw
	LIBBASARDBSQL_API void					close				();


	// --- method section - row handling -------------------------------------

	//! return number of inserted rows							\n no-throw
	LIBBASARDBSQL_API UInt32				getInsertedRows		() const;

	// --- method section - column handling  ---------------------------------

	/*! \brief set a row to the cursor set								\n no-throw */
	LIBBASARDBSQL_API void					insertRow			();

	//! return the number of rows which were inserted in the database	\n no-throw
	LIBBASARDBSQL_API UInt32				flushRows			();


private:
	//! construct insertset reference object by shared pointer to implementation object \n no-throw
											InsertsetRef		(InsertsetSharedPtr pImpl		//!< shared pointer to implementation object
																);
	
	//! check and give shared pointer to implementation object	\n throw NullReferenceException
	const InsertsetSharedPtr &				getImpl				() const;


	InsertsetSharedPtr						m_pImpl;	//!< shared pointer to implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

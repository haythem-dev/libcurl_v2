//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for SQL statements
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_STATEMENTREF_H
#define GUARD_LIBBASARDBSQL_STATEMENTREF_H

//------------------------------------------------------------------------------
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
// forward
class ResultsetRef;
class InsertsetRef;

//------------------------------------------------------------------------------
//! reference class representing SQL statement \n final class 
class StatementRef
{
	friend class ConnectionInfx;	//!< creator class
	friend class ConnectionOdbc;	//!< creator class
	friend class ConnectionCics;	//!< creator class

public:
	//! construct reference object and initialise shared pointer to implementation object to null \n no-throw
	LIBBASARDBSQL_API				StatementRef		();

	//! destruct reference object																	\n no-throw
	LIBBASARDBSQL_API			   ~StatementRef		();

	LIBBASARDBSQL_API void			reset				() ;		//!< set pointer to implementation object to null		\n no-throw
	LIBBASARDBSQL_API bool			isNull				() const;	//!< check if shared pointer to implementation class is null	\n no-throw

	/*! 
        \brief execute a SQL select statement, return a resultset					
        \throw no-throw
        \attention The life cycle of the returned ResultsetRef depends on the life cycle of the StatementRef that returned the ResultsetRef.<BR>
                           Destroying the StatementRef instance invalidates the ResultsetRef.
    */
	LIBBASARDBSQL_API ResultsetRef	executeQuery		(const VarString & rSql,						//!< SQL select statement
														 CursorTypeEnum	   eCursorType = CURS_DEFAULT	//!< cursor type
														);
                            
  /*!
        \brief execute a SQL single select statement, return a one row resultset
        \throw no-throw
        \attention The life cycle of the returned ResultsetRef depends on the life cycle of the StatementRef that returned the ResultsetRef.<BR>
                           Destroying the StatementRef instance invalidates the ResultsetRef.
      */
	LIBBASARDBSQL_API ResultsetRef	executeSingleQuery	(const VarString & rSql		//!< SQL select statement
														);
	//! execute insert cursor statement										\n no-throw
	LIBBASARDBSQL_API InsertsetRef	executeInsert		(const VarString & rSql,						//!< SQL select statement
														 CursorTypeEnum	   eCursorType = CURS_DEFAULT	//!< cursor type
														);
	//! execute a given SQL statement										\n no-throw
	LIBBASARDBSQL_API ExecuteReturnInfo	execute			(const VarString & rSql,				//!< SQL non-select statement
														 bool              flagExcept = true	//!< if error: DB exception / return code
														);

	//! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n no-throw
	LIBBASARDBSQL_API basar::Decimal executeAggregate	(const VarString & rSql		//!< SQL select statement
														);
	
	//! release statement resources											\n no-throw
	LIBBASARDBSQL_API void			close				();
	//! check if statement is open (i.e. prepared)							\n no-throw
	LIBBASARDBSQL_API bool			isOpen				() const;

private:
	//! construct statement reference object by shared pointer to implementation object \n no-throw
									StatementRef		(StatementSharedPtr   pImpl);
	
	//! check and return shared pointer to implementation object \n throw NullReferenceException
	const StatementSharedPtr &		getImpl				() const;

	StatementSharedPtr m_pImpl;	//!< shared pointer to implementation object
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

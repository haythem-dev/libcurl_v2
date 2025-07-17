//----------------------------------------------------------------------------
/*! \file
 *  \brief  statement implementation class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_STATEMENT_H
#define GUARD_STATEMENT_H

//----------------------------------------------------------------------------
#include "sqloperative.h"
#include "libbasardbsql_resultsetref.h"
#include "libbasardbsql_insertsetref.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief statement implementation class \n base class
//!
class Statement : public SqlOperative
{
public:

	// --- method section ----------------------------------------------------

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~Statement				();

	//! execute a SQL select statement, return a resultset					\n no-throw
	//! \return reference to created resultset
	virtual ResultsetRef		executeQuery			(const VarString & rSql,		//!< SQL select statement
														 CursorTypeEnum	   eCursorType	//!< cursor type
														) = 0;
	//! execute a SQL single select statement, return a one row resultset	\n no-throw
	//! \return ResultsetRef containing a one row resultset
	virtual ResultsetRef		executeSingleQuery		(const VarString & rSql			//!< SQL select statement
														) = 0;
	//! insert cursor statement												\n throw InvalidStateException
	virtual InsertsetRef		executeInsert			(const VarString & rSql,		//!< SQL select statement
														 CursorTypeEnum	   eCursorType	//!< cursor type
		                                                );
	//! execute a given SQL statement (non-select)							\n no-throw
	//! \return ExecuteReturnInfo containing number of rows affected by statement (update, delete, insert)
	virtual ExecuteReturnInfo   execute					(const VarString & rSql,		//!< SQL non-select statement
														 bool              flagExcept	//!< if error: DB exception / return code
														) = 0;
	//! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n no-throw
	//! //! \return result of aggregate select
	virtual basar::Decimal		executeAggregate		(const VarString & rSql		//!< SQL select statement
														) = 0;

	
	//! release statement resources											\n no-throw
	virtual void				close					();
	//! check if statement is open													\n no-throw
	bool						isOpen					() const;	


protected:

	// --- method section ----------------------------------------------------

	//! constructs implementation object by connection reference			\n no-throw
								Statement				(ConnectionSharedPtr pConn	//!<  shared pointer to connection implementation object
														);

								Statement				();							//!< forbidden
								Statement				(const Statement & rCopy);	//!< forbidden
	Statement &    				operator =				(const Statement & rCopy);	//!< forbidden

	//! check connection state                        						\n no-throw
	virtual void				checkConnState			(const VarString & rWhat,	//!< text description 
														 const VarString & rFile,	//!< file where exception is thrown
														 UInt32			   line		//!< line of file where exception is thrown 
														) const;

	// --- member section ----------------------------------------------------

	ResultsetRef				m_ResultsetRef;		//!< resultset reference object
	InsertsetRef				m_InsertsetRef;		//!< insertset reference object
	ConnectionSharedPtr			m_pConn;			//!< shared pointer to connection implementation object
	bool						m_Open;				//!< statement state: is statement prepared?
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

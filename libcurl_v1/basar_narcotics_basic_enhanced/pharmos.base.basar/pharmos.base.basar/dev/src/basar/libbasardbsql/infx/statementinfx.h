//----------------------------------------------------------------------------
/*! \file
 *  \brief  statement informix implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_STATEMENTINFX_H
#define GUARD_STATEMENTINFX_H

//----------------------------------------------------------------------------
#include "statement.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief statement implementation class for informix\n final class
//!
class StatementInfx : public Statement
{
public:

	// --- method section ----------------------------------------------------

	//! create statement implementation object								\n no-throw
	static StatementSharedPtr	create					(ConnectionSharedPtr pConn	//!< shared pointer to connection implementation object
														);

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~StatementInfx			();

	//! execute a SQL select statement, return a resultset					\n no-throw
	virtual ResultsetRef		executeQuery			(const VarString & rSql,		//!< SQL select statement
														 CursorTypeEnum	   eCursorType	//!< cursor type
														);
	//! execute a SQL single select statement, return a one row resultset	\n no-throw
	virtual ResultsetRef		executeSingleQuery		(const VarString & rSql			//!< SQL select statement
														);
	//! insert cursor statement												\n no-throw
	virtual InsertsetRef		executeInsert			(const VarString & rSql,		//!< SQL select statement
														 CursorTypeEnum	   eCursorType	//!< cursor type
														);
	//! execute a given SQL statement (non-select)							\n no-throw
	virtual ExecuteReturnInfo	execute					(const VarString & rSql,		//!< SQL non-select statement
														 bool              flagExcept	//!< if error: DB exception / return code
														);

	/*! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	
		\n InvalidStateException, InvalidParameterException */
	virtual basar::Decimal		executeAggregate		(const VarString & rSql		//!< SQL select statement
														);
	
	//! release statement resources											\n no-throw
	virtual void				close					();
	

private:

	// --- method section ----------------------------------------------------

	//!< cursor types, needed for calculation cursor id
	enum CursorIdEnum
	{
		CURSORID_RESULTSET,		//!< for resultset
		CURSORID_INSERTSET		//!< for insertset
	};

	//! constructs implementation object by connection reference			\n no-throw
								StatementInfx			(ConnectionSharedPtr pConn		//!<  shared pointer to connection implementation object
														);

								StatementInfx			();								//!< forbidden
								StatementInfx			(const StatementInfx & rCopy);	//!< forbidden
	StatementInfx &    			operator =				(const StatementInfx & rCopy);	//!< forbidden

	//! get cursor id for created resultset/insertset	\n no-throw
	VarString					getCursorId				(CursorIdEnum eCursorId		//!< cursor type
														);
	//! set connection to current	\n no-throw
	void						setCurrent				();

	// --- member section ----------------------------------------------------

	VarString					m_StmtId;			//!< application's unique statement ID (i.e. object address)
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

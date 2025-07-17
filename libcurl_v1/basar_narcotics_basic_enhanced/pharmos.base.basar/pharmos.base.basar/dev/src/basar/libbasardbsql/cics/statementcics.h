/** $Id$
 *
 * @file statementcics.h
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */


#ifndef  __GUARD_BASAR_LIBBASARDBSQL_CICS_STATEMENTCICS_H__ 
#define  __GUARD_BASAR_LIBBASARDBSQL_CICS_STATEMENTCICS_H__ 

//----------------------------------------------------------------------------
#include "statement.h"
#include "cicsdefinitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{

//! Statement CICS Version
class StatementCics : public Statement
{
public:

	//! create statement implementation object								\n no-throw
	static StatementSharedPtr	create				(ConnectionSharedPtr        pConn,	//!< shared pointer to connection implementation object
													 const CicsConnectionInfo & info	//!< CICS connection info
													);
	//! create statement implementation object								\n no-throw
								StatementCics		(ConnectionSharedPtr        pConn,	//!< shared pointer to connection implementation object 
													 const CicsConnectionInfo & info	//!< CICS connection info
													);

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~StatementCics		();

	//! execute a SQL select statement, return a resultset					\n no-throw
	virtual ResultsetRef		executeQuery		(const VarString & rSql,			//!< SQL select statement
													 CursorTypeEnum	   eCursorType		//!< cursor type(not used for CICS)
													);
	//! execute a SQL single select statement, return a one row resultset	\n InvalidParameterException
	virtual ResultsetRef		executeSingleQuery	(const VarString & rSql				//!< SQL select statement
													);
	//! execute a given SQL statement(non-select)							\n no-throw
	virtual ExecuteReturnInfo	execute				(const VarString & rSql,			//!< SQL non-select statement
													 bool              flagExcept		//!< if error: DB exception / return code
													);
	/*! execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields
		\n throw InvalidParameterException */
	virtual basar::Decimal		executeAggregate	(const VarString & rSql				//!< SQL select statement
													);
	//! release statement resources	\n no-throw
	virtual void				close				();

private:

	CicsStatementInfo			m_statementInfo;			//!< info of the last statement

};

// namespaces
}
}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

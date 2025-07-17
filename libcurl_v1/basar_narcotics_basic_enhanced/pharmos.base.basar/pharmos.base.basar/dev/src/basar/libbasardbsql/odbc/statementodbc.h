//----------------------------------------------------------------------------
/*! \file
 *  \brief  statement odbc implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_STATEMENTODBC_H
#define GUARD_STATEMENTODBC_H

//----------------------------------------------------------------------------
#include "statement.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // qstring.h - C4127: conditional expression is constant
#pragma warning(disable: 4800) // qmetatype.h - C4800: forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include <QtSql/QSqlQuery.h>
#ifdef _WIN32
#pragma warning (pop)
#endif

//----------------------------------------------------------------------------
class QSqlDatabase;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
class SqlExceptHandlerOdbc;

//----------------------------------------------------------------------------
//! \brief statement implementation class for odbc\n final class
//!
class StatementOdbc : public Statement
{
public:

	// --- method section ----------------------------------------------------

	//! create statement implementation object								\n no-throw
	static StatementSharedPtr	create					(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
														 QSqlDatabase      & rQSqlDb	//!< reference to Qt-Connection, so that QSqlQuery knows to which connection it belongs
														);

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~StatementOdbc			();

	//! execute a SQL select statement, return a resultset					\n no-throw
	virtual ResultsetRef		executeQuery			(const VarString & rSql,		//!< SQL select statement
														 CursorTypeEnum	   eCursorType	//!< cursor type (not used for ODBC)
														);
	//! execute a SQL single select statement, return a one row resultset	\n InvalidParameterException
	virtual ResultsetRef		executeSingleQuery		(const VarString & rSql			//!< SQL select statement
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

	//! constructs implementation object by connection reference			\n no-throw
								StatementOdbc			(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
														 QSqlDatabase& rQSqlDb			//!< reference to Qt-Connection, so that QSqlQuery knows to which connection it belongs
														);

								StatementOdbc			();								//!< forbidden
								StatementOdbc			(const StatementOdbc & rCopy);	//!< forbidden
	StatementOdbc &    			operator =				(const StatementOdbc & rCopy);	//!< forbidden

	
	//! execute a SQL select statement, return a resultset	\n InvalidParameterException
	ResultsetRef				execQuery				(const VarString & rSql,	//!< SQL select statement
														 ConstString func			//!< function from which method is called - for exception!
														);
		
	QSqlQuery					m_QSqlQuery;				//!< QSqlQuery from QT where all statement calls are sent to
	SqlExceptHandlerOdbc &      m_rSqlExceptHndOdbc;		//!< reference to ODBC exception handler
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

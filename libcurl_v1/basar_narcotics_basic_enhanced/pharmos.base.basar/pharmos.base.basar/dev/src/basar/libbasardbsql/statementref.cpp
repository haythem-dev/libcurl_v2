//----------------------------------------------------------------------------
/*! \file
 *  \brief  statement reference class
 *  \author Michael Eichenlaub
 *  \date   27.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_statementref.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_decimal.h"
#include "statement.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
StatementRef::StatementRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \param  pImpl shared pointer to implementation object
StatementRef::StatementRef ( StatementSharedPtr pImpl )
                           : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
StatementRef::~StatementRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void StatementRef::reset()
{
	m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool StatementRef::isNull() const
{
	return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
const StatementSharedPtr & StatementRef::getImpl() const
{
	if (0 == m_pImpl.get())
		throw NullReferenceException(ExceptInfo("basar.db.sql.StatementRef.getImpl", 
												"m_pImpl.get() == 0", 
												__FILE__, __LINE__));

	return m_pImpl;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void StatementRef::close()
{
	getImpl()->close();
}

//------------------------------------------------------------------------------
//!	\retval true  if statement is opened
//! \retval false if statement is closed/not opened
//! \throw no-throw
bool StatementRef::isOpen() const
{
	return getImpl()->isOpen();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return reference to created resultset
ResultsetRef StatementRef::executeQuery(const VarString & rSql, 
										CursorTypeEnum    eCursorType /* = RST_DEFAULT */)
{
	return getImpl()->executeQuery(rSql, eCursorType);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return reference to created resultset
ResultsetRef StatementRef::executeSingleQuery(const VarString & rSql)
{
	return getImpl()->executeSingleQuery(rSql);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return reference to created insertset
InsertsetRef StatementRef::executeInsert(const VarString & rSql, 
										 CursorTypeEnum eCursorType /* = RST_DEFAULT */)
{
	return getImpl()->executeInsert(rSql, eCursorType);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return number of rows affected by statement (update, delete, insert)
ExecuteReturnInfo StatementRef::execute(const VarString & rSql,
										bool  flagExcept /* = true */)
{
	return getImpl()->execute(rSql, flagExcept);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return result of aggregate select
basar::Decimal StatementRef::executeAggregate	(const VarString & rSql)
{
	return getImpl()->executeAggregate(rSql);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

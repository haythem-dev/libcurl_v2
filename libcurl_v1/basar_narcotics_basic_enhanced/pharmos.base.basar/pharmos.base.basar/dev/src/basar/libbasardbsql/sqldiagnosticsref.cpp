//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's reference class
 *  \author Michael Eichenlaub
 *  \date   12.08.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_sqldiagnosticsref.h"

#include "sqldiagnostics.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsRef::SqlDiagnosticsRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \param  pImpl shared pointer to implementation object
SqlDiagnosticsRef::SqlDiagnosticsRef ( SqlDiagSharedPtr pImpl )
                                     : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsRef::~SqlDiagnosticsRef()
{
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool SqlDiagnosticsRef::isNull() const
{
	return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void SqlDiagnosticsRef::reset()
{
	m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
const SqlDiagSharedPtr & SqlDiagnosticsRef::getImpl() const
{
	if (0 == m_pImpl.get())
		throw NullReferenceException(ExceptInfo ("basar.db.sql.SqlDiagnosticsRef.getImpl", 
												 "m_pImpl.get() == 0", 
												 __FILE__, __LINE__));

	return m_pImpl;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlDiagnosticsRef::getAllExcepts(VarString & outString) const
{
	getImpl()->getAllExcepts(outString);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void SqlDiagnosticsRef::absolute(Int32 exceptIndex)
{
	getImpl()->absolute(exceptIndex);
}

//----------------------------------------------------------------------------
//! \return number of current Informix SQL exceptions
//! \throw  no-throw
Int32 SqlDiagnosticsRef::getCountExcepts() const
{
	return getImpl()->getCountExcepts();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return name of dbserver
const VarString & SqlDiagnosticsRef::getDbServer() const
{
	return getImpl()->getDbServer();
}

//----------------------------------------------------------------------------
//! \return connection ID
//! \throw  no-throw
const VarString & SqlDiagnosticsRef::getConnId() const
{
	return getImpl()->getConnId();
}

//----------------------------------------------------------------------------
//! \return Informix SQL state
//! \throw  no-throw
const VarString & SqlDiagnosticsRef::getSqlState() const
{
	return getImpl()->getSqlState();
}

//----------------------------------------------------------------------------
//! \return Informix SQL code
//! \throw  no-throw
BLong SqlDiagnosticsRef::getSqlCode() const
{
	return getImpl()->getSqlCode();
}

//----------------------------------------------------------------------------
//! \return message text that describes exception
//! \throw  no-throw
const VarString & SqlDiagnosticsRef::getMsgTxt() const
{
	return getImpl()->getMsgTxt();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

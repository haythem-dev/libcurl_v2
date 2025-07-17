//----------------------------------------------------------------------------
/*! \file
 *  \brief  handler for SQL exceptions
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#include "sqlexcepthandler.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandler::SqlExceptHandler()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandler::~SqlExceptHandler()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandler::clearWarnings()
{
	m_SqlDiagRef.m_pImpl.reset();
}

//------------------------------------------------------------------------------
//!	\return sql diagnostics reference object
//! \throw no-throw
SqlDiagnosticsRef SqlExceptHandler::getWarnings() const
{
	return m_SqlDiagRef;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

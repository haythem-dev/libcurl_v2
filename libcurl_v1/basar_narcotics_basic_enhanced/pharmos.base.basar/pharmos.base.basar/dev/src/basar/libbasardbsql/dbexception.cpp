//----------------------------------------------------------------------------
/*! \file
 *  \brief  exception class for physical database errors
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbException::DbException ( const ExceptInfo & sInfo                             ,
						   const VarString  & exceptionType/* = "DbException" */,
						   const VarString  & addReason    /* = ""            */)
                         : Exception   (sInfo, exceptionType, addReason)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbException::~DbException ( )
{
}


//----------------------------------------------------------------------------
//! \throw  no-throw
//!
DbExceptionStatementLengthExceeded::DbExceptionStatementLengthExceeded(const ExceptInfo & sInfo ,
					const VarString  & exceptionType /*= "DbExceptionStatementLengthExceeded" */,
					const VarString  & addReason     /*= ""                                   */)
					: DbException (sInfo, exceptionType, addReason)
{
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

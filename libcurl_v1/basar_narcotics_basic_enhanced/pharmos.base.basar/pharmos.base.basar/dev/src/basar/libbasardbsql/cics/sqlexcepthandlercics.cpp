/*
 * @file
 *
 * @brief ...
 * @author Anke Klink

 * @date 2007
 */
//----------------------------------------------------------------------------

#include "sqlexcepthandlercics.h"
#include "cicsdefinitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return shared_ptr to SqlExceptHandlerCics instance
boost::shared_ptr < SqlExceptHandler >	SqlExceptHandlerCics::create()
{
	return   boost::shared_ptr < SqlExceptHandler >(new SqlExceptHandlerCics( ));
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
SqlExceptHandlerCics::SqlExceptHandlerCics	()
{
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
SqlExceptHandlerCics::~SqlExceptHandlerCics	()
{
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return true (always)
bool SqlExceptHandlerCics::checkSqlState(const VarString & ,
										 const VarString & ,
										 UInt32			   ,
										 bool              )
{
	return true;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return returns the last Error as DBErrorCode::Code
DBErrorCode::Code SqlExceptHandlerCics::getLastError()
{
	return CicsDefinitions::getLastError();
}

DBError SqlExceptHandlerCics::getRawDBError()
{
    DBError retVal;

    CicsErrorInfo cicsError = CicsDefinitions::getLastCicsError();

    retVal.m_rawMainError = cicsError.m_errorCode;
    retVal.m_errorMsg = cicsError.m_errorMessage;

    return retVal;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//! \return true (always)
bool SqlExceptHandlerCics::isSqlStateNotFound(
	const VarString &  ,	//!< text description
	const VarString &  ,	//!< file where exception is thrown
	UInt32					//!< line of file where exception is thrown
	)
{
	return true;
}

// ------------------------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandlerCics::createWarnings()
{
}

// ------------------------------------------------------------------------------------------------
// namespace
}
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

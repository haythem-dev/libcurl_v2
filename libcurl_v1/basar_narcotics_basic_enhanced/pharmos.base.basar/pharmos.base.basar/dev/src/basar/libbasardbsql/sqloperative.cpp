//----------------------------------------------------------------------------
/*! \file
 *  \brief  base class for data manipulation classes
 *  \author Michael Eichenlaub
 *  \date   12.08.2005
 */
//----------------------------------------------------------------------------

#include "sqloperative.h"
#include "sqlexcepthandler.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return true if space is found after searchvaluegiven, false otherwise
bool SqlOperative::checkStmtStart(const VarString & rStmt, 
                                  const VarString & searchvaluegiven)
{
    bool      retval = false;

    VarString test(rStmt);
    test.trim(VarString::FIRST);

    if (test.length() > searchvaluegiven.length())
    {
        VarString searchvalue(searchvaluegiven);

		searchvalue.lower();
        test       .lower();

        if (0 == test.find(searchvalue))	
        {
            if ( isspace(static_cast<UInt8> ( test.at(searchvaluegiven.length()) )) )
                retval = true;	
        }
    }

    return retval;	
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
SqlOperative::~SqlOperative()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlOperative::clearWarnings()
{
	m_SqlExceptHnd->clearWarnings();
}

//------------------------------------------------------------------------------
//!	\return sql diagnostics reference object
//! \throw no-throw
SqlDiagnosticsRef SqlOperative::getWarnings() const
{
	return m_SqlExceptHnd->getWarnings();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return true if last executed statement didn't cause an error, false otherwise
bool SqlOperative::checkSqlState(const VarString & rWhat,
								 const VarString & rFile,
								 UInt32			   line,
								 bool              flagExcept /* = true */)
{
	return m_SqlExceptHnd->checkSqlState (rWhat, rFile, line, flagExcept);
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return DBErrorCode indicating last occured error
DBErrorCode::Code SqlOperative::getLastError()
{
    return m_SqlExceptHnd->getLastError ();
}


DBError SqlOperative::getRawDBError() const
{
    return m_SqlExceptHnd->getRawDBError();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \retval true if no data found
//! \retval false if data found
bool SqlOperative::isSqlStateNotFound(const VarString & rWhat,
									  const VarString & rFile,
									  UInt32			line)
{
	return m_SqlExceptHnd->isSqlStateNotFound(rWhat, rFile, line);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

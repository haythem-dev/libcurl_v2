//----------------------------------------------------------------------------
/*! \file
*  \brief  handler for SQL exceptions (infx)
*  \author Michael Eichenlaub
*  \date   11.08.2005
*/
//----------------------------------------------------------------------------

#include "sqlexcepthandlerinfx.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasardbsql_exceptions.h"
#include "sqldiagnosticsinfx.h"

#include <esqlc.h>

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \return	shared pointer to sql exception handler implementation object
//! \throw  no-throw
boost::shared_ptr < SqlExceptHandler > SqlExceptHandlerInfx::create ()
{
    return boost::shared_ptr < SqlExceptHandler >(new SqlExceptHandlerInfx);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandlerInfx::SqlExceptHandlerInfx()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlExceptHandlerInfx::~SqlExceptHandlerInfx()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \retval true, if no data found
//! \retval false, if data found
bool SqlExceptHandlerInfx::isSqlStateNotFound(
	const VarString & rWhat,
    const VarString & rFile,
    UInt32			  line)
{
    switch (infxGetSqlState())
    {
    case SQL_WARNING:
		{
			createWarnings();

			log4cplus::Logger logger = cmnutil::Logging::getInstance(rWhat);

			if(logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) 
			{
				VarString out, tmp;

				out.format("isSqlStateNotFound: SQL WARNING @ %s\n", rWhat.c_str());
				getWarnings().getAllExcepts(tmp);

				out += tmp;
				logger.forcedLog(log4cplus::WARN_LOG_LEVEL, out, rFile.c_str(), line);
			}
		}

		break;

    case SQL_ERROR:			
        throw DbExceptionInfx(ExceptInfo(rWhat, "SQLSTATE == ERROR", rFile, line));

    case SQL_NOTFOUND:
        return true;

    default:
        clearWarnings();
		break;
    }

    return false;
}

//----------------------------------------------------------------------------
//! \throw DbExceptionInfx
//! \return true in case of no error, false (or DbExceptionInfx depending on flagExcept) otherwise
bool SqlExceptHandlerInfx::checkSqlState(
	const VarString & rWhat     ,
    const VarString & rFile     ,
    UInt32		      line      ,
    bool              flagExcept)
{
    switch (infxGetSqlState())
    {
    case SQL_NOTFOUND:	// may not occur -> fall through to ERROR
    case SQL_ERROR:
        {
            if ( flagExcept )
            {
                throw DbExceptionInfx(ExceptInfo(rWhat, "SQLSTATE == ERROR", rFile, line));
            }
            else
            {
                return false;
            }
        }
        break; //will not be reached
    case SQL_WARNING:
        if ( flagExcept )
        {
            createWarnings();

            log4cplus::Logger logger = basar::cmnutil::Logging::getInstance(rWhat);

            if(logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) 
            {
                VarString out, tmp;

                out.format("checkSqlState: SQL WARNING @ %s\n", rWhat.c_str());
                getWarnings().getAllExcepts(tmp);

                out += tmp;
                logger.forcedLog(log4cplus::WARN_LOG_LEVEL, out, rFile.c_str(), line);
            }
        }
        break;

    default:
        clearWarnings();
        break;
    }

    return true;
}
//------------------------------------------------------------------------------
//! \throw no-throw
//! \return returns the last Error as DBErrorCode::Code
DBErrorCode::Code SqlExceptHandlerInfx::getLastError()
{
    // Could not insert new row - duplicate value in a UNIQUE INDEX column
    const Int32 duplErrorIndex     = -239; 
    // Unique constraint <constraint-name> violated
    const Int32 duplErrorContraint = -268;
    // ISAM error: duplicate value for a record with unique key
    const Int32 duplIsamError      = -100;

    DBErrorCode::Code     retval = DBErrorCode::SUCCESS;
    struct InfxErrorCodes errors = infxGetErrorCodes();

    if (0 != errors.main || 0 != errors.isam )
    {
        if ( (duplErrorIndex     == errors.main) || 
             (duplErrorContraint == errors.main) || 
             (duplIsamError      == errors.isam)   )
        {
            retval = DBErrorCode::INSERT_DUPL;
        }
        else  
        {
            retval = DBErrorCode::NONSPECIFIC;
        }
    }

    return retval;
}

DBError SqlExceptHandlerInfx::getRawDBError()
{
    DBError retVal;

    struct InfxErrorCodes error = infxGetErrorCodes();

    retVal.m_rawMainError = error.main;
    retVal.m_rawSubError = error.isam;

    createWarnings();

    m_SqlDiagRef.getAllExcepts(retVal.m_errorMsg);

    return retVal;

}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void SqlExceptHandlerInfx::createWarnings()
{
    m_SqlDiagRef.m_pImpl = SqlDiagnosticsInfx::create();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

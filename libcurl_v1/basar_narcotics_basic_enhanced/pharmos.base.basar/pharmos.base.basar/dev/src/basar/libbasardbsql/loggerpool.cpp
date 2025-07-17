//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//---------------------------------------------------------------------------

using log4cplus::Logger;
using basar::cmnutil::Logging;

//! common logger prefix in libbasardbsql
#define LOGGER_BASARDBSQL	"basar.db.sql."

//---------------------------------------------------------------------------
//! \brief get logging instance for ConnectionInfx
//! \return logging instance for ConnectionInfx
Logger & LoggerPool::connInfx   ()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Connection.Infx");
    return log;
}

//! \brief get logging instance for ConnectionOdbc
//! \return logging instance for ConnectionOdbc
Logger & LoggerPool::connOdbc	()
{
	static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Connection.Odbc");
	return log;
}

//! \brief get logging instance for ConnectionCics
//! \return logging instance for ConnectionCics
Logger & LoggerPool::connCics	()
{    
	static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Connection.Cics");
	return log;
}

//! \brief get logging instance for StatementInfx
//! \return logging instance for StatementInfx
Logger & LoggerPool::stmtInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx");
	return log;
}

//! \brief get logging instance for StatementInfx::execute
//! \return logging instance for StatementInfx::execute
Logger & LoggerPool::stmtInfxExec			()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx.execute");
	return log;
}

//! \brief get logging instance for StatementInfx::executeQuery
//! \return logging instance for StatementInfx::executeQuery
Logger & LoggerPool::stmtInfxExecQuery		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx.executeQuery");
	return log;
}

//! \brief get logging instance for StatementInfx::executeSingleQuery
//! \return logging instance for StatementInfx::executeSingleQuery
Logger & LoggerPool::stmtInfxExecSingleQuery()
{    
	static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx.executeSingleQuery");
	return log;
}

//! \brief get logging instance for StatementInfx::executeInsert
//! \return logging instance for StatementInfx::executeInsert
Logger & LoggerPool::stmtInfxExecInsert		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx.executeInsert");
	return log;
}

//! \brief get logging instance for StatementInfx::executeAggregate
//! \return logging instance for StatementInfx::executeAggregate
Logger & LoggerPool::stmtInfxExecAggregate	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Infx.executeAggregate");
	return log;
}


//! \brief get logging instance for StatementOdbc
//! \return logging instance for StatementOdbc
Logger & LoggerPool::stmtOdbc				()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Odbc");
	return log;
}

//! \brief get logging instance for StatementOdbc::execute
//! \return logging instance for StatementOdbc::execute
Logger & LoggerPool::stmtOdbcExec			()
{    
	static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Odbc.execute");
	return log;
}

//! \brief get logging instance for StatementOdbc::executeQuery
//! \return logging instance for StatementOdbc::executeQuery
Logger & LoggerPool::stmtOdbcExecQuery		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Odbc.execQuery");
	return log;
}

//! \brief get logging instance for StatementOdbc::executeAggregate
//! \return logging instance for StatementOdbc::executeAggregate
Logger & LoggerPool::stmtOdbcExecAggregate	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Odbc.executeAggregate");
	return log;
}


//! \brief get logging instance for StatementCics
//! \return logging instance for StatementCics
Logger & LoggerPool::stmtCics				()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Cics");
	return log;
}

//! \brief get logging instance for StatementCics::execute
//! \return logging instance for StatementCics::execute
Logger & LoggerPool::stmtCicsExec			()
{    
	static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Cics.execute");
	return log;
}

//! \brief get logging instance for StatementCics::executeQuery
//! \return logging instance for StatementCics::executeQuery
Logger & LoggerPool::stmtCicsExecQuery		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Cics.execQuery");
	return log;
}

//! \brief get logging instance for StatementCics::executeAggregate
//! \return logging instance for StatementCics::executeAggregate
Logger & LoggerPool::stmtCicsExecAggregate	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Statement.Cics.executeAggregate");
	return log;
}



//! \brief get logging instance for PreparedStatementInfx
//! \return logging instance for PreparedStatementInfx
Logger & LoggerPool::preparedStmtInfx		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx");
	return log;
}

//! \brief get logging instance for PreparedStatementInfx::execute
//! \return logging instance for PreparedStatementInfx::execute
Logger & LoggerPool::preparedStmtInfxExec	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx.execute");
	return log;
}

//! \brief get logging instance for PreparedStatementInfx::executeQuery
//! \return logging instance for PreparedStatementInfx::executeQuery
Logger & LoggerPool::preparedStmtInfxExecQuery	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx.executeQuery");
	return log;
}

//! \brief get logging instance for PreparedStatementInfx::executeSingleQuery
//! \return logging instance for PreparedStatementInfx::executeSingleQuery
Logger & LoggerPool::preparedStmtInfxExecSingleQuery()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx.executeSingleQuery");
	return log;
}

//! \brief get logging instance for PreparedStatementInfx::executeInsert
//! \return logging instance for PreparedStatementInfx::executeInsert
Logger & LoggerPool::preparedStmtInfxExecInsert	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx.executeInsert");
	return log;
}

//! \brief get logging instance for PreparedStatementInfx::set... values
//! \return logging instance for PreparedStatementInfx::set... values
Logger & LoggerPool::preparedStmtInfxSetVal	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Infx.setVal");
	return log;
}


//! \brief get logging instance for PreparedStatementOdbc
//! \return logging instance for PreparedStatementOdbc
Logger & LoggerPool::preparedStmtOdbc	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Odbc");
	return log;
}

//! \brief get logging instance for PreparedStatementOdbc::execute
//! \return logging instance for PreparedStatementOdbc::execute
Logger & LoggerPool::preparedStmtOdbcExec()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Odbc.execute");
	return log;
}

//! \brief get logging instance for PreparedStatementOdbc::executeQuery
//! \return logging instance for PreparedStatementOdbc::executeQuery
Logger & LoggerPool::preparedStmtOdbcExecQuery	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Odbc.execQuery");
	return log;
}

//! \brief get logging instance for PreparedStatementOdbc::set... values
//! \return logging instance for PreparedStatementOdbc::set... values
Logger & LoggerPool::preparedStmtOdbcSetVal	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Odbc.setVal");
	return log;
}


//! \brief get logging instance for PreparedStatementCics
//! \return logging instance for PreparedStatementCics
Logger & LoggerPool::preparedStmtCics	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Cics");
	return log;
}

//! \brief get logging instance for PreparedStatementCics::execute
//! \return logging instance for PreparedStatementCics::execute
Logger & LoggerPool::preparedStmtCicsExec()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Cics.execute");
	return log;
}

//! \brief get logging instance for PreparedStatementCics::executeQuery
//! \return logging instance for PreparedStatementCics::executeQuery
Logger & LoggerPool::preparedStmtCicsExecQuery()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Cics.execQuery");
	return log;
}

//! \brief get logging instance for PreparedStatementCics::set... values
//! \return logging instance for PreparedStatementCics::set... values
Logger & LoggerPool::preparedStmtCicsSetVal	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "PreparedStatement.Cics.setVal");
	return log;
}



//! \brief get logging instance for Resultset
//! \return logging instance for Resultset
Logger & LoggerPool::resSet	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset");
	return log;
}

//! \brief get logging instance for ResultsetInfx
//! \return logging instance for ResultsetInfx
Logger & LoggerPool::resSetInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx");
	return log;
}

//! \brief get logging instance for ResultsetInfx::get... values
//! \return logging instance for ResultsetInfx::get... values
Logger & LoggerPool::resSetInfxGetVal()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.getVal");
	return log;
}


//! \brief get logging instance for ResultsetInfxMulti
//! \return logging instance for ResultsetInfxMulti
Logger & LoggerPool::resSetInfxMlt	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.Multi");
	return log;
}

//! \brief get logging instance for ResultsetInfxMulti::next
//! \return logging instance for ResultsetInfxMulti::next
Logger & LoggerPool::resSetInfxMltNext	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.Multi.next");
	return log;
}

//! \brief get logging instance for ResultsetInfxMulti::execCurrentRow
//! \return logging instance for ResultsetInfxMulti::execCurrentRow
Logger & LoggerPool::resSetInfxMltExecCurrentRow()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.Multi.execCurrentRow");
	return log;
}


//! \brief get logging instance for ResultsetInfxSingle
//! \return logging instance for ResultsetInfxSingle
Logger & LoggerPool::resSetInfxSgl	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.Single");
	return log;
}

//! \brief get logging instance for ResultsetInfxSingle::next
//! \return logging instance for ResultsetInfxSingle::next
Logger & LoggerPool::resSetInfxSglNext	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Infx.Single.next");
	return log;
}


//! \brief get logging instance for ResultsetOdbc
//! \return logging instance for ResultsetOdbc
Logger & LoggerPool::resSetOdbc	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Odbc");
	return log;
}

//! \brief get logging instance for ResultsetOdbc::get... values
//! \return logging instance for ResultsetOdbc::get... values
Logger & LoggerPool::resSetOdbcGetVal()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Odbc.getVal");
	return log;
}


//! \brief get logging instance for ResultsetCics
//! \return logging instance for ResultsetCics
Logger & LoggerPool::resSetCics		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Cics");
	return log;
}

//! \brief get logging instance for ResultsetCics::get... values
//! \return logging instance for ResultsetCics::get... values
Logger & LoggerPool::resSetCicsGetVal()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Cics.getVal");
	return log;
}

//! \brief get logging instance for ResultsetCics::next
//! \return logging instance for ResultsetCics::next
Logger & LoggerPool::resSetCicsNext	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Resultset.Cics.next");
	return log;
}



//! \brief get logging instance for Insertset
//! \return logging instance for Insertset
Logger & LoggerPool::insSet		()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Insertset");
	return log;
}

//! \brief get logging instance for InsertsetInfx
//! \return logging instance for InsertsetInfx
Logger & LoggerPool::insSetInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Insertset.Infx");
	return log;
}

//! \brief get logging instance for InsertsetInfx::insertRow
//! \return logging instance for InsertsetInfx::insertRow
Logger & LoggerPool::insSetInfxInsertRow()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "Insertset.Infx.insertRow");
	return log;
}



//! \brief get logging instance for ManagerImpl
//! \return logging instance for ManagerImpl
Logger & LoggerPool::manager()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "ManagerImpl"   );
	return log;
}


//! \brief get logging instance for CicsDefinitions
//! \return logging instance for CicsDefinitions
Logger & LoggerPool::cicsDefinitions()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "CicsDefinitions");
	return log;
}


//! \brief get logging instance for DbLocaleInfx
//! \return logging instance for DbLocaleInfx
Logger & LoggerPool::dbLocaleInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "DbLocaleInfx");						//!< logger for DbLocaleInfx
	return log;
}

//! \brief get logging instance for SetEnvInfx
//! \return logging instance for SetEnvInfx
Logger & LoggerPool::setEnvInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "SetEnvInfx");						//!< logger for DbLocaleInfx
	return log;
}

//! \brief get logging instance for SqlHostsInfx
//! \return logging instance for SqlHostsInfx
Logger & LoggerPool::sqlHostsInfx	()
{
    static Logger log = Logging::getInstance(LOGGER_BASARDBSQL "SqlHostsInfx");						//!< logger for DbLocaleInfx
	return log;
}



//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

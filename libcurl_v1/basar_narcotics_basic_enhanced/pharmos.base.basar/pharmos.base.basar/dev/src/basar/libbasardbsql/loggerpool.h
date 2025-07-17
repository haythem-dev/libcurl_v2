//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_LOGGERPOOL_H
#define GUARD_LIBBASARDBSQL_LOGGERPOOL_H

//----------------------------------------------------------------------------
#include <log4cplus/logger.h>

//----------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger &	connInfx();							//!< logger for ConnectionInfx
	static log4cplus::Logger &	connOdbc();							//!< logger for ConnectionOdbc
    static log4cplus::Logger &	connCics();							//!< logger for ConnectionCics
	
	static log4cplus::Logger &	stmtInfx();							//!< logger for StatementInfx
	static log4cplus::Logger &	stmtInfxExec();						//!< logger for StatementInfx::execute
	static log4cplus::Logger &	stmtInfxExecQuery();				//!< logger for StatementInfx::executeQuery
	static log4cplus::Logger &	stmtInfxExecSingleQuery();			//!< logger for StatementInfx::executeSingleQuery
	static log4cplus::Logger &	stmtInfxExecInsert();				//!< logger for StatementInfx::executeInsert
	static log4cplus::Logger &	stmtInfxExecAggregate();			//!< logger for StatementInfx::executeAggregate

	static log4cplus::Logger &	stmtOdbc();							//!< logger for StatementOdbc
	static log4cplus::Logger &	stmtOdbcExec();						//!< logger for StatementOdbc::execute
	static log4cplus::Logger &	stmtOdbcExecQuery();				//!< logger for StatementOdbc::executeQuery
	static log4cplus::Logger &	stmtOdbcExecAggregate();			//!< logger for StatementOdbc::executeAggregate


	static log4cplus::Logger &	stmtCics();	                        //!< logger for StatementCics
	static log4cplus::Logger &	stmtCicsExec();						//!< logger for StatementCics::execute
	static log4cplus::Logger &	stmtCicsExecQuery();				//!< logger for StatementCics::executeQuery
	static log4cplus::Logger &	stmtCicsExecAggregate();			//!< logger for StatementCics::executeAggregate
	
	static log4cplus::Logger &	preparedStmtInfx();					//!< logger for PreparedStatementInfx
	static log4cplus::Logger &	preparedStmtInfxExec();				//!< logger for PreparedStatementInfx::execute
	static log4cplus::Logger &	preparedStmtInfxExecQuery();		//!< logger for PreparedStatementInfx::executeQuery
	static log4cplus::Logger &	preparedStmtInfxExecSingleQuery();	//!< logger for PreparedStatementInfx::executeSingleQuery
	static log4cplus::Logger &	preparedStmtInfxExecInsert();		//!< logger for PreparedStatementInfx::executeInsert
	static log4cplus::Logger &	preparedStmtInfxSetVal();			//!< logger for PreparedStatementInfx::set... values

	static log4cplus::Logger &	preparedStmtOdbc();					//!< logger for PreparedStatementOdbc
	static log4cplus::Logger &	preparedStmtOdbcExec();				//!< logger for PreparedStatementOdbc::execute
	static log4cplus::Logger &	preparedStmtOdbcExecQuery();		//!< logger for PreparedStatementOdbc::executeQuery
	static log4cplus::Logger &	preparedStmtOdbcSetVal();			//!< logger for PreparedStatementOdbc::set... values

    static log4cplus::Logger &	preparedStmtCics();					//!< logger for PreparedStatementCics
	static log4cplus::Logger &	preparedStmtCicsExec();				//!< logger for PreparedStatementCics::execute
	static log4cplus::Logger &	preparedStmtCicsExecQuery();		//!< logger for PreparedStatementCics::executeQuery
	static log4cplus::Logger &	preparedStmtCicsSetVal();			//!< logger for PreparedStatementCics::set... values

	static log4cplus::Logger &	resSet();							//!< logger for Resultset
	static log4cplus::Logger &	resSetInfx();						//!< logger for ResultsetInfx
	static log4cplus::Logger &	resSetInfxGetVal();					//!< logger for ResultsetInfx::get... values
	static log4cplus::Logger &	resSetInfxMlt();					//!< logger for ResultsetInfxMulti
	static log4cplus::Logger &	resSetInfxMltNext();				//!< logger for ResultsetInfxMulti::next
	static log4cplus::Logger &	resSetInfxMltExecCurrentRow();		//!< logger for ResultsetInfxMulti::execCurrentRow
	static log4cplus::Logger &	resSetInfxSgl();					//!< logger for ResultsetInfxSingle
	static log4cplus::Logger &	resSetInfxSglNext();				//!< logger for ResultsetInfxSingle::next

	static log4cplus::Logger &	resSetOdbc();						//!< logger for ResultsetOdbc
	static log4cplus::Logger &	resSetOdbcGetVal();					//!< logger for ResultsetOdbc::get... values

	static log4cplus::Logger &	resSetCics();						//!< logger for ResultsetCics
	static log4cplus::Logger &	resSetCicsGetVal();					//!< logger for ResultsetCics::get... values
	static log4cplus::Logger &	resSetCicsNext();					//!< logger for ResultsetCics::next

	static log4cplus::Logger &	cicsDefinitions();                  //!< logger for CicsDefinitions

	static log4cplus::Logger &	insSet();							//!< logger for Insertset
	static log4cplus::Logger &	insSetInfx();						//!< logger for InsertsetInfx
	static log4cplus::Logger &	insSetInfxInsertRow();				//!< logger for InsertsetInfx::insertRow

	static log4cplus::Logger &	manager();							//!< logger for ManagerImpl

	static log4cplus::Logger &	dbLocaleInfx();						//!< logger for DbLocaleInfx
	static log4cplus::Logger &	setEnvInfx();						//!< logger for SetEnvInfx
	static log4cplus::Logger &	sqlHostsInfx();						//!< logger for SqlHostsInfx
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

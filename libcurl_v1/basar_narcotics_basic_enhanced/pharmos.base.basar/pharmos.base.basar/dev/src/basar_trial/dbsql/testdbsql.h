
#ifndef GUARD_TESTDBSQL_H
#define GUARD_TESTDBSQL_H

#include <libbasarcmnutil_logging.h>

//------------------------------------------------------------------------------
void	misc			();
void	connection		();
void	version			();
void    prepstmt        ();

//-----------------------------------------------------------------------------
namespace basar
{
	namespace db
	{
		namespace sql
		{
			class ConnectionRef;
		};
	};
};

//------------------------------------------------------------------------------
class LoggerPool
{
public:
	static log4cplus::Logger s_LoggerStmt;	
};

//------------------------------------------------------------------------------
namespace __statement_odbc__
{
	void	statementodbc	(basar::db::sql::ConnectionRef connRef1,
							 basar::db::sql::ConnectionRef connRef2);
}
//------------------------------------------------------------------------------
namespace __statement_infx__
{
	void	statementinfx	(basar::db::sql::ConnectionRef connRef1,
							 basar::db::sql::ConnectionRef connRef2);
}

//------------------------------------------------------------------------------
namespace __statement_cics__
{
	void	statementcics	(basar::db::sql::ConnectionRef connRef);
}

#endif


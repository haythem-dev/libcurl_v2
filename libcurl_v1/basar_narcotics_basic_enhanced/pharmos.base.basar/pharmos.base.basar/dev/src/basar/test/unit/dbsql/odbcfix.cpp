#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "odbcfix.h"
#include "testconnection.h"

//---------------------------------------------------------------
using basar::db::sql::DatabaseInfo;
using basar::db::sql::ConnectionRef;
using basar::db::sql::Manager;

//---------------------------------------------------------------
OdbcFix::OdbcFix()
{
#ifdef HAS_ODBC

#ifdef INFX_CSDK_370
	dbODBC .setODBC(ODBC_DSN_WEST);
	dbESQLC.setInfx(ESQLC_DBSERVER, ESQLC_DB_WEST);
#endif

#endif
}

//---------------------------------------------------------------
OdbcFix::~OdbcFix()
{
#ifdef HAS_ODBC

#ifdef INFX_CSDK_370
	ConnectionRef connODBC  = Manager::getInstance().createConnect(dbODBC );
	ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);

	// workaround for Informix CSDK 3.70 Window issue
	// close ODBC Connection to Informix DB only after ESQLC has been opened
	connODBC .close();
	connESQLC.close();
#endif

#endif
}
	
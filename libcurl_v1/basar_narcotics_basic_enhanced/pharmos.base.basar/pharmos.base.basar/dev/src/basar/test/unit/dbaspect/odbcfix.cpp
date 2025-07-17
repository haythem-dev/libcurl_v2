#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "odbcfix.h"

#include "libbasardbsql_databaseinfo.h"

//---------------------------------------------------------------
OdbcFix::OdbcFix()
{
}

//---------------------------------------------------------------
OdbcFix::~OdbcFix()
{
#ifdef HAS_ODBC

#ifdef INFX_CSDK_370
	using basar::db::aspect::ConnectionRef;
	using basar::db::aspect::Manager;

	ConnectionRef::DatabaseInfo dbODBC;
	ConnectionRef::DatabaseInfo dbESQLC;

	dbODBC .setODBC(INFX_DBSERVER);
	dbESQLC.setInfx(ESQLC_DBSERVER, INFX_DB_CYR);

	ConnectionRef connODBC  = Manager::getInstance().createConnect(dbODBC );
	ConnectionRef connESQLC = Manager::getInstance().createConnect(dbESQLC);

	// workaround for Informix CSDK 3.70 Window issue
	// close ODBC Connection to Informix DB only after ESQLC has been opened
	connODBC .close();
	connESQLC.close();
#endif

#endif
}

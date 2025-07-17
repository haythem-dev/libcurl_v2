
#include <libbasardbsql.h>

#include "testdbsql.h"

#include <assert.h>
#include <stdio.h>

using basar::db::sql::ConnectionRef;
using basar::db::sql::DatabaseInfo;
using basar::cmnutil::Singleton;
using basar::db::sql::ManagerImpl;
using basar::db::sql::Manager;
using basar::db::sql::Connection;

//------------------------------------------------------------------------------
ConnectionRef test_conninfx (const char * const user, const char * const passwd)
{
	ManagerImpl & manager = Manager::getInstance();
	DatabaseInfo  dbInfo;

	dbInfo.dbServer = "zdev21_tcp";//infx
	dbInfo.database = "ode21";		//infx
	dbInfo.eConnectType = basar::INFORMIX;

	dbInfo.user     = user;
	dbInfo.passwd   = passwd; 

	printf("\n\n");
	printf("--- test_conn infx: ---------------------------------\n");
	printf("\n");
	printf("connect :\n");
	printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
	printf("  database : %s\n", dbInfo.database.c_str());
	printf("  user     : %s\n", dbInfo.user    .c_str());
	printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
	printf("\n");

	ConnectionRef connRef = manager.createConnect(dbInfo);
	
	dbInfo  = connRef.getDbInfo   (); 


	connRef.begin ();
	connRef.commit();

	connRef.begin   ();
	connRef.rollback();

	connRef.setIsolationLevel(basar::INFX_DIRTYREAD);
	connRef.setLockModeWait  (basar::WAIT);
	connRef.setLockModeWait  (basar::WAITSECS, 20);

	//connRef.close();


//--- Connection-Manager -----
	basar::db::sql::ConnectionListSize iSize = 0;
	iSize = manager.sizeOpen();
	iSize = manager.size();

	/*connRef.close();
	iSize = manager.sizeOpen();
	iSize = manager.size();*/

	/*basar::db::sql::CollConnection::const_iterator itbegin = manager.begin();
	basar::db::sql::CollConnection::const_iterator itend   = manager.end();

	basar::VarString connId;
	bool open = false;
	
	while (itbegin != itend)
	{
		dbInfo = manager.getConnInfo(itbegin, connId);
		open   = manager.isOpen(itbegin);
		++itbegin;
	}
	++itbegin;*/
	
	return connRef;
}

//------------------------------------------------------------------------------
ConnectionRef test_connodbc1 (const char * const user, const char * const passwd)
{
	ManagerImpl & manager = Manager::getInstance();
	DatabaseInfo  dbInfo;

	dbInfo.dbServer = "";
	dbInfo.database = "zdev21_ode21";
	dbInfo.user     = user;
	dbInfo.passwd   = passwd; //odbc sofumi1 
	dbInfo.eConnectType = basar::ODBC;

	printf("\n\n");
	printf("--- test_conn odbc1: ---------------------------------\n");
	printf("\n");
	printf("connect :\n");
	printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
	printf("  database : %s\n", dbInfo.database.c_str());
	printf("  user     : %s\n", dbInfo.user    .c_str());
	printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
	printf("\n");

	ConnectionRef connRef = manager.createConnect(dbInfo);
	
	dbInfo  = connRef.getDbInfo   (); 


	connRef.begin ();
	connRef.commit();

	connRef.begin   ();
	connRef.rollback();

	return connRef;
}

//------------------------------------------------------------------------------
ConnectionRef test_connodbc2 (const char * const user, const char * const passwd)
{
	ManagerImpl & manager = Manager::getInstance();
	DatabaseInfo  dbInfo;

	dbInfo.dbServer = "";
	dbInfo.database = "zdev21_ode21";
	dbInfo.user     = user;
	dbInfo.passwd   = passwd; 
	dbInfo.eConnectType = basar::ODBC;

	printf("\n\n");
	printf("--- test_conn odbc2: ---------------------------------\n");
	printf("\n");
	printf("connect :\n");
	printf("  dbserver : %s\n", dbInfo.dbServer.c_str());
	printf("  database : %s\n", dbInfo.database.c_str());
	printf("  user     : %s\n", dbInfo.user    .c_str());
	printf("  passwd   : %s\n", dbInfo.passwd  .c_str());
	printf("\n");

	ConnectionRef connRef = manager.createConnect(dbInfo);
	
	dbInfo  = connRef.getDbInfo   (); 


	connRef.begin ();
	connRef.commit();

	connRef.begin   ();
	connRef.rollback();

	return connRef;
}

//------------------------------------------------------------------------------

#define CICS_CONFIG_WEST		"BasarDefault_west"
#define CICS_CONFIG_CYR			"BasarDefault_cyr"
#define CICS_HOST				"129.210.15.02"
#define CICS_PORT				"3102"
#define CICS_START				"TCAS"
#define CICS_USER				""
#define CICS_PWD				""

void multiconn()
{
	DatabaseInfo infxbg, infx, infxdef;
	DatabaseInfo odbc, odbcbg, odbc35, odbcms;
	DatabaseInfo cics;

	infx   .setInfx("zdev21_tcp", "ode21");
	infxbg .setInfx("zdev21_tcp", "obg21");
	infxdef.setInfx("zdev21_tcp", "ode21", "devuser", "phoenix");

	odbc  .setODBC("zdev21_ode21");
	odbc35.setODBC("zdev21_ode215");
	odbcbg.setODBC("zdev21_obg21");
	odbcms.setODBC("erwin");

	cics.setCICS(CICS_CONFIG_WEST, CICS_HOST, CICS_PORT, CICS_START, CICS_USER, CICS_PWD);

	for (int i = 0; i < 4; ++i)
	{
		try
		{
			{
				ConnectionRef connRef;
				assert(connRef.isNull());

				connRef = Manager::getInstance().createConnect(infx);
				assert(!connRef.isNull());
				connRef.reset();
				assert(connRef.isNull());
			}

			{
				ConnectionRef connRef2 = Manager::getInstance().createConnect(odbc);
				assert(!connRef2.isNull());
				connRef2.reset();
				assert(connRef2.isNull());
			}

		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			assert(ExceptionOccured);
		}
		catch (...)
		{
			std::cout << "unknown exception" << std::endl;
			const bool ExceptionOccured = false;
			assert(ExceptionOccured);
		}
	}
}

//------------------------------------------------------------------------------
void connection()
{
//#define CONNECT_INFX
//#define CONNECT_ODBC
//#define CONNECT_CICS
//#define CONNECT_ODBC_MS
//#define CONNECT_MY

	multiconn();

#ifdef CONNECT_INFX
	{
		ConnectionRef connRef1,
					  connRef2;

		//connRef1 = test_conninfx("devuser"     , "phoenix");		//infx
		connRef2 = test_conninfx("", "" );		//infx

		__statement_infx__::statementinfx(connRef1, connRef2);

		printf("\n\n");
		printf("--- disconnectinfx ---------------------------------\n");
		printf("\n");
	}
#endif

#ifdef CONNECT_ODBC
	{
		ConnectionRef connRef1,
					  connRef2;

		connRef2 = test_connodbc1("t.hoerat"     , "55handball1");	//odbc sofumi1_pcua1
		connRef1 = test_connodbc2("t.hoerat"     , "55handball1");	//odbc sofumi1_pps1cg
		
		__statement_odbc__::statementodbc(connRef1, connRef2);

		printf("\n\n");
		printf("--- disconnectodbc ---------------------------------\n");
		printf("\n");
	}
#endif

#ifdef CONNECT_ODBC_MS
	{
		DatabaseInfo  dbInfo;
		dbInfo.dbServer = "buoux";
		dbInfo.database = "buoux_but";
		dbInfo.user     = "sa";
		dbInfo.passwd   = "80286";
		dbInfo.eConnectType = basar::ODBC;

		ConnectionRef conn = Manager::getInstance().createConnect(dbInfo);

		basar::ConstString sql = "select id, colfloat, colsmfloat from butmaster;";

		basar::db::sql::StatementRef stmt = conn.createStatement();
		basar::db::sql::ResultsetRef res  = stmt.executeQuery(sql);


		basar::Int32  cnt = 0;
		
		printf("statement       : <%s>\n\n", sql);

		while (true == res.next())
		{
			++cnt;
			printf("fetched rows    : %d\n", cnt);

			printf("    value id        : %d\n", res.getInt32  (0));
			printf("    value float     : %f\n", res.getFloat64(1));
			printf("    value smallfloat: %f\n", res.getFloat32(2));
		}



		printf("\n\n");
		printf("--- disconnectodbc ---------------------------------\n");
		printf("\n");
	}
#endif

#ifdef CONNECT_MY
	{
		DatabaseInfo  dbInfo;
		dbInfo.dbServer = "buoux";
		dbInfo.database = "buoux_but";
		dbInfo.user     = "sa";
		dbInfo.passwd   = "80286";
		dbInfo.eConnectType = basar::ODBC;

		ConnectionRef conn = Manager::getInstance().createConnect(dbInfo);

		conn.setRawFetching(true);	


		basar::ConstString sql =	"UPDATE butmaster "
									"SET    id = ? "
									"WHERE  id  = ? "
									"and colch1 = ? "
									"and colch2 = ? "
									";";

		basar::db::sql::PreparedStatementRef stmt = conn.createPreparedStatement(sql);
		
		basar::Int32 i = 0;

		stmt.setInt32 (i++, 1);
		stmt.setInt32 (i++, 1);
		stmt.setString(i++, "a");
		stmt.setString(i++, "abcd");

		basar::db::sql::ExecuteReturnInfo ret = stmt.execute();
		assert(!ret.hasError());
		//assert(0 == ret.m_affectedRows);

		printf("\n\n");
		printf("--- disconnectodbc ---------------------------------\n");
		printf("\n");
	}
#endif

#ifdef CONNECT_CICS
	{
		DatabaseInfo  dbInfo;
		dbInfo.eConnectType = basar::CICS;
		dbInfo.dbServer     = "VSE94_82";
		dbInfo.database     = "";		
		dbInfo.user         = "";
		dbInfo.passwd       = "";

		ConnectionRef connRef =  Manager::getInstance().createConnect( dbInfo );
		
		__statement_cics__::statementcics(connRef);

		printf("\n\n");
		printf("--- disconnectcics ---------------------------------\n");
		printf("\n");
	}
#endif


	//Manager::clear();
}

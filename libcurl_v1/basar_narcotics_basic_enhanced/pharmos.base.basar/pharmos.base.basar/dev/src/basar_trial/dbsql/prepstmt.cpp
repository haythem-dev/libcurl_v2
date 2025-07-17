
#include "testdbsql.h"

#include <libbasardbsql.h>

#include <time.h>
#include <stdio.h>
#include <assert.h>

//--------------------------------------------
using basar::ConstString;
using basar::Int32;

using basar::db::sql::ConnectionRef;
using basar::db::sql::Manager;
using basar::db::sql::DatabaseInfo;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::CURS_HOLD_OVER_COMMIT;


static const Int32 MAXROWS = 100000;

//--------------------------------------------
static void insertRows(ConnectionRef conn)
{
	ConstString sql = "insert into but_var "
		              "(colser, colint) "
					  "values "
					  "(0, -9);";
	
	PreparedStatementRef stmt = conn.createPreparedStatement(sql);

	conn.begin();

	InsertsetRef ins = stmt.executeInsert();

	for (Int32 i = 0; i < MAXROWS; ++i)
	{
		ins.insertRow();
	}

	conn.commit();
}

//--------------------------------------------
static void deleteRows(ConnectionRef conn)
{
	ConstString sql = "delete from but_var "
		              "where colint = -9;";
	
	PreparedStatementRef stmt = conn.createPreparedStatement(sql);

	conn.begin();
	
	ExecuteReturnInfo ret = stmt.execute();

	assert(ret.getAffectedRows() == MAXROWS);

	conn.commit();
}


//--------------------------------------------
static ConnectionRef connect()
{
	DatabaseInfo  dbInfo;

	dbInfo.setInfx("zdev21_tcp",
			       "ode21");

	return Manager::getInstance().createConnect(dbInfo);
}

//--------------------------------------------
static void update_unprep(ConnectionRef conn)
{
	ConstString sql = "select * from but_var "
		              "where colint = -9 "
					  "for update;";

	ConstString upd = "update but_var "
		              "set colsmint = -99";
	
	PreparedStatementRef stmt = conn.createPreparedStatement(sql);

	conn.begin();

	ResultsetRef res = stmt.executeQuery();
	Int32        rows;

	while (res.next())
	{
		rows = res.execCurrentRow(upd);
		assert(1 == rows); 
	}
	
	conn.commit();
}

//--------------------------------------------
static void update_prep(ConnectionRef conn)
{
	ConstString sel = "select * from but_var "
		              "where colint = -9 "
					  "for update;";

	ConstString upd = "update but_var "
		              "set colsmint = -99";
	
	PreparedStatementRef stmtsel = conn.createPreparedStatement(sel);
	PreparedStatementRef stmtupd = conn.createPrepRowStmt      (upd, stmtsel);

	conn.begin();

	ResultsetRef      res = stmtsel.executeQuery();
	ExecuteReturnInfo ret;
	
	while (res.next())
	{
		ret = stmtupd.execute();
		assert(1 == ret.getAffectedRows() );
	}
	
	conn.commit();
}

//--------------------------------------------
static void update_transsingle(ConnectionRef conn)
{
	ConstString sel = "select * from but_var "
		              "where colint = -9 ";

	ConstString upd = "update but_var "
		              "set colsmint = -99 "
					  "where colser = ?";
	
	PreparedStatementRef stmtsel = conn.createPreparedStatement(sel);
	PreparedStatementRef stmtupd = conn.createPreparedStatement(upd);

	conn.begin();

	ResultsetRef      res = stmtsel.executeQuery();
	ExecuteReturnInfo ret;

	Int32 colser;

	while (res.next())
	{
		colser = res.getInt32(0);

		stmtupd.setInt32(0, colser);
		ret = stmtupd.execute();
		assert(1 == ret.getAffectedRows() );
	}
	
	conn.commit();
}

//--------------------------------------------
static void update_transmulti(ConnectionRef conn)
{
	ConstString sel = "select * from but_var "
		              "where colint = -9 ";

	ConstString upd = "update but_var "
		              "set colsmint = -99 "
					  "where colser = ?";
	
	PreparedStatementRef stmtsel = conn.createPreparedStatement(sel,
																CURS_HOLD_OVER_COMMIT);
	PreparedStatementRef stmtupd = conn.createPreparedStatement(upd);

	ResultsetRef      res = stmtsel.executeQuery();
	ExecuteReturnInfo ret;
	
	Int32 colser;

	while (res.next())
	{
		colser = res.getInt32(0);

		stmtupd.setInt32(0, colser);

		conn.begin();
		ret = stmtupd.execute();
		conn.commit();
		
		assert(1 == ret.getAffectedRows() );
	}
}

//--------------------------------------------
static void update_transwithout(ConnectionRef conn)
{
	ConstString sel = "select * from but_var "
		              "where colint = -9 ";

	ConstString upd = "update but_var "
		              "set colsmint = -99 "
					  "where colser = ?";
	
	PreparedStatementRef stmtsel = conn.createPreparedStatement(sel);
	PreparedStatementRef stmtupd = conn.createPreparedStatement(upd);

	ResultsetRef      res = stmtsel.executeQuery();
	ExecuteReturnInfo ret;
	
	Int32 colser;

	while (res.next())
	{
		colser = res.getInt32(0);

		stmtupd.setInt32(0, colser);
		ret = stmtupd.execute();
		
		assert(1 == ret.getAffectedRows() );
	}
}

//--------------------------------------------
void prepstmt()
{
	setbuf(stdout, 0);
	
	ConnectionRef conn = connect();

	printf("number of rows                  : %d\n", MAXROWS);

	{
		printf("inserting test rows             :");
		
		time_t start = time(0);

		insertRows(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}

	{
		printf("updating unprepared current row :");
		
		time_t start = time(0);

		update_unprep(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}
	
	{
		printf("updating prepared current row   :");
		
		time_t start = time(0);
		
		update_prep(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}

	{
		printf("updating one transaction        :");
		
		time_t start = time(0);
		
		update_transsingle(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}
	
	{
		printf("updating many transactions      :");
		
		time_t start = time(0);
		
		update_transmulti(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}
	
	{
		printf("updating without transactions   :");
		
		time_t start = time(0);
		
		update_transwithout(conn);

		time_t stop = time(0);
		double secs = difftime(stop, start);
		
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}
		{
		time_t start = time(0);

		printf("deleting test rows              :");

		deleteRows(conn);
	
		time_t stop = time(0);
		double secs = difftime(stop, start);
	
		printf(" elapsed time: %02d:%02d:%02d h\n",
				(int)secs / 3600,
				((int)secs - ((int)secs / 3600) * 3600) / 60,
				(int)secs % 60 );
	}
}


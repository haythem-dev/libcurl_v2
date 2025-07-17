
#include <libbasardbsql.h>
#include "testdbsql.h"

#include <sys/timeb.h>

//------------------------------------------------------------------------------
using basar::Int16;
using basar::Int32;
using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::ConstString;
using basar::UInt32;
using basar::VarString;

//------------------------------------------------------------------------------
namespace __statement_cics__
{

log4cplus::Logger logger = basar::cmnutil::Logging::getInstance("testdbsql");

//------------------------------------------------------------------------------
void OneSelect(ConnectionRef connect)
{
	static ConstString sqlSel = 

		"SELECT "
			"dkpar_name "
		"FROM  vsam.dkpar_ut "
		"WHERE dkpar_fil  = '01' "
		"AND   dkpar_name = '01' "
		"AND   dkpar_nr   = '03';";

	try
	{
    	// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			Int32     cnt = 0;
			VarString name;

			while (result.next())
			{
				name = result.getString("dkpar_name"  );

				++cnt;
			}

			VarString msg;

			msg.format("\n"
				"1. select: rows %d\n"
				"	dkpar_name (01)%s\n",
				cnt,
				name.c_str());

			BLOG_DEBUG(logger, msg.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
    }
}

//------------------------------------------------------------------------------
void TwoSelects(ConnectionRef connect)
{
	static ConstString sqlSel = 

		"SELECT "
			"dkpar_name "
		"FROM  vsam.dkpar_ut "
		"WHERE dkpar_fil  = '01' "
		"AND   dkpar_name = '01' "
		"AND   dkpar_nr   = '03';";

	try
	{
		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			Int32     cnt = 0;
			VarString name;

			while (result.next())
			{
				name = result.getString("dkpar_name"  );

				++cnt;
			}

			VarString msg;

			msg.format("\n"
				"1. select: rows %d\n"
				"	dkpar_name (01)%s\n",
				cnt,
				name.c_str());

			BLOG_DEBUG(logger, msg.c_str());
		}
        	// --- step 1 a 
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			Int32     cnt = 0;
			VarString name;

			while (result.next())
			{
				name = result.getString("dkpar_name"  );

				++cnt;
			}

			VarString msg;

			msg.format("\n"
				"2. select: rows %d\n"
				"	dkpar_name (01)%s\n",
				cnt,
				name.c_str());

			BLOG_DEBUG(logger, msg.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
    }
}

//------------------------------------------------------------------------------
void TwoSelectsConcurrent(ConnectionRef connect)
{
	static ConstString sqlSel = 

		"SELECT "
			"dkpar_name "
		"FROM  vsam.dkpar_ut "
		"WHERE dkpar_fil  = '01' "
		"AND   dkpar_name = '01' "
		"AND   dkpar_nr   = '03';";

	try
	{
        StatementRef      stmtGlobal;

		// --- step 1
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			Int32     cnt = 0;
			VarString name;

			while (result.next())
			{
				name = result.getString("dkpar_name"  );

				++cnt;
			}

			VarString msg;

			msg.format("\n"
				"1. select: rows %d\n"
				"	dkpar_name (01)%s\n",
				cnt,
				name.c_str());

			BLOG_DEBUG(logger, msg.c_str());
            
			stmtGlobal = stmt;
		}
        	// --- step 1 a 
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();
			result = stmt.executeQuery(sqlSel);

			Int32     cnt = 0;
			VarString name;

			while (result.next())
			{
				name = result.getString("dkpar_name"  );

				++cnt;
			}

			VarString msg;

			msg.format("\n"
				"2. select: rows %d\n"
				"	dkpar_name (01)%s\n",
				cnt,
				name.c_str());

			BLOG_DEBUG(logger, msg.c_str());
		}
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
    }
}

//------------------------------------------------------------------------------
void StmtPerf(ConnectionRef connect)
{
	static ConstString sqlSel = 

		"SELECT * "
		"FROM  vsam.dkrpr;";

	Int32     cnt = 0;
	timeb start, stop;
	

	try
	{
		{
			StatementRef      stmt;
			ResultsetRef      result;

			stmt   = connect.createStatement();

			ftime(&start);

			result = stmt.executeQuery(sqlSel);


			while (result.next() && (cnt < 1000))
			{
				++cnt;
			}
		}
	
		ftime(&stop);
	}
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
    }

	printf("StmtPerf: rows %d\n", cnt);
	printf("----------------------------------\n");
	printf("          %10d.%03d sec\n", 
		   (start.millitm > stop.millitm) ? (stop.time - start.time - 1): (stop.time - start.time),
		   (start.millitm > stop.millitm) ? (stop.millitm + 1000 - start.millitm ) : (stop.millitm - start.millitm));

}

//------------------------------------------------------------------------------
void statementcics(ConnectionRef conn)
{
	//OneSelect (conn);
	//TwoSelects(conn);
	//TwoSelectsConcurrent(conn);
	StmtPerf(conn);
}

//------------------------------------------------------------------------------
}	// namespace
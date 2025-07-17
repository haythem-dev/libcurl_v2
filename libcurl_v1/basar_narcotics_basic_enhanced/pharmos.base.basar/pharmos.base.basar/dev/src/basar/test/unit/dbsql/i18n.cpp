//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   31.03.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasarcmnutil.h"

#include "testconnection.h"
#include "odbcfix.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef HAS_ODBC
#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable : 4512)
#endif
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>
#ifdef _WIN32
#pragma warning (pop)
#endif
#endif


//------------------------------------------------------------------------------
SUITE(I18n)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::I18nString;
using basar::Int16;
using basar::Int32;
using basar::UInt32;
using basar::Float64;
using basar::Float32;
using basar::Decimal;
using basar::Date;
using basar::DateTime;
using basar::Time;

using basar::cmnutil::Locale;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::ExecuteReturnInfo;

using basar::test::unit::dbsql::TestConnection;

//------------------------------------------------------------------------------
static void setEncoding(TestConnection::TestConnectionType eConnType)
{
#ifdef HAS_ODBC
	switch (eConnType)
	{
	case TestConnection::ODBC:
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1252"));
		QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
		break;

	case TestConnection::ODBC_BG:
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
		break;

    case TestConnection::MSSQL_ODBC:
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1252"));
		QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));    
        break;

	default:
		break;
	}

#else
	eConnType = eConnType;
#endif
}

//------------------------------------------------------------------------------
class Fixture
{
public:
	Fixture	() : fx_MARK				 (1234567),  

				 fx_FL					 (1234.5678),
				 fx_SMFL				 (7654.31f),
				 fx_DEC					 (0.0),
				 fx_MON					 (0.0),
				 fx_DATE				 (1964, 11, 23),
				 fx_DT1					 (18191022, 213456000),
				 fx_DT2					 (17340925, 201347021),
				 fx_DT2_MSSQL            (17530925, 201347020), // MS SQL range for datetime: January 1, 1753, through December 31, 9999
				 fx_TIME1				 (19, 12, 34,   0),
				 fx_TIME2				 (17, 59,  1, 987),

		         fx_CLIENT_STRING		 ("\x41\x42\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),	// Win CP1252: "ABc ÄÖüß ÇÏ÷"
				 fx_CLIENT_STRING_ALL	 ("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		// Win CP1252: "0B€Šž£µÄÜäü"
#ifdef _WIN32
				 fx_CLIENT_STRING_EN_ALL ("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		
				 fx_CLIENT_STRING_BG_ALL ("\x30\x42\x80\x8A\x9E\xA3\x3F\xC4\xDC\xE4\xFC"),		

				 fx_SERVER_STRING_EN_ALL ("\\x30\\x42\\x80\\x8A\\x9E\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	
				 fx_SERVER_STRING_BG_ALL ("\\x30\\x42\\xA2\\xA9\\xFB\\xA8\\x3F\\xB4\\xCC\\xD4\\xEC"),	

				 fx_CLIENT_STRING_EN_PREP("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		
				 fx_CLIENT_STRING_BG_PREP("\x30\x42\x80\x8A\x9E\xA3\x1A\xC4\xDC\xE4\xFC"),		

				 fx_SERVER_STRING_EN_PREP("\\x30\\x42\\x80\\x8A\\x9E\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	
				 fx_SERVER_STRING_BG_PREP("\\x30\\x42\\xA2\\xA9\\xFB\\xA8\\x1A\\xB4\\xCC\\xD4\\xEC"),	

				 fx_CLIENT_LOW_EN		 ("\x61\x62\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),
				 
				 fx_CLIENT_LOW_MS_SQL    ("\x61\x62\x63\x20\xE4\xF6\xFC\xDF\x20\xE7\xEF\xF7"),
				 
				 fx_CLIENT_LOW_BG		 ("\x61\x62\x63\x20\xE4\xF6\xFC\xFF\x20\xE7\xEF\xF7"),
		         fx_SERVER_STRING_EN	 ("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7"),
		         fx_SERVER_STRING_BG	 ("\\x41\\x42\\x63\\x20\\xB4\\xC6\\xEC\\xCF\\x20\\xB7\\xBF\\xE7"),

		         fx_SERVER_STRING_EN_CICS("C1C283404AE0D0A1406877E1"),
		         fx_SERVER_STRING_BG_CICS("C1C28340BCBBB0DD40FADCB6")

#else
				 fx_CLIENT_STRING_EN_ALL ("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		
				 fx_CLIENT_STRING_BG_ALL ("\x30\x42\x3F\x3F\x3F\xA3\xB5\xC4\xDC\xE4\xFC"),		

				 fx_SERVER_STRING_EN_ALL ("\\x30\\x42\\x80\\x8A\\x9E\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	
				 fx_SERVER_STRING_BG_ALL ("\\x30\\x42\\x3F\\x3F\\x3F\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	

				 fx_CLIENT_STRING_EN_PREP("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		
				 fx_CLIENT_STRING_BG_PREP("\x30\x42\x80\x8A\x9E\xA3\xB5\xC4\xDC\xE4\xFC"),		

				 fx_SERVER_STRING_EN_PREP("\\x30\\x42\\x80\\x8A\\x9E\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	
				 fx_SERVER_STRING_BG_PREP("\\x30\\x42\\x80\\x8A\\x9E\\xA3\\xB5\\xC4\\xDC\\xE4\\xFC"),	

				 fx_CLIENT_LOW_EN		 ("\x61\x62\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),
				 
				 fx_CLIENT_LOW_MS_SQL    ("\x61\x62\x63\x20\xE4\xF6\xFC\xDF\x20\xE7\xEF\xF7"),
				 
				 fx_CLIENT_LOW_BG		 ("\x61\x62\x63\x20\xE4\xD6\xFC\xDF\x20\xE7\xEF\xF7"),
		         fx_SERVER_STRING_EN	 ("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7"),
		         fx_SERVER_STRING_BG	 ("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7"),

		         fx_SERVER_STRING_EN_CICS("C1C283404AE0D0A1406877E1"),
		         fx_SERVER_STRING_BG_CICS("C1C28340BEAE549F40FEDD48")
#endif
	{
		fx_DEC.fromNonLocString("3629.32");
		fx_MON.fromNonLocString("9409.12");
	};

	~Fixture	() {};

	static bool initStatics()
	{
		Int32 rd = ::rand() % 0x10000;	// 4 digits
		::sprintf(randName, "%4.4x", rd);

		return true;
	};

	static char       randName[8];	// marks vsam.dkpar_ut.par_k_name
	static const bool dummy;


	const Int32     fx_MARK;

	const Float64   fx_FL;
	const Float32   fx_SMFL;
		  Decimal   fx_DEC;
		  Decimal   fx_MON;
	const Date      fx_DATE;
	const DateTime  fx_DT1;
	const DateTime  fx_DT2;
	const DateTime  fx_DT2_MSSQL;
	const Time      fx_TIME1;
	const DateTime  fx_TIME2;

	ConstString     fx_CLIENT_STRING;
	ConstString		fx_CLIENT_STRING_ALL;		// string with representative characters of particular codepage

	ConstString		fx_CLIENT_STRING_EN_ALL;
	ConstString		fx_CLIENT_STRING_BG_ALL;

	ConstString		fx_SERVER_STRING_EN_ALL;
	ConstString		fx_SERVER_STRING_BG_ALL;

	ConstString		fx_CLIENT_STRING_EN_PREP;
	ConstString		fx_CLIENT_STRING_BG_PREP;

	ConstString		fx_SERVER_STRING_EN_PREP;
	ConstString		fx_SERVER_STRING_BG_PREP;

	ConstString     fx_CLIENT_LOW_EN;
	ConstString     fx_CLIENT_LOW_MS_SQL;
	ConstString     fx_CLIENT_LOW_BG;

	ConstString     fx_SERVER_STRING_EN;
	ConstString     fx_SERVER_STRING_BG;

	ConstString		fx_SERVER_STRING_EN_CICS;
	ConstString		fx_SERVER_STRING_BG_CICS;

	OdbcFix			odbcfix;

private:
	Fixture &	operator = (const Fixture &);

};


const bool Fixture::dummy       = Fixture::initStatics();
char       Fixture::randName[8] = { 0 };

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
        TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();

		for (TestConnection::TestConnectionMap::const_iterator it = conns.begin();
			 it != conns.end();
			 ++it)
		{
			ConnectionRef conn = it->second;
			eConnType          = it->first;

			setEncoding(eConnType);

			conn.begin();

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					        fx_MARK,
							fx_CLIENT_STRING);
                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				
				if (TestConnection::MSSQL_ODBC == eConnType)
				{
				    sql.format(	"SELECT colint, colvch3, "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  1, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  2, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  3, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  4, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  5, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  6, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  7, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  8, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  9, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 10, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 11, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 12, 1))), 9, 2)) as serverstring "
					            "FROM but_var WHERE colvch3 = '%s';", 
							    fx_CLIENT_STRING);				
				}
				else
				{
				    sql.format(	"SELECT colint, colvch3, "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as serverstring "
					            "FROM but_var WHERE colvch3 = '%s';", 
							    fx_CLIENT_STRING);
				}
				
                StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32      col0;
				I18nString col1;
				I18nString col2;

				while (res.next())
				{
					col0 = res.getInt32 ("colint");
					col1 = res.getString("colvch3");
					col2 = res.getString("serverstring");

					CHECK_EQUAL(fx_MARK, col0);
					CHECK_EQUAL(fx_CLIENT_STRING, col1.c_str());

					if ( (eConnType == TestConnection::INFX_BG) || (eConnType == TestConnection::ODBC_BG) )
						CHECK_EQUAL(fx_SERVER_STRING_BG, col2.c_str());
					else
						CHECK_EQUAL(fx_SERVER_STRING_EN, col2.c_str());

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}

			conn.rollback();
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "connection type " << eConnType << std::endl;
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_parallel_connections)
{
	try
	{
		ConnectionRef conn_en = TestConnection::createTestConnection(TestConnection::INFX_CONCURRENT_TRANSACTION);
		ConnectionRef conn_bg = TestConnection::createTestConnection(TestConnection::INFX_CONCURRENT_TRANSACTION_BG);

		conn_en.begin();
		conn_bg.begin();

		// --- insert
		{
			VarString sql;
			sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					    fx_MARK,
						fx_CLIENT_STRING);

            StatementRef stmt_en  = conn_en.createStatement();
            StatementRef stmt_bg  = conn_bg.createStatement();

			ExecuteReturnInfo retinfo;

			retinfo = stmt_en.execute(sql.c_str());
			CHECK( !retinfo.hasError() );
			CHECK_EQUAL(1, (int)retinfo.m_affectedRows);

			retinfo = stmt_bg.execute(sql.c_str());
			CHECK( !retinfo.hasError() );
			CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
		}

		// --- select/check data
		{
			VarString sql;
			sql.format(	"SELECT colvch3, "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
                                "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as serverstring "
					    "FROM but_var WHERE colint = %d;", 
						fx_MARK);

            StatementRef  stmt_en = conn_en.createStatement();
            StatementRef  stmt_bg = conn_bg.createStatement();

			ResultsetRef  res_en  = stmt_en.executeQuery(sql);
			ResultsetRef  res_bg  = stmt_bg.executeQuery(sql);

			Int32 cnt = 0;

			I18nString col0;
			I18nString col1;

			while (res_en.next())
			{
				col0 = res_en.getString("colvch3");
				col1 = res_en.getString("serverstring");

				CHECK_EQUAL(fx_CLIENT_STRING   , col0.c_str());
				CHECK_EQUAL(fx_SERVER_STRING_EN, col1.c_str());

				++cnt;
			}

			CHECK_EQUAL(1, cnt);


			cnt = 0;

			while (res_bg.next())
			{
				col0 = res_bg.getString("colvch3");
				col1 = res_bg.getString("serverstring");

				CHECK_EQUAL(fx_CLIENT_STRING   , col0.c_str());
				CHECK_EQUAL(fx_SERVER_STRING_BG, col1.c_str());

				++cnt;
			}

			CHECK_EQUAL(1, cnt);
		}

		conn_en.rollback();
		conn_bg.rollback();
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_lowercase)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
        TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();

		for (TestConnection::TestConnectionMap::const_iterator it = conns.begin();
			 it != conns.end();
			 ++it)
		{
			ConnectionRef conn = it->second;
			eConnType          = it->first;

			setEncoding(eConnType);

			conn.begin();

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					        fx_MARK,
							fx_CLIENT_STRING);
                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				
				
				if (TestConnection::MSSQL_ODBC == eConnType)
				{
				    sql.format(	"SELECT colint, colvch3, lower(colvch3) as low, "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  1, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  2, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  3, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  4, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  5, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  6, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  7, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  8, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3,  9, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 10, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 11, 1))), 9, 2)) + "
                                    "'\\x' + UPPER(SUBSTRING(master.sys.fn_varbintohexstr(ASCII(SUBSTRING(colvch3, 12, 1))), 9, 2)) as serverstring "
					            "FROM but_var WHERE colvch3 = '%s';", 
							    fx_CLIENT_STRING);			
				}
				else
				{
				    sql.format(	"SELECT colint, colvch3, lower(colvch3) as low, "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) || "
                                    "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 12, 1))), 9, 2) as serverstring "
					            "FROM but_var WHERE colvch3 = '%s';", 
							    fx_CLIENT_STRING);
				}
                StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32      col0;
				I18nString col1;
				I18nString col2;
				I18nString col3;

				while (res.next())
				{
					col0 = res.getInt32 ("colint");
					col1 = res.getString("colvch3");
					col2 = res.getString("serverstring");
					col3 = res.getString("low");

					CHECK_EQUAL(fx_MARK         , col0);
					CHECK_EQUAL(fx_CLIENT_STRING, col1.c_str());

					if ( (eConnType == TestConnection::INFX_BG) || (eConnType == TestConnection::ODBC_BG) )
					{
						CHECK_EQUAL(fx_SERVER_STRING_BG, col2.c_str());
						CHECK_EQUAL(fx_CLIENT_LOW_BG   , col3.c_str());
					}
					else if (TestConnection::MSSQL_ODBC == eConnType)
					{
						CHECK_EQUAL(fx_SERVER_STRING_EN , col2.c_str());
						CHECK_EQUAL(fx_CLIENT_LOW_MS_SQL, col3.c_str());					    
					}
					else
					{
						CHECK_EQUAL(fx_SERVER_STRING_EN, col2.c_str());
						CHECK_EQUAL(fx_CLIENT_LOW_EN   , col3.c_str());
					}

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}

			conn.rollback();
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "connection type " << eConnType << std::endl;
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_format_date_decimal)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
        TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();

		for (TestConnection::TestConnectionMap::const_iterator it = conns.begin();
			 it != conns.end();
			 ++it)
		{
			ConnectionRef conn = it->second;
			eConnType          = it->first;

			setEncoding(eConnType);

			conn.begin();

			// --- insert
			{
				VarString sql;
				
				if (TestConnection::MSSQL_ODBC == eConnType)
				{
				    sql.format(	"INSERT INTO but_var "
					            "(colint, "
					            "colfl, colsmfl, coldec, colmon, "
							    "coldate, coldt1, coldt2, coltime1, coltime2) "
					            "VALUES "
							    "(%d, "
							    "%f, %f, %s, %s, "
							    "CONVERT(datetime, '%s', 101), "   //mm/dd/yyyy
							    "CONVERT(datetime, '%s', 121), "   //yyyy-mm-dd hh:mi:ss.mmm(24h)
							    "CONVERT(datetime, '%s', 121), "   //yyyy-mm-dd hh:mi:ss.mmm(24h)
							    "CONVERT(datetime, '%s', 121), "   //yyyy-mm-dd hh:mi:ss.mmm(24h)
							    "CONVERT(datetime, '%s', 121));",  //yyyy-mm-dd hh:mi:ss.mmm(24h)
					            fx_MARK,
							    fx_FL, 
							    fx_SMFL, 
							    fx_DEC  .toNonLocString().c_str(), 
							    fx_MON  .toNonLocString().c_str(),
							    fx_DATE .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX).c_str(), 
							    fx_DT1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_DT2_MSSQL.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_TIME1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_TIME2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());			        
				
				}
				else
				{
				    sql.format(	"INSERT INTO but_var "
					            "(colint, "
					            "colfl, colsmfl, coldec, colmon, "
							    "coldate, coldt1, coldt2, coltime1, coltime2) "
					            "VALUES "
							    "(%d, "
							    "%f, %f, %s, %s, "
							    "DATE('%s'), "
							    "DATETIME(%s) YEAR TO FRACTION(3), "
							    "DATETIME(%s) YEAR TO FRACTION(3), "
							    "DATETIME(%s) YEAR TO FRACTION(3), "
							    "DATETIME(%s) YEAR TO FRACTION(3));", 
					            fx_MARK,
							    fx_FL, 
							    fx_SMFL, 
							    fx_DEC  .toNonLocString().c_str(), 
							    fx_MON  .toNonLocString().c_str(),
							    fx_DATE .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX).c_str(), 
							    fx_DT1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_DT2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_TIME1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
							    fx_TIME2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
				}

                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				sql.format(	"SELECT colint, "
					               "colfl, colsmfl, coldec, colmon, "
								   "coldate, coldt1, coldt2, coltime1, coltime2 "
					        "FROM but_var WHERE colint = %d;", 
							fx_MARK);
                StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32     colint;
				Float64   colfl;
				Float32   colsmfl;
				Decimal   coldec;
				Decimal   colmon;
				Date      coldate;
				DateTime  coldt1;
				DateTime  coldt2;
				Time      coltime1;
				DateTime  coltime2;

				while (res.next())
				{
					colint   = res.getInt32   ("colint"  );
					colfl    = res.getFloat64 ("colfl"   );
					colsmfl  = res.getFloat32 ("colsmfl" );
					coldec   = res.getDecimal ("coldec"  );
					colmon   = res.getDecimal ("colmon"  );
					if (TestConnection::MSSQL_ODBC == eConnType)
					{
					    DateTime dt = res.getDateTime("coldate");
					    coldate  = dt.getDate();
					    coldt1   = res.getDateTime("coldt1"  );
					    coldt2   = res.getDateTime("coldt2"  );
					    coltime1 = res.getDateTime("coltime1");
					    coltime2 = res.getDateTime("coltime2");					    
					}
					else
					{
					    coldate  = res.getDate    ("coldate" );
					    coldt1   = res.getDateTime("coldt1"  );
					    coldt2   = res.getDateTime("coldt2"  );
					    coltime1 = res.getTime    ("coltime1");
					    coltime2 = res.getDateTime("coltime2");					    
					}

					CHECK_EQUAL(fx_MARK, colint);

					CHECK_CLOSE(fx_FL  , colfl  , 0.00001);
					CHECK_CLOSE(fx_SMFL, colsmfl, 0.001  );
					
					CHECK_EQUAL(fx_DEC.toNonLocString().c_str(), coldec.toNonLocString().c_str());
					CHECK_EQUAL(fx_MON.toNonLocString().c_str(), colmon.toNonLocString().c_str());

					CHECK_EQUAL(fx_DATE .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX)    .c_str(), 
						        coldate .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX)    .c_str());
					CHECK_EQUAL(fx_DT1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						        coldt1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
					if (TestConnection::MSSQL_ODBC == eConnType)
					{
					    CHECK_EQUAL(fx_DT2_MSSQL  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						            coldt2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());				    
					}
					else
					{
					    CHECK_EQUAL(fx_DT2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						            coldt2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
				    }
					CHECK_EQUAL(fx_TIME1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						        coltime1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
					CHECK_EQUAL(fx_TIME2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						        coltime2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}

			conn.rollback();
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "connection type " << eConnType << std::endl;
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_allowed_chars)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
		TestConnection::TestTypes types;
		types.push_back(TestConnection::INFX);
		types.push_back(TestConnection::INFX_BG);

		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator it = conns.begin();
			 it != conns.end();
			 ++it)
		{
			ConnectionRef conn = it->second;
			eConnType          = it->first;

			if (TestConnection::INFX_BG == eConnType)
			{
				Locale::setLocale  (Locale::BULGARIAN_BG);
				Locale::setCodepage(Locale::CP_CYRILLIC);
			}

			setEncoding(eConnType);

			conn.begin();

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					        fx_MARK,
							fx_CLIENT_STRING_ALL);
                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- update
			{
				VarString sql;
				sql.format(	"UPDATE but_var set colvch3 = '%s' || 'X' WHERE colint = %d;", 
							fx_CLIENT_STRING_ALL,
					        fx_MARK);
                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				sql.format(	"SELECT colint, colvch3, "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) as serverstring "
					        "FROM but_var WHERE colint = %d;", 
							fx_MARK);
                StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32      col0;
				I18nString col1;
				I18nString col2;
				I18nString cmp_col1;

				while (res.next())
				{
					col0 = res.getInt32 ("colint");
					col1 = res.getString("colvch3");
					col2 = res.getString("serverstring");

					CHECK_EQUAL(fx_MARK, col0);

					if (eConnType == TestConnection::INFX_BG)
					{
						cmp_col1.format("%sX", fx_CLIENT_STRING_BG_ALL);
						CHECK_EQUAL(fx_SERVER_STRING_BG_ALL, col2.c_str());
					}
					else
					{
						cmp_col1.format("%sX", fx_CLIENT_STRING_EN_ALL);
						CHECK_EQUAL(fx_SERVER_STRING_EN_ALL, col2.c_str());
					}

					CHECK_EQUAL(cmp_col1.c_str(), col1.c_str());

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}

			conn.rollback();
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "connection type " << eConnType << std::endl;
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_allowed_chars_preparedstmt)
{
	TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	try
	{
		TestConnection::TestTypes types;
		types.push_back(TestConnection::INFX);
		types.push_back(TestConnection::INFX_BG);

		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator it = conns.begin();
			 it != conns.end();
			 ++it)
		{
			ConnectionRef conn = it->second;
			eConnType          = it->first;

			if (TestConnection::INFX_BG == eConnType)
			{
				Locale::setLocale  (Locale::BULGARIAN_BG);
				Locale::setCodepage(Locale::CP_CYRILLIC);
			}

			setEncoding(eConnType);

			conn.begin();

			// --- insert
			{
				ConstString          sql  = "INSERT INTO but_var (colint, colvch3) VALUES (?, ?);";
                PreparedStatementRef stmt = conn.createPreparedStatement(sql);

				VarString value = fx_CLIENT_STRING_ALL;

				stmt.setInt32 (0, fx_MARK);
				stmt.setString(1, value.c_str());

				ExecuteReturnInfo retinfo = stmt.execute();

				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- update
			{
				ConstString          sql  = "UPDATE but_var set colvch3 = ? WHERE colint = ?;";
                PreparedStatementRef stmt = conn.createPreparedStatement(sql);

				VarString value = fx_CLIENT_STRING_ALL;
				value          += "X";

				stmt.setString(0, value.c_str());
				stmt.setInt32 (1, fx_MARK);

				ExecuteReturnInfo retinfo = stmt.execute();

				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				sql.format(	"SELECT colint, colvch3, "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  1, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  2, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  3, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  4, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  5, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  6, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  7, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  8, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3,  9, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 10, 1))), 9, 2) || "
                                   "'\\x' || SUBSTR(HEX(ASCII(SUBSTR(colvch3, 11, 1))), 9, 2) as serverstring "
					        "FROM but_var WHERE colint = %d;", 
							fx_MARK);
                StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32      col0;
				I18nString col1;
				I18nString col2;
				I18nString cmp_col1;

				while (res.next())
				{
					col0 = res.getInt32 ("colint");
					col1 = res.getString("colvch3");
					col2 = res.getString("serverstring");

					CHECK_EQUAL(fx_MARK, col0);

					if (eConnType == TestConnection::INFX_BG)
					{
						cmp_col1.format("%sX", fx_CLIENT_STRING_BG_PREP);
						CHECK_EQUAL(fx_SERVER_STRING_BG_PREP, col2.c_str());
					}
					else
					{
						cmp_col1.format("%sX", fx_CLIENT_STRING_EN_PREP);
						CHECK_EQUAL(fx_SERVER_STRING_EN_PREP, col2.c_str());
					}

					CHECK_EQUAL(cmp_col1.c_str(), col1.c_str());

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}

			conn.rollback();
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "connection type " << eConnType << std::endl;
        std::cout << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "connection type " << eConnType << std::endl;
		const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
#ifdef HAS_CICS

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_CICS)
{
	TestConnection::TestConnectionType eConnType = TestConnection::CICS_OPENED;

	TestConnection::TestTypes types;
	types.push_back(TestConnection::CICS_OPENED);
	types.push_back(TestConnection::CICS_BG_OPENED);

	TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

	ConstString FIL  = "03";
	ConstString NR   = "89";
	const Int32 LINT = fx_MARK; 

	for (TestConnection::TestConnectionMap::const_iterator it = conns.begin(); it != conns.end(); ++it)
	{
		ConnectionRef conn = it->second;
		eConnType          = it->first;

		try
		{

			setEncoding(eConnType);

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO vsam.dkpar_ut "
					             "(par_k_fil, par_k_name, par_k_nr, dummy, test_lint) "
					             "VALUES "
								 "('%s', '%s', '%s', '%s', %d);", 
					        FIL,
							randName, 
							NR,
							fx_CLIENT_STRING,
							LINT);
                StatementRef      stmt    = conn.createStatement();
				ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}

			// --- select/check data
			{
				VarString sql;
				sql.format(	"SELECT test_lint, dummy, HEX(SUBSTR(dummy, 1, 12)) as serverstring "
					        "FROM vsam.dkpar_ut "
							"WHERE par_k_fil  = '%s' "
							"  AND par_k_name = '%s' "
							"  AND par_k_nr   = '%s';",
							FIL,
							randName,
							NR);

				StatementRef  stmt = conn.createStatement();
				ResultsetRef  res  = stmt.executeQuery(sql);

				Int32 cnt = 0;

				Int32      col0;
				I18nString col1;
				I18nString col2;

				while (res.next())
				{
					col0 = res.getInt32 ("test_lint");
					col1 = res.getString("dummy");
					col2 = res.getString("serverstring");

					CHECK_EQUAL(fx_MARK, col0);
					CHECK_EQUAL(fx_CLIENT_STRING, col1.c_str());

					if (eConnType == TestConnection::CICS_BG_OPENED)
						CHECK_EQUAL(fx_SERVER_STRING_BG_CICS, col2.c_str());
					else
						CHECK_EQUAL(fx_SERVER_STRING_EN_CICS, col2.c_str());

					++cnt;
				}

				CHECK_EQUAL(1, cnt);
			}
		}
		catch (basar::Exception & ex)
		{
			std::cout << "connection type " << eConnType << std::endl;
			std::cout << ex.what().c_str() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
		catch (...)
		{
			std::cout << "connection type " << eConnType << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}

		// --- clean
		{
			VarString sql;
			sql.format(	"DELETE FROM vsam.dkpar_ut "
				        "WHERE par_k_fil  = '%s' "
						"  AND par_k_name = '%s' "
						"  AND par_k_nr   = '%s';",
				        FIL,
						randName,
						NR);
            StatementRef      stmt    = conn.createStatement();
			ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
			CHECK( !retinfo.hasError() );
			CHECK(0 < retinfo.m_affectedRows);
		}
	}	// for connections
}

//------------------------------------------------------------------------------
#endif // HAS_CICS

//------------------------------------------------------------------------------
}	// SUITE

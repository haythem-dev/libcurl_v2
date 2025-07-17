//----------------------------------------------------------------------------
/** $Id: $
 *  \author Michael Eichenlaub
 *  \date   21.04.2009
*/
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql.h"

#include "testconnection.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "odbcfix.h"

#ifdef HAS_ODBC
#include <QtCore/QTextCodec>
#endif


#define TESTDBASPECT_TESTI18N
#ifdef TESTDBASPECT_TESTI18N

//------------------------------------------------------------------------------
SUITE(TestI18n)
{

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::I18nString;
using basar::VarString;
using basar::Int32;
using basar::Float64;
using basar::Float32;
using basar::Decimal;
using basar::Date;
using basar::DateTime;
using basar::Time;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
static void setEncoding(TestConnection::TestConnectionType type)
{
#ifdef HAS_ODBC
	switch (type)
	{
	case TestConnection::ODBC:
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1252"));
		break;

	case TestConnection::ODBC_BG:
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
		break;

	default:
		break;
	}

#else
	type = type;
#endif
}

//------------------------------------------------------------------------------
class Fixture
{
public:

	Fixture	   () 
		       : fx_Conns(TestConnection::createTestConnections()),
				 fx_MARK            (1234567),  

				 fx_FL              (1234.5678),
				 fx_SMFL            (7654.31f),
				 fx_DEC             (0.0),
				 fx_MON             (0.0),
				 fx_DATE            (1964, 11, 23),
				 fx_DT1             (18191022, 213456000),
				 fx_DT2             (17340925, 201347021),
				 fx_TIME1           (19, 12, 34,   0),
				 fx_TIME2           (17, 59,  1, 987),

		         fx_CLIENT_STRING   ("\x41\x42\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),	// Win CP1252: "ABc ÄÖüß ÇÏ÷"
#ifdef _WIN32
				 fx_CLIENT_LOW_EN   ("\x61\x62\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),
				 fx_CLIENT_LOW_BG   ("\x61\x62\x63\x20\xE4\xF6\xFC\xFF\x20\xE7\xEF\xF7"),
		         fx_SERVER_STRING_EN("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7"),
		         fx_SERVER_STRING_BG("\\x41\\x42\\x63\\x20\\xB4\\xC6\\xEC\\xCF\\x20\\xB7\\xBF\\xE7")
#else
				 fx_CLIENT_LOW_EN   ("\x61\x62\x63\x20\xC4\xD6\xFC\xDF\x20\xC7\xCF\xF7"),
				 fx_CLIENT_LOW_BG   ("\x61\x62\x63\x20\xE4\xD6\xFC\xDF\x20\xE7\xEF\xF7"),
		         fx_SERVER_STRING_EN("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7"),
		         fx_SERVER_STRING_BG("\\x41\\x42\\x63\\x20\\xC4\\xD6\\xFC\\xDF\\x20\\xC7\\xCF\\xF7")
#endif
	{
		fx_DEC.fromNonLocString("3629.32");
		fx_MON.fromNonLocString("9409.12");
	};

	TestConnection::TestConnectionMap	fx_Conns;

	const Int32     fx_MARK;

	const Float64   fx_FL;
	const Float32   fx_SMFL;
	      Decimal   fx_DEC;
	      Decimal   fx_MON;
	const Date      fx_DATE;
	const DateTime  fx_DT1;
	const DateTime  fx_DT2;
	const Time      fx_TIME1;
	const DateTime  fx_TIME2;

	ConstString     fx_CLIENT_STRING;
	ConstString     fx_CLIENT_LOW_EN;
	ConstString     fx_CLIENT_LOW_BG;
	ConstString     fx_SERVER_STRING_EN;
	ConstString     fx_SERVER_STRING_BG;

	OdbcFix			odbcfix;

private:
	Fixture &	operator = (const Fixture &);
};

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection)
{
	try
	{
		for (TestConnection::TestConnectionMap::const_iterator it = fx_Conns.begin(); it != fx_Conns.end(); ++it)
		{
			setEncoding(it->first);
			basar::db::sql::ConnectionRef dbConn = it->second.getSqlConnection();
			dbConn.begin();

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					        fx_MARK,
							fx_CLIENT_STRING);
				basar::db::sql::StatementRef      stmt    = dbConn.createStatement();
				basar::db::sql::ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}


			AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButTable_Encoding_Projection",
																						ACCESSORNAME_TestI18n,
																						it->second,
																						ON_DEMAND_CACHING);
			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.insert(basar::FOR_CLEAN);
			assert( ! yit.isNull() );
			yit.setString(PROPDEF_COLVCH3.getName(), fx_CLIENT_STRING);

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButTables_I18n_SelectByString, yit);
			CHECK( ! ret.hasError() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			Int32      col0;
			I18nString col1;
			I18nString col2;

			while (yit != propTbl.end())
			{
				col0 = yit.getInt32 (PROPDEF_COLINT         .getName());
				col1 = yit.getString(PROPDEF_COLVCH3        .getName());
				col2 = yit.getString(PROPDEF_COLSERVERSTRING.getName());
		
				CHECK_EQUAL(fx_MARK         , col0);
				CHECK_EQUAL(fx_CLIENT_STRING, col1.c_str());

				if ( (it->first == TestConnection::INFX_BG) || (it->first == TestConnection::ODBC_BG) )
					CHECK_EQUAL(fx_SERVER_STRING_BG, col2.c_str());
				else
					CHECK_EQUAL(fx_SERVER_STRING_EN, col2.c_str());

				++yit;
			}

			CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));

			dbConn.rollback();
		}
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_parallel_connections)
{
	try
	{
		ConnectionRef conn_en = TestConnection::createTestConnection(TestConnection::INFX_CONCURR_TRANSACT   );
		ConnectionRef conn_bg = TestConnection::createTestConnection(TestConnection::INFX_CONCURR_TRANSACT_BG);

		basar::db::sql::ConnectionRef dbConn_en = conn_en.getSqlConnection();
		basar::db::sql::ConnectionRef dbConn_bg = conn_bg.getSqlConnection();

		dbConn_en.begin();
		dbConn_bg.begin();

		// --- insert
		{
			VarString sql;
			sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					    fx_MARK,
						fx_CLIENT_STRING);

			basar::db::sql::StatementRef      stmt_en = dbConn_en.createStatement();
			basar::db::sql::StatementRef      stmt_bg = dbConn_bg.createStatement();

			basar::db::sql::ExecuteReturnInfo retinfo;
			
			retinfo = stmt_en.execute(sql.c_str());
			CHECK( !retinfo.hasError() );
			CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			
			retinfo = stmt_bg.execute(sql.c_str());
			CHECK( !retinfo.hasError() );
			CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
		}


		AccessorInstanceRef	accInst_en = Manager::getInstance().createAccessorInstance("Inst_ButTable_Encoding_Projection_Parallel_en",
																					   ACCESSORNAME_TestI18n,
																					   conn_en,
																					   ON_DEMAND_CACHING);
		AccessorInstanceRef	accInst_bg = Manager::getInstance().createAccessorInstance("Inst_ButTable_Encoding_Projection_Parallel_bg",
																					   ACCESSORNAME_TestI18n,
																					   conn_bg,
																					   ON_DEMAND_CACHING);

		AccessorPropertyTableRef propTbl_en = accInst_en.getPropertyTable();
		CHECK( !propTbl_en.isNull() );

		AccessorPropertyTableRef propTbl_bg = accInst_bg.getPropertyTable();
		CHECK( !propTbl_bg.isNull() );

		AccessorPropertyTable_YIterator yit_en = propTbl_en.insert(basar::FOR_CLEAN);
		assert( ! yit_en.isNull() );
		yit_en.setInt32(PROPDEF_COLINT.getName(), fx_MARK);

		AccessorPropertyTable_YIterator yit_bg = propTbl_bg.insert(basar::FOR_CLEAN);
		assert( ! yit_bg.isNull() );
		yit_bg.setInt32(PROPDEF_COLINT.getName(), fx_MARK);

		ExecuteResultInfo ret;
		
		ret = accInst_en.execute(ACCESSMETHOD_ButTables_I18n_SelectByInt, yit_en);
		CHECK( ! ret.hasError() );

		ret = accInst_bg.execute(ACCESSMETHOD_ButTables_I18n_SelectByInt, yit_bg);
		CHECK( ! ret.hasError() );

		I18nString col1;
		I18nString col2;

		yit_en = propTbl_en.begin();
		CHECK( ! yit_en.isNull() );
		CHECK( yit_en != propTbl_en.end());

		while (yit_en != propTbl_en.end())
		{
			col1 = yit_en.getString(PROPDEF_COLVCH3        .getName());
			col2 = yit_en.getString(PROPDEF_COLSERVERSTRING.getName());
	
			CHECK_EQUAL(fx_CLIENT_STRING   , col1.c_str());
			CHECK_EQUAL(fx_SERVER_STRING_EN, col2.c_str());

			++yit_en;
		}

		CHECK_EQUAL(1, static_cast<Int32>(propTbl_en.size()));

		yit_bg = propTbl_bg.begin();
		CHECK( ! yit_bg.isNull() );
		CHECK( yit_bg != propTbl_bg.end());

		while (yit_bg != propTbl_bg.end())
		{
			col1 = yit_bg.getString(PROPDEF_COLVCH3        .getName());
			col2 = yit_bg.getString(PROPDEF_COLSERVERSTRING.getName());
	
			CHECK_EQUAL(fx_CLIENT_STRING   , col1.c_str());
			CHECK_EQUAL(fx_SERVER_STRING_BG, col2.c_str());

			++yit_bg;
		}

		CHECK_EQUAL(1, static_cast<Int32>(propTbl_bg.size()));

		dbConn_en.rollback();
		dbConn_bg.rollback();
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_lowercase)
{
	try
	{
		for (TestConnection::TestConnectionMap::const_iterator it = fx_Conns.begin(); it != fx_Conns.end(); ++it)
		{
			setEncoding(it->first);
			basar::db::sql::ConnectionRef dbConn = it->second.getSqlConnection();
			dbConn.begin();

			// --- insert
			{
				VarString sql;
				sql.format(	"INSERT INTO but_var (colint, colvch3) VALUES (%d, '%s');", 
					        fx_MARK,
							fx_CLIENT_STRING);
				basar::db::sql::StatementRef      stmt    = dbConn.createStatement();
				basar::db::sql::ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}


			AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButTable_Encoding_Projection_Low",
																						ACCESSORNAME_TestI18n,
																						it->second,
																						ON_DEMAND_CACHING);
			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.insert(basar::FOR_CLEAN);
			assert( ! yit.isNull() );
			yit.setString(PROPDEF_COLVCH3.getName(), fx_CLIENT_STRING);

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButTables_I18n_SelectLow, yit);
			CHECK( ! ret.hasError() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

			Int32      col0;
			I18nString col1;
			I18nString col2;
			I18nString col3;

			while (yit != propTbl.end())
			{
				col0 = yit.getInt32 (PROPDEF_COLINT         .getName());
				col1 = yit.getString(PROPDEF_COLVCH3        .getName());
				col2 = yit.getString(PROPDEF_COLSERVERSTRING.getName());
				col3 = yit.getString(PROPDEF_COLLOW         .getName());
		
				CHECK_EQUAL(fx_MARK         , col0);
				CHECK_EQUAL(fx_CLIENT_STRING, col1.c_str());

				if ( (it->first == TestConnection::INFX_BG) || (it->first == TestConnection::ODBC_BG) )
				{
					CHECK_EQUAL(fx_SERVER_STRING_BG, col2.c_str());
					CHECK_EQUAL(fx_CLIENT_LOW_BG   , col3.c_str());
				}
				else
				{
					CHECK_EQUAL(fx_SERVER_STRING_EN, col2.c_str());
					CHECK_EQUAL(fx_CLIENT_LOW_EN   , col3.c_str());
				}

				++yit;
			}

			CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));

			dbConn.rollback();
		}
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, encoding_projection_format_date_decimal)
{
	try
	{
		for (TestConnection::TestConnectionMap::const_iterator it = fx_Conns.begin(); it != fx_Conns.end(); ++it)
		{
			setEncoding(it->first);
			basar::db::sql::ConnectionRef dbConn = it->second.getSqlConnection();
			dbConn.begin();

			// --- insert
			{
				VarString sql;
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
				basar::db::sql::StatementRef      stmt    = dbConn.createStatement();
				basar::db::sql::ExecuteReturnInfo retinfo = stmt.execute(sql.c_str());
				CHECK( !retinfo.hasError() );
				CHECK_EQUAL(1, (int)retinfo.m_affectedRows);
			}


			AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance("Inst_ButTable_Encoding_Projection_Format",
																						ACCESSORNAME_TestI18n,
																						it->second,
																						ON_DEMAND_CACHING);
			AccessorPropertyTableRef propTbl = accInst.getPropertyTable();
			CHECK( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit = propTbl.insert(basar::FOR_CLEAN);
			assert( ! yit.isNull() );
			yit.setInt32(PROPDEF_COLINT.getName(), fx_MARK);

			ExecuteResultInfo ret = accInst.execute(ACCESSMETHOD_ButTables_I18n_SelectFormat, yit);
			CHECK( ! ret.hasError() );

			yit = propTbl.begin();
			CHECK( ! yit.isNull() );
			CHECK( yit != propTbl.end());

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

			while (yit != propTbl.end())
			{
				colint   = yit.getInt32   (PROPDEF_COLINT  .getName());
				colfl    = yit.getFloat64 (PROPDEF_COLFL   .getName());
				colsmfl  = yit.getFloat32 (PROPDEF_COLSMFL .getName());
				coldec   = yit.getDecimal (PROPDEF_COLDEC  .getName());
				colmon   = yit.getDecimal (PROPDEF_COLMON  .getName());
				coldate  = yit.getDate    (PROPDEF_COLDATE .getName());
				coldt1   = yit.getDateTime(PROPDEF_COLDT1  .getName());
				coldt2   = yit.getDateTime(PROPDEF_COLDT2  .getName());
				coltime1 = yit.getTime    (PROPDEF_COLTIME1.getName());
				coltime2 = yit.getDateTime(PROPDEF_COLTIME2.getName());
		
				CHECK_EQUAL(fx_MARK, colint);

				CHECK_CLOSE(fx_FL  , colfl  , 0.00001);
				CHECK_CLOSE(fx_SMFL, colsmfl, 0.001  );
				
				CHECK_EQUAL(fx_DEC.toNonLocString().c_str(), coldec.toNonLocString().c_str());
				CHECK_EQUAL(fx_MON.toNonLocString().c_str(), colmon.toNonLocString().c_str());

				CHECK_EQUAL(fx_DATE .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX)    .c_str(), 
						    coldate .toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX)    .c_str());
				CHECK_EQUAL(fx_DT1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						    coldt1  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
				CHECK_EQUAL(fx_DT2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						    coldt2  .toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
				CHECK_EQUAL(fx_TIME1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						    coltime1.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());
				CHECK_EQUAL(fx_TIME2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str(), 
						    coltime2.toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX).c_str());

				++yit;
			}

			CHECK_EQUAL(1, static_cast<Int32>(propTbl.size()));

			dbConn.rollback();
		}
	}
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
}	// SUITE

#endif	// GUARD

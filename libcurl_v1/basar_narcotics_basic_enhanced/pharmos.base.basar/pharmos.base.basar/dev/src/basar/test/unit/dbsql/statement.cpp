#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasarcmnutil.h"

#include "utdbsqlhelper.h"
#include "testconnection.h"
#include "odbcfix.h"
#include "defaultmasterbuttable.h"

#include <iostream>
#include <fstream>

#include <cstdio>

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ExecuteReturnInfo;
using basar::db::sql::DbException;

// general:
using basar::ConstString;
using basar::UInt32;
using basar::Int32;
using basar::VarString;
using basar::Decimal;
using basar::BULong;

// test:
using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::DefaultMasterBUTTable;
using basar::test::unit::dbsql::BUTTable;
using basar::test::unit::dbsql::Defines;

#define TESTDBSQLSTATEMENTREFTESTS
#ifdef TESTDBSQLSTATEMENTREFTESTS
//------------------------------------------------------------------------------
SUITE(Stmt)
{
// ----------------------------------------------------------

    //TEST(disc_infx_bigint)
    //{
    //    basar::db::sql::DatabaseInfo dbInfo;

    //    dbInfo.dbServer = ESQLC_DBSERVER;
    //    dbInfo.database = "but21";
    //    dbInfo.concurrTransaction = false;

    //    try
    //    {
    //        ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

    //        StatementRef stmt = conn.createStatement();

    //        ExecuteReturnInfo eInfo = stmt.execute("create temp table mytest (id bigserial(42), myint bigint )");

    //        eInfo = stmt.execute("insert into mytest (myint) values (142)");

    //        ResultsetRef res = stmt.executeQuery("select first 1 * from mytest");

    //        res.next();

    //        basar::Int64 test = res.getInt64("myint"); test;



    //    }
    //    catch (basar::Exception& ex)
    //    {
    //        std::cout << "caught exception: " << ex.what() << std::endl;
    //    }
    //}

    //TEST(disc_infx_clob)
    //{
    //    basar::db::sql::DatabaseInfo dbInfo;

    //    dbInfo.dbServer = ESQLC_DBSERVER;
    //    dbInfo.database = "but21";
    //    dbInfo.concurrTransaction = false;

    //    try
    //    {
    //        ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

    //        StatementRef stmt = conn.createStatement();

    //        ExecuteReturnInfo eInfo = stmt.execute("create temp table mytest (id serial(42), myint integer, data clob )");

    //        eInfo = stmt.execute("insert into mytest (myint) values (142)");

    //        basar::db::sql::PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest (data) values (?)");

    //        std::string s("hallo welt, dies ist ein test");

    //        basar::Binary bin(s.begin(), s.end());

    //        prepStmt.setBlob(0, bin);

    //        eInfo = prepStmt.execute();

    //        basar::db::sql::ResultsetRef res = stmt.executeQuery("select * from mytest");

    //        res.next();

    //        basar::Int32 myint = res.getInt32(1); myint;

    //        basar::Binary fromDB = res.getBlob(2);

    //        std::string sFromDB(fromDB.begin(), fromDB.end());

    //    }
    //    catch (basar::Exception& ex)
    //    {
    //        std::cout << "caught exception: " << ex.what() << std::endl;
    //    }
    //}

    //TEST(disc_infx_byte)
    //{
    //    //aka db copy

    //    basar::db::sql::DatabaseInfo dbInfo;

    //    dbInfo.dbServer = ESQLC_DBSERVER;
    //    dbInfo.database = "ode21";
    //    dbInfo.concurrTransaction = false;

    //    try
    //    {
    //        ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

    //        StatementRef stmt = conn.createStatement();

    //        ExecuteReturnInfo eInfo = stmt.execute("create temp table mytest (id serial(42), data byte not null)");

    //        std::ifstream ifs("C:\\temp\\20151007_57_0.pdf", std::ios::in | std::ios::binary);

    //        basar::Binary bin((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    //        ifs.close();

    //        {
    //            std::ofstream fout("C:\\temp\\wrote_read.pdf", std::ios::binary | std::ios::trunc);
    //            fout.write(&bin[0], bin.size());
    //            fout.close();
    //        }

    //        basar::db::sql::PreparedStatementRef prepStmt = conn.createPreparedStatement("insert into mytest (data) values (?)");

    //        prepStmt.setBinary(0, bin);

    //        eInfo = prepStmt.execute();


    //        ResultsetRef res = stmt.executeQuery("select first 1 * from mytest");

    //        bool hasNext = res.next(); hasNext;

    //        const basar::Int32 id = res.getInt32(0); id;

    //        const basar::Binary& bin_out = res.getBinary(1);

    //        if (bin.size() == bin_out.size())
    //        {
    //            CHECK_ARRAY_EQUAL(bin, bin_out, bin.size());
    //        }

    //        std::ofstream fout("C:\\temp\\copied.pdf", std::ios::binary | std::ios::trunc);

    //        fout.write(&bin_out[0], bin_out.size());

    //        fout.close();

    //    }
    //    catch (basar::Exception& ex)
    //    {
    //        std::cout << "caught exception: " << ex.what() << std::endl;
    //    }
    //}



    //TEST(disc_infx_text)
    //{
    //    basar::db::sql::DatabaseInfo dbInfo;

    //    dbInfo.dbServer = ESQLC_DBSERVER;
    //    dbInfo.database = "ode21";
    //    dbInfo.concurrTransaction = false;

    //    try
    //    {

    //        ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

    //        StatementRef stmt = conn.createStatement();

    //        {
    //            ResultsetRef res = stmt.executeQuery("select first 1 * from systables s left join orderresponses o on s.tabid = o.id");

    //            bool hasNext = res.next(); hasNext;

    //            basar::VarString txt = res.getString("responseasxml");

    //            res.close();
    //            res.reopen();

    //            res.next();

    //            txt = res.getString("responseasxml");
    //        }

    //        conn.begin();

    //        //stmt.execute("update orderresponses set responseasxml = 'hallo welt' where id = 7825256"); // throws error -> ok

    //        basar::db::sql::PreparedStatementRef prepStmt = conn.createPreparedStatement("update orderresponses set responseasxml = ? where id = 7962856");

    //        basar::VarString meinTest = "hallo welt";

    //        prepStmt.setString(0, meinTest.c_str());

    //        prepStmt.execute();

    //        prepStmt = conn.createPreparedStatement("insert into orderresponses ( request_id, support_id, branchno, customerno, is_offer, responseasxml, create_date ) values (?, ?, ?, ?, ?, ?, 20161014) ");
    //        prepStmt.setString(0, "0815");
    //        prepStmt.setInt32(1, 4711);
    //        prepStmt.setInt16(2, 42);
    //        prepStmt.setInt32(3, 47110815);
    //        prepStmt.setInt16(4, 0);
    //        prepStmt.setString(5, "dies ist ein text");

    //        ExecuteReturnInfo eri = prepStmt.execute();

    //        basar::Int64 serial = eri.m_lastSerial.at(0);

    //        std::ostringstream os;

    //        os << "select * from orderresponses where id = " << serial;

    //        ResultsetRef res = stmt.executeQuery(os.str());

    //        res.next();

    //        basar::VarString txt = res.getString("responseasxml");

    //        conn.rollback();

    //    }
    //    catch (basar::Exception& ex)
    //    {
    //        std::cout << "caught exception: " << ex.what() << std::endl;
    //    }
    //}

TEST(testdbsqlstatementref_create_open_close_execute)
{
    TestConnection::TestConnectionMap                 connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap::const_iterator iter;

    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        ConnectionRef connRef = iter->second;

        {
            StatementRef statement = connRef.createStatement();

            // needs query to be open
            CHECK(!statement.isNull());
            CHECK(!statement.isOpen());

            try
            {
                ExecuteReturnInfo execResult = statement.execute("begin work");
                CHECK_EQUAL(0, (int)execResult.m_affectedRows);
                CHECK_EQUAL(0, static_cast<int>(execResult.m_lastSerial.size()));

                statement.close();
                CHECK(!statement.isOpen());
            }
            catch (const DbException &)
            {
                if ( (TestConnection::INFX    == iter->first) || 
                     (TestConnection::INFX_BG == iter->first)    )
                     CHECK(false);
            }
        }
        
		CHECK_THROW(connRef.rollback(), basar::db::sql::DbException);
    }
}


// ----------------------------------------------------------

TEST(testdbsqlstatementref_execute_update)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        ConnectionRef connRef = iter->second;
        {	
            connRef.begin();
            StatementRef statement = connRef.createStatement();
            ExecuteReturnInfo execResult = statement.execute(
                "update but_detail "
                "set colsmint = colsmint + 1 "
                "where colser = 12");
            CHECK_EQUAL(1, (int)execResult.m_affectedRows);
            CHECK_EQUAL(0, static_cast<int>( execResult.m_lastSerial.size()));

            connRef.rollback();
        }
    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_execute_insert)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {	
            connRef.begin();
            StatementRef statement = connRef.createStatement();
            basar::db::sql::ExecuteReturnInfo execResult = statement.execute("insert into but_var  (colint, colsmint) values ( 22, 1) ");
            CHECK_EQUAL(1, (int)execResult.m_affectedRows);

            if ( (TestConnection::INFX == iter->first) || (TestConnection::INFX_BG == iter->first))
				CHECK_EQUAL(1, static_cast<int>( execResult.m_lastSerial.size()));
            else
                CHECK(2 > execResult.m_lastSerial.size());

            CHECK_EQUAL(basar::db::sql::DBErrorCode::SUCCESS,execResult.m_error);  

            if (execResult.m_lastSerial.size() > 0 )
                CHECK( execResult.m_lastSerial[0]> 0);

            connRef.rollback();
        }
        catch (basar::Exception & ex)
        {
            std::cout <<  "Exception at Connection = " << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl ;

            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_execute_duplInsertError_index)
{
    TestConnection::TestConnectionMap connRefs = 
        TestConnection::createDefaultTestConnections();

    for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
         iter != connRefs.end(); 
         ++iter)
    {	
        ConnectionRef connRef = iter->second;

        try
        {	
            connRef.begin();

            StatementRef statement = connRef.createStatement();
            ConstString stm = "insert into but_master (colser) values (1)";

            CHECK_THROW(statement.execute(stm), basar::Exception) ;

            ExecuteReturnInfo execResult = statement.execute(stm, false);

            if ( (TestConnection::ODBC == iter->first) || (TestConnection::ODBC_BG == iter->first) ||
                 (TestConnection::MSSQL_ODBC == iter->first)
               )
            {
                CHECK_EQUAL(basar::db::sql::DBErrorCode::UNKNOWN, execResult.m_error);
                CHECK(0 != execResult.getErrorMessage().length());
                if (TestConnection::MSSQL_ODBC == iter->first)
                {
                    CHECK_EQUAL(544, execResult.getRawMainError());
                }
                else
                {
                    CHECK_EQUAL(-239, execResult.getRawMainError());
                }
                CHECK_EQUAL(-1, execResult.getRawSubError());
                CHECK_THROW(connRef.rollback(), basar::Exception);  
                CHECK_THROW(connRef.close   (), basar::Exception);  
            }
            else
            {
                CHECK_EQUAL(basar::db::sql::DBErrorCode::INSERT_DUPL, execResult.m_error);
                CHECK(0 != execResult.getErrorMessage().length());
                CHECK_EQUAL(-239, execResult.getRawMainError());
                CHECK_EQUAL(-100, execResult.getRawSubError());
                connRef.rollback();  
                connRef.close   ();
            } 

            CHECK_EQUAL(0, (int)execResult.m_affectedRows);
            CHECK_EQUAL(0, (int)execResult.m_lastSerial.size());
        }
        catch (basar::Exception & ex)
        {
            std::cout <<  "Exception at Connection = " << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl ;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_execute_duplInsertError_constraint)
{
    TestConnection::TestConnectionMap connRefs = 
        TestConnection::createDefaultTestConnections();

    for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
         iter != connRefs.end(); 
         ++iter)
    {	
        TestConnection::TestConnectionType eConnType = iter->first;
        ConnectionRef connRef = iter->second;
        try
        {	
            connRef.begin();

            StatementRef statement = connRef.createStatement();
            ConstString stm = "insert into but_master (colsmint) values (2001)";

            CHECK_THROW(statement.execute(stm), basar::Exception) ;

            ExecuteReturnInfo execResult = statement.execute(stm, false);

            if ( (TestConnection::ODBC == eConnType) || (TestConnection::ODBC_BG == eConnType) ||
                 (TestConnection::MSSQL_ODBC == eConnType)
               )
            {
                CHECK_EQUAL(basar::db::sql::DBErrorCode::UNKNOWN, execResult.m_error);
                CHECK_THROW(connRef.rollback(), basar::Exception);  
                CHECK_THROW(connRef.close   (), basar::Exception);  
            }
            else
            {
                CHECK_EQUAL(basar::db::sql::DBErrorCode::INSERT_DUPL, execResult.m_error);
                connRef.rollback();  
                connRef.close   ();
            } 

            CHECK_EQUAL(0, (int)execResult.m_affectedRows);
            CHECK_EQUAL(0, (int)execResult.m_lastSerial.size());
        }
        catch (basar::Exception & ex)
        {
            std::cout <<  "Exception at Connection = " << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl ;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_execute_nonsenceStatementError)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {	       
            // nonsense statement
            {
                StatementRef statement = connRef.createStatement();
                connRef.begin();
                basar::db::sql::ExecuteReturnInfo execResult = statement.execute("this is no valid statement", false);
                if ( (TestConnection::ODBC == iter->first) || (TestConnection::ODBC_BG == iter->first) ||
                     (TestConnection::MSSQL_ODBC == iter->first)
                   )
                {
                    CHECK_EQUAL(basar::db::sql::DBErrorCode::UNKNOWN,execResult.m_error);
                    CHECK_THROW(connRef.rollback(), basar::Exception);  
                    CHECK_THROW(connRef.close(), basar::Exception);  
                }
                else
                {
                    CHECK_EQUAL(basar::db::sql::DBErrorCode::NONSPECIFIC,execResult.m_error);
                    connRef.rollback() ;  
                    connRef.close();
                } 
                CHECK_EQUAL(0, (int)execResult.m_affectedRows);
                CHECK_EQUAL(0, static_cast<int>( execResult.m_lastSerial.size()));
            }
        }
        catch (basar::Exception & ex)
        {
            std::cout <<  "Exception at Connection = " << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl ;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeSingleQuery_on_colser)
{	
    const basar::UInt32 expectetCols = 2;
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        TestConnection::TestConnectionType eConnType = iter->first;
        basar::db::sql::ConnectionRef connRef = iter->second;

        try
        {
            StatementRef statement = connRef.createStatement();
            connRef.begin();

            ConstString stmt =	"select colser, coldate from but_master where colser = 1 ";

            StatementRef stmtRef = connRef.createStatement();
            ResultsetRef resRef  = stmtRef.executeSingleQuery(stmt);

            CHECK_EQUAL(expectetCols, resRef.getSelectedCols());

            Int32  cnt = 0;
            while (true == resRef.next())
            {
                ++cnt;
                CHECK_EQUAL(resRef.getInt32(0)          ,        1);
                if (TestConnection::MSSQL_ODBC == eConnType)
                {
                    CHECK_EQUAL(99970405, resRef.getDateTime(1).getDate());
                }
                else
                {
                    CHECK_EQUAL(99970405, resRef.getDate (1).getDate());
                }
            }
            CHECK(cnt < 2);

            resRef = stmtRef.executeQuery(stmt);
            CHECK_EQUAL(expectetCols, resRef.getSelectedCols() );

            cnt = 0;
            while (true == resRef.next())
            {
                ++cnt;
                CHECK_EQUAL(resRef.getInt32(0)          ,        1);
                if (TestConnection::MSSQL_ODBC == eConnType)
                {
                    CHECK_EQUAL(99970405, resRef.getDateTime(1).getDate());
                }
                else
                {
                    CHECK_EQUAL(99970405, resRef.getDate (1).getDate());
                }
            }
            CHECK(cnt < 2);

            connRef.rollback();
            resRef.close();
            statement.close();
            // -> no Exception:
            CHECK(true);	
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

        connRef.close();
    }
}

// ----------------------------------------------------------

TEST(testdbsqlstatementref_executeSingleQuery_count_max)
{
    const basar::UInt32 expectedRows = 1;
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

        {
            try
            {
                StatementRef statement = connRef.createStatement();
                connRef.begin();
                StatementRef stmtRef = connRef.createStatement();
                ResultsetRef resRef;

                VarString stmt = "select count(*), min(colint), sum(colser), count(*) as num from but_master ";		
                std::string countname = "count(*)";

                if ( (TestConnection::ODBC == iter->first) || (TestConnection::ODBC_BG == iter->first) ||
                     (TestConnection::MSSQL_ODBC == iter->first)
                   )
                {
                    StatementRef stmtRef2 = connRef.createStatement();
                    // Feature or Error ?
                    CHECK_THROW(resRef  = stmtRef2.executeSingleQuery(stmt), basar::InvalidParameterException);
                    stmt = "select count(*) as count, min(colint) as min, sum(colser) as sum, count(*) as num from but_master ";
                }

                resRef  = stmtRef.executeSingleQuery(stmt);
                UInt32  cnt = 0;

                DefaultMasterBUTTable  & defaulttable = DefaultMasterBUTTable::getInstance();
                basar::Int32 numberOfRowsInDefaultMasterBUTTable = (basar::Int32)( defaulttable.getNumberOfRows());
                basar::Int32 min = defaulttable.getMinInt32(BUTTable::colnumber_colint);
                basar::Int32 sum = defaulttable.getSumInt32(BUTTable::colnumber_colser);

                while (true == resRef.next())
                {
                    ++cnt;
                    if (TestConnection::MSSQL_ODBC == iter->first)
                    {
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getInt32(0));
                        CHECK_EQUAL( min , resRef.getInt32(1));
                        CHECK_EQUAL( sum , resRef.getInt32(2));
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getInt32(3));
                    }
                    else
                    {
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal(0).toLong32());
                        CHECK_EQUAL( min , resRef.getInt32(1));
                        CHECK_EQUAL( sum , resRef.getDecimal(2).toLong32());
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal(3).toLong32());                    
                    }

                    if ((TestConnection::ODBC != iter->first) && (TestConnection::ODBC_BG != iter->first) && (TestConnection::MSSQL_ODBC != iter->first))
					{
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal("(count(*))").toLong32());
						CHECK_EQUAL( min , resRef.getInt32("(min)"));
	                    CHECK_EQUAL( sum , resRef.getDecimal("(sum)").toLong32());
                        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal("num").toLong32());
					}
                    else
					{
					    if (TestConnection::MSSQL_ODBC == iter->first)
                        {
                            CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getInt32("count"));
						    CHECK_EQUAL( min , resRef.getInt32("min"));
						    CHECK_EQUAL( sum , resRef.getInt32("sum"));
						    CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getInt32("num"));
                        }
                        else
                        {
                            CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal("count").toLong32());
						    CHECK_EQUAL( min , resRef.getInt32("min"));
						    CHECK_EQUAL( sum , resRef.getDecimal("sum").toLong32());
						    CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , resRef.getDecimal("num").toLong32());
						}
					}
				}
                CHECK_EQUAL(expectedRows, cnt);
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
            connRef.close();
        }
    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeQuery_selectleftouterjoin)
{
    const basar::UInt32 expectetRows = 9;
    const basar::UInt32 expectetCols = 6;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

        try
        {
            StatementRef statement = connRef.createStatement();
            connRef.begin();

            ResultsetRef resultset;
            CHECK(resultset.isNull());

            // test faulty db-string:
            ConstString faultystmt_dberror = "select k.colser , k.colint, k.colsmint,  t.colser, t.colint, t.colsmint  from but_master k left outer join but_detail t  ";
            CHECK_THROW(resultset  = statement.executeQuery(faultystmt_dberror),basar::db::sql::DbException);
            CHECK(resultset.isNull());

            // test double colnames:
            ConstString faultystmt_doublenames =
                "select k.colser , k.colint, k.colsmint,  t.colser, t.colint, t.colsmint  from but_master k left outer join but_detail t  on k.colser = t.colint where k.colser = 10 order by t.colsmint ";
            CHECK_THROW(resultset  = statement.executeQuery(faultystmt_doublenames),basar::InvalidParameterException);
            CHECK(resultset.isNull());

            // test outer join succeeds:
            ConstString stmt =
                "select k.colser as master_colser , k.colint as master_colint , k.colsmint as master_colsmint ,  t.colser as detail_colser, t.colint as detail_colint, t.colsmint as detail_colsmint from but_master k left outer join but_detail t  on k.colser = t.colint where k.colser = 10 order by t.colsmint";
            resultset  = statement.executeQuery(stmt);
            CHECK(!resultset.isNull());
            CHECK_EQUAL(expectetCols, resultset.getSelectedCols());

            UInt32 cnt = 0;
            while(resultset.next())
            {
                ++cnt;
                CHECK_EQUAL(resultset.getInt32("master_colser"), resultset.getInt32("detail_colint"));
            }
            CHECK_EQUAL(expectetRows,cnt);
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
        connRef.close();
    }
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeQuery_selectleftouterjoin_nodata_indetail)
{
    const basar::UInt32 expectetRows = 1;
    const basar::UInt32 expectetCols = 19;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        TestConnection::TestConnectionType eConnType = iter->first;
        basar::db::sql::ConnectionRef connRef = iter->second;

        try
        {
            StatementRef statement = connRef.createStatement();
            connRef.begin();

            ResultsetRef resultset;
            CHECK(resultset.isNull());

            // test outer join succeeds, but there are no values in detail:
            ConstString stmt =
                "select k.colser as master_colser , "
                "t.colser as detail_colser,"
                "t.colint as detail_colint,"
                " t.colsmint as detail_colsmint , "
                " t.colfl as detail_colfl , "
                " t.colsmfl as detail_colsmfl , "
                " t.coldec as detail_coldec , "
                " t.colmon as detail_colmon , "
                " t.colch1 as detail_colch1 , "
                " t.colch2 as detail_colch2 , "
                " t.colvch1 as detail_colvch1 , "
                " t.colvch2 as detail_colvch2 , "
                " t.colvch3 as detail_colvch3 , "
                " t.colvch4 as detail_colvch4 , "
                " t.collvch as detail_collvch , "
                " t.coldate as detail_coldate , "
                " t.coldt1 as detail_coldt1 , "
                " t.coldt2 as detail_coldt2 , "
                " t.coltime1 as detail_coltime1  "
                " from but_master k left outer join but_detail t  on "
                " k.colser = t.colint where k.colser = 1 order by t.colsmint";
            resultset  = statement.executeQuery(stmt);
            CHECK(!resultset.isNull());
            CHECK_EQUAL(expectetCols, resultset.getSelectedCols());

            UInt32 cnt = 0;
            while(resultset.next())
            {
                ++cnt;
                
                CHECK_EQUAL           (Defines::csm_int32DefaultNullValue  , resultset.getInt32("detail_colint"));
                CHECK_EQUAL           (Defines::csm_int16DefaultNullValue  , resultset.getInt16("detail_colsmint"));
                CHECK_EQUAL           (Defines::csm_float64DefaultNullValue, resultset.getFloat64("detail_colfl"));
                CHECK_EQUAL           (Defines::csm_float32DefaultNullValue, resultset.getFloat32("detail_colsmfl"));
                CHECK_EQUAL_SHOW_FUNCT(Defines::csm_decimalDefaultNullValue, resultset.getDecimal("detail_coldec"), toString);
                CHECK_EQUAL_SHOW_FUNCT(Defines::csm_decimalDefaultNullValue, resultset.getDecimal("detail_colmon"), toString);
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colch1"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colch2"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colvch1"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colvch2"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colvch3"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_colvch4"));
                CHECK_EQUAL           (Defines::csm_stringDefaultNullValue , resultset.getString("detail_collvch"));
                if (TestConnection::MSSQL_ODBC == eConnType)
                {
                    basar::Date d(resultset.getDateTime("detail_coldate").getDate());
                    CHECK_EQUAL_SHOW_FUNCT(Defines::csm_dateDefaultNullValue   , d,toStrDate);
                }
                else
                {
                    CHECK_EQUAL_SHOW_FUNCT(Defines::csm_dateDefaultNullValue   , resultset.getDate("detail_coldate"),toStrDate);
                }

                //CHECK_EQUAL_SHOW_FUNCT2(Defines::csm_dateTimeDefaultNullValue, resultset.getDateTime("detail_coldt1"),toStrDate, toStrTime);
                //CHECK_EQUAL_SHOW_FUNCT2(Defines::csm_dateTimeDefaultNullValue, resultset.getDateTime("detail_coldt2"),toStrDate,toStrTime);

                //CHECK_EQUAL_SHOW_FUNCT2(Defines::csm_dateTimeDefaultNullValue, resultset.getTime("detail_coltime1"),toStrDate,toStrTime);
                //CHECK_EQUAL_SHOW_FUNCT2(Defines::csm_dateTimeDefaultNullValue, resultset.getTime("detail_coltime1"),toStrDate,toStrTime);
            }
            CHECK_EQUAL(expectetRows,cnt);
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
        connRef.close();
    }
}

// ----------------------------------------------------------

TEST(testdbsqlstatementref_executeAggregate_infx)
{
    //only in infx:
    basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::INFX);
    try
    {
        StatementRef statement = connRef.createStatement();
        connRef.begin();

        ConstString faultystmt1 =	"select count(*),  min(colint) from but_master ";
        CHECK_THROW( statement.executeAggregate(faultystmt1), basar::InvalidStateException);

        ConstString stmt =	"select count(*) as num from but_master ";

        basar::Decimal countresult  = statement.executeAggregate(stmt);

        DefaultMasterBUTTable  & defaulttable = DefaultMasterBUTTable::getInstance();
        basar::Int32 numberOfRowsInDefaultMasterBUTTable = (basar::Int32)( defaulttable.getNumberOfRows());
        CHECK_EQUAL( numberOfRowsInDefaultMasterBUTTable , countresult.toLong32());

        ConstString stmt2 =	"select colmon from but_master where colser = 1 ";
        basar::Decimal onevalue = statement.executeAggregate(stmt2);
        CHECK_CLOSE(defaulttable.getDecimal(1,BUTTable::colnumber_colmon).toFloat64(), 
                    onevalue.toFloat64(), 
                    Defines::csm_defaultDecimalCloseTolerance	);

        ConstString stmt3 =	"select colser from but_master where colser = 1 ";
        basar::Decimal onevalue2 = statement.executeAggregate(stmt3);
        CHECK_EQUAL(((Int32)1), onevalue2.toLong32() );

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
    connRef.close();
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeInsert_byInsertSetRef)
{

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

        try
        {
            StatementRef statement = connRef.createStatement();
            connRef.begin();

            InsertsetRef insertsetref;
            CHECK(insertsetref.isNull());

            UInt32 expectedRows = 0;

            // test faultystatement with ?
            ConstString faultystatement = " insert into but_var  (colint, colsmint) values ( 0815, ?) ";
            if (  (TestConnection::ODBC == iter->first ) || (TestConnection::ODBC_BG == iter->first ) ||
                  (TestConnection::MSSQL_ODBC == iter->first)
               )
                CHECK_THROW(insertsetref  = statement.executeInsert(faultystatement),  basar::InvalidStateException);
            else
            {
                insertsetref  = statement.executeInsert(faultystatement);
                CHECK_THROW(insertsetref.insertRow(), basar::db::sql::DbException);
            
	            CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	

				ConstString stmt = " insert into but_var  (colint, colsmint) values ( 0815, 0815) ";
				insertsetref  = statement.executeInsert(stmt);
				insertsetref.insertRow();
				expectedRows++;
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	

				insertsetref.insertRow();
				expectedRows++;
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	

				UInt32 flushedRows = insertsetref.flushRows();	
				CHECK_EQUAL(expectedRows,flushedRows);

				connRef.rollback();
			}
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
        connRef.close();
    }
}

#ifdef HAS_ODBC

// ----------------------------------------------------------
TEST_FIXTURE(OdbcFix, testdbsqlstatementref_executeInsert_intfloatdec_odbc)
{
    basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::ODBC);
    try
    {
        StatementRef statement = connRef.createStatement();
        connRef.begin();
        InsertsetRef insertsetref;

        ConstString stmt = " insert into but_var  (colint, colsmint, colfl, colsmfl, coldec, colmon) "
            "values ( 815,   815,    0.815, 0.815,   0.815,  0.815) ";

        CHECK_THROW(insertsetref  = statement.executeInsert(stmt), basar::InvalidStateException);
        connRef.rollback();
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
#endif

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeInsert_intfloatdec_infx)
{
    basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::INFX);
    try
    {
        StatementRef statement = connRef.createStatement();
        connRef.begin();

        InsertsetRef insertsetref;
        CHECK(insertsetref.isNull());
        UInt32 expectedRows = 1;
        ConstString stmt = " insert into but_var  (colint, colsmint, colfl, colsmfl, coldec, colmon) "
            "values ( 815,   815,    0.815, 0.815,   0.815,  0.815) ";

        insertsetref  = statement.executeInsert(stmt);
        insertsetref.insertRow();		
        UInt32 flushedRows = insertsetref.flushRows();	
        CHECK_EQUAL(expectedRows,flushedRows);

        connRef.rollback();
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
    connRef.close();
}

// ----------------------------------------------------------
TEST(testdbsqlstatementref_executeInsert_strings_infx)
{
    basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::INFX);
    try
    {
        StatementRef statement = connRef.createStatement();


        InsertsetRef insertsetref;
        CHECK(insertsetref.isNull());

        UInt32 expectedRows = 1;
        // test 1 string length = 1, ok:
        {
            ConstString stmt = " insert into but_var  (colint, colch1) "
                "values ( 2001, \"1\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }
        // test 2 string is too long, but valid:
        {
            ConstString stmt = " insert into but_var  (colint, colch1) "
                "values ( 2002, \"11\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }
        // test 3 string has length of 0, is valid:
        {
            ConstString stmt = " insert into but_var  (colint, colch1) "
                "values ( 2003, \"\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }

        // test 1b string length = 1, ok:
        {
            ConstString stmt = " insert into but_var  (colint, colvch1) "
                "values ( 2004, \"1\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }
        // test 2b string is too long, but valid:
        {
            ConstString stmt = " insert into but_var  (colint, colvch1) "
                "values ( 2005, \"11\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }
        // test 3b string has length of 0, is valid:
        {
            ConstString stmt = " insert into but_var  (colint, colvch1) "
                "values ( 2006, \"\") ";
            connRef.begin();
            insertsetref  = statement.executeInsert(stmt);
            insertsetref.insertRow();
            UInt32 flushedRows = insertsetref.flushRows();	
            CHECK_EQUAL(expectedRows,flushedRows);
            connRef.rollback();
        }
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
    connRef.close();
}


// ----------------------------------------------------------
TEST(testdbsqlstatement_executeQuery_leftouterjoin_onlywarning_infx)
{
	static ConstString sqlStmt = 

		"SELECT k.colser, count(t.colsmint) "
		"FROM but_master k LEFT OUTER JOIN but_detail t "  
		"ON k.colser = t.colint "
		"WHERE k.colser <> 10 "
		"GROUP BY k.colser;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::INFX);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
			++cnt;
		}

		CHECK_EQUAL(9, cnt);
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
	catch ( ... )
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}
// ----------------------------------------------------------
#ifdef HAS_ODBC
TEST_FIXTURE(OdbcFix, testdbsqlstatement_executeQuery_leftouterjoin_onlywarning_odbc)
{
	static ConstString sqlStmt = 

		"SELECT k.colser, count(t.colsmint) "
		"FROM but_master k LEFT OUTER JOIN but_detail t "  
		"ON k.colser = t.colint "
		"WHERE k.colser <> 10 "
		"GROUP BY k.colser;";

	try
	{
		ConnectionRef connect = TestConnection::createTestConnection(TestConnection::ODBC);
		StatementRef  stmt    = connect.createStatement();
		ResultsetRef  result  = stmt.executeQuery(sqlStmt);

		CHECK(stmt.isOpen  ());
		CHECK(result.isOpen());

		Int32 cnt = 0;

		while (result.next())
		{
			++cnt;
		}

		CHECK_EQUAL(9, cnt);
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
	catch ( ... )
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}
#endif // HAS_ODBC

// ------------------------------------------------------------------------------------------------------------------
TEST(testdbsqlstatement_deletecursor_a)
{
    ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);

    try
    {	
        StatementRef stmt = conn.createStatement();

		conn.begin();

		const BULong      LIM = 20;
		ExecuteReturnInfo ret;
		BULong            cnt = 0;

		// prolog: insert 
		for (BULong i = 0; i < LIM; ++i)
		{
			ret = stmt.execute("insert into but_var (colsmint) values (4122);");
			cnt += ret.m_affectedRows;
		}

		CHECK_EQUAL(LIM, cnt);


		// test
        ResultsetRef resset = stmt.executeQuery("select first 100 1 from but_var where colsmint = 4122 for update;");
		cnt = 0;

        while (resset.next())
        {
            cnt += resset.execCurrentRow("delete from but_var");	
        }

		CHECK(LIM == cnt);


		// epilog: check
		Decimal rows = stmt.executeAggregate("select count(*) from but_var where colsmint = 4122;");
		CHECK_CLOSE(0, rows.toFloat64(), 0.0001);

        conn.rollback();
    }
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown Exception " << std::endl ;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------------------------
TEST(testdbsqlstatement_deletecursor_b)
{
    ConnectionRef conn = TestConnection::createTestConnection(TestConnection::INFX);

    try
    {	
        StatementRef stmt  = conn.createStatement();

		conn.begin();

		const BULong      LIM = 20;
		ExecuteReturnInfo ret;
		BULong            cnt = 0;

		// prolog: insert 
		for (BULong i = 0; i < LIM; ++i)
		{
			ret = stmt.execute("insert into but_var (colsmint) values (4122);");
			cnt += ret.m_affectedRows;
		}

		CHECK_EQUAL(LIM, cnt);


		// test
        ResultsetRef resset = stmt.executeQuery("select first 100 1 from but_var where colsmint = 4122 for update;");

        resset.next();
		CHECK_THROW(stmt.execute("delete from but_var where current of"), DbException);

		// epilog: check
		Decimal rows = stmt.executeAggregate("select count(*) from but_var where colsmint = 4122;");
		CHECK_CLOSE((int)LIM, (int)rows.toFloat64(), 0.0001);

        conn.rollback();
    }
    catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        std::cout << "unknown Exception " << std::endl ;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

#endif

//------------------------------------------------------------------------------
}	// SUITE

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

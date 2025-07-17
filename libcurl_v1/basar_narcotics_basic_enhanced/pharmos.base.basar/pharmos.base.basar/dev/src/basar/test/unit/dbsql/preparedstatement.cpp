 
#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_decimal.h"

#include "utdbsqlhelper.h"
#include "testconnection.h"
#include "buttable.h"
#include "defaultmasterbuttable.h"

#include <iostream>

//------------------------------------------------------------------------------
using basar::db::sql::ConnectionRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ExecuteReturnInfo;

using basar::ConstString;
using basar::UInt32;
using basar::BULong;

using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::DefaultMasterBUTTable;
using basar::test::unit::dbsql::BUTTable;


//------------------------------------------------------------------------------
SUITE(PrepStmt)
{

//------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_execute_after_close)
{
	TestConnection::TestConnectionType eConnType    = TestConnection::INFX;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;

    try
    {
		for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
		{	
			eConnType = iter->first;
			ConnectionRef connRef   = iter->second;

			connRef.begin();
			UInt32 rownumber = 1;

			ConstString stmt =" select colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon , "
									"colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch, "
									"coldate ,coldt1 ,coldt2 ,coltime1 "
								"from but_master where colser = ? ";

			PreparedStatementRef statement;
			CHECK(statement.isNull());
			statement = connRef.createPreparedStatement(stmt);

			CHECK(!statement.isNull());
			CHECK(statement.isOpen());

			statement.setInt32(0,rownumber);

			statement.close();

			CHECK_THROW(statement.executeQuery(),		basar::InvalidStateException);
			CHECK_THROW(statement.execute(),			basar::InvalidStateException);
			CHECK_THROW(statement.executeSingleQuery(), basar::InvalidStateException);
			
			if(TestConnection::INFX == eConnType || TestConnection::INFX_BG == eConnType)
			{
				CHECK_THROW(statement.executeInsert(), basar::InvalidStateException);
			}
			
			connRef.close();

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
TEST(testdbsqlpreparedstatementref_select_query)
{
	TestConnection::TestConnectionType eConnType;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap::const_iterator iter;

		for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
		{	
			eConnType = iter->first;
			ConnectionRef connRef   = iter->second;

			connRef.begin();
			UInt32 rownumber = 1;

		ConstString stmt = 
			"select "
			"colser, colint, colsmint, colfl, colsmfl, coldec, colmon, "
			"colch1, colch2, colvch1, colvch2, colvch3, colvch4, collvch, "
			"coldate, coldt1, coldt2,coltime1 "
			"from but_master "
		    "where colser = ?";

			PreparedStatementRef statement;
			CHECK(statement.isNull());
			statement = connRef.createPreparedStatement(stmt);

			CHECK(!statement.isNull());
			CHECK(statement.isOpen());

		statement.setInt32(0, rownumber);

		ResultsetRef res = statement.executeQuery();

		DefaultMasterBUTTable  & tbl = DefaultMasterBUTTable::getInstance();
			UInt32 cnt = 0;

		while (res.next())
			{
				cnt++;

			CHECK_EQUAL(tbl.getInt32 (rownumber, BUTTable::colnumber_colser  ), res.getInt32 (BUTTable::colnumber_colser  ));
			CHECK_EQUAL(tbl.getInt32 (rownumber, BUTTable::colnumber_colint  ), res.getInt32 (BUTTable::colnumber_colint  ));
			CHECK_EQUAL(tbl.getInt16 (rownumber, BUTTable::colnumber_colsmint), res.getInt16 (BUTTable::colnumber_colsmint));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colch1  ), res.getString(BUTTable::colnumber_colch1  ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colch2  ), res.getString(BUTTable::colnumber_colch2  ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colvch1 ), res.getString(BUTTable::colnumber_colvch1 ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colvch2 ), res.getString(BUTTable::colnumber_colvch2 ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colvch3 ), res.getString(BUTTable::colnumber_colvch3 ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_colvch4 ), res.getString(BUTTable::colnumber_colvch4 ));
			CHECK_EQUAL(tbl.getString(rownumber, BUTTable::colnumber_collvch ), res.getString(BUTTable::colnumber_collvch ));

				if (TestConnection::MSSQL_ODBC == eConnType)
				{
				CHECK_EQUAL_SHOW_FUNCT(tbl.getDate(rownumber,BUTTable::colnumber_coldate), basar::Date(res.getDateTime(BUTTable::colnumber_coldate ).getDate()), toStrDate);
				}
				else
				{
                CHECK_EQUAL_SHOW_FUNCT(tbl.getDate(rownumber,BUTTable::colnumber_coldate), res.getDate(BUTTable::colnumber_coldate ), toStrDate);				
				}
				
			CHECK_EQUAL(tbl.getDateTime(rownumber,BUTTable::colnumber_coldt1).getDate(), res.getDateTime(BUTTable::colnumber_coldt1).getDate());
			CHECK_EQUAL(tbl.getDateTime(rownumber,BUTTable::colnumber_coldt1).getTime(), res.getDateTime(BUTTable::colnumber_coldt1).getTime());

			CHECK_EQUAL(tbl.getDateTime(rownumber,BUTTable::colnumber_coldt2).getDate(), res.getDateTime(BUTTable::colnumber_coldt2).getDate());
			CHECK_EQUAL(tbl.getDateTime(rownumber,BUTTable::colnumber_coldt2).getTime(), res.getDateTime(BUTTable::colnumber_coldt2).getTime());

                if (TestConnection::MSSQL_ODBC == eConnType)
                {
				CHECK_EQUAL(tbl.getTime(rownumber, BUTTable::colnumber_coltime1 ).getDate(), 
							res.getDateTime(BUTTable::colnumber_coltime1        ).getDate());              
				CHECK_EQUAL(tbl.getTime(rownumber, BUTTable::colnumber_coltime1 ).getTime(), 
							res.getDateTime(BUTTable::colnumber_coltime1        ).getTime());                
                }
                else
                {
				CHECK_EQUAL(tbl.getTime(rownumber, BUTTable::colnumber_coltime1 ).getDate(), 
							res.getTime(BUTTable::colnumber_coltime1            ).getDate());              
				CHECK_EQUAL(tbl.getTime(rownumber, BUTTable::colnumber_coltime1 ).getTime(), 
							res.getTime(BUTTable::colnumber_coltime1            ).getTime());                
                }

			CHECK_EQUAL(tbl.getFloat64(rownumber,BUTTable::colnumber_colfl  ), res.getFloat64(BUTTable::colnumber_colfl   ));
			CHECK_CLOSE(tbl.getFloat32(rownumber,BUTTable::colnumber_colsmfl), res.getFloat32(BUTTable::colnumber_colsmfl ), 0.001);

			CHECK_CLOSE(tbl.getDecimal(rownumber,BUTTable::colnumber_coldec ).toFloat64(), res.getDecimal(BUTTable::colnumber_coldec  ).toFloat64(), 0.000001);
			CHECK_CLOSE(tbl.getDecimal(rownumber,BUTTable::colnumber_colmon ).toFloat64(), res.getDecimal(BUTTable::colnumber_colmon  ).toFloat64(), 0.000001);
			}

		CHECK_EQUAL(1, (int)cnt);

			statement.close();
			connRef.close();
		}
}

//------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_execute_update)
{
    const basar::UInt32 colserNumber = 11;
    const BULong        expectedRows = 1;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {
            PreparedStatementRef statement ;
            {
                CHECK(statement.isNull());
                ConstString stmt = "update but_detail set colsmint = colsmint + 1 where colser = ?";
                connRef.begin();

                statement = connRef.createPreparedStatement(stmt);

                statement.setInt32(0, colserNumber);

                ExecuteReturnInfo	execResult = statement.execute();

                CHECK_EQUAL(expectedRows,execResult.m_affectedRows);
                CHECK_EQUAL(0, static_cast<int>(execResult.m_lastSerial.size()));

                connRef.commit();
            }

            statement.close();
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

//------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_execute_insert)
{
    const BULong expectedRows = 1;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {
            PreparedStatementRef statement ;
            {
                CHECK(statement.isNull());
                ConstString stmt = "insert into but_var  (colint, colsmint) values ( 22, ?)";
                connRef.begin();
                statement = connRef.createPreparedStatement(stmt);
                {
                    statement.setInt16(0,0);
                    ExecuteReturnInfo	execResult = statement.execute();
                    CHECK_EQUAL(expectedRows,execResult.m_affectedRows);

                    if ((TestConnection::INFX == iter->first) || (TestConnection::INFX_BG == iter->first))
                    {
                        CHECK_EQUAL(1, static_cast<int>( execResult.m_lastSerial.size()));
                    }

					CHECK(2 > execResult.m_lastSerial.size());

                    CHECK_EQUAL(basar::db::sql::DBErrorCode::SUCCESS,execResult.m_error);    
                    if (execResult.m_lastSerial.size() > 0 )
                    {
                        CHECK( execResult.m_lastSerial[0]> 0);
                    }
                    connRef.rollback();
                }
            }
            statement.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        connRef.close();
    }
}

//------------------------------------------------------------------------------
TEST(testdbsqlstatementref_execute_insertError_duplInsert)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {
            PreparedStatementRef statement ;
            // duplicate insert:
            {

                CHECK(statement.isNull());
                ConstString stmt = "insert into but_detail (colser, colint, colsmint) values ( ?, 22, 0)";
                connRef.begin();
                statement = connRef.createPreparedStatement(stmt);
                {
                    statement.setInt32(0, 1);
                    ExecuteReturnInfo	execResult = statement.execute(false);

                    if ( (TestConnection::ODBC       == iter->first) ||
			             (TestConnection::ODBC_BG    == iter->first) ||
			             (TestConnection::MSSQL_ODBC == iter->first)  )
                    {
                        CHECK_EQUAL(basar::db::sql::DBErrorCode::UNKNOWN,execResult.m_error);
                        CHECK_THROW(connRef.rollback(), basar::Exception);  
                    }
                    else
                    {
                        CHECK_EQUAL(basar::db::sql::DBErrorCode::INSERT_DUPL,execResult.m_error);
                        connRef.rollback() ;  
                    } 
                    CHECK_EQUAL(0, (int)execResult.m_affectedRows);
                    CHECK_EQUAL(0, static_cast<int>( execResult.m_lastSerial.size()));
                } 
                statement.close();
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
            std::cout << "Exception at Connection = " << iter->first << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        connRef.close();
    }
}

// -------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_executeQuery)
{
    const basar::UInt32 colserNumber = 10;
    const basar::UInt32 expectedRows = 9;
    const basar::UInt32 expectetCols = 6;

    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

        TestConnection::TestConnectionType eConnType = iter->first;

        try
        {
            PreparedStatementRef statement ;
            CHECK(statement.isNull());

            connRef.begin();

            ResultsetRef resultset;
            CHECK(resultset.isNull());

            // test faulty db-string:
            ConstString faultystmt_dberror = "select k.colser , k.colint, k.colsmint,  t.colser, t.colint, t.colsmint "
					                            "from but_master k left outer join but_detail t  ";
            
            //strangly MSSQL returns no error via ODBC for the above statement
            if (TestConnection::MSSQL_ODBC != eConnType)
            {
                CHECK_THROW(statement = connRef.createPreparedStatement(faultystmt_dberror), basar::db::sql::DbException);
                CHECK(statement.isNull());
            }

            // test double colnames:
            ConstString faultystmt_doublenames =
                "select k.colser , k.colint, k.colsmint,  t.colser, t.colint, t.colsmint "
				"from but_master k left outer join but_detail t  on k.colser = t.colint "
				"where k.colser = ? order by t.colsmint ";
            statement = connRef.createPreparedStatement(faultystmt_doublenames);
            CHECK(!statement.isNull());
            statement.setInt32(0,colserNumber);
            CHECK_THROW(resultset  = statement.executeQuery(),basar::InvalidParameterException);
            CHECK(resultset.isNull());
            statement.close();

			// test parameter not set:
            ConstString stmt_unique =
                "select k.colser as master_colser , k.colint as master_colint , k.colsmint as master_colsmint ,  "
					    "t.colser as detail_colser, t.colint as detail_colint, t.colsmint as detail_colsmint "
				"from but_master k left outer join but_detail t on k.colser = t.colint "
				"where k.colser = ? order by t.colsmint";
            statement = connRef.createPreparedStatement(stmt_unique);
            CHECK(!statement.isNull());
            CHECK_THROW(resultset  = statement.executeQuery(),basar::InvalidParameterException);
            CHECK(resultset.isNull());
            statement.close();

            // test outer join succeeds:
            ConstString stmt =
                "select k.colser as master_colser , k.colint as master_colint , k.colsmint as master_colsmint ,  "
					    "t.colser as detail_colser, t.colint as detail_colint, t.colsmint as detail_colsmint "
				"from but_master k left outer join but_detail t  on k.colser = t.colint "
				"where k.colser = ? order by t.colsmint";
            statement = connRef.createPreparedStatement(stmt);
            statement.setInt32(0,colserNumber);
            resultset  = statement.executeQuery();
            CHECK(!resultset.isNull());
            CHECK_EQUAL(expectetCols, resultset.getSelectedCols());

            UInt32 cnt = 0;
            while(resultset.next())
            {
                ++cnt;
                CHECK_EQUAL(resultset.getInt32("master_colser"), resultset.getInt32("detail_colint"));
            }
            CHECK_EQUAL(expectedRows,cnt);
            statement.close();
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

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_insert_introws)
{
    try
    {
		TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
		TestConnection::TestConnectionMap ::const_iterator iter;

		for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
		{	
			ConnectionRef connRef = iter->second;

			connRef.begin();
			UInt32 testvalue = 2341;

			ConstString stmt = " insert into but_var  (colint, colsmint) values ( ?, 2411) ";

			PreparedStatementRef statement = connRef.createPreparedStatement(stmt);

			InsertsetRef insertsetref;
			
			if ( (TestConnection::ODBC       == iter->first) ||
			     (TestConnection::ODBC_BG    == iter->first) ||
			     (TestConnection::MSSQL_ODBC == iter->first)  )
			{
				CHECK_THROW(insertsetref  = statement.executeInsert(), basar::InvalidStateException);
		    }
			else
			{
				insertsetref  = statement.executeInsert();

				UInt32 expectedRows = 0;

				// no data yet:
				CHECK_THROW(insertsetref.insertRow(), basar::db::sql::DbException);
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
				statement.setInt32(0,testvalue);
				insertsetref.insertRow();
				expectedRows++;

				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
				insertsetref.insertRow();
				expectedRows++;
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	

				UInt32 flushedRows = insertsetref.flushRows();	
				CHECK_EQUAL(expectedRows,flushedRows);
				connRef.rollback();
				statement.close();
				connRef.close();
			}
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
}

// -------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_looping_selects)
{
	TestConnection::TestTypes types;
	types.push_back(TestConnection::INFX);
	types.push_back(TestConnection::INFX_BG);

    try
    {
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef connRef = itConn->second;


            ConstString outerStmt = "select first 10 colser from but_master";
            ConstString innerStmt = "select * from but_master where colser = ?";

            PreparedStatementRef inner = connRef.createPreparedStatement(innerStmt);          

            PreparedStatementRef outer = connRef.createPreparedStatement(outerStmt);

            ResultsetRef outerRes = outer.executeQuery();

            while(outerRes.next())
            {
                
                basar::Int32 ser = outerRes.getInt32("colser");                
                
                ResultsetRef innerRes;
                
                inner.setInt32(0, ser);

                innerRes = inner.executeQuery();

                innerRes.next();

                CHECK_EQUAL(ser, innerRes.getInt32("colser"));

            }

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
}

// -------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_insert_EmptyString_infx)
{
	TestConnection::TestTypes types;
	types.push_back(TestConnection::INFX);
	types.push_back(TestConnection::INFX_BG);

    basar::ConstString empty = "";

    try
    {
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef connRef = itConn->second;
			connRef.begin();

			ConstString stmt = " insert into but_var ( colvch1 , colvch2, colvch3, colvch4, collvch) "
					            " values "
								                    "( "
													"             ?,       ?,       ?,       ?,       ?) ";

			PreparedStatementRef statement = connRef.createPreparedStatement(stmt);

			int          i            = 0;

			statement.setString(i++, empty);
			statement.setString(i++, empty);
			statement.setString(i++, empty);
			statement.setString(i++, empty);
			statement.setString(i++, empty);

            ExecuteReturnInfo eri;

			try
            {
                 eri = statement.execute();
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


            CHECK(!eri.hasError());

			connRef.rollback();
			statement.close();
		    connRef.close();

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
}

// -------------------------------------------------------------------------------
TEST(testdbsqlpreparedstatementref_insert_StringFloatDecimal_infx)
{
	TestConnection::TestTypes types;
	types.push_back(TestConnection::INFX);
	types.push_back(TestConnection::INFX_BG);
    try
    {
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef connRef = itConn->second;
			connRef.begin();

			ConstString stmt = " insert into but_var (colint, colsmint, colfl   , colsmfl, coldec , colmon, "
					                                    "colch1, colch2  , colvch1 , colvch2, colvch3, colvch4, collvch) "
					            " values "
								                    "(     ?,        ?,        ?,       ?,       ?,       ?, "
													"      ?,        ?,        ?,       ?,       ?,       ?,       ?) ";

			PreparedStatementRef statement = connRef.createPreparedStatement(stmt);

			DefaultMasterBUTTable  & defaulttable = DefaultMasterBUTTable::getInstance();

			InsertsetRef insertsetref  = statement.executeInsert();

			const UInt32 rownumber    = 2;
			UInt32       expectedRows = 0;
			int          i            = 0;

			statement.setInt32(i++,defaulttable.getInt32(rownumber, BUTTable::colnumber_colint  ));
			statement.setInt16(i++,defaulttable.getInt16(rownumber, BUTTable::colnumber_colsmint));
			statement.setFloat64(i++,defaulttable.getFloat64(rownumber, BUTTable::colnumber_colfl   ));
			statement.setFloat32(i++,defaulttable.getFloat32(rownumber, BUTTable::colnumber_colsmfl ));
			statement.setDecimal(i++,defaulttable.getDecimal(rownumber, BUTTable::colnumber_coldec  ));
			statement.setDecimal(i++,defaulttable.getDecimal(rownumber, BUTTable::colnumber_colmon  ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colch1  ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colch2  ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colvch1 ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colvch2 ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colvch3 ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_colvch4 ));
			statement.setString(i++,defaulttable.getString(rownumber, BUTTable::colnumber_collvch ));

			insertsetref.insertRow();

			expectedRows++;
			CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
			UInt32 flushedRows = insertsetref.flushRows();
			CHECK_EQUAL(expectedRows,flushedRows);

			// faulty data colnumber_colvch1 needs to have the length of 1
			statement.setString(BUTTable::colnumber_colvch1 -1,"aa");
			insertsetref.insertRow();	

			insertsetref.flushRows();

			// faulty data colnumber_colvch1 needs to have the length of 1
			statement.setString(BUTTable::colnumber_colch1 -1,"aa");
			insertsetref.insertRow();	

			insertsetref.flushRows();

			connRef.rollback();
			statement.close();
		    connRef.close();
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
}

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_insert_DateTime_infx)
{
	TestConnection::TestTypes types;
	types.push_back(TestConnection::INFX);
	types.push_back(TestConnection::INFX_BG);
    try
    {
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef connRef = itConn->second;

			ConstString stmt = " insert into but_var  (coldate ,coldt1 ,coldt2 ,coltime1) "
				"                              values (    ?,       ?,      ?,       ?) ";


			// insert current date
			{
				connRef.begin();
				PreparedStatementRef statement = connRef.createPreparedStatement(stmt);

				InsertsetRef insertsetref  = statement.executeInsert();
				UInt32 expectedRows = 0;

				int i=0;


                basar::Date date;

				statement.setDate(i++,date);
				statement.setDateTime(i++, basar::DateTime::getCurrent());

				statement.setDateTime(i++, basar::DateTime::getCurrent());

                statement.setDateTime(i++, basar::DateTime::getCurrent());
				insertsetref.insertRow();

				expectedRows++;
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
				UInt32 flushedRows = insertsetref.flushRows();
				CHECK_EQUAL(expectedRows,flushedRows);

				connRef.rollback();
				statement.close();
			}
			ConstString stmt2 = stmt;

			// insert null dates
			{
				connRef.begin();
				PreparedStatementRef statement = connRef.createPreparedStatement(stmt2);

				InsertsetRef insertsetref  = statement.executeInsert();
				UInt32 expectedRows = 0;

				int i=0;

				basar::Date nulldate;
				basar::DateTime nulldatetime;
				nulldate.setInvalid();
				nulldatetime.setInvalid();

				statement.setDate(i++,nulldate);
				statement.setDateTime(i++,nulldatetime);
				statement.setDateTime(i++,nulldatetime);
				statement.setDateTime(i++,nulldatetime);
				insertsetref.insertRow();

				expectedRows++;
				CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
				UInt32 flushedRows = insertsetref.flushRows();
				CHECK_EQUAL(expectedRows,flushedRows);

				connRef.rollback();
				statement.close();
			}

			connRef.close();
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
}

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_insert_allData_infx)
{
	TestConnection::TestTypes types;
	types.push_back(TestConnection::INFX);
	types.push_back(TestConnection::INFX_BG);
    try
    {
		TestConnection::TestConnectionMap conns = TestConnection::createTestConnections(types);

		for (TestConnection::TestConnectionMap::const_iterator itConn = conns.begin();
				itConn != conns.end();
				++itConn)
		{
			ConnectionRef connRef = itConn->second;
			connRef.begin();

			ConstString stmt =	" INSERT INTO BUT_VAR "
								" (colint , colsmint, colfl  , colsmfl, coldec  , colmon , "
								"  colch1 , colch2  , colvch1, colvch2, colvch3 , colvch4, "
								"  collvch, coldate , coldt1 , coldt2 , coltime1) "
								" VALUES "
								" (      ?,        ?,        ?,       ?,       ?,       ?, "
								"        ?,        ?,        ?,       ?,       ?,       ?, "
								"        ?,        ?,        ?,       ?,       ?);         ";

			PreparedStatementRef statement = connRef.createPreparedStatement(stmt);

			DefaultMasterBUTTable & defaulttable = DefaultMasterBUTTable::getInstance();
			InsertsetRef            insertsetref = statement.executeInsert();
			const UInt32            rownumber    = 2;
			UInt32                  expectedRows = 0;
			int                     i            = 0;

			statement.setInt32   (i++,defaulttable.getInt32   (rownumber, BUTTable::colnumber_colint  ));
			statement.setInt16   (i++,defaulttable.getInt16   (rownumber, BUTTable::colnumber_colsmint));
			statement.setFloat64 (i++,defaulttable.getFloat64 (rownumber, BUTTable::colnumber_colfl   ));
			statement.setFloat32 (i++,defaulttable.getFloat32 (rownumber, BUTTable::colnumber_colsmfl ));
			statement.setDecimal (i++,defaulttable.getDecimal (rownumber, BUTTable::colnumber_coldec  ));
			statement.setDecimal (i++,defaulttable.getDecimal (rownumber, BUTTable::colnumber_colmon  ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colch1  ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colch2  ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colvch1 ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colvch2 ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colvch3 ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_colvch4 ));
			statement.setString  (i++,defaulttable.getString  (rownumber, BUTTable::colnumber_collvch ));
			statement.setDate    (i++,defaulttable.getDate    (rownumber, BUTTable::colnumber_coldate ));
			statement.setDateTime(i++,defaulttable.getDateTime(rownumber, BUTTable::colnumber_coldt1  ));
			statement.setDateTime(i++,defaulttable.getDateTime(rownumber, BUTTable::colnumber_coldt2  ));
			statement.setDateTime(i++,defaulttable.getDateTime(rownumber, BUTTable::colnumber_coltime1));

			insertsetref.insertRow();

			expectedRows++;
			CHECK_EQUAL(expectedRows,insertsetref.getInsertedRows());	
			UInt32 flushedRows = insertsetref.flushRows();
			CHECK_EQUAL(expectedRows,flushedRows);

			// faulty data colnumber_colvch1 needs to have the length of 1
			statement.setString(BUTTable::colnumber_colvch1 -1,"aa");
			insertsetref.insertRow();	

			insertsetref.flushRows();

			// faulty data colnumber_colvch1 needs to have the length of 1
			statement.setString(BUTTable::colnumber_colch1 -1,"aa");
			insertsetref.insertRow();	

			insertsetref.flushRows();

			connRef.rollback();
			statement.close();
			connRef.close();
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
}

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_select_query_without_parameter)
{
    const basar::UInt32 expectedRows	=	1;
	const basar::UInt32 key				=   2;
	ConstString			stmt			=	"select colser, colint, colfl "
											"from but_master where colser = 2 ";

	TestConnection::TestConnectionMap connRefs = TestConnection::createDefaultTestConnections();

    for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
			iter != connRefs.end(); 
			++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

        try
        {
            PreparedStatementRef statement;
            CHECK(statement.isNull());

            statement = connRef.createPreparedStatement(stmt);
            CHECK(!statement.isNull());
            CHECK(statement.isOpen());

			ResultsetRef resultSet = statement.executeQuery();

            DefaultMasterBUTTable & defaulttable = DefaultMasterBUTTable::getInstance();

            UInt32 cnt = 0;

            while (resultSet.next())
            {
                cnt++;

                CHECK_EQUAL(defaulttable.getInt32  (key, BUTTable::colnumber_colser), resultSet.getInt32  (0));
                CHECK_EQUAL(defaulttable.getInt32  (key, BUTTable::colnumber_colint), resultSet.getInt32  (1));
                CHECK_EQUAL(defaulttable.getFloat64(key, BUTTable::colnumber_colfl ), resultSet.getFloat64(2));

			}

            CHECK_EQUAL(expectedRows, cnt);

            statement.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

        connRef.close();
	}
}

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_execute_insert_without_parameter)
{
    const BULong  expectedRows	=	1;
	ConstString	  stmt			=	"insert into but_var  (colint, colsmint) values ( 0, 5)";

	TestConnection::TestConnectionMap connRefs = TestConnection::createDefaultTestConnections();

    for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
			iter != connRefs.end(); 
			++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

		try
        {
            PreparedStatementRef statement;
            CHECK(statement.isNull());

            connRef.begin();

            statement = connRef.createPreparedStatement(stmt);
            CHECK(!statement.isNull());
            CHECK(statement.isOpen());

            ExecuteReturnInfo execResult = statement.execute();
            CHECK_EQUAL(expectedRows, execResult.m_affectedRows);
            
            if ( (TestConnection::ODBC == iter->first)      ||
                 (TestConnection::ODBC_BG == iter->first)    )
            {
                CHECK_EQUAL(0, static_cast<int>(execResult.m_lastSerial.size()));
            }
            else
            {
                CHECK_EQUAL(1, static_cast<int>(execResult.m_lastSerial.size()));
            }

            connRef.rollback();
            statement.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

        connRef.close();
    }
}

// -------------------------------------------------------------------------------

TEST(testdbsqlpreparedstatementref_execute_update_without_parameter)
{
    const BULong  expectedRows	=	1;
	ConstString	  stmt			=	"update but_detail set colsmint = colsmint + 1 "
											"where colser = 2";

	TestConnection::TestConnectionMap connRefs = TestConnection::createDefaultTestConnections();

    for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
			iter != connRefs.end(); 
			++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;

		try
        {
            PreparedStatementRef statement;
            CHECK(statement.isNull());

            connRef.begin();

            statement = connRef.createPreparedStatement(stmt);
            CHECK(!statement.isNull());
            CHECK(statement.isOpen());

            ExecuteReturnInfo execResult = statement.execute();
            CHECK_EQUAL(expectedRows, execResult.m_affectedRows);
            CHECK_EQUAL(0, static_cast<int>(execResult.m_lastSerial.size()));

            connRef.rollback();
            statement.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }
        catch (...)
        {
            std::cout << "Exception at Connection = " << iter->first << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
        }

        connRef.close();
    }
}

// -------------------------------------------------------------------------------
TEST(prepstatement_updatecursor)
{
    ConstString  select = "select colser, colint, colsmint "
				          "from but_detail "
						  "where colint = 8 for update";

    ConstString  cursor = "update but_detail set colsmint = 1";

	TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();
    
	for (TestConnection::TestConnectionMap::const_iterator iter = conns.begin(); 
		 iter != conns.end(); 
		 ++iter)
    {	
        ConnectionRef conn = iter->second;

        try
        {	
            PreparedStatementRef stmtSelect = conn.createPreparedStatement(select);
		    PreparedStatementRef stmtCursor;

            if ( (TestConnection::ODBC    == iter->first) || 
				 (TestConnection::ODBC_BG == iter->first) ||
				 (TestConnection::MSSQL_ODBC == iter->first)   )
            {
				CHECK_THROW(conn.createPrepRowStmt(cursor, stmtSelect), 
					        basar::InvalidStateException);
				continue;
            }
			
			stmtCursor = conn.createPrepRowStmt(cursor, stmtSelect);

            conn.begin();

            ResultsetRef res = stmtSelect.executeQuery();

			ExecuteReturnInfo ret;

            while (res.next())
			{
                ret = stmtCursor.execute();	

				CHECK_EQUAL(1, (int)ret.getAffectedRows());
			}

			conn.rollback();

			stmtCursor.close();

            res       .close();
            stmtSelect.close();
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

        conn.close();
    }
}

// -------------------------------------------------------------------------------
TEST(prepstatement_deletecursor)
{
    ConstString  select = "select colser, colint, colsmint "
				          "from but_detail "
						  "where colint = 8 for update";

    ConstString  cursor = "delete from but_detail";

	TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();
    
	for (TestConnection::TestConnectionMap::const_iterator iter = conns.begin(); 
		 iter != conns.end(); 
		 ++iter)
    {	
        ConnectionRef conn = iter->second;

        try
        {	
            PreparedStatementRef stmtSelect = conn.createPreparedStatement(select);
		    PreparedStatementRef stmtCursor;

            if ( (TestConnection::ODBC    == iter->first) || 
				 (TestConnection::ODBC_BG == iter->first) ||
				 (TestConnection::MSSQL_ODBC == iter->first)  )
            {
				CHECK_THROW(conn.createPrepRowStmt(cursor, stmtSelect), 
					        basar::InvalidStateException);
				continue;
            }
			
			stmtCursor = conn.createPrepRowStmt(cursor, stmtSelect);

            conn.begin();

            ResultsetRef res = stmtSelect.executeQuery();

			ExecuteReturnInfo ret;

            while (res.next())
			{
                ret = stmtCursor.execute();	

				CHECK_EQUAL(1, (int)ret.getAffectedRows());
			}

			conn.rollback();

			stmtCursor.close();

            res       .close();
            stmtSelect.close();
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

        conn.close();
    }
}

// -------------------------------------------------------------------------------
TEST(prepstatement_aggregatequery)
{
    ConstString  select = "select count(*) "
				        "from but_master "
						"where colser > ?";
    
    const basar::Decimal expectedCount(8);
    
    TestConnection::TestConnectionMap conns = TestConnection::createDefaultTestConnections();
    
	for (TestConnection::TestConnectionMap::const_iterator iter = conns.begin(); 
		 iter != conns.end(); 
		 ++iter)
    {	
        ConnectionRef conn = iter->second;
        TestConnection::TestConnectionType type = iter->first;

        try
        {
            PreparedStatementRef stmtSelect = conn.createPreparedStatement(select);
            
            if ( (TestConnection::ODBC    == type) || 
				 (TestConnection::ODBC_BG == type) ||
				 (TestConnection::MSSQL_ODBC == type)  )
            {
				CHECK_THROW(stmtSelect.executeAggregate(), 
					        basar::NotImplementedException);
				continue;
            }
            else
            {
                basar::Int32 value(2);
                
                stmtSelect.setInt32(0, value);            
                
                basar::Decimal resultCount = stmtSelect.executeAggregate();
                
                CHECK_EQUAL(expectedCount, resultCount);
            }            
            	
        }
        catch (basar::Exception & ex)
        {
            std::cout <<  "Exception at Connection = " << type << std::endl 
                      << ex.what().c_str() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);        
        }
        catch(...)
        {
            std::cout << "Exception at Connection = " << type << std::endl ;

            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);            
        }
        
        conn.close();
    }
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
}	// SUITE

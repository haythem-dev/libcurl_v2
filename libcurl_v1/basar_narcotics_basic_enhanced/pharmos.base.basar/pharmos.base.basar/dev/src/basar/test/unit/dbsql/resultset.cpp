#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasarcmnutil_i18nstring.h"

#include "utdbsqlhelper.h"
#include "testconnection.h"
#include "buttable.h"
#include "defaultmasterbuttable.h"
#include "odbcfix.h"

#include <iostream>

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::UInt32;
using basar::Int32;

using basar::db::sql::ConnectionRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;

// test:
using basar::test::unit::dbsql::TestConnection;
using basar::test::unit::dbsql::DefaultMasterBUTTable;
using basar::test::unit::dbsql::BUTTable;
using basar::test::unit::dbsql::Defines;

#define TestDBsqlResultSetRefTests
#ifdef TestDBsqlResultSetRefTests

//------------------------------------------------------------------------------
SUITE(Resultset)
{

//------------------------------------------------------------------------------
TEST(NullReferenceException)
{
    try
    {
        ResultsetRef resultset;
        CHECK(resultset.isNull());
        CHECK_THROW(resultset.next(), basar::NullReferenceException);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}
//------------------------------------------------------------------------------

TEST(InvalidStateExceptionOnGetWithoutCallingNext)
{
    //infx
    {
        basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::INFX);

        ConstString statementstring = "select first 1 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colch1 ,colmon ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1, coldt1 - coldt2 as colintrv from but_master";

        StatementRef stmt = connRef.createStatement();

        ResultsetRef res = stmt.executeQuery(statementstring);

        //only check each access type (int16, int32, etc) once
        CHECK_THROW(res.getInt32("colser"), basar::InvalidStateException);
        CHECK_THROW(res.getInt16("colsmint"), basar::InvalidStateException);
        CHECK_THROW(res.getFloat64("colfl"), basar::InvalidStateException);
        CHECK_THROW(res.getFloat32("colsmfl"), basar::InvalidStateException);
        CHECK_THROW(res.getDecimal("coldec"), basar::InvalidStateException);
        CHECK_THROW(res.getString("colch1"), basar::InvalidStateException);
        CHECK_THROW(res.getDateTime("coldt1"), basar::InvalidStateException);
        CHECK_THROW(res.getDate("coldate"), basar::InvalidStateException);
        CHECK_THROW(res.getTime("coltime1"), basar::InvalidStateException);
        CHECK_THROW(res.getTimeSpan("colintrv"), basar::InvalidStateException);
    }

#ifdef HAS_ODBC
    //odbc
    {
        basar::db::sql::ConnectionRef connRef = TestConnection::createTestConnection(TestConnection::ODBC);

        ConstString statementstring = "select first 1 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colch1 ,colmon ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";

        StatementRef stmt = connRef.createStatement();

        ResultsetRef res = stmt.executeQuery(statementstring);

        //only check each access type (int16, int32, etc) once
        CHECK_THROW(res.getInt32("colser"), basar::InvalidStateException);
        CHECK_THROW(res.getInt16("colsmint"), basar::InvalidStateException);
        CHECK_THROW(res.getFloat64("colfl"), basar::InvalidStateException);
        CHECK_THROW(res.getString("colch1"), basar::InvalidStateException);
        CHECK_THROW(res.getDateTime("coldt1"), basar::InvalidStateException);
        CHECK_THROW(res.getDate("coldate"), basar::InvalidStateException);
        CHECK_THROW(res.getTime("coltime1"), basar::InvalidStateException);
    }
#endif
}

//------------------------------------------------------------------------------
TEST(stringAfterEmptyString)
{
        basar::db::sql::DatabaseInfo dbInfo;

        dbInfo.dbServer = ESQLC_DBSERVER;
        dbInfo.database = ESQLC_DB_WEST;
        dbInfo.concurrTransaction = false;

        try
        {
            ConnectionRef conn = basar::db::sql::Manager::getInstance().createConnect(dbInfo);

            StatementRef stmt = conn.createStatement();

            //setup

            std::ostringstream os;

            {
                os << "create temp table mytest (id integer default 0 not null, mytext lvarchar(2048) default '' not null)";

                stmt.execute(os.str()); //don't care for return here, throws on error anyway

                os.str("");

                os << "insert into mytest(id) values (1)";

                stmt.execute(os.str());

                os.str("");

                os << "insert into mytest(id, mytext) values (2, 'hallo test')";

                stmt.execute(os.str());

                os.str("");
            }

            //test

            os << "select * from mytest order by id";

            ResultsetRef res = stmt.executeQuery(os.str());

            CHECK(res.next());

            do
            {
                basar::VarString s = res.getString(1);
            }
            while(res.next());

        }
        catch (basar::Exception& ex)
        {
            std::cout << "caught exception: " << ex.what() << std::endl;
            CHECK(false);
        }
}

//------------------------------------------------------------------------------
TEST(columnNamesAndTypes)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {
        basar::db::sql::ConnectionRef connRef = iter->second;
        TestConnection::TestConnectionType eConnType = iter->first;
        try
        {
            StatementRef statement1 = connRef.createStatement();
            connRef.begin();

            // statements :
            ConstString statementstring = "select first 1 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colch1 ,colmon ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
            ConstString statementstringMSSQL = "select top 1 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colch1 ,colmon ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
            const basar::UInt32 expectednumberOfCols = BUTTable::colnumber_coltime1 + 1;

            ResultsetRef resultSet1;

            if ( TestConnection::MSSQL_ODBC == eConnType)
            {
                resultSet1 = statement1.executeQuery(statementstringMSSQL);
            }
            else
            {
                resultSet1 = statement1.executeQuery(statementstring);
            }

            CHECK(!resultSet1.isNull());
            CHECK_EQUAL(expectednumberOfCols, resultSet1.getSelectedCols());

            // check column names
            {
                const basar::db::sql::CollColNames& colNames = resultSet1.getColumnNames();

                //get nice columnNames from statementstring
                basar::VarString columnNamesRaw(statementstring);
                columnNamesRaw = columnNamesRaw.substr(15, 152);
                basar::cmnutil::CollBString columnNames;
                columnNamesRaw.tokenizeAndTrim(columnNames, ",", basar::cmnutil::BString::ALL);

                for (basar::UInt32 i = 0; i < expectednumberOfCols; ++i)
                {
                    CHECK_EQUAL(columnNames.at(i), colNames.at(i));
                }
            }


                //check column types
                {
                    //giev initalizer lists plz
                    std::vector<basar::SupportedTypeEnum> expectedTypes;

                    if ( TestConnection::MSSQL_ODBC != eConnType && TestConnection::ODBC != eConnType && TestConnection::ODBC_BG != eConnType )
                    {
                        //expected types from informix
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT16 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::FLOAT32 );
                        expectedTypes.push_back( basar::DECIMAL );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::DECIMAL );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::DATE );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                    }
                    else if ( TestConnection::MSSQL_ODBC != eConnType )
                    {
                        //expected types from odbc
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::DATE );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::TIME );
                    }
                    else
                    {
                        //expected types from mssql odbc
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::INT32 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::FLOAT64 );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::STRING );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                        expectedTypes.push_back( basar::DATETIME );
                    }

                    const basar::db::sql::CollColTypes& colTypes = resultSet1.getColumnTypes();

                    for ( basar::UInt32 i = 0; i < expectednumberOfCols; ++i )
                    {
                        CHECK_EQUAL( expectedTypes.at( i ), colTypes.at( i ) );
                    }
                }

            resultSet1.close();
            statement1.close();
        }
        catch (basar::Exception & ex)
        {
            std::cout << "Exception at Connection = " 
                << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
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
TEST(checkDiffDataTwoResults_checkDefaultData)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        basar::db::sql::ConnectionRef connRef = iter->second;
        TestConnection::TestConnectionType eConnType = iter->first;
        try
        {	
            StatementRef statement1 = connRef.createStatement();
            StatementRef statement2 = connRef.createStatement();
            connRef.begin();

            // statements :
            ConstString statementstring = "select first 2 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon ,colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
            const basar::UInt32 expectednumberOfCols = BUTTable::colnumber_coltime1 + 1;
            const basar::UInt32 expectednumberOfRows = 2;


            ResultsetRef resultSet1;
            ResultsetRef resultSet2;
            
            if (TestConnection::MSSQL_ODBC == eConnType)
            {
                //see http://msdn.microsoft.com/de-de/library/ms131686%28v=sql.90%29.aspx
                std::cout << "TEST(checkDiffDataTwoResults_checkDefaultData) skipped for MS SQL connections. " << std::endl;
                std::cout << "MS SQL does not handle multiple active record sets on a single connection by default" << std::endl;
                continue;  
            }
            else
            {
                resultSet1 = statement1.executeQuery(statementstring);
                resultSet2 = statement2.executeQuery(statementstring);            
            }

            CHECK(!resultSet1.isNull());
            CHECK_EQUAL(expectednumberOfCols, resultSet1.getSelectedCols());
            CHECK_EQUAL(expectednumberOfCols, resultSet2.getSelectedCols());

            // check on same data (name and number)
            {
                UInt32 cnt = 0;
                while (true == resultSet1.next())
                {
                    CHECK(resultSet2.next());
                    CHECK(!resultSet2.isAfterLast());
                    CHECK(!resultSet2.isBeforeFirst());
                    ++cnt;
                    // normal:
                    CHECK_EQUAL(resultSet1.getFetchedRows(), cnt);

                    // data on two sides:
                    CHECK_EQUAL(resultSet1.getInt32("colser") , resultSet2.getInt32(BUTTable::colnumber_colser));
                    CHECK_EQUAL(resultSet1.getInt32("colint") , resultSet2.getInt32(BUTTable::colnumber_colint));
                    CHECK_EQUAL(resultSet1.getInt16("colsmint") , resultSet2.getInt16(BUTTable::colnumber_colsmint));
                    CHECK_EQUAL(resultSet1.getFloat32("colsmfl") , resultSet2.getFloat32(BUTTable::colnumber_colsmfl));
                    CHECK_EQUAL(resultSet1.getFloat64("colfl") , resultSet2.getFloat64(BUTTable::colnumber_colfl));

                    // std::cout << resultSet1.getDecimal("coldec") << std::endl;

                    CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("coldec") , resultSet2.getDecimal(BUTTable::colnumber_coldec), toString);
                    CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("colmon") , resultSet2.getDecimal(BUTTable::colnumber_colmon), toString);

                    CHECK_EQUAL(resultSet1.getDecimal("coldec").toString() , resultSet2.getDecimal(BUTTable::colnumber_coldec).toString());
                    CHECK_EQUAL(resultSet1.getDecimal("colmon").toString() , resultSet2.getDecimal(BUTTable::colnumber_colmon).toString());

                    CHECK_EQUAL(resultSet1.getString("colch1"),resultSet2.getString(BUTTable::colnumber_colch1));
                    CHECK_EQUAL(resultSet1.getString("colch2"),resultSet2.getString(BUTTable::colnumber_colch2));
                    CHECK_EQUAL(resultSet1.getString("colvch1"),resultSet2.getString(BUTTable::colnumber_colvch1));
                    CHECK_EQUAL(resultSet1.getString("colvch2"),resultSet2.getString(BUTTable::colnumber_colvch2));
                    CHECK_EQUAL(resultSet1.getString("colvch3"),resultSet2.getString(BUTTable::colnumber_colvch3));
                    CHECK_EQUAL(resultSet1.getString("colvch4"),resultSet2.getString(BUTTable::colnumber_colvch4));
                    CHECK_EQUAL(resultSet1.getString("collvch"),resultSet2.getString(BUTTable::colnumber_collvch));

                    if (TestConnection::MSSQL_ODBC == eConnType)
                    {
                        CHECK_EQUAL(resultSet1.getDateTime("coldate" ).toStrDate(),
							        resultSet2.getDateTime(BUTTable::colnumber_coldate).toStrDate());
                        CHECK_EQUAL(resultSet1.getDateTime    ("coltime1").toStrDate(),
							        resultSet2.getDateTime(BUTTable::colnumber_coltime1).toStrDate());                    
					    CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coltime1") ,
							                resultSet2.getDateTime(BUTTable::colnumber_coltime1) , toStrDate , toStrTime);
                        
                        CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDateTime("coldate") ,resultSet2.getDateTime(BUTTable::colnumber_coldate) , toStrDate);							        
                    }
                    else
                    {
                        CHECK_EQUAL(resultSet1.getDate("coldate" ).toStrDate(),
							        resultSet2.getDate(BUTTable::colnumber_coldate).toStrDate());
                        CHECK_EQUAL(resultSet1.getTime    ("coltime1").toStrDate(),
							        resultSet2.getTime(BUTTable::colnumber_coltime1).toStrDate());
                        CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getTime("coltime1") ,
							                resultSet2.getTime(BUTTable::colnumber_coltime1) , toStrDate , toStrTime);
                        CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDate("coldate") ,resultSet2.getDate(BUTTable::colnumber_coldate) , toStrDate);							        
					}
							    							    
                    CHECK_EQUAL(resultSet1.getDateTime("coldt1"  ).toStrDate(),
							    resultSet2.getDateTime(BUTTable::colnumber_coldt1).toStrDate());
                    CHECK_EQUAL(resultSet1.getDateTime("coldt2"  ).toStrDate(),
							    resultSet2.getDateTime(BUTTable::colnumber_coldt2).toStrDate());



                    CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt1") ,resultSet2.getDateTime(BUTTable::colnumber_coldt1), toStrDate , toStrTime);
                    CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt2") ,resultSet2.getDateTime(BUTTable::colnumber_coldt2), toStrDate  , toStrTime);

                }
                CHECK_EQUAL(expectednumberOfRows, cnt );
            }

            if ( 
                  (TestConnection::ODBC == iter->first )    ||
                  (TestConnection::ODBC_BG == iter->first ) ||
                  (TestConnection::MSSQL_ODBC == iter->first) )
            {
                CHECK_THROW(resultSet1.reopen(),basar::InvalidStateException);
            }
            else
            {
                {
                    resultSet1.reopen();
                    if (resultSet1.next())
                    {
                        // exists:
                        CHECK(!resultSet1.isExist("huhu"));
                        CHECK(resultSet1.isExist("colser"));

                        // getIndex
                        CHECK_EQUAL(((UInt32)BUTTable::colnumber_colser), resultSet1.getIndex("colser"));
                        CHECK_EQUAL(((UInt32)BUTTable::colnumber_coltime1), resultSet1.getIndex("coltime1"));

                        // throw tests with large index and wrong index names:
                        CHECK_THROW(resultSet1.getIndex("huhu"),basar::OutOfRangeIndexException);
                        CHECK_THROW(resultSet1.getInt16("huhu"),basar::OutOfRangeIndexException);
                        CHECK_THROW(resultSet1.getInt16(23456),basar::OutOfRangeIndexException);

                        // throw tests with wrong data types:
                        CHECK_THROW(resultSet1.getString("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getDateTime("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getDate("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getDecimal("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getFloat64("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getFloat32("colser"),basar::InvalidParameterException);
                        CHECK_THROW(resultSet1.getInt16("colser"),basar::InvalidParameterException);

                    }
                    else
                    {
                        bool noreopen = false;
                        CHECK(noreopen);
                    }
                }

                // check to stored data
                {
                    resultSet1.reopen();
                    DefaultMasterBUTTable  & defaulttable = DefaultMasterBUTTable::getInstance();

                    UInt32 cnt = 0;
                    while(resultSet1.next())
                    {
                        cnt = resultSet1.getFetchedRows();

                        CHECK_EQUAL(defaulttable.getInt32(cnt, BUTTable::colnumber_colser  ), resultSet1.getInt32(BUTTable::colnumber_colser  ));
                        CHECK_EQUAL(defaulttable.getInt32(cnt,BUTTable::colnumber_colint  ), resultSet1.getInt32(BUTTable::colnumber_colint  ));
                        CHECK_EQUAL(defaulttable.getInt16(cnt,BUTTable::colnumber_colsmint), resultSet1.getInt16(BUTTable::colnumber_colsmint));

                        CHECK_EQUAL(defaulttable.getFloat64(cnt,BUTTable::colnumber_colfl   ), 
                                    resultSet1.getFloat64(BUTTable::colnumber_colfl   ));
                        CHECK_EQUAL(defaulttable.getFloat32(cnt,BUTTable::colnumber_colsmfl ), 
                                    resultSet1.getFloat32(BUTTable::colnumber_colsmfl ));
                        CHECK_CLOSE(defaulttable.getDecimal(cnt,BUTTable::colnumber_coldec  ).toFloat64(), 
                                    resultSet1.getDecimal(BUTTable::colnumber_coldec  ).toFloat64(), 
                                    Defines::csm_defaultDecimalCloseTolerance);
                        CHECK_CLOSE(defaulttable.getDecimal(cnt,BUTTable::colnumber_colmon  ).toFloat64(), 
                                    resultSet1.getDecimal(BUTTable::colnumber_colmon  ).toFloat64(), 
                                    Defines::csm_defaultDecimalCloseTolerance);

                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colch1  ), resultSet1.getString(BUTTable::colnumber_colch1  ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colch2  ), resultSet1.getString(BUTTable::colnumber_colch2  ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colvch1 ), resultSet1.getString(BUTTable::colnumber_colvch1 ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colvch2 ), resultSet1.getString(BUTTable::colnumber_colvch2 ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colvch3 ), resultSet1.getString(BUTTable::colnumber_colvch3 ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_colvch4 ), resultSet1.getString(BUTTable::colnumber_colvch4 ));
                        CHECK_EQUAL(defaulttable.getString(cnt,BUTTable::colnumber_collvch ), resultSet1.getString(BUTTable::colnumber_collvch ));

                        CHECK_EQUAL_SHOW_FUNCT(defaulttable.getDate(cnt,BUTTable::colnumber_coldate ), resultSet1.getDate(BUTTable::colnumber_coldate ), toStrDate);
                        CHECK_EQUAL_SHOW_FUNCT(defaulttable.getDate(cnt,BUTTable::colnumber_coldate  ), resultSet1.getDate(BUTTable::colnumber_coldate  ), toStrDate );

                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt1  ).getDate(), resultSet1.getDateTime(BUTTable::colnumber_coldt1  ).getDate());
                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt1  ).getTime(), resultSet1.getDateTime(BUTTable::colnumber_coldt1  ).getTime());

                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt2  ).getDate(), resultSet1.getDateTime(BUTTable::colnumber_coldt2  ).getDate());
                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt2 ).getTime(), resultSet1.getDateTime(BUTTable::colnumber_coldt2  ).getTime());

                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coltime1  ).getDate(), resultSet1.getDateTime(BUTTable::colnumber_coltime1  ).getDate());
                        CHECK_EQUAL(defaulttable.getDateTime(cnt,BUTTable::colnumber_coltime1 ).getTime(), resultSet1.getDateTime(BUTTable::colnumber_coltime1  ).getTime());

                        //CHECK_EQUAL_SHOW_FUNCT2(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt1  ), resultSet1.getDateTime(BUTTable::colnumber_coldt1  ), toStrDate , toStrTime);
                        //CHECK_EQUAL_SHOW_FUNCT2(defaulttable.getDateTime(cnt,BUTTable::colnumber_coldt2  ), resultSet1.getDateTime(BUTTable::colnumber_coldt2  ), toStrDate, toStrTime);
                        //CHECK_EQUAL_SHOW_FUNCT2(defaulttable.getDateTime(cnt,BUTTable::colnumber_coltime1), resultSet1.getDateTime(BUTTable::colnumber_coltime1), toStrDate, toStrTime);
                    }
                }


                resultSet1.close();
                resultSet2.close();
                statement1.close();
                statement2.close();	
            }
        }
        catch (basar::Exception & ex)
        {
            std::cout << "Exception at Connection = " 
                << iter->first << std::endl 
                << ex.what().c_str() << std::endl;
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
TEST(checkDiffDataTwoConnRef_Results_same_rawfetching_infx)
{
    basar::db::sql::ConnectionRef connRef1 = TestConnection::createTestConnection(TestConnection::INFX);
    basar::db::sql::ConnectionRef connRef2 = TestConnection::createTestConnection(TestConnection::INFX);
    {
        try
        {	
            bool rawfetching = true;
            connRef1.setRawFetching(rawfetching);
            connRef2.setRawFetching(rawfetching);
            {
                StatementRef statement1 = connRef1.createStatement();
                StatementRef statement2 = connRef2.createStatement();

                // statements :
                ConstString statementstring = "select first 2 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon ,colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
                const basar::UInt32 expectednumberOfCols = BUTTable::colnumber_coltime1 + 1;
                const basar::UInt32 expectednumberOfRows = 2;

                ResultsetRef resultSet1  = statement1.executeQuery(statementstring);
                ResultsetRef resultSet2  = statement2.executeQuery(statementstring);

                CHECK(!resultSet1.isNull());
                CHECK_EQUAL(expectednumberOfCols, resultSet1.getSelectedCols());
                CHECK_EQUAL(expectednumberOfCols, resultSet2.getSelectedCols());

                // check on same data (name and number)
                {

                    UInt32 cnt = 0;
                    while (true == resultSet1.next())
                    {
                        CHECK(resultSet2.next());
                        CHECK(!resultSet2.isAfterLast());
                        CHECK(!resultSet2.isBeforeFirst());
                        ++cnt;
                        // normal:
                        CHECK_EQUAL(resultSet1.getFetchedRows(), cnt);

                        // data on two sides:
                        CHECK_EQUAL(resultSet1.getInt32("colser") , resultSet2.getInt32(BUTTable::colnumber_colser));
                        CHECK_EQUAL(resultSet1.getInt32("colint") , resultSet2.getInt32(BUTTable::colnumber_colint));
                        CHECK_EQUAL(resultSet1.getInt16("colsmint") , resultSet2.getInt16(BUTTable::colnumber_colsmint));
                        CHECK_EQUAL(resultSet1.getFloat32("colsmfl") , resultSet2.getFloat32(BUTTable::colnumber_colsmfl));
                        CHECK_EQUAL(resultSet1.getFloat64("colfl") , resultSet2.getFloat64(BUTTable::colnumber_colfl));

                        CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("coldec") , resultSet2.getDecimal(BUTTable::colnumber_coldec), toString);
                        CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("colmon") , resultSet2.getDecimal(BUTTable::colnumber_colmon), toString);

                        CHECK_EQUAL(resultSet1.getDecimal("coldec").toString() , resultSet2.getDecimal(BUTTable::colnumber_coldec).toString());
                        CHECK_EQUAL(resultSet1.getDecimal("colmon").toString() , resultSet2.getDecimal(BUTTable::colnumber_colmon).toString());

                        // strings
                        CHECK_EQUAL(resultSet1.getString("colch1"),resultSet2.getString(BUTTable::colnumber_colch1));
                        CHECK_EQUAL(resultSet1.getString("colch2"),resultSet2.getString(BUTTable::colnumber_colch2));

                        std::string helperstring = resultSet1.getString("colch1");
                        CHECK_EQUAL(BUTTable::csm_collengthColch1, ((UInt32) helperstring.size()) );

                        helperstring = resultSet1.getString("colch2");
                        CHECK_EQUAL(BUTTable::csm_collengthColch2,  ((UInt32) helperstring.size()) );

                        CHECK_EQUAL(resultSet1.getString("colvch1"),resultSet2.getString(BUTTable::colnumber_colvch1));
                        CHECK_EQUAL(resultSet1.getString("colvch2"),resultSet2.getString(BUTTable::colnumber_colvch2));
                        CHECK_EQUAL(resultSet1.getString("colvch3"),resultSet2.getString(BUTTable::colnumber_colvch3));
                        CHECK_EQUAL(resultSet1.getString("colvch4"),resultSet2.getString(BUTTable::colnumber_colvch4));
                        CHECK_EQUAL(resultSet1.getString("collvch"),resultSet2.getString(BUTTable::colnumber_collvch));

                        // Date:
                        CHECK_EQUAL(resultSet1.getDate("coldate").toStrDate(),resultSet2.getDate(BUTTable::colnumber_coldate).toStrDate());
                        CHECK_EQUAL(resultSet1.getDateTime("coldt1").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coldt1).toStrDate());
                        CHECK_EQUAL(resultSet1.getDateTime("coldt2").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coldt2).toStrDate());
                        CHECK_EQUAL(resultSet1.getDateTime("coltime1").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coltime1).toStrDate());

                        CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDate("coldate") ,resultSet2.getDate(BUTTable::colnumber_coldate) , toStrDate);
                        CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt1") ,resultSet2.getDateTime(BUTTable::colnumber_coldt1), toStrDate , toStrTime);
                        CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt2") ,resultSet2.getDateTime(BUTTable::colnumber_coldt2), toStrDate  , toStrTime);
                        CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coltime1") ,resultSet2.getDateTime(BUTTable::colnumber_coltime1) , toStrDate , toStrTime);

                    }
                    CHECK_EQUAL(expectednumberOfRows, cnt );
                }
                resultSet1.close();
                resultSet2.close();
                statement1.close();
                statement2.close();	
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
        connRef1.close();
        connRef2.close();

    }
}

//------------------------------------------------------------------------------
TEST(checkDiffDataTwoConnRef_Results_rawfetching_changed_infx)
{
    basar::db::sql::ConnectionRef connRef1 = TestConnection::createTestConnection(TestConnection::INFX);
    basar::db::sql::ConnectionRef connRef2 = TestConnection::createTestConnection(TestConnection::INFX);
    try
    {	
        // setRaw Fetching
        connRef1.setRawFetching(true);
        connRef2.setRawFetching(false);
        StatementRef statement1 = connRef1.createStatement();
        StatementRef statement2 = connRef2.createStatement();

        // statements :
        ConstString statementstring = "select first 2 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon ,colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
        const basar::UInt32 expectednumberOfCols = BUTTable::colnumber_coltime1 + 1;
        const basar::UInt32 expectednumberOfRows = 2;

        ResultsetRef resultSet1  = statement1.executeQuery(statementstring);
        ResultsetRef resultSet2  = statement2.executeQuery(statementstring);

        CHECK(!resultSet1.isNull());
        CHECK_EQUAL(expectednumberOfCols, resultSet1.getSelectedCols());
        CHECK_EQUAL(expectednumberOfCols, resultSet2.getSelectedCols());

        // check on same data (name and number)
        {
            UInt32 cnt = 0;
            while (true == resultSet1.next())
            {
                CHECK(resultSet2.next());
                CHECK(!resultSet2.isAfterLast());
                CHECK(!resultSet2.isBeforeFirst());
                ++cnt;
                // normal:
                CHECK_EQUAL(resultSet1.getFetchedRows(), cnt);

                // data on two sides:
                CHECK_EQUAL(resultSet1.getInt32("colser") , resultSet2.getInt32(BUTTable::colnumber_colser));
                CHECK_EQUAL(resultSet1.getInt32("colint") , resultSet2.getInt32(BUTTable::colnumber_colint));
                CHECK_EQUAL(resultSet1.getInt16("colsmint") , resultSet2.getInt16(BUTTable::colnumber_colsmint));
                CHECK_EQUAL(resultSet1.getFloat32("colsmfl") , resultSet2.getFloat32(BUTTable::colnumber_colsmfl));
                CHECK_EQUAL(resultSet1.getFloat64("colfl") , resultSet2.getFloat64(BUTTable::colnumber_colfl));

                CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("coldec") , resultSet2.getDecimal(BUTTable::colnumber_coldec), toString);
                CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDecimal("colmon") , resultSet2.getDecimal(BUTTable::colnumber_colmon), toString);

                CHECK_EQUAL(resultSet1.getDecimal("coldec").toString() , resultSet2.getDecimal(BUTTable::colnumber_coldec).toString());
                CHECK_EQUAL(resultSet1.getDecimal("colmon").toString() , resultSet2.getDecimal(BUTTable::colnumber_colmon).toString());

                CHECK_EQUAL(resultSet1.getDate("coldate").toStrDate(),resultSet2.getDate(BUTTable::colnumber_coldate).toStrDate());
                CHECK_EQUAL(resultSet1.getDateTime("coldt1").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coldt1).toStrDate());
                CHECK_EQUAL(resultSet1.getDateTime("coldt2").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coldt2).toStrDate());
                CHECK_EQUAL(resultSet1.getDateTime("coltime1").toStrDate(),resultSet2.getDateTime(BUTTable::colnumber_coltime1).toStrDate());

                CHECK_EQUAL_SHOW_FUNCT(resultSet1.getDate("coldate") ,resultSet2.getDate(BUTTable::colnumber_coldate) , toStrDate);
                CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt1") ,resultSet2.getDateTime(BUTTable::colnumber_coldt1), toStrDate , toStrTime);
                CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coldt2") ,resultSet2.getDateTime(BUTTable::colnumber_coldt2), toStrDate  , toStrTime);
                CHECK_EQUAL_SHOW_FUNCT2(resultSet1.getDateTime("coltime1") ,resultSet2.getDateTime(BUTTable::colnumber_coltime1) , toStrDate , toStrTime);

                CHECK_EQUAL(resultSet1.getString("colvch1"),resultSet2.getString(BUTTable::colnumber_colvch1));
                CHECK_EQUAL(resultSet1.getString("colvch2"),resultSet2.getString(BUTTable::colnumber_colvch2));
                CHECK_EQUAL(resultSet1.getString("colvch3"),resultSet2.getString(BUTTable::colnumber_colvch3));
                CHECK_EQUAL(resultSet1.getString("colvch4"),resultSet2.getString(BUTTable::colnumber_colvch4));
                CHECK_EQUAL(resultSet1.getString("collvch"),resultSet2.getString(BUTTable::colnumber_collvch));

                // strings may be differnet
                std::string helperstring1 = resultSet1.getString("colch1");
                std::string helperstring2 = resultSet2.getString("colch1");

                CHECK_EQUAL(BUTTable::csm_collengthColch1, ((UInt32) helperstring1.size()) );
                CHECK(BUTTable::csm_collengthColch1 >= ((UInt32) helperstring2.size()) );
                CHECK(helperstring1.size() >= helperstring2.size() );

                helperstring1 = resultSet1.getString("colch2");
                helperstring2 = resultSet2.getString("colch2");

                CHECK_EQUAL(BUTTable::csm_collengthColch2, ((UInt32) helperstring1.size()) );
                CHECK(BUTTable::csm_collengthColch2 >= ((UInt32) helperstring2.size()) );
                CHECK(helperstring1.size() >= helperstring2.size() );
            }
            CHECK_EQUAL(expectednumberOfRows, cnt );
        }
        resultSet1.close();
        resultSet2.close();
        statement1.close();
        statement2.close();	

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
    connRef1.close();
    connRef2.close();

}

// ------------------------------------------------------------------------------------------------------------------

#ifdef HAS_ODBC

TEST_FIXTURE(OdbcFix, checkODBCResults_INFXResults)
{
    ConstString sql = 
		"select first 2 "
			"colser,  colint, colsmint, colfl  , colsmfl, coldec,  colmon, "
			"colch1,  colch2, colvch1 , colvch2, colvch3, colvch4, collvch, "
			"coldate, coldt1, coldt2  , coltime1 "
		"from but_master";

    try
    {	
		ConnectionRef conn1 = TestConnection::createTestConnection(TestConnection::ODBC);
		ConnectionRef conn2 = TestConnection::createTestConnection(TestConnection::INFX);

		for (Int32 i = 0; 
			 i < 2;
			 ++i,
			 conn1 = TestConnection::createTestConnection(TestConnection::ODBC_BG),
			 conn2 = TestConnection::createTestConnection(TestConnection::INFX_BG))
		{
            StatementRef stmt1 = conn1.createStatement();
            StatementRef stmt2 = conn2.createStatement();

            // statements :
            const UInt32 expNoCols = BUTTable::colnumber_coltime1 + 1;
            const UInt32 expNoRows = 2;

            ResultsetRef resSet1  = stmt1.executeQuery(sql);
            ResultsetRef resSet2  = stmt2.executeQuery(sql);

            CHECK(!resSet1.isNull());
            CHECK(!resSet2.isNull());

            CHECK_EQUAL(expNoCols, resSet1.getSelectedCols());
            CHECK_EQUAL(expNoCols, resSet2.getSelectedCols());

            // check on same data (name and number)
            UInt32 cnt = 0;

            while (resSet1.next())
            {
                CHECK(resSet2.next());
                CHECK(!resSet2.isAfterLast());
                CHECK(!resSet2.isBeforeFirst());
                ++cnt;

				CHECK_EQUAL(resSet1.getFetchedRows(), cnt);

                // data on two sides:
                CHECK_EQUAL(resSet1.getInt32("colser") , 
					        resSet2.getInt32(BUTTable::colnumber_colser));
                CHECK_EQUAL(resSet1.getInt32("colint") , 
					        resSet2.getInt32(BUTTable::colnumber_colint));
                CHECK_EQUAL(resSet1.getInt16("colsmint") , 
					        resSet2.getInt16(BUTTable::colnumber_colsmint));
                CHECK_CLOSE(resSet1.getFloat32("colsmfl") , 
					        resSet2.getFloat32(BUTTable::colnumber_colsmfl), 
							0.001);
                CHECK_EQUAL(resSet1.getFloat64("colfl") , 
					        resSet2.getFloat64(BUTTable::colnumber_colfl));

                CHECK_CLOSE(resSet1.getDecimal("coldec")                  .toFloat64() , 
					        resSet2.getDecimal(BUTTable::colnumber_coldec).toFloat64(), 
							Defines::csm_defaultDecimalCloseTolerance);
                CHECK_CLOSE(resSet1.getDecimal("colmon")                  .toFloat64() , 
					        resSet2.getDecimal(BUTTable::colnumber_colmon).toFloat64(), 
							Defines::csm_defaultDecimalCloseTolerance);

                CHECK_EQUAL(resSet1.getString("colch1"),
					        resSet2.getString(BUTTable::colnumber_colch1));
                CHECK_EQUAL(resSet1.getString("colch2"),
					        resSet2.getString(BUTTable::colnumber_colch2));
                CHECK_EQUAL(resSet1.getString("colvch1"),
					        resSet2.getString(BUTTable::colnumber_colvch1));
                CHECK_EQUAL(resSet1.getString("colvch2"),
					        resSet2.getString(BUTTable::colnumber_colvch2));
                CHECK_EQUAL(resSet1.getString("colvch3"),
					        resSet2.getString(BUTTable::colnumber_colvch3));
                CHECK_EQUAL(resSet1.getString("colvch4"),
					        resSet2.getString(BUTTable::colnumber_colvch4));
                CHECK_EQUAL(resSet1.getString("collvch"),
					        resSet2.getString(BUTTable::colnumber_collvch));

                CHECK_EQUAL(resSet1.getDate("coldate")                  .toStrDate(), 
					        resSet2.getDate(BUTTable::colnumber_coldate).toStrDate());
                CHECK_EQUAL(resSet1.getDateTime("coldt1")                  .toStrDate(),
					        resSet2.getDateTime(BUTTable::colnumber_coldt1).toStrDate());
                CHECK_EQUAL(resSet1.getDateTime("coldt2")                  .toStrDate(),
					        resSet2.getDateTime(BUTTable::colnumber_coldt2).toStrDate());
                CHECK_EQUAL(resSet1.getTime("coltime1")                  .toStrDate(),
					        resSet2.getTime(BUTTable::colnumber_coltime1).toStrDate());
                CHECK_EQUAL(resSet1.getDateTime("coldt1")                  .toStrTime(),
					        resSet2.getDateTime(BUTTable::colnumber_coldt1).toStrTime());
                CHECK_EQUAL(resSet1.getDateTime("coldt2")                  .toStrTime(),
					        resSet2.getDateTime(BUTTable::colnumber_coldt2).toStrTime());
                CHECK_EQUAL(resSet1.getTime("coltime1")                  .toStrTime(),
					        resSet2.getTime(BUTTable::colnumber_coltime1).toStrTime());


                CHECK_EQUAL_SHOW_FUNCT(resSet1.getDate("coldate") ,
					                   resSet2.getDate(BUTTable::colnumber_coldate) , 
									   toStrDate);
            }

			CHECK_EQUAL(expNoRows, cnt );

            resSet1.close();
            resSet2.close();
            stmt1  .close();
            stmt2  .close();
			conn1  .close();
			conn2  .close();
        }	// end for
	}	// try
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

// ------------------------------------------------------------------------------------------------------------------
TEST(holdCursor)
{
    TestConnection::TestConnectionMap  connRefs = TestConnection::createDefaultTestConnections();
    TestConnection::TestConnectionMap ::const_iterator iter;
    
    TestConnection::TestConnectionType eConnType = TestConnection::INFX;
    
    for (iter = connRefs.begin(); iter != connRefs.end(); ++iter)
    {	
        eConnType = iter->first;
        
        if (TestConnection::MSSQL_ODBC == eConnType)
        {
            continue;
        }
        
        basar::db::sql::ConnectionRef connRef = iter->second;
        try
        {	
            StatementRef statement1 = connRef.createStatement();
            ConstString selectstmt = "select colser, colint, colsmint from but_master ";

            connRef.begin();

            ResultsetRef resultSet1 = statement1.executeQuery(selectstmt );
            CHECK(!resultSet1.isNull());

            connRef.commit();

			CHECK_THROW(resultSet1.next(), basar::db::sql::DbException);

            if ( 
                  (TestConnection::ODBC != iter->first)       &&
                  (TestConnection::ODBC_BG != iter->first)     )
            {
                connRef.begin();
                resultSet1 = statement1.executeQuery(selectstmt, basar::db::sql::CURS_HOLD_OVER_COMMIT );
                CHECK(!resultSet1.isNull());
                connRef.commit();

                CHECK(resultSet1.next());
            }

            resultSet1.close();
            statement1.close();
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

// ------------------------------------------------------------------------------------------------------------------
TEST(execCurrentRow_update)
{
	TestConnection::TestConnectionMap connRefs = TestConnection::createDefaultTestConnections();

    TestConnection::TestConnectionType eConnType = TestConnection::INFX;

	for (TestConnection::TestConnectionMap::const_iterator iter = connRefs.begin(); 
		 iter != connRefs.end(); 
		 ++iter)
    {	
        
        eConnType = iter->first;
        
        if (TestConnection::MSSQL_ODBC == eConnType)
        {
            continue;
        }        
        
        ConnectionRef conn = iter->second;

        try
        {	
            StatementRef stmt = conn.createStatement();
            ConstString  sel  = "select colser, colint, colsmint "
				                "from but_detail "
							    "where colint = 8 for update";
            ConstString  upd  = "update but_detail set colsmint = 1";

            conn.begin();

            ResultsetRef res = stmt.executeQuery(sel);

            if ( 
                  (TestConnection::ODBC == iter->first)       ||
                  (TestConnection::ODBC_BG == iter->first) )
            {
                if (res.next())
                    CHECK_THROW(res.execCurrentRow(upd), 
					            basar::InvalidStateException);
            }
            else
            {
				Int32 rows;

                while (res.next())
				{
                    rows = res.execCurrentRow(upd);	

					CHECK_EQUAL(1, rows);
				}
            }

			conn.rollback();

            res.close();
            stmt.close();
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

// ------------------------------------------------------------------------------------------------------------------
}	// SUITE

#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

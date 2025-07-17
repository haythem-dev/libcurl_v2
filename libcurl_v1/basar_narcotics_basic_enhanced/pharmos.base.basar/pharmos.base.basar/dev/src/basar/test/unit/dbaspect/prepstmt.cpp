//----------------------------------------------------------------------------
/*
 *  \author Michael Eichenlaub
 *  \date   01.06.2011
*/
//----------------------------------------------------------------------------

//libraries
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "libbasardbsql.h"
#include "libbasarproperty.h"
#include "libbasardbaspect_stringbuildertypeindexes.h"

//common ut helpers
#include "testconnection.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessmethodnames.h"
#include "accessornames.h"

//specific helpers
#include "testpreparedsqlbuilder.h"

//------------------------------------------------------------------------------
using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;

//------------------------------------------------------------------------------
SUITE(PrepStmt)
{

//------------------------------------------------------------------------------

    TEST(BasicQueryTest)
    {
        //tests basic functionality of prepared query builder
        
        SQLPreparedQuery_BasicTest myQueryBuilder;
        
        CHECK_EQUAL(true, myQueryBuilder.isPrepareable());
        CHECK_EQUAL(true, myQueryBuilder.isQuery().first);
        CHECK_EQUAL(basar::db::aspect::MULTI_QUERY, myQueryBuilder.isQuery().second);
    }
    
//------------------------------------------------------------------------------    
    
    TEST(BasicWriteTest)
    {
        //tests basic functionality of prepared write builder
        SQLPreparedWrite_BasicTest myWriteBuilder;
        
        CHECK_EQUAL(true, myWriteBuilder.isPrepareable());
        CHECK_EQUAL(false, myWriteBuilder.isQuery().first);
        CHECK_EQUAL(basar::db::aspect::MULTI_QUERY, myWriteBuilder.isQuery().second);
    }

//------------------------------------------------------------------------------
    
    TEST_FIXTURE(FixtureQuerySqlResolvTest, QuerySqlResolvTest)
    {
        const basar::VarString expectedSql = "select * from bogustable where indexnumber = ?";
 
        try
        {           
            SQLPreparedQuery_BasicTest myQueryBuilder;
            
            bool buildSuccess = myQueryBuilder.build(yit);
            
            CHECK_EQUAL(true, buildSuccess);
            
            CHECK_EQUAL(expectedSql, myQueryBuilder.getSQLString());
            
            basar::db::aspect::IndexTypeCollection myColl = myQueryBuilder.getPreparedParameterTypePositionCollection();
            
            CHECK_EQUAL(0 , myColl.begin()->m_IndexYit);
            CHECK_EQUAL(basar::INT32, myColl.begin()->m_Type.getType());
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }
    }

//------------------------------------------------------------------------------

    TEST_FIXTURE(FixtureQuerySqlResolvTestWrong, WrongQuerySyntax)
    {
        try
        {           
        
            CHECK_THROW(ai.execute(ACCESSMETHOD_PreparedQueryWrong), basar::db::sql::DbException);
                        
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }        
    }

//------------------------------------------------------------------------------  

    TEST_FIXTURE(FixturePreparedNoVariable, QueryNoVariable)
    {
        try
        {           
        
            CHECK_THROW(ai.execute(ACCESSMETHOD_PreparedQueryNoVariable), basar::db::sql::DbException);
                        
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }         
    }

//------------------------------------------------------------------------------    
    
    TEST_FIXTURE(FixtureWriteSqlResolvTest, WriteSqlResolvTest)
    {
        const basar::VarString expectedSql = "update bogustable set somevalue = ? where indexnumber = ?";
 
        basar::Int32 expectedIndexPrepStmt = 0;
        basar::Int32 expectedIndexYit = 1;
 
        try
        {           
            SQLPreparedWrite_BasicTest myWriteBuilder;
            
            bool buildSuccess = myWriteBuilder.build(yit);
            
            CHECK_EQUAL(true, buildSuccess);
            
            CHECK_EQUAL(expectedSql, myWriteBuilder.getSQLString());
            
            const basar::db::aspect::IndexTypeCollection myColl = myWriteBuilder.getPreparedParameterTypePositionCollection();
            
            basar::db::aspect::IndexTypeCollection::const_iterator itEnd = myColl.end();
            
            for(basar::db::aspect::IndexTypeCollection::const_iterator it = myColl.begin(); itEnd != it; ++it)
            {
                CHECK_EQUAL(expectedIndexYit , it->m_IndexYit);
                CHECK_EQUAL(expectedIndexPrepStmt, it->m_IndexPreparedStatement);
                CHECK_EQUAL(basar::INT32, it->m_Type.getType());
                
                ++expectedIndexPrepStmt;
                --expectedIndexYit;
            }
            
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }
    }

//------------------------------------------------------------------------------  

    TEST_FIXTURE(FixturePreparedButtableRead, PreparedButtableRead)
    {
        
        const basar::Int32 expectedResults = 10;
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedQueryButMasterTableReader_All);
            
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                ++results;
                ++yit;
            }
            
            CHECK_EQUAL(expectedResults, results);
            
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }        
        
        
    }
    
//------------------------------------------------------------------------------  

    TEST_FIXTURE(FixturePreparedButtableRead, PreparedButtableReadSuccessive)
    {
        
        const basar::Int32 expectedResults = 10;
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedQueryButMasterTableReader_All);
            
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                ++results;
                ++yit;
            }
            
            CHECK_EQUAL(expectedResults, results);
            
            //second run
            results = 0;
            
            res = ai.execute(ACCESSMETHOD_PreparedQueryButMasterTableReader_All);
            
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            basar::db::aspect::AccessorPropertyTable_YIterator yitSucc = ai.getPropertyTable().begin();           
            
            while(ai.getPropertyTable().end() !=  yitSucc)
            {
                ++results;
                ++yitSucc;
            }
            
            CHECK_EQUAL(expectedResults, results);
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }        
    }    
    
//------------------------------------------------------------------------------ 
    
    TEST_FIXTURE(FixturePreparedButtableReadWithVariable, PreparedButtableReadWithVariable)
    {
        const basar::Int32 expectedResults = 1;
        const basar::Int32 expectedColSerValue = 2;
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedQueryButMasterTableReader_All_WithVaraible, yitSearch);
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            CHECK_EQUAL(false, ai.getPropertyTable().empty());
            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                CHECK_EQUAL(expectedColSerValue, yit.getInt32("colser"));
                ++results;
                ++yit;
            }
            
            CHECK_EQUAL(expectedResults, results);                    
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }         
    }
//------------------------------------------------------------------------------ 
    TEST_FIXTURE(FixturePreparedSingleQueryWithVariable, PreparedSingleQueryWithVariable)
    {
        const basar::Int32 expectedResults = 1;
        
        const basar::Int32 expectedColser = 3;
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedSingleQueryWithVariable, yitSearch);
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            CHECK_EQUAL(false, ai.getPropertyTable().empty());            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            CHECK_EQUAL(false, yit.isNull());
            
            CHECK_EQUAL(expectedColser, yit.getInt32("colser"));
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                ++yit;
                ++results;
            }
            
            CHECK_EQUAL(expectedResults, results);
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }                  
    }


//------------------------------------------------------------------------------ 
    TEST_FIXTURE(FixturePreparedSingleQueryWithVariable, PreparedAggregateQueryWithVariable)
    {
        const basar::Int32 expectedResults = 1;
        
        const basar::Decimal expectedCount(8);
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedAggregateQueryWithVariable, yitSearch);
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            CHECK_EQUAL(false, ai.getPropertyTable().empty());            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            CHECK_EQUAL(false, yit.isNull());
            
            CHECK_EQUAL(expectedCount, yit.getDecimal("coldec"));
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                ++yit;
                ++results;
            }
            
            CHECK_EQUAL(expectedResults, results);
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }                  
    }

//------------------------------------------------------------------------------ 
    
    TEST_FIXTURE(FixturePreparedRoundtrip, PreparedRoundtrip)
    {
        //read result with prepared statement
        //insert result's values in database
        //get last inserted id and query anew
        //compair values with initial read
        const basar::Int32 expectedResults = 1;
        const basar::Int32 expectedColSerValue = 2;
        const basar::Float64 TOL = 0.00001;
        
        try
        {
            basar::db::aspect::ExecuteResultInfo res = ai.execute(ACCESSMETHOD_PreparedQueryButMasterTableReader_All_WithVaraible, yitSearch);
            CHECK_EQUAL(false, res.hasError());
            CHECK_EQUAL(basar::db::aspect::SUCCESS, res.getError());
            
            CHECK_EQUAL(false, ai.getPropertyTable().empty());
            
            basar::db::aspect::AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();
            
            basar::Int32 results = 0;
            
            while(ai.getPropertyTable().end() !=  yit)
            {
                CHECK_EQUAL(expectedColSerValue, yit.getInt32("colser"));
            
                //testing copy semantics, yitInsert should copy values from yit    
                yitInsert.setInt32("colint",    yit.getInt32("colint"));                
                yitInsert.setInt16("colsmint",  yit.getInt16("colsmint"));
                
                yitInsert.setFloat64("colfl",   yit.getFloat64("colfl"));
                yitInsert.setFloat32("colsmfl", yit.getFloat32("colsmfl"));
                yitInsert.setDecimal("coldec",  yit.getDecimal("coldec"));
                yitInsert.setDecimal("colmon",  yit.getDecimal("colmon"));
                
                yitInsert.setString("colch1",   yit.getString("colch1"));
                yitInsert.setString("colch2",   yit.getString("colch2"));
                yitInsert.setString("colvch1",  yit.getString("colvch1"));
                yitInsert.setString("colvch2",  yit.getString("colvch2"));
                yitInsert.setString("colvch3",  yit.getString("colvch3"));
                yitInsert.setString("colvch4",  yit.getString("colvch4"));
                yitInsert.setString("collvch",   yit.getString("collvch"));
                
                yitInsert.setDate("coldate",    yit.getDate("coldate"));
                
                yitInsert.setDateTime("coldt1", yit.getDateTime("coldt1"));
                yitInsert.setDateTime("coldt2", yit.getDateTime("coldt2"));
                
                yitInsert.setTime("coltime1",   yit.getTime("coltime1"));
                
                //todo: timespan
                
                ++results;
                ++yit;
            }
            
            ai_write.execute(ACCESSMETHOD_PreparedWriteButVarTableWriter_All_WithVariables, yitInsert);            
            
            basar::Int64 lastInsSerial = ai_write.getLastInsertedSerial(0);
            
            
            yitSearch = ai_read_written.getPropertyTable().insert(basar::FOR_CLEAN);
            
            yitSearch.setInt32("colser", static_cast<basar::Int32>(lastInsSerial));
            
            ai_read_written.execute(ACCESSMETHOD_PreparedWriteButVarTableReader_All_WithVariables, yitSearch);
            
            basar::db::aspect::AccessorPropertyTable_YIterator yitRead = ai_read_written.getPropertyTable().begin();
            
            yit = ai.getPropertyTable().begin();
            
            //todo: check all
            CHECK_EQUAL(yit.getInt32("colint"), yitRead.getInt32("colint"));
            CHECK_EQUAL(yit.getInt16("colsmint"), yitRead.getInt16("colsmint"));
            
            CHECK_CLOSE(yit.getFloat64("colfl"), yitRead.getFloat64("colfl"), TOL);
            CHECK_CLOSE(yit.getFloat32("colsmfl"), yitRead.getFloat32("colsmfl"), TOL);
            
            CHECK_EQUAL(yit.getDecimal("coldec"), yitRead.getDecimal("coldec"));
            CHECK_EQUAL(yit.getDecimal("colmon"), yitRead.getDecimal("colmon"));
                
            CHECK_EQUAL(yit.getString("colch1"), yitRead.getString("colch1"));
            CHECK_EQUAL(yit.getString("colch2"), yitRead.getString("colch2"));
            CHECK_EQUAL(yit.getString("colvch1"), yitRead.getString("colvch1"));
            CHECK_EQUAL(yit.getString("colvch2"), yitRead.getString("colvch2"));
            CHECK_EQUAL(yit.getString("colvch3"), yitRead.getString("colvch3"));
            CHECK_EQUAL(yit.getString("colvch4"), yitRead.getString("colvch4"));
            CHECK_EQUAL(yit.getString("collvch"), yitRead.getString("collvch"));
            CHECK_EQUAL(yit.getDateTime("coldt1"), yitRead.getDateTime("coldt1"));
            CHECK_EQUAL(yit.getDateTime("coldt2"), yitRead.getDateTime("coldt2"));
            CHECK_EQUAL(yit.getTime("coltime1"), yitRead.getTime("coltime1"));
            
            CHECK_EQUAL(expectedResults, results);                    
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }         
    }
//------------------------------------------------------------------------------
    TEST_FIXTURE(FixturePreparedSingleQueryWithVariable, PreparedAggregateQueryWithVariableAsAggregate)
    {        
        const basar::Decimal expectedCount(8);
        
        try
        {
            std::pair<bool, basar::Decimal> res = ai.executeAggregate(ACCESSMETHOD_PreparedAggregateQueryWithVariable, yitSearch);
            
            CHECK_EQUAL(true, res.first);
           
            
            CHECK_EQUAL(expectedCount, res.second);
            
            
        }
        catch(const basar::Exception& ex)
        {
            bool exceptionOccurred = false;
            
            std::cout << "basar exception occurred: " << ex.what() << std::endl;
            
            CHECK(exceptionOccurred);
        }
        catch(const std::exception&)
        {
            bool exceptionOccurred = false;
            
            std::cout << "std exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);           
        }
        catch(...)
        {
            bool exceptionOccurred = false;
            
            std::cout << "unknown exception occurred" << std::endl;
            
            CHECK(exceptionOccurred);
        }                  
    }

//------------------------------------------------------------------------------
}	// SUITE

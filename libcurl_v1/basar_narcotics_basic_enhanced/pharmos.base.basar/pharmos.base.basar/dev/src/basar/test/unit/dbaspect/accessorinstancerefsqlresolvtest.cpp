/** $Id$                                          
*                                                  
* @file AccessorDefinitionList.cpp                                                                      
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testaccessorexecutelistener.h"
#include "testnames.h"

using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent;
using basar::cmnutil::ParameterList;

#define TESTDBACPECT_ACCESSORINSTANCEREF_SQLRESOLVTEST
#ifdef TESTDBACPECT_ACCESSORINSTANCEREF_SQLRESOLVTEST

//------------------------------------------------------------------------------
SUITE(DBAcpect_AccessorInstanceRef_SqlresolvTest)
{
    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_WhereSelect_AccessorInstanceRef_nodate",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_nodate, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai3 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_WhereSelect_AccessorInstanceRef_nofloat",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_nofloat, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            TestAccessorExecuteListenerHoldLastEvent listener1;
            ai1.setAccessorExecuteListener (listener1);
            ai2.setAccessorExecuteListener (listener1);
            ai3.setAccessorExecuteListener (listener1);

            {
                ExecuteResultInfo executeresultinfo = 
                    ai1.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nodate, iter1);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                executeresultinfo = ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, iter1);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                AccessorPropertyTableRef  aptr3 = ai3.getPropertyTable();
                AccessorPropertyTable_YIterator iter3 = aptr3.begin();
                CHECK(iter1.compareSetValues(iter3).first);

                // more than one iter:
                {
                    AccessorPropertyTable_YIterator _iter1 = aptr1.begin();
                    AccessorPropertyTable_YIterator _iter1e = aptr1.end();
                    executeresultinfo = ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, _iter1, _iter1);

                    CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                    executeresultinfo = ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, _iter1, _iter1e);     
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                    AccessorPropertyTableRef  aptr3e = ai3.getPropertyTable();          
                    CHECK_EQUAL(static_cast<int>(aptr1.size()), static_cast<int>(aptr3e.size()));

                    executeresultinfo = ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, _iter1, _iter1e, false, false);     
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                    CHECK_EQUAL((2*static_cast<int>(aptr1.size())), static_cast<int>(aptr3e.size()));
                }

                aptr1.clear();
                aptr2.clear();
                aptr3.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_allRows)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            {
                ExecuteResultInfo executeresultinfo =  ai1.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                // all rows:
                for (;!iter1.isEnd(); iter1++)
                {
                    AccessorInstanceRef ai4 = Manager::getInstance().createAccessorInstance(
                        "test_sqlbuilderWhere_WhereSelect_AccessorInstanceRef_nofloat_2",
                        ACCESSORNAME_TestSqlBuilder_SelectWhere_nofloat, 
                        connRef,
                        basar::db::aspect::FULL_CACHING);
                    {
                        executeresultinfo =  ai4.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, iter1);

                        CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                        CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                        AccessorPropertyTableRef  aptr4 = ai4.getPropertyTable();
                        AccessorPropertyTable_YIterator iter4 = aptr4.begin();
                        CHECK(iter1.compareSetValues(iter4).first);
                        aptr4.clear();
                    }
                }
                aptr1.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }
    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_specialCases1)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai0 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest1",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest1, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest2",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest2, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai3 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest3",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest3, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai4 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest4",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest4, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            {
                ExecuteResultInfo executeresultinfo = ai0.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                AccessorPropertyTableRef  aptr0 = ai0.getPropertyTable();
                AccessorPropertyTable_YIterator iter0 = aptr0.begin();
                ++iter0;


                executeresultinfo = ai1.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest1, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest2, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo = ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest3, iter0); 
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo =  ai4.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest4, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                AccessorPropertyTableRef  aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                AccessorPropertyTableRef  aptr3 = ai3.getPropertyTable();
                AccessorPropertyTable_YIterator iter3 = aptr3.begin();

                AccessorPropertyTableRef  aptr4 = ai4.getPropertyTable();
                AccessorPropertyTable_YIterator iter4 = aptr4.begin();

                CHECK(iter0.compareSetValues(iter1).first);
                CHECK(iter0.compareSetValues(iter2).first);
                CHECK(iter0.compareSetValues(iter3).first);
                CHECK(iter0.compareSetValues(iter4).first);

                aptr0.clear();
                aptr1.clear();
                aptr2.clear();
                aptr3.clear();
                aptr4.clear();
            }

        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_specialCases_mask)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai0 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest1",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest1, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest2",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest2, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai3 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest3",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest3, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai4 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_markedStringSelectTest4",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_markedStringSelectTest4, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            {
                ExecuteResultInfo executeresultinfo =   ai0.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr0 = ai0.getPropertyTable();
                AccessorPropertyTable_YIterator iter0 = aptr0.begin();

                // the intersting row is colser=2, which includes many values with "#"
                ++iter0;

                executeresultinfo =    ai1.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest1, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo =    ai2.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest2, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo =    ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest3, iter0); 
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                executeresultinfo =    ai4.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_markedStringSelectTest4, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                AccessorPropertyTableRef  aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                AccessorPropertyTableRef  aptr3 = ai3.getPropertyTable();
                AccessorPropertyTable_YIterator iter3 = aptr3.begin();

                AccessorPropertyTableRef  aptr4 = ai4.getPropertyTable();
                AccessorPropertyTable_YIterator iter4 = aptr4.begin();

                CHECK(iter0.compareSetValues(iter1).first);
                CHECK(iter0.compareSetValues(iter2).first);
                CHECK(iter0.compareSetValues(iter3).first);
                CHECK(iter0.compareSetValues(iter4).first);

                aptr0.clear();
                aptr1.clear();
                aptr2.clear();
                aptr3.clear();
                aptr4.clear();
            }

        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_specialCases_like)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai0 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_likeTest1",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_likeTest1, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo executeresultinfo =   ai0.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr0 = ai0.getPropertyTable();
                AccessorPropertyTableRef  aptr1 ;
                AccessorPropertyTable_YIterator iter0;
                AccessorPropertyTable_YIterator iter1;

                for (iter0 = aptr0.begin(); iter0 != aptr0.end(); ++iter0)
                {
                    executeresultinfo =  ai1.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_likeTest1, iter0);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                    aptr1 = ai1.getPropertyTable();
                    CHECK_EQUAL(1, (int) aptr1.size());
                    AccessorPropertyTable_YIterator _iter1 = aptr1.begin();
                    CHECK(iter0.compareSetValues(_iter1).first);
                    aptr1.clear();
                }
                aptr0.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------

    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_specialCases_date)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai0 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_Date",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_Date, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {

                ExecuteResultInfo executeresultinfo =  
                    ai0.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr0 = ai0.getPropertyTable();
                AccessorPropertyTableRef  aptr1 ;
                AccessorPropertyTable_YIterator iter0 = aptr0.begin();

                executeresultinfo =  ai1.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_Date, iter0);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                aptr1 = ai1.getPropertyTable();

                AccessorPropertyTable_YIterator iter1 = aptr1.begin();
                CHECK(iter0.compareSetValues(iter1).first);
                aptr1.clear();
                aptr0.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }
    // -----------------------------------------------------------------------------------------------------------------

    TEST(DBAcpect_AccessorInstanceRef_SqlResolvTest_SelectWhere_specialCases_datetime)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai0 = Manager::getInstance().createAccessorInstance(
                "test_sqlbuilderWhere_Select_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Accessor_TestSqlBuilder_SelectWhere_DateTime",
                ACCESSORNAME_TestSqlBuilder_SelectWhere_DateTime, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo executeresultinfo =  ai0.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr0 = ai0.getPropertyTable();
                AccessorPropertyTableRef  aptr1 ;
                AccessorPropertyTable_YIterator iter0 = aptr0.begin();

                executeresultinfo = ai1.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_DateTime, iter0);

                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                aptr1 = ai1.getPropertyTable();

                AccessorPropertyTable_YIterator iter1 = aptr1.begin();
                CHECK(iter0.compareSetValues(iter1).first);
                aptr1.clear();
                aptr0.clear();
            }
        }
        catch (basar::Exception & ex)
        {
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
        connRef.close();
    }

  
    // -----------------------------------------------------------------------------------------------------------------
}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

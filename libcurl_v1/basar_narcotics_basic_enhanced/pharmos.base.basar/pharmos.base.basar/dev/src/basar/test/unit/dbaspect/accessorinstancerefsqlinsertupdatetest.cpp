/** $Id$                                          
*                                                  
* @file accessorinstancerefsqlinsertupdatetest.cpp                                                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testaccessorexecutelistener.h"
#include "testnames.h"
#include "libbasardbsql_connectionref.h"
#include "buttablepropertydescriptions.h"

using namespace basar::db::aspect;
using basar::test::unit::dbaspect::TestConnection;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerStdOutLog;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent;
using basar::cmnutil::ParameterList;

#define TESTDBACPECT_ACCESSORINSTANCEREF_SQLINSERTUPDATETEST
#ifdef TESTDBACPECT_ACCESSORINSTANCEREF_SQLINSERTUPDATETEST

//------------------------------------------------------------------------------
SUITE(DBAcpect_AccessorInstanceRef_SqlInsertUpdateTest)
{

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlInsertUpdateTest_DateTimeUpdateDate)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        std::vector<basar::db::aspect::CachingPolicyEnum > caching_test;
        caching_test.push_back( basar::db::aspect::FULL_CACHING );
        caching_test.push_back( basar::db::aspect::ON_DEMAND_CACHING );
        caching_test.push_back( basar::db::aspect::SINGLE_ROW_CACHING );
        std::vector<basar::db::aspect::CachingPolicyEnum>::const_iterator iter;
        for (iter = caching_test.begin(); iter != caching_test.end(); ++iter)
        {
            basar::db::aspect::CachingPolicyEnum caching = *iter;

            try
            {
                AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                    "test_Acc_DateTimeSelect",
                    ACCESSORNAME_DateTimeSelect, 
                    connRef,
                    caching);

                TestAccessorExecuteListenerHoldLastEvent listener1;
                ai1.setAccessorExecuteListener (listener1);
                int times = 0;
                {
                    basar::Int32 expectedRows = 1; 
                    basar::Int32 expectedRows2 = 2; 
                    ExecuteResultInfo executeresultinfo = 
                        ai1.execute(ACCESSMETHOD_DateTimeSelect2Rows);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                    //check listener:
                    {
                        // select only:
                        CHECK_EQUAL(times, listener1.m_lastEvent->m_times);
                        CHECK(0 == listener1.m_lastEvent->m_lastEvent.get());
                    }

                    AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                    AccessorPropertyTable_YIterator iter1 = aptr1.begin();


                    AccessorPropertyTableRef a = AccessorPropertyTableRef(iter1.getPropertyTable());

                    AccessorPropertyTable_YIterator iter1e =  aptr1.end();
                    if (basar::db::aspect::FULL_CACHING  == caching )
                    {
                        CHECK_EQUAL(expectedRows2, static_cast<basar::Int32>(aptr1.size()));
                    }

                    iter1 = aptr1.begin();

                    executeresultinfo = 
                        ai1.execute(ACCESSMETHOD_DateTimeUpdateDate, iter1);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(expectedRows, (int)executeresultinfo.getAffectedRows());

                    //check listener:
                    {
                        CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                        CHECK_EQUAL(++times, listener1.m_lastEvent->m_times);
                        AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                        CHECK_EQUAL(ACCESSMETHOD_DateTimeUpdateDate, event.m_methodName);
                        CHECK(event.m_yIterator == iter1);                  
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                        CHECK_EQUAL(1, (int)event.m_executeResult.getAffectedRows());
                        CHECK_EQUAL(1, static_cast<int>(event.m_separateBuilderResults.size()));
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_separateBuilderResults[0].getError());
                        CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                    }
                    AccessorPropertyTableRef  aptr2 = ai1.getPropertyTable();
                    if (basar::db::aspect::FULL_CACHING  == caching )
                    {
                        CHECK_EQUAL(expectedRows2, static_cast<basar::Int32>(aptr1.size()));
                    }

                    executeresultinfo = 
                        ai1.execute(ACCESSMETHOD_DateTimeUpdateDate, iter1, true);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(expectedRows, (int)executeresultinfo.getAffectedRows());

                    //check listener:
                    {
                        CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                        CHECK_EQUAL(++times, listener1.m_lastEvent->m_times);
                        AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                        CHECK_EQUAL(ACCESSMETHOD_DateTimeUpdateDate, event.m_methodName);
                        CHECK(event.m_yIterator == iter1);                  
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                        CHECK_EQUAL(1, (int)event.m_executeResult.getAffectedRows());
                        CHECK_EQUAL(1, static_cast<int>(event.m_separateBuilderResults.size()));
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_separateBuilderResults[0].getError());
                        CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                    }

                    // reset:
                    listener1.m_lastEvent->m_lastEvent =  boost::shared_ptr<basar::db::aspect::AccessorExecuteEvent> ();

                    executeresultinfo = 
                        ai1.execute(ACCESSMETHOD_DateTimeUpdateDate, iter1, iter1, true);
                    CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
                    CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                    CHECK(0 == listener1.m_lastEvent->m_lastEvent.get());
                    CHECK_EQUAL(times, listener1.m_lastEvent->m_times);

                    executeresultinfo = 
                        ai1.execute(ACCESSMETHOD_DateTimeUpdateDate, iter1, iter1e, true);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                    CHECK_EQUAL(expectedRows2, (int)executeresultinfo.getAffectedRows());

                    //check listener :
                    {
                        CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                        times += 2;
                        CHECK_EQUAL(times, listener1.m_lastEvent->m_times);
                        AccessorPropertyTable_YIterator iter2 = iter1;
                        ++iter2;

                        AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                        CHECK_EQUAL(ACCESSMETHOD_DateTimeUpdateDate, event.m_methodName);
                       
                        if (caching != basar::db::aspect::SINGLE_ROW_CACHING)
                        {
                            CHECK(event.m_yIterator == (iter2));                  
                        }
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                        CHECK_EQUAL(1, (int)event.m_executeResult.getAffectedRows());
                        CHECK_EQUAL(1, static_cast<int>(event.m_separateBuilderResults.size()));
                        CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_separateBuilderResults[0].getError());
                        CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                    }

                    AccessorPropertyTableRef  aptr3 = ai1.getPropertyTable();
                    CHECK_EQUAL(expectedRows2, static_cast<basar::Int32>(aptr3.getNumberOfFetchedRows()));

                    aptr1.clear();
                }
            }
            catch (basar::Exception & ex)
            {
                connRef.close();	
                std::cout << "Exception in caching mode : "<< caching << ": " 
                    << ex.what() << std::endl;
                const bool ExceptionOccured = false;
                CHECK(ExceptionOccured);
                //throw;
            }
        }
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlInsertUpdateTest_InsertOne_and_InsertDouplicate)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		basar::db::sql::ConnectionRef sqlConn = connRef.getSqlConnection();
		sqlConn.begin();

        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertUpdate_InsertOne",
                ACCESSORNAME_TestInsertUpdate_InsertOne, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertUpdate_InsertDuplicate",
                ACCESSORNAME_TestInsertUpdate_InsertDuplicate, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            TestAccessorExecuteListenerHoldLastEvent listener1;
            ai1.setAccessorExecuteListener (listener1);
            TestAccessorExecuteListenerHoldLastEvent listener2;
            ai2.setAccessorExecuteListener (listener2);

            {
                ExecuteResultInfo executeresultinfo = 
                    ai1.execute(ACCESSMETHOD_TestInsertUpdate_InsertOne);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

                //check listener:
                {
                    CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                    AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                    CHECK_EQUAL(ACCESSMETHOD_TestInsertUpdate_InsertOne, event.m_methodName);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                    CHECK_EQUAL(1, (int)event.m_executeResult.getAffectedRows());
                    CHECK_EQUAL(1, static_cast<int>(event.m_separateBuilderResults.size()));
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_separateBuilderResults[0].getError());
                    CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                }

                CHECK(ai1.hasLastInsertedSerial());
                CHECK_EQUAL(1, ai1.getNumberOfLastInsertedSerials());
                CHECK((ai1.getLastInsertedSerial(0) > 0));
                CHECK_THROW(ai1.getLastInsertedSerial(1), basar::Exception);
                CHECK_THROW(ai1.getLastInsertedSerial(-1), basar::Exception);

                basar::Int64 lastcolser = ai1.getLastInsertedSerial(0);

                ai1.clearLastInsertedSerials();
                CHECK(!ai1.hasLastInsertedSerial());
                CHECK_EQUAL(0, ai1.getNumberOfLastInsertedSerials());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                CHECK_EQUAL(0, static_cast<basar::Int32>(aptr1.size()));

                std::stringstream str;
                str << lastcolser ;
                basar::VarString s(str.str());
                ParameterList list;
                list.push_back(PROPDEF_COLSER.getName(),s.c_str());
                ai2.setDefaultParameter(list, 
                    ACCESSMETHOD_TestInsertUpdate_InsertDuplicate);
                executeresultinfo = 
                    ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertDuplicate, false, false);

                CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                CHECK_EQUAL(0, ai2.getNumberOfLastInsertedSerials());

                //check listener:
                {
                    CHECK(listener2.m_lastEvent->m_lastEvent != 0);
                    AccessorExecuteEvent & event = *(listener2.m_lastEvent->m_lastEvent.get());
                    CHECK_EQUAL(ACCESSMETHOD_TestInsertUpdate_InsertDuplicate, event.m_methodName);
                    CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT, event.m_executeResult.getError());
                    CHECK_EQUAL(0, (int)event.m_executeResult.getAffectedRows());
                    CHECK_EQUAL(1, static_cast<int>(event.m_separateBuilderResults.size()));
                    CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT,event.m_separateBuilderResults[0].getError());
                    CHECK_EQUAL(0, (int)event.m_separateBuilderResults[0].getAffectedRows());
                }

                CHECK_THROW(ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertDuplicate), basar::Exception);

                // returns error [<0] or rows [>=0] 
				{
					basar::Int32 retval_as_int = ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertDuplicate, false, false).getError();
					CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT, retval_as_int);
					CHECK(retval_as_int < 0);
				}

				{
					basar::BULong retval = ai1.execute(ACCESSMETHOD_TestInsertUpdate_InsertOne, false, false).getAffectedRows();
					CHECK_EQUAL(1, (int)retval);
				}

                aptr1.clear();
            }
        }
        catch (basar::Exception & ex)
        {
			sqlConn.rollback();
            connRef.close();	

            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }

		sqlConn.rollback();
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlInsertUpdateTest_InsertList)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		basar::db::sql::ConnectionRef sqlConn = connRef.getSqlConnection();
		sqlConn.begin();

        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertUpdate_InsertList",
                ACCESSORNAME_TestInsertUpdate_InsertList, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            TestAccessorExecuteListenerHoldLastEvent listener1;
            ai1.setAccessorExecuteListener (listener1);
            {
                ExecuteResultInfo executeresultinfo = 
                    ai1.execute(ACCESSMETHOD_TestInsertUpdate_InsertList);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(2, (int)executeresultinfo.getAffectedRows());
                CHECK(ai1.hasLastInsertedSerial());
                CHECK_EQUAL(2, ai1.getNumberOfLastInsertedSerials());
                CHECK((ai1.getLastInsertedSerial(0) > 0));
                CHECK((ai1.getLastInsertedSerial(1) > 0));
                CHECK_THROW(ai1.getLastInsertedSerial(2), basar::Exception);
                CHECK_THROW(ai1.getLastInsertedSerial(-1), basar::Exception);

                //check listener:
                {
                    CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                    AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                    CHECK_EQUAL(ACCESSMETHOD_TestInsertUpdate_InsertList, event.m_methodName);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                    CHECK_EQUAL(2, (int)event.m_executeResult.getAffectedRows());
                    CHECK_EQUAL(2, static_cast<int>(event.m_separateBuilderResults.size()));
                    CHECK_EQUAL(basar::db::aspect::SUCCESS,event.m_separateBuilderResults[0].getError());
                    CHECK_EQUAL(basar::db::aspect::SUCCESS,event.m_separateBuilderResults[1].getError());
                    CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                    CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                }

				basar::BULong retval_as_int = ai1.execute(ACCESSMETHOD_TestInsertUpdate_InsertList, false, false).getAffectedRows();
                CHECK_EQUAL(2, (int)retval_as_int);
                //check listener:
                {
                    CHECK(listener1.m_lastEvent->m_lastEvent != 0);
                    AccessorExecuteEvent & event = *(listener1.m_lastEvent->m_lastEvent.get());
                    CHECK_EQUAL(ACCESSMETHOD_TestInsertUpdate_InsertList, event.m_methodName);
                    CHECK_EQUAL(basar::db::aspect::SUCCESS, event.m_executeResult.getError());
                    CHECK_EQUAL(2, (int)event.m_executeResult.getAffectedRows());
                    CHECK_EQUAL(2, static_cast<int>(event.m_separateBuilderResults.size()));
                    CHECK_EQUAL(basar::db::aspect::SUCCESS,event.m_separateBuilderResults[0].getError());
                    CHECK_EQUAL(basar::db::aspect::SUCCESS,event.m_separateBuilderResults[1].getError());
                    CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                    CHECK_EQUAL(1, (int)event.m_separateBuilderResults[0].getAffectedRows());
                }
            }
        }
        catch (basar::Exception & ex)
        {
			sqlConn.rollback();
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }
		sqlConn.rollback();
        connRef.close();
    }

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAcpect_AccessorInstanceRef_SqlInsertUpdateTest_InsertList_including_Douplicate)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		basar::db::sql::ConnectionRef sqlConn = connRef.getSqlConnection();
		sqlConn.begin();

		try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertUpdate_InsertOne",
                ACCESSORNAME_TestInsertUpdate_InsertOne, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_AccMethod_TestInsertUpdate_InsertListWithDuplicate",
                ACCESSORNAME_TestInsertUpdate_InsertListWithDuplicate, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo executeresultinfo = 
                    ai1.execute(ACCESSMETHOD_TestInsertUpdate_InsertOne);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
                CHECK(ai1.hasLastInsertedSerial());
                CHECK_EQUAL(1, ai1.getNumberOfLastInsertedSerials());
                CHECK((ai1.getLastInsertedSerial(0) > 0));
                CHECK_THROW(ai1.getLastInsertedSerial(1), basar::Exception);
                CHECK_THROW(ai1.getLastInsertedSerial(-1), basar::Exception);

                basar::Int64 lastcolser = ai1.getLastInsertedSerial(0);

                ai1.clearLastInsertedSerials();
                CHECK(!ai1.hasLastInsertedSerial());
                CHECK_EQUAL(0, ai1.getNumberOfLastInsertedSerials());


                std::stringstream str;
                str << lastcolser ;
                basar::VarString s(str.str());
                ParameterList list;
                list.push_back("colser",s.c_str());
                ai2.setDefaultParameter(list, 
                    ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate);
                executeresultinfo = 
                    ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate, false, false);

                CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT, executeresultinfo.getError());
                CHECK_EQUAL(2, (int)executeresultinfo.getAffectedRows());
                CHECK_EQUAL(2, ai2.getNumberOfLastInsertedSerials());

                CHECK_THROW(ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate), basar::Exception);

                // returns error [<0] or rows [>=0] 
                basar::Int32 retval_as_int = ai2.execute(ACCESSMETHOD_TestInsertUpdate_InsertListWithDuplicate, false, false).getError();
                CHECK_EQUAL(basar::db::aspect::SQL_ERROR_DUPL_INSERT, retval_as_int);
                CHECK(retval_as_int < 0);

            }
        }
        catch (basar::Exception & ex)
        {
			sqlConn.rollback();
            connRef.close();	
            std::cout << ex.what() << std::endl;
            const bool ExceptionOccured = false;
            CHECK(ExceptionOccured);
            throw;
        }

		sqlConn.rollback();
        connRef.close();
    }

}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

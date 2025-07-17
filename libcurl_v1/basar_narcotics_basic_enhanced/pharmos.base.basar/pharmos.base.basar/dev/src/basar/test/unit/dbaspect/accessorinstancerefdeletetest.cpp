 /** $Id$                                          
  *                                                  
  * @file accessorinstancerefdeletetest.cpp                                  
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testaccessorexecutelistener.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"

using namespace basar::db::aspect;
using basar::test::unit::dbaspect::TestConnection;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerStdOutLog;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent;
using basar::cmnutil::ParameterList;
using basar::Int32;
using basar::Int64;

#define TESTDBASPECT_ACCESSORINSTANCEREF_DELETETEST
#ifdef TESTDBASPECT_ACCESSORINSTANCEREF_DELETETEST

//------------------------------------------------------------------------------
SUITE(DBAspect_AccessorInstanceRef_DeleteTest)
{

    // -----------------------------------------------------------------------------------------------------------------
    TEST(DBAspect_AccessorInstanceRef_DeleteTest_deleteRow)
    {
        
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        const Int32 testint = 11; // could be any other number;
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertDeleteRow",
                ACCESSORNAME_TestDelete_DeleteRow, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorPropertyTable_YIterator yiter = ai1.getPropertyTable().insert(basar::FOR_INSERT);
            CHECK(!yiter.isEnd());
            yiter.setInt32(PROPDEF_COLINT.getName(), testint);
            yiter.setInt16(PROPDEF_COLSMINT.getName(), testint);

            ExecuteResultInfo executeresultinfo = 
                ai1.execute(ACCESSMETHOD_TestDelete_InsertRow, yiter);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
            CHECK(ai1.hasLastInsertedSerial());
            CHECK_EQUAL(1, ai1.getNumberOfLastInsertedSerials());
            Int64 lastcolser = ai1.getLastInsertedSerial(0);
            CHECK(lastcolser > 0);
            ai1.clearLastInsertedSerials();
            CHECK(!ai1.hasLastInsertedSerial());

            yiter = ai1.getPropertyTable().insert(basar::FOR_DELETE);
            CHECK(!yiter.isEnd());
            yiter.setInt32(PROPDEF_COLSER.getName(), static_cast<basar::Int32>(lastcolser));

       
            executeresultinfo = ai1.execute(ACCESSMETHOD_TestDelete_DeleteRow, yiter);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
            CHECK(!ai1.hasLastInsertedSerial());
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
    
    TEST(DBAspect_AccessorInstanceRef_DeleteTest_deleteRowsInIterator)
    {   
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        const Int32 testint = 11; // could be any other number;
        const int testnumber = 4; // number of statements to be executed;
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertDeleteRow",
                ACCESSORNAME_TestDelete_DeleteRow, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorPropertyTable_YIterator yiter;
            AccessorPropertyTable_YIterator yiter_end;
            for (int i=0; i< testnumber; ++i)
            {
              yiter = ai1.getPropertyTable().insert(basar::FOR_INSERT);
              CHECK(!yiter.isEnd());
              yiter.setInt32(PROPDEF_COLINT.getName(), testint);
              yiter.setInt16(PROPDEF_COLSMINT.getName(), testint);
            }

            CHECK_EQUAL(testnumber, static_cast<Int32>(ai1.getPropertyTable().size()));

            TestAccessorExecuteListenerHoldLastEvent listener1;

            yiter = ai1.getPropertyTable().begin();
            yiter_end = ai1.getPropertyTable().end();
            ExecuteResultInfo executeresultinfo = 
                ai1.execute(ACCESSMETHOD_TestDelete_InsertRow, yiter, yiter_end);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(testnumber, (int)executeresultinfo.getAffectedRows());

            CHECK(ai1.hasLastInsertedSerial());
            CHECK_EQUAL(testnumber, ai1.getNumberOfLastInsertedSerials());

            ai1.getPropertyTable().clear();
            for (int i=0; i< testnumber; ++i)
            {
              Int64 lastcolser = ai1.getLastInsertedSerial(i);
              CHECK(lastcolser > 0);  
              yiter = ai1.getPropertyTable().insert(basar::FOR_DELETE);
              CHECK(!yiter.isEnd());
              yiter.setInt32(PROPDEF_COLSER.getName(), static_cast<basar::Int32>(lastcolser));
            }
            ai1.clearLastInsertedSerials();
            CHECK(!ai1.hasLastInsertedSerial());
            
            CHECK_EQUAL(testnumber, static_cast<Int32>(ai1.getPropertyTable().size()));

            yiter = ai1.getPropertyTable().begin();
            yiter_end = ai1.getPropertyTable().end();
           
            executeresultinfo = ai1.execute(ACCESSMETHOD_TestDelete_DeleteRow, yiter,yiter_end);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());

            CHECK_EQUAL(testnumber, (int)executeresultinfo.getAffectedRows());
            CHECK(!ai1.hasLastInsertedSerial());

            yiter = ai1.getPropertyTable().begin();
            CHECK(yiter.isEnd());

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
    
    TEST(DBAspect_AccessorInstanceRef_DeleteTest_deleteRowsInIterator_Uses_2_AccessorInstanceRef)
    {   
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        const Int32 testint = 11; // could be any other number;
        const int testnumber = 4; // number of statements to be executed;
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertDeleteRow",
                ACCESSORNAME_TestDelete_DeleteRow, 
                connRef,
                basar::db::aspect::FULL_CACHING, 
                true);
            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test_Acc_TestInsertDeleteRow2",
                ACCESSORNAME_TestDelete_DeleteRow, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            AccessorPropertyTable_YIterator yiter;
            AccessorPropertyTable_YIterator yiter_end;
            for (int i=0; i< testnumber; ++i)
            {
              yiter = ai2.getPropertyTable().insert(basar::FOR_INSERT);
              CHECK(!yiter.isEnd());
              yiter.setInt32(PROPDEF_COLINT.getName(), testint);
              yiter.setInt16(PROPDEF_COLSMINT.getName(), testint);
            }

            CHECK_EQUAL(testnumber, static_cast<Int32>(ai2.getPropertyTable().size()));

            TestAccessorExecuteListenerHoldLastEvent listener1;

            yiter = ai2.getPropertyTable().begin();
            yiter_end = ai2.getPropertyTable().end();
            ExecuteResultInfo executeresultinfo = 
                ai1.execute(ACCESSMETHOD_TestDelete_InsertRow, yiter, yiter_end);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(testnumber, (int)executeresultinfo.getAffectedRows());

            CHECK(ai1.hasLastInsertedSerial());
            CHECK_EQUAL(testnumber, ai1.getNumberOfLastInsertedSerials());

            ai2.getPropertyTable().clear();
            for (int i=0; i< testnumber; ++i)
            {
              Int64 lastcolser = ai1.getLastInsertedSerial(i);
              CHECK(lastcolser > 0);  
              yiter = ai2.getPropertyTable().insert(basar::FOR_DELETE);
              CHECK(!yiter.isEnd());
              yiter.setInt32(PROPDEF_COLSER.getName(), static_cast<basar::Int32>(lastcolser));
            }
            ai1.clearLastInsertedSerials();
            CHECK(!ai1.hasLastInsertedSerial());
            
            CHECK_EQUAL(testnumber, static_cast<Int32>(ai2.getPropertyTable().size()));

            yiter = ai2.getPropertyTable().begin();
            yiter_end = ai2.getPropertyTable().end();
           
            executeresultinfo = ai1.execute(ACCESSMETHOD_TestDelete_DeleteRow, yiter,yiter_end);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());

            CHECK_EQUAL(testnumber, (int)executeresultinfo.getAffectedRows());
            CHECK(!ai1.hasLastInsertedSerial());

            yiter = ai2.getPropertyTable().begin();
            CHECK(yiter.isEnd());

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


}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

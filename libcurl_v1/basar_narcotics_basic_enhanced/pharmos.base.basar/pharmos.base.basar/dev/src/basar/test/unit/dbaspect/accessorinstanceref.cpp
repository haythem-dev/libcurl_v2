/*
* @file AccessorDefinitionList.cpp                                                                     
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testnames.h"
#include "buttablepropertydescriptionvectors.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarproperty_propertytable_xiterator.h"

#include "libbasarproperty_exceptions.h"
#include "libbasarcmnutil_exceptions.h"

// -----------------------------------------------------------------------------------------------------------------

using basar::ConstString;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::Manager;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::dbaspect::TestConnection;

// -----------------------------------------------------------------------------------------------------------------
#define TESTDBASPECT_ACCESSORDEFINITIONLIST
#ifdef TESTDBASPECT_ACCESSORDEFINITIONLIST
//------------------------------------------------------------------------------
SUITE(DBAspect_AccessorInstanceRef)
{
    // -----------------------------------------------------------------------------------------------------------------
	
	static ConstString ACCESSORNAME_NoConnection =  "Accessor_NoConnection";

	BEGIN_ACCESSOR_DEFINITION(ACCESSORNAME_NoConnection)
		PROPERTY_DESCRIPTION_LIST("int32 colser; int32 colint; int16 colsmint;")
		// no methods
	END_ACCESSOR_DEFINITION

    TEST(NoConnection)
    {
		{
			ConstString   ACCINSTNAME_NoConnection  =  "AccessorInstance_NoConnection";
			ConnectionRef conn;

			AccessorInstanceRef accInst = Manager::getInstance().createAccessorInstance(
				ACCINSTNAME_NoConnection,
				ACCESSORNAME_NoConnection, 
				conn);
		}

		// no assertion (because of "null" connection) may occur
		CHECK(true);
	}

    // -----------------------------------------------------------------------------------------------------------------

    TEST(DBAspect_AccessorInstanceRef_Int)
    {

        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test5_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            {
                ExecuteResultInfo executeresultinfo = ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                AccessorPropertyTableRef aptr2 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                CHECK(iter1.compareSetValues(iter2).first);
                CHECK(iter1 == iter2);
                basar::db::aspect::NumberRows  numberrows = propertyDescriptionVectorButTableDescriptionIntValues.m_vector.size();
                CHECK_EQUAL(((unsigned int)numberrows), ((unsigned int)iter1.size()));
                CHECK_EQUAL(((unsigned int)numberrows), ((unsigned int)iter2.size()));

                basar::db::aspect::AccessorPropertyTable_XIterator xiter = iter1.begin();
                CHECK(xiter.isBegin());
                CHECK(!xiter.isEnd());
                xiter = iter1.end();
                CHECK(!xiter.isBegin());
                CHECK(xiter.isEnd());

                iter2++;
                CHECK(!(iter1.compareSetValues(iter2).first));
                CHECK(iter1 != iter2);
                iter2--;
                CHECK(iter1 == iter2);
                iter2++;
                iter1.match(iter2);
                CHECK(iter1.compareSetValues(iter2).first);
                CHECK(iter1 != iter2);

                CHECK_EQUAL(iter2.getInt32(0), iter1.getInt32(0));
                CHECK_EQUAL(iter2.getInt32(1), iter1.getInt32(1));
                CHECK_EQUAL(iter2.getInt16(2), iter1.getInt16(2));
                iter1 = aptr1.begin();
                iter2 = aptr1.begin();
                CHECK(iter1 == iter2);
                aptr1.clear();
                //TODO: test leads to debug assertion in VS2010
				//CHECK(iter1 == iter2);
                aptr2.clear();
                // CHECK(iter1.isBegin());
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

    TEST(DBAspect_AccessorInstanceRef_2differentselects)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test5_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test6_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo	executeresultinfo = ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                executeresultinfo = ai2.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                AccessorPropertyTableRef aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                CHECK(!iter1.compareSetValues(iter2).first);

                CHECK(iter1.size() != iter2.size());

                iter1.match(iter2);

                iter1++;
                iter2++;
                iter2.match(iter1);

                aptr1.clear();
                aptr2.clear();
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
    TEST(DBAspect_AccessorInstanceRef_selectAll)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test5_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test6_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo executeresultinfo;
                executeresultinfo = ai1.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                executeresultinfo = ai2.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTable_YIterator iter1 = aptr1.begin();

                AccessorPropertyTableRef aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();


                CHECK(iter1.compareSetValues(iter2).first);

                aptr1.clear();
                aptr2.clear();
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
    TEST(DBAspect_AccessorInstanceRef_2differentselects_join)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test5_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
                "test6_AccessorInstanceRef",
                ACCESSORNAME_ButTable_Join_byColser_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);
            AccessorInstanceRef ai3 = Manager::getInstance().createAccessorInstance(
                "test6_AccessorInstanceRef",
                ACCESSORNAME_ButTable_Join_byColser_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            {
                ExecuteResultInfo executeresultinfo;
                executeresultinfo = ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);			 

                ai2.setDefaultParameter("selectedid=10;");
                executeresultinfo = ai2.execute(ACCESSMETHOD_ButTable_Join_byColser_Int);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                ai3.setDefaultParameter("selectedid=9;");
                executeresultinfo = ai3.execute(ACCESSMETHOD_ButTable_Join_byColser_Int);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();
                AccessorPropertyTableRef aptr2 = ai2.getPropertyTable();
                AccessorPropertyTableRef aptr3 = ai3.getPropertyTable();

                AccessorPropertyTable_YIterator iter1 = aptr1.begin();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();
                AccessorPropertyTable_YIterator iter3 = aptr3.begin();

                CHECK(!iter1.compareSetValues(iter2).first);

                CHECK(iter2.compareSetValues(iter2).first);

                CHECK(!iter3.compareSetValues(iter2).first);

/* obsolete
                CHECK_THROW(iter1 != iter2, basar::DifferentContainersIteratorException);
                CHECK_THROW(iter1 == iter2, basar::DifferentContainersIteratorException);
                CHECK_THROW(iter3 == iter2, basar::DifferentContainersIteratorException);
*/
                iter3.match(iter2);
                CHECK(iter3.compareSetValues(iter2).first);

                CHECK_THROW(iter3.getString("master_colint"),basar::property::WrongPropertyTypeException);

                CHECK_EQUAL(iter3.getInt32("master_colint"), iter2.getInt32("master_colint"));
                CHECK_THROW(iter3.getString("master_colint"), basar::Exception);
                CHECK_EQUAL(iter3.getInt32("master_colser"), iter2.getInt32("master_colser"));
                CHECK_EQUAL(iter3.getInt16("master_colsmint"), iter2.getInt16("master_colsmint"));
                CHECK_THROW(iter3.getInt32("detail_colsint"), basar::cmnutil::BasarUnknownPropertyNameException);

                iter1.match(iter2);
                iter1++;
                iter2++;
                iter2.match(iter1);

                // no same parameter:
                CHECK(!iter2.compareSetValues(iter1).first);

                aptr1.clear();
                aptr2.clear();
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
    TEST(DBAspect_AccessorInstanceRef_Sqlbuilder_SelectWhere)
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
                executeresultinfo =   ai3.execute(ACCESSMETHOD_TestSqlBuilder_SelectWhere_nofloat, iter1);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

                AccessorPropertyTableRef  aptr2 = ai2.getPropertyTable();
                AccessorPropertyTable_YIterator iter2 = aptr2.begin();

                AccessorPropertyTableRef  aptr3 = ai3.getPropertyTable();
                AccessorPropertyTable_YIterator iter3 = aptr3.begin();
                CHECK(iter1.compareSetValues(iter3).first);			

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
    // up to basar 01.11.00.00 non-executed accessors would not cleanup properly as the accessors never registered with their db connection
    TEST(DBAspect_AccessorInstanceRef_ProperCleanupOfNotYetExecuteedAccessors)
    {
        ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

        connRef.beginTransaction();

        try
        {
            AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
                "test5_AccessorInstanceRef",
                ACCESSORNAME_ButMasterTableReader_Int, 
                connRef,
                basar::db::aspect::FULL_CACHING);

            ai1.getPropertyTable().insert( basar::FOR_CLEAN );

            ai1.reset();
        }
        catch (basar::Exception& ex )
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

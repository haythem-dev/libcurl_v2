/*
* @file accessorpropertytableref.cpp                                  
* @author Anke Klink                      
* @date 2008                            
*/    
#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessorpropertytablerefbeginfixture.h"

using basar::Int16;
using basar::Int32;

using basar::property::PropertyDescription;

using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;


#define TESTDBASPECT_ACCESSORPROPERTYTABLEREF
#ifdef TESTDBASPECT_ACCESSORPROPERTYTABLEREF

namespace db_aspect_unit_test
{
//------------------------------------------------------------------------------
SUITE(DBAspect_AccessorPropertyTableRef)
{
// -----------------------------------------------------------------------------------------------------------------
/*
//commented due to unclairity if basar has to support erasing during fetching with on demand caching
TEST( AccessorPropertyTableRef_FetchAfterErase )
{
    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance("mytest", ACCESSORNAME_ButMasterTableReader_All, connRef, basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo executeresultinfo = ai.execute(ACCESSMETHOD_ButMasterTableReader_All);

        CHECK_EQUAL( false, executeresultinfo.hasError() );

        CHECK_EQUAL( 1, static_cast<int>( ai.getPropertyTable().size() ) );

        AccessorPropertyTable_YIterator yit = ai.getPropertyTable().begin();

        ++yit;

        CHECK_EQUAL( 2, static_cast<int>( ai.getPropertyTable().size() ) );

        yit = ai.getPropertyTable().erase( ai.getPropertyTable().begin() );

        CHECK_EQUAL( 1, static_cast<int>( ai.getPropertyTable().size() ) );

        ++yit;

        CHECK_EQUAL( 2, static_cast<int>( ai.getPropertyTable().size() ) );

    }
    catch (basar::Exception & ex)
    {
        connRef.close();    
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}
*/
// -----------------------------------------------------------------------------------------------------------------
TEST(DBAspect_AccessorPropertyTableRef_at)
{
    std::vector<basar::db::aspect::CachingPolicyEnum > caching_test;
    caching_test.push_back( basar::db::aspect::FULL_CACHING );
    caching_test.push_back( basar::db::aspect::ON_DEMAND_CACHING );
    caching_test.push_back( basar::db::aspect::SINGLE_ROW_CACHING );
    caching_test.push_back( basar::db::aspect::NO_CACHING );

    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

    try
    {
        std::vector<basar::db::aspect::CachingPolicyEnum >::const_iterator veciter;

        for ( veciter = caching_test.begin(); veciter != caching_test.end(); ++veciter )
        {
            AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance(
                                         "cachingTest1",
                                         ACCESSORNAME_ButMasterTableReader_All,
                                         connRef,
                                         *veciter );

            ai.execute( ACCESSMETHOD_ButMasterTableReader_All );

            AccessorPropertyTableRef propTab = ai.getPropertyTable();

            switch ( *veciter )
            {
                case basar::db::aspect::NO_CACHING:
                case basar::db::aspect::SINGLE_ROW_CACHING:
                {
                    CHECK_THROW( propTab.at( 0 ), basar::db::aspect::InvalidMethodCallException );
                }
                break;

                case basar::db::aspect::FULL_CACHING:
                    {
                        //check edges:
                        CHECK_THROW( propTab.at( -1 ), basar::OutOfRangeIndexException );
                        CHECK_THROW( propTab.at( 11 ), basar::OutOfRangeIndexException );

                        for ( int i = 0; i < 11; ++i )
                        {
                            AccessorPropertyTable_YIterator yit = propTab.at( i );

                            if ( 0 == i )
                            {
                                CHECK( true == yit.isBegin() );
                            }

                            if ( 10 == i )
                            {
                                CHECK( true == yit.isEnd() );
                            }
                        }

                    }
                    break;

                case basar::db::aspect::ON_DEMAND_CACHING:
                    {
                        //force replication
                        AccessorPropertyTable_YIterator yit = propTab.at( 8 );

                        CHECK_EQUAL( 9, yit.getInt32("colser") );
                        //check edges:
                        CHECK_THROW( propTab.at( -1 ), basar::OutOfRangeIndexException );
                        CHECK_THROW( propTab.at( 11 ), basar::OutOfRangeIndexException );
                    }
                    break;
                default:
                    CHECK( false );
                    break;
            };

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
TEST(DBAspect_AccessorPropertyTableRef_cachingPolicies_sizes)
{
    std::vector<basar::db::aspect::CachingPolicyEnum > caching_test;
    caching_test.push_back( basar::db::aspect::FULL_CACHING );
    caching_test.push_back( basar::db::aspect::ON_DEMAND_CACHING );
    caching_test.push_back( basar::db::aspect::SINGLE_ROW_CACHING );
    caching_test.push_back( basar::db::aspect::NO_CACHING );

    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
    try
    {
        std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef> accessorInstanceRefMap;
        const double deltaFloat = 0.0001;

        std::vector<basar::db::aspect::CachingPolicyEnum >::const_iterator veciter;
        for (veciter = caching_test.begin(); veciter != caching_test.end(); ++veciter)
        {
            AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance(
                "cachingTest1",
                ACCESSORNAME_ButMasterTableReader_All, 
                connRef,
                *veciter);
            accessorInstanceRefMap[*veciter] = ai;
        }

        // execute and test:
        {
            ExecuteResultInfo executeresultinfo; 
            int resultnumberNo = 0; 
            int resultnumberOne = 1; 
            int resultnumberAll = 10; 
            int resultnumberOnDemand = 1; 
            int resultnumberAct = 0; 

            std::map<basar::db::aspect::CachingPolicyEnum, AccessorPropertyTableRef> accessorPropertyTableRefMap;
            std::map<basar::db::aspect::CachingPolicyEnum, AccessorPropertyTableRef>::iterator aptriter;

            std::map<basar::db::aspect::CachingPolicyEnum, AccessorPropertyTable_YIterator> accessorPropertyTable_YIteratorMap;
            std::map<basar::db::aspect::CachingPolicyEnum, AccessorPropertyTable_YIterator> accessorPropertyTable_YIteratorMap2;
            std::map<basar::db::aspect::CachingPolicyEnum, AccessorPropertyTable_YIterator>::iterator yiter;

            // 1. execute Statement:        
            std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef>::iterator aiiter;
            for (aiiter = accessorInstanceRefMap.begin(); aiiter != accessorInstanceRefMap.end(); ++aiiter)
            { 
                AccessorPropertyTableRef aptr = aiiter->second.getPropertyTable();
                CHECK(aptr.empty());
                executeresultinfo = aiiter->second.execute(ACCESSMETHOD_ButMasterTableReader_All);
                CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
                CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
                accessorPropertyTableRefMap[aiiter->first] = aptr;
                CHECK(!aptr.empty());
            }

            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].size()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].size()));
            CHECK_EQUAL( resultnumberNo,       static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].size()));
            CHECK_EQUAL( resultnumberOne,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].size()));

            // max_size:
            const int max = static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].max_size());
            CHECK_EQUAL( max, static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING ].max_size()));
            CHECK_EQUAL( max, static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING        ].max_size()));
            CHECK_EQUAL( max, static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].max_size()));

            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].getNumberOfFetchedRows()));

            for (aptriter = accessorPropertyTableRefMap.begin(); aptriter != accessorPropertyTableRefMap.end(); ++aptriter)
            {
                accessorPropertyTable_YIteratorMap[aptriter->first] = aptriter->second.begin();
                accessorPropertyTable_YIteratorMap2[aptriter->first] = aptriter->second.begin();
            }

            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].size()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].size()));
            CHECK_EQUAL( resultnumberNo,       static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].size()));
            CHECK_EQUAL( resultnumberOne,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].size()));
            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].getNumberOfFetchedRows()));

            resultnumberAct = 1;

            for (; resultnumberAct < resultnumberAll; ++resultnumberAct)
            {
                for (yiter = accessorPropertyTable_YIteratorMap.begin(); yiter != accessorPropertyTable_YIteratorMap.end(); ++yiter)
                {
                    yiter->second++;
                }

                ++resultnumberOnDemand;
                CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].size()));
                CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].size()));
                CHECK_EQUAL( resultnumberNo,       static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].size()));
                CHECK_EQUAL( resultnumberOne,      static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].size()));
                CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].getNumberOfFetchedRows()));
                CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].getNumberOfFetchedRows()));
                CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].getNumberOfFetchedRows()));
                CHECK_EQUAL( resultnumberOnDemand, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].getNumberOfFetchedRows()));

                yiter = accessorPropertyTable_YIteratorMap.begin();
                if (yiter != accessorPropertyTable_YIteratorMap.end())
                {
                    AccessorPropertyTable_YIterator firstIterator = accessorPropertyTable_YIteratorMap.begin()->second;
                    AccessorPropertyTable_YIterator secondIterator ;
                    for (yiter++; yiter != accessorPropertyTable_YIteratorMap.end(); ++yiter)
                    {
                        secondIterator = yiter->second;
                        CHECK_EQUAL(firstIterator.getInt32(PROPDEF_COLSER.getName()),   secondIterator.getInt32(PROPDEF_COLSER.getName()));
                        CHECK_EQUAL(firstIterator.getInt32(PROPDEF_COLINT.getName()),   secondIterator.getInt32(PROPDEF_COLINT.getName()));
                        CHECK_EQUAL(firstIterator.getInt16(PROPDEF_COLSMINT.getName()), secondIterator.getInt16(PROPDEF_COLSMINT.getName()));
                        std::vector<PropertyDescription>::const_iterator iter;
                        for (iter = propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.begin(); iter != propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.end(); ++iter)
                        {
                            CHECK_EQUAL(firstIterator.getString(iter->getName()), secondIterator.getString(iter->getName()));
                        }
                        CHECK_CLOSE(firstIterator.getFloat64(PROPDEF_COLFL.getName()), secondIterator.getFloat64(PROPDEF_COLFL.getName()), deltaFloat );
                        CHECK_CLOSE(firstIterator.getFloat32(PROPDEF_COLSMFL.getName()), secondIterator.getFloat32(PROPDEF_COLSMFL.getName()), deltaFloat);
                        CHECK_CLOSE(firstIterator.getDecimal(PROPDEF_COLDEC.getName()).toFloat64(), secondIterator.getDecimal(PROPDEF_COLDEC.getName()).toFloat64(), deltaFloat);
                        CHECK_CLOSE(firstIterator.getDecimal(PROPDEF_COLMON.getName()).toFloat64(), secondIterator.getDecimal(PROPDEF_COLMON.getName()).toFloat64(), deltaFloat);
                        CHECK_EQUAL(firstIterator.getDate(PROPDEF_COLDATE.getName()).toStrDate(), secondIterator.getDate(PROPDEF_COLDATE.getName()).toStrDate());
                        CHECK_EQUAL(firstIterator.getDateTime(PROPDEF_COLDT1.getName()).toStrDate(), secondIterator.getDateTime(PROPDEF_COLDT1.getName()).toStrDate());
                        CHECK_EQUAL(firstIterator.getDateTime(PROPDEF_COLDT1.getName()).toStrTime(), secondIterator.getDateTime(PROPDEF_COLDT1.getName()).toStrTime());
                        CHECK_EQUAL(firstIterator.getDateTime(PROPDEF_COLDT2.getName()).toStrDate(), secondIterator.getDateTime(PROPDEF_COLDT2.getName()).toStrDate());
                        CHECK_EQUAL(firstIterator.getDateTime(PROPDEF_COLDT2.getName()).toStrTime(), secondIterator.getDateTime(PROPDEF_COLDT2.getName()).toStrTime());                      
                        CHECK_EQUAL(firstIterator.getTime(PROPDEF_COLTIME1.getName()).toStrTime(), secondIterator.getTime(PROPDEF_COLTIME1.getName()).toStrTime());                      
                        firstIterator = secondIterator;
                    }
                }
            }

            for (yiter = accessorPropertyTable_YIteratorMap.begin(); yiter != accessorPropertyTable_YIteratorMap.end(); ++yiter)
            {
                yiter->second++;
                CHECK(yiter->second.isEnd());
            }

            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].size()));
            CHECK_EQUAL( resultnumberAll,      static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].size()));
            CHECK_EQUAL( resultnumberNo,       static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].size()));
            CHECK_EQUAL( resultnumberOne,      static_cast<int>( accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].size()));

            CHECK_EQUAL( resultnumberAll, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::FULL_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberAll, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::ON_DEMAND_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberAll, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::NO_CACHING].getNumberOfFetchedRows()));
            CHECK_EQUAL( resultnumberAll, static_cast<int>(accessorPropertyTableRefMap[basar::db::aspect::SINGLE_ROW_CACHING].getNumberOfFetchedRows()));

            for (aptriter = accessorPropertyTableRefMap.begin(); aptriter != accessorPropertyTableRefMap.end(); ++aptriter)
            {
                aptriter->second.clear();
            }
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

// ------------------------------------------------------------------------------------------------
TEST_FIXTURE( AccessorPropertyTableRefBeginFixture, DBAspect_AccessorPropertyTableRef_cachingPolicies_begin )
{
	try
	{
		CachingPolicyCollection::const_iterator iter;
		for( iter = m_cachingPolicies.begin(); iter != m_cachingPolicies.end(); ++iter )
		{
			CachingPolicyEnum policy = ( *iter );
			switch( policy )
			{
				case SINGLE_ROW_CACHING:
				case NO_CACHING:
				{
					// nothing to do, todo: tests useful?
					continue;
					break;
				}
				default:
					break;
			}

			AccessorPropertyTableRef table = m_propertyTables[ policy ];
			AccessorPropertyTable_YIterator yit = table.begin();
			CHECK( !yit.isEnd() );

			// change some states
			basar::Int32 countUpdate = 0;
			basar::Int32 countInsert = 0;
			basar::Int32 countDelete = 0;
			basar::Int32 countAll = 0;
			while( !yit.isEnd() )
			{
				basar::Int32 serial = yit.getInt32( PROPDEF_COLSER );
				if( 0 == ( serial % 2 ) )
				{
					yit.setPropertyState( PROPDEF_COLSER, basar::SS_UPDATE );
					++countUpdate;
				}
				else if( 0 == ( serial % 3 ) )
				{
					yit.setPropertyState( PROPDEF_COLSER, basar::SS_INSERT );
					++countInsert;
				}
				else
				{
					yit.setPropertyState( PROPDEF_COLSER, basar::SS_DELETE );
					++countDelete;
				}
				++yit;
				++countAll;
			}

			// count SS_UPDATE, don't use AccessorPropertyTableRefBeginFixture::testStateCount() here! [OPL 163]
			basar::Int32 count = 0;
			yit = table.begin( basar::SS_UPDATE );
			CHECK( PropertyStateSet( basar::SS_UPDATE ) == yit.getFilter() );
			while( !yit.isEnd() )
			{
				++count;
				++yit;
			}

			CHECK_EQUAL( countUpdate, count );

			// count all states, don't use AccessorPropertyTableRefBeginFixture::testStateCount() here! [OPL 163]
			count = 0;
			yit = table.begin();
			CHECK( PropertyStateSet( basar::SS_UNKNOWN ) == yit.getFilter() );
			while( !yit.isEnd() )
			{
				++count;
				++yit;
			}

			CHECK_EQUAL( countAll, count );

			// count SS_INSERT
			testStateCount( table, basar::SS_INSERT, countInsert );

			// count SS_DELETE
			testStateCount( table, basar::SS_DELETE, countDelete );

			// todo: set data into empty table and test
			table.clear();
			countInsert = 0;
			countUpdate = 0;
			countDelete = 0;
			for( count = 0; count < 15; ++count )
			{
				if( 0 == ( count % 2 ) )
				{
					yit = table.insert( basar::FOR_DELETE );
					yit.setInt32( PROPDEF_COLSER, count );
					++countDelete;
				}
				else if( 0 == ( count % 3 ) )
				{
					yit = table.insert( basar::FOR_UPDATE );
					yit.setInt32( PROPDEF_COLSER, count );
					++countUpdate;
				}
				else
				{
					yit = table.insert( basar::FOR_INSERT );
					yit.setInt32( PROPDEF_COLSER, count );
					++countInsert;
				}
			}

			// count SS_INSERT
			testStateCount( table, basar::SS_INSERT, countInsert );

			// count SS_DELETE
			testStateCount( table, basar::SS_DELETE, countDelete );

			// count SS_UPDATE
			testStateCount( table, basar::SS_UPDATE, countUpdate );
		}
	}
    catch( basar::Exception & e )
    {
        std::cout << e.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK( ExceptionOccured );
        throw;
    }
}

// ------------------------------------------------------------------------------------------------
// erase 1st fetched row (2nd row hasn't been fetched) and check return YIterator
TEST(PropTbl_erase_DemandCaching_1_row_after1RowFetched)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_1_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100001, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2001, yit.getInt16(PROPDEF_COLSMINT));

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(yit);
		CHECK_EQUAL(0, yitSucc.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK(!yitSucc.isEnd());
		CHECK_EQUAL(100002, yitSucc.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2002, yitSucc.getInt16(PROPDEF_COLSMINT));
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase 2nd fetched row (2 rows have been fetched) and check return YIterator
TEST(PropTbl_erase_DemandCaching_lastrow_after2RowsFetched)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_1_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100001, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2001, yit.getInt16(PROPDEF_COLSMINT));

		++yit;
		CHECK_EQUAL(1, yit.getRowNumber());
		CHECK_EQUAL(2, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100002, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2002, yit.getInt16(PROPDEF_COLSMINT));

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(yit);

		CHECK_EQUAL(1, yitSucc.getRowNumber());
		CHECK_EQUAL(2, (int)proptbl.size());
		CHECK(!yitSucc.isEnd());
		CHECK_EQUAL(100003, yitSucc.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2003, yitSucc.getInt16(PROPDEF_COLSMINT));
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase 1st fetched row (2 rows have been fetched) and check return YIterator
TEST(PropTbl_erase_DemandCaching_firstrow_after2RowsFetched)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_1_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100001, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2001, yit.getInt16(PROPDEF_COLSMINT));

		++yit;
		CHECK_EQUAL(1, yit.getRowNumber());
		CHECK_EQUAL(2, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100002, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2002, yit.getInt16(PROPDEF_COLSMINT));

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(proptbl.begin());

		CHECK_EQUAL(0, yitSucc.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK(!yitSucc.isEnd());
		CHECK(yitSucc == yit);
		CHECK_EQUAL(100002, yitSucc.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2002, yitSucc.getInt16(PROPDEF_COLSMINT));
		CHECK_EQUAL(100002, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2002, yit.getInt16(PROPDEF_COLSMINT));
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase last fetched row (all 10 rows have been fetched, no more following rows) and check 
// return YIterator
TEST(PropTbl_erase_DemandCaching_lastrow_afterAllRowsFetched)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_3_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());

		for (Int16 i = 0; yit != proptbl.end(); ++i, ++yit)
		{
			CHECK_EQUAL(100001 + i, yit.getInt32(PROPDEF_COLINT  ));
			CHECK_EQUAL(  2001 + i, yit.getInt16(PROPDEF_COLSMINT));
			CHECK_EQUAL(         i, yit.getRowNumber());
		}

		CHECK_EQUAL(   -99, yit.getRowNumber());
		CHECK_EQUAL(    10, (int)proptbl.size());
		CHECK      (yit.isEnd());

		--yit;
		CHECK_EQUAL(100010, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2010, yit.getInt16(PROPDEF_COLSMINT));
		CHECK_EQUAL(     9, yit.getRowNumber());
		CHECK_EQUAL(    10, (int)proptbl.size());
		CHECK      (!yit.isEnd());

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(yit);
		CHECK_EQUAL(   -99, yitSucc.getRowNumber());
		CHECK_EQUAL(     9, (int)proptbl.size());
		CHECK(yitSucc.isEnd());
		CHECK(yitSucc != yit);
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase 2 fetched rows (following rows haven't been fetched) and check return YIterator
TEST(PropTbl_range_erase_DemandCaching_2_row)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_3_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());

		for (Int16 i = 0; i < 2; ++i)
		{
			CHECK_EQUAL(100001 + i, yit.getInt32(PROPDEF_COLINT  ));
			CHECK_EQUAL(  2001 + i, yit.getInt16(PROPDEF_COLSMINT));
			++yit;
		}

		CHECK_EQUAL(     2, yit.getRowNumber());
		CHECK_EQUAL(     3, (int)proptbl.size());
		CHECK      (!yit.isEnd());
		CHECK_EQUAL(100003, yit.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2003, yit.getInt16(PROPDEF_COLSMINT));

		CHECK_EQUAL(0, proptbl.begin().getRowNumber());

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(proptbl.begin(), yit);
		CHECK_EQUAL(     0, yitSucc.getRowNumber());
		CHECK_EQUAL(     1, (int)proptbl.size());
		CHECK(!yitSucc.isEnd());
		CHECK(yitSucc == yit);
		CHECK_EQUAL(100003, yitSucc.getInt32(PROPDEF_COLINT  ));
		CHECK_EQUAL(  2003, yitSucc.getInt16(PROPDEF_COLSMINT));
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase 10 (all) fetched rows (no more following rows) and check return YIterator
TEST(PropTbl_range_erase_DemandCaching_10_row)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_DemandCaching_3_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::ON_DEMAND_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(1, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(1, (int)proptbl.size());
		CHECK      (!yit.isEnd());

		for (Int16 i = 0; yit != proptbl.end(); ++i, ++yit)
		{
			CHECK_EQUAL(100001 + i, yit.getInt32(PROPDEF_COLINT  ));
			CHECK_EQUAL(  2001 + i, yit.getInt16(PROPDEF_COLSMINT));
		}

		CHECK_EQUAL(   -99, yit.getRowNumber());
		CHECK_EQUAL(    10, (int)proptbl.size());
		CHECK      (yit.isEnd());

		CHECK_EQUAL(0, proptbl.begin().getRowNumber());

		AccessorPropertyTable_YIterator yitSucc = proptbl.erase(proptbl.begin(), yit);
		CHECK_EQUAL(   -99, yitSucc.getRowNumber());
		CHECK_EQUAL(     0, (int)proptbl.size());
		CHECK(yitSucc.isEnd());
		CHECK(yitSucc == yit);
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

// ------------------------------------------------------------------------------------------------
// erase 10 (all) fetched rows (no more following rows) and check return YIterator
TEST(PropTbl_single_erase_FullCaching_10_row_size_checking)
{
    ConnectionRef conn = TestConnection::createDefaultConnectionRef();

	const int max_rows = 10;

    try
    {
        AccessorInstanceRef accinst = 
			Manager::getInstance().createAccessorInstance(  "PropTbl_erase_FullCaching_3_row",
															ACCESSORNAME_ButMasterTableReader_Int, 
															conn,
															basar::db::aspect::FULL_CACHING);

        ExecuteResultInfo resinfo = accinst.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, resinfo.getError());
        CHECK_EQUAL(0, (int)resinfo.getAffectedRows());

        AccessorPropertyTableRef proptbl = accinst.getPropertyTable();
		CHECK_EQUAL(max_rows, (int)proptbl.size());

		AccessorPropertyTable_YIterator yit = proptbl.begin();
		CHECK_EQUAL(0, yit.getRowNumber());
		CHECK_EQUAL(max_rows, (int)proptbl.size());
		CHECK      (!yit.isEnd());

		for (Int16 i = 0; yit != proptbl.end(); ++i, ++yit)
		{
			CHECK_EQUAL(100001 + i, yit.getInt32(PROPDEF_COLINT  ));
			CHECK_EQUAL(  2001 + i, yit.getInt16(PROPDEF_COLSMINT));
		}

		CHECK_EQUAL(   -99, yit.getRowNumber());
		CHECK_EQUAL(    max_rows, (int)proptbl.size());
		CHECK      (yit.isEnd());

		CHECK_EQUAL(0, proptbl.begin().getRowNumber());

		

		int row_size = max_rows;

		while(proptbl.begin() != proptbl.end())
		{	
			proptbl.erase(proptbl.begin());

			--row_size;

			CHECK_EQUAL(row_size, (int)proptbl.size());
		}

		CHECK_EQUAL(0, (int)proptbl.size());

    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }

    conn.close();
}

TEST(ProbTbl_empty)
{
    //execute select returning no rows -> probTbl should have empty() == true in all caching policies
    std::vector<basar::db::aspect::CachingPolicyEnum > caching_test;
    caching_test.push_back( basar::db::aspect::FULL_CACHING );
    caching_test.push_back( basar::db::aspect::ON_DEMAND_CACHING );
    caching_test.push_back( basar::db::aspect::SINGLE_ROW_CACHING );
    caching_test.push_back( basar::db::aspect::NO_CACHING );

    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
    try
    {
        std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef> accessorInstanceRefMap;

        std::vector<basar::db::aspect::CachingPolicyEnum >::const_iterator itCachingPolicy;
        for (itCachingPolicy = caching_test.begin(); itCachingPolicy != caching_test.end(); ++itCachingPolicy)
        {
            AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance(
                "cachingTest1",
                ACCESSORNAME_ButMasterTableReader_All_no_results, 
                connRef,
                *itCachingPolicy);
            accessorInstanceRefMap[*itCachingPolicy] = ai;
        }

        std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef>::iterator itAccessors;
        for (itAccessors = accessorInstanceRefMap.begin(); itAccessors != accessorInstanceRefMap.end(); ++itAccessors)
        { 
            AccessorPropertyTableRef aptr = itAccessors->second.getPropertyTable();
            CHECK(aptr.empty());
            ExecuteResultInfo executeresultinfo = itAccessors->second.execute(ACCESSMETHOD_ButMasterTableReader_All_no_results);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

            CHECK_EQUAL(0, (int)aptr.size() );

            CHECK(aptr.empty());
            CHECK(aptr.isEmpty());
        }

    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

TEST(ProbTbl_empty_after_erase)
{
    //execute select returning no rows -> probTbl should have empty() == true in all caching policies
    std::vector<basar::db::aspect::CachingPolicyEnum > caching_test;
    caching_test.push_back( basar::db::aspect::FULL_CACHING );
    caching_test.push_back( basar::db::aspect::ON_DEMAND_CACHING );
    caching_test.push_back( basar::db::aspect::SINGLE_ROW_CACHING );
    caching_test.push_back( basar::db::aspect::NO_CACHING );

    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
    try
    {
        std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef> accessorInstanceRefMap;

        std::vector<basar::db::aspect::CachingPolicyEnum >::const_iterator itCachingPolicy;
        for (itCachingPolicy = caching_test.begin(); itCachingPolicy != caching_test.end(); ++itCachingPolicy)
        {
            AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance(
                "cachingTest1",
                ACCESSORNAME_ButMasterTableReader_All_one_result, 
                connRef,
                *itCachingPolicy);
            accessorInstanceRefMap[*itCachingPolicy] = ai;
        }

        std::map<basar::db::aspect::CachingPolicyEnum, AccessorInstanceRef>::iterator itAccessors;
        for (itAccessors = accessorInstanceRefMap.begin(); itAccessors != accessorInstanceRefMap.end(); ++itAccessors)
        { 
            AccessorPropertyTableRef aptr = itAccessors->second.getPropertyTable();
            CHECK(aptr.empty());
            ExecuteResultInfo executeresultinfo = itAccessors->second.execute(ACCESSMETHOD_ButMasterTableReader_All_one_result);
            CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
            CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());
            CHECK(!aptr.empty());
            CHECK(!aptr.isEmpty());

            if (basar::db::aspect::NO_CACHING == itAccessors->first)
            {
                CHECK_EQUAL(0, (int)aptr.size() );
            }
            else
            {
                CHECK_EQUAL(1, (int)aptr.size() );
            }

            if ( basar::db::aspect::SINGLE_ROW_CACHING == itAccessors->first || basar::db::aspect::NO_CACHING == itAccessors->first )
            {
                CHECK_THROW( aptr.erase( aptr.begin() ), basar::db::aspect::InvalidMethodCallException );
            }
            else
            {
                aptr.erase( aptr.begin() );

                CHECK_EQUAL(0, (int)aptr.size() );

                CHECK(aptr.empty());
                CHECK(aptr.isEmpty());
            }
        }

    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

// ------------------------------------------------------------------------------------------------
}
} // end namespace db_aspect_unit_test

#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

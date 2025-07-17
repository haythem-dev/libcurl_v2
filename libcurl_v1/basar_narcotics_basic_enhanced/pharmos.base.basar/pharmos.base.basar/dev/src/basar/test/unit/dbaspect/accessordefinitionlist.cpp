/** $Id$                                          
*                                                  
* @file AccessorDefinitionList.cpp                                                                   
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"
#include "libbasardbaspect_exceptions.h"

#include "testconnection.h"


using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;

#define TESTDBASPECT_ACCESSORDEFINITIONLIST
#ifdef TESTDBASPECT_ACCESSORDEFINITIONLIST

//------------------------------------------------------------------------------
SUITE(DBAspect_AccessorDefinitionList)
{
	// -----------------------------------------------------------------------------------------------------------------
	TEST (DBAspect_AccessorDefinitionList_AccessorDefinitionList_sizeIterator)
	{
		AccessorDefinitionListRef adlr = Manager::getInstance().getAccessorDefinitionList();
		unsigned int size = (unsigned int ) adlr.size();
        AccessorDefinitionList_Iterator iter1 = adlr.begin();
        CHECK(iter1.isBegin());
		if (0 == size)
		{
			CHECK(iter1.isEnd());
			CHECK( adlr.begin() ==  adlr.end());
		}
		AccessorDefinitionRef adr = Manager::getInstance().createAccessorDefinition("Bsp1");
		size++;
		CHECK_EQUAL(size, ((unsigned int) adlr.size()));
		AccessorDefinitionRef adr1 = Manager::getInstance().createAccessorDefinition("Bsp2");
		size++;
		CHECK_EQUAL(size, ((unsigned int) adlr.size()));
		Manager::getInstance().createAccessorDefinition("Bsp3");
		CHECK_EQUAL(++size, ((unsigned int) adlr.size()));

		// iterator:
		AccessorDefinitionList_Iterator iter = adlr.begin();
		CHECK(iter.isBegin());
		CHECK(!iter.isEnd());
		CHECK(!iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		iter = adlr.end();
		CHECK(iter.isEnd());
		CHECK(!iter.isNull());
		CHECK(!iter.isBegin());

		// release:
		Manager::getInstance().releaseAccessorDefinition("Bsp1");
		size--;
		CHECK_EQUAL(size, ((unsigned int) adlr.size()));
		Manager::getInstance().releaseAccessorDefinition("Bsp2");
		size--;
		CHECK_EQUAL(size, ((unsigned int) adlr.size()));
		Manager::getInstance().releaseAccessorDefinition("Bsp3");
		size--;
		CHECK_EQUAL(size, ((unsigned int) adlr.size()));

		CHECK_THROW(Manager::getInstance().releaseAccessorDefinition("nonsense"), UnknownAccessorDefinitionException);

	}

	// -----------------------------------------------------------------------------------------------------------------
 	TEST(DBAspect_AccessorDefinitionList_AccessorDefinitionList_AccessorPropertyTableRef_YIterator)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

		try
		{
			AccessorDefinitionRef ard1 = Manager::getInstance().createAccessorDefinition("test1_AccessorDefinitionRef");
			AccessorDefinitionRef ard2 = Manager::getInstance().createAccessorDefinition("test2_AccessorDefinitionRef");

			PropertyDescriptionListRef pdl1 = ard1.getPropertyDescriptionList();
			PropertyDescriptionListRef pdl2 = ard2.getPropertyDescriptionList();

			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1_AccessorInstanceRef",
				"test1_AccessorDefinitionRef", 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test2_AccessorInstanceRef",
				"test2_AccessorDefinitionRef", 
				connRef,
				basar::db::aspect::FULL_CACHING);

			//Initializing first AccessorPropertyTable
			AccessorPropertyTableRef aptr1 = ai1.getPropertyTable();
			AccessorPropertyTableRef aptr2 = ai2.getPropertyTable();

			// Iterator:
			AccessorPropertyTable_YIterator iter;
			iter = aptr1.end();
			CHECK(iter.isEnd());
			CHECK_THROW(iter++, basar::OutOfRangeIteratorException);

			iter = aptr1.begin();
			CHECK(iter.isBegin());
			CHECK_THROW(iter--, basar::OutOfRangeIteratorException);

			CHECK(aptr1.begin() == aptr1.end());
			CHECK(aptr1.begin().isBegin());
			CHECK(aptr1.end().isEnd());

			AccessorPropertyTable_YIterator iter2 = aptr2.begin();
			AccessorPropertyTable_YIterator iter1 = aptr1.begin();

// obsolete
//			CHECK_THROW(iter1 == iter2, basar::DifferentContainersIteratorException);
			CHECK(iter1 == iter1);
			CHECK(iter2 == iter2);
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}

		connRef.close();
	}
	
	// -----------------------------------------------------------------------------------------------------------------
	TEST (DBAspect_AccessorDefinition_create)
	{
		AccessorDefinitionListRef accdeflist = Manager::getInstance().getAccessorDefinitionList();
		AccessorListSize          no         = accdeflist.size();

		AccessorDefinitionRef     accdef1    = Manager::getInstance().createAccessorDefinition("Bsp1");
		CHECK_EQUAL(++no, (unsigned)accdeflist.size());

		AccessorDefinitionRef     accdef2    = Manager::getInstance().createAccessorDefinition("Bsp2");
		CHECK_EQUAL(++no, (unsigned)accdeflist.size());

		CHECK_THROW(Manager::getInstance().createAccessorDefinition("Bsp1"), ExistAccessorDefinitionException);
		CHECK_THROW(Manager::getInstance().createAccessorDefinition("Bsp2"), ExistAccessorDefinitionException);
	}

// -----------------------------------------------------------------------------------------------------------------
}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

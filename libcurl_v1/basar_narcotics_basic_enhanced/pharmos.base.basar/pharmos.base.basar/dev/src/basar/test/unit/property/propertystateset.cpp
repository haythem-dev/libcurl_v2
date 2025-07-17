/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"

using  basar::property::PropertyDescriptionListRef;
using  basar::property::PropertyDescriptionList;
using  basar::property::PropertyDescription;
using  basar::property::PropertyType;
using  basar::property::PropertyDescriptionVector;
using  basar::property::PropertyStateSet;
using  basar::property::PropertyTableRef;
using  basar::property::PropertyTable;
using  basar::property::PropertyTable_YIterator;
using  basar::property::PropertyTableSharedPtr;

//------------------------------------------------------------------------------
SUITE(property_Test_PropertyStateSet)
{
	TEST (property_Test_PropertyStateSet1)
	{
		PropertyStateSet ps;
		CHECK_EQUAL(basar::SS_UNKNOWN, ps.getStates());
		ps.insert(basar::SS_INSERT);
		CHECK_EQUAL(basar::SS_INSERT|basar::SS_UNKNOWN, ps.getStates());
		CHECK(ps.contains(basar::SS_INSERT));
		CHECK(ps.contains(basar::SS_UNKNOWN));
		CHECK(!ps.contains(!basar::SS_DELETE));
		CHECK(!ps.contains(basar::SS_UPDATE));
		CHECK(!ps.contains(basar::SS_CLEAN));
		PropertyStateSet ps2;
		CHECK(ps2.contains(basar::SS_UNKNOWN));
		PropertyStateSet ps3(0);
		CHECK(!ps3.contains(basar::SS_UNKNOWN));
		CHECK(ps.contains(ps2));
		CHECK(!ps.contains(ps3));
	}

	class Fixture
	{
		PropertyDescriptionListRef m_propertydescriptionlistref ;

		PropertyTableRef m_propertytableref  ;

		PropertyStateSet ps;
		PropertyTable_YIterator yiter;
		PropertyTable_YIterator yiter2;

		Fixture():m_propertytableref( PropertyTableSharedPtr())
		{};
		virtual ~Fixture();

		bool init()
		{
			m_propertydescriptionlistref = PropertyDescriptionListRef(PropertyDescriptionList::create() );
			m_propertydescriptionlistref.push_back( "testcol1", basar::INT32  );
			m_propertydescriptionlistref.push_back( "testcol2", basar::INT32 );
			m_propertydescriptionlistref.push_back( "testcol3", basar::INT32 );

			m_propertytableref = PropertyTableRef( PropertyTable::create(m_propertydescriptionlistref) );
			return true;
		}

	};

	TEST (property_Test_PropertyStateSet2)
	{
		try
		{
			basar::property::PropertyDescriptionListRef propertydescriptionlistref( 
				basar::property::PropertyDescriptionList::create() );
			propertydescriptionlistref.push_back( "testcol1", basar::INT32  );
			propertydescriptionlistref.push_back( "testcol2", basar::INT32 );
			propertydescriptionlistref.push_back( "testcol3", basar::INT32 );

			basar::property::PropertyTableRef propertytableref( 
				basar::property::PropertyTable::create(propertydescriptionlistref) );

			PropertyStateSet ps;
			basar::property::PropertyTable_YIterator yiter;
			basar::property::PropertyTable_YIterator yiter2;
			// insert:
			{
				yiter = propertytableref.insert( basar::FOR_INSERT );
				ps = yiter.getState();
				CHECK_EQUAL(basar::SS_UNSET, ps.getStates());
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK_EQUAL( basar::SS_INSERT, ps.getStates());
				//CHECK(ps.contains(basar::SS_UNSET));
				CHECK(ps.contains(basar::SS_INSERT));
				yiter.setInt32("testcol2", 2);
				ps = yiter.getState();
				CHECK_EQUAL( basar::SS_INSERT, ps.getStates());
				CHECK(ps.contains(basar::SS_INSERT));

				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				// error op 201
				{
					CHECK(!ps.contains(basar::SS_INSERT));
					CHECK(ps.contains(basar::SS_CLEAN)); 
					CHECK_EQUAL( basar::SS_CLEAN, ps.getStates());
				}
			}

			// update
			{
				yiter = propertytableref.insert( basar::FOR_UPDATE );
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UPDATE));
				CHECK(!ps.contains(basar::SS_INSERT));

				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				// error op 201
				{
					CHECK(!ps.contains(basar::SS_INSERT));
					CHECK(ps.contains(basar::SS_CLEAN)); 
					CHECK_EQUAL( basar::SS_CLEAN, ps.getStates());
				}
			}

			// insert2
			{
				yiter = propertytableref.insert( basar::FOR_INSERT);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				CHECK(!ps.contains(basar::SS_INSERT));
				// error op 201
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				CHECK(ps.contains(basar::SS_CLEAN)); 

			}

			// insert2
			{
				yiter = propertytableref.insert( basar::FOR_INSERT);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				yiter.setInt32("testcol2", 2);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				yiter.begin().setPropertyState(basar::SS_UPDATE);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(ps.contains(basar::SS_UPDATE)); 

				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				// error op 201
				{
					CHECK(!ps.contains(basar::SS_INSERT));
					CHECK(!ps.contains(basar::SS_UPDATE)); 
					CHECK(ps.contains(basar::SS_CLEAN)); 
				}
			}
		}
		catch (basar::Exception & ex)
		{
			std::cout 
				<< ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}

	} 


	TEST (property_Test_PropertyStateSet3)
	{
		try
		{
			basar::property::PropertyDescriptionListRef propertydescriptionlistref( 
				basar::property::PropertyDescriptionList::create() );
			propertydescriptionlistref.push_back( "testcol1", basar::INT32  );
			propertydescriptionlistref.push_back( "testcol2", basar::INT32 );
			propertydescriptionlistref.push_back( "testcol3", basar::INT32 );

			basar::property::PropertyTableRef propertytableref( 
				basar::property::PropertyTable::create(propertydescriptionlistref) );

			PropertyStateSet ps;
			basar::property::PropertyTable_YIterator yiter;
			basar::property::PropertyTable_YIterator yiter2;
			// insert:
			{
				yiter = propertytableref.insert( basar::FOR_INSERT );
				ps = yiter.getState();
				CHECK_EQUAL(basar::SS_UNSET, ps.getStates());
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK_EQUAL( basar::SS_INSERT, ps.getStates());
				//CHECK(ps.contains(basar::SS_UNSET));
				CHECK(ps.contains(basar::SS_INSERT));
				yiter.setInt32("testcol2", 2);
				ps = yiter.getState();
				CHECK_EQUAL( basar::SS_INSERT, ps.getStates());
				//CHECK(ps.contains(basar::SS_UNSET));
				CHECK(ps.contains(basar::SS_INSERT));
				yiter.begin().setPropertyState(basar::SS_UPDATE);
				ps = yiter.getState();

				CHECK_EQUAL( basar::SS_INSERT|basar::SS_UPDATE, ps.getStates());
				//CHECK(ps.contains(basar::SS_UNSET));
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(ps.contains(basar::SS_UPDATE));
			}

			// update
			{
				yiter = propertytableref.insert( basar::FOR_UPDATE );
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UPDATE));
				CHECK(!ps.contains(basar::SS_INSERT));
			}

			// insert2
			{
				yiter = propertytableref.insert( basar::FOR_INSERT);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				CHECK(!ps.contains(basar::SS_INSERT));
				// error op 201
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				CHECK(ps.contains(basar::SS_CLEAN)); 

			}

			// insert2
			{
				yiter = propertytableref.insert( basar::FOR_INSERT);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_UNSET));
				yiter.setInt32("testcol1", 1);
				yiter.setInt32("testcol2", 2);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(!ps.contains(basar::SS_UPDATE)); 
				yiter.begin().setPropertyState(basar::SS_UPDATE);
				ps = yiter.getState();
				CHECK(ps.contains(basar::SS_INSERT));
				CHECK(ps.contains(basar::SS_UPDATE)); 

				yiter2 = propertytableref.executePerformed(yiter);
				CHECK(yiter2 == yiter);
				ps = yiter.getState();
				// error op 201
				{
					CHECK(!ps.contains(basar::SS_INSERT));
					CHECK(!ps.contains(basar::SS_UPDATE)); 
					CHECK(ps.contains(basar::SS_CLEAN)); 
				}
			}
		}
		catch (basar::Exception & ex)
		{
			std::cout 
				<< ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}

	} 

}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

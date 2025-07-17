/*
* @file                                 
* @author Anke Klink                      
* @date 2008                            
*/    


#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"

using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList;
using basar::property::PropertyDescription;
using basar::property::PropertyType;
using basar::property::PropertyDescriptionVector;
using basar::property::PropertyTable;
using basar::property::PropertyTableRef;
using basar::property::PropertyTable_YIterator;
using basar::property::PropertyTable_XIterator;
using basar::property::NumberRows;
using basar::property::NumberColumns;

//------------------------------------------------------------------------------
SUITE(PropertyTable)
{
// ----------------------------------------------------------
class Fixture
{
public:
    static NumberColumns       NO_COLS;
	static NumberRows		   NO_ROWS;

    PropertyDescriptionListRef m_pdl;
	PropertyTableRef           m_propTbl;

	Fixture() : m_pdl    (PropertyDescriptionList::create()     ),
	            m_propTbl(PropertyTable          ::create(m_pdl))
	{
		//PropertyTable_YIterator::setCheckMode(PropertyTable_YIterator::CHK_ITERATOR);

		m_pdl.push_back( "testcol1", basar::INT32  );
		m_pdl.push_back( "testcol2", basar::STRING );
		m_pdl.push_back( "testcol3", basar::STRING );

		m_propTbl.insert( basar::FOR_INSERT );
		m_propTbl.insert( basar::FOR_INSERT );
		m_propTbl.insert( basar::FOR_INSERT );

		CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());

		{
			PropertyTable_YIterator yit;
			int                     i;

			for (i = 0, yit = m_propTbl.begin(); yit != m_propTbl.end(); ++yit, ++i)
			{
				yit.setInt32("testcol1", i);
			}
		}
	};

	~Fixture()
	{
	};
};

class FixtureForClean
{
public:
    static NumberColumns       NO_COLS;
	static NumberRows		   NO_ROWS;

    PropertyDescriptionListRef m_pdl;
	PropertyTableRef           m_propTbl;

	FixtureForClean() : m_pdl    (PropertyDescriptionList::create()     ),
	            m_propTbl(PropertyTable          ::create(m_pdl))
	{
		//PropertyTable_YIterator::setCheckMode(PropertyTable_YIterator::CHK_ITERATOR);

		m_pdl.push_back( "testcol1", basar::INT32  );
		m_pdl.push_back( "testcol2", basar::STRING );
		m_pdl.push_back( "testcol3", basar::STRING );

		m_propTbl.insert( basar::FOR_CLEAN );
		m_propTbl.insert( basar::FOR_CLEAN );
		m_propTbl.insert( basar::FOR_CLEAN );

		CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());

		{
			PropertyTable_YIterator yit;
			int                     i;

			for (i = 0, yit = m_propTbl.begin(); yit != m_propTbl.end(); ++yit, ++i)
			{
				yit.setInt32("testcol1", i);
			}
		}
	};

	~FixtureForClean()
	{
	};
};

NumberColumns Fixture::NO_COLS = 3;
NumberRows    Fixture::NO_ROWS = 3;

NumberColumns FixtureForClean::NO_COLS = 3;
NumberRows    FixtureForClean::NO_ROWS = 3;

// ----------------------------------------------------------
TEST_FIXTURE(Fixture, property_Test_PropertyTable)
{
    try
    {
        PropertyTable_YIterator yiter = m_propTbl.begin();
        CHECK(yiter.isBegin());
        CHECK(!yiter.isEnd());
        CHECK_EQUAL(((unsigned int)NO_COLS), ((unsigned int)yiter.size()));

        PropertyTable_XIterator xiter = yiter.getXIterator(0);
        CHECK(xiter.isBegin());
        CHECK(!xiter.isEnd());
        
		PropertyTable_XIterator xiter2 = yiter.getXIterator("testcol1");
        CHECK(xiter2.isBegin());
        CHECK(!xiter2.isEnd());
        CHECK_EQUAL(xiter.getName(), xiter2.getName());
        CHECK_EQUAL(static_cast<unsigned int>(xiter.getIndex()), static_cast<unsigned int>(xiter2.getIndex()));

        xiter = yiter.getXIterator(1);
        CHECK(!xiter.isBegin());
        CHECK(!xiter.isEnd());

		xiter2 = yiter.getXIterator("testcol2");
        CHECK(!xiter2.isBegin());
        CHECK(!xiter2.isEnd());
        CHECK_EQUAL(xiter.getName(), xiter2.getName());
        CHECK_EQUAL(static_cast<unsigned int>(xiter.getIndex()), static_cast<unsigned int>(xiter2.getIndex()));

        xiter = yiter.getXIterator(2);
        CHECK(!xiter.isBegin());
        CHECK(!xiter.isEnd());

		xiter2 = yiter.getXIterator("testcol3");
        CHECK(!xiter2.isBegin());
        CHECK(!xiter2.isEnd());
        CHECK_EQUAL(xiter.getName(), xiter2.getName());
        CHECK_EQUAL(static_cast<unsigned int>(xiter.getIndex()), static_cast<unsigned int>(xiter2.getIndex()));

        xiter = yiter.getXIterator(3);
        CHECK(!xiter.isBegin());
        CHECK(xiter.isEnd());

		CHECK_THROW(xiter2 = yiter.getXIterator("rewrew"), basar::Exception);
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

// ----------------------------------------------------------
// erase end() iterator
TEST_FIXTURE(Fixture, Erase_1_end)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	CHECK(!yit.isBegin());
	CHECK( yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	//CHECK_THROW(yit.getInt32("testcol1"), basar::InvalidIteratorException);

	CHECK_THROW(m_propTbl.erase(yit), basar::OutOfRangeIteratorException);
}

// ----------------------------------------------------------
// erase last item
TEST_FIXTURE(Fixture, Erase_1_last)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit);
	CHECK(!yitSucc.isBegin());
	CHECK( yitSucc.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
}

// ----------------------------------------------------------
// erase before last item
TEST_FIXTURE(Fixture, Erase_1_beforelast)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit);
	CHECK(!yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase first item
TEST_FIXTURE(Fixture, Erase_1_first)
{
	PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit);
	CHECK( yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(end(), end())
TEST_FIXTURE(Fixture, Erase_range0_end)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	CHECK(!yit.isBegin());
	CHECK( yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	//CHECK_THROW(yit.getInt32("testcol1"), basar::InvalidIteratorException);

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit);
	CHECK(!yitSucc.isBegin());
	CHECK( yitSucc.isEnd  ());
	CHECK( yitSucc == yit);
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
}

// ----------------------------------------------------------
// erase range(last item, last item)
TEST_FIXTURE(Fixture, Erase_range0_last)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit);
	CHECK(!yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit);
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
}

// ----------------------------------------------------------
// erase range(before last item, before last item)
TEST_FIXTURE(Fixture, Erase_range0_beforelast)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit);
	CHECK(!yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit);
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(first item, first item)
TEST_FIXTURE(Fixture, Erase_range0_first)
{
	PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit);
	CHECK( yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit);
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(last item, end())
TEST_FIXTURE(Fixture, Erase_range1_last)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitEnd  = m_propTbl.end();

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yitEnd);
	CHECK(!yitSucc.isBegin());
	CHECK( yitSucc.isEnd  ());
	CHECK( yitSucc == yitEnd);
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
}

// ----------------------------------------------------------
// erase range(2nd item, last item)
TEST_FIXTURE(Fixture, Erase_range1_beforelast)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yit2  = m_propTbl.end();
	--yit2;

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit2);
	CHECK(!yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit2);
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(first item, 2nd item)
TEST_FIXTURE(Fixture, Erase_range1_first)
{
	PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yit2  = m_propTbl.begin();
	++yit2;

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit2);
	CHECK( yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit2);
	CHECK_EQUAL((unsigned int)NO_ROWS - 1, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(2nd item, end())
TEST_FIXTURE(Fixture, Erase_range2_last)
{
	PropertyTable_YIterator yit = m_propTbl.end();
	--yit;
	--yit;
	CHECK(!yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(1, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitEnd  = m_propTbl.end();

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yitEnd);
	CHECK(!yitSucc.isBegin());
	CHECK( yitSucc.isEnd  ());
	CHECK( yitSucc == yitEnd);
	CHECK_EQUAL((unsigned int)NO_ROWS - 2, (unsigned int)m_propTbl.size());
}

// ----------------------------------------------------------
// erase range(first item, last item)
TEST_FIXTURE(Fixture, Erase_range2_first)
{
	PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yit2  = m_propTbl.end();
	--yit2;

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yit2);
	CHECK( yitSucc.isBegin());
	CHECK(!yitSucc.isEnd  ());
	CHECK( yitSucc == yit2);
	CHECK_EQUAL((unsigned int)NO_ROWS - 2, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(2, yitSucc.getInt32("testcol1"));
}

// ----------------------------------------------------------
// erase range(1st item, end())
TEST_FIXTURE(Fixture, Erase_range3_last)
{
	PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	CHECK(!yit.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS, (unsigned int)m_propTbl.size());
	CHECK_EQUAL(0, yit.getInt32("testcol1"));
	CHECK_EQUAL(basar::SS_INSERT, yit.getState().getStates());

	PropertyTable_YIterator yitEnd  = m_propTbl.end();

	PropertyTable_YIterator yitSucc = m_propTbl.erase(yit, yitEnd);
	CHECK( yitSucc == yitEnd);
	CHECK( yitSucc.isBegin());
	CHECK( yitSucc.isEnd  ());
	CHECK_EQUAL((unsigned int)NO_ROWS - 3, (unsigned int)m_propTbl.size());
}

TEST_FIXTURE(FixtureForClean, States)
{
    PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	
	CHECK_EQUAL(basar::SS_CLEAN, yit.getState().getStates());
	
	yit.setInt32("testcol1", 25);
	
	CHECK_EQUAL(basar::SS_UPDATE, yit.getState().getStates());
	
}

TEST_FIXTURE(FixtureForClean, setPropertyState)
{
    PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	
	CHECK_EQUAL(basar::SS_CLEAN, yit.getState().getStates());
	
    yit.setPropertyState("testcol1", basar::SS_UPDATE);
    
    CHECK_EQUAL(basar::SS_UPDATE, yit.getState().getStates());	
}

TEST_FIXTURE(FixtureForClean, filter)
{
    PropertyTable_YIterator yit = m_propTbl.begin();
	CHECK( yit.isBegin());
	
	CHECK_EQUAL(basar::SS_CLEAN, yit.getState().getStates());
	
	yit = m_propTbl.begin(basar::SS_UPDATE);
	
	CHECK(yit.isEnd());
}

TEST_FIXTURE(Fixture, at)
{
    PropertyTable_YIterator yit = m_propTbl.at(0);

    CHECK( yit.isBegin() );

    CHECK_EQUAL(yit.getInt32( "testcol1" ), 0 );

    yit = m_propTbl.at(2);

    CHECK( false == yit.isEnd() );

    ++yit;

    CHECK( yit.isEnd() );

    yit = m_propTbl.at( 3 );

    CHECK( yit.isEnd() );

    CHECK_THROW( m_propTbl.at( 4 ), basar::OutOfRangeIteratorException );
    CHECK_THROW( m_propTbl.at( -1 ), basar::OutOfRangeIteratorException );

}
// -------------------------------------------------------------------------------------------
}	// SUITE
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

/** $Id$                                          
*                                                  
* @file                                      
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"
 
using basar::InvalidIteratorException;
using basar::NullReferenceException;

using basar::property::PropertyTable_XIterator;
using basar::property::PropertyTable_YIterator;
using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList;
using basar::property::PropertyTableRef ;
using basar::property::PropertyTable;

//------------------------------------------------------------------------------
SUITE(property_TestIterator)
{
	// ----------------------------------------------------------

	TEST (testiterator_PropertyDescriptionList_Iterator)
	{
		basar::property::PdlSharedPtr                     rPdl;
        basar::property::ColNamePropTypeColl              vec;
		basar::property::ColNamePropTypeCollIter          rIt = vec.begin();
		basar::property::PropertyDescriptionList_Iterator iter(rPdl, rIt);

		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		//CHECK_THROW(iter.isBegin(),basar::InvalidIteratorException);
	}
	//// ----------------------------------------------------------

	TEST (testiterator_PropertyTable_XIterator)
	{
		PropertyTable_XIterator iter;

		CHECK(iter.isNull());

		iter.reset();
		CHECK(iter.isNull());

		CHECK_THROW(iter.getString(), InvalidIteratorException);
		CHECK_THROW(iter.isBegin  (), InvalidIteratorException);
		CHECK_THROW(iter.isEnd    (), InvalidIteratorException);
	}

	//// ----------------------------------------------------------

	TEST (testiterator_PropertyTable_YIterator)
	{
		PropertyTable_YIterator iter;

		CHECK(iter.isNull());

		iter.reset();
		CHECK(iter.isNull());

		CHECK_THROW(iter.isBegin(), NullReferenceException);
		CHECK_THROW(iter.isEnd  (), NullReferenceException);
		CHECK_THROW(++iter        , NullReferenceException);
		CHECK_THROW(iter++        , NullReferenceException);
		CHECK_THROW(--iter        , NullReferenceException);
		CHECK_THROW(iter--        , NullReferenceException);
	}

	//// ----------------------------------------------------------

	TEST (testiterator_PropertyTable_beginAndEnd)
	{
		PropertyDescriptionListRef propertydescriptionlistref(PropertyDescriptionList::create());

		propertydescriptionlistref.push_back( "testcol1", basar::INT32  );
		propertydescriptionlistref.push_back( "testcol2", basar::STRING );
		propertydescriptionlistref.push_back( "testcol3", basar::STRING );
		basar::property::NumberColumns numbercolumns = 3;

		PropertyTableRef propertytableref(PropertyTable::create(propertydescriptionlistref) );

		propertytableref.insert( basar::FOR_INSERT );
		propertytableref.insert( basar::FOR_INSERT );
		propertytableref.insert( basar::FOR_INSERT );

		basar::property::NumberRows numberrows = 3;

		CHECK_EQUAL(((unsigned int)numberrows), ((unsigned int) propertytableref.size()));
		
        PropertyTable_YIterator yiter = propertytableref.begin();
		CHECK(yiter.isBegin());
		CHECK(!yiter.isEnd());
		CHECK_EQUAL(((unsigned int)numbercolumns), ((unsigned int)yiter.size()));
		yiter++;
		CHECK(!yiter.isBegin());
		CHECK(!yiter.isEnd());
		yiter = propertytableref.end();
		CHECK(!yiter.isBegin());
		CHECK(yiter.isEnd());
		yiter = propertytableref.begin();
		
        PropertyTable_XIterator xiter = yiter.begin();
		CHECK(xiter.isBegin());
		CHECK(!xiter.isEnd());
		xiter++;
		CHECK(!xiter.isBegin());
		CHECK(!xiter.isEnd());
		xiter = yiter.end();
		CHECK(!xiter.isBegin());
		CHECK(xiter.isEnd());

		yiter = propertytableref.begin();
		xiter = yiter.begin();
		CHECK_THROW(xiter.getInt32(),basar::Exception);
		xiter.setInt32(1);
		CHECK_EQUAL(1, xiter.getInt32());
		CHECK_THROW(xiter.setString("1"),basar::Exception);
	}

	// ----------------------------------------------------------

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

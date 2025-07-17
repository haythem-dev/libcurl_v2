/** $Id$                                          
*                                                  
* @file iterator.cpp                                                                        
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasarproperty.h"

using namespace basar::db::aspect;

#define TESTDDBACPECT_TESTITERATOR
#ifdef TESTDDBACPECT_TESTITERATOR
//------------------------------------------------------------------------------
SUITE(DBAspect_TestIterator)
{
	// ----------------------------------------------------------

	TEST (testiterator_AccessorInstanceList_Iterator)
	{
		AccessorInstanceList_Iterator iter;
		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		CHECK_THROW(iter.isBegin(),basar::InvalidIteratorException);
		CHECK_THROW(iter.isEnd(),basar::InvalidIteratorException);
	}
	// ----------------------------------------------------------

	TEST (testiterator_AccessorDefinitionList_Iterator)
	{
		AccessorDefinitionList_Iterator iter;

		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		CHECK_THROW(iter.isBegin(),basar::InvalidIteratorException);
		CHECK_THROW(iter.isEnd(),basar::InvalidIteratorException);
	}

	// ----------------------------------------------------------

	TEST (testiterator_SQLStringBuilderList_Iterator)
	{
		SQLStringBuilderList_Iterator iter;
		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		CHECK_THROW(iter.isBegin(),basar::InvalidIteratorException);

	}
	// ----------------------------------------------------------

    TEST(DBAcpect_AccessorPropertyTable_Iterator)
	{
        AccessorPropertyTable_YIterator iter1;
        CHECK(iter1.isNull());
        AccessorPropertyTable_XIterator iterX;
        CHECK(iterX.isNull());
    }

}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

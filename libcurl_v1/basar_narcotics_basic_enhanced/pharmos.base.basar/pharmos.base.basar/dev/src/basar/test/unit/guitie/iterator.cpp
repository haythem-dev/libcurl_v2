 /** $Id$                                          
  *                                                  
  * @file iterator.cpp                                                                        
  * @author Anke Klink                      
  * @date 2007                            
  */    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

using namespace basar::gui::tie;

//------------------------------------------------------------------------------
SUITE(GuiTie_TestIterator)
{
	// ----------------------------------------------------------

	TEST (testiterator_GuiPropertyTable_YIteratorr)
	{
		GuiPropertyTable_YIterator iter;
		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		CHECK_THROW(iter++,basar::InvalidIteratorException);
		CHECK_THROW(iter.getXIterator(0),basar::InvalidIteratorException);
		CHECK_THROW(iter.getXIterator("testcolumn"),basar::InvalidIteratorException);


	}
	// ----------------------------------------------------------

	TEST (testiterator_GuiPropertyTable_XIterator)
	{
		GuiPropertyTable_YIterator iter;
		CHECK(iter.isNull());
		iter.reset();
		CHECK(iter.isNull());
		CHECK_THROW(iter++,basar::InvalidIteratorException);

	}

	// ----------------------------------------------------------
	
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

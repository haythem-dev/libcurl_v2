 /** $Id$                                          
  *                                                  
  * @file iterator.cpp                                                                    
  * @author Anke Klink                      
  * @date 2007                            
  */    

#include <UnitTest++/UnitTest++.h>
#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#include "libbasarguitie_viewconnptrold.hpp"
#include "test1vc.h"
#include "test2vc.h"

using  basar::gui::tie::ViewConnPtrOlderVersion;
using  basar::test::unit::guitie::Test1VC;
using  basar::test::unit::guitie::Test2VC;

//------------------------------------------------------------------------------
SUITE(GuiTie_TestViewConnPtrOlderVersion)
{
	// ----------------------------------------------------------

	TEST (testViewConnPtrOlderVersion_ViewConnPtrOlderVersion_creationwithoutcontence)
	{
		ViewConnPtrOlderVersion<int> a;
        // constant needs to be right sided
        CHECK(a.get() == 0);
        CHECK(!(a.get() != 0));
        CHECK(!a);

        CHECK(!(a));
	}
	
		// ----------------------------------------------------------

	TEST (testViewConnPtrOlderVersion_ViewConnPtrOlderVersion_CopyAndCoherence)
	{
		ViewConnPtrOlderVersion<Test1VC> vcp2;
        CHECK(vcp2.get() == 0);
        CHECK(!(vcp2.get() != 0));
        CHECK(!vcp2);
        CHECK(!(vcp2));
		vcp2->use();
        CHECK((vcp2.get() != 0));
        CHECK(vcp2);
		{
		 /* ViewConnPtrOlderVersion<Test1VC> vcp2Copy = vcp2->getViewConnPtrOlderVersion();
          CHECK((vcp2Copy.get() != 0));
          CHECK(vcp2Copy);
		  CHECK(vcp2Copy.get() == vcp2.get());*/
		}

	}
	
TEST(testViewConnPtrOlderVersion_ViewConnPtrOlderVersion_VCfind)
	{
		// creation:
		ViewConnPtrOlderVersion<Test1VC> vcp0;
		ViewConnPtrOlderVersion<Test1VC> vcp0a;
	

		ViewConnPtrOlderVersion<Test1VC> vcp1;
		
		vcp1->use();
		

		ViewConnPtrOlderVersion<Test2VC> vcp2;
		
		vcp2->use();
		
		ViewConnPtrOlderVersion<Test1VC> vcp1a;
		
		vcp1a->use();
		

	    ViewConnPtrOlderVersion<Test1VC> vcp1copy;
		
		vcp1copy = vcp1;
		
		
		// test:
		CHECK( vcp1 == vcp1);
		CHECK( vcp1 == vcp1copy);
		//CHECK( vcp1 != vcp2);
		CHECK( vcp1 != vcp1a);
		CHECK( vcp1 != vcp0);
		CHECK( vcp0 == vcp0);
		CHECK( vcp0 == vcp0a);
	
	}


	// ----------------------------------------------------------
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

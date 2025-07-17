/** $Id$                                          
*                                                  
* @file iterator.cpp                                                                    
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "test1vc.h"
#include "test2vc.h"
#include "test1avc.h"

using  basar::gui::tie::ViewConnPtr;
using  basar::test::unit::guitie::Test1VC;
using  basar::test::unit::guitie::Test2VC;
using  basar::test::unit::guitie::Test1AVC;

//------------------------------------------------------------------------------
SUITE(GuiTie_TestViewConnPtr)
{
	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_creationwithoutcontence)
	{
		ViewConnPtr<int> a;
		// constant needs to be right sided
		CHECK(a.get() == 0);
		CHECK(!(a.get() != 0));
		CHECK(!a);
		CHECK(!(a));
	}

	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_CopyAndCoherence)
	{
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);

		vcp2->use();
		CHECK((vcp2.get() != 0));
		CHECK(vcp2);
		CHECK(vcp2.getEntry() != 0);
		CHECK(vcp2.getParent() == 0);

		int n = 1;
		CHECK(vcp2.unique());
		CHECK_EQUAL(n,  vcp2.use_count());
		{
			ViewConnPtr<Test1VC> vcp2Copy = vcp2->getViewConnPtr();
			CHECK((vcp2Copy.get() != 0));
			CHECK(vcp2Copy);
			CHECK(vcp2Copy.get() == vcp2.get());

			CHECK(!vcp2.unique());
			CHECK_EQUAL(n+1,  vcp2.use_count());

			CHECK(vcp2 == vcp2Copy);
		}

		CHECK(vcp2.unique());
		CHECK_EQUAL(n,  vcp2.use_count());

		ViewConnPtr<Test1VC> vcp3;
		vcp3 = vcp2;
		CHECK((vcp3.get() != 0));
		CHECK(vcp3);
		CHECK(vcp3.get() == vcp2.get());

		CHECK(!vcp2.unique());
		CHECK_EQUAL(++n,  vcp2.use_count());
			CHECK(vcp2 == vcp3);

		ViewConnPtr<Test1VC> vcp4(vcp2);
		CHECK((vcp4.get() != 0));
		CHECK(vcp4);
		CHECK(vcp4.get() == vcp2.get());
			CHECK(vcp2 == vcp4);

		CHECK(!vcp2.unique());
		CHECK_EQUAL(++n,  vcp2.use_count());

		ViewConnPtr<Test1VC> vcp5(vcp2.get());
		CHECK((vcp5.get() != 0));
		CHECK(vcp5);
		CHECK(vcp5.get() == vcp2.get());
			CHECK(vcp2 == vcp5);

		CHECK(!vcp5.unique());
		CHECK_EQUAL(++n,  vcp5.use_count());


	}

	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage1)
	{
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));

		ViewConnPtr<Test1VC> vcp1;
		CHECK(vcp1.get() == 0);
		CHECK(!(vcp1.get() != 0));
		CHECK(!vcp1);
		CHECK(!(vcp1));

		vcp2.setParent(vcp1.getEntry());

		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		CHECK(vcp2.getParent() == vcp1.getEntry());

		vcp1.initialize();
		CHECK((vcp1.get() != 0));
		CHECK(vcp1);

		vcp2.setParent(vcp1.getEntry());

		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() != 0);
		CHECK(vcp2.getParent() == vcp1.getEntry());

		vcp2->use();

		CHECK((vcp2.get() != 0));
		CHECK(vcp2);
		CHECK(vcp2->parent() ==  vcp2.getParent());

		CHECK(vcp2.getEntry() != 0);
		CHECK(vcp2.getParent() != 0);
		CHECK(vcp2.getParent() == vcp1.getEntry());

		CHECK(vcp2.getEntry() != 0);
		CHECK(vcp2.getParent() != 0);
		CHECK(vcp2.getParent() == vcp1.getEntry());

		{
			ViewConnPtr<Test1VC> vcp2Copy = vcp2->getViewConnPtr();
			CHECK((vcp2Copy.get() != 0));
			CHECK(vcp2Copy);
			CHECK(vcp2Copy.get() == vcp2.get());
		}

		ViewConnPtr<Test1VC> vcp3 = vcp2;
		CHECK((vcp3.get() != 0));
		CHECK(vcp3);
		CHECK(vcp3.get() == vcp2.get());
	}

	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_youngerParent1)
	{
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		{
			ViewConnPtr<Test1VC> vcp1;
			CHECK(vcp1.get() == 0);
			CHECK(!(vcp1.get() != 0));
			CHECK(!vcp1);
			CHECK(!(vcp1));

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() == 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp1.initialize();
			CHECK((vcp1.get() != 0));
			CHECK(vcp1);

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp2->use();
			CHECK((vcp2.get() != 0));
			CHECK(vcp2);

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());
			{
				ViewConnPtr<Test1VC> vcp2Copy = vcp2->getViewConnPtr();
				CHECK((vcp2Copy.get() != 0));
				CHECK(vcp2Copy);
				CHECK(vcp2Copy.get() == vcp2.get());
			}
			CHECK(vcp2.unique());
			CHECK(1 == vcp2.use_count());
		}
		// removed entry and parent ?
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		CHECK(vcp2.get() == 0);

		CHECK(!vcp2.unique());
		CHECK(0 == vcp2.use_count());
		//CHECK(vcp2.getParent() == vcp1.getEntry());
	}



	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_youngerParent2)
	{
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		{
			ViewConnPtr<Test1VC> vcp3;
			CHECK(vcp3.get() == 0);
			CHECK(!(vcp3.get() != 0));
			CHECK(!vcp3);
			CHECK(!(vcp3));
			vcp3.initialize();
			CHECK((vcp3.get() != 0));
			CHECK(vcp3);
			{
				ViewConnPtr<Test1VC> vcp1;
				CHECK(vcp1.get() == 0);
				CHECK(!(vcp1.get() != 0));
				CHECK(!vcp1);
				CHECK(!(vcp1));

				vcp2.setParent(vcp1.getEntry());

				CHECK(vcp2.getEntry() == 0);
				CHECK(vcp2.getParent() == 0);
				CHECK(vcp2.getParent() == vcp1.getEntry());

				vcp1.initialize();
				CHECK((vcp1.get() != 0));
				CHECK(vcp1);

				vcp2.setParent(vcp1.getEntry());

				CHECK(vcp2.getEntry() == 0);
				CHECK(vcp2.getParent() != 0);
				CHECK(vcp2.getParent() == vcp1.getEntry());

				vcp2->use();
				CHECK((vcp2.get() != 0));
				CHECK(vcp2);

				CHECK(vcp2.getEntry() != 0);
				CHECK(vcp2.getParent() != 0);
				CHECK(vcp2.getParent() == vcp1.getEntry());

				CHECK(vcp2.getEntry() != 0);
				CHECK(vcp2.getParent() != 0);
				CHECK(vcp2.getParent() == vcp1.getEntry());
				{
					ViewConnPtr<Test1VC> vcp2Copy = vcp2->getViewConnPtr();
					CHECK((vcp2Copy.get() != 0));
					CHECK(vcp2Copy);
					CHECK(vcp2Copy.get() == vcp2.get());
				}
				vcp2.setParent(vcp3.getWidgetPtr());
			}
			// removed entry and parent ?
			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.get() != 0);

			CHECK(vcp2.unique());
			CHECK(1 == vcp2.use_count());
		}
		// removed entry and parent ?
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		CHECK(vcp2.get() == 0);

		CHECK(!vcp2.unique());
		CHECK(0 == vcp2.use_count());
	}

	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_copy)
	{
		ViewConnPtr<Test1VC> vcp1;
		CHECK(vcp1.get() == 0);
		CHECK(!(vcp1.get() != 0));
		CHECK(!vcp1);
		CHECK(!(vcp1));
		ViewConnPtr<Test1VC> vcp2Copy;
		{
			ViewConnPtr<Test1VC> vcp2;
			CHECK(vcp2.get() == 0);
			CHECK(!(vcp2.get() != 0));
			CHECK(!vcp2);
			CHECK(!(vcp2));

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() == 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp1.initialize();
			CHECK((vcp1.get() != 0));
			CHECK(vcp1);

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp2->use();
			vcp2Copy = vcp2;
			CHECK((vcp2.get() != 0));
			CHECK(vcp2);

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());
			{
				ViewConnPtr<Test1VC> vcp2Copy2 = vcp2->getViewConnPtr();
				CHECK((vcp2Copy2.get() != 0));
				CHECK(vcp2Copy2);
				CHECK(vcp2Copy2.get() == vcp2.get());
			}
			//vcp2.setParent(vcp3.getWidgetPtr());			
		}
		// removed entry and parent ?
		CHECK(vcp2Copy.getEntry() != 0);
		CHECK(vcp2Copy.getParent() != 0);
		CHECK(vcp2Copy.get() != 0);
	}
	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_copy2)
	{
		ViewConnPtr<Test1VC> vcp1;
		CHECK(vcp1.get() == 0);
		CHECK(!(vcp1.get() != 0));
		CHECK(!vcp1);
		CHECK(!(vcp1));
		boost::shared_ptr<Test1VC> vcp2Copy;
		{
			ViewConnPtr<Test1VC> vcp2;
			CHECK(vcp2.get() == 0);
			CHECK(!(vcp2.get() != 0));
			CHECK(!vcp2);
			CHECK(!(vcp2));

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() == 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp1.initialize();
			CHECK((vcp1.get() != 0));
			CHECK(vcp1);

			vcp2.setParent(vcp1.getEntry());

			CHECK(vcp2.getEntry() == 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			vcp2->use();
			vcp2Copy = vcp2.get();
			CHECK((vcp2.get() != 0));
			CHECK(vcp2);

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());

			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.getParent() == vcp1.getEntry());
			{
				ViewConnPtr<Test1VC> vcp2Copy2 = vcp2->getViewConnPtr();
				CHECK((vcp2Copy2.get() != 0));
				CHECK(vcp2Copy2);
				CHECK(vcp2Copy2.get() == vcp2.get());
			}
			//vcp2.setParent(vcp3.getWidgetPtr());			
		}
		// removed entry and parent ?
		CHECK(vcp2Copy != 0);
	}


	// ----------------------------------------------------------
	TEST(testviewconnptr_viewconnptr_VCfind)
	{
		// creation:
		ViewConnPtr<Test1VC> vcp0;
		ViewConnPtr<Test1VC> vcp0a;
		CHECK(vcp0.get() == 0);
		CHECK(!(vcp0.get() != 0));
		CHECK(!vcp0);
		CHECK(!(vcp0));
		CHECK(vcp0a.get() == 0);
		CHECK(!(vcp0a.get() != 0));
		CHECK(!vcp0a);
		CHECK(!(vcp0a));

		ViewConnPtr<Test1VC> vcp1;
		CHECK(vcp1.get() == 0);
		CHECK(!(vcp1.get() != 0));
		CHECK(!vcp1);
		CHECK(!(vcp1));
		CHECK(vcp1.getEntry() == 0);
		CHECK(vcp1.getParent() == 0);
		vcp1->use();
		CHECK(vcp1.getEntry() != 0);
		CHECK(vcp1.getParent() == 0);

		ViewConnPtr<Test2VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		vcp2->use();
		CHECK(vcp2.getEntry() != 0);
		CHECK(vcp2.getParent() == 0);

		ViewConnPtr<Test1AVC> vcpChildA;
		CHECK(vcpChildA.get() == 0);
		CHECK(!(vcpChildA.get() != 0));
		CHECK(!vcpChildA);
		CHECK(!(vcpChildA));
		CHECK(vcpChildA.getEntry() == 0);
		CHECK(vcpChildA.getParent() == 0);
		vcpChildA->use();
		CHECK(vcpChildA.getEntry() != 0);
		CHECK(vcpChildA.getParent() == 0);

		ViewConnPtr<Test1VC> vcp1a;
		CHECK(vcp1a.get() == 0);
		CHECK(!(vcp1a.get() != 0));
		CHECK(!vcp1a);
		CHECK(!(vcp1a));
		CHECK(vcp1a.getEntry() == 0);
		CHECK(vcp1a.getParent() == 0);
		vcp1a->use();
		CHECK(vcp1a.getEntry() != 0);
		CHECK(vcp1a.getParent() == 0);

	    ViewConnPtr<Test1VC> vcp1copy;
		CHECK(vcp1copy.get() == 0);
		CHECK(!(vcp1copy.get() != 0));
		CHECK(!vcp1copy);
		CHECK(!(vcp1copy));
		CHECK(vcp1copy.getEntry() == 0);
		CHECK(vcp1copy.getParent() == 0);
		vcp1copy = vcp1;
		CHECK(vcp1copy.getEntry() != 0);
		CHECK(vcp1copy.getParent() == 0);
		
		// test:
		CHECK( vcp1 == vcp1);
		CHECK( vcp1 == vcp1copy);
		//CHECK( vcp1 != vcp2);
		CHECK( vcp1 != vcp1a);
		CHECK( vcp1 != vcp0);
		CHECK( vcp0 == vcp0);
		CHECK( vcp0 == vcp0a);
		CHECK( vcp1 != vcpChildA);
	
	}


	// ----------------------------------------------------------
	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_youngerParent1_innerParent)
	{
		 
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		{
			ViewConnPtr<Test1VC> vcp1;
			CHECK(vcp1.get() == 0);
			CHECK(!(vcp1.get() != 0));
			CHECK(!vcp1);
			CHECK(!(vcp1));

vcp2->use();
		
			vcp1.initialize();
			CHECK((vcp1.get() != 0));
			CHECK(vcp1);

			vcp2.getWidgetPtr()->setParent(vcp1.getEntry());


			CHECK((vcp2.get() != 0));
			CHECK(vcp2);
			
			{
				ViewConnPtr<Test1VC> vcp2Copy = vcp2->getViewConnPtr();
				CHECK((vcp2Copy.get() != 0));
				CHECK(vcp2Copy);
				CHECK(vcp2Copy.get() == vcp2.get());
			}
			CHECK(vcp2.unique());
			CHECK(1 == vcp2.use_count());
		}
		// removed entry and parent ?
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		CHECK(vcp2.get() == 0);

		CHECK(!vcp2.unique());
		CHECK(0 == vcp2.use_count());
		//CHECK(vcp2.getParent() == vcp1.getEntry());
	}



	// ----------------------------------------------------------
	TEST (testviewconnptr_viewconnptr_Parentage_youngerParent2_inner)
	{
		ViewConnPtr<Test1VC> vcp2;
		CHECK(vcp2.get() == 0);
		CHECK(!(vcp2.get() != 0));
		CHECK(!vcp2);
		CHECK(!(vcp2));
		{
			ViewConnPtr<Test1VC> vcp3;
			CHECK(vcp3.get() == 0);
			CHECK(!(vcp3.get() != 0));
			CHECK(!vcp3);
			CHECK(!(vcp3));
			vcp3.initialize();
			CHECK((vcp3.get() != 0));
			CHECK(vcp3);
			{
				ViewConnPtr<Test1VC> vcp1;
				CHECK(vcp1.get() == 0);
				CHECK(!(vcp1.get() != 0));
				CHECK(!vcp1);
				CHECK(!(vcp1));
 
				vcp1.initialize();
				CHECK((vcp1.get() != 0));
				CHECK(vcp1);

                vcp2->use();
				vcp2.getWidgetPtr()->setParent(vcp1.getEntry());

			
				vcp2.setParent(vcp3.getWidgetPtr());
			}
			// removed entry and parent ?
			CHECK(vcp2.getEntry() != 0);
			CHECK(vcp2.getParent() != 0);
			CHECK(vcp2.get() != 0);

			CHECK(vcp2.unique());
			CHECK(1 == vcp2.use_count());
		}
		// removed entry and parent ?
		CHECK(vcp2.getEntry() == 0);
		CHECK(vcp2.getParent() == 0);
		CHECK(vcp2.get() == 0);

		CHECK(!vcp2.unique());
		CHECK(0 == vcp2.use_count());
	}

}
// ----------------------------------------------------------

	
// ----------------------------------------------------------


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

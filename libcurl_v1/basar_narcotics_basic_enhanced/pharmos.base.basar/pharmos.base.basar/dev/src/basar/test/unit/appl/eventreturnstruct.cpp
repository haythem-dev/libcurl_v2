 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2009                            
  */    

 #include <UnitTest++/UnitTest++.h>

#include "libbasarappl.h"

using namespace basar::appl;


//------------------------------------------------------------------------------
SUITE(BasarAppl_TestEventReturnStruct)
{
	// ----------------------------------------------------------

	TEST (BasarAppl_TestEventReturnStruct_test1)
	{
		basar::appl::EventReturnStruct ers1;
		CHECK(basar::appl::HANDLER_OK == ers1.ret);
		CHECK(ers1.isOK());
		CHECK(ers1.message.size() == 0);
		CHECK(ers1.isHandlerOK());
		basar::appl::EventReturnStruct ers2(basar::appl::HANDLER_INFO);
		CHECK(ers2.isOK());
		CHECK(!ers2.isHandlerOK());
        CHECK(ers2.message.size() == 0);
		basar::appl::EventReturnStruct ers3(basar::appl::HANDLER_ERROR);
		CHECK(!ers3.isOK());
		CHECK(!ers3.isHandlerOK());
        CHECK(ers3.message.size() == 0);
		basar::appl::EventReturnStruct ers4(basar::appl::HANDLER_NOT_FOUND);
		CHECK(!ers4.isOK());
		CHECK(!ers4.isHandlerOK());
        CHECK(ers4.message.size() == 0);

		basar::appl::EventReturnStruct ers11(basar::appl::HANDLER_OK, "test");
		CHECK(basar::appl::HANDLER_OK == ers1.ret);
		CHECK(ers11.isOK());
		CHECK(ers11.message.size() > 0);
		CHECK(ers11.isHandlerOK());
		basar::appl::EventReturnStruct ers12(basar::appl::HANDLER_INFO, "test");
		CHECK(ers12.isOK());
		CHECK(!ers12.isHandlerOK());
        CHECK(ers12.message.size() > 0);
		basar::appl::EventReturnStruct ers13(basar::appl::HANDLER_ERROR, "test");
		CHECK(!ers13.isOK());
		CHECK(!ers13.isHandlerOK());
        CHECK(ers13.message.size() > 0);
		basar::appl::EventReturnStruct ers14(basar::appl::HANDLER_NOT_FOUND, "test");
		CHECK(!ers14.isOK());
		CHECK(!ers14.isHandlerOK());
        CHECK(ers14.message.size() > 0);
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

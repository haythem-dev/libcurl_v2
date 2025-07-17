/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    


#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "testexceptionhandler.h"
#include "testexceptionthrower.h"

using  basar::gui::tie::ExceptionFoundInfo;
using  basar::test::unit::guitie::TestExceptionHandler;
using  basar::test::unit::guitie::TestExceptionThrower;

//------------------------------------------------------------------------------
SUITE(GuiTie_TestExceptionHandler)
{ 
	TEST(GuiTie_TestExceptionHandler_testExceptionHandler1)
	{
		try
		{
			TestExceptionHandler handler;
			CHECK_EQUAL(0, handler.m_timesBasarException);
			CHECK_EQUAL(0, handler.m_timesSTDException);
			CHECK_EQUAL(0, handler.m_timesUnknownException);
			bool ok = true;
			ExceptionFoundInfo info;
			basar::Exception exB(basar::ExceptInfo("test","test",  __FILE__, __LINE__), "test"); 
			std::exception exS;
			CHECK_EQUAL(ok, handler.handleException(exB, info, ok));
			CHECK_EQUAL(ok, handler.handleException(exS, info, ok));
			CHECK_EQUAL(ok, handler.handleUnknownException(info, ok));
			CHECK_EQUAL(1, handler.m_timesBasarException);
			CHECK_EQUAL(1, handler.m_timesSTDException);
			CHECK_EQUAL(1, handler.m_timesUnknownException);

			ok = false;
			CHECK_EQUAL(ok, handler.handleException(exB, info, ok));
			CHECK_EQUAL(ok, handler.handleException(exS, info, ok));
			CHECK_EQUAL(ok, handler.handleUnknownException(info, ok));
			CHECK_EQUAL(2, handler.m_timesBasarException);
			CHECK_EQUAL(2, handler.m_timesSTDException);
			CHECK_EQUAL(2, handler.m_timesUnknownException);
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured); 
		}
		catch (...)
		{
			const bool ExceptionOccured2 = false;
			CHECK(ExceptionOccured2); 
			throw;
		}
	}

	TEST(GuiTie_TestExceptionHandler_testExceptionHandlerManager1)
	{
		try
		{
			TestExceptionHandler * handler = new TestExceptionHandler();
			boost::shared_ptr<basar::gui::tie::ExceptionHandler> exhandler = boost::shared_ptr<basar::gui::tie::ExceptionHandler> (handler);
			basar::gui::tie::ExceptionHandlerRef exceptionHandlerRef(exhandler);
			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setExceptionHandlerRef(exceptionHandlerRef);

			CHECK_EQUAL(0, handler->m_timesBasarException);
			CHECK_EQUAL(0, handler->m_timesSTDException);
			CHECK_EQUAL(0, handler->m_timesUnknownException);

			TestExceptionThrower thrower;
			thrower.throwBasarException();

			CHECK_EQUAL(1, handler->m_timesBasarException);
			CHECK_EQUAL(0, handler->m_timesSTDException);
			CHECK_EQUAL(0, handler->m_timesUnknownException);
			thrower.throwBasarException();
			CHECK_EQUAL(2, handler->m_timesBasarException);
			CHECK_EQUAL(0, handler->m_timesSTDException);
			CHECK_EQUAL(0, handler->m_timesUnknownException);

			thrower.throwSTDException();
			CHECK_EQUAL(2, handler->m_timesBasarException);
			CHECK_EQUAL(1, handler->m_timesSTDException);
			CHECK_EQUAL(0, handler->m_timesUnknownException);
			thrower.throwUnknownException();
			CHECK_EQUAL(2, handler->m_timesBasarException);
			CHECK_EQUAL(1, handler->m_timesSTDException);
			CHECK_EQUAL(1, handler->m_timesUnknownException);
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false; 
			CHECK(ExceptionOccured); 
		}
		catch (...)
		{
			const bool ExceptionOccured2 = false;
			CHECK(ExceptionOccured2); 
			throw;
		}
	}

	TEST(GuiTie_TestExceptionHandler_testExceptionHandlerManager2)
	{
		try
		{
			TestExceptionHandler * handler = new TestExceptionHandler();
			boost::shared_ptr<basar::gui::tie::ExceptionHandler> exhandler = boost::shared_ptr<basar::gui::tie::ExceptionHandler> (handler);
			basar::gui::tie::ExceptionHandlerRef exceptionHandlerRef(exhandler);
			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setExceptionHandlerRef(exceptionHandlerRef);

			int  timesBasarException = 0;
			int	timesSTDException = 0;
			int timesUnknownException = 0;
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			TestExceptionThrower thrower;

			thrower.throwBasarException();

			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalBasarException(false);

			CHECK_THROW(thrower.throwBasarException(), basar::Exception);

			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			thrower.throwSTDException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalSTDException(false);

			CHECK_THROW(thrower.throwSTDException(), std::exception);
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			thrower.throwUnknownException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalUnknownException(false);

			try
			{
				thrower.throwUnknownException();
				CHECK(false);
			}
			catch (...)
			{
				CHECK(true);
			}
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(true, true, true);

			thrower.throwUnknownException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);


			thrower.throwBasarException();
			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			thrower.throwSTDException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(false, true, true);


			CHECK_THROW(thrower.throwBasarException(), basar::Exception);
			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			thrower.throwSTDException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);
			thrower.throwUnknownException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);

			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(true, false, true);

			thrower.throwBasarException() ;
			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			CHECK_THROW(thrower.throwSTDException(), std::exception);
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);
			thrower.throwUnknownException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);


			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(true, true, false);

			thrower.throwBasarException() ;
			CHECK_EQUAL(++timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			thrower.throwSTDException();
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(++timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(timesUnknownException, handler->m_timesUnknownException);

			try
			{
				thrower.throwUnknownException();
				CHECK(false);
			}
			catch (...)
			{
				CHECK(true);
			}
			CHECK_EQUAL(timesBasarException, handler->m_timesBasarException);
			CHECK_EQUAL(timesSTDException, handler->m_timesSTDException);
			CHECK_EQUAL(++timesUnknownException, handler->m_timesUnknownException);
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured); 
		}
		catch (...)
		{
			const bool ExceptionOccured2 = false;
			CHECK(ExceptionOccured2); 
			throw;
		}
	}
	// -----------------------------------------------------------------------------------------------------

}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~


#include <libbasarcmnutil.h>

#include "testcmnutil.h"

using basar::ExceptInfo;
using basar::NullReferenceException;
using basar::Exception;

//------------------------------------------------------------------------------
void test_logging()
{
	printf("\n");
	printf("--- test_logging ---------------------------\n");
	printf("\n");

	basar::cmnutil::Logging::ContextCreator context("1st testcontext");
	basar::cmnutil::Logging::pushContext("2nd testcontext");

	log4cplus::Logger logger = basar::cmnutil::Logging::getInstance("testcmnutil.test_logging");

	BLOG_TRACE(logger, "trace message kdlkslk");
	BLOG_DEBUG(logger, "debug message lalak");
	BLOG_INFO (logger, "info  message  kkkk j");
	BLOG_WARN (logger, "warn  message skss");
	BLOG_ERROR(logger, "error message");
	BLOG_FATAL(logger, "fatal message");

	try
	{
		throw NullReferenceException (ExceptInfo ("testcmnutil.test_logging",	// context
			                                      "error occurred",			// reason
									              __FILE__, __LINE__));			// file, line
	}
	catch (const Exception & except)
	{
		printf("%s\n", except.what().c_str());
	}

	basar::VarString lastcontext = basar::cmnutil::Logging::popContext();

	BLOG_INFO (logger, "info  message with first context");
}


#include <libbasarcmnutil.h>

#include "testcmnutil.h"

using basar::ExceptInfo;
using basar::NullReferenceException;
using basar::RuntimeException;
using basar::Exception;

//------------------------------------------------------------------------------
void test_exception()
{
	printf("\n");
	printf("--- test_exception ---------------------------\n");
	printf("\n");

	try
	{
		throw NullReferenceException (ExceptInfo ("testcmnutil.test_exception",					// context
			                                      "any error occurred -> immediately caught",	// reason
									              __FILE__, __LINE__));							// file, line
	}
	catch (const Exception & except)
	{
		printf("%s\n", except.what().c_str());
	}

	throw RuntimeException (ExceptInfo ("testcmnutil.test_exception",					// context
			                             "any error occurred -> caught in main()",		// reason
									     __FILE__, __LINE__));							// file, line
}

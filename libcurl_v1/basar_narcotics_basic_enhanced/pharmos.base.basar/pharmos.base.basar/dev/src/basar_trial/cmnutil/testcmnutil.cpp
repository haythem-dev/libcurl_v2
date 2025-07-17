
#include <libbasarcmnutil.h>

#include "testcmnutil.h"

//------------------------------------------------------------------------------
void test_version()
{
	printf("\n");
	printf("--- test_version ---------------------------\n");
	printf("\n");

//	printf("pkg           : %s\n", basar::cmnutil::getPkgVersion ());
	printf("ver           : %s\n", basar::cmnutil::getVersion    ());

	VarString ver1 = basar::cmnutil::getFullVersion();
	VarString ver2 = ver1.right("IT ");
	printf("full          : %s\n", ver1.left (" version").c_str());
	printf("                %s\n", ver2.left (" built:" ).c_str());

#ifdef _DEBUG
	printf("                %s\n", ver2.right("dbg "    ).c_str());
#else
	printf("                %s\n", ver2.right("rel "    ).c_str());
#endif

	printf("\n");
}

//------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
	// eliminate compiler warnings 
	argc = argc;
	argv = argv;

	printf("################################################################################\n");
	printf("#                                                                              #\n");
	printf("# test libbasarcmnutil                                                         #\n");
	printf("#                                                                              #\n");
	printf("################################################################################\n");
	printf("\n");


	try
	{
		test_version	();
		test_email      ();

#ifdef ABCDE

		test_datetime	();
		test_codec      ();
		test_logging	();
		test_decimal	();
		test_printing	();


		test_pipedprocess();
		test_exception();
		test_parameterlist();
		test_string	    ();
#endif

	}
	catch (const basar::Exception & except)
	{
		printf("\n");
		printf("=== exception caught ===========================================================\n");
		printf("\n");
		printf("%s\n", except.what().c_str());
	}

	printf("\n");
	printf("################################################################################\n");

	printf("\npress return to continue\n");
	getchar();

	return 0;
}

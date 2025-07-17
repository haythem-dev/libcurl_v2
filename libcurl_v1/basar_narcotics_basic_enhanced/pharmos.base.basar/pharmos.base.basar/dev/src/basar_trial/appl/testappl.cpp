
#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>

#include "testappl.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>

#include <locale.h>

//------------------------------------------------------------------------------
void test_version()
{
	printf("\n");
	printf("--- test_version ---------------------------\n");
	printf("\n");

//	printf("pkg           : %s\n", basar::appl::getPkgVersion ());
	printf("ver           : %s\n", basar::appl::getVersion    ());

	VarString ver1 = basar::appl::getFullVersion();
	VarString ver2 = ver1.right("IT ");
	printf("full          : %s\n", ver1.left (" version").c_str());
	printf("                %s\n", ver2.left (" built:" ).c_str());

#ifdef _DEBUG
	printf("                %s\n", ver2.right("dbg "    ).c_str());
#else
	printf("                %s\n", ver2.right("rel "    ).c_str());
#endif

	VarString test;
	test = ver2.right("rel ");

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
	printf("# test libbasarappl                                                            #\n");
	printf("#                                                                              #\n");
	printf("################################################################################\n");
	printf("\n");

	
	try
	{
		test_version	();
		test_systemevents();
	}
	catch (const basar::Exception & except)
	{
		printf("\n");
		printf("=== exception caught ===========================================================\n");
		printf("\n");
		printf("%s\n", except.what().c_str());
	}

	printf("\npress return to continue\n");
	getchar();
	
	printf("\n");
	printf("################################################################################\n");

	printf("\npress return to continue\n");
	getchar();

	return 0;
}

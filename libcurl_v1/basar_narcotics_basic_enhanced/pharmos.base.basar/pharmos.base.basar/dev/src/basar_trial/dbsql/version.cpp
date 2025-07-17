
#include <libbasardbsql.h>

#include "testdbsql.h"

#include <stdio.h>
#include <stdlib.h>

using basar::VarString;

//------------------------------------------------------------------------------
static void print()
{
	printf("################################################################################\n");
	printf("#                                                                              #\n");
	printf("# test libbasardbsql                                                          #\n");
	printf("#                                                                              #\n");
	printf("################################################################################\n");
	printf("\n");
	printf("INFORMIXDIR   : %s\n", getenv("INFORMIXDIR"   ));
	printf("INFORMIXSERVER: %s\n", getenv("INFORMIXSERVER"));
	printf("\n");
//	printf("pkg           : %s\n", basar::db::sql::getPkgVersion ());
	printf("ver           : %s\n", basar::db::sql::getVersion    ());

	VarString ver1 = basar::db::sql::getFullVersion(),
		      ver2 = ver1.right("IT ");
	printf("full          : %s\n", ver1.left (" version").c_str());
	printf("                %s\n", ver2.left (" built:" ).c_str());
#ifdef NDEBUG
	printf("                %s\n", ver2.right("rel "    ).c_str());
#else
	printf("                %s\n", ver2.right("dbg "    ).c_str());
#endif

	printf("\n");
}

//------------------------------------------------------------------------------
void version()
{
	print();
}


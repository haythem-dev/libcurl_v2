#include <libbasardbsql.h>
#include "testdbsql.h"

#include <stdio.h>

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable : 4127 4311 4312 4512 4800 4244)
	#include <Qt/qcoreapplication.h>
	#pragma warning (pop)
#endif
	

using basar::cmnutil::Singleton;
using basar::db::sql::ManagerImpl;
using basar::db::sql::Manager;


//------------------------------------------------------------------------------
log4cplus::Logger LoggerPool::s_LoggerStmt = basar::cmnutil::Logging::getInstance("testdbsql.statement");

//------------------------------------------------------------------------------
void printExceptTitle()
{
	printf("\n");
	printf("=== exception catcher ==========================================================\n");
	printf("\n");
}

//------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
	// eliminate compiler warnings 
	argc = argc;
	argv = argv;

#ifdef WIN32
	QCoreApplication a(argc, argv); // for odbc-connection!
#endif  


	try
	{
		//misc		();
		version		();
		prepstmt	();

		printf("\n\n");
		printf("--- finished -----------------------------------\n");
		printf("\n");

		//basar::db::sql::Manager::clear();
	}
	catch (const basar::Exception & except)
	{
		printExceptTitle();
		printf("%s\n", except.what().c_str());
	}
	catch (...)
	{
		printExceptTitle();
		printf("any exception caught\n");
		assert(0);
	}


	//basar::db::sql::Manager::clear();

	printf("\n");
	printf("################################################################################\n");

	//printf("\npress return to continue\n");
	//getchar();

	return 0;
}

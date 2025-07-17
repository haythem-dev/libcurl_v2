#include <UnitTest++/UnitTest++.h>

#include "libbasarguitie.h"
#include "libbasardebug.h"
#include "libbasarguitie_qtmessagehandler.h"

#include <QtWidgets/QApplication>

#include <UnitTest++/TestReporterStdoutEx.h>

using namespace basar::gui::tie;

//------------------------------------------------------------------------------
bool g_RUNSHORT = false;			// short test mode; avoids long running tests

//------------------------------------------------------------------------------
static void printversion(basar::ConstString suite)
{
	using basar::VarString;

	VarString ver1 = getFullVersion();
	VarString ver2 = ver1.right("GmbH ");

	printf("--- unit test ---------------------------\n");
	printf("test mode : running %s\n", g_RUNSHORT ? "short" : "long");
	printf("test suite: running %s\n", suite      ? suite   : "all" );
	

	printf("\n");
	printf("ver : %s\n", getVersion    ());
	printf("full: %s\n", ver1.left (" version").c_str());
	printf("      %s\n", ver2.left (" built:" ).c_str());
#ifndef NDEBUG
	printf("      %s\n", ver2.right("dbg "    ).c_str());
#else
	printf("      %s\n", ver2.right("rel "    ).c_str());
#endif
	printf("\n");
}

//------------------------------------------------------------------------------
int main(int argc, char * argv[])
{ 
	basar::debug::enableLeakCheck();

	//qt5: qgroupbox leaks 24 bytes per instance if setChecked(false) is called
	basar::debug::ignoreMemoryDiff(26, 624);

	// try environment
	basar::ConstBuffer suite = getenv("BUTGUITIE");

	if (1 < argc)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (0 == strcmp("-short", argv[i]))
			{
				g_RUNSHORT = true;
				continue;
			}

			if (0 == strncmp("-suite=", argv[i], 7))
			{
				suite = argv[i] + 7;
				continue;
			}
		}
	}

	printversion(suite);

    QApplication a(argc, argv);
       
    qInstallMessageHandler( basar::gui::tie::basarQTMessageHandler); 

    UnitTest::TestReporterStdoutEx reporter(true);
	UnitTest::TestRunner         runner(reporter);

	// if suite not set then execute all testsuites
	return runner.RunTestsIf ( UnitTest::Test::GetTestList(), 
							   suite,
							   UnitTest::True(),
							   0);
}

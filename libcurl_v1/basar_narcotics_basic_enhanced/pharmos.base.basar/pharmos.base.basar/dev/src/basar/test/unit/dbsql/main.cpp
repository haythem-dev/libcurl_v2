// #######################################################################################################################
//
// Informix
// =======================================================================================================================
/* 
   Databases: 
   ----------
       Informix:
       zdev21_tcp: but21
                   but21bg
       
       MS-SQL:
       localdb:  basar_ut

   Connections:
   ------------
	     setnet32:
	             host						denu0uazdev21
	             dbserver					zdev21_tcp
				 protocol					onsoctcp
				 service					53004
				 no environment set

		 ODBC 32-bit:
		   Informix (Driver: IBM Informix ODBC Driver):   
		   General:
				 Data Source Name			zdev21_but21_VS2010 (zdev21_but21_x64 for 64-Bit)
		   Connection:
				 Server Name				zdev21_tcp
				 Service					53004
				 Protocol					onsoctcp
				 Database Name				but21
		   Environment:
				 Client Locale				en_US.UTF8 (QODBC expects UTF8 encoded string)
				 Database Locale			en_US.819
				 Use Server Database Locale	off
		   Advanced:
		         Report Char Columns as     
				 Wide Char Columns          on

		   General:
		         Data Source Name			zdev21_but21bg_VS2010 (zdev21_but21_x64 for 64-Bit)
		   Connection:
				 Server Name				zdev21_tcp
				 Service					53004
				 Protocol					onsoctcp
				 Database Name				but21bg
		   Environment:
				 Client Locale				ru_RU.UTF8 (QODBC expects UTF8 encoded string)
				 Database Locale			bg_BG.8859-5
				 Use Server Database Locale	off
		   Advanced:
		         Report Char Columns as     
				 Wide Char Columns          on

           MS-SQL (Driver: ODBC Driver 11 for SQL Server):
           General:
                Data Source name            basar_ut
                Server                      (localdb)\MSSQLLocalDB
                verify authenticity         Integrated Windows Authentication
                Default Database            basar_ut
                Use ANSI quotation marks    yes
                Use ANSI null               yes
                Convert character data      yes
                

   DDL: see data/ddl.sql, data/ddl.mssql.sql
   DML: see data/dml.sql, data/dml.mssql.sql

*/

// #######################################################################################################################
//
// CICS - tcAccess 8.0 >= SP3 32-bit
// =======================================================================================================================
/*
  Connections:
  ------------
  Test CICS VSETEST A02CICS2: 129.210.15.2:3102

  need 4 slots with config "BasarUnitTest" 
    (Win/Unix) - EBCDIC: (1252/8859-1) - 1141
    129.210.15.2:3102, StartTransaction: TCAS

  need 1 slot  with config "BasarUnitTest_cyr" 
    (Win/Unix) - EBCDIC: (1251/8859-5) - 1025
    129.210.15.2:3102, StartTransaction: TCAS

  need 4 slot  with config "BasarDefault_west"
    (Win/Unix) - EBCDIC: (1252/8859-1) - 1141, 
    127.0.0.1:1, StartTransaction: XXXX

  need 1 slot  with config "BasarDefault_cyr"       
    (Win/Unix) - EBCDIC: (1251/8859-5) - 1025, 
    127.0.0.1:1, StartTransaction: XXXX

  need 1 slot  with config "BasarUnitTest.mintimeout"
    (Win/Unix) - EBCDIC: (1252/8859-1) - 1141, 
    127.0.0.1:1, StartTransaction: XXXX
    timeout: 3 sec


  slot 0 should be "BasarUnitTest" or point to 129.210.15.2:3102

  Definition of SQL table / VSAM file vsam.dkpar_ut:
    see data/dkpar_ut.ftb


  Data:
  -----
 
tests need exact 3 values with VSAM.DKPAR_UT.PAR_K_FIL = '01', f.ex.

delete from VSAM.DKPAR_UT where PAR_K_FIL = '01';

insert into VSAM.DKPAR_UT (PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY, test_dec_scale_0, dkpar_euihw ) 
	values ('01','PA29', '01','BasarUnitTest (insert-update-test)', 123456789, 1.23);

insert into VSAM.DKPAR_UT (PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY, test_dec_scale_0 ) 
	values ('01','01', '02','BasarUnitTest (insert-update-test)', -4321);

insert into VSAM.DKPAR_UT (PAR_K_FIL, PAR_K_NAME, PAR_K_NR, DUMMY, test_dec_scale_0, dkpar_euihw ) 
	values ('01', '01', '03','BasarUnitTest (insert-update-test)', -987654, -23.0);

*/

//------------------------------------------------------------------------------
#include <UnitTest++/UnitTest++.h>

#include "libbasardbsql.h"
#include "libbasardebug.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
	#pragma warning (push)
	#pragma warning(disable : 4311 4312 4512 4127)
	#include <QtCore/QCoreApplication>
	#pragma warning (pop)

	#include "libbasarguitie_qtmessagehandler.h"
#endif

#include <UnitTest++/TestReporterStdoutEx.h>

//------------------------------------------------------------------------------
using namespace basar::db::sql;

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
	{
		basar::debug::enableLeakCheck();
		//basar::debug::ignoreMemoryDiff(8, 308 );	// leaks 8 blocks = 308 bytes constantly: QSqlDriverPlugin
	}

	basar::ConstBuffer suite = getenv("BUTDBSQL");

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

	// init random generator
	::srand(static_cast<unsigned int>(time(0)));

#ifdef _WIN32
	QCoreApplication a(argc, argv); // for odbc-connection!
	qInstallMessageHandler( basar::gui::tie::basarQTMessageHandler);
#endif  

	// helper method to init the data when it changes:
	//basar::test::unit::dbsql::DefaultMasterBUTTable table;
	// table.readAndShow();

	UnitTest::TestReporterStdoutEx reporter(true);
	UnitTest::TestRunner         runner(reporter);

	// if suite not set then execute all testsuites
	return runner.RunTestsIf ( UnitTest::Test::GetTestList(), 
                               suite,
							   UnitTest::True(),
							   0);
}

#include <UnitTest++/UnitTest++.h>

#include "libbasarlogin.h"

#include <stdio.h>
#include <stdlib.h>

#include <boost/filesystem.hpp>

//------------------------------------------------------------------------------

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define putenv	_putenv		//!< use MS function
#endif

using basar::ConstString;
using basar::VarString;
using basar::I18nString;
using basar::Int16;

using basar::login::Manager;

//------------------------------------------------------------------------------
SUITE(Backup)
{

//------------------------------------------------------------------------------
static ConstString ENV_INFXSQLHOSTS = "INFORMIXSQLHOSTS";

class EnvInfx
{
public:	
	
	EnvInfx()
	{
		char * pVal = getenv(ENV_INFXSQLHOSTS);

		if (pVal)
			envOrg = pVal;
	};

	~EnvInfx()
	{
		if ( ! envOrg.empty() )
		{
			sprintf(envbuf, "%s=%s",
							ENV_INFXSQLHOSTS,
							envOrg.c_str());
			putenv(envbuf);
		}
	};

	void useUtSqlHosts()
	{
		using boost::filesystem::current_path;
		using boost::filesystem::path;
		
		path sqlhosts = current_path();
		sqlhosts /= "data";
		sqlhosts /= "sqlhosts.ut";
		
		sprintf(envbuf, "%s=%s",
						ENV_INFXSQLHOSTS,
						sqlhosts.string().c_str());
		putenv(envbuf);
	};

private:	
	VarString   envOrg;			// save INFORMIXSQLHOSTS
	static char	envbuf[512];	// for key value pair
};

char EnvInfx::envbuf[] = { 0 };

//------------------------------------------------------------------------------
class Fixture
{
public:
	Fixture	() : appl("DEPENT"),
		         default_hostmain  (Manager::getInstance().getHostMain  ().c_str()),
				 default_hostbackup(Manager::getInstance().getHostBackup().c_str()),
				 default_dbmain    (Manager::getInstance().getDbMain    ().c_str()),
				 default_dbbackup  (Manager::getInstance().getDbBackup  ().c_str())
	{};

	~Fixture()
	{
		Manager::clear();
	};

	// Unix   : use test sqlhosts
    // Windows: must be configured in registry (see main.cpp)
	void fakeDbServer()
    {
#ifndef _WIN32
		envInfx.useUtSqlHosts();
#endif
	};

#ifndef _WIN32
	EnvInfx            envInfx;	// restore Informix environment
#endif

	I18nString         appl;

	VarString		   default_hostmain;
	VarString		   default_hostbackup;
	VarString		   default_dbmain;
	VarString          default_dbbackup;

	static ConstString INVALID_DBSERVER;	// see main.cpp: "Informix requirements"

	static ConstString USER;
	static ConstString PWD;
	static Int16       AREA;
};

ConstString Fixture::INVALID_DBSERVER = "zdev21_tcp_invalid";

ConstString Fixture::USER	          = "t.hoerath";
ConstString Fixture::PWD	          = "rdzdSEQE";
Int16       Fixture::AREA	          = 5;

//------------------------------------------------------------------------------
#define TMCONSTR_DEFAULT	3000	// ms

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, Login_ok)
{
	UNITTEST_TIME_CONSTRAINT(TMCONSTR_DEFAULT);

	Manager::getInstance().setConnectionConfig(default_hostmain,
											   default_hostbackup,
											   default_dbmain,
											   default_dbbackup);

	CHECK( Manager::getInstance().nonInteractiveLogin( appl, USER, PWD, AREA ) );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, LoginOk_PrimaryDBFail)
{
	UNITTEST_TIME_CONSTRAINT(TMCONSTR_DEFAULT);

	Manager::getInstance().setConnectionConfig(default_hostmain,
											   default_hostbackup,
											   "XXX",
											   default_dbbackup);

	CHECK( Manager::getInstance().nonInteractiveLogin( appl, USER, PWD, AREA ) );
}


//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, LoginOk_PrimaryDBFail_SecondaryDBFail)
{
	UNITTEST_TIME_CONSTRAINT(TMCONSTR_DEFAULT);

	Manager::getInstance().setConnectionConfig(default_hostmain,
											   default_hostbackup,
											   "XXX",
											   "YYY");

	CHECK_THROW( Manager::getInstance().nonInteractiveLogin( appl, USER, PWD, AREA ),
		         basar::login::BackupDBException);
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, LoginOk_PrimaryHostFail)
{
	UNITTEST_TIME_CONSTRAINT(20000);	// ms

    try
    {
	    fakeDbServer();

	    Manager::getInstance().setConnectionConfig(INVALID_DBSERVER,
											    default_hostbackup,
											    default_dbmain,
											    default_dbbackup);

	    CHECK( Manager::getInstance().nonInteractiveLogin( appl, USER, PWD, AREA ) );
    }
    catch (const basar::InvalidParameterException &)
	{
		fprintf(stderr, "  please see main.cpp: \"Informix requirements\"\n"
				        "  define invalid Informix DB server \"%s\"\n", 
						INVALID_DBSERVER);
		CHECK(false);
	}
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, LoginFail_PrimaryHostFail_SecondaryHostFail)
{
	UNITTEST_TIME_CONSTRAINT(35000);	// ms

	fakeDbServer();

	Manager::getInstance().setConnectionConfig(INVALID_DBSERVER,
											   INVALID_DBSERVER,
											   default_dbmain,
											   default_dbbackup);

	CHECK_THROW( Manager::getInstance().nonInteractiveLogin( appl, USER, PWD, AREA ),
		         basar::login::BackupDBException);
}

//------------------------------------------------------------------------------
}

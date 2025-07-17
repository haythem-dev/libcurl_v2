#include <UnitTest++/UnitTest++.h>

#include "libbasarlogin.h"	// pch marker 

#include "libbasarcmnutil_config.h"
#include "libbasarcmnutil_functions.h"

#include <boost/filesystem.hpp>

//------------------------------------------------------------------------------
using boost::filesystem::path;
using boost::filesystem::current_path;

using basar::ConstString;
using basar::VarString;
using basar::cmnutil::Config;
using basar::login::Manager;

//------------------------------------------------------------------------------
SUITE(Config)
{

//------------------------------------------------------------------------------
struct Fixture
{
	Fixture() : dir          (current_path()),
				conffile_org (dir),
				testfile_org (dir),
				conffile_tmp (dir)
	{
		Config ::clear();
		Manager::clear();

		conffile_org /= CONFFILE;
		testfile_org /= TESTFILE;

		{
			VarString tmpfile = CONFFILE;
			tmpfile += ".tmp";

			conffile_tmp /= tmpfile.c_str();
		}
	};

	~Fixture()
	{
		Config ::clear();
		Manager::clear();
	}

	enum PhaseEnum  
	{
		PREPARE,
		CLEANUP
	};

	int NoFiles (PhaseEnum phase)
	{
		if (PREPARE == phase)
			return ::rename(conffile_org.string().c_str(), conffile_tmp.string().c_str());	// success == 0
		else
			return ::rename(conffile_tmp.string().c_str(), conffile_org.string().c_str());	// success == 0
	};

	int ConfFile (PhaseEnum)
	{
		return 0;
	};

	int TestFile (PhaseEnum phase)
	{
		if (PREPARE == phase)
			return ::rename(conffile_org.string().c_str(), testfile_org.string().c_str());
		else
			return ::rename(testfile_org.string().c_str(), conffile_org.string().c_str());
	};

	int UTConfFile (PhaseEnum phase)
	{
        path execdirConfFile(basar::cmnutil::getExecDir().c_str());
		path utdirConfFile  (execdirConfFile.parent_path());

		execdirConfFile /= CONFFILE;
		utdirConfFile   /= UTDATADIR;
		utdirConfFile   /= CONFFILE;

		if (PREPARE == phase)
			return ::rename(utdirConfFile.string().c_str(), execdirConfFile.string().c_str());	// success == 0
		else
			return ::rename(execdirConfFile.string().c_str(), utdirConfFile.string().c_str());	// success == 0
	};

	static ConstString UTDATADIR;
	static ConstString CONFFILE;
	static ConstString TESTFILE;

	path dir;

	path conffile_org;
	path testfile_org;

	path conffile_tmp;
};

ConstString Fixture::UTDATADIR = "data";
ConstString Fixture::CONFFILE  = "basar.config.xml";
ConstString Fixture::TESTFILE  = "login4test.cfg";

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, setConnectionParam)
{
	ConstString hostMain   = "abc";
	ConstString hostBackup = "def";
	ConstString dbMain	   = "ghi";
	ConstString dbBackup   = "jkl";
	
	Manager::getInstance().setConnectionConfig(hostMain, hostBackup, dbMain, dbBackup);
	
	CHECK_EQUAL(hostMain  , Manager::getInstance().getHostMain	().c_str());
	CHECK_EQUAL(hostBackup, Manager::getInstance().getHostBackup().c_str());
	CHECK_EQUAL(dbMain    , Manager::getInstance().getDbMain	().c_str());
	CHECK_EQUAL(dbBackup  , Manager::getInstance().getDbBackup	().c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, getConnectParam_noConfTestFile)
{
	CHECK_EQUAL(0, NoFiles(PREPARE));

	ConstString hostMain   = "ode21_tcp";
	ConstString hostBackup = "ide21_tcp";
	ConstString dbMain	   = "zpcua1";
	ConstString dbBackup   = "zpcua2";

	CHECK_EQUAL(hostMain  , Manager::getInstance().getHostMain	().c_str());
	CHECK_EQUAL(hostBackup, Manager::getInstance().getHostBackup().c_str());
	CHECK_EQUAL(dbMain    , Manager::getInstance().getDbMain	().c_str());
	CHECK_EQUAL(dbBackup  , Manager::getInstance().getDbBackup	().c_str());

	CHECK_EQUAL(0, NoFiles(CLEANUP));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, getConnectParam_onlyConfFile)
{
	CHECK_EQUAL(0, ConfFile(PREPARE));

	ConstString hostMain   = "zdev21_tcp";
	ConstString hostBackup = "zdev21_tcp";
	ConstString dbMain	   = "zpcua1";
	ConstString dbBackup   = "zpcua2";

	CHECK_EQUAL(hostMain  , Manager::getInstance().getHostMain	().c_str());
	CHECK_EQUAL(hostBackup, Manager::getInstance().getHostBackup().c_str());
	CHECK_EQUAL(dbMain    , Manager::getInstance().getDbMain	().c_str());
	CHECK_EQUAL(dbBackup  , Manager::getInstance().getDbBackup	().c_str());

	CHECK_EQUAL(0, ConfFile(CLEANUP));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, getConnectParam_onlyTestFile)
{
	CHECK_EQUAL(0, TestFile(PREPARE));

	ConstString hostMain   = "zdev21_tcp";
	ConstString hostBackup = "zdev21_tcp";
	ConstString dbMain	   = "zpcua1";
	ConstString dbBackup   = "zpcua2";

	CHECK_EQUAL(hostMain  , Manager::getInstance().getHostMain	().c_str());
	CHECK_EQUAL(hostBackup, Manager::getInstance().getHostBackup().c_str());
	CHECK_EQUAL(dbMain    , Manager::getInstance().getDbMain	().c_str());
	CHECK_EQUAL(dbBackup  , Manager::getInstance().getDbBackup	().c_str());

	CHECK_EQUAL(0, TestFile(CLEANUP));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, getConnectParam_onlyUTConfFile)
{
	CHECK_EQUAL(0, UTConfFile(PREPARE));

	ConstString hostMain   = "primary_database_server_value";
	ConstString hostBackup = "secondary_database_server_value";
	ConstString dbMain	   = "primary_database_value";
	ConstString dbBackup   = "secondary_database_value";

	CHECK_EQUAL(hostMain  , Manager::getInstance().getHostMain	().c_str());
	CHECK_EQUAL(hostBackup, Manager::getInstance().getHostBackup().c_str());
	CHECK_EQUAL(dbMain    , Manager::getInstance().getDbMain	().c_str());
	CHECK_EQUAL(dbBackup  , Manager::getInstance().getDbBackup	().c_str());

	CHECK_EQUAL(0, UTConfFile(CLEANUP));
}

//------------------------------------------------------------------------------
}	// end SUITE


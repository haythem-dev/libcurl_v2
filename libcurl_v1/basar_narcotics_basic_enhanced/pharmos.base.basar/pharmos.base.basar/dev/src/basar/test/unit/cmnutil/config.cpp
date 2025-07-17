#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include "libbasarcmnutil_config.h"

#include <stdio.h>

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::current_path;

using basar::ConstString;
using basar::VarString;
using basar::cmnutil::Config;
using basar::cmnutil::getExecPath;

#ifdef NDEBUG
#define CONFIG  "Release"
#else
#define CONFIG  "Debug"
#endif

#ifdef _WIN64
#define PLATFORM "/x64"
#else
#define PLATFORM ""
#endif

#ifdef _WIN32
#define UTDIR           "basar" PLATFORM
#else
#define UTDIR           "basar/test/unit/cmnutil"
#endif

static ConstString SHARED_LIB_DIR = "basar/library/lib";

static ConstString CONFIGFILE = "basar.config.xml";

//------------------------------------------------------------------------------
SUITE(TestConfig)
{
	
//----------------------------------------------------------------------------
struct Fixture
{
	Fixture() : workdir     (current_path()),
                execdir     (basar::cmnutil::getExecDir().c_str()),
				workdir_org (workdir),
				execdir_org (execdir),
				workdir_tmp (workdir),
				execdir_tmp (execdir)
	{
		Config::clear();
		
		workdir_org /= CONFIGFILE;
		execdir_org /= CONFIGFILE;

		{
			VarString tmpfile = CONFIGFILE;
			tmpfile += ".tmp";

			workdir_tmp /= tmpfile.c_str();
			execdir_tmp /= tmpfile.c_str();

            boost::system::error_code ec;
            boost::filesystem::remove(workdir_tmp, ec);
            boost::filesystem::remove(execdir_org, ec);
            boost::filesystem::remove(execdir_tmp, ec);
		}
	};

	enum PhaseEnum  
	{
		PREPARE,
		CLEANUP
	};

	int ConfFileRemove (PhaseEnum phase)
	{
		if (PREPARE == phase)
			return ::rename(workdir_org.string().c_str(), workdir_tmp.string().c_str());
		else
			return ::rename(workdir_tmp.string().c_str(), workdir_org.string().c_str());
	};

	int ConfFileExecDir(PhaseEnum phase)
	{
		if (PREPARE == phase)
			return ::rename(workdir_org.string().c_str(), execdir_org.string().c_str());
		else
			return ::rename(execdir_org.string().c_str(), workdir_org.string().c_str());
	};

	int ConfFileWorkDir(PhaseEnum)
	{
		return 0;
	};

	static ConstString KEYS  [8]; 
	static ConstString VALS  [8];
	static const bool  FOUNDS[8];
	static ConstString STREAMED;

	path workdir;
	path execdir;

	path workdir_org;
	path execdir_org;

	path workdir_tmp;
	path execdir_tmp;
};

ConstString Fixture::KEYS [] = 
{
	"basar",
	"basar.login",
	"basar.login.dbserver",
	"basar.login.dbserver.main",
	"basar.login.dbserver.backup",
	"basar.login.database",
	"basar.login.database.main",
	"basar.login.database.backup"
};

ConstString Fixture::VALS [] = 
{
	"",
	"",
	"",
	"zdev21_tcp",
	"zdev21_tcp",
	"",
	"zpcua1",
	"zpcua2"
};

const bool Fixture::FOUNDS[] = 
{
	false,
	false,
	false,
	true,
	true,
	false,
	true,
	true
};

ConstString Fixture::STREAMED =
{
	"config key: <basar>\n"
	"config key: <basar.property>\n"
	"config key: <basar.property.propertytable_yiterator>\n"
	"config key: <basar.login>\n"
	"config key: <basar.login.dbserver>\n"
	"config key: <basar.login.dbserver.main>, config value: <zdev21_tcp>\n"
	"config key: <basar.login.dbserver.backup>, config value: <zdev21_tcp>\n"
	"config key: <basar.login.database>\n"
	"config key: <basar.login.database.main>, config value: <zpcua1>\n"
	"config key: <basar.login.database.backup>, config value: <zpcua2>\n"
};

//----------------------------------------------------------------------------
#ifdef _AIX
struct AIXFixture : public Fixture
{
    AIXFixture() : Fixture(), libDir( basar::cmnutil::getLibDir().c_str() )
    {
        libDir /= CONFIGFILE;
    }

    void ConfFileLibDir(Fixture::PhaseEnum phase)
    {
        using boost::filesystem::copy;

        if ( Fixture::PREPARE == phase )
        {
            copy( workdir_org, libDir );
        }
        else
        {
            remove( libDir.string().c_str() );
        }
    }

    path libDir;

};
#endif
//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, GetConfigFile_wo_file)
{
	// --- remove from working/exec dir: don't find file

	REQUIRE CHECK_EQUAL(0, ConfFileRemove(PREPARE));

	VarString configpath;

	CHECK( ! Config::getInstance().getConfigFile(configpath) );
	CHECK( configpath.empty());

	CHECK_EQUAL(0, ConfFileRemove(CLEANUP));
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, GetConfigFile_w_execdir_file)
{
	// --- find in exec dir
	REQUIRE CHECK_EQUAL(0, ConfFileExecDir(PREPARE));

	VarString configpath;

	CHECK( Config::getInstance().getConfigFile(configpath) );
	CHECK( ! configpath.empty());

	path p(configpath.c_str());
	configpath = p.generic_string();

	ConstString PART = UTDIR "/" CONFIG "/";

	VarString sub(PART);
	sub += CONFIGFILE;

	sub.lower();
	configpath.lower();

	CHECK(VarString::npos != configpath.find(sub));
	CHECK_EQUAL(sub.c_str(), configpath.substr(configpath.find(sub) == VarString::npos 
			                                                            ? 0 
																		: configpath.find(sub)).c_str());
	CHECK_EQUAL(0, ConfFileExecDir(CLEANUP));
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, GetConfigFile_w_workdir_file)
{
	// --- find in work dir
	CHECK_EQUAL(0, ConfFileWorkDir(PREPARE));

	VarString configpath;

	CHECK( Config::getInstance().getConfigFile(configpath) );
	CHECK( ! configpath.empty());

	path p(configpath.c_str());
	configpath = p.generic_string();

#ifdef _WIN32	
	ConstString PART = "/sharedlib/" CONFIG "/";
#else
	ConstString PART = UTDIR "/";
#endif

	VarString sub(PART);
	sub += CONFIGFILE;

	sub.lower();
	configpath.lower();

	CHECK(VarString::npos != configpath.find(sub));
	CHECK_EQUAL(sub.c_str(), configpath.substr(configpath.find(sub) == VarString::npos 
			                                                            ? 0 
																		: configpath.find(sub)).c_str());
	CHECK_EQUAL(0, ConfFileWorkDir(CLEANUP));
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, ConfigGetValue)
{
	VarString val;
	bool      found;

	for (size_t i = 0; i < sizeof(KEYS)/sizeof(KEYS[0]); ++i, val.clear())
	{
		found = Config::getInstance().getValue(KEYS[i], val);

		CHECK_EQUAL(FOUNDS[i], found);
		CHECK_EQUAL(VALS[i], val.c_str());
	}
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, ConfigGetValue_withoutFile)
{
	// --- remove from working/exec dir: don't find file

	CHECK_EQUAL(0, ConfFileRemove(PREPARE));

	VarString val;
	bool      found;

	for (size_t i = 0; i < sizeof(KEYS)/sizeof(KEYS[0]); ++i, val.clear())
	{
		found = Config::getInstance().getValue(KEYS[i], val);

		CHECK_EQUAL(false, found);
		CHECK_EQUAL("", val.c_str());
	}

	CHECK_EQUAL(0, ConfFileRemove(CLEANUP));
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, ConfigToStream)
{
	std::ostringstream out;
	Config::getInstance().toStream(out);

	CHECK_EQUAL(STREAMED, out.str().c_str());
}

//----------------------------------------------------------------------------
TEST_FIXTURE(Fixture, ConfigToStream_withoutFile)
{
	// --- remove from working/exec dir: don't find file
	CHECK_EQUAL(0, ConfFileRemove(PREPARE));

	std::ostringstream out;
	Config::getInstance().toStream(out);

	CHECK_EQUAL("", out.str().c_str());

	CHECK_EQUAL(0, ConfFileRemove(CLEANUP));
}
//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, ConfigWithOptionalEntry)
{
    const char* const optionalKey = "optional_key";
    
    VarString val;
    
    bool found = Config::getInstance().getValue(optionalKey, val);
    
    CHECK_EQUAL(false, found);
    CHECK_EQUAL(true, val.empty());
}

//------------------------------------------------------------------------------

TEST_FIXTURE(Fixture, ConfigWithOptionalEntryInsideBasar)
{
    const char* const optionalKey = "basar.login.optional_key";
    
    VarString val;
    
    bool found = Config::getInstance().getValue(optionalKey, val);
    
    CHECK_EQUAL(false, found);
    CHECK_EQUAL(true, val.empty());
}

//------------------------------------------------------------------------------
#ifdef _AIX
TEST_FIXTURE( AIXFixture, ConfigFileInBasarLibDir )
{
	// --- find in lib dir
	ConfFileLibDir(PREPARE);
    ConfFileRemove(PREPARE);
    ConfFileExecDir(PREPARE);


	VarString configpath;

	CHECK( Config::getInstance().getConfigFile(configpath) );
	CHECK( ! configpath.empty());

	path p(configpath.c_str());
	configpath = p.generic_string();

	VarString sub(SHARED_LIB_DIR);
	sub += "/";
	sub += CONFIG;
	sub += "/";
	sub += CONFIGFILE;

	sub.lower();
	configpath.lower();

	CHECK(VarString::npos != configpath.find(sub));
	CHECK_EQUAL(sub.c_str(), configpath.substr(configpath.find(sub) == VarString::npos 
			                                                            ? 0 
																		: configpath.find(sub)).c_str());
	ConfFileLibDir(CLEANUP);
    ConfFileRemove(CLEANUP);
    ConfFileExecDir(CLEANUP);
}
#endif
//------------------------------------------------------------------------------
}	// end TESTSUITE

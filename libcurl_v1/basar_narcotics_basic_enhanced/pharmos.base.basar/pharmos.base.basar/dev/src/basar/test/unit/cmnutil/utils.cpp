#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

using basar::ConstString;
using basar::VarString;

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::current_path;
using boost::filesystem::is_directory;
using boost::filesystem::exists;

#ifdef NDEBUG
#define CONFIG	"release"
#else
#define CONFIG	"debug"
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

#define UTBINRAW	"utcmnutil"

static ConstString SHARED_LIB_DIR_WIN = UTDIR;

static ConstString SHARED_LIB_DIR_UNIX = "library/lib";

#ifdef _WIN32
#define UTBIN		"utcmnutil.exe"
#else
#define UTBIN		"utcmnutil"
#endif

//------------------------------------------------------------------------------
SUITE(TestUtils)
{
#ifdef _WIN32
	static ConstString s_existFile    = "./libbasarcmnutil.dll";
	static ConstString s_existDir     = "../" CONFIG;
#else
	static ConstString s_existFile    = CONFIG "/" UTBIN;
	static ConstString s_existDir     = "./" CONFIG;
#endif
	static ConstString s_nonexistFile = "./nonsense.file.xxx";
	static ConstString s_nonexistDir  = "../nonsense";

//----------------------------------------------------------------------------
TEST(BoostExists)
{
	{
		ConstString existFile    = s_existFile;
		ConstString existDir     = s_existDir;
		ConstString nonexistFile = s_nonexistFile;
		ConstString nonexistDir  = s_nonexistDir;

		CHECK (  exists(existFile   ));
		CHECK (! exists(nonexistFile));
		CHECK (  exists(existDir    ));
		CHECK (! exists(nonexistDir ));
	}

	{
		path existFile    = s_existFile;
		path existDir     = s_existDir;
		path nonexistFile = s_nonexistFile;
		path nonexistDir  = s_nonexistDir;

		CHECK (  exists(existFile   ));
		CHECK (! exists(nonexistFile));
		CHECK (  exists(existDir    ));
		CHECK (! exists(nonexistDir ));
	}
}

//----------------------------------------------------------------------------
TEST(BoostIsDirectory)
{
	// ConstString
	{
		ConstString existFile    = s_existFile;
		ConstString existDir     = s_existDir;
		ConstString nonexistFile = s_nonexistFile;
		ConstString nonexistDir  = s_nonexistDir;

		CHECK (! is_directory(existFile   ));
		CHECK (! is_directory(nonexistFile));
		CHECK (  is_directory(existDir    ));
		CHECK (! is_directory(nonexistDir ));
	}

	// path
	{
		path existFile    = s_existFile;
		path existDir     = s_existDir;
		path nonexistFile = s_nonexistFile;
		path nonexistDir  = s_nonexistDir;

		CHECK (! is_directory(existFile   ));
		CHECK (! is_directory(nonexistFile));
		CHECK (  is_directory(existDir    ));
		CHECK (! is_directory(nonexistDir ));
	}
}

//----------------------------------------------------------------------------
TEST(getExecPath)
{
	VarString str      = basar::cmnutil::getExecPath();
	VarString execpath = path(str.c_str()).generic_string();
	execpath.lower();

	ConstString PART = UTDIR "/" CONFIG "/" UTBIN;

	CHECK(VarString::npos != execpath.find(PART));
	CHECK_EQUAL(PART, 
		        execpath.substr(
					execpath.find(PART) == VarString::npos ? 
						0 : 
						execpath.find(PART)).c_str());
}

//----------------------------------------------------------------------------
TEST(getExecName)
{
	VarString str = basar::cmnutil::getExecName();
	str.lower();
    CHECK_EQUAL(UTBINRAW, str.c_str());

    str = basar::cmnutil::getExecName(true);
	str.lower();
    CHECK_EQUAL(UTBIN, str.c_str());
}

//----------------------------------------------------------------------------
TEST(getExecDir)
{
	VarString str     = basar::cmnutil::getExecDir();
	VarString execdir = path(str.c_str()).generic_string();
	execdir.lower();

	ConstString PART = UTDIR "/" CONFIG;

	CHECK(VarString::npos != execdir.find(PART));
	CHECK_EQUAL(PART, 
		        execdir.substr(
					execdir.find(PART) == VarString::npos ? 
						0 : 
						execdir.find(PART)).c_str());
}
//----------------------------------------------------------------------------
TEST(getLibDir)
{
    VarString str    = basar::cmnutil::getLibDir();
    VarString libDir = path(str.c_str()).generic_string();

    VarString expectedPart;

#ifdef _WIN32
    expectedPart += SHARED_LIB_DIR_WIN;
#else
    expectedPart += SHARED_LIB_DIR_UNIX;
#endif

    expectedPart += "/" CONFIG;

    CHECK( VarString::npos != libDir.find( expectedPart ) );

    CHECK_EQUAL( expectedPart, libDir.substr( libDir.find( expectedPart ) == VarString::npos ? 0 : libDir.find( expectedPart )).c_str() );

}

//----------------------------------------------------------------------------
TEST(BoostGetExecName)
{
	ConstString execNameRaw = "utcmnutil";
	path		execPath (basar::cmnutil::getExecPath().c_str());

	CHECK_EQUAL(execNameRaw, execPath.stem().string().c_str());

#ifdef _WIN32
	ConstString execName = UTBIN;

	CHECK_EQUAL(execName, execPath.filename().string().c_str());
#endif
}

//----------------------------------------------------------------------------
TEST(BoostGetExecDir)
{
    VarString execDir = path(basar::cmnutil::getExecDir().c_str()).generic_string();

	execDir.lower();

	ConstString PART = UTDIR "/" CONFIG;

	CHECK(VarString::npos != execDir.find(PART));
	CHECK_EQUAL(PART, 
		        execDir.substr(
					execDir.find(PART) == VarString::npos ? 
						0 : 
						execDir.find(PART)).c_str());
}

//----------------------------------------------------------------------------
TEST(BoostGetWorkDir)
{
	path full_path(current_path());

	VarString workdir = full_path.generic_string();
	workdir.lower();

#ifdef _WIN32

	ConstString PART = "/sharedlib/" CONFIG;

#else

	ConstString PART = UTDIR;

#endif

	CHECK(VarString::npos != workdir.find(PART));
	CHECK_EQUAL(
		PART, 
		workdir.substr(
			workdir.find(PART) == VarString::npos ? 
				0 : 
				workdir.find(PART)).c_str());
}

//----------------------------------------------------------------------------
TEST(BoostConvertSlashes)
{
#ifdef _WIN32
	CHECK_EQUAL("\\abc\\def\\", path("/abc/def/").make_preferred());
	CHECK_EQUAL("\\abc\\def\\", path("/abc/def/").make_preferred());
	CHECK_EQUAL("\\abc\\def"  , path("/abc/def" ).make_preferred());
	CHECK_EQUAL("\\abc\\def"  , path("/abc/def" ).make_preferred());
#else
	CHECK_EQUAL("/abc/def/", path("/abc/def/").make_preferred());
	CHECK_EQUAL("/abc/def/", path("/abc/def/").make_preferred());
	CHECK_EQUAL("/abc/def" , path("/abc/def" ).make_preferred());
	CHECK_EQUAL("/abc/def" , path("/abc/def" ).make_preferred());
#endif

	CHECK_EQUAL("/abc/def/", path("/abc/def/").generic_string().c_str());
	CHECK_EQUAL("/abc/def/", path("/abc/def/").generic_string().c_str());
	CHECK_EQUAL("/abc/def" , path("/abc/def" ).generic_string().c_str());
	CHECK_EQUAL("/abc/def" , path("/abc/def" ).generic_string().c_str());
}

//------------------------------------------------------------------------------
}	// end TESTSUITE


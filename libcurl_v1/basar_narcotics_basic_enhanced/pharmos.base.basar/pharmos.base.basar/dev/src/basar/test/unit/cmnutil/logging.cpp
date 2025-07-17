#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // __GNUC__

#include <log4cplus/spi/factory.h>
#include <log4cplus/helpers/property.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif //__GNUC__

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include "stringappender.h"
#include <iomanip>

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define getpid	_getpid		//!< use MS function
#endif

/*
  pattern layout:

		log4cplus:
			%D{%Y.%m.%d %H:%M:%S.[%q|%Q]}	date, time
            %i                              process id
			%t								thread id
			%p								log level
			%x								context
			%c								logger name
			%m								logging message
			%n								linefeed
			%l = %F:%L						file name, line number
*/

//------------------------------------------------------------------------------
SUITE(TestLogging)
{

//------------------------------------------------------------------------------
using basar::Int32;
using basar::Float64;
using basar::ConstString;
using basar::VarString;

using log4cplus::helpers::SharedObjectPtr;
using log4cplus::spi::LayoutFactory;
using log4cplus::Logger;
using log4cplus::Layout;
using log4cplus::PatternLayout;

//------------------------------------------------------------------------------
class Fixture
{
public:
	Fixture() : logger (Logger::getInstance("basar.test.unit.cmnutil.TestLogging")),
		        append (new StringAppender())
				
	{
		append->setName  ("UTCMNUTIL_STRINGAPPENDER");

		logger.addAppender  (append);
		logger.setAdditivity(false);
	};

	VarString getLog()
	{
		return static_cast<StringAppender *>(append.get())->getLogs();
	};

	void configure(ConstString pattern)
	{
		log4cplus::helpers::Properties props;
		props.setProperty("ConversionPattern", pattern);

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // __GNUC__

		std::auto_ptr <Layout> layout(log4cplus::spi::getLayoutFactoryRegistry().get("log4cplus::PatternLayout")->createObject(props));

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__

		append->setLayout(layout);
	};

	Logger						logger;


private:

								Fixture		(const Fixture & rCopy);
	Fixture &					operator =	(const Fixture & rCopy);

	SharedObjectPtr <log4cplus::Appender>	append;
};

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestMessage_String)
{
    try 
	{
		ConstString	msg = "message 0";

		configure("%m");

		BLOG_INFO(logger, msg);

		CHECK_EQUAL(msg, getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestMessage_Stream1)
{
    try 
	{
		ConstString	msg = "message 1";

		configure("%m");

		BLOG_INFO_STREAM(logger, msg);

		CHECK_EQUAL(msg, getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestMessage_Stream2)
{
    try 
	{
        double d = 1.2345234234;

        std::ostringstream str;
        str << "message " << std::setprecision(15) << d;

		configure("%m");

        BLOG_INFO_STREAM(logger, "message " << std::setprecision(15) << d);

        CHECK_EQUAL(str.str(), getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestMessage_Stream3)
{
    try 
	{
        std::ostringstream str;
        
        str << "message 3";

		configure("%m");

        BLOG_INFO_STREAM(logger, str.str());

        CHECK_EQUAL(str.str(), getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestProcessId)
{
    try 
	{
		ConstString	msg = "message 1";
		VarString   pid;
		pid.format("%d", getpid());

		configure("%i");

		BLOG_INFO(logger, msg);

		CHECK_EQUAL(pid, getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestDateTimeMilliSec_0)
{
    try 
	{
		ConstString	msg = "message 2";

		configure("%D{%q}");

		BLOG_INFO(logger, msg);

		VarString strmsec = getLog();
		Int32     intmsec = strmsec.stoi();

		CHECK_EQUAL(3, static_cast<Int32>(strmsec.length()));
		CHECK      (   0 <= intmsec);
		CHECK      (1000 >  intmsec);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestDateTimeMilliSec_1)
{
    try 
	{
		ConstString	msg = "message 3";

		configure("%D{%Q}");

		BLOG_INFO(logger, msg);

		VarString strusec = getLog();
		Float64   flusec  = strusec.stof();

		CHECK_EQUAL(7, static_cast<Int32>(strusec.length()));
		CHECK      ( 0.0   <= flusec);
		CHECK      (1000.0 >  flusec);
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, TestContext)
{
    try 
	{
		ConstString	msg = "message 4";

		configure("%x");

		BLOG_INFO(logger, msg);

        CHECK_EQUAL(basar::cmnutil::getExecName(true), getLog());
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
}

//------------------------------------------------------------------------------
}	// end TESTSUITE

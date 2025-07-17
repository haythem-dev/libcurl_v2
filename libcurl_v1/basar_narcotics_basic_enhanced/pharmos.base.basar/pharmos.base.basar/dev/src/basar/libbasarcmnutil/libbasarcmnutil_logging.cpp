//----------------------------------------------------------------------------
/*! \file
 *  \brief  logger class definitions
 *  \author Michael Eichenlaub
 *  \date   17.10.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_functions.h"
#include "libbasardebug.h"

#include <stdlib.h>

#include <log4cplus/configurator.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>

//! don't use deprecated Boost functions
#define BOOST_SYSTEM_NO_DEPRECATED 
//! don't use deprecated Boost functions
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem/operations.hpp>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return logger-object with given name */
log4cplus::Logger Logging::getInstance (const VarString & name)
{
	return log4cplus::Logger::getInstance(name);
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//!
void Logging::pushContext(const VarString & context)
{
	log4cplus::getNDC().push(context);
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return last pushed context information
VarString Logging::popContext()
{
	return log4cplus::getNDC().pop();
}

//----------------------------------------------------------------------------
/*!	\throw InvalidParameterException */
Logging::Logging()
{
	using boost::filesystem::path;

    bool outofScope   = false;    // VarStrings must go out of scope for cleanup
	path execNameFull(getExecName(true).c_str());

#ifdef _AIX
	// somehow boost::filesystem::path::begin() will segfault under AIX/gcc 
    // when called too early during static init.
	// using own implementation of boost::filesystem::path::stem (which calles begin() eventually)
	VarString execNameShort = execNameFull.string();
	
	size_t pos = execNameShort.rfind(".");
	
	if (pos != std::string::npos)
	{
	    execNameShort = execNameShort.substr(0, pos);
	}
#else
	VarString execNameShort = getExecName();
#endif

	{
		static ConstString ENV_LOG_CONFIG_DIR = "MARATHON_LOG_CONFIGDIR";	// directory for logging configuration
		static ConstString CONFIGDIR          = getenv(ENV_LOG_CONFIG_DIR);
		
		// check environment

		if ( ! CONFIGDIR )
		{
			VarString msg;
			msg.format( "basar.cmnutil.Logging.Logging [%s:%d]:\n\n"
						"    environment variable <%s> not set", 
						__FILE__, __LINE__,
						ENV_LOG_CONFIG_DIR);

            debug::printError(msg.c_str(), execNameShort.c_str());
			outofScope = true;
		}

		if ( ! outofScope )
		{
			// check environment
			static ConstString ENV_LOG_LOGS_DIR = "MARATHON_LOG_LOGSDIR";		// directory for logging output files
			static ConstString LOGSDIR          = getenv(ENV_LOG_LOGS_DIR);

			if ( ! LOGSDIR )
			{
				VarString msg;
				msg.format( "basar.cmnutil.Logging.Logging [%s:%d]:\n\n"
							"    environment variable <%s> not set", 
							__FILE__, __LINE__,
							ENV_LOG_LOGS_DIR);

                debug::printError(msg.c_str(), execNameShort.c_str());
				outofScope = true;
			}
		}

		if ( ! outofScope )
		{
			static ConstString EXT      = "cfg";
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
			static ConstString ENV_USER = "USERNAME";		// environment user name for WINDOWS
#else
			static ConstString ENV_USER = "LOGNAME";		// environment user name for SOLARIS
#endif
			static ConstString USER     = getenv(ENV_USER);

			VarString configFileName = execNameShort; 

			if (0 != USER)
			{
				configFileName += ".";
				configFileName += USER;
			}

			configFileName += ".";
			configFileName += EXT;

			path file = CONFIGDIR;
			file /= configFileName.c_str();

			// check config file <path>/<executable>.<user>.cfg
			if ( ! boost::filesystem::exists(file) )
			{
				file  = CONFIGDIR;
				configFileName = execNameShort;
				configFileName += ".";
				configFileName += EXT;

				file /= configFileName.c_str();

				// check config file <path>/<executable>.cfg
				if ( ! boost::filesystem::exists(file) )
				{
					VarString msg;
					msg.format( "basar.cmnutil.Logging.Logging [%s:%d]:\n\n"
								"    logging config file <%s> doesn't exist", 
								__FILE__, __LINE__,
								file.string().c_str());

                    debug::printError(msg.c_str(), execNameShort.c_str());
					outofScope = true;
				}
			}
			
			// no quiet mode; call avoids Solaris internal log4cplus error messages (linker problem?)
			log4cplus::helpers::LogLog::getLogLog()->setQuietMode        (false);
			// disable internal debugging
			log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(false);
			// log4cplus handling configured by file
			log4cplus::PropertyConfigurator::doConfigure(file.generic_string());
            // set logging context object for basar process
            m_pPrimaryContext.reset(new log4cplus::NDCContextCreator(execNameFull.string()));
		}
	}

	if (outofScope)
		exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
Logging::~Logging()
{
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

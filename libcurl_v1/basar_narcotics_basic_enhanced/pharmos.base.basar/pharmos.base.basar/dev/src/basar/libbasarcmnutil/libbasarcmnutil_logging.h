//----------------------------------------------------------------------------
/*! \file
 *  \brief  logger class definitions
 *  \author Michael Eichenlaub
 *  \date   17.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_LOGGING_H
#define GUARD_LIBBASARCMNUTIL_LOGGING_H

//----------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

#include "libbasar_definitions.h"

#include <log4cplus/logger.h>

#ifdef __GNUG__
#	ifdef LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
#		undef LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
#	endif
 
#	ifdef LOG4CPLUS_HAVE_GNU_VARIADIC_MACROS
#		undef LOG4CPLUS_HAVE_GNU_VARIADIC_MACROS
#	endif
#endif

#include <log4cplus/loggingmacros.h>

#include <boost/scoped_ptr.hpp>

//----------------------------------------------------------------------------

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#pragma warning( disable : 4127 )		// because of "do ... while(0)" macros
//#define WIN32_LEAN_AND_MEAN				//!< log4cplus/logger includes windows.h
#endif

//! creates a TraceLogger to log a TRACE_LOG_LEVEL message to <code>logger</code> 
//! upon entry and existing of a method
#define BLOG_TRACE_METHOD(logger, logEvent) \
    log4cplus::TraceLogger _log4cplus_trace_logger(logger, logEvent, __FILE__, __LINE__)

// --- string interface
//! log a TRACE_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_TRACE LOG4CPLUS_TRACE_STR

//! log a DEBUG_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_DEBUG LOG4CPLUS_DEBUG_STR

//! log a INFO_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_INFO LOG4CPLUS_INFO_STR

//! log a WARN_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_WARN LOG4CPLUS_WARN_STR

//! log a ERROR_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_ERROR LOG4CPLUS_ERROR_STR

//! log a FATAL_LOG_LEVEL message using string to <code>logger</code>
#define BLOG_FATAL LOG4CPLUS_FATAL_STR


// --- stream interface
//! log a TRACE_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_TRACE_STREAM LOG4CPLUS_TRACE

//! log a DEBUG_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_DEBUG_STREAM LOG4CPLUS_DEBUG

//! log a INFO_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_INFO_STREAM LOG4CPLUS_INFO

//! log a WARN_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_WARN_STREAM LOG4CPLUS_WARN

//! log a ERROR_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_ERROR_STREAM LOG4CPLUS_ERROR

//! log a FATAL_LOG_LEVEL message using stringstream to <code>logger</code>
#define BLOG_FATAL_STREAM LOG4CPLUS_FATAL

//---------------------------------------------------------------------------
namespace log4cplus
{
    class NDCContextCreator;
}

//---------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//---------------------------------------------------------------------------
//! class for initialization of logging\n final class
class Logging
{
public:	
	//! retrieve logger \n no-throw
	LIBBASARCMNUTIL_API static log4cplus::Logger getInstance (const VarString & name	//!< name of the logger to retrieve
															 );
	//! push new context information for logging message on context stack (log4cplus::NDC)		\n no-throw
	LIBBASARCMNUTIL_API static void				 pushContext (const VarString & context	//!< new context information
															 );
	//! pop current context information for logging message from context stack (log4cplus::NDC)	\n no-throw
	LIBBASARCMNUTIL_API static VarString		 popContext	 ();
																			

	Logging	 ();		//!< constructor	\n throw InvalidParameterException
	~Logging ();		//!< destructor		\n no-throw

private:
    boost::scoped_ptr<log4cplus::NDCContextCreator> m_pPrimaryContext;	//!< automatically set logging context object for process
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

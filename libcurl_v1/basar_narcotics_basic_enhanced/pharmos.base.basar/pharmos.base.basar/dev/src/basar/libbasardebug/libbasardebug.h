//----------------------------------------------------------------------------
/*! \file
 *  \brief  library for debug purpose
 *  \author Michael Eichenlaub
 *  \date   07.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDEBUG_H
#define GUARD_LIBBASARDEBUG_H

//----------------------------------------------------------------------------
// displays stage of compiling, so not fulfilled precompile conditions can be detected
#ifdef _WIN32
#ifndef COMPILEDHDR__MESSAGE__LIBBASARDEBUG_H
#pragma message("    compiling " __FILE__ " ...")
#define COMPILEDHDR__MESSAGE__LIBBASARDEBUG_H
#endif
#endif

//----------------------------------------------------------------------------
#ifndef LIBBASARDEBUG_API
	#ifdef _WIN32
		#ifdef LIBBASARDEBUG_EXPORTS
			#define LIBBASARDEBUG_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDEBUG_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDEBUG_API
	#endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
//! debug purpose
namespace debug
{

//----------------------------------------------------------------------------
/*! \brief switch on Windows memory leak checking (debug mode)
 	\n     no-throw		*/
LIBBASARDEBUG_API void		enableLeakCheck		(bool on = true			//!< switch on/off checking
												);
//----------------------------------------------------------------------------
/*! \brief for Windows only: ignore deviation of start and stop heap state snapshot
 	\n     no-throw		*/
LIBBASARDEBUG_API void		ignoreMemoryDiff    (int blocks,			//!< deviation of heap block number to ignore
												 int bytes				//!< deviation of heap bytes to ignore
												);
//----------------------------------------------------------------------------
//! print out error message	in suitable way	\n no-throw
LIBBASARDEBUG_API void		printError			(ConstString msg,		//!< error message 
												 ConstString heading	//!< title or caption
												);
//----------------------------------------------------------------------------
//! \brief print debug message:
//! \n     Windows: OutputDebugString()
//! \n     Unix:    fprintf(stderr, ...)
LIBBASARDEBUG_API void		printDbgMsg			(ConstString msg		//!< debug message to print
												);
//----------------------------------------------------------------------------
//! is process running at build server? (important for avoiding GUI output)	\n no-throw
LIBBASARDEBUG_API bool		isBuildServer		();

//----------------------------------------------------------------------------
}	// namespace debug
}	// namespace basar

//----------------------------------------------------------------------------
#endif


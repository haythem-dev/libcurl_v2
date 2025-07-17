//----------------------------------------------------------------------------
/*! \file
 *  \brief  helper methods for printing
 *  \author Michael Eichenlaub
 *  \date   15.05.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_print.h"
#include "libbasarcmnutil_functions.h"
#include "loggerpool.h"

#include <boost/filesystem/operations.hpp>

using boost::filesystem::path;
using boost::filesystem::is_directory;

//----------------------------------------------------------------------------
namespace basar	
{
namespace cmnutil
{

//------------------------------------------------------------------------------
//! \throw  no-throw
void PrintTools::insertStr2Coll ( const VarString   & input,
								  CollPrintRawInput & collPrintRawInput )
{
	VarString::const_iterator itCur = input.begin();
	VarString::const_iterator itEnd = input.end  ();

	for (; itCur != itEnd; ++itCur)
		collPrintRawInput.push(*itCur);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return postscript trigger lines
VarString PrintTools::getTrigger(const VarString & formPath    ,
								 const VarString & formFileName,
								 const VarString & formName    ,
							 	 Int32             fromPage    ,
								 Int32             toPage	   )
{
	static ConstString linePS  = "%!PS-Adobe\n";				// Postscript header
	VarString          trigger = linePS;

	if (-1 != fromPage)
	{
		static ConstString rangeFrom = "/userprintfrom %d def\n";

		VarString fmt;
		fmt.format(rangeFrom, fromPage);

		trigger += fmt;
	}

	if (-1 != toPage)
	{
		static ConstString rangeTo = "/userprintto %d def\n";

		VarString fmt;
		fmt.format(rangeTo, toPage);

		trigger += fmt;
	}


	static ConstString run1 = "(%s%s) run %s\n";
	static ConstString run2 = "(%s/%s) run %s\n";
	VarString          run;

	run.format (formPath.empty() ? run1 : run2,
				formPath    .c_str(),
				formFileName.c_str(),
				formName    .c_str());

	trigger += run;

	return trigger;
}

//------------------------------------------------------------------------------
//! search for subdirectory "print" in directory where executable is located. 
//! If not found then search in parent directory
//! \throw InvalidParameterException
//! \return print path
VarString PrintTools::getFormPathClient()
{
	ConstString subDir = "print";

	VarString execDir = path(getExecDir().c_str()).generic_string();

	if (LoggerPool::printTools().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("getExecPath() = \"%s\"", execDir.c_str());
		LoggerPool::printTools().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if (execDir.empty())
		throw InvalidParameterException(ExceptInfo("basar.cmnutil.PrintTools.getFormPathClient()",
		                                           "executable path is empty!",
											       __FILE__, __LINE__));

	path curPath   = execDir.c_str();
	path printPath = curPath;

	bool printPathFound = false;

	// first probe ./print, then probe ../print, ../../print etc.
	while ( curPath.has_parent_path() )
	{
		printPath /= subDir;

		if ( is_directory(printPath) )	// printpath exists and is directory
		{
			if (LoggerPool::printTools().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format(
					"getFormPathClient() = \"%s\"", 
					printPath.generic_string().c_str());
				LoggerPool::printTools().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}

			printPathFound = true;
			break;
		}

		printPath = curPath.parent_path();
		curPath   = printPath;
	}

	if (!printPathFound)
	{
		VarString msg;
		msg.format("found subdirectory \"%s\" for client print forms neither in directory \"%s\" "
			       "nor in its parent directories", 
					subDir,
					execDir.c_str());
		throw InvalidParameterException(ExceptInfo("basar.cmnutil.PrintTools.getFormPath()",
												msg,
												__FILE__, __LINE__));
	}

	return printPath.generic_string();
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

//---------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main function
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//---------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narctransferappl.h"
#include "parameter.h"
#include "version.h"
#include "loggerpool.h"

//---------------------------------------------------------------------------------------------------------//
// main
//---------------------------------------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
	basar::VarString msg;
	try
	{
		// -------------------------------
		//   init application parameters
		// -------------------------------
		Parameter::getInstance().init(argc, argv);

		// -------------------------------
		//   log start
		// -------------------------------
		msg.format("=== %s (%s) BRANCH %i STARTED   ==============.",
			       Parameter::getInstance().getProcessName().c_str(),
				   VERSION_NUMBER,
				   Parameter::getInstance().getBranchNo());
		BLOG_INFO(LoggerPool::loggernarctransfer, msg.c_str());

		// -------------------------------
		//   run applicationcontroller
		// -------------------------------
		basar::appl::EventReturnStruct ret;
		ret = narcotics::NarcTransferAppl::getInstance().run(
			basar::appl::NullEventSource::getInstance(),
			basar::db::aspect::AccessorPropertyTable_YIterator());

	}
	catch (const basar::Exception& bex)
	{
		msg.format("=== basar exception caught ===========================================================\n%s",
			bex.what().c_str());
		BLOG_FATAL(LoggerPool::loggernarctransfer, msg.c_str());
		return EXIT_FAILURE;
	}
	catch (const std::exception& sex)
	{
		msg.format("=== std exception caught ===========================================================\n%s",
			sex.what());
		BLOG_FATAL(LoggerPool::loggernarctransfer, msg.c_str());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		msg.format("=== unknown exception caught ===========================================================");
		BLOG_FATAL(LoggerPool::loggernarctransfer, msg.c_str());
		return EXIT_FAILURE;
	}

	// -------------------------------
	// log finish
	// -------------------------------
	msg.exchange("STARTED ", "FINISHED");
	BLOG_INFO(LoggerPool::loggernarctransfer, msg.c_str());

	return EXIT_SUCCESS;
}

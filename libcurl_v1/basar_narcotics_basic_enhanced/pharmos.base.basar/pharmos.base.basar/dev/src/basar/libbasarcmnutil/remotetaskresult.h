//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task result 
 *  \author Marco Köppendörfer
 *  \date   02.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKRESULT
#define GUARD_REMOTETASKRESULT

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iremotetaskresult.h"

#include "remotetaskexitstatus.h"

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"


//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{

		//------------------------------------------------------------------------------------------------------//
		//! \brief RemoteTaskResult: stores result of task (task output, exit code, exit signal)
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//
		class RemoteTaskResult : public IRemoteTaskResult
		{
			public:

				//! \brief c'tor
				RemoteTaskResult();

				//! \brief d'tor
				~RemoteTaskResult();

				//! \brief getter for task's result - output to stdout
				virtual const basar::VarString& getResult(); 

				//! \brief specifies whether task encountered error - either ExitCode != 0 or !Signal.empty() - note: depending on your task, exit by signal may led to correct result
				virtual bool hasError();

				//! \brief specifies whether task was terminated by signal - !Signal.empty()
				virtual bool hasSignal();

				//! \brief getter for task result
				virtual basar::Int32 getExitCode();

				//! \brief getter for signal if task was teminated by signal
				virtual const VarString& getSignal();

				//! \brief setter for task result
				void setResult(const basar::VarString& result);

				//! \brief setter for exit status (RemoteTaskExitStatus is a compound value class for exitCode and exitSignal)
				void setExitStatus(const RemoteTaskExitStatus& exitStatus);
			
			private:
				//functions
				RemoteTaskResult& operator=(const RemoteTaskResult&); //!< forbidden
				RemoteTaskResult(const RemoteTaskResult&); //!< forbidden

				//variables
				basar::VarString m_Result; //!< stores remote task's output to stdout
				
				basar::VarString m_Signal; //!< stores signal name if remote task was terminated by signal

				basar::Int32     m_ExitCode; //!< stores exit code of remote task

		};
	}
}

#endif //GUARD_REMOTETASKRESULT
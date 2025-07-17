//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task exit status - value class
 *  \author Marco Köppendörfer
 *  \date   10.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKEXITSTATUS
#define GUARD_REMOTETASKEXITSTATUS

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
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
		//! \brief RemoteTaskExitStatus: stores task exit code and exit signal (if any)
		//!	\n final value class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//
		class RemoteTaskExitStatus
		{
			public:
				//!< defaulting c'tor, d'tor, copy c'tor, assignment op

				//! \brief getter for signal string
				//! \return VarString containing signal name of remote task's exited by signal, empty otherwise
				const basar::VarString& getExitSignal() const;

				//! \brief getter for exit code
				//! \return Int32 representing remote task's exit code
				basar::Int32 getExitCode() const;

				//! \brief getter for signal string
				void setExitSignal(const basar::VarString& exitSignal);

				//! \brief setter for exit code
				void setExitCode(basar::Int32 exitCode);

			private:
				basar::Int32 m_ExitCode; //!< remote task's exit code
				basar::VarString m_ExitSignal; //!< if remote task was terminated by a signal, the signal's name will be stored here, otherwise it's empty string
		};

	} // end namespace cmnutil
} // end namespace basar


#endif //GUARD_REMOTETASKEXITSTATUS

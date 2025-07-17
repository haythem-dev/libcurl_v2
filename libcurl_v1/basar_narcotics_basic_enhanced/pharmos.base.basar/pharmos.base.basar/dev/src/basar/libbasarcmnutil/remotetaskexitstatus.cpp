//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task exit status - value class
 *  \author Marco Köppendörfer
 *  \date   10.10.2013
 */
//----------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "remotetaskexitstatus.h"


//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		const basar::VarString& RemoteTaskExitStatus::getExitSignal() const
		{
			return m_ExitSignal;
		}

		basar::Int32 RemoteTaskExitStatus::getExitCode() const
		{
			return m_ExitCode;
		}

		void RemoteTaskExitStatus::setExitSignal(const basar::VarString& exitSignal)
		{
			m_ExitSignal = exitSignal;
		}

		void RemoteTaskExitStatus::setExitCode(basar::Int32 exitCode)
		{
			m_ExitCode = exitCode;
		}

	}
}

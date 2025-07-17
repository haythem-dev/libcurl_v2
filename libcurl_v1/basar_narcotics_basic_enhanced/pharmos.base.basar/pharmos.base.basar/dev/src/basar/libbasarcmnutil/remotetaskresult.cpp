//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task result 
 *  \author Marco Köppendörfer
 *  \date   02.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "remotetaskresult.h"


//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		RemoteTaskResult::RemoteTaskResult() : m_ExitCode(0)
		{
		}

		RemoteTaskResult::~RemoteTaskResult()
		{
		}

		const VarString& RemoteTaskResult::getResult()
		{
			return m_Result;
		}

		bool RemoteTaskResult::hasError()
		{
			return (!m_Signal.empty()) || (0 != m_ExitCode);
		}

		bool RemoteTaskResult::hasSignal()
		{
			return !(m_Signal.empty());
		}

		basar::Int32 RemoteTaskResult::getExitCode()
		{
			return m_ExitCode;
		}

		const VarString& RemoteTaskResult::getSignal()
		{
			return m_Signal;
		}

		void RemoteTaskResult::setResult(const basar::VarString& result)
		{
			m_Result = result;
		}

		void RemoteTaskResult::setExitStatus(const RemoteTaskExitStatus& exitStatus)
		{
			m_ExitCode = exitStatus.getExitCode();
			m_Signal = exitStatus.getExitSignal();
		}

	}
}
//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "remotetask.h"
#include "libbasarcmnutil_icredentialptr.h"
#include "libbasarcmnutil_iremotetaskcallback.h"
#include "libbasarcmnutil_icredential.h"

#include "libbasarcmnutil_remotetaskexceptions.h"


//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		RemoteTask::RemoteTask() : m_Endpoint(), m_RemoteCommand(), m_Listener(), m_Credential()
		{
		}

		RemoteTask::~RemoteTask()
		{
		}
		
		void RemoteTask::setEndpoint(const RemoteEndpoint& endpoint)
		{
			m_Endpoint = endpoint;
		}

		void RemoteTask::setCredentials(ICredentialPtr credential)
		{
			m_Credential = credential;
		}

		void RemoteTask::setTaskCommand(const VarString& command)
		{
			m_RemoteCommand = command;
		}

		void RemoteTask::registerListener(IRemoteTaskCallBackPtr listener)
		{
			m_Listener = listener;
		}

		void RemoteTask::notifyListeners(const VarString& message) const
		{
			if (hasListeners())
			{
				m_Listener->notify(message);
			}
		}

		const BString& RemoteTask::getRemoteEndpointAddress() const
		{
			if (!hasNonEmptyEndpoint())
			{
				throw RemoteEndpointAddressEmptyException(ExceptInfo("basar::cmnutil::RemoteTask::getRemoteEndpointAddress()", "endpoint address is empty", __FILE__, __LINE__));
			}
			
			return m_Endpoint.getRemoteEndpointAddress();
		}

		basar::Int16 RemoteTask::getRemoteEndpointPort() const
		{
			//no check here, even 0 is a valid port number - technically
			return m_Endpoint.getRemoteEndpointPort();
		}

		const basar::VarString& RemoteTask::getTaskCommand() const
		{
			if (!hasCommand())
			{
				throw RemoteTaskCommandEmptyException(ExceptInfo("basar::cmnutil::RemoteTask::getTaskCommand()", "task command is empty", __FILE__, __LINE__));
			}
			return m_RemoteCommand;
		}

		void RemoteTask::authenticateUsingCredentials(SshSessionPtr session) const
		{
			if (!hasCredentials())
			{
				throw BasarNullReferenceException(ExceptInfo("basar::cmnutil::RemoteTask::authenticateUsingCredentials()", "0 == m_Credential.get()", __FILE__, __LINE__));
			}
			m_Credential->authenticate(session);
		}

		bool RemoteTask::hasListeners() const
		{
			return (0 != m_Listener.get());
		}

		bool RemoteTask::hasCommand() const
		{
			return !(m_RemoteCommand.empty());
		}

		bool RemoteTask::hasCredentials() const
		{
			return (0 != m_Credential.get());
		}

		bool RemoteTask::hasNonEmptyEndpoint() const
		{
			return !( m_Endpoint.isEmpty() );
		}

	}
}
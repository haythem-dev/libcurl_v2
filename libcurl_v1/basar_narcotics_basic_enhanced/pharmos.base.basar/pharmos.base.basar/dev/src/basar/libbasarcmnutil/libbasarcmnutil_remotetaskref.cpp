//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task ref wrapper - only implementing setter part of IRemoteTasks interface
 *  \author Marco Köppendörfer
 *  \date   11.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_remotetaskref.h"
#include "remotetask.h"
#include "libbasarcmnutil_exceptions.h"

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{

		RemoteTaskRef::RemoteTaskRef() : m_RemoteTask(new RemoteTask)
		{
		}

		RemoteTaskRef::~RemoteTaskRef()
		{
		}

		void RemoteTaskRef::setEndpoint(const RemoteEndpoint& endpoint)
		{
			getIImpl()->setEndpoint(endpoint);
		}

		void RemoteTaskRef::setCredentials(ICredentialPtr c)
		{
			getIImpl()->setCredentials(c);
		}

		void RemoteTaskRef::setTaskCommand(const BString& command)
		{
			getIImpl()->setTaskCommand(command);
		}

		void RemoteTaskRef::registerListener(IRemoteTaskCallBackPtr p)
		{
			getIImpl()->registerListener(p);
		}

		bool RemoteTaskRef::isNull() const
		{
			return (0 == m_RemoteTask.get());
		}
			
		void RemoteTaskRef::reset()
		{
			m_RemoteTask.reset();
		}

		const IRemoteTaskPtr& RemoteTaskRef::getIImpl()
		{
			if (0 == m_RemoteTask.get())
			{
				NullReferenceException(ExceptInfo( "basar::cmnutil::RemoteTaskRef::getIImpl", "m_RemoteTask.get() == 0",__FILE__, __LINE__) );
			}

			return m_RemoteTask;
		}
	}
}

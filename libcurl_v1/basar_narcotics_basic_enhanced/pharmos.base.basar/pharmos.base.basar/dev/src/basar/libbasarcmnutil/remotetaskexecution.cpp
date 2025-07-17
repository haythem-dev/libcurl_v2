//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for exection of remote task - implementation
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#include "remotetaskexecution.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_remotetaskexceptions.h"

#include "remotetaskresultptr.h"
#include "remotetaskresult.h"
#include "libbasarcmnutil_remoteendpoint.h"


#include "libbasarcmnutil_socket.h"

#include "iremotetask.h"

#include "libbasarcmnutil_icredential.h"



#include "libbasarcmnutil_sshsessionptr.h"

#include "sshsession.h"
#include "sshchannel.h"

#include "libbasarcmnutil_logging.h"
#include "loggerpool.h" //needs to be included after sshsession.h due to inclusion of win socket functions


//todo: at some point, map library's error codes to text representations

namespace basar
{
	namespace cmnutil
	{
		RemoteTaskExecution::RemoteTaskExecution(IRemoteTaskPtr task) : m_Task(task)
		{
			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), 
							  "creating RemoteTaskExecution instance with " <<
							  " endpoint address: " << m_Task->getRemoteEndpointAddress() <<
							  " endpoint port: " << m_Task->getRemoteEndpointPort() <<
							  " task command: " << m_Task->getTaskCommand()
							  );
		}

		RemoteTaskExecution::~RemoteTaskExecution()
		{
			cleanup();
		}

		IRemoteTaskResultPtr RemoteTaskExecution::execute()
		{
		
			//open tcp socket
			openSocket();
			
			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "opened tcp socket");

			//SshSession needs open socket
			SshSessionPtr  sess(new SshSession(m_Socket) );

			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "created ssh session");

			m_Task->authenticateUsingCredentials(sess);
			
			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "authenticated using supplied credentials");

			////5) omit known hosts handling - need to know how known hosts are distributed, also would need all 2*15 machines
			//// possible to read known hosts from memory, see libssh2_knownhost_readline 

			SshChannel chan(sess);

			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "created ssh channel");

			chan.startProcess(m_Task->getTaskCommand());

			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "started remote process");

			while(SSH_TASK_FINISHED != chan.getTaskStatus())
			{									
				if (SSH_TASK_WAITING_ON_SOCKET == chan.getTaskStatus())
				{
					continue;
				}

				VarString s = chan.readTaskOutput();

				m_TaskOutput.append(s); 

				m_Task->notifyListeners(s);

				BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "read from remote: >" << s << "<");
			}
			
			RemoteTaskResultPtr result = RemoteTaskResultPtr(new RemoteTaskResult());

			result->setExitStatus(chan.getExitStatus());
			result->setResult(m_TaskOutput);

			BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "remote process finished, got results");
			//should not call cleanup here, SshChannel still references m_Socket (needs socket in destructor)

			return result;

		}

		void RemoteTaskExecution::openSocket()
		{
			//1) open socket
			//todo: have endpoint use struct (address, port)
			m_Socket = TcpSocketSharedPtr(new TcpSocket(m_Task->getRemoteEndpointAddress(), m_Task->getRemoteEndpointPort()));

			TcpSocket::RetCodeEnum ret = m_Socket->open();

			if (ret != TcpSocket::TCP_SUCCESS)
			{
				basar::VarString msg;
				
				msg.format("TCP-Socket opening failed with error code %d", ret);

				if (LoggerPool::remoteTask().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
				{
					LoggerPool::remoteTask().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
				}

				throw RemoteConnectionFailedException(ExceptInfo("basar::cmnutil::RemoteTaskExecution::openSocket()", msg, __FILE__, __LINE__));
			}

			BLOG_INFO_STREAM(LoggerPool::remoteTask(), "socket opened, endpoint: " << m_Task->getRemoteEndpointAddress() << ":" <<  m_Task->getRemoteEndpointPort());
		}

		void RemoteTaskExecution::closeSocket()
		{
			if (m_Socket->isOpen())
			{
				m_Socket->close();
			}
		}

		void RemoteTaskExecution::cleanup()
		{		
			//delete shared ptr's
			m_Task.reset();
			m_Socket.reset();
		}
	}
}
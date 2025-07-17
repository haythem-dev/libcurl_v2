//----------------------------------------------------------------------------
/*! \file
 *  \brief  RAII like class representing a ssh_channel
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//

#include "sshchannel.h"

#include "sshsession.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_remotetaskexceptions.h"


#include "libbasarcmnutil_logging.h"
#include "loggerpool.h" //needs to be included after sshsession.h due to inclusion of win socket functions



//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		SshChannel::SshChannel(SshSessionPtr session) : m_Session(session), m_Channel(0), m_SshTaskStatus(SSH_TASK_NOT_STARTED), m_SshChannelStatus(SSH_CHANNEL_UNDEFINED), m_TaskExitStatus(), m_TaskOutput()
		{
		
			initChannel();
		}

		void SshChannel::initChannel()
		{

			if (!m_Session->isSessionInitialized())
			{
				VarString msg("supplied ssh session was not initialized");
				
				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), msg);
				throw SshSessionNotInitializedException(ExceptInfo("basar::cmnutil::RemoteTaskExecution::initChannel()", msg, __FILE__, __LINE__));
			}
					
			while( (m_Channel = libssh2_channel_open_session(m_Session->getRawSession())) == NULL && libssh2_session_last_error(m_Session->getRawSession(),NULL,NULL,0) == LIBSSH2_ERROR_EAGAIN )
			{
				m_Session->waitSocket();
			}

			if (0 == m_Channel)
			{
				VarString msg("Ssh channel open failed (libssh2_channel_open_session returned null-pointer)");

				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), msg);

				throw SshOpenChannelFailedException(ExceptInfo("basar::cmnutil::RemoteTaskExecution::openChannel()", msg, __FILE__, __LINE__));

			}

            m_SshChannelStatus = SSH_CHANNEL_OPEN;

			BLOG_INFO_STREAM(LoggerPool::remoteTask(), "SshChannel initialized (opened)");
		}

		void SshChannel::startProcess(const basar::VarString& command)
		{
			int rc = -1;
			
			while( (rc = libssh2_channel_process_startup(m_Channel,"exec", sizeof("exec")-1, command.c_str(), static_cast<unsigned int>(command.length()) )) == LIBSSH2_ERROR_EAGAIN )
			{
				m_Session->waitSocket();
			}

			if (rc)
			{
				//handle error

				std::ostringstream os;
				os << "Ssh send remote command >" << command << "< failed - libssh2_channel_process_startup returned " << static_cast<long>(rc);

				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), os.str());

				throw SshExecFailedException(ExceptInfo("basar::cmnutil::SshChannel::startProcess()", os.str(), __FILE__, __LINE__));

			}

			m_SshTaskStatus = SSH_TASK_RUNNING;
		}

		SshTaskStatus SshChannel::getTaskStatus() const
		{
			return m_SshTaskStatus;
		}

		const basar::VarString& SshChannel::readTaskOutput()
		{
			
			m_TaskOutput.clear();
			
			if (SSH_TASK_RUNNING == m_SshTaskStatus)
			{
				// read one buffer
				::ssize_t rc = 0;
				//16k buffer
				char buffer[0x4000] = {0};
				do
				{
					rc = libssh2_channel_read( m_Channel, buffer, sizeof(buffer) );
					if( rc > 0 )
					{
						m_TaskOutput += std::string(buffer, rc);

						//have to use static cast to long, << does not take ssize_t
						BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "buffer read returned with " << static_cast<long>(rc) << " bytes read: >" << buffer << "<");
						
						//reset buffer to zero
						memset(buffer, 0, sizeof(buffer) );				

					}
					else
					{
						//here rc is already <= 0, so casting to long should be ok... 
						if( rc != LIBSSH2_ERROR_EAGAIN && rc != 0)
						{
							BLOG_ERROR_STREAM(LoggerPool::remoteTask(), "libssh2_channel_read failed with: " << static_cast<long>(rc));

							switch(rc)
							{
								case LIBSSH2_ERROR_SOCKET_SEND:
								{
									std::ostringstream os;
									os << "libssh2_channel_read failed with: " << static_cast<long>(rc) << " (LIBSSH2_ERROR_SOCKET_SEND)";
									
									throw SshSocketSendFailedException(ExceptInfo("basar::cmnutil::SshChannel::readTaskOutput()", os.str(), __FILE__, __LINE__));

								}
								break;
								case LIBSSH2_ERROR_CHANNEL_CLOSED:
								{
									std::ostringstream os;
									os << "libssh2_channel_read failed with: " << static_cast<long>(rc) << " (LIBSSH2_ERROR_CHANNEL_CLOSED)";
									
									throw SshChannelClosedException(ExceptInfo("basar::cmnutil::SshChannel::readTaskOutput()", os.str(), __FILE__, __LINE__));
								}
								break;
								default:
								break;
							}
						}
						//LIBSSH2_ERROR_SOCKET_SEND
						//LIBSSH2_ERROR_CHANNEL_CLOSED
					}
				}
				while( rc > 0 ); //while there is something which can be read

				/* this is due to blocking that would occur otherwise so we loop on
				this condition */
				if( rc == LIBSSH2_ERROR_EAGAIN )
				{
					BLOG_DEBUG_STREAM(LoggerPool::remoteTask(), "waiting for socket");
					m_SshTaskStatus = SSH_TASK_WAITING_ON_SOCKET;
					m_Session->waitSocket();
					m_SshTaskStatus = SSH_TASK_RUNNING;
				}
				else
				{
					//nothing more to read - todo: test!
					m_SshTaskStatus = SSH_TASK_FINISHED;
				}
			}
			return m_TaskOutput;
		}

		const RemoteTaskExitStatus& SshChannel::getExitStatus()
		{
			int exitcode = 1028;
			int rc = -1;

			char* exitsignal = NULL;

			while( (rc = libssh2_channel_close(m_Channel)) == LIBSSH2_ERROR_EAGAIN )
			{
				m_Session->waitSocket();
			}

            m_SshChannelStatus = SSH_CHANNEL_CLOSED;

			BLOG_INFO_STREAM(LoggerPool::remoteTask(), "SshChannel closed");

			if( rc == 0 )
			{
				//from doc: Returns 0 on failure, otherwise the Exit Status reported by remote host 
				//great: Exit Status 0 on remote host means no failure...
				exitcode = libssh2_channel_get_exit_status( m_Channel );

				libssh2_channel_get_exit_signal(m_Channel, &exitsignal,	NULL, NULL, NULL, NULL, NULL);

				//11) handle exitcode and signal
				m_TaskExitStatus.setExitCode(exitcode);
				
				if (NULL != exitsignal)
				{
					m_TaskExitStatus.setExitSignal(exitsignal);
				}
			}

			return m_TaskExitStatus;
		}


		SshChannel::~SshChannel()
		{
			freeChannel();
		}

		void SshChannel::freeChannel()
		{
			if (0 != m_Channel)
			{
                if (SSH_CHANNEL_OPEN == m_SshChannelStatus)
                {
                    ::ssize_t rc = 0;
                    while ((rc = libssh2_channel_close(m_Channel)) == LIBSSH2_ERROR_EAGAIN)
                    {
                        m_Session->waitSocket();
                    }

                    m_SshChannelStatus = SSH_CHANNEL_CLOSED;
                }

				libssh2_channel_free(m_Channel);
				BLOG_INFO_STREAM(LoggerPool::remoteTask(), "SshChannel freed");
			}

		}


	} // end namespace cmnutil
} // end namespace basar
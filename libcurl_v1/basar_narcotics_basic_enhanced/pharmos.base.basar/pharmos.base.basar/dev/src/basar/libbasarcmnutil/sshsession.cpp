//----------------------------------------------------------------------------
/*! \file
 *  \brief  RAII like class representing a ssh_session
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//

#include "sshsession.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_remotetaskexceptions.h"
#include "libbasarcmnutil_socket.h"

#include "libbasarcmnutil_icredential.h"

#include "libbasarcmnutil_logging.h"
#include "loggerpool.h"

#ifdef _WIN32
#pragma warning( disable : 4127 )		// because of "do ... while(0)" macros
#endif

#ifdef _WIN32
typedef SOCKET			 Socket;							//!< platform specific socket type
#else
typedef int				 Socket;							//!< platform specific socket type
#endif //_WIN32


//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		SshSession::SshSession(TcpSocketSharedPtr socket) : m_Socket(socket), m_Session(0)
		{
			if (!m_Socket->isOpen())
			{
				VarString msg("socket is not open");

				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), msg);
				
				throw SshSessionSocketNotOpenException(ExceptInfo("basar::cmnutil::SshSession::SshSession()", msg, __FILE__, __LINE__));				
			}
			initSshSession();

			BLOG_INFO_STREAM(LoggerPool::remoteTask(), "SshSession initialized");
		}

		basar::Int32 SshSession::waitSocket()
		{
			struct timeval timeout;
			basar::Int32 rc;
			fd_set fd;
			fd_set *writefd = NULL;
			fd_set *readfd = NULL;
			basar::Int32 dir;

			timeout.tv_sec = 10;
			timeout.tv_usec = 0;

			FD_ZERO(&fd);

			FD_SET(static_cast<Socket>(m_Socket->getSocketDescriptor()), &fd);

			/* now make sure we wait in the correct direction */
			dir = libssh2_session_block_directions(m_Session);

			if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
			{
				readfd = &fd;
			}

			if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
			{
				writefd = &fd;
			}

			rc = select(
#ifdef _WIN32
				0, // avoid warning "conversion from SOCKET to int"
#else
				m_Socket->getSocketDescriptor() + 1,
#endif
				readfd, writefd, NULL, &timeout
			);

			return rc;
		}

		bool SshSession::isSessionInitialized() const
		{
			return (0 != m_Session);
		}

		LIBSSH2_SESSION* SshSession::getRawSession()
		{
			return m_Session;
		}

		SshSession::~SshSession()
		{
			freeSshSession();
		}

		void SshSession::initSshSession()
		{
			int rc = -1;
			
			m_Session = libssh2_session_init();

			if(0 == m_Session)
			{
				//session creation failed, throw
				VarString msg("SSH session init failed (libssh2_session_init returned null-pointer)");

				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), msg);

				throw SshSessionInitFailedException(ExceptInfo("basar::cmnutil::RemoteTaskExecution::initializeSession()", msg, __FILE__, __LINE__));
			}

			//tell libssh2 we want it all done non-blocking (=0)
			libssh2_session_set_blocking(m_Session, false);

			//do handshake
			while ((rc = libssh2_session_handshake(m_Session, m_Socket->getSocketDescriptor())) == LIBSSH2_ERROR_EAGAIN)
            {
                waitSocket();
            }

			if (rc)
			{
				//handshake failed, throw
				std::ostringstream os;

				os << "SSH handshake failed (libssh2_session_handshake returned " << rc << ")";

				BLOG_ERROR_STREAM(LoggerPool::remoteTask(), os.str());
				
				throw SshHandshakeFailedException(ExceptInfo("basar::cmnutil::RemoteTaskExecution::initializeSession()", os.str(), __FILE__, __LINE__));
			}

		}

		void SshSession::freeSshSession()
		{
			if (0 != m_Session)
			{
                int rc = -1;

                while( (rc = libssh2_session_disconnect(m_Session, "Normal Shutdown, Thank you for playing")) == LIBSSH2_ERROR_EAGAIN )
                {
                    waitSocket();
                }

				while ( (rc = libssh2_session_free(m_Session) ) == LIBSSH2_ERROR_EAGAIN )
                {
                    waitSocket();
                }

				BLOG_INFO_STREAM(LoggerPool::remoteTask(), "SshSession freed");
			}
		}

	} // namespace cmnutil
} // end namespace basar
//----------------------------------------------------------------------------
/*! \file
 *  \brief  RAII like class representing a ssh_session
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_SSHSESSION_H
#define GUARD_SSHSESSION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

#include <libssh2.h>

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#include "libbasar_definitions.h"

#include "tcpsocketsharedptr.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		//------------------------------------------------------------------------------------------------------//
		//! \brief SshSession: RAII style class representing the sshsession
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//
		class SshSession
		{
			public:	
				//! \brief c'tor - needs open tcp socket, calls initSshSession
				SshSession(TcpSocketSharedPtr socket);

				//! \brief getter for internal raw session pointer - needed by sshchannel (to call interal library functions)
				LIBSSH2_SESSION* getRawSession();

				//! \brief blocks until socket can be read again
				basar::Int32 waitSocket();

				//! \brief tells whether or not session has been initialized (internal session pointer != null)
				bool isSessionInitialized() const;

				//! \brief d'tor - calls freeSshSession
				~SshSession();
			
			private:
				SshSession(); //!< forbidden - documentary, won't be defaulted

				SshSession(const SshSession&); //!< forbidden
				SshSession& operator=(const SshSession&); //!< forbidden

				void initSshSession(); //!< initializes SshSession by calling internal library functions

				void freeSshSession(); //!< frees SshSession resources

				TcpSocketSharedPtr m_Socket; //!< shared pointer to TcpSocket class

				LIBSSH2_SESSION* m_Session; //!< pointer to SshSession struct
		};
	}
}


#endif // GUARD_SSHSESSION_H

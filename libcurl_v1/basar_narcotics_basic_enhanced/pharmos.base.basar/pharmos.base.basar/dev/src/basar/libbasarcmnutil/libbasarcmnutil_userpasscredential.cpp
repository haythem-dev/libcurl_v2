//----------------------------------------------------------------------------
/*! \file
 *  \brief  username password credential
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_userpasscredential.h"

#include "sshsession.h"

#include "loggerpool.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_remotetaskexceptions.h"

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		UserPassCredential::UserPassCredential() : CredentialBase()
		{
		}

		void UserPassCredential::authenticate(SshSessionPtr session) const
		{
			int rc = -1;

			//form libssh2's documentation:
			//Attempt basic password authentication. Note that many SSH servers which appear to support ordinary password authentication actually have it disabled and use Keyboard Interactive authentication (routed via PAM or another authentication backed) instead.
			//would force us to use certificates - adding 15 times a public key to the authorized keys file of a unix user

			//also use long (_ex) version, short version issues warnings
			while ((rc = libssh2_userauth_password_ex(session->getRawSession(), m_Identity.c_str(), static_cast<unsigned int>(m_Identity.length()), m_Password.c_str(), static_cast<unsigned int>(m_Password.length()), 0)) == LIBSSH2_ERROR_EAGAIN);

			if (rc)
			{
				//todo handle error
				VarString msg;
				msg.format("Login with username >%s< returned %d (libssh2_userauth_password_ex)", m_Identity.c_str(), rc);
				if (LoggerPool::remoteTask().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
				{
					LoggerPool::remoteTask().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
				}
				throw SshLoginUsernamePasswordFailedException(ExceptInfo("basar::cmnutil::UserPassCredential::authenticate()", msg, __FILE__, __LINE__));
			}			
		}
	}
}
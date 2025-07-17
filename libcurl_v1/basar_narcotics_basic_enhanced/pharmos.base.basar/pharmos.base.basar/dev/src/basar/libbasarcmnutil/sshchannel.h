//----------------------------------------------------------------------------
/*! \file
 *  \brief  RAII like class representing a ssh_channel
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SSHCHANNEL_H
#define GUARD_SSHCHANNEL_H

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

#include "libbasarcmnutil_sshsessionptr.h"

#include "libbasarcmnutil_iremotetaskptr.h"
#include "libbasarcmnutil_iremotetaskresultptr.h"

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

#include "remotetaskexitstatus.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
	
		//------------------------------------------------------------------------------------------------------//
		//! \brief SshTaskStatus: enum for all stati of a ssh task (governs if it's possible to read from remote shell)
		//!	\n enum
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//	
		enum SshTaskStatus
		{
			SSH_TASK_NOT_STARTED,		 //!< task has not been started
			SSH_TASK_RUNNING,			 //!< task is running
			SSH_TASK_WAITING_ON_SOCKET,  //!< task is waiting on socket
			SSH_TASK_FINISHED			 //!< task is finished
		};

		//------------------------------------------------------------------------------------------------------//
		//! \brief SshChannel: RAII style class representing the sshchannel
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//		
		class SshChannel
		{
			public:
				//! \brief c'tor - calls initChannel, needs established SshSession
				SshChannel(SshSessionPtr session);

				//! \brief d'tor - calls freeChannel
				~SshChannel();

				//! \brief issues command remotely
				void startProcess(const basar::VarString& command);

				//! \brief indicates whether or not task can have readable output
				SshTaskStatus getTaskStatus() const;

				//! \brief reads remote task's output from stdout
				const VarString& readTaskOutput();

				//! \brief gets remote task's ExitStatus (compound of ExitCode and termination signal)
				const RemoteTaskExitStatus& getExitStatus();

			private:

                enum SshChannelStatus
                {
                    SSH_CHANNEL_UNDEFINED, //<! initial state, was never opened
                    SSH_CHANNEL_OPEN,      //<! channel is open
                    SSH_CHANNEL_CLOSED     //<! channel was open and is now closed
                };

				SshChannel(); //!< forbidden - documentary, won't be defaulted

				SshChannel(const SshChannel& channel); //!< forbidden
				SshChannel& operator=(const SshChannel&); //!< forbidden

				SshSessionPtr    m_Session; //!< shared ptr to SshSession
				LIBSSH2_CHANNEL* m_Channel; //!< pointer to SshChannel struct

				void initChannel(); //!< initalizes channel, called in c'tor
				void freeChannel(); //!< frees channel resources, called in d'tor

				SshTaskStatus m_SshTaskStatus; //!< flag indicating if task can have output

                SshChannelStatus m_SshChannelStatus; //!< internal status of the ssh channel per SshChannelStatus enum

				RemoteTaskExitStatus m_TaskExitStatus; //!< ExitStatus of task, to enable return by reference

				VarString m_TaskOutput; //!< internal cache of remote task's output to stdout - this will cache only the contents of one read operation

		};

	} //end namespace cmnutil
} // end namespace basar


#endif // GUARD_SSHCHANNEL_H


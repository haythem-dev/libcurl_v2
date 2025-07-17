//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASK
#define GUARD_REMOTETASK

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iremotetask.h"
#include "libbasarcmnutil_remoteendpoint.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteTask : public IRemoteTask
		{
			public:

				//! \brief c'tor - constructs empty RemoteTask
				RemoteTask();

				//! \brief d'tor
				~RemoteTask();

				//! \brief setter for endpoint
				virtual void setEndpoint(const RemoteEndpoint& endpoint);

				//! \brief setter for credentials
				virtual void setCredentials(ICredentialPtr credential);

				//! \brief setter for task command (command that is remotly executed)
				virtual void setTaskCommand(const VarString& command);

				//! \brief register listener, listener will be called when new buffer entry is read from remote host
				virtual void registerListener(IRemoteTaskCallBackPtr listener);

				//! \brief method to notify listener, delegates to IRemoteTaskCallBack object's notify member function
				virtual void notifyListeners(const VarString& message) const;

				//! \brief getter for endpoint address - delegated to Endpoint class
				//! \n throw RemoteEndpointAddressEmptyException
				virtual const BString& getRemoteEndpointAddress() const;
				
				//! \brief getter for endpoint port - delegated to Endpoint class
				virtual basar::Int16 getRemoteEndpointPort() const;

				//! \brief getter task command
				//! \n throw RemoteTaskCommandEmptyException
				virtual const basar::VarString& getTaskCommand() const;

				//! \brief method to perform authentication, delegates to ICredential's authenticate
				//! \n throw BasarNullReferenceException
				virtual void authenticateUsingCredentials(SshSessionPtr session) const;

			private:
				//functions
				RemoteTask& operator=(const RemoteTask&); //!< forbidden

				RemoteTask (const RemoteTask&); //!< forbidden

				//! \brief check wether or not task has listeners registered
				bool hasListeners() const;

				//! \brief checks whether or not command is empty
				bool hasCommand() const;

				//! \brief checks whether or not m_credentials is null
				bool hasCredentials() const;

				//! \brief checks whether or not m_Endpoint's address is empty
				bool hasNonEmptyEndpoint() const;

				//variables
				RemoteEndpoint m_Endpoint; //!< endpoint
				basar::VarString m_RemoteCommand; //!< remote command

				IRemoteTaskCallBackPtr m_Listener; //!< pointer to registered listener
				
				ICredentialPtr m_Credential; //!< pointer to credential interface implementation

		};
	}
}

#endif //GUARD_REMOTETASK
//----------------------------------------------------------------------------
/*! \file
 *  \brief  iremote task
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_IREMOTETASK_H
#define GUARD_IREMOTETASK_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_iremotetaskcallbackptr.h"
#include "libbasarcmnutil_icredentialptr.h"
#include "libbasarcmnutil_sshsessionptr.h"

#include "libbasar_definitions.h"
//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteEndpoint;
		class BString;
	}
}


//--------------------------------------------------------------------------------------------------//
//  interface declaration section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		class IRemoteTask
		{
			public:
				/*! \brief virtual(!) d'tor */
				virtual ~IRemoteTask(){};

				//remote host - use either ip4 or dnsname
				virtual void setEndpoint(const RemoteEndpoint& endpoint) = 0;

				virtual void setCredentials(ICredentialPtr credentials) = 0;
				//task to be executed at endpoint
				virtual void setTaskCommand(const BString& command) = 0;

				virtual void registerListener(IRemoteTaskCallBackPtr listener) = 0;

				virtual void notifyListeners(const BString& message) const = 0;

				virtual const BString& getRemoteEndpointAddress() const = 0;
				
				virtual basar::Int16 getRemoteEndpointPort() const = 0;

				virtual const BString& getTaskCommand() const = 0;

				virtual void authenticateUsingCredentials(SshSessionPtr session) const = 0;


		};
	}
}

#endif // GUARD_IREMOTETASK
//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task ref wrapper
 *  \author Marco Köppendörfer
 *  \date   11.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_REMOTETASKREF_H
#define GUARD_LIBBASARCMNUTIL_REMOTETASKREF_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_iremotetaskptr.h"
#include "libbasarcmnutil_icredentialptr.h"
#include "libbasarcmnutil_iremotetaskcallbackptr.h"
//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteEndpoint;
		class BString;
		class RemoteTaskSubmitterImpl;
	}
}


//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		//------------------------------------------------------------------------------------------------------//
		//! \brief RemoteTaskRef: ref wrapper for IRemoteTaskPtr (setter part of IRemoteTask)
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//
		class RemoteTaskRef
		{
			friend class RemoteTaskSubmitterImpl;
		
			public:
				
				//! \brief c'tor - constructs empty RemoteTask
				LIBBASARCMNUTIL_API RemoteTaskRef();

				//! \brief d'tor
				LIBBASARCMNUTIL_API ~RemoteTaskRef();

				//! \brief setter for endpoint - delegates to IRemoteTask's setEndpoint
				LIBBASARCMNUTIL_API void setEndpoint(const RemoteEndpoint& endpoint);

				//! \brief setter for credentials - delegates to IRemoteTask's setCredentials
				LIBBASARCMNUTIL_API void setCredentials(ICredentialPtr credentials);

				//! \brief setter for task command (command that is remotly executed) - delegates to IRemoteTask's setTaskCommand
				LIBBASARCMNUTIL_API void setTaskCommand(const BString& command);

				//! \brief register listener, listener will be called when new buffer entry is read from remote host - delegates to IRemoteTask's registerListener
				LIBBASARCMNUTIL_API void registerListener(IRemoteTaskCallBackPtr listener);

				//! \brief checks whether m_RemoteTask is null
				LIBBASARCMNUTIL_API bool isNull() const;

				//! \brief resets m_RemoteTask
				LIBBASARCMNUTIL_API void reset();

			private:
				IRemoteTaskPtr m_RemoteTask; //!< pointer to IRemoteTask implementation

				const IRemoteTaskPtr& getIImpl(); //!< getter for m_RemoteTask


		};
	}
}

#endif // GUARD_LIBBASARCMNUTIL_REMOTETASKREF_H
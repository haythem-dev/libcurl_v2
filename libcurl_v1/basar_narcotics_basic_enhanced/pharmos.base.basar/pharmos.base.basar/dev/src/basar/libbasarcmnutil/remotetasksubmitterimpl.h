//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task submitter
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKSUBMITTERIMPL_H
#define GUARD_REMOTETASKSUBMITTERIMPL_H


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iremotetasksubmitter.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		//! \brief singleton implementation
		class RemoteTaskSubmitterImpl : public IRemoteTaskSubmitter
		{
			public:

				friend class RemoteTaskSubmitterBase; //!< friend for calling private constructor

				typedef boost::shared_ptr<IRemoteTaskSubmitter> IRemoteTaskSubmitterPtr; //!< typedef for create's return type

				//! \brief static create method
				static IRemoteTaskSubmitterPtr create();

				//! \brief d'tor
				~RemoteTaskSubmitterImpl();
				
				//! \brief sumbits (starts) remote task specified by RemoteTaskRef
				virtual RemoteTaskResultRef submitRemoteTask( RemoteTaskRef remoteTask );
			
			private:
				//! \brief c'tor
				RemoteTaskSubmitterImpl();

				//functions
				RemoteTaskSubmitterImpl& operator=(const RemoteTaskSubmitterImpl&); //!< forbidden
				RemoteTaskSubmitterImpl(const RemoteTaskSubmitterImpl&);			//!< forbidden

				//! \brief function for initializing backend library, called once in c'tor
				void initBackendLibrary();

				//! \brief function for de-initializing backend library, called once in d'tor
				void shutdownBackendLibrary();

				boost::mutex                    m_Guard;                //!< for guarding critical section around calls to backend library
				

		};
	}
}
#endif // GUARD_REMOTETASKSUBMITTERIMPL_H


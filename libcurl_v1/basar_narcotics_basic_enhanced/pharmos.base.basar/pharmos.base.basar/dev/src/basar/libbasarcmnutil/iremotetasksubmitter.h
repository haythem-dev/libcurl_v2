//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for remote task submitter
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_IREMOTETASKSUBMITTER_H
#define GUARD_IREMOTETASKSUBMITTER_H

//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteTaskResultRef;
		class RemoteTaskRef;
	}
}

//--------------------------------------------------------------------------------------------------//
//  interface declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{

		class IRemoteTaskSubmitter
		{
			public:
				
				/*! \brief virtual(!) d'tor */
				virtual ~IRemoteTaskSubmitter(){};

				//submits task synchronous - client has to wait until task finishes
				virtual RemoteTaskResultRef submitRemoteTask( RemoteTaskRef remoteTask ) = 0;
		};
	}
}


#endif // GUARD_IREMOTETASKSUBMITTER_H

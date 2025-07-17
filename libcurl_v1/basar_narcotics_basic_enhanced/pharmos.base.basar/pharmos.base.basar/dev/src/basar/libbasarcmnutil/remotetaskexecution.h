//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for exection of remote task
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKEXECUTION_H
#define GUARD_REMOTETASKEXECUTION_H


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_iremotetaskptr.h"
#include "libbasarcmnutil_iremotetaskresultptr.h"

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

#include "tcpsocketsharedptr.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteTaskExecution
		{
			public:

				//! \brief c'tor
				RemoteTaskExecution(IRemoteTaskPtr task);
				
				//! \brief d'tor
				~RemoteTaskExecution();

				//! \brief execute task specified by internal IRemoteTaskPtr
				//! \throw RemoteTaskExecption
				IRemoteTaskResultPtr execute();

			private:

				RemoteTaskExecution(); //!< forbidden - documentary, won't be defaulted

				RemoteTaskExecution(const RemoteTaskExecution& rte); //!< forbidden
				
				RemoteTaskExecution& operator=(const RemoteTaskExecution& rte); //!< forbidden

				//functions
				//using tcp socket
				void openSocket(); //!< opens socket using task's endpoint
				void closeSocket(); //!< closes socket

				void cleanup(); //!< performs cleanup

				//variables
				IRemoteTaskPtr m_Task; //!< local copy of task ptr

				VarString m_TaskOutput; //!< buffer for all of task's output to stdout

				TcpSocketSharedPtr m_Socket; //!< shared ptr to tcpsocket
		};
	}
}

#endif // GUARD_REMOTETASKEXECUTION
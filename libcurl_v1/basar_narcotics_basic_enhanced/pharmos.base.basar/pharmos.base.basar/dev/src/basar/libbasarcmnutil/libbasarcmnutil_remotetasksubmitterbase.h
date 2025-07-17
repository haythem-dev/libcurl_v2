//----------------------------------------------------------------------------
/*! \file
 *  \brief  singleton class holding remote task submitter instance
 *  \author Marco Köppendörfer
 *  \date   11.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTERBASE_H
#define GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTERBASE_H


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
#include "libbasarcmnutil_remotetasksubmitter.h"
#include <boost/shared_ptr.hpp>
#include "libbasarcmnutil_iremotetaskresultptr.h"
#include "libbasarcmnutil_iremotetaskptr.h"

//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class IRemoteTaskSubmitter;
		class RemoteTaskResultRef;
		class RemoteTaskRef;
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
		//! \brief RemoteTaskSubmitterBase Singleton
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//		
		class RemoteTaskSubmitterBase
		{
			friend class Singleton<RemoteTaskSubmitterBase>; //!< friend for calling private constructor

			public:

				//! \brief submits remote task
				//! \n throw BasarNullReferenceException
				//! \n throw RemoteException
				//! \n throw RemoteException
				LIBBASARCMNUTIL_API RemoteTaskResultRef submitRemoteTask(RemoteTaskRef execInfo);

			private:
				LIBBASARCMNUTIL_API RemoteTaskSubmitterBase(); //!< forbidden

				RemoteTaskSubmitterBase(const RemoteTaskSubmitterBase&); //!< forbidden

				RemoteTaskSubmitterBase& operator=(const RemoteTaskSubmitterBase&); //!< forbidden

				const boost::shared_ptr<IRemoteTaskSubmitter>& getIImpl(); //!< getter for pointer to interface implementation

				boost::shared_ptr<IRemoteTaskSubmitter> m_pIImpl; //!< pointer to interface implementation - hence the second I
		};
	}
}




#endif // GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTERBASE_H
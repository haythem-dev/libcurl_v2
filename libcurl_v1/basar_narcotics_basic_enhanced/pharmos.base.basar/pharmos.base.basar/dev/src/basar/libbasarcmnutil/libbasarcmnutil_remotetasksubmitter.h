//----------------------------------------------------------------------------
/*! \file
 *  \brief  singleton class holding remote task submitter instance
 *  \author Marco Köppendörfer
 *  \date   11.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTER_H
#define GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTER_H

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
#include "libbasarcmnutil_pattern.h"
#include "libbasarcmnutil_remotetasksubmitterbase.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{

		class LIBBASARCMNUTIL_API RemoteTaskSubmitter : public Singleton< RemoteTaskSubmitterBase >
		{
			private:
				RemoteTaskSubmitter();
				RemoteTaskSubmitter(const RemoteTaskSubmitter&);
				RemoteTaskSubmitter& operator=(const RemoteTaskSubmitter&);
		};
	}
}




#endif // GUARD_LIBBASARCMNUTIL_REMOTETASKSUBMITTER_H

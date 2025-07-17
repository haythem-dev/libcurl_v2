//----------------------------------------------------------------------------
/*! \file
 *  \brief  ref wrapper of an iremotetaskresultptr
 *  \author Marco Köppendörfer
 *  \date   10.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_REMOTETASKRESULTREF_H
#define GUARD_LIBBASARCMNUTIL_REMOTETASKRESULTREF_H


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
#include "libbasarcmnutil_iremotetaskresultptr.h"
#include "libbasar_definitions.h"

//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{
		class BString;
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
		//! \brief RemoteTaskResultRef: ref wrapper for IRemoteTaskResultPtr
		//!	\n final class
		//! \n throws no-throw
		//------------------------------------------------------------------------------------------------------//	
		class RemoteTaskResultRef
		{
			public:

				//! \brief c'tor
				LIBBASARCMNUTIL_API RemoteTaskResultRef( IRemoteTaskResultPtr );
			
				//! \brief getter for task's result - output to stdout - delegated to IRemoteTaskResult
				LIBBASARCMNUTIL_API const VarString& getResult() const;

				//! \brief specifies whether task encountered error - either ExitCode != 0 or !Signal.empty() - note: depending on your task, exit by signal may led to correct result - delegated
				LIBBASARCMNUTIL_API bool hasError() const;
                
				//! \brief specifies whether task was terminated by signal - !Signal.empty() - delegated
				LIBBASARCMNUTIL_API bool hasSignal() const;

				//! \brief getter for task result
				LIBBASARCMNUTIL_API basar::Int32 getExitCode() const;

				//! \brief getter for signal if task was teminated by signal - delegated
				LIBBASARCMNUTIL_API const VarString& getSignal() const;

				//! \brief indicates whether internal pointer is null or not
				LIBBASARCMNUTIL_API bool isNull() const;

				//! \brief resets internal pointer
				LIBBASARCMNUTIL_API void reset();

			private:
				IRemoteTaskResultPtr m_pRemoteTaskResult; //!< pointer to IRemoteTask Implementation

				const IRemoteTaskResultPtr& getIImpl() const; //!< getter for m_pRemoteTaskResult
		};
	}
}


#endif // GUARD_LIBBASARCMNUTIL_REMOTETASKRESULTREF_H
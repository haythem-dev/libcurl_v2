//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task result interface
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_IREMOTETASKRESULT_H
#define GUARD_IREMOTETASKRESULT_H

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
//  interface declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class IRemoteTaskResult
		{
			public:

				/*! \brief virtual(!) d'tor */
				virtual ~IRemoteTaskResult(){};

				virtual const BString& getResult() = 0; 

				virtual bool hasError() = 0;

				virtual bool hasSignal() = 0;

				virtual basar::Int32 getExitCode() = 0;

				virtual const BString& getSignal() = 0;
		};
	}
}

#endif // GUARD_IREMOTETASKRESULT_H
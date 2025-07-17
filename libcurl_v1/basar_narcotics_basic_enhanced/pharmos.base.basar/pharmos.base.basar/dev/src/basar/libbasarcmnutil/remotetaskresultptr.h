//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task result ptr
 *  \author Marco Köppendörfer
 *  \date   02.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKRESULTPTR_H
#define GUARD_REMOTETASKRESULTPTR_H


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class RemoteTaskResult;

		/*! \brief  typedef to boost::shared_ptr<RemoteTaskResult> */
		typedef boost::shared_ptr<RemoteTaskResult> RemoteTaskResultPtr;
	}
}


#endif //GUARD_REMOTETASKRESULTPTR_H
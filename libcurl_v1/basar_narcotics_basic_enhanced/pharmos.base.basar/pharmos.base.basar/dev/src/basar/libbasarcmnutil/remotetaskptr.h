//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task ptr
 *  \author Marco Köppendörfer
 *  \date   08.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTETASKPTR_H
#define GUARD_REMOTETASKPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class RemoteTask;

		/*! \brief  typedef to boost::shared_ptr<RemoteTask> */
		typedef boost::shared_ptr<RemoteTask> RemoteTaskPtr;
	}
}

#endif // GUARD_IREMOTETASKPTR_H
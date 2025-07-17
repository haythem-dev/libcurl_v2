//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task result ptr
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_IREMOTETASKRESULTPTR_H
#define GUARD_IREMOTETASKRESULTPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class IRemoteTaskResult;

		/*! \brief  typedef to boost::shared_ptr<IRemoteTaskResult> */
		typedef boost::shared_ptr<IRemoteTaskResult> IRemoteTaskResultPtr;

	}
}

#endif // GUARD_IREMOTETASKRESULTPTR_H

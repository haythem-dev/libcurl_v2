//----------------------------------------------------------------------------
/*! \file
 *  \brief  iremote task ptr
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_IREMOTETASKPTR_H
#define GUARD_IREMOTETASKPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class IRemoteTask;
 
		/*! \brief  typedef to boost::shared_ptr<IRemoteTask> */
		typedef boost::shared_ptr<IRemoteTask> IRemoteTaskPtr;
	}
}

#endif // GUARD_IREMOTETASKPTR_H
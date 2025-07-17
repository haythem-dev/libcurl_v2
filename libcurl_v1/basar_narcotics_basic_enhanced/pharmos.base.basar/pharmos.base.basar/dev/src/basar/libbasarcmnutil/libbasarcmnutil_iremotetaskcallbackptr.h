//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task callback ptr
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_IREMOTETASKCALLBACKPTR_H
#define GUARD_IREMOTETASKCALLBACKPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class IRemoteTaskCallBack;

		/*! \brief  typedef to boost::shared_ptr<IRemoteTaskCallBack> */
		typedef boost::shared_ptr<IRemoteTaskCallBack> IRemoteTaskCallBackPtr;

	}
}

#endif // GUARD_IREMOTETASKCALLBACKPTR_H
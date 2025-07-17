//----------------------------------------------------------------------------
/*! \file
 *  \brief  ssh session shared ptr
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SSHSESSIONPTR_H
#define GUARD_SSHSESSIONPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class SshSession;

		/*! \brief  typedef to boost::shared_ptr<SshSession> */
		typedef boost::shared_ptr<SshSession> SshSessionPtr;

	}
}

#endif // GUARD_SSHSESSIONPTR_H
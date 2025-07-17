//----------------------------------------------------------------------------
/*! \file
 *  \brief  credentialbase ptr
 *  \author Marco Köppendörfer
 *  \date   08.10.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CREDENTIALBASEPTR_H
#define GUARD_CREDENTIALBASEPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class CredentialBase;

		/*! \brief  typedef to boost::shared_ptr<CredentialBase> */
		typedef boost::shared_ptr<CredentialBase> CredentialBasePtr;

	}
}

#endif // GUARD_CREDENTIALBASEPTR_H
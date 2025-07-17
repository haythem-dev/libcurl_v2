//----------------------------------------------------------------------------
/*! \file
 *  \brief  icredential ptr
 *  \author Marco Köppendörfer
 *  \date   07.10.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ICREDENTIALPTR_H
#define GUARD_ICREDENTIALPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class ICredential;

		/*! \brief  typedef to boost::shared_ptr<ICredential> */
		typedef boost::shared_ptr<ICredential> ICredentialPtr; 

	}
}

#endif // GUARD_ICREDENTIALPTR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  credential interface (getter part)
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ICREDENTIAL_H
#define GUARD_ICREDENTIAL_H

//--------------------------------------------------------------------------------------------------//
//  include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_sshsessionptr.h"

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
		//class SshSession;
		class ICredential
		{
			public:

				/*! \brief virtual(!) d'tor */
				virtual ~ICredential() {};

				/*! \brief virtual method to perform authentication */
				virtual void authenticate(SshSessionPtr session) const = 0;
		};
	}
}


#endif //GUARD_ICREDENTIAL_H
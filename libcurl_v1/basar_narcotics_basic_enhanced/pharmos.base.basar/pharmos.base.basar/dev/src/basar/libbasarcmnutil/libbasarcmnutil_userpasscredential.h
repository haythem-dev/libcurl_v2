//----------------------------------------------------------------------------
/*! \file
 *  \brief  username password credential
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_USERPASSCREDENTIAL_H
#define GUARD_USERPASSCREDENTIAL_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarcmnutil_credentialbase.h"

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class UserPassCredential : public CredentialBase
		{
			public:
				//! \brief c'tor
				LIBBASARCMNUTIL_API UserPassCredential();

				//! \brief perform authentication using username and password
				LIBBASARCMNUTIL_API void authenticate(SshSessionPtr session) const;
		};
	}
}

#endif //GUARD_USERPASSCREDENTIAL_H

//----------------------------------------------------------------------------
/*! \file
 *  \brief  credential base
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CREDENTIALBASE
#define GUARD_CREDENTIALBASE

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
#include "libbasarcmnutil_icredential.h"
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"


//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{		

		//----------------------------------------------------------------------------
		/*! \brief Common base class for credentials which implements the ICredential interface

			This is a abstract base class.
		 */
		class CredentialBase : public ICredential
		{
			public:
				/*! \brief virtual(!) d'tor */
				LIBBASARCMNUTIL_API virtual ~CredentialBase();

				/*! \brief set identity (username) */
				LIBBASARCMNUTIL_API void setIdentity(const basar::VarString& identity); 
				
				/*! \brief set password for login or for private key */
				LIBBASARCMNUTIL_API void setPassword(const basar::VarString& password);

				/*! \brief virtual method to perform authentication */
				LIBBASARCMNUTIL_API virtual void authenticate(SshSessionPtr session) const = 0;

			protected:
				
				basar::VarString m_Identity; //!< VarString storing login name

				basar::VarString m_Password; //!< VarString storing login password or password for private key


			private:
				
				
		};
	}
}



#endif // GUARD_CREDENTIALBASE
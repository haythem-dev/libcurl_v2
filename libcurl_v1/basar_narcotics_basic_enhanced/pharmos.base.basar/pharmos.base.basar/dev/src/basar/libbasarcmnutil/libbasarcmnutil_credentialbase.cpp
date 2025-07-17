//----------------------------------------------------------------------------
/*! \file
 *  \brief  credential base
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_credentialbase.h"


//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		CredentialBase::~CredentialBase()
		{
		}
		
		void CredentialBase::setIdentity(const basar::VarString& identity)
		{
			m_Identity = identity;
		}

		void CredentialBase::setPassword(const basar::VarString& password)
		{
			m_Password = password;
		}			
	}
}




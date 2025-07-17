#ifndef GUARD_ADUSERCHECKERUNIX_H
#define GUARD_ADUSERCHECKERUNIX_H

//----------------------------------------------------------------------------
#include "iaduserchecker.h"
#include "libbasarcmnutil_i18nstring.h"

#include "configurationptr.h"

#include <ldap.h>


//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {


//----------------------------------------------------------------------------
//! login implementation class \n final class
        class ADUserCheckerUnix : public IADUserChecker
        {

            public:

                /*! \brief constructer
                    \n     no-throw */
                ADUserCheckerUnix( Configurationptr config );

                /*! \brief destructor
                    \n     no-throw */
                virtual ~ADUserCheckerUnix();

                /*! \brief performing login operation
                    \n     no-throw */
                virtual bool doLogin( const basar::cmnutil::CollBString& sUserName, const basar::VarString& sPassword );

                virtual bool isAccountActive( const basar::VarString& accountName );

            private:

                bool createLdapConnection();

                void shutdownLdapConnection();

                LDAPMessage* findLdapUserEntry( const basar::VarString& accountName );

                inline bool isAccountActive( LDAPMessage* ldapAccountEntry );

                bool getDistinguishedName( const basar::cmnutil::CollBString& sUserName, basar::VarString& sDistinguishedName );

                LDAP*                   m_pldapConnection;     //  Pointer to the connection handle.

                Configurationptr m_Config;
        };

//----------------------------------------------------------------------------
    }   // namespace login
}   // namespace basar

#endif  // GUARD

#ifdef _WIN32
#ifndef GUARD_ADUSERCHECKERWINDOWS_H
#define GUARD_ADUSERCHECKERWINDOWS_H

//----------------------------------------------------------------------------
#include <windows.h>
#include <winldap.h>
#include <ntldap.h>

#include "libbasarcmnutil_i18nstring.h"
#include "iaduserchecker.h"
#include "configurationptr.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {


//----------------------------------------------------------------------------
//! login implementation class \n final class
        class ADUserCheckerWindows : public IADUserChecker
        {
            public:

                static bool verifyServer( PLDAP, PCCERT_CONTEXT* ppServerCert );

                static bool queryClientCert ( PLDAP , PSecPkgContext_IssuerListInfoEx , PCCERT_CONTEXT* );

            public:

                /*! \brief constructer
                    \n     no-throw */
                ADUserCheckerWindows( Configurationptr config );

                /*! \brief destructor
                    \n     no-throw */
                virtual ~ADUserCheckerWindows();

                /*! \brief performing login operation
                    \n     no-throw */
                virtual bool doLogin( const basar::cmnutil::CollBString& sUserName, const basar::VarString& sPassword );

                virtual bool isAccountActive( const basar::VarString& accountName );

            private:

                bool createLdapConnection();

                void shutdownLdapConnection();

                //message must be freed by caller
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
#endif  //_WIN32

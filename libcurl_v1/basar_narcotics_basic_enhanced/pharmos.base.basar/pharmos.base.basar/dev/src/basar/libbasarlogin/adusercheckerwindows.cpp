#include "adusercheckerwindows.h"

#include "loginappl.h"
#include "loggerpool.h"


//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {
        //----------------------------------------------------------------------------
            /*! \param config configuration object
                \throw no-throw */
        ADUserCheckerWindows::ADUserCheckerWindows( Configurationptr config ) : m_pldapConnection( NULL ), m_Config( config )
        {
        }

        bool ADUserCheckerWindows::verifyServer( PLDAP, PCCERT_CONTEXT* ppServerCert )
        {
            //MS documentation states the ppServerCert has to be freed
            CertFreeCertificateContext( *ppServerCert );
            return true;
        }

        bool ADUserCheckerWindows::queryClientCert ( PLDAP , PSecPkgContext_IssuerListInfoEx , PCCERT_CONTEXT* )
        {
            //return false b/c we don't want to supply a cert
            return false;
        }
        //----------------------------------------------------------------------------
        /*! \throw  no-throw */
        ADUserCheckerWindows::~ADUserCheckerWindows()
        {
            //  Free connection.
            shutdownLdapConnection();
        }

        bool ADUserCheckerWindows::doLogin( const basar::cmnutil::CollBString& sUserName, const basar::VarString& sPassword )
        {
            basar::VarString sSearchBaseDN = "";

            bool loginSuccessful = false;

            if ( false == createLdapConnection() )
            {
                BLOG_ERROR( LoggerPool::checkLogin(), "creation of ldap connection failed, returning..." );
                return false;
            }

            // Fetch the base DN from AD directory.
            if ( false == getDistinguishedName( sUserName, sSearchBaseDN ) )
            {
                return false;
            }

            basar::BULong      err = LDAP_OPERATIONS_ERROR;//  Return value for LDAP operations.

            //maybe: ldap_bind_s
            //see https://msdn.microsoft.com/en-us/library/aa366156%28v=vs.85%29.aspx
            //and https://msdn.microsoft.com/en-us/library/aa380132%28v=vs.85%29.aspx

            err = ldap_simple_bind_s( m_pldapConnection,      //  Connection handle
                                      const_cast<char*>( sSearchBaseDN    .c_str() ), //  Null pointer
                                      const_cast<char*>( sPassword        .c_str() )
                                    );

            //if bind failed -> password did not match with user name
            if ( LDAP_SUCCESS == err )
            {
                loginSuccessful = true;
            }

            shutdownLdapConnection();

            return loginSuccessful;
        }

        bool ADUserCheckerWindows::isAccountActive( const basar::VarString& accountName )
        {
            if ( true == accountName.empty() )
            {
                return false;
            }

            if ( false == createLdapConnection() )
            {
                BLOG_ERROR( LoggerPool::checkLogin(), "creation of ldap connection failed, returning..." );
                return false;
            }

            LDAPMessage* userDetails = findLdapUserEntry( accountName );

            bool accountEnabled = isAccountActive( userDetails );

            ldap_msgfree( userDetails );

            shutdownLdapConnection();

            return accountEnabled;
        }

        bool ADUserCheckerWindows::getDistinguishedName( const basar::cmnutil::CollBString& sUserName, basar::VarString& sDistinguishedName )
        {
            LDAPMessage* userDetails = findLdapUserEntry( sUserName.back() );

            bool accountEnabled = isAccountActive( userDetails );

            if ( false == accountEnabled )
            {
                BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "account <" << sUserName.back() << "> disabled in AD" );
                return false;
            }

            // cycle through all objects returned with our search
            for ( LDAPMessage* entry = ldap_first_entry( m_pldapConnection, userDetails );
                    entry != NULL; entry = ldap_next_entry( m_pldapConnection, entry ) )
            {
                // Fetch distinguished name of the user.
                sDistinguishedName = ldap_get_dn( m_pldapConnection, entry );
            }

            ldap_msgfree( userDetails );
            return true;
        }

        bool ADUserCheckerWindows::createLdapConnection()
        {
            if ( NULL == m_pldapConnection )
            {
                m_pldapConnection = ldap_init(
                                        const_cast<char*>( m_Config->getLdapServer().c_str() ),
                                        m_Config->getTcpPort().stoi() );

                if ( m_pldapConnection == NULL )
                {
                    BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "could not initialize ldap connection, error code was <" << LdapGetLastError() << ">" );
                    return false;
                }

                //Set the options on connection blocks to specify LDAP version 3.
                m_pldapConnection->ld_lberoptions = 0;

                basar::Int32            version = LDAP_VERSION3;
                ldap_set_option( m_pldapConnection, LDAP_OPT_VERSION, &version );

                //ignore server cert
                ldap_set_option( m_pldapConnection, LDAP_OPT_SERVER_CERTIFICATE, &ADUserCheckerWindows::verifyServer );
                //tell server we don't offer a client cert
                ldap_set_option( m_pldapConnection, LDAP_OPT_CLIENT_CERTIFICATE, &ADUserCheckerWindows::queryClientCert );

                int connectSuccess = ldap_connect( m_pldapConnection, NULL );

                if ( LDAP_SUCCESS != connectSuccess )
                {
                    BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "could not connect to server <" << m_Config->getLdapServer() << ">, error code was <" << LdapGetLastError() << ">" );
                    return false;
                }

                connectSuccess = ldap_start_tls_s( m_pldapConnection, NULL, NULL, NULL, NULL );

                if ( LDAP_SUCCESS != connectSuccess )
                {
                    BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "could not start tls with server <" << m_Config->getLdapServer() << ">, error code was <" << LdapGetLastError() << ">" );
                    return false;
                }
            }

            return true;
        }

        void ADUserCheckerWindows::shutdownLdapConnection()
        {
            if ( NULL != m_pldapConnection )
            {
                ldap_stop_tls_s( m_pldapConnection );
                ldap_unbind( m_pldapConnection );
                m_pldapConnection = NULL;
            }
        }

        inline bool ADUserCheckerWindows::isAccountActive( LDAPMessage* ldapAccountEntry )
        {
            bool accountEnabled = false;

            if ( NULL == ldapAccountEntry )
            {
                return accountEnabled;
            }

            // cycle through all objects returned with our search
            for ( LDAPMessage* entry = ldap_first_entry( m_pldapConnection, ldapAccountEntry );
                    entry != NULL; entry = ldap_next_entry( m_pldapConnection, entry ) )
            {
                char** uacValues = ldap_get_values( m_pldapConnection, entry, "userAccountControl" );

                int count = ldap_count_values( uacValues );

                if ( count > 0 )
                {
                    if ( NULL != uacValues[0] )
                    {
                        int uacValueInt = atoi( uacValues[0] );

                        accountEnabled = ( ( 0x0200 & uacValueInt ) == 0x0200 );
                    }
                }

                //free buffer
                ldap_value_free( uacValues );
            }

            return accountEnabled;
        }

        LDAPMessage* ADUserCheckerWindows::findLdapUserEntry( const basar::VarString& accountName )
        {
            basar::ULong32 lReturnVal    = LDAP_OPERATIONS_ERROR;

            basar::I18nString sAuthPassword  = m_Config->getPassword();
            basar::I18nString sAuthUserName  = m_Config->getDistingushedName();

            basar::VarString sSearchCriteria = "";

            LDAPMessage* userDetails = NULL;

            // Search Filter
            sSearchCriteria.format( "(&(objectClass=user)(sAMAccountName=%s))", accountName.c_str() );

            lReturnVal = ldap_simple_bind_sA( m_pldapConnection, const_cast<char*>( sAuthUserName.c_str() ), const_cast<char*>( sAuthPassword.c_str() ) );

            if ( lReturnVal != LDAP_SUCCESS )
            {
                BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "ldap_simple_bind_s failed < " << lReturnVal << ">, AuthDetails <" << sAuthUserName << ", " << sAuthPassword << ">" );

                return NULL;
            }

            lReturnVal    = LDAP_OPERATIONS_ERROR;

            // Perform the ldap search
            lReturnVal = ldap_search_ext_sA( m_pldapConnection,
                                             "DC=phoenix,DC=loc",
                                             LDAP_SCOPE_SUBTREE,
                                             const_cast<char*>( sSearchCriteria.c_str() ),
                                             NULL,                       // return all attributes
                                             0,                          // want both types and values of attrs
                                             NULL, NULL, LDAP_NO_LIMIT, LDAP_NO_LIMIT,
                                             &userDetails                // ldap will allocate room for return messages
                                           );

            if ( lReturnVal != LDAP_SUCCESS )
            {
                if ( NULL != userDetails )
                {
                    ldap_msgfree( userDetails );
                }

                BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "ldap_search_ext_s failed < " << lReturnVal << ">" );
                return NULL;
            }

            // Return the number of objects found during the search
            int countEntries = ldap_count_entries( m_pldapConnection, userDetails );

            if ( 0 == countEntries )
            {
                BLOG_ERROR( LoggerPool::checkLogin(), "ldap_count_entries is zero" );
                return NULL;
            }

            if ( 1 != countEntries )
            {
                BLOG_WARN_STREAM( LoggerPool::checkLogin(), "ldap_count_entries returned <" << countEntries << "> instead of 1, results might be unexpected" );
            }

            return userDetails;
        }

        //---------------------------------------------------------------------------
    } // login
} // basar
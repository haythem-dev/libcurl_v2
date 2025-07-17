#define LDAP_DEPRECATED 1

#include "adusercheckerunix.h"
#include "loginappl.h"

#include "libbasarcmnutil_logging.h"
#include "loggerpool.h"

#include <sstream>

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {
        //----------------------------------------------------------------------------
        /*! \param config configuration object
            \throw no-throw */
        ADUserCheckerUnix::ADUserCheckerUnix( Configurationptr config ) : m_pldapConnection( NULL ), m_Config( config )
        {
        }

        //----------------------------------------------------------------------------
        /*! \throw  no-throw */
        ADUserCheckerUnix::~ADUserCheckerUnix()
        {
            ldap_memfree( m_pldapConnection );
        }

        bool ADUserCheckerUnix::doLogin( const basar::cmnutil::CollBString& sUserName, const basar::VarString& sPassword )
        {
            basar::VarString sSearchBaseDN  = "";

            bool loginSuccessful = false;

            if ( false == createLdapConnection() )
            {
                BLOG_ERROR( LoggerPool::checkLogin(), "creation of ldap connection failed, returning..." );
                return false;
            }

            //fetch the base DN from AD
            if ( false == getDistinguishedName( sUserName, sSearchBaseDN ) )
            {
                return false;
            }

            BerValue cred;
            cred.bv_val = const_cast<char*>( sPassword.c_str() );
            cred.bv_len = strlen( sPassword.c_str() );

            basar::ULong32      err = LDAP_OPERATIONS_ERROR;//  Return value for LDAP operations.

            //perform simple bind
            err = ldap_sasl_bind_s ( m_pldapConnection, sSearchBaseDN.c_str(), NULL, &cred, NULL, NULL, NULL );

            //if bind failed -> password did not match with user name
            if ( LDAP_SUCCESS == err )
            {
                loginSuccessful = true;
            }

            shutdownLdapConnection();

            return loginSuccessful;

        }

        bool ADUserCheckerUnix::isAccountActive( const basar::VarString& accountName )
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

        bool ADUserCheckerUnix::getDistinguishedName( const basar::cmnutil::CollBString& sUserName, basar::VarString& sDistinguishedName )
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

        bool ADUserCheckerUnix::createLdapConnection()
        {
            if ( NULL == m_pldapConnection )
            {
                std::ostringstream os;

                os << "ldap:://";
                os << m_Config->getLdapServer();
                os << ":";
                os << m_Config->getTcpPort();

                int rc = ldap_initialize( &m_pldapConnection, os.str().c_str() );

                //  If the connect function fails, show an error message and go to the error section.
                if ( m_pldapConnection == NULL || LDAP_SUCCESS != rc )
                {
                    BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "connect to <" << os.str() << "> failed: <" << ldap_err2string( rc ) << ">" );
                    return false;
                }

                //ldap pointer set to null: set options globally
                //do not require a cert
                const int reqCert = LDAP_OPT_X_TLS_NEVER;
                ldap_set_option( NULL, LDAP_OPT_X_TLS_REQUIRE_CERT, &reqCert );
                //do not check for cert revocation
                const int clrCheck = LDAP_OPT_X_TLS_CRL_NONE;
                ldap_set_option( NULL, LDAP_OPT_X_TLS_CRLCHECK, &clrCheck );

                //use ldap version 3
                const int version = LDAP_VERSION3;
                ldap_set_option( m_pldapConnection, LDAP_OPT_PROTOCOL_VERSION, &version );

                //start tls encrypt
                rc = ldap_start_tls_s( m_pldapConnection, NULL, NULL );

                if ( LDAP_SUCCESS != rc )
                {
                    BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "could not start tls: <" << ldap_err2string( rc ) << ">" );
                    return false;
                }
            }

            return true;

        }

        void ADUserCheckerUnix::shutdownLdapConnection()
        {
            if ( NULL != m_pldapConnection )
            {
                ldap_unbind_ext( m_pldapConnection, NULL, NULL );
                ldap_memfree( m_pldapConnection );
                m_pldapConnection = NULL;
            }
        }

        LDAPMessage* ADUserCheckerUnix::findLdapUserEntry( const basar::VarString& accountName )
        {
            basar::ULong32 lReturnVal    = LDAP_OPERATIONS_ERROR;

            basar::I18nString sAuthPassword  = m_Config->getPassword();
            basar::I18nString sAuthUserName  = m_Config->getDistingushedName();

            basar::VarString sSearchCriteria = "";

            LDAPMessage* userDetails = NULL;

            // Search Filter
            sSearchCriteria.format( "(&(objectClass=user)(sAMAccountName=%s))", accountName.c_str() );

            lReturnVal = ldap_simple_bind_s( m_pldapConnection, const_cast<char*>( sAuthUserName.c_str() ), const_cast<char*>( sAuthPassword.c_str() ) );

            if ( lReturnVal != LDAP_SUCCESS )
            {
                BLOG_ERROR_STREAM( LoggerPool::checkLogin(), "ldap_simple_bind_s failed < " << lReturnVal << ">, AuthDetails <" << sAuthUserName << ", " << sAuthPassword << ">" );

                return NULL;
            }

            lReturnVal    = LDAP_OPERATIONS_ERROR;

            // Perform the ldap search
            lReturnVal = ldap_search_ext_s( m_pldapConnection,
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

        inline bool ADUserCheckerUnix::isAccountActive( LDAPMessage* ldapAccountEntry )
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

        //---------------------------------------------------------------------------
    } // login
} // basar

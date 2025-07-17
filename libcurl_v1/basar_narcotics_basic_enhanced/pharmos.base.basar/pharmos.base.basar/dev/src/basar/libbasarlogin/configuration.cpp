#include "configuration.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_config.h"
#include "libbasarlogin_exceptions.h"
#include "loggerpool.h"

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::current_path;

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------

        //default parameters
        static basar::ConstString LDAPSERVER                    = "10.130.247.202"                  ;       //!< ip address of ldap server
        static basar::ConstString LDAP_TCP_PORT                 = "389"                             ;       //!< port of ldap server
        static basar::ConstString SERVICE_ACCOUNT_DN            = "CN=svc - UADM Lookup,OU=Services,OU=Global,DC=phoenix,DC=loc"; //!< distinguished name of service account
        static basar::ConstString SERVICE_ACCOUNT_PASSWORD      = "Ago2Tod6";                               //!< password of service account

        //configuration paths
        static basar::ConstString AD_LOGIN                      =  "AD"                             ;       //!< value of login type config key for AD login
        static basar::ConstString CONFIGKEY_LOGINTYPE           =  "basar.login.logintype"          ;       //!< configuration key login type
        static basar::ConstString CONFIGKEY_LDAP_SERVER         =  "basar.login.adlogin.ldapserver" ;       //!< configuration key ldap server ip
        static basar::ConstString CONFIGKEY_TCP_PORT            =  "basar.login.adlogin.tcpport"    ;       //!< configuration key ldap server port
        static basar::ConstString CONFIGKEY_COMMONNAME          =  "basar.login.adlogin.commonname" ;       //!< configuration key account_cn
        static basar::ConstString CONFIGKEY_PASSWORD            =  "basar.login.adlogin.password"   ;       //!< configuration key password
        static basar::ConstString CONFIGKEY_DN_PREFIX           =  "basar.login.adlogin.dn_prefix"  ;       //!< configuration key dn_prefix
        static basar::ConstString CONFIGKEY_DN_POSTFIX          =  "basar.login.adlogin.dn_postfix" ;       //!< configuration key dn_postfix

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        Configuration::Configuration()
        {
            m_ADLogin = loginType::LOGINTYPE_NOT_SET;

            readConfiguration();
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        Configuration::~Configuration()
        {
        }

        /*\throw no-throw */
        void Configuration::readConfiguration()
        {
            using basar::cmnutil::Config;

            bool cfgFound = false;

            VarString val;

            std::ostringstream os;

            Config::getInstance().toStream( os );

            BLOG_DEBUG_STREAM( LoggerPool::loginData(), os.str() );

            if ( Config::getInstance().getValue( CONFIGKEY_LDAP_SERVER, val ) )
            {
                m_LdapServer = val;
                cfgFound = true;
            }

            if ( Config::getInstance().getValue( CONFIGKEY_TCP_PORT, val ) )
            {
                m_TcpPort    = val;
                cfgFound = true;
            }

            if ( Config::getInstance().getValue( CONFIGKEY_LOGINTYPE, val ) )
            {
                if ( val == AD_LOGIN )
                {
                    m_ADLogin = loginType::LOGINTYPE_AD;
                }
                else
                {
                    m_ADLogin = loginType::LOGINTYPE_NON_AD;
                }

                cfgFound = true;
            }

            basar::VarString dn;

            if ( Config::getInstance().getValue( CONFIGKEY_DN_PREFIX, val ) )
            {
                dn = val;
            }

            if ( Config::getInstance().getValue( CONFIGKEY_COMMONNAME, val ) )
            {
                dn.append( val );
            }

            if ( Config::getInstance().getValue( CONFIGKEY_DN_POSTFIX, val ) )
            {
                dn.append( val );

                m_DN = dn;
                cfgFound = true;
            }

            if ( Config::getInstance().getValue( CONFIGKEY_PASSWORD, val ) )
            {
                m_Password = val;
                cfgFound = true;
            }

            if ( false == cfgFound )
            {
                setProductionDefaults();
            }
        }

//----------------------------------------------------------------------------

        void Configuration::setProductionDefaults()
        {
            m_LdapServer    = LDAPSERVER;
            m_TcpPort       = LDAP_TCP_PORT;

            m_DN            = SERVICE_ACCOUNT_DN;
            m_Password      = SERVICE_ACCOUNT_PASSWORD;
        }

//----------------------------------------------------------------------------

        bool Configuration::isADLogin()
        {
            return m_ADLogin == loginType::LOGINTYPE_AD;
        }

//----------------------------------------------------------------------------

        loginType::LoginTypeEnum Configuration::getLoginType() const
        {
            return m_ADLogin;
        }

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return const basar::VarString& - the ldap servers network address
        const basar::VarString& Configuration::getLdapServer        () const
        {
            return m_LdapServer;
        }

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return const basar::VarString& - the ldap servers network port
        const basar::VarString& Configuration::getTcpPort           () const
        {
            return m_TcpPort;
        }

        const basar::VarString& Configuration::getDistingushedName() const
        {
            return m_DN;
        }

        const basar::VarString& Configuration::getPassword() const
        {
            return m_Password;
        }

//----------------------------------------------------------------------------
    }//login
}//basar

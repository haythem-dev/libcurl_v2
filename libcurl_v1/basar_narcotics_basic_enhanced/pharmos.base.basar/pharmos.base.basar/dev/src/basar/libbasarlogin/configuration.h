#ifndef GUARD_CONFIGURATION_LIBBASARLOGIN_H
#define GUARD_CONFIGURATION_LIBBASARLOGIN_H

//----------------------------------------------------------------------------
#include "logintype.h"
#include "libbasarcmnutil_i18nstring.h"

#include <boost/shared_ptr.hpp>

namespace loginType
{
    enum LoginTypeEnum;
}
//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//---------------------------------------------------------------------------
        /*! \brief class for use case configuration; handling standard and backup connect
            \n throws BasarException
            \n final class */
        class Configuration
        {

            public:

                /*! \brief std-destructor
                    \n no-throw */
                virtual ~Configuration();


                /*! \brief return true for AD login
                    \n no-throw */
                bool isADLogin() ;

                loginType::LoginTypeEnum getLoginType() const;

                /*! \brief return name of host (backup)
                    \n no-throw */
                const basar::VarString& getLdapServer       () const;

                /*! \brief return name of database (backup)
                    \n no-throw */
                const basar::VarString& getTcpPort          () const;

                const basar::VarString& getDistingushedName() const;

                const basar::VarString& getPassword() const;

                /*! \brief std-constructor
                    \n no-throw */
                Configuration();

            private:

                ///*! \brief set main connection infos to m_DbInfo
                //\n no-throw */
                //void setLdapServer( const basar::VarString    & ldapServer );
                //
                ///*! \brief set backup connection infos to m_DbInfo
                //  \n no-throw */
                //void setTcpPort( const basar::VarString   & ldapServer );

//   void setPassword( const basar::VarString& password );

//   void setDN ( const basar::VarString& dn );

                //! \brief evaluate config parameters for configuration
                //! \n     no-throw
                void readConfiguration();

                void setProductionDefaults();

                basar::VarString                m_LdapServer;       //!< LDAP Server
                basar::VarString                m_TcpPort;      //!< TCP Port
                basar::VarString                m_Password;
                basar::VarString                m_DN;

                loginType::LoginTypeEnum        m_ADLogin;  //!< AD login flag
        };

//----------------------------------------------------------------------------
    } // login
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD

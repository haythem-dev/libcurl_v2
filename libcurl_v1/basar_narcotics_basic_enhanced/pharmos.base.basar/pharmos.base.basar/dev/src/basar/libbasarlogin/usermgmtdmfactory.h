#ifndef GUARD_USERMGMTDMFACTORY_H
#define GUARD_USERMGMTDMFACTORY_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "baseusermgmtdm.h"
#include "configurationptr.h"
#include "configuration.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//---------------------------------------------------------------------------
        /*! \brief class for use case configuration; handling standard and backup connect
            \n throws BasarException
            \n final class */
        class UserMgmtDMFactory
        {

            public:

                typedef boost::shared_ptr< UserMgmtDMFactory >   UserMgmtDMFactoryPtr;

                /*! \brief std-constructor
                    \n no-throw */
                UserMgmtDMFactory( basar::login::Configurationptr );

                /*! \brief std-destructor
                    \n no-throw */
                ~UserMgmtDMFactory();


                /*! \brief returns usermanagementdm pointer
                    \n no-throw */
                domMod::BaseUserManagement::BaseUserManagementSharedPtr         getUserManagementDM();

                /*! \brief resets usermanagementdm pointer
                    \n no-throw */
                void                                                            reset();

            private:

                //! < configuration shared pointer
                basar::login::Configurationptr                  m_ConfigurationPtr;

                //! < userDM shared pointer
                domMod::BaseUserManagement::BaseUserManagementSharedPtr         m_UserDMPtr;
        };

    }
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD

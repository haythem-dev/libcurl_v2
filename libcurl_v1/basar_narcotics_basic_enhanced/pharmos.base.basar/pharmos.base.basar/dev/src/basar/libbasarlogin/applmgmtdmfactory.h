#ifndef GUARD_APPLMGMTDMFACTORY_H
#define GUARD_APPLMGMTDMFACTORY_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "baseapplmgmtdm.h"
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
        class ApplMgmtDMFactory
        {

            public:

                typedef boost::shared_ptr< ApplMgmtDMFactory >   ApplMgmtDMFactoryPtr;

                /*! \brief std-constructor
                    \n no-throw */
                ApplMgmtDMFactory( basar::login::Configurationptr );

                /*! \brief std-destructor
                    \n no-throw */
                ~ApplMgmtDMFactory();


                /*! \brief return applcationmgmt shared ptr
                    \n no-throw */
                domMod::BaseApplManagement::BaseApplManagementSharedPtr         getApplManagementDM();

                /*! \brief resets applcationmgmt shared ptr
                    \n no-throw */
                void                                                            reset();

            private:

                //! < configuration shared pointer
                basar::login::Configurationptr                  m_ConfigurationPtr;

                //! < applicationDM shared pointer
                domMod::BaseApplManagement::BaseApplManagementSharedPtr         m_ApplDMPtr;
        };

    }
} // basar

//----------------------------------------------------------------------------
#endif  // GUARD

#include "applmgmtdmfactory.h"
#include "adloginapplmgmtdm.h"
#include "applmanagementdm.h"

using namespace basar::login;
using namespace domMod;

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {
        ApplMgmtDMFactory::ApplMgmtDMFactory( Configurationptr configuration ):
            m_ConfigurationPtr( configuration )
        {
        }

        ApplMgmtDMFactory::~ApplMgmtDMFactory()
        {
            reset();
        }

        BaseApplManagement::BaseApplManagementSharedPtr     ApplMgmtDMFactory::getApplManagementDM()
        {
            if( m_ApplDMPtr == NULL )
            {
                if( m_ConfigurationPtr->isADLogin() )
                {
                    m_ApplDMPtr = BaseApplManagement::BaseApplManagementSharedPtr( new domMod::ADLoginApplMgmt );
                }
                else
                {
                    m_ApplDMPtr = BaseApplManagement::BaseApplManagementSharedPtr( new domMod::ApplManagement );
                }
            }

            return m_ApplDMPtr;
        }

        void    ApplMgmtDMFactory::reset()
        {
            m_ApplDMPtr.reset();
        }
    }
}//basar

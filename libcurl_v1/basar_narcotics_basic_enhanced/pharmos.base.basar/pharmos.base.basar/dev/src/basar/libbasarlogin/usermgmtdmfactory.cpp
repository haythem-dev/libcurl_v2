#include "usermgmtdmfactory.h"
#include "adloginusermgmtdm.h"
#include "usermanagementdm.h"

using namespace basar::login;
using namespace domMod;

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

        UserMgmtDMFactory::UserMgmtDMFactory( Configurationptr configuration ) : m_ConfigurationPtr( configuration )
        {
        }

        UserMgmtDMFactory::~UserMgmtDMFactory()
        {
            reset();
        }

        BaseUserManagement::BaseUserManagementSharedPtr UserMgmtDMFactory::getUserManagementDM()
        {
            if ( m_UserDMPtr == NULL )
            {
                if ( m_ConfigurationPtr->isADLogin() )
                {
                    m_UserDMPtr = BaseUserManagement::BaseUserManagementSharedPtr( new domMod::ADLoginUserMgmt );
                }
                else
                {
                    m_UserDMPtr = BaseUserManagement::BaseUserManagementSharedPtr( new domMod::UserManagement );
                }
            }

            return m_UserDMPtr;
        }

        void UserMgmtDMFactory::reset()
        {
            m_UserDMPtr.reset();
        }
    }
}//basar

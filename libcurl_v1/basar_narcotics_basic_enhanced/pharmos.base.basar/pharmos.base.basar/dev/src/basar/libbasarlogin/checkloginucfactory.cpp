#include "checkloginucfactory.h"
#include "checkadloginuc.h"
#include "checkloginuc.h"

using namespace basar::login;
using namespace useCase;

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

        CheckLoginUCFactory::CheckLoginUCFactory( Configurationptr configuration ):
            m_ConfigurationPtr( configuration )
        {
        }

        CheckLoginUCFactory::~CheckLoginUCFactory()
        {
            reset();
        }

        CheckLoginBase::CheckLoginBaseSharedPtr     CheckLoginUCFactory::getCheckLoginUC()
        {
            if ( m_CheckLoginUCPtr == NULL )
            {
                if ( m_ConfigurationPtr->isADLogin() )
                {
                    m_CheckLoginUCPtr = CheckLoginBase::CheckLoginBaseSharedPtr( new CheckADLogin( m_ConfigurationPtr) );
                }
                else
                {
                    m_CheckLoginUCPtr = CheckLoginBase::CheckLoginBaseSharedPtr( new CheckLogin( m_ConfigurationPtr ) );
                }
            }

            return m_CheckLoginUCPtr;
        }

        void    CheckLoginUCFactory::reset()
        {
            m_CheckLoginUCPtr.reset();
        }
    }//login
}//basar

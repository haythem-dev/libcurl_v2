#include "checkloginbaseuc.h"

#include "loginappl.h"
#include "crypt.h"
#include "loggerpool.h"
#include "configuration.h"

#include <string.h>

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
        namespace useCase
        {

//----------------------------------------------------------------------------
            /*! \param config configuration object
                \throw no-throw */
            CheckLoginBase::CheckLoginBase( Configurationptr config ) : m_Config( config )
            {
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            CheckLoginBase::~CheckLoginBase()
            {
            }

            void CheckLoginBase::setLoginMode( bool )
            {
                //n/a
            }

            bool CheckLoginBase::isUserIdOrPasswordEmpty()
            {
                return true;
            }
            bool CheckLoginBase::isRightAvailable()
            {
                return false;
            }

            Configurationptr CheckLoginBase::getConfig() const
            {
                return m_Config;
            }

            void CheckLoginBase::setPassword( const basar::VarString& password )
            {
                m_Password = password;
            }

            const basar::VarString CheckLoginBase::getPassword() const
            {
                return m_Password;
            }

//----------------------------------------------------------------------------
        }//useCase
    }//login
}//basar

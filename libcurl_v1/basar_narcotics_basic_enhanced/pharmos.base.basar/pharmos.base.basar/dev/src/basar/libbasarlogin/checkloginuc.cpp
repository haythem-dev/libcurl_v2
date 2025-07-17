//----------------------------------------------------------------------------
/*! \file
 *  \brief  check login implemtation
 *  \author	Thomas Hörath
 *  \date   19.12.2005
 */
//----------------------------------------------------------------------------

#include "checkloginuc.h"

#include "loginappl.h"
#include "crypt.h"
#include "loggerpool.h"

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
            CheckLogin::CheckLogin( Configurationptr config ) : CheckLoginBase( config )
            {
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw */
            CheckLogin::~CheckLogin()
            {
            }

//----------------------------------------------------------------------------
            /*! \throw no-throw
                \retval true if login is ok
                \return otherwise returns false */
            bool CheckLogin::run()
            {
                basar::I18nString UsrName = getAppl()->m_currentUser.getString( "username" );
                basar::I18nString Where = "username=";

                if ( UsrName.empty() )
                {
                    Where.append( "XXXXX" ); //!< if no username-input on gui, parameterlist must contain some dummy value, otherwise execute causes an error
                }
                else
                {
                    Where.append( UsrName );
                }

                Where.append( ";" );
                basar::cmnutil::ParameterList pl( Where );

                getAppl()->m_currentUser = getAppl()->m_UserDM->findUserIDByPattern( pl );

                char cryptedPW [PASSWORD_MAX_LENGTH + 1] = {0};

                if ( getAppl()->isDialog() )
                {
                    // crypt m_PwDlgInput to compare with db value
                    char unCryptedPW [PASSWORD_MAX_LENGTH + 1]; //uncrypted length has to be max-length!
                    memset( unCryptedPW, ' ', PASSWORD_MAX_LENGTH );
                    memcpy( unCryptedPW, CheckLoginBase::getPassword().c_str(), CheckLoginBase::getPassword().length() > PASSWORD_MAX_LENGTH ? PASSWORD_MAX_LENGTH : CheckLoginBase::getPassword().length() );
                    unCryptedPW[PASSWORD_MAX_LENGTH] = '\0';
                    util::encryptPW( unCryptedPW, cryptedPW, PASSWORD_MAX_LENGTH );
                }
                else
                {
                    strncpy( cryptedPW, CheckLoginBase::getPassword().c_str(), PASSWORD_MAX_LENGTH );
                }

                if ( getAppl()->m_currentUser.isEnd() == true ||
                    getAppl()->m_currentUser.getString("password") != cryptedPW )
                {
                    if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "no userid found to <%s> or password <%s> wrong.", Where.c_str(), CheckLoginBase::getPassword().c_str() );
                        LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }

                    return false; //username doesn't match password
                }

                if ( getAppl()->m_currentUser.getInt16( "activ" ) == ACCOUNT_INACTIV )
                {
                    if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "account to <%s> is inactiv.", Where.c_str() );
                        LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }

                    return false; // user inactiv
                }

                // changepw only if dlg!
                if ( getAppl()->isDialog() &&
                        getAppl()->m_currentUser.getInt16( "changepw" ) == PW_HAS_TO_BE_CHANGED )
                {
                    if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                    {
                        VarString msg;
                        msg.format( "<%s> has to change his / her password.", Where.c_str() );
                        LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                    }

                    if ( false == getAppl()->managePwChanging() )
                    {
                        if ( LoggerPool::checkLogin().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
                        {
                            VarString msg;
                            msg.format( "<%s> canceled password changing. Login aborted!", Where.c_str() );
                            LoggerPool::checkLogin().forcedLog( log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__ );
                        }

                        return false;
                    }
                }

                return true;
            }
//----------------------------------------------------------------------------
}//useCase
}//login
}//basar

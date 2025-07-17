#include "libbasarlogin_definitions.h"
#include "baseusermgmtdm.h"
#include "libbasardbaspect_manager.h"
#include "configuration.h"

#include "usermgmt_definitions.h"

#include "adloginusermgmtdm.h"
#include "usermanagementdm.h"
#include "loggerpool.h"

using namespace basar::login;
using namespace log4cplus;

//----------------------------------------------------------------------------
namespace domMod
{

    BaseUserManagement::BaseUserManagement()
    {
    }

    BaseUserManagement::~BaseUserManagement()
    {
    }

    void BaseUserManagement::shutdown()
    {
        if ( false == m_UserACC.isNull() )
        {
            m_UserACC.reset();
        }

        m_AppRightACC.reset();
        m_UserBackupACC.reset();
    }

    // user
    /*! \brief select single user for given ID
        \return basar::db::aspect::AccessorPropertyTable_YIterator containing user data
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseUserManagement::findUserByID
    ( const basar::Int32 userID //!< userid to select userdata for
    )
    {
        try
        {
            basar::I18nString where;
            where.format( "userid=%d;", userID );
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_UserACC.execute( domMod::userManagement::lit::SELECT_USER_BY_ID, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during findUserByID: " << e.what() );

            throw;
        }

        return m_UserACC.getPropertyTable().begin();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator  BaseUserManagement::findUserByUsername
    ( const basar::I18nString& username //!< userid to select userdata for
    )
    {
        try
        {
            basar::I18nString where;
            where.format( "username=%s;", username.c_str() );
            basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

            m_UserACC.execute( domMod::userManagement::lit::SELECT_ID_BY_NAME, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during findUserByUsername: " << e.what() );

            throw;
        }

        return m_UserACC.getPropertyTable().begin();
    }

    /*! \brief select single user by pattern (parameterlist)
        \return basar::db::aspect::AccessorPropertyTable_YIterator containing user data
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseUserManagement::findUserIDByPattern( basar::cmnutil::ParameterList where //!< parameter-list, containing where-statement
    )
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

        return findUserIDByPattern( yit );
    }

    /*! \brief select single user by pattern (yiterator)
        \return basar::db::aspect::AccessorPropertyTable_YIterator containing user data
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseUserManagement::findUserIDByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
    )
    {
        try
        {
            m_UserACC.execute(  userManagement::lit::SELECT_ID_BY_NAME, yit );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during findUserIDByPattern: " << e.what() );

            throw;
        }

        return m_UserACC.getPropertyTable().begin();
    }

    /*! \brief get empty yiterator, marked for update in accessor-user-propertytable
        \n no-throw */
    basar::db::aspect::AccessorPropertyTable_YIterator  BaseUserManagement::addEmptyUser()
    {
        return m_UserACC.getPropertyTable().insert( basar::FOR_UPDATE );
    }

    /*! \brief update single user
        \return basar::BULong indicating affected rows by operation
        \n no-throw */
    basar::BULong   BaseUserManagement::saveUser
    ( const basar::db::aspect::AccessorPropertyTable_YIterator userData //!< yiterator to user which is written to db
    )
    {
        basar::BULong affRows = 0;

        try
        {
            affRows = m_UserACC.execute( domMod::userManagement::lit::SAVE_USER_DATA, userData ).getAffectedRows();
        }
        catch( basar::Exception& e )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during saveUser: " << e.what() );

            throw;
        }

        return affRows;
    }

    /*! \brief update single user on backup connection
        \return basar::BULong indicating affected rows by operation
        \n no-throw */
    basar::BULong   BaseUserManagement::saveUserBackup ( basar::db::aspect::AccessorPropertyTable_YIterator userData, const basar::db::aspect::ConnectionRef backupConn )
    {
        m_UserBackupACC = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_UserBackupInst", "User", backupConn );

        BLOG_DEBUG( LoggerPool::baseUserManagement(), "usermanagement initialized for backup." );

        basar::BULong result = m_UserBackupACC.execute( domMod::userManagement::lit::SAVE_USER_DATA, userData ).getAffectedRows();

        m_UserBackupACC.execute( domMod::userManagement::lit::SAVE_PREV_PASSWORD, userData, false, false );

        std::pair<bool, basar::Decimal> noPwd = m_UserBackupACC.executeAggregate( domMod::userManagement::lit::SELECT_NUMBER_OF_PREV_PASSWORDS, userData );

        if ( noPwd.second.toInt16() > NUM_OF_PW_IN_HISTORY )
        {
            std::pair<bool, basar::Decimal> minDate = m_UserBackupACC.executeAggregate( domMod::userManagement::lit::SELECT_MIN_DATE_IN_PASSWORD_HISTORY, userData );

            if ( minDate.first )
            {
                userData.setInt32( "datevalidfrom", minDate.second.toInt32() );
            }

            m_UserBackupACC.execute( domMod::userManagement::lit::SAVE_PREV_PASSWORD, userData );
        }

        return result;
    }

// appright
    /*! \brief select rights by pattern (parameterlist)
        \return basar::db::aspect::AccessorPropertyTableRef containing application rights
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef BaseUserManagement::findRightsByPattern
    ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
    )
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

        return findRightsByPattern( yit );
    }

    /*! \brief select rights by pattern (yiterator)
        \return basar::db::aspect::AccessorPropertyTableRef containing application rights
        \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef         BaseUserManagement::findRightsByPattern
    ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
    )
    {
        try
        {
            m_AppRightACC.execute( domMod::appRight::lit::SELECT_RIGHTS_BY_PATTERN, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during findRightsByPattern: " << e.what() );

            throw;
        }

        return m_AppRightACC.getPropertyTable();
    }

    //----------------------------------------------------------------------------
    /*! \throw no-throw
        \return reference to propertytable, containing multiline-select-result */
    basar::db::aspect::AccessorPropertyTableRef         BaseUserManagement::findUserRightsByPattern
    ( const basar::cmnutil::ParameterList& )
    {
        basar::db::aspect::AccessorPropertyTableRef tabRefReturn;
        return tabRefReturn;
    }

    /*! \brief get property table for appl-rights
        \n no-throw */
    const basar::db::aspect::AccessorPropertyTableRef   BaseUserManagement::getRightsPropTab    () const
    {
        return m_AppRightACC.getPropertyTable();
    }



// passwordprevious
    /*! \brief returns number of previous passwords
        \return basar::Int16 - number of previous passwords
        \n no-throw */
    basar::Int16                                        BaseUserManagement::findNumOfPreviousPasswords
    (   const basar::db::aspect::AccessorPropertyTable_YIterator yit    //!< yit containing search criteria
    )
    {
        std::pair<bool, basar::Decimal> pair =
            m_UserACC.executeAggregate( domMod::userManagement::lit::SELECT_NUMBER_OF_PREV_PASSWORDS, yit );

        if ( false == pair.first )
        {
            return basar::db::aspect::SQL_ERROR_UNKNOWN;    // error occured
        }
        else
        {
            return pair.second.toInt16();                   // number of previous passwords
        }
    }

    /*! \brief save ( insert ) password to password history
        \return basar::db::aspect::ExecuteResultInfo indicating success or failure of operation
        \n no-throw */
    const basar::db::aspect::ExecuteResultInfo          BaseUserManagement::savePasswordPrevious
    ( const basar::db::aspect::AccessorPropertyTable_YIterator yit      //!< yit containing data to save
    )
    {
        try
        {
            return m_UserACC.execute( domMod::userManagement::lit::SAVE_PREV_PASSWORD, yit, false, false );
        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during savePasswordPrevious: " << e.what() );
            throw;
        }
    }

    /*! \brief delete oldest password from password history
        \return basar::db::aspect::ExecuteResultInfo indicating success or failure of operation
        \n no-throw */
    const basar::db::aspect::ExecuteResultInfo          BaseUserManagement::deletePasswordPrev
    ( basar::db::aspect::AccessorPropertyTable_YIterator yit    //!< yit containing data to delete
    )
    {
        try
        {
            std::pair<bool, basar::Decimal> pair =
                m_UserACC.executeAggregate( domMod::userManagement::lit::SELECT_MIN_DATE_IN_PASSWORD_HISTORY, yit );

            if ( true == pair.first )
            {
                yit.setInt32( "datevalidfrom", pair.second.toInt32() );
            }

            return m_UserACC.execute( domMod::userManagement::lit::DELETE_PREV_PASSWORD, yit );

        }
        catch ( basar::Exception& e  )
        {
            BLOG_ERROR_STREAM( LoggerPool::baseUserManagement(), "exception during deletePasswordPrev: " << e.what() );

            throw;
        }
    }
//----------------------------------------------------------------------------
}//domMod

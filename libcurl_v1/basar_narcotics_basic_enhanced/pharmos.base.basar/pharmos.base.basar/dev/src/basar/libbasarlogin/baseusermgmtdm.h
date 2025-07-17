#ifndef GUARD_BASEUSERMANAGEMENTDM_H
#define GUARD_BASEUSERMANAGEMENTDM_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar
{
    namespace cmnutil
    {
        class ParameterList;
    }

    namespace db
    {
        namespace aspect
        {
            class ConnectionRef;
            class AccessorPropertyTable_YIterator;
            class AccessorPropertyTableRef;
        }
    }
}

//----------------------------------------------------------------------------
namespace domMod
{

//---------------------------------------------------------------------------
    /*! \brief class for managing application specific access
        \n final class
        \n throws InvalidApplNameException */
    class BaseUserManagement
    {
        public:
            //! < shared pointer for BaseUserManagement
            typedef boost::shared_ptr< BaseUserManagement > BaseUserManagementSharedPtr;

            /*! \brief create a BaseUserManagement.
            \n     no-throw */
            BaseUserManagement();

            /*! \brief destructer BaseUserManagement.
            \n     no-throw */
            virtual ~BaseUserManagement();

            /*! \brief reset accessor instances and contents of property tables
                \n no-throw */
            void shutdown();

            /*! \brief initialize accessor instances
                \n no-throw */
            virtual void init( const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
                              ) = 0;

            /*! \brief select rights by pattern (parameterlist)
                \n throws exception */
            virtual basar::db::aspect::AccessorPropertyTableRef
            findUserRightsByPattern ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                    );

// user
            /*! \brief select single user for given ID
                \n throws exception */
            basar::db::aspect::AccessorPropertyTable_YIterator
            findUserByID            ( const basar::Int32 userID //!< userid to select userdata for
                                    );

            // user
            /*! \brief select single user for given ID
                \return basar::db::aspect::AccessorPropertyTable_YIterator containing user data
                \n throws exception */
            basar::db::aspect::AccessorPropertyTable_YIterator
            findUserByUsername      ( const basar::I18nString& username //!< userid to select userdata for
                                    );

            /*! \brief select single user by pattern (parameterlist)
                \n throws exception */
            basar::db::aspect::AccessorPropertyTable_YIterator
            findUserIDByPattern     ( basar::cmnutil::ParameterList where //!< parameter-list, containing where-statement
                                    );

            /*! \brief select single user by pattern (yiterator)
                \n throws exception */
            basar::db::aspect::AccessorPropertyTable_YIterator
            findUserIDByPattern     ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                    );

            /*! \brief get empty yiterator, marked for update in accessor-user-propertytable
                \n no-throw */
            basar::db::aspect::AccessorPropertyTable_YIterator          addEmptyUser();

            /*! \brief update single user
                \n throws exception */
            basar::BULong
            saveUser                ( const basar::db::aspect::AccessorPropertyTable_YIterator userData //!< yiterator to user which is written to db
                                    );

            /*! \brief update single user on backup connection
                \n no-throw */
            basar::BULong                                               saveUserBackup
            ( basar::db::aspect::AccessorPropertyTable_YIterator userData,      //!< yiterator to user which is written to db
              const basar::db::aspect::ConnectionRef backupConn//!< reference to backup connection for initializing accessor
            );

// appright
            /*! \brief select rights by pattern (parameterlist)
                \n throws exception */
            basar::db::aspect::AccessorPropertyTableRef
            findRightsByPattern ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                );

            /*! \brief select rights by pattern (yiterator)
                \n throws exception */
            basar::db::aspect::AccessorPropertyTableRef
            findRightsByPattern ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                );

            /*! \brief get property table for appl-rights
                \n no-throw */
            virtual const basar::db::aspect::AccessorPropertyTableRef   getRightsPropTab    () const ;

// passwordprevious
            /*! \brief update single user
                \n throws exception */
            virtual basar::Int16
            findNumOfPreviousPasswords  (   const basar::db::aspect::AccessorPropertyTable_YIterator yit    //!< yit containing search criteria
                                        );

            /*! \brief save ( insert ) password to password history
                \n throws exception */
            virtual const basar::db::aspect::ExecuteResultInfo
            savePasswordPrevious        ( const basar::db::aspect::AccessorPropertyTable_YIterator yit      //!< yit containing data to save
                                        );

            /*! \brief delete oldest password from password history
                \n throws exception */
            virtual const basar::db::aspect::ExecuteResultInfo
            deletePasswordPrev          ( basar::db::aspect::AccessorPropertyTable_YIterator yit    //!< yit containing data to delete
                                        );

        protected:
            basar::db::aspect::AccessorInstanceRef m_UserACC;       //!< instance of user-accessor
            basar::db::aspect::AccessorInstanceRef m_AppRightACC;   //!< instance of right-accessor
            basar::db::aspect::AccessorInstanceRef m_UserBackupACC; //!< instance of user-accessor for backup connection
    };

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif  // GUARD
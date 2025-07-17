#ifndef GUARD_BASEAPPLMANAGEMENTDM_H
#define GUARD_BASEAPPLMANAGEMENTDM_H

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
    class BaseApplManagement
    {
        public:
            typedef boost::shared_ptr< BaseApplManagement >    BaseApplManagementSharedPtr;

            static const basar::UInt32 LENGTH_APPNAME = 30;

            BaseApplManagement();

            virtual ~BaseApplManagement();

            //! \brief init function
            virtual  void   init( const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
                                ) = 0;

            //! \brief finds application languages
            //! \return basar::db::aspect::AccessorPropertyTableRef containing application languages
            virtual  basar::db::aspect::AccessorPropertyTableRef
            findApplicationLanguage ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                    ) = 0;

            /*! \brief select areas by pattern (yiterator)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing areas
            virtual  basar::db::aspect::AccessorPropertyTableRef
            findAreaByPattern       ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                    ) = 0;

            /*! \brief select global area
                \n always throws not implemented exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing global area
            virtual basar::db::aspect::AccessorPropertyTableRef findGlobalArea          () = 0;

            /*! \brief reset accessor instances and contents of property tables
                \n no-throw */
            void shutdown();


// appl
            /*! \brief select single application for given application name
                \n throws exception
                \return basar::db::aspect::AccessorPropertyTable_YIterator containing application id
            */
            basar::db::aspect::AccessorPropertyTable_YIterator
            findApplIDByName        ( basar::I18nString& rApplName //!< application name to select ID for
                                    );

            /*! \brief check length of application name
                \n InvalidApplNameException */
            void
            checkApplNameLength     ( const basar::I18nString& rApplName //!< application name to check
                                    );

// language
            /*! \brief select languages by pattern (parameterlist)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing languages
            basar::db::aspect::AccessorPropertyTableRef
            findLanguageByPattern   ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                    );

            /*! \brief select languages by pattern (yiterator)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing languages
            basar::db::aspect::AccessorPropertyTableRef
            findLanguageByPattern   ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                    );


            /*! \brief get property table for language
                \n no-throw */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing languages
            const basar::db::aspect::AccessorPropertyTableRef       getLanguagePropTab      () const;

// area
            /*! \brief select areas by pattern (parameterlist)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing area
            basar::db::aspect::AccessorPropertyTableRef
            findAreaByPattern       ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                    );

            /*! \brief get property table for areas
                \n no-throw */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing area
            const basar::db::aspect::AccessorPropertyTableRef       getAreaPropTab          () const;

            /*! \brief select single area by ID
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTable_YIterator containing area name
            basar::db::aspect::AccessorPropertyTable_YIterator
            findAreaNameByID        ( const basar::Int32 areaID //!< areaid to select area for
                                    );

// previous
            /*! \brief select single previous data by pattern (parameterlist)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTableRef containing previous
            basar::db::aspect::AccessorPropertyTable_YIterator
            findPreviousByPattern   ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                    );

            /*! \brief select single previous data by pattern (yiterator)
                \n throws exception */
            //! \return basar::db::aspect::AccessorPropertyTable_YIterator containing previous
            basar::db::aspect::AccessorPropertyTable_YIterator
            findPreviousByPattern   ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                    );

            /*! \brief update / insert single previous data
                \n throws exception */
            //! \return basar::BULong affected rows
            virtual basar::BULong
            savePrevious            ( const basar::db::aspect::AccessorPropertyTable_YIterator previousData
                                      //!< yiterator containing previous data to save
                                    ) = 0;

            /*! \brief get empty yiterator, marked for insert in accessor-previous-propertytable
                \n no-throw */
            //! \return basar::db::aspect::AccessorPropertyTable_YIterator pointing to new emptry previous
            basar::db::aspect::AccessorPropertyTable_YIterator      addEmptyPrevious();

// parameter
            /*! \brief select parameters by application ID
                \n throws exception */
            basar::db::aspect::AccessorPropertyTableRef
            findParaByApplID        ( const basar::Int32 applID //!< applicationid to select parameters for
                                    );
            /*! \brief select parameters by application Name
                \n no-throw */
            basar::db::aspect::AccessorPropertyTableRef findParaByApplName( basar::I18nString sApplName );

            /*! \brief get property table for parameters
                \n no-throw */
            const basar::db::aspect::AccessorPropertyTableRef       getParaPropTab          () const;

        protected:

            basar::db::aspect::AccessorInstanceRef m_ApplACC;       //!< instance of application-accessor
            basar::db::aspect::AccessorInstanceRef m_LanguageACC;   //!< instance of language-accessor
            basar::db::aspect::AccessorInstanceRef m_AreaACC;       //!< instance of area-accessor
            basar::db::aspect::AccessorInstanceRef m_PreviousACC;   //!< instance of previous-accessor
            basar::db::aspect::AccessorInstanceRef m_ParaACC;       //!< instance of parameter-accessor

    };

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif  // GUARD

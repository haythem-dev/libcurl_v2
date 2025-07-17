#ifndef GUARD_ADLOGINAPPLMGMTDM_H
#define GUARD_ADLOGINAPPLMGMTDM_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessor.h"
#include "baseapplmgmtdm.h"

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
    class ADLoginApplMgmt : public BaseApplManagement
    {
            //friend class BaseApplManagement;

        public:
            /*! \brief std-constructor
                \n no-throw */
            ADLoginApplMgmt();

            /*! \brief std-destructor
                \n no-throw */
            virtual ~ADLoginApplMgmt();

            /*! \brief initialize accessor instances
                \n no-throw */

            virtual  void init( const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
                              );

            /*! \brief select languages mapped to application (yiterator)
                \n throws exception */
            virtual  basar::db::aspect::AccessorPropertyTableRef
            findApplicationLanguage ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement

                                    );
            /*! \brief select areas by pattern (yiterator)
                \n throws exception */
            virtual  basar::db::aspect::AccessorPropertyTableRef            findAreaByPattern       ( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
                                                                                                    );

            /*! \brief select global area
                \n throws exception */
            virtual basar::db::aspect::AccessorPropertyTableRef             findGlobalArea          ();

            /*! \brief update / insert single previous data
                \n throws exception */
            //! \return basar::BULong affected rows
            virtual basar::BULong
            savePrevious            ( const basar::db::aspect::AccessorPropertyTable_YIterator previousData
                                      //!< yiterator containing previous data to save
                                    );

    };

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif  // GUARD

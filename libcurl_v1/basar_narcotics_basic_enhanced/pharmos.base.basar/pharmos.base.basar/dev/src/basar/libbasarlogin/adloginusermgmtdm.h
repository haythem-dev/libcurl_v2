#ifndef GUARD_ADLOGINUSERMGMTDM_H
#define GUARD_ADLOGINUSERMGMTDM_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessor.h"
#include "baseusermgmtdm.h"

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
            class  ConnectionRef;
            class  AccessorPropertyTable_YIterator;
            class  AccessorPropertyTableRef;
            struct ExecuteResultInfo;
        }
    }
}

//----------------------------------------------------------------------------
//! namespace for domain modules
namespace domMod
{

//---------------------------------------------------------------------------
    /*! \brief class for managing user specific access
        \n final class
        \n throws no exceptions */
    class ADLoginUserMgmt: public BaseUserManagement
    {
        public:
            /*! \brief std-constructor
                \n no-throw */
            ADLoginUserMgmt();

            /*! \brief std-destructor
                \n no-throw */
            ~ADLoginUserMgmt();

            /*! \brief initialize accessor instances
                \n no-throw */
            virtual void init( const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
                             );

            /*! \brief select rights by pattern (parameterlist)
                \n throws exception */
            virtual basar::db::aspect::AccessorPropertyTableRef
            findUserRightsByPattern ( const basar::cmnutil::ParameterList& where //!< parameter-list, containing where-statement
                                    );
    };

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif  // GUARD

#ifndef GUARD_LIBBASARCMNUTIL_DATETYPES_COMMON_H
#define GUARD_LIBBASARCMNUTIL_DATETYPES_COMMON_H

//----------------------------------------------------------------------------
/*! \file
 *  \brief  common header file for all date/time-types
 *  \author Marco Köppendörfer
 *  \date   03.05.2013
 */
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {

//------------------------------------------------------------------------------------------------------//
// predefined useful strings used by DateTime, Date
//------------------------------------------------------------------------------------------------------//
        static basar::ConstString FORMAT_SQL_DATE_INFORMIX      = "mm/dd/yyyy";                          //!< useful format control string for Informix SQL type date
        static basar::ConstString FORMAT_SQL_DATETIME_INFORMIX  = "%04Y-%02m-%02d %02H:%02M:%02S.%03F3";  //!< useful format control string for Informix SQL type datetime
        static basar::ConstString FORMAT_SQL_TIME_INFORMIX      = "%02H:%02M:%02S.%03F3";                 //!< useful format control string for Informix SQL type time (= datetime)

        static const basar::Int32 NULLVAL_SQL_DATE              = 99991231;                              //!< date: null value/ DB default value handled as NULL
        static const basar::Int32 NULLVAL_SQL_TIME              = 235959999;                             //!< time: null value/ DB default value handled as NULL

        static basar::ConstString NULLVAL_SQL_INTERVAL_SHORT    = "999999999 23:59:59.999";              //!< interval (days to fraction): null value/ DB default value handled as NULL

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

#endif //GUARD_LIBBASARCMNUTIL_DATETYPES_COMMON_H

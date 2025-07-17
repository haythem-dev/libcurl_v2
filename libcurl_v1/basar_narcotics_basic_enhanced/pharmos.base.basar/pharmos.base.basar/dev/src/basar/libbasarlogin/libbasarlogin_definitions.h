//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: definitions
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARLOGIN_DEFINITIONS_H
#define GUARD_LIBBASARLOGIN_DEFINITIONS_H

//------------------------------------------------------------------------------
#include <map>
#include <list>
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

        static const basar::UInt16 PASSWORD_MAX_LENGTH       = 8;                    //!< maximum length of password, limited by db
        static const basar::UInt16 PASSWORD_MIN_LENGTH       = 6;                    //!< minimum length of password, limited by guidelines
        static const basar::UInt16 NUM_OF_PW_IN_HISTORY      = 6;                    //!< number of passwords stored in history, which can't be used as new password
        static const basar::Int16 ACCOUNT_ACTIV             = 1;                    //!< user account is activated
        static const basar::Int16 ACCOUNT_INACTIV           = 0;                    //!< user account is deactivated
        static const basar::Int16 PW_HAS_TO_BE_CHANGED      = 1;                    //!< user has to change his password
        static const basar::Int16 PW_NOT_TO_CHANGE          = 0;                    //!< user's password is up to date
        static basar::ConstString STD_LANGUAGE              = "en";                 //!< standard language
        static const basar::Int16 APPLID_BASARLOGIN         = 1;                    //!< application id of this dll
        static const basar::Int16 MODID_BASARLOGIN          = 1;                    //!< one and only module of this dll

        static basar::ConstString PARAM_BASAR_LOCALE        = "BASAR_LOCALE";       //!< parameter name for basar locale that is associated with area (1st parameter condition)
        static basar::ConstString PARAM_BASAR_CODEPAGE      = "BASAR_CODEPAGE";     //!< parameter name for basar codepage that is associated with area (and connected DB; 1st parameter condition)

        static basar::ConstString USER_MGMT_APP_NAME        = "UADM";               //!< application name of user management application

//! \brief definition for collection of branch-numbers of special right, which is returned by interface "getBranchesWithRight()"
//! \n     key  : int16 - areaid
//! \n     value: string - areaname
        typedef std::map < basar::Int32, basar::I18nString >    CollBranchesWithRight;

//! \brief definition for collection of branch-names of special right, which is returned by interface "getBranchesWithRightOrderedByName()"
//! \n     value: int16 - areaid
//! \n     key  : string - areaname
        typedef std::map < basar::I18nString, basar::Int32 >    CollBranchesWithRightOrderedByName;

//! \brief definition for collection of rightnames for special module and area, which is returned by interface "getRightsByModuleAndArea()"
        typedef std::list < basar::I18nString >                 CollRightsForModAndArea;

//! \brief enum for valid login modes
        enum LoginModeEnum
        {
            LOGIN_MODE_DEFAULT,             //!< default behaviour, load all user's rights upon login (this is the way login worked ever since)
            LOGIN_MODE_REGION_ONLY          //!< region only behaviour, just load the rights for the region that the user requested upon login
        };

    }   // namespace login
}   // namespace basar


//------------------------------------------------------------------------------
#endif  // GUARD

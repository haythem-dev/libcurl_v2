#ifndef GUARD_USERMGMT_DEFINITIONS_H
#define GUARD_USERMGMT_DEFINITIONS_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

namespace domMod
{
    namespace userManagement
    {
        namespace lit
        {
            const basar::VarString SELECT_ID_BY_NAME = "SelIDByName";

            const basar::VarString SELECT_USER_BY_ID = "SelUserByID";

            const basar::VarString SELECT_NUMBER_OF_PREV_PASSWORDS = "amSelNumOfPrevPw";

            const basar::VarString SELECT_MIN_DATE_IN_PASSWORD_HISTORY = "amSelMinDateInPwHistory";

            const basar::VarString SAVE_PREV_PASSWORD = "amSavePrevPw";

            const basar::VarString DELETE_PREV_PASSWORD = "amDeletePrevPw";

            const basar::VarString SAVE_USER_DATA = "SaveUserData";
        }
    }

    namespace appRight
    {
        namespace lit
        {
            const basar::VarString SELECT_RIGHTS_BY_PATTERN = "SelRightsByPattern";
        }
    }
}

#endif

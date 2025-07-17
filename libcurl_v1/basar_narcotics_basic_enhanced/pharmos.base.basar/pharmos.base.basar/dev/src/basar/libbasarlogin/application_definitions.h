#ifndef GUARD_APPLICATION_DEFINITIONS_H
#define GUARD_APPLICATION_DEFINITIONS_H

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

namespace domMod
{
    namespace area
    {
        namespace lit
        {
            const basar::VarString SELECT_AREA_BY_APPLICATION_ID = "SelAreaByApplID";

            const basar::VarString SELECT_AREA_ID_BY_AREA_NO = "SelAreaIDByAreaNo";

            const basar::VarString SELECT_GLOBAL_AREA = "SelGlobalArea";

            const basar::VarString SELECT_BRANCH_BY_APPLICATION_ID = "SelBranchByApplID";

            const basar::VarString SELECT_REGION_BY_APPLICATION_ID = "SelRegionByApplID";

            const basar::VarString SELECT_AREA_BY_ID = "SelSingleArea";
        }
    }

    namespace language
    {
        namespace lit
        {
            const basar::VarString SELECT_LANGUAGE_BY_APPLICATION_ID = "SelLanguageByApplID";

            const basar::VarString SELECT_APPLICATION_LANGUAGE = "SelApplicationLanguage";
        }
    }

    namespace application
    {
        namespace lit
        {
            const basar::VarString SELECT_APPLICATION_ID_BY_PATTERN = "SelIDByPattern";
        }
    }

    namespace previous
    {
        namespace lit
        {
            const basar::VarString SELECT_PREVIOUS_BY_PATTERN = "SelPreviousByPattern";

            const basar::VarString SAVE_PREVIOUS = "SavePrevious";

            const basar::VarString DELETE_PREVIOUS = "DeletePrevious";
        }
    }

    namespace parameter
    {
        namespace lit
        {
            const basar::VarString SELECT_PARAMETER_BY_APPLICATION_ID = "SelParaByApplID";

            const basar::VarString SELECT_PARAMETER_BY_APPLICATION_NAME = "SelParaByApplName";
        }
    }

    namespace text
    {
        namespace lit
        {
            const basar::VarString SELECT_TEXT_BY_PATTERN = "SelTextByPattern";
        }
    }
}

#endif

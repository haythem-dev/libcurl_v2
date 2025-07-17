#ifndef GUARD_LOGINTYPE_H
#define GUARD_LOGINTYPE_H

// Include section
#include <libbasar_definitions.h>

// namespace section
namespace loginType
{
    enum LoginTypeEnum
    {
        LOGINTYPE_NOT_SET               = 0 ,
        LOGINTYPE_AD                    = 1 ,
        LOGINTYPE_NON_AD                = 2
    } ;
} // loginType
#endif // GUARD_LOGINTYPE_H
#ifndef GUARD_CONFIGURATIONPTR_H
#define GUARD_CONFIGURATIONPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
    namespace login
    {
        class Configuration;
        typedef boost::shared_ptr< Configuration > Configurationptr;
    }
}

#endif

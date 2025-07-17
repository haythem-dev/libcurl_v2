//----------------------------------------------------------------------------
/*! \file
 *  \brief  argument container
 *  \date   13.07.2015
 */
//----------------------------------------------------------------------------

#ifndef GUARD_COLLARGS_H
#define GUARD_COLLARGS_H

//----------------------------------------------------------------------------
#include <vector>
#include "../libbasarcmnutil_i18nstring.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//---------------------------------------------------------------------------
//! argument container
class CollArgs : public std::vector<I18nString>
{
public:
   	//! \brief  dump arguments
    //! \return all arguments dumped into string
    LIBBASARCMNUTIL_API I18nString dumpArgs() const;
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

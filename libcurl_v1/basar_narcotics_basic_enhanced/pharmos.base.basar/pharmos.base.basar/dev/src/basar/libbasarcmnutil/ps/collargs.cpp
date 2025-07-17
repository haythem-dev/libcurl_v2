//----------------------------------------------------------------------------
/*! \file
 *  \brief  argument container
 *  \date   13.07.2015
 */
//----------------------------------------------------------------------------

#include "collargs.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//------------------------------------------------------------------------------
I18nString CollArgs::dumpArgs() const
{
	I18nString options;

    for (size_type i = 0; i < size(); ++i)
	{
		if (0 != i)
			options += " ";

		options += (*this)[i];
	}

	return options;
}

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar


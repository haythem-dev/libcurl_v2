//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarqtwidget_functions.h"
#include "libbasar_pkgversion.h"

//----------------------------------------------------------------------------
//! component name
#define PROGRAM_NAME				"libbasarqtwidget"

//----------------------------------------------------------------------------
namespace basar	{
namespace gui	{
namespace widget{

//----------------------------------------------------------------------------
//! in binary embedded full version string
static ConstString s_SCCSID = "@(#) "		COMPANYNAME 
							  " version: "	PROGRAM_NAME 
							  " "			PRODUCT_NUMBER 
							  " built:   "	__DATE__ 
							  " "			__TIME__;

//---------------------------------------------------------------------------
//! \return	string with package's version
//! \throw  no-throw
ConstString getVersion()
{
	return PRODUCT_NUMBER;
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//! \throw  no-throw
ConstString getFullVersion()
{
	return s_SCCSID;
}

//----------------------------------------------------------------------------
}	// namespace widget
}	// namespace gui
}	// namespace basar

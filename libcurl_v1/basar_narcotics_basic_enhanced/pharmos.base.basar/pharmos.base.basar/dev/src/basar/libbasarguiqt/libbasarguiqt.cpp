//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguiqt_functions.h"
#include "libbasar_pkgversion.h"

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME			"libbasarguiqt"	

/*----------------------------------------------------------------------------*/
namespace basar {
namespace gui {
namespace qt {

/*----------------------------------------------------------------------------*/
//! in binary embedded full version string
static ConstString s_SCCSID =	"@(#) "			COMPANYNAME 
								" version: "	PROGRAM_NAME 
								 " "			PRODUCT_NUMBER 
								" built:   "	__DATE__ 
								" "				__TIME__;

//---------------------------------------------------------------------------
//! \return	string with package's version
ConstString getVersion()
{
	return PRODUCT_NUMBER;
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
ConstString getFullVersion()
{
	return s_SCCSID;
}

//---------------------------------------------------------------------------
}// qt
}// gui
}// basar

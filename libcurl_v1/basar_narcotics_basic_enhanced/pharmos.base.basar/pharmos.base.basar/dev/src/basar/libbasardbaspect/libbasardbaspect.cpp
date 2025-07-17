//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasar_pkgversion.h"
#include "libbasardbaspect_functions.h"

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME				"libbasardbaspect"

/*----------------------------------------------------------------------------*/
namespace basar {
namespace db {
namespace aspect {

/*----------------------------------------------------------------------------*/
//! in binary embedded full version string
static ConstString s_SCCSID =	"@(#) "			COMPANYNAME 
								" version: "	PROGRAM_NAME 
								" "				PRODUCT_NUMBER 
								" built:   "	__DATE__ 
								" "				__TIME__;

//---------------------------------------------------------------------------
//! \return	string with package's version
//! \throw  no-throw
ConstBuffer getVersion()
{
	return PRODUCT_NUMBER;
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//! \throw  no-throw
ConstBuffer getFullVersion()
{
	return s_SCCSID;
}

//---------------------------------------------------------------------------
}// namespace aspect
}// namespace db
}// namespace basar

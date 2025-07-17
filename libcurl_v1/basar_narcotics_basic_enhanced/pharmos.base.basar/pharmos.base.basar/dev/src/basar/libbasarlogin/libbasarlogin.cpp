//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions login component
 *  \author Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarlogin_functions.h"
#include "libbasar_pkgversion.h"

/*----------------------------------------------------------------------------*/
//! libbasarlogin name
#define PROGRAM_NAME			"libbasarlogin"	

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace login
{

/*----------------------------------------------------------------------------*/
//! in binary embedded full version string
static ConstString s_SCCSID =	"@(#) "			COMPANYNAME 
								" version: "	PROGRAM_NAME 
								 " "			PRODUCT_NUMBER 
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

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar


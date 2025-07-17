//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions application utilities
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarappl_functions.h"
#include "libbasar_pkgversion.h"

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME			"libbasarappl"	

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace appl
{

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
	return static_cast<ConstBuffer>(PRODUCT_NUMBER);
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//! \throw  no-throw
ConstBuffer getFullVersion()
{
	return static_cast<ConstBuffer>(s_SCCSID);
}

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions property
 *  \author Björn Bischof
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarproperty_functions.h"
#include "libbasar_pkgversion.h"

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME				"libbasarproperty"

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace property
{

/*----------------------------------------------------------------------------*/
//! in binary embedded full version string
static ConstString s_SCCSID =	"@(#) "			COMPANYNAME 
								" version: "	PROGRAM_NAME 
								 " "			PRODUCT_NUMBER 
								" built:   "	__DATE__ 
								" "				__TIME__;

//---------------------------------------------------------------------------
//! \return	string with package's and component's version
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
}// namespace property
}// namespace basar

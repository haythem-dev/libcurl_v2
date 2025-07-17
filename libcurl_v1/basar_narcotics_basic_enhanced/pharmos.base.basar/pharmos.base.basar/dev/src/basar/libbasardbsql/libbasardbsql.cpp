//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_functions.h"
#include "profiling.h"
#include "libbasar_pkgversion.h"

//----------------------------------------------------------------------------
//! component name
#define PROGRAM_NAME				"libbasardbsql"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

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
//! \return	summarized fetch time in ms
//! \throw  no-throw
UInt32 getDbLibFetchTime()
{
	return getCsdkFetchTotalTime();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

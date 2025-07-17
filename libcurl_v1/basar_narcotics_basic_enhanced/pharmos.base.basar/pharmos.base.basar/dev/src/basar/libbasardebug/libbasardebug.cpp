//----------------------------------------------------------------------------
/*! \file
 *  \brief  library for debug purpose
 *  \author Michael Eichenlaub
 *  \date   07.10.2005
 */
//----------------------------------------------------------------------------

#include "libbasardebug.h"

#include "libbasardebug_functions.h"
#include "libbasar_pkgversion.h"
#include "checks.h"

#include <stdio.h>
#include <stdlib.h>

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#endif

//----------------------------------------------------------------------------
//! component name
#define PROGRAM_NAME			"libbasardebug"	

//----------------------------------------------------------------------------
namespace basar
{
namespace debug
{

	//----------------------------------------------------------------------------
//!	\throw  no-throw 
//! \return is process running at build server?
//!
bool isBuildServer ()
{
	static ConstString ENV_BUILDSERVER = "CISERVER";
	static ConstString BUILDSERVER     = getenv(ENV_BUILDSERVER);	// check it once

	return (BUILDSERVER > 0);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void printError(ConstString msg, ConstString heading)
{
/*! \ingroup DIFF_WIN_UNIX*/ 
#ifdef _WIN32

	if ( ! isBuildServer() )
	{
		MessageBox(0, msg, heading, MB_OK | MB_ICONERROR);
		return;
	}

#endif

	fprintf(stderr, "%s:\n%s\n", heading, msg);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void printDbgMsg(ConstString msg)
{
/*! \ingroup DIFF_WIN_UNIX*/ 
#ifdef _WIN32
	OutputDebugString(msg);
#else
	fprintf(stderr, "%s", msg);
#endif
}

//---------------------------------------------------------------------------
//! \throw  no-throw
//!
void enableLeakCheck (bool on /* = true */)
{
	check().m_flagCheck = on;
}

//---------------------------------------------------------------------------
//! \throw  no-throw
//!
void ignoreMemoryDiff (int blocks, int bytes)
{
	check().m_Ignore.blocks = blocks;
	check().m_Ignore.bytes  = bytes;
	check().m_flagIgnoreSet = true;
}

//---------------------------------------------------------------------------
//! \return	string with package's version
//! \throw  no-throw
ConstBuffer getVersion()
{
	return static_cast<ConstString>(PRODUCT_NUMBER);
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//! \throw  no-throw
ConstBuffer getFullVersion()
{
	static ConstString s_SCCSID =	
		"@(#) "			COMPANYNAME 
		" version: "	PROGRAM_NAME 
		" "			PRODUCT_NUMBER 
		" built:   "	__DATE__ 
		" "				__TIME__;
		
	return s_SCCSID;
}

//----------------------------------------------------------------------------
}	// namespace debug
}	// namespace basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions common utilities
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_functions.h"

#include "libbasarcmnutil_bstring.h"
#include "libbasar_pkgversion.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#else
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include <iostream>

#ifdef _AIX
#include "getexecname.h"
#endif

#include "getlibpath.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

/*----------------------------------------------------------------------------*/
//! component name
#define PROGRAM_NAME			"libbasarcmnutil"	

/*----------------------------------------------------------------------------*/
namespace basar
{
namespace cmnutil
{

//---------------------------------------------------------------------------
//! \return	string with package's version
//!
ConstBuffer getVersion()
{
	return PRODUCT_NUMBER;
}

//---------------------------------------------------------------------------
//! \return	string with company, programname, package, component and builddate info
//!
ConstBuffer getFullVersion()
{
	// in binary embedded full version string
	ConstString s_SCCSID  = "@(#) "		COMPANYNAME 
						    " version: "	PROGRAM_NAME 
							" "			PRODUCT_NUMBER 
							" built:   "	__DATE__ 
							" "			__TIME__;
								   
	return s_SCCSID;
}

//------------------------------------------------------------------------------
//! \return executable's absolute path and name
VarString getExecPath()
{
	using boost::filesystem::path;

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	char filename [MAX_PATH + 1] = { 0 };
	::GetModuleFileName(NULL, filename, MAX_PATH);

	path execpath(filename);
	execpath = execpath.lexically_normal(); // this removes \.\ or \..\ from paths
#else

	path execpath(getexecname());

	if (execpath.is_relative())
	{
		char cwd[PATH_MAX + 1] = { 0 };

		getcwd(cwd, PATH_MAX);

		path tmp(cwd);
		execpath.swap(tmp);

		execpath /= tmp;
	}

#endif

	return execpath.string();
}

//----------------------------------------------------------------------------
//! \return depending on full: filename with/without extension
VarString getExecName(bool full /* = false */ )
{
	using boost::filesystem::path;

	path execpath(getExecPath().c_str());

	return full ? execpath.filename().string() 
		        : execpath.stem    ().string();
}

//----------------------------------------------------------------------------
//! \return executable's absolute directory name
VarString getExecDir()
{
    using boost::filesystem::path;

    return path(getExecPath().c_str()).parent_path().string();
}

//----------------------------------------------------------------------------
//! \return library's absolute directory name
VarString   getLibDir()
{
    using boost::filesystem::path;
    using boost::filesystem::canonical;

    basar::VarString libPath( canonical(path( getLibPath().c_str())).string() );

    return libPath.toLower();
}

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

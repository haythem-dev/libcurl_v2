//----------------------------------------------------------------------------
/*! \file
 *  \brief  retrieving Informix sqlhosts parameter
 *  \author Michael Eichenlaub
 *  \date   05.08.2010
 */
//----------------------------------------------------------------------------

#include "sqlhostsinfx.h"

#include "libbasarcmnutil_exceptions.h"
#include "loggerpool.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>

#else

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <boost/filesystem.hpp>

#endif

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw no-throw
//!
SqlHostsInfx::SqlHostsInfx ( const VarString & dbserver )
						   : m_DbServer(dbserver)
{
	queryParams();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing DB hostname
VarString SqlHostsInfx::getHost () const
{
	return m_Host;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing protocol name
VarString SqlHostsInfx::getProtocol () const
{
	return m_Protocol;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return VarString containing portnumber
VarString SqlHostsInfx::getService () const
{
	return m_Service;
}

/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32

//----------------------------------------------------------------------------
//! \brief evaluate Unix sqlhosts file
//!	\throw InvalidParameterException
//!
static VarString getSqlHostsPath()
{
    using boost::filesystem::path;

	static ConstString func = "basar.db.sql.getSqlHostsPath";
	
	// evaluate sqlhosts file
	ConstString envHostsKey = "INFORMIXSQLHOSTS";
	char *      envVal      = getenv(envHostsKey);

	path file;
	
	if (envVal)
		file = envVal;
	else
	{
		ConstString envDirKey = "INFORMIXDIR";

		envVal = getenv(envDirKey);

		if (!envVal)
		{
			VarString msg;
			msg.format("environment variable INFORMIXDIR not set");
			throw InvalidParameterException (ExceptInfo (func, msg, __FILE__, __LINE__));
		}

		file  = envVal;
		file /= "etc";
		file /= "sqlhosts";
	}

	if (! boost::filesystem::exists(file) ) 
	{ 
		VarString msg;
		msg.format("file %s doesn't exist", file.string().c_str());
		throw InvalidParameterException (ExceptInfo (func, msg, __FILE__, __LINE__));
	}

	return file.string();
}

#endif

//----------------------------------------------------------------------------
//! \throw RuntimeException, InvalidStateException, InvalidParameterException
//!
void SqlHostsInfx::queryParams()
{
	static ConstString func = "basar.db.sql.SqlHostsInfx.queryParam";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	HKEY      hKey;
	VarString subkey = "SOFTWARE\\Informix\\SqlHosts\\";
	subkey += m_DbServer;

	LONG retval = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,	// key
								 subkey.c_str()    ,	// subkey
								 0                 ,	// options
								 KEY_QUERY_VALUE   ,	// security descriptor
								 &hKey             );

	if (ERROR_SUCCESS != retval)
	{
		VarString msg;
		msg.format("::RegOpenKeyEx(HKLM, %s) failed with %ld",
				   subkey.c_str(), 
				   retval);
		throw InvalidParameterException(ExceptInfo (func, msg, __FILE__, __LINE__));
	}

	ConstString regvals[] = 
	{
		"HOST",
		"PROTOCOL",
		"SERVICE"
	};

	DWORD type;
	BYTE  data[3][256];
	DWORD datalen;

	for (int i = 0; i < sizeof(regvals)/sizeof(regvals[0]); ++i)
	{
		datalen = sizeof(data[0]);

		retval = ::RegQueryValueEx( hKey,		// opened key
								    regvals[i],	// to be queried value
								    NULL,		// reserved
								    &type,		// indicator for data type
								    data[i],	// data buffer
								    &datalen);	// data buffer length => data length

		if( (retval != ERROR_SUCCESS) || (datalen > sizeof(data)) )
		{
			::RegCloseKey(hKey);

			VarString msg;
			msg.format("::RegQueryValueEx(HKLM\\%s:%s) failed with %ld",
					subkey.c_str(), 
					regvals[i],
					retval);
			throw RuntimeException (ExceptInfo (func, msg, __FILE__, __LINE__));
		}

		if (REG_SZ != type)
		{
			::RegCloseKey(hKey);

			VarString msg;
			msg.format("::RegQueryValueEx(HKLM\\%s:%s) queried wrong data type %s",
					subkey.c_str(), 
					regvals[i],
					type);
			throw RuntimeException (ExceptInfo (func, msg, __FILE__, __LINE__));
		}
		
		if (LoggerPool::sqlHostsInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("::RegQueryValueEx(HKLM\\%s:%s) queried data %s",
					subkey.c_str(), 
					regvals[i],
					data[i]);
			LoggerPool::sqlHostsInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
					                                   msg, 
													   __FILE__, __LINE__);
		}
	}

    ::RegCloseKey( hKey );

	m_Host     = reinterpret_cast<VarString::value_type *>(data[0]);
	m_Protocol = reinterpret_cast<VarString::value_type *>(data[1]);
	m_Service  = reinterpret_cast<VarString::value_type *>(data[2]);

#else
	
	VarString sqlhosts = getSqlHostsPath();
	
	if (LoggerPool::sqlHostsInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("found file %s", sqlhosts.c_str());
		LoggerPool::sqlHostsInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
												   msg, 
												   __FILE__, __LINE__);
	}

	std::ifstream ifstr(sqlhosts.c_str(), std::ios_base::in);

	if (!ifstr.is_open())
	{
		VarString msg;
		msg.format("couldn't open file %s", sqlhosts.c_str());
		throw InvalidStateException (ExceptInfo (func, msg, __FILE__, __LINE__));
	}

	using cmnutil::CollBString;
	
	static const VarString::value_type  comment = '#';
	static const VarString::value_type  tab     = '\t';
	static ConstString                  space   = " ";
	
	VarString   line;
	CollBString items;	// delimited by space

	while (std::getline(ifstr, line))
	{
		line.trim(VarString::FIRST_END);
		line.exchange(tab, space);

		if ( ! line.length() || (line[0] == comment) )
			continue;

		items = line.tokenizeAndTrim(space, VarString::FIRST_END, true, true);

		// line must contain atleast 4 items: server, protocol, host, service(, optional)
		if (4 > items.size())
		{
			continue;
		}

		// entry found
		if (m_DbServer == items[0])
		{
			m_Host     = items[2];
			m_Protocol = items[1];
			m_Service  = items[3];
			
			if (LoggerPool::sqlHostsInfx().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("found line: <%s> <%s> <%s> <%s>",
						   m_DbServer.c_str(),
						   m_Protocol.c_str(),
						   m_Host    .c_str(),
						   m_Service .c_str());
				LoggerPool::sqlHostsInfx().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
														   msg, 
														   __FILE__, __LINE__);
			}
		
			return;
		}
	}
	
	VarString msg;
	msg.format("entry %s not found in file %s", 
			   m_DbServer.c_str(),
			   sqlhosts  .c_str());
	throw InvalidParameterException (ExceptInfo (func, msg, __FILE__, __LINE__));

#endif
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

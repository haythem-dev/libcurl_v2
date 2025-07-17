//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for debug checks
 *  \n       - check memory leaks        
 *  \author Michael Eichenlaub
 *  \date   10.10.2005
 */
//----------------------------------------------------------------------------

#include "checks.h"
#include "libbasardebug.h"

#include <stdio.h>
#include <assert.h>

//----------------------------------------------------------------------------
namespace basar
{
namespace debug
{

//---------------------------------------------------------------------------
Checks & check()
{
	// inited once
	static Checks chk;

	return chk;
}

//---------------------------------------------------------------------------
Checks::Checks() : m_flagCheck    (false),
				   m_flagIgnoreSet(false)
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#ifndef NDEBUG

	_CrtMemCheckpoint(&m_start);	// snapshot for first basar instruction

	// build server doesn't allow GUI elements
	if ( isBuildServer() )
	{
		_CrtSetReportMode(_CRT_WARN  , _CRTDBG_MODE_FILE); 
		_CrtSetReportFile(_CRT_WARN  , _CRTDBG_FILE_STDERR); 
		_CrtSetReportMode(_CRT_ERROR , _CRTDBG_MODE_FILE); 
		_CrtSetReportFile(_CRT_ERROR , _CRTDBG_FILE_STDERR); 
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE); 
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR); 
	}

#endif
#endif
}

//---------------------------------------------------------------------------
Checks::~Checks()
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#ifndef NDEBUG

	// switchable because of QT memory leaks -> Qt must be checked first
	if (!m_flagCheck)
		return;

	_CrtMemCheckpoint(&m_stop);
	_CrtMemState diff;

	if ( ! _CrtMemDifference(&diff, &m_start, &m_stop) )
		return;

	// settings for memory leak dumps
	if ( ! isBuildServer() )
	{
		_CrtSetReportMode(_CRT_WARN  , _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG); 
		_CrtSetReportFile(_CRT_WARN  , _CRTDBG_FILE_STDERR); 
		_CrtSetReportMode(_CRT_ERROR , _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG); 
		_CrtSetReportFile(_CRT_ERROR , _CRTDBG_FILE_STDERR); 
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG); 
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR); 
	}

	if ( ! m_flagIgnoreSet )
	{
		fprintf(stderr, "Memory leak checking:\n");
		fflush(stderr);
		_CrtMemDumpStatistics     (&diff);
		_CrtMemDumpAllObjectsSince(&m_start);		// check leaks with {,,msvcr71d.dll}_crtBreakAlloc
		assert(0);
		return;
	}

	_CrtMemState ignore = { 0 };

	ignore.lCounts[_NORMAL_BLOCK] = m_Ignore.blocks;
	ignore.lSizes [_NORMAL_BLOCK] = m_Ignore.bytes;

	_CrtMemState diff_ign;

	if ( ! _CrtMemDifference(&diff_ign, &diff, &ignore) )
		return;

	fprintf(stderr, "Memory leak checking:\n");
	fflush(stderr);
	_CrtMemDumpStatistics     (&diff_ign);
	_CrtMemDumpAllObjectsSince(&m_start);		// check leaks with {,,msvcr71d.dll}_crtBreakAlloc

	assert(0);

#endif
#endif
}

//----------------------------------------------------------------------------
}	// namespace debug
}	// namespace basar

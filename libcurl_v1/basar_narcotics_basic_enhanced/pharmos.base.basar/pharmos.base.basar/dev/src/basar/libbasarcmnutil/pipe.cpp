//----------------------------------------------------------------------------
/*! \file
 *  \brief  pipe stuff
 *  \author Michael Eichenlaub
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------

#include "pipe.h"

#include <assert.h>
#include <stdlib.h>

/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#endif


//----------------------------------------------------------------------------
/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32

//! Unix signal handler for detecting SIGPIPE
extern "C" void signalpipe (basar::Int32  	//!< signal number
						   )
{
	basar::cmnutil::Pipe::stopWrite2Pipe();
}

#endif

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32

//! flag to broken pipe; is set by signalhandler signalpipe (SIGPIPE)
bool			Pipe::m_Write2Pipe    = false;	// regard: not threadsafe !

//! saves original signal handler for SIGPIPE to restore after child's termination
signalhandler *	Pipe::m_pfnSigHndPrev = 0;

#endif

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
Pipe::Pipe()
{
	init();
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
Pipe::~Pipe()
{
	cleanup();
}

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
void Pipe::init()
{
	m_hPipeReadOut  = INVALID_HANDLE_VALUE;
	m_hPipeWriteOut = INVALID_HANDLE_VALUE;
	m_hPipeReadErr  = INVALID_HANDLE_VALUE;
	m_hPipeWriteErr = INVALID_HANDLE_VALUE;
	m_hPipeReadIn   = INVALID_HANDLE_VALUE;
	m_hPipeWriteIn  = INVALID_HANDLE_VALUE;

	m_hSaveOut      = INVALID_HANDLE_VALUE;
	m_hSaveErr      = INVALID_HANDLE_VALUE;
	m_hSaveIn       = INVALID_HANDLE_VALUE;

	::ZeroMemory( &m_ProcInfo, sizeof(m_ProcInfo ));
	m_ProcInfo.hProcess = INVALID_HANDLE_VALUE;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
void Pipe::cleanup()
{
	closePipes    ();
	restoreStdIo  ();
	terminateChild();
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32 Pipe::saveStdIo()
{
	m_hSaveOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	m_hSaveErr = ::GetStdHandle(STD_ERROR_HANDLE);
	m_hSaveIn  = ::GetStdHandle(STD_INPUT_HANDLE);

	if ( (INVALID_HANDLE_VALUE == m_hSaveOut) || (0 == m_hSaveOut) ||
		 (INVALID_HANDLE_VALUE == m_hSaveErr) || (0 == m_hSaveErr) ||
		 (INVALID_HANDLE_VALUE == m_hSaveIn ) || (0 == m_hSaveIn )    )
		return ::GetLastError();

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32 Pipe::restoreStdIo()
{
	if ( (INVALID_HANDLE_VALUE != m_hSaveOut) && !::SetStdHandle(STD_OUTPUT_HANDLE, m_hSaveOut) ||
	 	 (INVALID_HANDLE_VALUE != m_hSaveErr) && !::SetStdHandle(STD_ERROR_HANDLE , m_hSaveErr) ||
		 (INVALID_HANDLE_VALUE != m_hSaveIn ) && !::SetStdHandle(STD_INPUT_HANDLE , m_hSaveIn )    )
		return ::GetLastError();

	m_hSaveOut = INVALID_HANDLE_VALUE;
	m_hSaveErr = INVALID_HANDLE_VALUE;
	m_hSaveIn  = INVALID_HANDLE_VALUE;

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32	Pipe::closePipes()
{
	if ( closePipe(m_hPipeReadOut ) || closePipe(m_hPipeWriteOut) ||
		 closePipe(m_hPipeReadErr ) || closePipe(m_hPipeWriteErr) ||
		 closePipe(m_hPipeReadIn  ) || closePipe(m_hPipeWriteIn )   )
		return ::GetLastError();

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32 Pipe::closePipe(::HANDLE & hPipe)
{
	if (INVALID_HANDLE_VALUE != hPipe)
	{
		if ( !::CloseHandle(hPipe) )
			return ::GetLastError();

		hPipe = INVALID_HANDLE_VALUE;
	}

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32 Pipe::terminateChild()
{
	if ( (INVALID_HANDLE_VALUE != m_ProcInfo.hProcess)          && 
		 !::TerminateProcess(m_ProcInfo.hProcess, EXIT_FAILURE) &&
		 !::CloseHandle     (m_ProcInfo.hProcess)                  )
		return ::GetLastError();

	::ZeroMemory( &m_ProcInfo, sizeof(m_ProcInfo ));
	m_ProcInfo.hProcess = INVALID_HANDLE_VALUE;

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code
ULong32 Pipe::closeChild()
{
	if ( (INVALID_HANDLE_VALUE != m_ProcInfo.hProcess) && 
		 !::CloseHandle(m_ProcInfo.hThread)            &&
		 !::CloseHandle(m_ProcInfo.hProcess)              )
		return ::GetLastError();

	::ZeroMemory( &m_ProcInfo, sizeof(m_ProcInfo ));
	m_ProcInfo.hProcess = INVALID_HANDLE_VALUE;

	return ERROR_SUCCESS;
}

//----------------------------------------------------------------------------
#else	// Unix

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
void Pipe::stopWrite2Pipe()
{
	m_Write2Pipe = false;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
void Pipe::init()
{
	m_PipeIn [PIPEREAD] = m_PipeIn [PIPEWRITE] =
	m_PipeOut[PIPEREAD] = m_PipeOut[PIPEWRITE] = 
	m_PipeErr[PIPEREAD] = m_PipeErr[PIPEWRITE] = PIPE_FD_INVALID;

	m_Pid           = 0;
	m_pfnSigHndPrev = 0;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
void Pipe::cleanup()
{
	closePipes      ();
	terminateChild  ();
	resetSignHandler();
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code (0 == success)
Int32 Pipe::closePipes()
{
	if ( closePipe(m_PipeIn [PIPEREAD]) || closePipe(m_PipeIn [PIPEWRITE]) ||
		 closePipe(m_PipeOut[PIPEREAD]) || closePipe(m_PipeOut[PIPEWRITE]) ||
		 closePipe(m_PipeErr[PIPEREAD]) || closePipe(m_PipeErr[PIPEWRITE])   )
		return errno;

	return 0;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code (0 == success)
Int32 Pipe::closePipe(Int32 & pipe)
{
	if (PIPE_FD_INVALID != pipe)
	{
		if ( 0 > ::close(pipe) )
			return errno;

		pipe = PIPE_FD_INVALID;
	}

	return 0;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code (0 == success)
Int32 Pipe::terminateChild()
{
	if (0 != m_Pid)
	{
		if (0 > ::kill(m_Pid, SIGTERM))
			return errno;

		m_Pid = 0;
	}

	return 0;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code (0 == success)
Int32 Pipe::setSignHandler()
{
	assert(0 == m_pfnSigHndPrev);

	if ( SIG_ERR == (m_pfnSigHndPrev = signal(SIGPIPE, signalpipe)) )
		return errno;

	return 0;
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return error code (0 == success)
Int32 Pipe::resetSignHandler()
{
	if (0 != m_pfnSigHndPrev)
	{
		if ( signalpipe != signal(SIGPIPE, m_pfnSigHndPrev) )
			return errno;

		m_pfnSigHndPrev = 0;
	}

	return 0;
}

//----------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------
}	//cmnutil
}	//basar

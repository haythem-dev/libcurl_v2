//----------------------------------------------------------------------------
/*! \file
 *  \brief  pipe stuff
 *  \author Michael Eichenlaub
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_PIPE_H
#define GUARD_LIBBASARCMNUTIL_PIPE_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#endif

//----------------------------------------------------------------------------
/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32

//! Unix signal handler for detecting SIGPIPE
extern "C" typedef void signalhandler (basar::Int32 signo	//!< signal number
									  );

#endif

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! \brief pipe descriptor/handles
//! \n final class
class Pipe
{
public:
	
								Pipe			();		//!< constructor		\n no-throw
							   ~Pipe			();		//!< d'estructor		\n no-throw
	void						init			();		//!< init members		\n no-throw
	void						cleanup			();		//!< clean resources	\n no-throw

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	//! close pipe handle						\n no-throw
	static ULong32				closePipe		(::HANDLE & hPipe	//!< pipe handle to close
												);
	//! save standard IO handles				\n no-throw
	ULong32						saveStdIo		();
	//! restore saved standard IO handles		\n no-throw
	ULong32						restoreStdIo	();
	//! close all pipe handles					\n no-throw
	ULong32						closePipes		();
	//! terminate child process (only when child isn't yet finished)	\n no-throw
	ULong32						terminateChild	();
	//! close child's process handles			\n no-throw
	ULong32						closeChild		();


	::HANDLE					m_hPipeReadOut;		//!< parent: duplicated noninherited read handle for stdout
	::HANDLE					m_hPipeWriteOut;	//!< child : write handle for stdout

	::HANDLE					m_hPipeReadErr;		//!< parent: duplicated noninherited read handle for stderr
	::HANDLE					m_hPipeWriteErr;	//!< child : write handle for stderror

	::HANDLE					m_hPipeReadIn;		//!< child : read handle for stdinput
	::HANDLE					m_hPipeWriteIn;		//!< parent: duplicated noninherited write handle for stdin

	::HANDLE					m_hSaveOut;			//!< parent: save stdout   handle for restore
	::HANDLE					m_hSaveErr;			//!< parent: save stderror handle for restore
	::HANDLE					m_hSaveIn;			//!< parent: save stdin    handle for restore

	::PROCESS_INFORMATION		m_ProcInfo;			//!< child process information

#else

	enum PipeEnum		//!< pipe handling constants
	{
		PIPE_FD_INVALID = -1,	//!< value for invalid pipe descriptor

		PIPEREAD        =  0,	//!< pipe's read  end
		PIPEWRITE       =  1	//!< pipe's write end
	};

	//! set flag for stopping writing to child (called by signal handler)	\n no-throw
	static void					stopWrite2Pipe	();
	//! close pipe handle						\n no-throw
	static Int32				closePipe		(Int32 & pipe	//!< pipe descriptor to close
												);

	//! close all pipe handles					\n no-throw
	Int32						closePipes		();
	//! terminate child process (only when child isn't yet finished)	\n no-throw
	Int32						terminateChild	();
	//! set signal handler for SIGPIPE			\n no-throw
	Int32						setSignHandler	();
	//! reset signal handler for SIGPIPE to previous handler			\n no-throw
	Int32						resetSignHandler();


	Int32						m_PipeIn [2];		//!< stdinput
	Int32						m_PipeOut[2];		//!< stdoutput
	Int32						m_PipeErr[2];		//!< stderror

	pid_t						m_Pid;				//!< child process ID

	static bool                 m_Write2Pipe;		//!< decide to write further in pipe
	static signalhandler *		m_pfnSigHndPrev;	//!< save previous Unix signal handler for restoring

#endif
};


//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

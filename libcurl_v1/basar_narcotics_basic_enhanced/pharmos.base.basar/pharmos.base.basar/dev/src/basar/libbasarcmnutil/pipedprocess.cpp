//----------------------------------------------------------------------------
/*! \file
 *  \brief  piped child process (Windows/Unix)
 *  \author Michael Eichenlaub
 *  \date   17.05.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_pipedprocess.h"
#include "pipe.h"
#include "libbasarcmnutil_exceptions.h"
#include "loggerpool.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifndef _WIN32
#include <sys/wait.h>
#include <errno.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <limits.h>

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
PipedProcess::PipedProcess ( const VarString & command) 
                           : m_pIntern      (new Pipe),
						     m_pDataProvider(0),
							 m_Command      (command)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
PipedProcess::PipedProcess ( const VarString      & command,
						     IProcessDataProvider & rDataProvider) 
						   : m_pIntern      (new Pipe      ),
							 m_pDataProvider(&rDataProvider),
							 m_Command      (command       )
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
PipedProcess::~PipedProcess()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \return child's exit code
Int32 PipedProcess::execute()
{
	clearColls();

	createPipe();
	createProcess();

	handleProcessData();

	Int32 retcode = closeProcess();
	closePipe();

	return retcode;
}
//----------------------------------------------------------------------------
//! \throw RuntimeException
//! 
void PipedProcess::createPipe()
{
	static ConstString func = "basar.cmnutil.PipedProcess.createPipe()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	m_pIntern->saveStdIo();

	::SECURITY_ATTRIBUTES secAttrib;
	::ZeroMemory(&secAttrib, sizeof(secAttrib));
	secAttrib.nLength              = sizeof(secAttrib);
	secAttrib.bInheritHandle       = TRUE;					// inherit handle to child process
	secAttrib.lpSecurityDescriptor = NULL;

	::HANDLE hReadOut;
	::HANDLE hReadErr;
	::HANDLE hWriteIn;
    
	if ( !::CreatePipe(&hReadOut                  , &(m_pIntern->m_hPipeWriteOut), &secAttrib, 0) ||
	     !::CreatePipe(&hReadErr                  , &(m_pIntern->m_hPipeWriteErr), &secAttrib, 0) ||   
		 !::CreatePipe(&(m_pIntern->m_hPipeReadIn), & hWriteIn                   , &secAttrib, 0)    )	
	{
		VarString msg;
		msg.format("CreatePipe() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

	// set parent handles to pipe handles --> inherited by child
	if ( !::SetStdHandle(STD_OUTPUT_HANDLE, m_pIntern->m_hPipeWriteOut) ||
		 !::SetStdHandle(STD_ERROR_HANDLE , m_pIntern->m_hPipeWriteErr) ||
		 !::SetStdHandle(STD_INPUT_HANDLE , m_pIntern->m_hPipeReadIn  )    )
 	{
		VarString msg;
		msg.format("::SetStdHandle() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

	// create noninheritable duplicates of pipe handles
	HANDLE curProcess = ::GetCurrentProcess();

	if ( !DuplicateHandle(curProcess,   hReadOut                  ,	// source
		                  curProcess, &(m_pIntern->m_hPipeReadOut),	// target
		                  0,										// ignored
						  FALSE,									// not inherited       
		                  DUPLICATE_SAME_ACCESS                   )	// same access as source handle
		||
	     !DuplicateHandle(curProcess,   hReadErr                  ,	// source
		                  curProcess, &(m_pIntern->m_hPipeReadErr),	// target
		                  0,										// ignored
						  FALSE,									// not inherited       
		                  DUPLICATE_SAME_ACCESS                   )	// same access as source handle
		||
		 !DuplicateHandle(curProcess,   hWriteIn                  ,	// source
		                  curProcess, &(m_pIntern->m_hPipeWriteIn),	// target
		                  0,										// ignored
						  FALSE,									// not inherited       
		                  DUPLICATE_SAME_ACCESS                   )	// same access as source handle
		)
 	{
		VarString msg;
		msg.format("::DuplicateHandle() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

	// close duplicated inheritable pipe handles
	if ( !CloseHandle(hReadOut) || !CloseHandle(hReadErr) || !CloseHandle(hWriteIn) )
 	{
		VarString msg;
		msg.format("::CloseHandle() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

#else

	if ( (0 > pipe(m_pIntern->m_PipeIn )) ||
		 (0 > pipe(m_pIntern->m_PipeOut)) ||
		 (0 > pipe(m_pIntern->m_PipeErr))    )
	{
		VarString msg;
		msg.format("pipe() failed with %d", errno);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

#endif

	if (LoggerPool::pipedProcess().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("createPipe()");
		LoggerPool::pipedProcess().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  RuntimeException
//! 
void PipedProcess::closePipe()
{
	static ConstString func = "basar.cmnutil.PipedProcess.closePipe()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	ULong32 ret;

	if ( ERROR_SUCCESS != (ret = m_pIntern->closePipes()) )
	{
		VarString msg;
		msg.format("closePipes failed with %d", ret);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

#else

	Int32 ret;

	if ( 0 != (ret = m_pIntern->closePipes()) )
	{
		VarString msg;
		msg.format("closePipes() failed with %d", ret);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

#endif


	if (LoggerPool::pipedProcess().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("closePipe()");
		LoggerPool::pipedProcess().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw RuntimeException
//! 
void PipedProcess::createProcess()
{
	static ConstString func = "basar.cmnutil.PipedProcess.createProcess()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	::STARTUPINFO startInfo;
	::ZeroMemory( &startInfo, sizeof(startInfo));

	startInfo.cb           = sizeof(startInfo);
	startInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startInfo.wShowWindow  = SW_HIDE;
	startInfo.hStdOutput   = m_pIntern->m_hPipeWriteOut;
	startInfo.hStdError    = m_pIntern->m_hPipeWriteErr;
	startInfo.hStdInput    = m_pIntern->m_hPipeReadIn ;

	static ConstString SHELL  = "cmd";
	static ConstString OPTION = "/c";
	char               cmd[MAX_PATH];	// changeable copy of member variable

	sprintf(cmd, "%s %s %s",
		    SHELL,
			OPTION,
		    m_Command.c_str());

	if ( !::CreateProcess(	0,								// pointer to name of executable module
							cmd,							// command line 
							0,								// pointer to process security attributes 
							0,								// pointer to thread security attributes (use primary thread security attributes)
							TRUE,							// inherit handles
							0,								// creation flags
							0,								// pointer to new environment block (use parent's)
							0,								// pointer to current directory name
							&startInfo,						// pointer to STARTUPINFO
							&(m_pIntern->m_ProcInfo) ) )	// pointer to PROCESS_INFORMATION
	{
		VarString msg;
		msg.format("CreateProcess() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	m_pIntern->restoreStdIo();

#else

	if ( 0 > (m_pIntern->m_Pid = fork()) )		// failure
	{
		VarString msg;
		msg.format("fork() failed with %d", errno);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	if ( 0 < m_pIntern->m_Pid )					// parent process
	{
		m_pIntern->m_Write2Pipe = true;

		Int32 ret;

		if ( 0 != (ret = m_pIntern->setSignHandler()) )
		{
			VarString msg;
			msg.format("installing parent's signal handler failed with %d", ret);
			throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
		}

		if ( (0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeIn [Pipe::PIPEREAD ]))) ||		// close pipe's not used read  channel
		     (0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeOut[Pipe::PIPEWRITE]))) ||		// close pipe's not used write channel
		     (0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeErr[Pipe::PIPEWRITE])))    )	// close pipe's not used write channel
		{
			VarString msg;
			msg.format("parent's pipe close() failed with %d", ret);
			throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
		}
	}

	if ( 0 == m_pIntern->m_Pid )				// child process
	{
		Int32 ret;

		if ((0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeIn [Pipe::PIPEWRITE]))) ||		// close pipe's not used write channel
		    (0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeOut[Pipe::PIPEREAD ]))) ||		// close pipe's not used read  channel
		    (0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeErr[Pipe::PIPEREAD ])))    )		// close pipe's not used read  channel
		{
			VarString msg;
			msg.format("child's pipe close() failed with %d", ret);
			throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
		}

		// try to set child's stdin to pipe read channel (only if it hasn't already been set)
		if (STDIN_FILENO != m_pIntern->m_PipeIn[Pipe::PIPEREAD])		
		{
			// duplicate pipe's read channel to stdin
			if ( STDIN_FILENO != dup2(m_pIntern->m_PipeIn[Pipe::PIPEREAD], STDIN_FILENO) )
			{
				VarString msg;
				msg.format("stdin: dup2() failed with %d", errno);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
			}

            // close not needed pipe's read channel						
			if ( 0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeIn[Pipe::PIPEREAD])) )
			{
				VarString msg;
				msg.format("child's pipe close() failed with %d", ret);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
			}
		}

		// try to set child's stdout to pipe write channel (only if it hasn't already been set)
		if (STDOUT_FILENO != m_pIntern->m_PipeOut[Pipe::PIPEWRITE])		
		{
			// duplicate pipe's write channel to stdout
			if ( STDOUT_FILENO != dup2(m_pIntern->m_PipeOut[Pipe::PIPEWRITE], STDOUT_FILENO) )
			{
				VarString msg;
				msg.format("stdout: dup2() failed with %d", ret);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
			}

            // close not needed pipe's write channel						
			if ( 0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeOut[Pipe::PIPEWRITE])) )
			{
				VarString msg;
				msg.format("child's pipe close() failed with %d", ret);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
			}
		}

		// try to set child's stderr to pipe write channel (only if it hasn't already been set)
		if (STDERR_FILENO != m_pIntern->m_PipeErr[Pipe::PIPEWRITE])		
		{
			// duplicate pipe's write channel to stderr
			if ( STDERR_FILENO != dup2(m_pIntern->m_PipeErr[Pipe::PIPEWRITE], STDERR_FILENO) )
			{
				VarString msg;
				msg.format("stderr: dup2() failed with %d", errno);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
			}

            // close not needed pipe's write channel						
			if ( 0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeErr[Pipe::PIPEWRITE])) )
			{
				VarString msg;
				msg.format("child's pipe close() failed with %d", errno);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
			}
		}

		static ConstString SHELL  = "sh";
		static ConstString OPTION = "-c";

		if ( 0 > execlp(SHELL, SHELL, OPTION, m_Command.c_str(), NULL) )	// execute command
			exit(errno);
	}

#endif

	if (LoggerPool::pipedProcess().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("createProcess(\"%s\")", m_Command.c_str());
		LoggerPool::pipedProcess().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  RuntimeException
//! \return child's exit code
Int32 PipedProcess::closeProcess() const
{
	static ConstString func = "basar.cmnutil.PipedProcess.closeProcess()";
	Int32              retCode;

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	DWORD exitCode;

	if ( !::GetExitCodeProcess(m_pIntern->m_ProcInfo.hProcess, &exitCode) )
	{
		VarString msg;
		msg.format("GetExitCodeProcess(ProcInfo.hProcess) failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	retCode = exitCode;

	ULong32 ret;

	if ( ERROR_SUCCESS != (ret = m_pIntern->closeChild()) )
	{
		VarString msg;
		msg.format("closeChild() failed with %d", ret);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

#else

	assert(0 < m_pIntern->m_Pid);

	Int32 state;

	if ( 0 > waitpid(m_pIntern->m_Pid, &state, 0) )
	{
		VarString msg;
		msg.format("waitpid() failed with %d", errno);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	if ( WIFEXITED(state) )
		retCode = WEXITSTATUS(state);
	else
	{
		if ( WIFSIGNALED(state) )
		{
			VarString msg;
			msg.format("child process (%d) aborted by signal %d", 
				       m_pIntern->m_Pid,
					   WTERMSIG(state));
			throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
		}
		else
		{
			if ( WIFSTOPPED(state) )
			{
				VarString msg;
				msg.format("child process (%d) stopped by signal %d", 
						m_pIntern->m_Pid,
						WSTOPSIG(state));
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
			}
			else
			{
				VarString msg;
				msg.format("child process (%d) exited with unknown state %d", 
						m_pIntern->m_Pid,
						state);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
			}
		}
	}

	m_pIntern->m_Pid = 0;

	Int32 ret;
	if ( 0 != (ret = m_pIntern->resetSignHandler()) )
	{
		VarString msg;
		msg.format("restoring parent's signal handler failed with %d", ret);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
	}

#endif

	if (LoggerPool::pipedProcess().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("closeProcess() = %d", retCode);
		LoggerPool::pipedProcess().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retCode;
}


//----------------------------------------------------------------------------
//! \throw  RuntimeException
//! 
void PipedProcess::handleProcessData()
{
	static ConstString  func = "basar.cmnutil.PipedProcess.handleProcessData()";

	bool                flagOut   = true;
	bool                flagError = true;
	
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	//static const basar::ULong32 MILLISEC = 100;
	ULong32                     retProcess;

/*	doesn't work although it's recommended by MSDN "creating a child process with redirected input and output" 

	if ( !::CloseHandle(m_pIntern->m_hPipeReadIn  ) ||
		 !::CloseHandle(m_pIntern->m_hPipeWriteOut) ||
		 !::CloseHandle(m_pIntern->m_hPipeWriteErr)    )
		 assert(0);

	m_pIntern->m_hPipeWriteOut = 
	m_pIntern->m_hPipeWriteErr = 
	m_pIntern->m_hPipeReadIn   = INVALID_HANDLE_VALUE;
*/

	for (;;)
	{
		if (m_pDataProvider)
		{
			if ( m_CollInput.empty() )
				m_pDataProvider->onSendInput(m_CollInput);		// give caller chance to send data
		}

		if ( m_CollInput.empty() )		// no more data -> send child EOF and don't write anymore
		{
			ULong32 ret;
			if ( ERROR_SUCCESS != (ret = m_pIntern->closePipe(m_pIntern->m_hPipeWriteIn)) )
			{
				VarString msg;
				msg.format("closePipe(hPipeWriteIn) failed with %d", ret);
				throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
			}
		}
		else
			writePipe(&(m_pIntern->m_hPipeWriteIn), m_CollInput);

		if ( true == (flagOut = readPipe(&(m_pIntern->m_hPipeReadOut), m_CollOutput)) )
		{
			if ( m_pDataProvider )
				m_pDataProvider->onReceiveOutput(m_CollOutput);
		}

		if ( true == (flagError = readPipe(&(m_pIntern->m_hPipeReadErr), m_CollError)) )
		{
			if ( m_pDataProvider )
				m_pDataProvider->onReceiveError(m_CollError);
		}

		if ( !flagOut && !flagError)
		{
			// wait until child process exits
			if ( WAIT_OBJECT_0 == (retProcess = ::WaitForSingleObject( m_pIntern->m_ProcInfo.hProcess, 0)) )
				break;

			//::Sleep(MILLISEC);	// look for new data after little rest (but slows down) 
		}
	}

#else

	fd_set      fdSetRead;
	fd_set      fdSetWrite;
	Int32       retSelect;
	const Int32 cntFd = 1 + std::max(m_pIntern->m_PipeOut[Pipe::PIPEREAD],
		                             std::max(m_pIntern->m_PipeOut[Pipe::PIPEREAD], 
		                                      m_pIntern->m_PipeErr[Pipe::PIPEREAD]));
	timeval timeout;
	timeout.tv_sec    = 0;
	timeout.tv_usec   = 10 * 1000;	// 10 ms

	for (;;)
	{
		FD_ZERO(&fdSetRead);
		FD_SET (m_pIntern->m_PipeOut[Pipe::PIPEREAD], &fdSetRead);
		FD_SET (m_pIntern->m_PipeErr[Pipe::PIPEREAD], &fdSetRead);

		FD_ZERO(&fdSetWrite);
		if ( m_pIntern->m_Write2Pipe )
			FD_SET (m_pIntern->m_PipeIn[Pipe::PIPEWRITE], &fdSetWrite);

		if (0 > (retSelect = select(cntFd, &fdSetRead, &fdSetWrite, 0, &timeout)))	
		{
			VarString msg;
			msg.format("select() returned %d and failed with %d", 
				       retSelect, 
					   errno);
			throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
		}

		if (0 < retSelect)	// file descriptor is ready for being written/read
		{
			if ( m_pIntern->m_Write2Pipe && FD_ISSET(m_pIntern->m_PipeIn[Pipe::PIPEWRITE], &fdSetWrite))
			{
				if (m_pDataProvider)
				{
					if ( m_CollInput.empty() )
						m_pDataProvider->onSendInput(m_CollInput);		// give caller chance to send data
				}

				if ( m_CollInput.empty() )		// no more data -> send child EOF and don't write anymore
				{
					m_pIntern->m_Write2Pipe = false;

					Int32 ret;
					if ( 0 != (ret = m_pIntern->closePipe(m_pIntern->m_PipeIn[Pipe::PIPEWRITE])) )
					{
						VarString msg;
						msg.format("closePipe(stdinput) failed with %d", ret);
						throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ) );
					}
				}
				else
					m_pIntern->m_Write2Pipe = writePipe(&m_pIntern->m_PipeIn[Pipe::PIPEWRITE], m_CollInput);
			}

			if (FD_ISSET(m_pIntern->m_PipeOut[Pipe::PIPEREAD], &fdSetRead))
			{
				if ( true == (flagOut = readPipe(&m_pIntern->m_PipeOut[Pipe::PIPEREAD], m_CollOutput)) )
				{
					if ( m_pDataProvider )
						m_pDataProvider->onReceiveOutput(m_CollOutput);
				}
			}
		
			if (FD_ISSET(m_pIntern->m_PipeErr[Pipe::PIPEREAD], &fdSetRead))
			{
				if ( true == (flagError = readPipe(&m_pIntern->m_PipeErr[Pipe::PIPEREAD], m_CollError)) )
				{
					if ( m_pDataProvider )
						m_pDataProvider->onReceiveError(m_CollError);
				}
			}
		}

	    if ( !flagOut && !flagError)	// pipes are closed by child -> EOF
			break;
	}

#endif

	if ( m_pDataProvider )
	{
		CollStdIo::size_type size;
		
		// --- stdout
		size = m_CollOutput.size() + 1;				// init + 1  -> for while condition

		while ( (0    < m_CollOutput.size()) &&		// collection not empty()
				(size > m_CollOutput.size())    )	// dataprovider emptied collection
		{
			size = m_CollOutput.size();						// save old value
			m_pDataProvider->onReceiveOutput(m_CollOutput);	// give caller chance to pick up data
		}

		// --- stderr
		size = m_CollError.size() + 1;				// init + 1  -> for while condition

		while ( (0    < m_CollError.size()) &&		// collection not empty()
				(size > m_CollError.size())    )	// dataprovider emptied collection
		{
			size = m_CollError.size();						// save old value
			m_pDataProvider->onReceiveError(m_CollError);	// give caller chance to pick up data
		}
	}

	if (LoggerPool::pipedProcess().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("handleProcessData()");
		LoggerPool::pipedProcess().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  RuntimeException
//! \return bool indication success
bool PipedProcess::writePipe(void * pPipe, CollStdIo &	rColl)
{
	static ConstString   func = "basar.cmnutil.PipedProcess.writePipe()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	static const ULong32 BUFSIZE  = 4096;								// size, in bytes, of data buffer 
	char	             buffer[BUFSIZE];								// receives pipe data 

	DWORD bytesWritten;
	ULong32 bytes2Write = min(BUFSIZE, static_cast<ULong32>(rColl.size()));
	HANDLE	hPipe       = *static_cast< ::HANDLE * > (pPipe);	// pipe to write into

	char *  pData;
	ULong32 i;

	for (i = 0, pData = buffer; i < bytes2Write; ++i, ++pData)
	{
		*pData = rColl.front();		// copy char from queue to ps buffer
		rColl.pop();				// delete char from queue
	}

	// blocking write
	if ( !::WriteFile (hPipe, buffer, bytes2Write, &bytesWritten, 0) )
	{
		VarString msg;
		msg.format("ReadFile() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

#else
	static const ULong32 BUFSIZE  = PIPE_BUF;				// size, in bytes, of data buffer 
	char	             buffer[BUFSIZE];					// send data into pipe

	ssize_t bytesWritten;
	ssize_t bytes2Write = std::min(BUFSIZE, static_cast<ULong32>(rColl.size()));
	Int32   fdPipe      = *static_cast< Int32 * > (pPipe);		// pipe to write into

	char * pData;
	Int32  i;
	for (i = 0, pData = buffer; i < bytes2Write; ++i, ++pData)
	{
		*pData = rColl.front();		// copy char from queue to ps buffer
		rColl.pop();				// delete char from queue
	}

	if (bytes2Write != (bytesWritten = write(fdPipe, buffer, bytes2Write)) )
	{
		if ((-1 == bytesWritten) && (EPIPE == errno))
			return false;

		VarString msg;
		msg.format("write(pipe = %d) failed with %d", 
			       fdPipe,
			       errno);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

#endif

	return true;
}

//----------------------------------------------------------------------------
//! \throw  RuntimeException
//! \return bool indicating success
bool PipedProcess::readPipe(void * pPipe, CollStdIo & rColl)
{
	static ConstString   func = "basar.cmnutil.PipedProcess.readPipe()";

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

	static const ULong32 BUFSIZE  = 4096;								// size, in bytes, of data buffer 
	char	             buffer[BUFSIZE];								// receives pipe data 

	HANDLE				 hPipe    = *static_cast< ::HANDLE * > (pPipe);	// pipe to copy from 

	DWORD                bytesRead;										// number of bytes read 
	DWORD                totalBytesAvailable;							// total number of bytes available
	DWORD  				 bytesLeft;										// unread bytes

	// asynchron -> non-blocking: look for data; doesn't remove data from pipe
	if ( !::PeekNamedPipe ( hPipe, buffer, 1, &bytesRead, &totalBytesAvailable,	&bytesLeft) )
	{
		VarString msg;
		msg.format("PeekNamedPipe() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	if ( 0 == bytesRead )	// no data
		return false;

	// blocking read
	if ( !::ReadFile (hPipe, buffer, BUFSIZE, &bytesRead, 0) )
	{
		VarString msg;
		msg.format("ReadFile() failed with %d", ::GetLastError());
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	if ( 0 == bytesRead )	// no data
		return false;

#else

	static const ULong32 BUFSIZE  = PIPE_BUF;				// size, in bytes, of data buffer 
	char	             buffer[BUFSIZE];					// receives pipe data 

	ssize_t bytesRead;
	Int32   fdPipe = 	*static_cast< Int32 * > (pPipe);		// pipe to copy from 

	if (-1 == (bytesRead = read(fdPipe, buffer, BUFSIZE)) )
	{
		VarString msg;
		msg.format("read(pipe = %d) failed with %d", 
			       fdPipe,
			       errno);
		throw RuntimeException(ExceptInfo(func, msg, __FILE__, __LINE__ ));
	}

	if (0 == bytesRead)
        return false;
	 
#endif

	for (const char * pData = buffer; bytesRead > 0; --bytesRead, ++pData)
	    rColl.push(*pData);

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
void PipedProcess::clearColls()
{
	CollStdIo * collArray[] = 
	{
		&m_CollOutput,
		&m_CollError
	};

	for (UInt32 i = 0; i < (sizeof(collArray)/sizeof(*collArray)); ++i)
	{
		while ( !collArray[i]->empty() )
			collArray[i]->pop();
	}
}

//----------------------------------------------------------------------------
//! \throw no-throw 
//! \return collection with child's stdinput
CollStdIo & PipedProcess::getInput()
{
	return m_CollInput;
}

//----------------------------------------------------------------------------
//! \throw no-throw 
//! \return collection with child's stdoutput
CollStdIo & PipedProcess::getOutput()
{
	return m_CollOutput;
}

//----------------------------------------------------------------------------
//! \throw no-throw 
//! \return collection with child's stderror
CollStdIo & PipedProcess::getError()
{
	return m_CollError;
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar

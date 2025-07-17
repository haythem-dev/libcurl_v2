//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task submitter implementation
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "remotetasksubmitterimpl.h"
#include "remotetaskexecution.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_remotetaskresultref.h"
#include "libbasarcmnutil_remotetaskref.h"

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

#include <libssh2.h>

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif


#include "loggerpool.h"

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//

namespace basar
{
	namespace cmnutil
	{

		RemoteTaskSubmitterImpl::RemoteTaskSubmitterImpl()
		{
			initBackendLibrary();
		}

		RemoteTaskSubmitterImpl::IRemoteTaskSubmitterPtr RemoteTaskSubmitterImpl::create()
		{
			if (LoggerPool::remoteTask().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
			{
				LoggerPool::remoteTask().forcedLog(log4cplus::TRACE_LOG_LEVEL, "statically creating cmnutil::remotetasksubmitterimpl", __FILE__, __LINE__);
			}

			IRemoteTaskSubmitterPtr t = IRemoteTaskSubmitterPtr(new RemoteTaskSubmitterImpl);
			return t;

		}

		RemoteTaskSubmitterImpl::~RemoteTaskSubmitterImpl()
		{
			shutdownBackendLibrary();
		}

		RemoteTaskResultRef RemoteTaskSubmitterImpl::submitRemoteTask( RemoteTaskRef remoteTask )
		{
			//todo: find better way of constructing a remotetasksharedptr from a remotetaskref
			RemoteTaskExecution remoteExec(remoteTask.getIImpl());

			return remoteExec.execute();
		}

		void RemoteTaskSubmitterImpl::initBackendLibrary()
		{
			//use boost scoped lock - libssh2_init needs to be called from one thread only
			boost::mutex::scoped_lock lock(m_Guard);

			if (LoggerPool::remoteTask().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format( "initializing libssh2, version %s\n", libssh2_version(0) );
				LoggerPool::remoteTask().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
			
			basar::Int32 rc = libssh2_init (0);
			
			if (LoggerPool::remoteTask().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format( "initialization of libssh2 returned: %d\n", rc );
				LoggerPool::remoteTask().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
		}

		void RemoteTaskSubmitterImpl::shutdownBackendLibrary()
		{
			libssh2_exit();
		}
	}
}
//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task submitter
 *  \author Marco Köppendörfer
 *  \date   11.10.2013
 */
//----------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_remotetasksubmitter.h"
#include "libbasarcmnutil_remotetaskresultref.h"
#include "libbasarcmnutil_iremotetaskresultptr.h"
#include "iremotetasksubmitter.h"
#include "remotetasksubmitterimpl.h"
#include "libbasarcmnutil_remotetaskref.h"

#include "libbasarcmnutil_exceptions.h"

//--------------------------------------------------------------------------------------------------//
//  template specialization section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		using cmnutil::RemoteTaskSubmitterBase;

		//!	specialised method
		//! \return shared_ptr to instance
		template <>
		typename Singleton<RemoteTaskSubmitterBase>::InstancePtr & Singleton<RemoteTaskSubmitterBase>::inst()
		{
			static InstancePtr s_Inst;
			return s_Inst;
		}
	}
}

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		RemoteTaskSubmitterBase::RemoteTaskSubmitterBase() : m_pIImpl(RemoteTaskSubmitterImpl::create())
		{
		}
		
		RemoteTaskResultRef RemoteTaskSubmitterBase::submitRemoteTask(RemoteTaskRef execInfo)
		{
			return getIImpl()->submitRemoteTask(execInfo);
		}

		const boost::shared_ptr<IRemoteTaskSubmitter>& RemoteTaskSubmitterBase::getIImpl()
		{
			if (0 == m_pIImpl.get())
			{
				throw NullReferenceException(ExceptInfo( "basar::cmnutil::RemoteTaskSubmitterBase::getIImpl", "m_pIImpl.get() == 0",__FILE__, __LINE__) );
			}

			return m_pIImpl;
		}
	}
}
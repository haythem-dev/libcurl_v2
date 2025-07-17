//----------------------------------------------------------------------------
/*! \file
 *  \brief  ref wrapper of an iremotetaskresultptr
 *  \author Marco Köppendörfer
 *  \date   10.10.2013
 */
//----------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_remotetaskresultref.h"
#include "libbasarcmnutil_exceptions.h"
#include "iremotetaskresult.h"

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		RemoteTaskResultRef::RemoteTaskResultRef(IRemoteTaskResultPtr res) : m_pRemoteTaskResult(res)
		{
		}

		const VarString& RemoteTaskResultRef::getResult() const
		{
			return getIImpl()->getResult();
		}

		bool RemoteTaskResultRef::hasError() const
		{
			return getIImpl()->hasError();
		}

		bool RemoteTaskResultRef::hasSignal() const
		{
			return getIImpl()->hasSignal();
		}

		basar::Int32 RemoteTaskResultRef::getExitCode() const
		{
			return getIImpl()->getExitCode();
		}

		const VarString& RemoteTaskResultRef::getSignal() const
		{
			return getIImpl()->getSignal();
		}

		bool RemoteTaskResultRef::isNull() const
		{
			return (0 == m_pRemoteTaskResult.get());
		}

		void RemoteTaskResultRef::reset()
		{
			m_pRemoteTaskResult.reset();
		}

		const IRemoteTaskResultPtr& RemoteTaskResultRef::getIImpl() const
		{
			if( isNull() )
			{
				throw NullReferenceException(ExceptInfo( "basar::cmnutil::RemoteTaskResultRef::getIImpl", "m_pRemoteTaskResult.get() == 0",__FILE__, __LINE__) );
			}		

			return m_pRemoteTaskResult;
		}

	}
}
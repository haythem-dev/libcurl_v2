//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote endpoint
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_remoteendpoint.h"


//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		RemoteEndpoint::RemoteEndpoint() : m_Port(22), m_Address()
		{
		}

		RemoteEndpoint::RemoteEndpoint(const basar::VarString& address, basar::Int16 port /* = 22 */) : m_Port(port), m_Address(address)
		{

		}

		void RemoteEndpoint::setRemoteEndpointAddress	(const basar::VarString& address)
		{
			m_Address = address;
		}

		void RemoteEndpoint::setRemoteEndpointPort		(const basar::Int16		 port)
		{
			m_Port = port;
		}

		const basar::VarString& RemoteEndpoint::getRemoteEndpointAddress() const
		{
			return m_Address;
		}

		basar::Int16      RemoteEndpoint::getRemoteEndpointPort   () const
		{
			return m_Port;
		}

		bool RemoteEndpoint::isEmpty() const
		{
			return m_Address.empty();
		}
	}
}
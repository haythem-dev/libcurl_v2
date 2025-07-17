//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote endpoint
 *  \author Marco Köppendörfer
 *  \date   04.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_REMOTEENDPOINT_H
#define GUARD_REMOTEENDPOINT_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"


//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		class RemoteEndpoint
		{
			public:
				
				//! \brief c'tor - constructs empty endpoint class (port member is defaulted to port 22)
				LIBBASARCMNUTIL_API RemoteEndpoint();
				
				//! \brief c'tor - constructs endpoint class (port member is defaulted to port 22)
				LIBBASARCMNUTIL_API RemoteEndpoint(const basar::VarString& address, basar::Int16 port = 22);

				//! \brief setter for endpoint address (either ip4 address or hostname)
				LIBBASARCMNUTIL_API void setRemoteEndpointAddress	(const basar::VarString& address);

				//! \brief setter for endpoint port
				LIBBASARCMNUTIL_API void setRemoteEndpointPort		(const basar::Int16		 port);

				//! \brief getter for endpoint address (either ip4 address or hostname)
				LIBBASARCMNUTIL_API const basar::VarString& getRemoteEndpointAddress() const;

				//! \brief getter for endpoint port
				LIBBASARCMNUTIL_API basar::Int16      getRemoteEndpointPort   () const;

				//! \brief returns m_Address is empty
				LIBBASARCMNUTIL_API bool				isEmpty   () const;

			private:
				//functions
				//RemoteEndpoint(const RemoteEndpoint&);
				//RemoteEndpoint& operator=(const RemoteEndpoint&);

				//variables
				basar::Int16     m_Port;    //!< port number
				basar::VarString m_Address; //!< address

		};
	}
}


#endif //GUARD_REMOTEENDPOINT_H
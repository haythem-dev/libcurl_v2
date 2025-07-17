//----------------------------------------------------------------------------
/*! \file
 *  \brief  tcp socket shared ptr
 *  \author Marco Köppendörfer
 *  \date   09.10.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_TCPSOCKETSHAREDPTR_H
#define GUARD_TCPSOCKETSHAREDPTR_H


#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class TcpSocket;

		/*! \brief  typedef to boost::shared_ptr<TcpSocket> */
		typedef boost::shared_ptr<TcpSocket> TcpSocketSharedPtr;
	}
}


#endif //GUARD_TCPSOCKETSHAREDPTR_H
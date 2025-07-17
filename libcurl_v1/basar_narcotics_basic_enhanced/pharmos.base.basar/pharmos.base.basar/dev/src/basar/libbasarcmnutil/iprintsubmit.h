//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for print submitter
 *  \author Michael Eichenlaub
 *  \date   31.08.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PRINTSUBMIT_H
#define GUARD_PRINTSUBMIT_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_socket.h"
#include "libbasarcmnutil_definitions.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! print submit: implemented by Lpr, PortPrint	\n interface class
class IPrintSubmit
{
public:
	//! virtual dummy destructor
	virtual						   ~IPrintSubmit	() {};

	virtual TcpSocket::RetCodeEnum	execute			() = 0;		//!< execute print action: connect, send data, disconnect

	virtual CollStdIo &				getInput		() = 0;		//!< get reference to print submitters 's input collection	
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

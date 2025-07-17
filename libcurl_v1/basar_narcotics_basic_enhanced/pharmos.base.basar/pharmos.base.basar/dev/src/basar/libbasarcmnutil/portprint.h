//----------------------------------------------------------------------------
/*! \file
 *  \brief  port print (jetdirect) class
 *  \author Michael Eichenlaub
 *  \date   31.08.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PORTPRINT_H
#define GUARD_PORTPRINT_H

//----------------------------------------------------------------------------
#include "iprintsubmit.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
class IPrintDataProvider;

//----------------------------------------------------------------------------
//! port print (jetdirect) class	\n final class
class PortPrint : public IPrintSubmit
{
public:
	
	//! constructor	\n no-throw
									PortPrint	(const VarString      & rRemoteHost,					//!< remote host name or ip address ("a.b.c.d")
									             Int16                  servicePort = s_ServicePort		//!< printer service port the daemon listens to
												);
	//! constructor	\n no-throw											
									PortPrint	(const VarString      & rRemoteHost,					//!< remote host name or ip address ("a.b.c.d")
									             IPrintDataProvider   & rDataProvider,					//!< provider for print data
									             Int16                  servicePort = s_ServicePort		//!< printer service port the daemon listens to
										        );
	//! destructor	\n no-throw
	virtual						   ~PortPrint	();

	//! establish connection to printer, send print data, close connection to printer	\n no-throw
	virtual TcpSocket::RetCodeEnum	execute		();

	virtual CollStdIo &				getInput	();		//!< get reference to PortPrint's input collection	\n no-throw


private:

									PortPrint	();                           		//!< forbidden
									PortPrint	(const PortPrint & rCopy);			//!< forbidden
	PortPrint &    					operator =	(const PortPrint & rCopy);			//!< forbidden

	//!	collect print data, send them	\n no-throw 
	void							sendColl	();
	//! copy collection's print data to buffer and send them via socket to printer	\n no-throw
	void							sendData	();

	//--------------------------------------------------------------------------

	static const Int16				s_ServicePort;			//!< printer's default service port

	TcpSocket						m_Socket;				//!< tcp socket for transmitting data to printer

	IPrintDataProvider *			m_pDataProvider;		//!< pointer to data provider (receives/sends print data)

	CollStdIo						m_CollInput;			//!< collection with PortPrint's input
};


//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

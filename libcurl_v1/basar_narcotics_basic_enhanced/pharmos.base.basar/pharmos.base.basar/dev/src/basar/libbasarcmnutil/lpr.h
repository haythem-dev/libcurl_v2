//----------------------------------------------------------------------------
/*! \file
 *  \brief  line printer daemon protocol (LPD) class
 *  \author Michael Eichenlaub
 *  \date   22.08.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LPR_H
#define GUARD_LPR_H

//----------------------------------------------------------------------------
#include "iprintsubmit.h"
#include "libbasarcmnutil_definitions.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
class IPrintDataProvider;

//----------------------------------------------------------------------------
//! lpd protocol client class: lpr	\n final class
class Lpr : public IPrintSubmit
{
public:

	//! constructor	\n no-throw
									Lpr				(const VarString      & rRemoteHost,				//!< remote host name or ip address ("a.b.c.d")
													 const VarString      & rQueue     ,				//!< printer's queue name
										             Int16                  servicePort = s_ServicePort	//!< printer service port the daemon listens to
											        );
	//! constructor	\n no-throw
									Lpr				(const VarString      & rRemoteHost  ,				//!< remote host name or ip address ("a.b.c.d")
													 const VarString      & rQueue       ,				//!< printer's queue name
										             IPrintDataProvider   & rDataProvider,				//!< provider for print data
										             Int16                  servicePort = s_ServicePort	//!< printer service port the daemon listens to
											        );
	//! destructor	\n no-throw
	virtual						   ~Lpr				();

	//! establish connection to printer, send protocol/print data, close connection to printer	\n no-throw
	virtual TcpSocket::RetCodeEnum	execute			();

	virtual CollStdIo &				getInput		();		//!< get reference to lpr's input collection	\n no-throw


private:

	enum PrintFuncEnum							//! LPD protocol commands/subcommands
	{
		PRINT_RECEIVE_JOB         = 2,			//!< command for receiving print job				(printer's view)

		PRINT_RECEIVE_CONTROLFILE = 2,			//!< subcommand	for receiving print control file	(printer's view)
		PRINT_RECEIVE_DATAFILE    = 3			//!< subcommand for receiving print data    file	(printer's view)
	};

	typedef std::vector < char >	CollSendData;		//!< collection with whole print data


	static UInt16					initNextJobId   ();		//!< init first job id			\n no-throw 
	static void						setNextJobId    ();		//!< set following job id		\n no-throw

									Lpr				();                           	//!< forbidden
									Lpr				(const Lpr & rCopy);			//!< forbidden
	Lpr &    						operator =		(const Lpr & rCopy);			//!< forbidden

	void							recvPrintJob	();		//!< printer receives print job			\n no-throw
	void							recvControlFile	();		//!< printer receives control info		\n throw LPDException
	void							recvDataFile	();		//!< printer receives print data		\n throw LPDException

	//! copy send collection's print data to buffer and via socket to printer				\n no-throw
	void							sendPrintData	(CollSendData    & rCollData	//!< collection with whole print data
													) const;
	//! send control info via socket to printer	\n no-throw
	void							sendControlData	(const VarString & rData		//!< control info
													) const;
	//! send LPD commands via socket to printer	\n no-throw
	void							sendCommand		(const VarString & rCommand		//!< LPD protocol command
													) const;
	//! send end-of-data via socket to printer	\n no-throw
	void							sendComplete	() const;
	//! request send-was-OK via socket from printer	\n throw LPDException
	void							getAck			();

	//--------------------------------------------------------------------------

	static const Int16				s_ServicePort;			//!< default service port number 
	static UInt16					s_JobId;				//!< LPD job ID (0 .. 999)
	static ConstString				s_PREFIX_CONTROL;		//!< prefix for LPD's control file
	static ConstString				s_PREFIX_DATA;			//!< prefix for LPD's data file
	static ConstString				s_SPACE;				//!< LPD's space

	TcpSocket						m_Socket;				//!< tcp socket for transmitting data to printer
	VarString						m_Queue;				//!< printer's queue name

	IPrintDataProvider *			m_pDataProvider;		//!< pointer to data provider (receives/sends print data)

	CollStdIo						m_CollInput;			//!< collection with lpr's input
	CollSendData					m_CollSendData;			//!< collection with whole print data	
};


//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD

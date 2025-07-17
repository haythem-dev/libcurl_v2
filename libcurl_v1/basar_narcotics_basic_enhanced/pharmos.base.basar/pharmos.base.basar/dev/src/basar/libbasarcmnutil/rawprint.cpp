//----------------------------------------------------------------------------
/*! \file
 *  \brief  raw print handling via lpr
 *  \author Michael Eichenlaub
 *  \date   16.05.2006
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_print.h"

#include "iprintsubmit.h"
#include "portprint.h"
#include "lpr.h"
#include "loggerpool.h"
#include "libbasarcmnutil_pipedprocess.h"

#include <boost/filesystem/path.hpp>

using boost::filesystem::path;

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
RawPrint::RawPrint () : m_ServicePort(0)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
RawPrint::~RawPrint ()
{
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return printer's return value
PrintRetCodeEnum RawPrint::print(const VarString    & rServer            ,
								 IPrintDataProvider & rDataProvider      ,
							     Int32                fromPage /* = -1 */,
								 Int32                toPage   /* = -1 */)
{
	return (print(rServer, 
		          "", 
				  rDataProvider, 
				  fromPage, 
				  toPage));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return printer's return value
PrintRetCodeEnum RawPrint::print(const VarString    & rServer            ,
								 const VarString    & rPrinter           ,
								 IPrintDataProvider & rDataProvider      ,
							     Int32                fromPage /* = -1 */,
								 Int32                toPage   /* = -1 */)
{
	std::auto_ptr < IPrintSubmit >	submit;

	if ( rPrinter.empty() )
	{
		if (0 == m_ServicePort)
			submit.reset(new PortPrint(rServer, rDataProvider));
		else
			submit.reset(new PortPrint(rServer, rDataProvider, m_ServicePort));
	}
	else
	{
		if (0 == m_ServicePort)
			submit.reset(new Lpr(rServer, rPrinter, rDataProvider));
		else
			submit.reset(new Lpr(rServer, rPrinter, rDataProvider, m_ServicePort));
	}


	// insert trigger before print data
	PrintTools::insertStr2Coll ( PrintTools::getTrigger(rDataProvider.getFormPath    (),
										                rDataProvider.getFormFileName(),
										                rDataProvider.getFormName    (),
														fromPage                       ,
														toPage                         ),
								 submit->getInput()                                     );



	// execute print
	TcpSocket::RetCodeEnum eTcpRet = submit->execute();

	// mapping tcp socket return codes to print return codes
	static const PrintRetCodeEnum	retCodeMap[TcpSocket::TCP_RETCODE_MAX] = 
	{
		PRINTER_SUCCESS,
		PRINTER_OFFLINE,
		PRINTER_CONNREFUSED,
		PRINTER_CONNINPROGRESS
	};

	assert(TcpSocket::TCP_RETCODE_MAX == sizeof(retCodeMap)/sizeof(retCodeMap[0]));

	PrintRetCodeEnum  ePrintRet = retCodeMap[eTcpRet];

	// --- logging
	if (LoggerPool::rawPrint().isEnabledFor((ePrintRet == PRINTER_SUCCESS) ? log4cplus::DEBUG_LOG_LEVEL : log4cplus::WARN_LOG_LEVEL))
	{
		VarString msg;
		msg.format("print(\"%s\",\"%s\", dataprovider, %d, %d) returns %d", 
				   rServer.c_str (),
				   rPrinter.c_str(),
				   fromPage        ,
				   toPage          ,
				   ePrintRet         );
		LoggerPool::rawPrint().forcedLog((ePrintRet == PRINTER_SUCCESS) ? log4cplus::DEBUG_LOG_LEVEL : log4cplus::WARN_LOG_LEVEL, 
											  msg, 
											  __FILE__, __LINE__);
	}

	return ePrintRet;
}

//------------------------------------------------------------------------------
//! \throw RuntimeException
//!
void RawPrint::print(const VarString & rServer       ,
					 const VarString & rPrinter      ,
					 const VarString & rInputFileName)
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	static ConstString FMT  = "lpr -S%s -P%s %s";		// Windows
#else
	static ConstString FMT  = "lpr -P%s:%s %s";			// Solaris
#endif

	VarString file = path(rInputFileName.c_str()).string();

	VarString command;
	command.format(FMT, rServer .c_str(),
						rPrinter.c_str(),
						file    .c_str());

	// --- logging
	if (LoggerPool::rawPrint().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("print(): \"%s\"", command.c_str());
		LoggerPool::rawPrint()	.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}


	// --- open pipe and execute command
	PipedProcess tool(command);
	Int32 retcode = tool.execute();

	// --- check for error during execution: 
	// Windows: errors are reported in stdout! Wrong IP address returns 0, but stdout is filled
	// Solaris: errors are reported in stderr, but wrong IP address returns 0 (no stderr)!
	if ( (0 != retcode             ) || 
		 (!tool.getError ().empty()) ||
		 (!tool.getOutput().empty())    )
	{
		VarString   error;
		CollStdIo & rCollErr = tool.getError();

		while ( !rCollErr.empty() )
		{
			error += rCollErr.front();
			rCollErr.pop();
		}

		VarString output;
		CollStdIo & rCollOut = tool.getOutput();

		while ( !rCollOut.empty() )
		{
			output += rCollOut.front();
			rCollOut.pop();
		}


		VarString msg;
		msg.format( "executing(\"%s\") failed with %d\n"
					"error :\n%s" 
					"output:\n%s", 
			        command.c_str(),
				    retcode,
				    error .c_str(),
				    output.c_str());
		throw RuntimeException( ExceptInfo( "basar.cmnutil.RawPrint.print", msg, __FILE__, __LINE__ ) );
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void RawPrint::setPort(Int16 port)
{
	m_ServicePort = port;
}

//----------------------------------------------------------------------------
}	// cmnutil
}	// basar

//----------------------------------------------------------------------------
/*! \file
 *  \brief  process data provider for raw print
 *  \author Michael Eichenlaub
 *  \date   30.06.2006
 */
//----------------------------------------------------------------------------

#include "rawprintprocdataprov.h"
#include "libbasarcmnutil_print.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//------------------------------------------------------------------------------
//! \throw no-throw
RawPrintProcDataProv::RawPrintProcDataProv ( IPrintDataProvider & rPrintDataProv )
										   : m_rPrintDataProv(rPrintDataProv)
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
void RawPrintProcDataProv::onSendInput(CollStdIo & rCollInput)
{
	m_rPrintDataProv.onSendData(rCollInput);
}

//------------------------------------------------------------------------------
//! \throw no-throw
void RawPrintProcDataProv::onReceiveOutput(CollStdIo & rCollOutput)
{
	while ( !rCollOutput.empty() )
	{
		m_Error += rCollOutput.front();
		rCollOutput.pop();
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
void RawPrintProcDataProv::onReceiveError(CollStdIo & rCollError)
{
	while ( !rCollError.empty() )
	{
		m_Error += rCollError.front();
		rCollError.pop();
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return child's output
const VarString & RawPrintProcDataProv::getOutput() const
{
	return m_Output;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return child's error
const VarString & RawPrintProcDataProv::getError() const
{
	return m_Error;
}


//----------------------------------------------------------------------------
}	//cmnutil
}	//basar

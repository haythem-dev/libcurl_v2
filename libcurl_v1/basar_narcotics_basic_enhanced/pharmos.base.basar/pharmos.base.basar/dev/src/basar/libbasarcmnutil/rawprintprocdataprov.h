//----------------------------------------------------------------------------
/*! \file
 *  \brief  process data provider for raw print
 *  \author Michael Eichenlaub
 *  \date   30.06.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_RAWPRINTPROCDATAPROV_H
#define GUARD_LIBBASARCMNUTIL_RAWPRINTPROCDATAPROV_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_pipedprocess.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
class IPrintDataProvider;

//----------------------------------------------------------------------------
//! \brief process data provider for raw print
//! \n final class
class RawPrintProcDataProv : public IProcessDataProvider
{
public:
	//! constructor			\n no-throw
							RawPrintProcDataProv	(IPrintDataProvider & rPrintDataProv	//!< provider for print data
													);

	//!    callback method: let caller send standard input data				\n no-throw
	virtual void			onSendInput				(CollStdIo & rCollInput		//!< collection for standard input data to piped process
													);
	//!    callback method: let caller receive standard output data			\n no-throw
	virtual void			onReceiveOutput			(CollStdIo & rCollOutput	//!< collection for standard output data from piped process
													);
	//!    callback method: let caller receive standard error data			\n no-throw
	virtual void			onReceiveError			(CollStdIo & rCollError		//!< collection for standard error data from piped process
													);

	const VarString &		getOutput				() const;		//!< get reference to child output
	const VarString &		getError				() const;		//!< get reference to child error

private:

							RawPrintProcDataProv	();                           					//!< forbidden
							RawPrintProcDataProv	(const RawPrintProcDataProv & rCopy);			//!< forbidden
	RawPrintProcDataProv &  operator =				(const RawPrintProcDataProv & rCopy);			//!< forbidden

	VarString				m_Output;			//!< child's output
	VarString				m_Error;			//!< child's error

	IPrintDataProvider &	m_rPrintDataProv;	//!< provider for print data
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

//----------------------------------------------------------------------------
/*! \file
 *  \brief  application parameter
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//---------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "parameter.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
#define BRANCHNO_OPTION		"-branchno"	//!< branchNo commandline option

//----------------------------------------------------------------------------
// Singleton
//----------------------------------------------------------------------------

//#ifdef __sun	// Solaris
//using basar::cmnutil::Singleton;
//! \brief explicit template instantiation for singleton 
//template <> Singleton<ParameterImpl>::InstancePtr Singleton<ParameterImpl>::m_Instance = Singleton<ParameterImpl>::InstancePtr();
//#endif

namespace basar {
	namespace cmnutil {
		template <> typename Singleton<ParameterImpl>::InstancePtr& Singleton<ParameterImpl>::inst()
		{
			static InstancePtr s_inst; 
			return s_inst;
		}
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ParameterImpl::ParameterImpl()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ParameterImpl::init(basar::Int32 argC, char* argV[])
{
	evalCmdLine(argC, argV);
}

//------------------------------------------------------------------------------
//! \throw rethrow basar::cmnutil::CmdLineTokenizer exceptions
//!
void ParameterImpl::evalCmdLine(basar::Int32 argC, char* argV[])
{
	basar::cmnutil::CmdLineTokenizer cmdline;
	basar::VarString branchno;

	try
	{
		cmdline.setCmdLine(argC, argV);
		cmdline.getValOfParam(BRANCHNO_OPTION, branchno);

		// convert branchno to int
		m_BranchNo = static_cast<basar::Int16>(branchno.stoi());
		m_ProcessName = cmdline.getProgName();

		if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) 
		{
			basar::VarString msg;
			msg.format("usage: %s %s <branchno>",
				        argV[0],
						BRANCHNO_OPTION);
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
	}
	catch (...)
	{
		if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::FATAL_LOG_LEVEL)) 
		{
			basar::VarString msg;
			msg.format("usage: %s %s <branchno>",
				        argV[0],
						BRANCHNO_OPTION);
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::FATAL_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
		throw;
	}
}

//------------------------------------------------------------------------------
//! \return database server
//! \throw  no-throw
const basar::VarString ParameterImpl::getProcessName() const
{
	return m_ProcessName;
}

//------------------------------------------------------------------------------
//! \return branch no
//! \throw  no-throw
basar::Int16 ParameterImpl::getBranchNo() const
{
	return m_BranchNo;
}

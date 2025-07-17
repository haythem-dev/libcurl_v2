//---------------------------------------------------------------------------//
/*! \file unloadforbiappl.cpp
 *  \brief definition if application controller for unloadforbi
 *  \author Dirk Kapusta
 *  \date 30.10.2008
 *  \version 00.00.00.01 first version
 */
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narctransferappl.h"
#include "loggerpool.h"
#include "parameter.h"

//---------------------------------------------------------------------------//
// Singleton
//---------------------------------------------------------------------------//

namespace basar {
	namespace cmnutil {
		using narcotics::NarcTransferApplBase;
		template <> typename Singleton<NarcTransferApplBase>::InstancePtr& Singleton<NarcTransferApplBase>::inst()
		{
			static InstancePtr s_inst; 
			return s_inst;
		}
	}
}

//---------------------------------------------------------------------------//

extern "C"
{
  void handleSIGINT(int sig)
  {
	  narcotics::NarcTransferAppl::getInstance().handleSIGINT(sig);
  }

  void handleSIGTERM(int sig)
  {
    narcotics::NarcTransferAppl::getInstance().handleSIGTERM(sig);
  }
} // extern "C"


//---------------------------------------------------------------------------//

namespace narcotics {

//---------------------------------------------------------------------------//
/*!	\throw  no-throw */
//---------------------------------------------------------------------------//
	NarcTransferApplBase::NarcTransferApplBase()
{
}

//---------------------------------------------------------------------------//
/*!	\throw  no-throw */
//---------------------------------------------------------------------------//
NarcTransferApplBase::~NarcTransferApplBase()
{
}

//---------------------------------------------------------------------------//
/*!	\throw  no-throw  
	\retval true if initialization was ok */
//---------------------------------------------------------------------------//
bool NarcTransferApplBase::init()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "NarcTransferApplBase::init()" );
	// login to TESTuseradm in debug
#ifdef _DEBUG
	basar::login::Manager::getInstance().setConnectionConfig("zdev21_tcp", "zdev21_tcp", "zpcua1", "zpcua2");
#endif
	// connection to uadm
	basar::I18nString applName("NARCOTICS"); //pwd Trans123
	if (true != basar::login::Manager::getInstance().nonInteractiveLogin(applName, "narc.transfer", "};I4Yy`;", 
		Parameter::getInstance().getBranchNo()))
	{
		BLOG_ERROR(LoggerPool::loggernarctransfer, "Error: login failed for user <narc.transfer>!");
		return false;
	}

	m_Connection.connect();
	return (m_Connection.getCurrentConnection().isOpen());
}

//---------------------------------------------------------------------------//
/*!	\throw  no-throw  
	\retval struct containing enum and message */
//---------------------------------------------------------------------------//
basar::appl::EventReturnStruct NarcTransferApplBase::run(basar::appl::IEventSource& source,
						 		basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc)
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "NarcTransferApplBase::run()");

	basar::appl::EventReturnStruct ret;

	signal(SIGTERM, ::handleSIGTERM);
	signal(SIGINT,  ::handleSIGINT); // interrupt CTRL+C

	m_ExitGracefull = false;

	m_Interval = -1;


	if (init() == false)
	{
		ret.ret = basar::appl::HANDLER_ERROR;
		ret.message = "Error at initializing NarcTransferApplBase";
		return ret;
	}

	while(!m_ExitGracefull)
	{
		// run use case
		ret = m_TransferUC.run(source, yitFromEvSrc);


		if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::INFO_LOG_LEVEL))
		{
			basar::VarString msg = "NarcTransfer - going to sleep";
				
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
// \!todo: make configurable
		#ifdef WIN32
			Sleep(30 * 1000);
		#else
			sleep(30);
		#endif

		if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::INFO_LOG_LEVEL))
		{
			basar::VarString msg = "NarcTransfer - woke up";
				
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
	}
	
	shutdown();

	return ret;
}

//---------------------------------------------------------------------------//
/*!	\throw  no-throw */
//---------------------------------------------------------------------------//
void NarcTransferApplBase::shutdown()
{
	
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer,"NarcTransferApplBase::shutdown()");
	m_Connection.disconnect();
}

void NarcTransferApplBase::handleSIGINT(int /*sig*/)
{
  BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "NarcTransferApplBase::handleSIGINT()");
  	if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
			basar::VarString msg = "NarcTransferApplBase got SIGINT - exiting gracefull on WIN32";
					
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
#ifdef WIN32
	m_ExitGracefull = true;
#endif
}

//-------------------------------------------------------------------------------------------------//
void NarcTransferApplBase::handleSIGTERM(int /*sig*/)
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "NarcTransferApplBase::handleSIGTERM()");
	if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
			basar::VarString msg = "NarcTransferApplBase got SIGTERM - exiting gracefull";
					
			LoggerPool::loggernarctransfer.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	m_ExitGracefull = true;
}

} // namespace narcotics

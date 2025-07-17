//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"

#include "narcoticsalertingapp.h"
#include "../transfer2bfarm/loggerpool.h"
#include "../transfer2bfarm/iconfiguration.h"
#include "../transfer2bfarm/configuration.h"
#include "../transfer2bfarm/crosschecksvc.h"
#include "../transfer2bfarm/crosschecksvcfactory.h"
#include "../transfer2bfarm/crosscheckcontributionvouchersuc.h"

//---------------------------------------------------------------------------------------------------------//
// singleton
//---------------------------------------------------------------------------------------------------------//

namespace basar {
namespace cmnutil {

template <> typename Singleton<narcotics::NarcoticsAlertingApplBase>::InstancePtr& Singleton<narcotics::NarcoticsAlertingApplBase>::inst()
{
	static InstancePtr s_inst; 
	return s_inst;
}

} // namespace cmnutil
} // namespace basar

//---------------------------------------------------------------------------------------------------------//

namespace narcotics {

//---------------------------------------------------------------------------------------------------------//
	void NarcoticsAlertingApplBase::closeConnections()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::closeConnections()" );

		m_firstDBConnection.disconnect();
	}

//---------------------------------------------------------------------------------------------------------//
	void NarcoticsAlertingApplBase::establishConnections()
	{

		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::establishConnections()" );

		m_firstDBConnection.initConnection(m_configuration->getFirstDBInfo());

		if(!m_firstDBConnection.isOpen())
		{
			m_firstDBConnection.connect();	
		}

	}

//---------------------------------------------------------------------------------------------------------//	
	void NarcoticsAlertingApplBase::initLoginManager()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::initLoginManager()" );

		basar::I18nString applName("NARCOTICS"); //pwd Trans123
		basar::Int16 dummyBranch = 6; //! \note branch is not needed, just for login to UADM

		if (basar::login::Manager::getInstance().nonInteractiveLogin(applName, "narc.transfer", "};I4Yy`;", dummyBranch) == false)
		{
			throw basar::Exception(	basar::ExceptInfo("initLoginManager", "login failed for narc.transfer", __FILE__, __LINE__), "", "" );  
		}

	}

//---------------------------------------------------------------------------------------------------------//	
	void NarcoticsAlertingApplBase::init(const basar::VarString contributionvoucherdir, basar::Int32 startDate, basar::Int32 endDate)
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::init()" );

		try
		{
			m_Crosscheck = true;

			initLoginManager();
			m_configuration->init(); // read UADM parameter
			if (false == contributionvoucherdir.empty()) // overwrite with passed (optional) arguments
			{
				m_configuration->setWorkingDirectory(contributionvoucherdir);
			}
			establishConnections();	

			m_serviceConfig.firstDBConnection = m_firstDBConnection;
			m_serviceConfig.transfer = false; // no transfer and cleanup, only checking and alerting
			m_serviceConfig.backupDir = m_configuration->getSaveDirectory();
			m_serviceConfig.workingDir = m_configuration->getWorkingDirectory();
			m_serviceConfig.vouchersTooMuchDir = m_configuration->getVouchersTooMuchDirectory();
			m_serviceConfig.archivingDir = m_configuration->getArchivingDirectory();
			m_serviceConfig.archivingMetaDataFileName = m_configuration->getArchivingMetaDataFileName();

			//should be injected but too much overhead for simple program
			//config passed as copy -> not so big -> no trouble
			services::CrossCheckSVCFactory crossCheckSVCFactory;
			boost::shared_ptr<services::CrossCheckSVC> crossCheckSVC = crossCheckSVCFactory.createCrossCheckSVC(m_serviceConfig);
			if (startDate && endDate)
			{
				crossCheckSVC.get()->SetCheckPeriod(startDate, endDate);
			}

            //boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> folderRepo(new narcotics::bgafolder::BGAFolderRepo());

			m_crossCheckUC = boost::shared_ptr<usecase::UseCaseController>(new usecase::CrossCheckContributionVouchersUC(crossCheckSVC));
		}
		catch (basar::Exception& ex)
		{
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(),ex.what() );
			throw ex;
		}
	}

//---------------------------------------------------------------------------------------------------------//
	basar::appl::EventReturnStruct NarcoticsAlertingApplBase::run()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "basar::appl::EventReturnStruct NarcoticsAlertingApplBase::run()" );

		try
		{
			if (m_Crosscheck == true)
			{
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "beginning crosschecking...");
				m_crossCheckUC->run();
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "finished crosschecking...");
			}
		}
		catch(basar::Exception& ex)
		{			
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(),ex.what().c_str() );
			throw ex;
		}

		return basar::appl::EventReturnStruct();
	}

//---------------------------------------------------------------------------------------------------------//
	void NarcoticsAlertingApplBase::shutdown()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::shutdown()" );

		try
		{
            closeConnections();
		}
		catch(basar::Exception& ex)
		{
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(),ex.what() );
			throw ex;
		}
	}

//---------------------------------------------------------------------------------------------------------//
	void NarcoticsAlertingApplBase::clear()
	{
	}

//---------------------------------------------------------------------------------------------------------//
boost::shared_ptr<narcotics::configuration::IConfiguration> NarcoticsAlertingApplBase::getConfiguration()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::getConfiguration()" );
	return m_configuration;
}

//---------------------------------------------------------------------------------------------------------//
NarcoticsAlertingApplBase::NarcoticsAlertingApplBase()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "NarcoticsAlertingApplBase::NarcoticsAlertingApplBase()" );
	//extract to servicelocator
	m_configuration = boost::shared_ptr<configuration::IConfiguration>(new configuration::Configuration() );

}

//---------------------------------------------------------------------------------------------------------//
NarcoticsAlertingApplBase::~NarcoticsAlertingApplBase(void)
{
}

} //namespace narcotics

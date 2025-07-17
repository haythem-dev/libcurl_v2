//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"

#include "transferbfarmappl.h"
#include "loggerpool.h"
#include "iconfiguration.h"
#include "configuration.h"
#include "crosschecksvc.h"
#include "crosschecksvcfactory.h"
#include "crosscheckcontributionvouchersuc.h"
#include "transfertobfarmuc.h"
#include "archivevouchersuc.h"
#include "coldmetadatawriter.h"
#include "metadatawriterptr.h"
#include "archivesvc.h"
#include "archivesvcfactory.h"

//---------------------------------------------------------------------------------------------------------//
// singleton
//---------------------------------------------------------------------------------------------------------//

namespace basar {
namespace cmnutil {

template <> typename Singleton<narcotics::TransferBfarmApplBase>::InstancePtr& Singleton<narcotics::TransferBfarmApplBase>::inst()
{
	static InstancePtr s_inst; 
	return s_inst;
}

} // namespace cmnutil
} // namespace basar

//---------------------------------------------------------------------------------------------------------//

namespace narcotics {

//---------------------------------------------------------------------------------------------------------//
	void TransferBfarmApplBase::closeConnections()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::closeConnections()" );

		m_firstDBConnection.disconnect();
	}

//---------------------------------------------------------------------------------------------------------//
	void TransferBfarmApplBase::establishConnections()
	{

		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::establishConnections()" );

		m_firstDBConnection.initConnection(m_configuration->getFirstDBInfo());

		if(!m_firstDBConnection.isOpen())
		{
			m_firstDBConnection.connect();	
		}

	}

//---------------------------------------------------------------------------------------------------------//	
	void TransferBfarmApplBase::initLoginManager()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::initLoginManager()" );

		basar::I18nString applName("NARCOTICS"); //pwd Trans123
		basar::Int16 dummyBranch = 6; //! \note branch is not needed, just for login to UADM

		if (basar::login::Manager::getInstance().nonInteractiveLogin(applName, "narc.transfer", "};I4Yy`;", dummyBranch) == false)
		{
			throw basar::Exception(	basar::ExceptInfo("initLoginManager", "login failed for narc.transfer", __FILE__, __LINE__), "", "" );  
		}

	}

//---------------------------------------------------------------------------------------------------------//	
	void TransferBfarmApplBase::init(const bool crosscheck, const bool transfer, const bool archive, const basar::VarString contributionvoucherdir)
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::init()" );

		try
		{
			m_Crosscheck = crosscheck;
			m_Transfer = transfer;
			m_Archive = archive;

			initLoginManager();
			m_configuration->init(); // read UADM parameter
			if (false == contributionvoucherdir.empty()) // overwrite with passed (optional) arguments
			{
				m_configuration->setWorkingDirectory(contributionvoucherdir);
			}
			establishConnections();	

			m_serviceConfig.firstDBConnection = m_firstDBConnection;
			m_serviceConfig.transfer = true; // transfer and cleanup
			m_serviceConfig.backupDir = m_configuration->getSaveDirectory();
			m_serviceConfig.workingDir = m_configuration->getWorkingDirectory();
			m_serviceConfig.vouchersTooMuchDir = m_configuration->getVouchersTooMuchDirectory();
			m_serviceConfig.archivingDir = m_configuration->getArchivingDirectory();
			m_serviceConfig.archivingMetaDataFileName = m_configuration->getArchivingMetaDataFileName();
			m_serviceConfig.archivingKofaxPath = m_configuration->getArchivingKofaxPath();
			m_serviceConfig.BFARM_User = m_configuration->getBfarmUser();
			m_serviceConfig.BFARM_IP = m_configuration->getBfarmIP();

			//should be injected but too much overhead for simple program
			//config passed as copy -> not so big -> no trouble
			services::CrossCheckSVCFactory crossCheckSVCFactory;
			boost::shared_ptr<services::CrossCheckSVC> crossCheckSVC = crossCheckSVCFactory.createCrossCheckSVC(m_serviceConfig);
			
            boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> folderRepo(new narcotics::bgafolder::BGAFolderRepo());
			boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> branchRepo(new narcotics::bgabranch::BGABranchRepo(m_firstDBConnection));


			utils::MetaDataWriterPtr metaDataWriter(new utils::ColdMetaDataWriter());

			services::ArchiveSVCFactory archiveSVCFactory;

			boost::shared_ptr<narcotics::services::ArchiveSVC> archiveSVC = archiveSVCFactory.createSVC(m_serviceConfig);

			m_crossCheckUC = boost::shared_ptr<usecase::UseCaseController>(new usecase::CrossCheckContributionVouchersUC(crossCheckSVC));

			m_transferToBfarmUC = boost::shared_ptr<usecase::UseCaseController>(new usecase::TransferToBfarmUC(folderRepo, branchRepo, m_serviceConfig));

			m_archiveUC = boost::shared_ptr<usecase::UseCaseController>(new usecase::ArchiveVouchersUC(archiveSVC, folderRepo));
		}
		catch (basar::Exception& ex)
		{
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(),ex.what() );
			throw ex;
		}
	}

//---------------------------------------------------------------------------------------------------------//
	basar::appl::EventReturnStruct TransferBfarmApplBase::run()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "basar::appl::EventReturnStruct TransferBfarmApplBase::run()" );

		try
		{
			if (m_Crosscheck == true)
			{
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "beginning crosschecking...");
				m_crossCheckUC->run();
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "finished crosschecking...");
			}

			if (m_Archive == true)
			{
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "start archiving...");
				m_archiveUC->run();
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "archiving finished...");
			}

			if (m_Transfer == true)
			{
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "starting transfer...");
				m_transferToBfarmUC->run();
				BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), "transfer finished...");
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
	void TransferBfarmApplBase::shutdown()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::shutdown()" );

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
	void TransferBfarmApplBase::clear()
	{
	}

//---------------------------------------------------------------------------------------------------------//
boost::shared_ptr<narcotics::configuration::IConfiguration> TransferBfarmApplBase::getConfiguration()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::getConfiguration()" );
	return m_configuration;
}

//---------------------------------------------------------------------------------------------------------//
TransferBfarmApplBase::TransferBfarmApplBase()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "TransferBfarmApplBase::TransferBfarmApplBase()" );
	//extract to servicelocator
	m_configuration = boost::shared_ptr<configuration::IConfiguration>(new configuration::Configuration() );

}

//---------------------------------------------------------------------------------------------------------//
TransferBfarmApplBase::~TransferBfarmApplBase(void)
{
}

} //namespace narcotics

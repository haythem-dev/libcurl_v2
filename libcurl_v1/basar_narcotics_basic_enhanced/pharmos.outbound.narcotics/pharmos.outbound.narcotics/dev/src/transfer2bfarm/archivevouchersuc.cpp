//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"

#include "archivevouchersuc.h"
#include "transferbfarmappl.h"
#include "iconfiguration.h"
#include "bgafolderrepo.h"
#include "bgafolder.h"
#include "archivesvc.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace usecase {

//------------------------------------------------------------------------------
basar::appl::EventReturnType ArchiveVouchersUC::run()
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "ArchiveVouchersUC::run()");

	boost::shared_ptr<configuration::IConfiguration> config = getManager().getConfiguration();

	basar::VarString msg;
	msg.format("archive vouchers in %s", config->getWorkingDirectory().c_str());
	BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), msg);

	m_folderRepo->includeDeliveryNoteCopies(true);
	std::map<basar::VarString, bgafolder::BGAFolderPtr> folders = m_folderRepo->getAllBGAFoldersFromDir(config->getWorkingDirectory());
	std::map<basar::VarString, bgafolder::BGAFolderPtr>::iterator folderIt = folders.begin();
	while(folderIt != folders.end())
	{
		m_archiveSVC->archiveBGAFolder(folderIt->second);
		folderIt->second->unloadFiles();
		folderIt++;
	}

	m_archiveSVC->shutdown();

	return basar::appl::EventReturnStruct();
}
	
//------------------------------------------------------------------------------
ArchiveVouchersUC::ArchiveVouchersUC(boost::shared_ptr<narcotics::services::ArchiveSVC> archiveSVC,
									boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo) :
									m_folderRepo(bgaFolderRepo),
									m_archiveSVC(archiveSVC)
{
}

//------------------------------------------------------------------------------
ArchiveVouchersUC::~ArchiveVouchersUC()
{
}

//------------------------------------------------------------------------------

} //namespace usecase
} //namespace narcotics

//------------------------------------------------------------------------------

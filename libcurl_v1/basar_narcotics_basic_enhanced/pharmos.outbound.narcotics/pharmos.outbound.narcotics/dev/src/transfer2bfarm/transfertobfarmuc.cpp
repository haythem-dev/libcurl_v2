//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

#include "commonheader.h"
#include "transferbfarmappl.h"
#include "transfertobfarmuc.h"
#include "loggerpool.h"
#include "bgafolderptr.h"
#include "bgafolder.h"
#include "bgafolderrepo.h"
#include "bgabranchrepo.h"
#include "bgabranchptr.h"
#include "bgabranch.h"
#include "extransfer.h"
#include "numberassignmentdm/numberassignmentdm.h"
#include "numberassignmentdm/numberassignmentdm_definitions.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace usecase {

//------------------------------------------------------------------------------

TransferToBfarmUC::TransferToBfarmUC(
	boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> folderRepo,
	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> branchRepo,
	narcotics::services::ServiceConfiguration config) :
	m_folderRepo(folderRepo),
	m_branchRepo(branchRepo),
	m_config(config)
{
}

//------------------------------------------------------------------------------

TransferToBfarmUC::~TransferToBfarmUC()
{
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::run()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "TransferToBfarmUC::run()" );

	renameDeliveryNoteCopyFolders();
	zipFolders();
	backupZippedFiles();
	sendZippedFiles();
	deleteZippedFiles();

	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::renameDeliveryNoteCopyFolders()
{
	m_folderRepo->includeDeliveryNoteCopies(true);
	std::map<basar::VarString, bgafolder::BGAFolderPtr> folders = m_folderRepo->getAllBGAFoldersFromDir(m_config.workingDir);

	std::map<basar::VarString, bgafolder::BGAFolderPtr>::iterator folderIt = folders.begin();
	while (folderIt != folders.end())
	{
		if (folderIt->second->getFolderType() == bgafolder::BGAFolder::DELIVERYNOTECOPY)
		{
			// rename to lfdnr.
			m_branchRepo->setBGABranchForBGAFolder(folderIt->second);
			basar::Int16 branchno = folderIt->second->getBGABranch()->getBranchno();
			domMod::NumberAssignmentDM numberAssignmentDM;
			numberAssignmentDM.init(m_config.firstDBConnection.getCurrentConnection(), branchno);

			basar::I18nString searchParam;
			searchParam.format("%s=%s;%s=%s;",
				lit::pdm_numberassignment::NUMBERTYPE.getName().c_str(), pdm_numberassignment::DELIVERYNOTECOPYFOLDERNO,
				lit::pdm_numberassignment::TABLENAME.getName().c_str(), pdm_numberassignment::NARCNUMBER);
			basar::db::aspect::AccessorPropertyTable_YIterator yit =
				basar::db::aspect::Manager::getInstance().createAccessorYIterator((searchParam));

			basar::Int32 num = numberAssignmentDM.findAndIncrementNoForContext(yit);
			basar::VarString newCounter;
			newCounter.format("%03d", num);
			basar::VarString dirName = folderIt->second->absolutePath();
			dirName.replace(dirName.length() - 3, 3, newCounter);
			folderIt->second->renameTo(dirName);
		}
		folderIt++;
	}
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::zipFolders()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "TransferToBfarmUC::zipFolders()" );

	m_folderRepo->includeDeliveryNoteCopies(true);
	std::map<basar::VarString, bgafolder::BGAFolderPtr> folders = m_folderRepo->getAllBGAFoldersFromDir(m_config.workingDir);
	std::map<basar::VarString, bgafolder::BGAFolderPtr>::iterator folderIt = folders.begin();

	while(folderIt != folders.end())
	{
		basar::VarString zipCmd;
		zipCmd.format("cd %s && zip -r %s.zip %s > /dev/null",
			m_config.workingDir.c_str(),
			folderIt->second->dirName().c_str(),
			folderIt->second->dirName().c_str());

		BLOG_INFO( LoggerPool::getLoggerTransferBfarm(), zipCmd );
		int ret = system(zipCmd.c_str());
		if (ret != 0)
		{
			basar::VarString msg = "cannot zip directory ";
			msg.append(folderIt->second->absolutePath());
			BLOG_ERROR( LoggerPool::getLoggerTransferBfarm(), msg );
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::zipFolders()",msg,__FILE__,__LINE__));
		}

		m_zippedFiles.push_back(folderIt->second->absolutePath() + ".zip");

		basar::VarString rmCmd;
		rmCmd.format("rm -rf %s",
			folderIt->second->absolutePath().c_str());

		BLOG_INFO( LoggerPool::getLoggerTransferBfarm(), rmCmd );
		ret = system(rmCmd.c_str());
		if (ret != 0)
		{
			basar::VarString msg = "cannot remove directory ";
			msg.append(folderIt->second->absolutePath());
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), msg);
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::zipFolders()",msg,__FILE__,__LINE__));
		}

		folderIt++;
	}
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::backupZippedFiles()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "TransferToBfarmUC::backupZippedFiles()" );

	basar::VarString msg;
	QDir backupDir(QString::fromLocal8Bit(m_config.backupDir.c_str()));
	if (false == backupDir.exists())
	{
		if(backupDir.mkpath(backupDir.absolutePath()) == false)
		{
			msg = "cannot create save directory ";
			msg.append(m_config.backupDir);
			BLOG_ERROR( LoggerPool::getLoggerTransferBfarm(), msg );
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::backupZippedFiles()",msg,__FILE__,__LINE__));
		}
	}

	basar::VarString timestamp;
	basar::DateTime ts = basar::DateTime::getCurrent();
	timestamp.format(".%d.%d%d%d", ts.getDate(), ts.getHour(), ts.getMinute(), ts.getSec());

	std::list<basar::VarString>::iterator it;
	for (it = m_zippedFiles.begin(); it != m_zippedFiles.end(); ++it)
	{
		QFileInfo fileInfo(QString::fromLocal8Bit((*it).c_str()));
		QString targetPath = backupDir.absolutePath() + QDir::separator() + fileInfo.fileName() + QString::fromLocal8Bit(timestamp.c_str());

		msg.format("copy %s to %s", fileInfo.absoluteFilePath().toStdString().c_str(), targetPath.toStdString().c_str());
		BLOG_INFO( LoggerPool::getLoggerTransferBfarm(), msg );
		bool ret = QFile::copy(fileInfo.absoluteFilePath(), targetPath);
		if (false == ret)
		{
			msg.format("cannot copy %s to %s", fileInfo.absoluteFilePath().toStdString().c_str(), targetPath.toStdString().c_str());
			BLOG_ERROR( LoggerPool::getLoggerTransferBfarm(), msg );
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::backupZippedFiles()",msg,__FILE__,__LINE__));
		}
	}
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::sendZippedFiles()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "TransferToBfarmUC::sendZippedFiles()" );

	std::list<basar::VarString>::iterator it;

	for (it = m_zippedFiles.begin(); it != m_zippedFiles.end(); ++it)
	{
		basar::VarString ftpCmd;
		ftpCmd.format("LD_LIBRARY_PATH=/usr/lib; echo \"put %s\" | sftp %s@%s",
			(*it).c_str(),
			m_config.BFARM_User.c_str(),
			m_config.BFARM_IP.c_str());
		BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), ftpCmd);
		int ret = system(ftpCmd.c_str());
		if (ret != 0)
		{
			basar::VarString msg = "cannot send zip file ";
			msg.append(*it);
			BLOG_ERROR( LoggerPool::getLoggerTransferBfarm(), msg );
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::sendZippedFiles()",msg,__FILE__,__LINE__));
		}
	}
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

basar::appl::EventReturnStruct TransferToBfarmUC::deleteZippedFiles()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "TransferToBfarmUC::deleteZippedFiles()" );

	basar::VarString msg;

	std::list<basar::VarString>::iterator it;

	for (it = m_zippedFiles.begin(); it != m_zippedFiles.end(); ++it)
	{
		msg.format("remove zip file %s", (*it).c_str());
		BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), msg);
		bool ret = QFile::remove(QString::fromLocal8Bit((*it).c_str()));
		if (false == ret)
		{
			basar::VarString mesg = "cannot delete zip file ";
			mesg.append(*it);
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), mesg);
			throw narcotics::exceptions::ExTransfer(basar::ExceptInfo("TransferToBfarmUC::deleteZippedFiles()",mesg,__FILE__,__LINE__));
		}
	}
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

} //namespace usecase
} //namespace narcotics

//------------------------------------------------------------------------------

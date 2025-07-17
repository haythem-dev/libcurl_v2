//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"

#include "exservice.h"
#include "bgafolder.h"
#include "bgabranch.h"
#include "archivesvc.h"
#include "coldmetadatawriter.h"
#include "serviceconfiguration.h"
#include "coldmetadata.h"
#include "contributionvoucherfile.h"
#include "contributionvoucherfileptr.h"
#include "bgafolderptr.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
void ArchiveSVC::archiveBGAFolder(bgafolder::BGAFolderPtr folder)
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "ArchiveSVC::archiveBGAFolder");

	m_bgaBranchRepo->setBGABranchForBGAFolder(folder);
	//reload files because might have changed during cleanup
	folder->reload();

	basar::VarString destFolderPath = m_config.archivingDir;	

	basar::VarString lastChar = destFolderPath.substr(destFolderPath.length()-1,1);

	if(lastChar != "\\" && lastChar != "/")
	{
		destFolderPath.append(QString(QDir::separator()).toStdString());
	}

	basar::VarString branchDir;
	branchDir.format("vz_%02d%c", folder->getBGABranch()->getBranchno(), QDir::separator().toLatin1());
	destFolderPath.append(branchDir);
	
	bgafolder::BGAFolderPtr archiveFolder = folder->copyTo( destFolderPath );

	m_bgaBranchRepo->setBGABranchForBGAFolder(archiveFolder);

	std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr> files = folder->getVoucherFiles();
	
	std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>::iterator fileIter = files.begin();

	narcotics::datecalc::DateInfo dateInfo = m_dateCalc->calculateStartEndDate(	folder->getBGABranch()->getElectronicVoucherStartDate(),
																				folder->getWeeksSinceStartCounter());
	if(dateInfo.getStartDate() == 0 || dateInfo.getEndDate() == 0)
	{
		basar::VarString msg = "could not retrieve timespan for folder:";
		msg.append(folder->absolutePath());
		throw narcotics::exceptions::ExService(basar::ExceptInfo("processFolder(bgafolder::BGAFolderPtr folder)",msg,__FILE__,__LINE__));
	}

	//as vouchers with transactiondate of week before could be in this folder (created saturday afternoon for example) extend the timespan
	//no more than 4 days without work expected
	std::map<basar::Int32, contributionvoucherentry::ContributionVoucherEntryPtr>* dbEntries;
	if (folder->getFolderType() == bgafolder::BGAFolder::DELIVERYNOTECOPY)
	{
		dateInfo.addDaysToStartDate(-100);
		dbEntries = m_contributionvoucherentryrepos->getDeliveryNoteCopyEntriesForBranchAndDate(folder->getBGABranch(),dateInfo);
	}
	else
	{
		dateInfo.addDaysToStartDate(-4);
		dbEntries = m_contributionvoucherentryrepos->getContributionVoucherEntriesForBranchAndDate(folder->getBGABranch(),dateInfo);
	}
	

	archiving::ColdMetaData metaData(archiveFolder->getBGABranch()->getBranchno());

	while(fileIter != files.end())
	{
		std::map<basar::Int32, contributionvoucherentry::ContributionVoucherEntryPtr>::iterator foundDBEntry;
		
		contributionvoucherfile::ContributionVoucherFilePtr file = fileIter->second;
		
		foundDBEntry = dbEntries->find(file->getContributionVoucherNo());

		if(foundDBEntry != dbEntries->end())
		{
			// determine Kofax file path
			basar::VarString pathTemplate = m_config.archivingKofaxPath;
			basar::VarString path;
			path.format(m_config.archivingKofaxPath.c_str(), folder->getBGABranch()->getBranchno());
			path.append(file->fileName().toStdString());
			metaData.addFileEntry(foundDBEntry->second,file,path);
		}

		fileIter++;
	}
	m_MetaDataWriter->write(metaData, archiveFolder);

	delete dbEntries;
}

//------------------------------------------------------------------------------
ArchiveSVC::ArchiveSVC(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
						boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
						boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
						boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
						narcotics::services::ServiceConfiguration config,
						utils::MetaDataWriterPtr metaDataWriter): 
						BfarmService(bgaBranchRepo,voucherRepo,bgaFolderRepo,dateCalc,config), m_MetaDataWriter(metaDataWriter)
{
}

//------------------------------------------------------------------------------
ArchiveSVC::~ArchiveSVC()
{
}

} //namespace services
} //namespace narcotics
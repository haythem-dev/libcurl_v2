//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "crosschecksvc.h"
#include "bgafolder.h"
#include "bgabranch.h"
#include "excrosschecksvc.h"
#include "contributionvoucherfile.h"
#include "contributionvoucherentry.h"
#include "contributionvoucherentryptr.h"
#include "convert.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
void CrossCheckSVC::renumberFiles(bgafolder::BGAFolderPtr folder)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::renumberFiles(bgafolder::BGAFolderPtr folder)" );

	folder->reload();

	std::map<basar::Int32, contributionvoucherfile::ContributionVoucherFilePtr> files = folder->getVoucherFiles();
	std::map<basar::Int32, contributionvoucherfile::ContributionVoucherFilePtr>::iterator fileIt = files.begin();

	basar::Int32 counter=1;

	while(fileIt != files.end())
	{
		if((fileIt->second)->getCounter() != counter)
		{
			(fileIt->second)->setCounter(counter);
		}
		counter++;
		fileIt++;
	}
}

//------------------------------------------------------------------------------
void CrossCheckSVC::checkForSurplusVouchers(bgafolder::BGAFolderPtr folder)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::checkForSurplusVouchers(bgafolder::BGAFolderPtr folder)" );

	m_currentFolder = folder;

	narcotics::datecalc::DateInfo dateInfo;
	if (m_Transfer == true)
	{
		// in transfer mode, calculate the dates
		dateInfo = m_dateCalc->calculateStartEndDate(folder->getBGABranch()->getElectronicVoucherStartDate(),folder->getWeeksSinceStartCounter());
	}
	else
	{
		// in check and alerting mode, take the dates from start parameters
		dateInfo.setStartDate(m_startDate);
		dateInfo.setEndDate(m_endDate);
	}
    
	if(dateInfo.getStartDate() == 0 || dateInfo.getEndDate() == 0)
	{
		basar::VarString msg = "could not retrieve timespan for folder:";
		msg.append(folder->absolutePath());
		
		throw narcotics::exceptions::ExCrossCheckSVC(basar::ExceptInfo("processFolder(bgafolder::BGAFolderPtr folder)",msg,__FILE__,__LINE__));
	}
	if (m_Transfer == true)
	{
		//as vouchers with transactiondate of week before could be in this folder (created saturday afternoon for example) extend the timespan
		//no more than 4 days without work expected
		dateInfo.addDaysToStartDate(-4);
	}
	m_foundDBEntries = m_contributionvoucherentryrepos->getContributionVoucherEntriesForBranchAndDate(folder->getBGABranch(),dateInfo);
	
	std::map<basar::Int32, contributionvoucherfile::ContributionVoucherFilePtr> files = folder->getVoucherFiles();
	std::map<basar::Int32, contributionvoucherfile::ContributionVoucherFilePtr>::iterator fileIt = files.begin();
	while(fileIt != files.end())
	{
		if(!isFileKnownInDB(fileIt->second))
		{	
			QString bDir(QString::fromLocal8Bit(m_config.vouchersTooMuchDir.c_str()));

			moveFileToBackupFolder(fileIt->second,bDir);

			logSurplusVoucher(fileIt->second, dateInfo);

			m_foundSurplusVouchers = true;
		}	
		fileIt++;
	}
	delete m_foundDBEntries;
}

//------------------------------------------------------------------------------
void CrossCheckSVC::logSurplusVoucher(contributionvoucherfile::ContributionVoucherFilePtr file, narcotics::datecalc::DateInfo &dateInfo)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::logSurplusVoucher(contributionvoucherfile::ContributionVoucherFilePtr file, narcotics::datecalc::DateInfo &dateInfo)" );

	basar::Int16 branchno = m_currentFolder->getBGABranch()->getBranchno();
	basar::VarString basarDirName = m_currentFolder->dirName();
	
	QString fileName = file->fileName();

	basar::VarString voucherfileName = fileName.toStdString();

	basar::VarString msg;
	msg.append(utils::Convert::toVarString(branchno) + "; ");
	msg.append(basarDirName + "; ");
	msg.append(voucherfileName + "; ");
	msg.append(utils::Convert::toVarString(dateInfo.getStartDate()) + "; ");
	msg.append(utils::Convert::toVarString(dateInfo.getEndDate()) + "; ");

	BLOG_ERROR( LoggerPool::getLoggerSurplusVoucher(), msg.c_str() );

	msg.format("Contribution voucher %s is over liquid! Not sending to BfArM", voucherfileName.c_str());
	BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), msg);
}

//------------------------------------------------------------------------------
void CrossCheckSVC::logMissingFile(contributionvoucherentry::ContributionVoucherEntryPtr entry, narcotics::datecalc::DateInfo &dateInfo)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::logMissingFile(contributionvoucherentry::ContributionVoucherEntryPtr entry, narcotics::datecalc::DateInfo &dateInfo)" );

	basar::Int32 voucherNo = entry->getVoucherNo();
	basar::VarString voucherNoStr = utils::Convert::toVarString(voucherNo);

	basar::Int16 branchno = m_currentFolder->getBGABranch()->getBranchno();
	
	basar::VarString basarDirName = m_currentFolder->dirName();

	basar::VarString msg;
	msg.append(utils::Convert::toVarString(branchno) + "; ");
	msg.append(basarDirName + "; ");
	msg.append(voucherNoStr + "; ");
	msg.append(utils::Convert::toVarString(dateInfo.getStartDate()) + "; ");
	msg.append(utils::Convert::toVarString(dateInfo.getEndDate()) + "; ");
	BLOG_ERROR( LoggerPool::getLoggerMissingVoucher(), msg.c_str());	

	msg.format("Contribution voucher %d is missing!", voucherNo);
	BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), msg);
}

//------------------------------------------------------------------------------
void CrossCheckSVC::moveFileToBackupFolder(contributionvoucherfile::ContributionVoucherFilePtr file, QString backupFolder)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::moveFileToBackupFolder(contributionvoucherfile::ContributionVoucherFilePtr file, QString backupFolder)" );

	QDir fileDir = file->dir();
	QString dirName = fileDir.dirName();
	QDir backupDir(backupFolder + QDir::separator() + dirName );	

	if(!backupDir.exists())
	{
		QString absPath(backupDir.absolutePath()); 

		if(!backupDir.mkpath(absPath))
		{
			basar::VarString msg = "path creation failed:";
			msg.append(backupDir.absolutePath().toStdString().c_str());
			
			throw narcotics::exceptions::ExCrossCheckSVC(basar::ExceptInfo("moveFileToBackupFolder",msg,__FILE__,__LINE__));
		}
	}
	QDateTime dateTime = QDateTime::currentDateTime();
	QString timeStampedFileName = file->fileName() + "." + dateTime.toString("ddMMyyhhmmss");
	file->moveTo(backupDir.absolutePath() + QDir::separator() + timeStampedFileName );	
}

//------------------------------------------------------------------------------
bool CrossCheckSVC::isFileKnownInDB(contributionvoucherfile::ContributionVoucherFilePtr file)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::isFileKnownInDB(contributionvoucherfile::ContributionVoucherFilePtr file)" );

	std::map<basar::Int32, contributionvoucherentry::ContributionVoucherEntryPtr>::iterator it;
	it=m_foundDBEntries->find(file->getContributionVoucherNo());

	return it!=m_foundDBEntries->end();
}

//------------------------------------------------------------------------------
void CrossCheckSVC::checkForMissingFiles(bgafolder::BGAFolderPtr folder, bgafolder::BGAFolderPtr followingFolder)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::checkForMissingFiles(bgafolder::BGAFolderPtr folder, bgafolder::BGAFolderPtr followingFolder)" );

	m_currentFolder = folder;

	narcotics::datecalc::DateInfo dateInfo;
	if (m_Transfer == true)
	{
		// in transfer mode, calculate the dates
		dateInfo = m_dateCalc->calculateStartEndDate(folder->getBGABranch()->getElectronicVoucherStartDate(),folder->getWeeksSinceStartCounter());
	}
	else
	{
		// in check and alerting mode, take the dates from start parameters
		dateInfo.setStartDate(m_startDate);
		dateInfo.setEndDate(m_endDate);
	}

	if(dateInfo.getStartDate() == 0 || dateInfo.getEndDate() == 0)
	{
		basar::VarString msg = "could not retrieve timespan for folder:";
		msg.append(folder->absolutePath());
		throw narcotics::exceptions::ExCrossCheckSVC(basar::ExceptInfo("checkForMissingFiles",msg,__FILE__,__LINE__));
	}

	m_foundDBEntries = m_contributionvoucherentryrepos->getContributionVoucherEntriesForBranchAndDate(folder->getBGABranch(),dateInfo);

	std::map<basar::Int32, contributionvoucherentry::ContributionVoucherEntryPtr>::iterator dbEntryIt = m_foundDBEntries->begin();

	while(dbEntryIt != m_foundDBEntries->end())
	{
		if(!folder->containsFileWithNumber(dbEntryIt->second->getVoucherNo()))
		{
			if( (followingFolder.get() == NULL ) || (followingFolder.get() != NULL && !followingFolder->containsFileWithNumber(dbEntryIt->second->getVoucherNo() )))
			{
				logMissingFile(dbEntryIt->second, dateInfo);
				m_foundMissingVouchers = true;
			}
		}
		dbEntryIt++;
	}

	delete m_foundDBEntries;
}

//------------------------------------------------------------------------------
narcotics::bgafolder::BGAFolderPtr CrossCheckSVC::getFollowUpFolder(bgafolder::BGAFolderPtr folder,std::map<basar::VarString, bgafolder::BGAFolderPtr>& folders)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::getFollowUpFolder(bgafolder::BGAFolderPtr folder,std::map<QString,bgafolder::BGAFolderPtr>& folders)" );

	basar::VarString searchedFolderName;
	searchedFolderName.format("%07d%03d",
		folder->getBGANo(),
		folder->getWeeksSinceStartCounter() + 1);

	std::map<basar::VarString,bgafolder::BGAFolderPtr>::iterator followingFolderIt = folders.find(searchedFolderName);

	if(followingFolderIt != folders.end())
	{
		return followingFolderIt->second;
	}

	return narcotics::bgafolder::BGAFolderPtr();
}

//------------------------------------------------------------------------------
void CrossCheckSVC::crossCheckFoldersAndCleanUp()
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), "CrossCheckSVC::crossCheckFoldersAndCleanUp()" );

	std::map<basar::VarString, bgafolder::BGAFolderPtr> folders = m_bgaFolderRepo->getAllBGAFoldersFromDir(m_config.workingDir);
	std::map<basar::VarString, bgafolder::BGAFolderPtr>::iterator folderIt;

	bool cleanup = m_Transfer;
	basar::VarString msg;
	msg.format("crosscheck folders in %s", m_config.workingDir.c_str());
	BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), msg);

	folderIt=folders.begin();
	while(folderIt!=folders.end())
	{
		//get branchinfo for folder (bgano, startdate, ...)
		m_bgaBranchRepo->setBGABranchForBGAFolder(folderIt->second);
		
		basar::VarString folderName = (folderIt->second)->dirName();

        msg.format("checking folder %s", folderName.c_str());
		BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), msg);

		if (folderIt->second->getFolderType() == bgafolder::BGAFolder::CONTRIBUTIONVOUCHER)
		{
			if (cleanup == true) folderIt->second->deleteDuplicateVoucherFiles();
			checkForSurplusVouchers(folderIt->second);
			bgafolder::BGAFolderPtr followingFolder = getFollowUpFolder(folderIt->second,folders);
			checkForMissingFiles(folderIt->second,followingFolder);
		}
		if (cleanup == true) renumberFiles(folderIt->second);
		folderIt->second->unloadFiles();
		folderIt++;
	}	

	if(m_foundMissingVouchers || m_foundSurplusVouchers )
	{
		sendMail();		
	}
}

//------------------------------------------------------------------------------
void CrossCheckSVC::SetCheckPeriod(basar::Int32 startDate, basar::Int32 endDate)
{
	m_startDate = startDate;
	m_endDate = endDate;
}

//------------------------------------------------------------------------------
void CrossCheckSVC::sendMail()
{
	//  JSI test purpose only
	BLOG_TRACE(LoggerPool::getLoggerTransferBfarm(), "found voucher differences between folder and DB!");
}

//------------------------------------------------------------------------------
CrossCheckSVC::CrossCheckSVC(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
								boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
								boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
								boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
								ServiceConfiguration config): BfarmService(bgaBranchRepo,voucherRepo,bgaFolderRepo,dateCalc,config),																									
																									m_foundMissingVouchers(false),
																									m_foundSurplusVouchers(false)
																									

{
	m_Transfer = config.transfer;
	if (m_Transfer == false)
	{
		// for check and alerting, do only last day
		basar::Date today;
		basar::Date yesterday;
		yesterday.addDays(-1);
		SetCheckPeriod(yesterday.getDate(), today.getDate());
	}
}

//------------------------------------------------------------------------------
CrossCheckSVC::~CrossCheckSVC()
{
}

} //namespace services
} //namespace narcotics
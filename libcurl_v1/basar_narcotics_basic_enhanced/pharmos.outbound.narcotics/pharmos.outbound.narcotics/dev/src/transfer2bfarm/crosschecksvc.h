#ifndef CROSSCHECKSVC_GUARD
#define CROSSCHECKSVC_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "bfarmservice.h"
#include "bgafolderptr.h"
#include "contributionvoucherfileptr.h"
#include "bgabranchrepo.h"
#include "contributionvoucherentryrepo.h"
#include "bgafolderrepo.h"
#include "startenddatecalculator.h"
#include "serviceconfiguration.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
class CrossCheckSVC : public BfarmService
{
public:	
	//crosschecks folders in an directory which is defined in transfertobfarm.ini (workingdir)
	void crossCheckFoldersAndCleanUp();
	void SetCheckPeriod(basar::Int32 startDate, basar::Int32 endDate);

	CrossCheckSVC(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
					boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
					boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
					boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
					narcotics::services::ServiceConfiguration config
					);

	virtual ~CrossCheckSVC(void);

private:
	void sendMail();

	void renumberFiles(bgafolder::BGAFolderPtr folder);

	//checks for surplus vouchers by comparing every fileentry in a folder with the databse entries(contributionvoucherno). As orders created
	//on a saturday can also be printed on monday (same for public holidays) it it checks timespan of the folder number since start -4 days. 
	//if no corresponding db entry is found the file is moved to folder defined in transferbfarm.ini
	void checkForSurplusVouchers(bgafolder::BGAFolderPtr folder);

	//checks for missing files by searching a corresponding file for each db entry. if not found -> logged to file defined in logging config
	void checkForMissingFiles(bgafolder::BGAFolderPtr folder, bgafolder::BGAFolderPtr followingFolder);

	//gets the follower of an folder 
	narcotics::bgafolder::BGAFolderPtr getFollowUpFolder(bgafolder::BGAFolderPtr folder, std::map<basar::VarString, bgafolder::BGAFolderPtr>& folders);

	bool isFileKnownInDB(contributionvoucherfile::ContributionVoucherFilePtr file);

	void moveFileToBackupFolder(contributionvoucherfile::ContributionVoucherFilePtr file,QString backupFolder);

	void logMissingFile(contributionvoucherentry::ContributionVoucherEntryPtr entry, narcotics::datecalc::DateInfo &info);

	void logSurplusVoucher(contributionvoucherfile::ContributionVoucherFilePtr, narcotics::datecalc::DateInfo &info);
	
	//freeing memory faster than deleting elements???
	std::map<basar::Int32, contributionvoucherentry::ContributionVoucherEntryPtr> *m_foundDBEntries;	

	bgafolder::BGAFolderPtr m_currentFolder;
	bool m_foundMissingVouchers;
	bool m_foundSurplusVouchers;

	bool m_Transfer;
	basar::Int32 m_startDate;
	basar::Int32 m_endDate;
};

} //namespace services
} //namespace narcotics

#endif //CROSSCHECKSVC_GUARD
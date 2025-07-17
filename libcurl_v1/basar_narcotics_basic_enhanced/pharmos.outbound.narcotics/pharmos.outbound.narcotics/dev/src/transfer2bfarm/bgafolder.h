#ifndef BGAFOLDER_GUARD
#define BGAFOLDER_GUARD
//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

#include "bgabranchptr.h"
#include "contributionvoucherfileptr.h"
#include "bgafolderptr.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace bgafolder {

//------------------------------------------------------------------------------
class BGAFolder // : public QDir
{
public:
	enum FolderType
	{
		UNKNOWN 			= -1,
		CONTRIBUTIONVOUCHER = 0,
		DELIVERYNOTECOPY	= 1,
		ARCHIVING			= 2
	};

	std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>& getVoucherFiles();

	basar::Int32 getWeeksSinceStartCounter();	
	basar::Int32 getBGANo();

	// delegates to QDir:
	const basar::VarString absolutePath();
	const basar::VarString dirName();
	const basar::VarString absoluteFilePath(const basar::VarString fileName);
	bool exists();

	BGAFolderPtr copyTo(const basar::VarString& des);
	void         renameTo(const basar::VarString& dir);

	void deleteDuplicateVoucherFiles();

	bool containsFileWithNumber(basar::Int32 voucherNo);

	void reload();

	void unloadFiles();

	void setBGABranch(narcotics::bgabranch::BGABranchPtr bgaBranch);
	narcotics::bgabranch::BGABranchPtr getBGABranch();
	FolderType getFolderType();

	BGAFolder(basar::VarString path);
	virtual ~BGAFolder();

protected:

private:
	bool isValidContributionVoucherFolder();
	bool isValidDeliveryNoteCopyFolder();
	bool isValidArchivingFolder();

	void loadFiles();

	QDir																		m_Dir;
	basar::Int32																m_weekCounter;
	basar::Int32																m_bgaNo;
	bool																		m_filesLoaded;
	bool																		m_loadDeliveryNoteCopies;
	FolderType																	m_FolderType;

	std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>	m_files;
	boost::shared_ptr<narcotics::bgabranch::BGABranch>							m_bgaBranch;
};

//------------------------------------------------------------------------------

} //namespace bgafolder
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //BGAFOLDER_GUARD

//------------------------------------------------------------------------------

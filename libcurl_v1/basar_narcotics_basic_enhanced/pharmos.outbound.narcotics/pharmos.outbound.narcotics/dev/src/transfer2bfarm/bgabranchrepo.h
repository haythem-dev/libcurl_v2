#ifndef BGABRANCHREPO_GUARD
#define BGABRANCHREPO_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "bgabranchptr.h"
#include "bgafolderptr.h"
#include "infxconnection.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace bgabranch {

//------------------------------------------------------------------------------
class BGABranchRepo
{
public:
	BGABranchRepo(narcotics::connection::InfxConnection con1);
	virtual ~BGABranchRepo();

	void shutdown();
	bgafolder::BGAFolderPtr setBGABranchForBGAFolder(bgafolder::BGAFolderPtr folder);

private:
	void createAccessors();
	void shutdownAccessors();

	std::map<basar::Int16,bgabranch::BGABranchPtr> m_foundBranches;

	bgabranch::BGABranchPtr getPartialBranch(bgafolder::BGAFolderPtr folder);
	bgabranch::BGABranchPtr setStartDate(bgabranch::BGABranchPtr branch);
	bgabranch::BGABranchPtr setBGANo(bgabranch::BGABranchPtr branch, basar::Int32 bgaNo);

	narcotics::connection::InfxConnection m_connection1;

	basar::db::aspect::AccessorInstanceRef m_searchAcc;
	basar::db::aspect::AccessorInstanceRef m_bgaBranchAcc1;
};

//------------------------------------------------------------------------------

} //namespace bgabranch
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //BGABRANCHREPO_GUARD

//------------------------------------------------------------------------------

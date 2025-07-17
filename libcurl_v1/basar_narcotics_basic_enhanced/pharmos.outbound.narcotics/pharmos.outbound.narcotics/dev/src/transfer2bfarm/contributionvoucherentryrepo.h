#ifndef CONTRIBUTIONVOUCHERENTRYREPO_GUARD
#define CONTRIBUTIONVOUCHERENTRYREPO_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

#include "contributionvoucherentryptr.h" 
#include "infxconnection.h"
#include "startenddatecalculator.h"
#include "bgabranchptr.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherentry
{

//------------------------------------------------------------------------------
class ContributionVoucherEntryRepo
{
public:
	std::map<basar::Int32, ContributionVoucherEntryPtr>* 
		getContributionVoucherEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates);

	std::map<basar::Int32, ContributionVoucherEntryPtr>* 
		getDeliveryNoteCopyEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates);

	void shutdown();

	ContributionVoucherEntryRepo( narcotics::connection::InfxConnection con1);
	virtual ~ContributionVoucherEntryRepo();

private:
	void createAccessors();
	void shutdownAccessors();

	basar::db::aspect::AccessorInstanceRef m_entriesDB1;
	basar::db::aspect::AccessorInstanceRef m_searchAcc;

	narcotics::connection::InfxConnection m_con1;
};

} //namespace contributionvoucherentry
} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERENTRYREPO_GUARD
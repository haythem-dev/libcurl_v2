#ifndef BFARMSERVICE_GUARD
#define BFARMSERVICE_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

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
class BfarmService
{
public:
	BfarmService(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
					boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
					boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
					boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
					narcotics::services::ServiceConfiguration config);

	virtual ~BfarmService(void);

	virtual void shutdown();

protected:
	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> m_bgaBranchRepo;
	boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> m_contributionvoucherentryrepos;
	boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> m_bgaFolderRepo;

	boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> m_dateCalc;

	ServiceConfiguration m_config;
};

} //namespace services
} //namespace narcotics

#endif //BFARMSERVICE_GUARD
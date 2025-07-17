#ifndef SERVICEFACTORY_GUARD
#define SERVICEFACTORY_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "bgabranchrepo.h"
#include "contributionvoucherentryrepo.h"
#include "bgafolderrepo.h"
#include "startenddatecalculator.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{
//------------------------------------------------------------------------------
	class ServiceConfiguration;

//------------------------------------------------------------------------------
struct CommonComponents
{
	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> branchRepo;
	boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo;
	boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo;
	boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc;
};

//------------------------------------------------------------------------------
class ServiceFactory
{
public:

	ServiceFactory();
	virtual ~ServiceFactory();
protected:
	virtual CommonComponents createCommonComponets(narcotics::services::ServiceConfiguration config);	
};

} //namespace services
} //namespace narcotics

#endif //SERVICEFACTORY_GUARD
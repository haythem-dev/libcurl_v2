//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "servicefactory.h"
#include "serviceconfiguration.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
using contributionvoucherentry::ContributionVoucherEntryRepo;
using bgabranch::BGABranchRepo;
using narcotics::bgafolder::BGAFolderRepo;
using narcotics::datecalc::StartEndDateCalculator;

//------------------------------------------------------------------------------
ServiceFactory::ServiceFactory()
{
}

//------------------------------------------------------------------------------
ServiceFactory::~ServiceFactory()
{
}

//------------------------------------------------------------------------------
CommonComponents ServiceFactory::createCommonComponets(ServiceConfiguration config)
{
	CommonComponents components;

	components.bgaFolderRepo =  boost::shared_ptr<BGAFolderRepo>(new BGAFolderRepo());
	components.branchRepo = boost::shared_ptr<BGABranchRepo>(new BGABranchRepo(config.firstDBConnection));
	components.dateCalc = boost::shared_ptr<StartEndDateCalculator>(new StartEndDateCalculator());
	components.voucherRepo = boost::shared_ptr<ContributionVoucherEntryRepo>(new ContributionVoucherEntryRepo(config.firstDBConnection));

	return components;
}

} //namespace services
} //namespace narcotics
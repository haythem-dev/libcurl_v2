//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "bgabranchrepo.h"
#include "bgafolderrepo.h"
#include "contributionvoucherentryrepo.h"
#include "serviceconfiguration.h"

#include "crosschecksvcfactory.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
boost::shared_ptr<CrossCheckSVC> CrossCheckSVCFactory::createCrossCheckSVC(ServiceConfiguration config)
{
	CommonComponents components = createCommonComponets(config);
	boost::shared_ptr<CrossCheckSVC> service (new CrossCheckSVC(components.branchRepo,components.voucherRepo,components.bgaFolderRepo,components.dateCalc,config));
	return service;
}

//------------------------------------------------------------------------------
CrossCheckSVCFactory::CrossCheckSVCFactory(void)
{
}

//------------------------------------------------------------------------------
CrossCheckSVCFactory::~CrossCheckSVCFactory(void)
{
}

} //namespace services
} //namespace narcotics
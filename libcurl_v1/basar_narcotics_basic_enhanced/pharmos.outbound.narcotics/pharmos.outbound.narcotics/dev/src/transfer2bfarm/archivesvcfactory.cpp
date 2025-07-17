//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "archivesvcfactory.h"
#include "servicefactory.h"
#include "archivesvc.h"
#include "metadatawriterptr.h"
#include "coldmetadatawriter.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//candidate for abstract factory but not here
boost::shared_ptr<ArchiveSVC> ArchiveSVCFactory::createSVC(ServiceConfiguration config)
{
	CommonComponents components = createCommonComponets(config);
	utils::MetaDataWriterPtr metaDataWriter = utils::MetaDataWriterPtr(new utils::ColdMetaDataWriter(config));
	boost::shared_ptr<ArchiveSVC> service (new ArchiveSVC(components.branchRepo,components.voucherRepo,components.bgaFolderRepo,components.dateCalc,config,metaDataWriter));
	return service;
}

//------------------------------------------------------------------------------
ArchiveSVCFactory::ArchiveSVCFactory(void)
{
}

//------------------------------------------------------------------------------
ArchiveSVCFactory::~ArchiveSVCFactory(void)
{
}

} //namespace services
} //namespace narcotics
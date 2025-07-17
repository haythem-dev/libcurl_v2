#ifndef ARCHIVESVCFACTORY_GUARD
#define ARCHIVESVCFACTORY_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "servicefactory.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
class ArchiveSVC;

//------------------------------------------------------------------------------
class ArchiveSVCFactory : public ServiceFactory 
{
public:
	boost::shared_ptr<ArchiveSVC> createSVC(ServiceConfiguration config);
	ArchiveSVCFactory(void);
	virtual ~ArchiveSVCFactory(void);
};

} //namespace services
} //namespace narcotics

#endif //ARCHIVESVCFACTORY_GUARD
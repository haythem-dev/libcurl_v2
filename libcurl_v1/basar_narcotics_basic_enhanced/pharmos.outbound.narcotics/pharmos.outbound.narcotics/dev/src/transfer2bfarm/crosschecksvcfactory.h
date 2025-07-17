#ifndef CROSSCHECKFACTORY_GUARD
#define CROSSCHECKFACTORY_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "crosschecksvc.h"
#include "servicefactory.h"

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
class CrossCheckSVCFactory:ServiceFactory
{
public:

	boost::shared_ptr<CrossCheckSVC> createCrossCheckSVC(ServiceConfiguration config);

	CrossCheckSVCFactory(void);
	virtual ~CrossCheckSVCFactory(void);
};

} //namespace services
} //namespace narcotics

#endif //CROSSCHECKFACTORY_GUARD
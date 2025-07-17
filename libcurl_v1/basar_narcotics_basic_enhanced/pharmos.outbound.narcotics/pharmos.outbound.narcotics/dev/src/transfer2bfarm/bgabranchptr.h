#ifndef BGABRANCHPTR_GUARD
#define BGABRANCHPTR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace bgabranch
{

	class BGABranch;
	typedef boost::shared_ptr< BGABranch > BGABranchPtr;

} //namespace bgabranch
} //namespace narcotics

#endif //BGABRANCHPTR_GUARD
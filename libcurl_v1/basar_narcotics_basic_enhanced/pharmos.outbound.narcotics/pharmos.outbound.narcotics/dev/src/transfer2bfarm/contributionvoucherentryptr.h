#ifndef VOUCHERENTRYPTR_GUARD
#define VOUCHERENTRYPTR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherentry
{
	class ContributionVoucherEntry;
	typedef boost::shared_ptr< ContributionVoucherEntry > ContributionVoucherEntryPtr;

} //namespace contributionvoucherentry
} //namespace narcotics

#endif //VOUCHERENTRYPTR_GUARD
#ifndef VOUCHERFILEPTR_GUARD
#define VOUCHERFILEPTR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherfile
{
	class ContributionVoucherFile;
	typedef boost::shared_ptr< ContributionVoucherFile > ContributionVoucherFilePtr;

} //namespace contributionvoucherfile
} //namespace narcotics

#endif //VOUCHERFILEPTR_GUARD
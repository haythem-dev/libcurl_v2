//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "exvoucherentryrepo.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
ExVoucherEntryRepo::ExVoucherEntryRepo(const basar::ExceptInfo& sInfo): basar::cmnutil::BasarRuntimeException( sInfo )
{
}

//--------------------------------------------------------------------------------------------------//
ExVoucherEntryRepo::~ExVoucherEntryRepo(void)
{
}

} //namespace exceptions
} //namespace narcotics
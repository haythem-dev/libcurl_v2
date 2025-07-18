//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "exbgabranchrepo.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
ExBGABranchRepo::ExBGABranchRepo(const basar::ExceptInfo& sInfo): basar::cmnutil::BasarRuntimeException( sInfo )
{
}

//--------------------------------------------------------------------------------------------------//
ExBGABranchRepo::~ExBGABranchRepo(void)
{
}

} //namespace exceptions
} //namespace narcotics
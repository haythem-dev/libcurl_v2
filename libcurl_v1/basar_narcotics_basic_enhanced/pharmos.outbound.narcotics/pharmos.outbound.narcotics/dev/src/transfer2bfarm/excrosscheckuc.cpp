//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "excrosscheckuc.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
ExCrossCheckUC::ExCrossCheckUC(const basar::ExceptInfo& sInfo): basar::cmnutil::BasarRuntimeException( sInfo )
{
}

//--------------------------------------------------------------------------------------------------//
ExCrossCheckUC::~ExCrossCheckUC(void)
{
}

} //namespace exceptions
} //namespace narcotics
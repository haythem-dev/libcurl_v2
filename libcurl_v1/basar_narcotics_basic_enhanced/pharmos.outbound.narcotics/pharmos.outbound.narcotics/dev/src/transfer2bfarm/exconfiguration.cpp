//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "exconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
ExConfiguration::ExConfiguration(const basar::ExceptInfo& sInfo): basar::cmnutil::BasarRuntimeException( sInfo )
{
}

//--------------------------------------------------------------------------------------------------//
ExConfiguration::~ExConfiguration(void)
{
}

} //namespace exceptions
} //namespace narcotics
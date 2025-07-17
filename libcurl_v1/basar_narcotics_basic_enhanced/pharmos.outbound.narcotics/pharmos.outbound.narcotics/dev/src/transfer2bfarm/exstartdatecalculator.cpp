//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "exstartdatecalculator.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
ExStartDateCalculator::ExStartDateCalculator(const basar::ExceptInfo& sInfo): basar::cmnutil::BasarRuntimeException( sInfo )
{
}

//--------------------------------------------------------------------------------------------------//
ExStartDateCalculator::~ExStartDateCalculator(void)
{
}

} //namespace exceptions
} //namespace narcotics